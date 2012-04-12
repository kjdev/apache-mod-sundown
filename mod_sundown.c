/*
**  mod_sundown.c -- Apache sundown module
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory
**  by running:
**
**    $ apxs -c -i mod_sundown.c
**
**  Then activate it in Apache's httpd.conf file for instance
**  for the URL /sundown in as follows:
**
**    # httpd.conf
**    LoadModule sundown_module modules/mod_sundown.so
**    AddHandler sundown .md
**    SundownLayoutPath      [path]
**    SundownLayoutDefault   layout
**    SundownLayoutExtension .html
**
**  Then after restarting Apache via
**
**    $ apachectl restart
*/

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

#include "util_script.h"

#include "apr_fnmatch.h"
#include "apr_strings.h"
#include "apr_hash.h"

#include "http_log.h"
#define _ERROR(rec, format, args...)                                  \
    ap_log_rerror(APLOG_MARK, APLOG_NOTICE, 0,                        \
                  rec, "%s(%d) "format, __FILE__, __LINE__, ##args);
#define _DEBUG(rec, format, args...)                                   \
    ap_log_rerror(APLOG_MARK, APLOG_NOTICE, 0,                         \
                  rec, "%s(%d) "format, __FILE__, __LINE__, ##args);


#include "sundown/markdown.h"
#include "sundown/html.h"
#include "sundown/buffer.h"

#define READ_UNIT   1024
#define OUTPUT_UNIT 64

#define SUNDOWN_CONTENT_TYPE "text/html";
#define SUNDOWN_TAG "<body*>"
#define SUNDOWN_LAYOUT_DEFAULT "layout"
#define SUNDOWN_LAYOUT_EXT ".html"

#define SUNDOWN_RAW_SUPPORT 1

typedef struct {
    char *layout_path;
    char *layout_default;
    char *layout_ext;
} sundown_config_rec;

module AP_MODULE_DECLARE_DATA sundown_module;


static char* get_param(apr_hash_t *hash, char *key, int n)
{
    apr_array_header_t *values;

    if (key == NULL) {
        return NULL;
    }

    values = apr_hash_get(hash, key, APR_HASH_KEY_STRING);
    if (values == NULL) {
        return NULL;
    }

    if (values->nelts < n) {
        return NULL;
    }

    char **elts = (char **)values->elts;

    return elts[n];
}

static apr_hash_t* parse_param_from_args(request_rec *r, char *args)
{
    const char *delim = "&";
    char *items, *last, *st;
    apr_hash_t *hash = NULL;
    apr_array_header_t *values;

    if (args == NULL) {
        return NULL;
    }

    /* ハッシュマップの作成 */
    hash = apr_hash_make(r->pool);

    /* delim('&')でトークンに分割 */
    for (items = apr_strtok(args, delim, &last);
         items != NULL;
         items = apr_strtok(NULL, delim, &last)) {
        /* space 変換 */
        for (st = items; *st; ++st) {
            if (*st == '+') {
                *st = ' ';
            }
        }

        /* key と value に分割 */
        st = strchr(items, '=');
        if (st) {
            *st++ = '\0';
            ap_unescape_url(items);
            ap_unescape_url(st);
        } else {
            st = "";
            ap_unescape_url(items);
        }

        /* ハッシュの確認 */
        values = apr_hash_get(hash, items, APR_HASH_KEY_STRING);
        if (values == NULL) {
            /* apr_array_header_t の初期化 */
            values = apr_array_make(r->pool, 1, sizeof(char*));
            /* ハッシュに要素を追加 */
            apr_hash_set(hash, items, APR_HASH_KEY_STRING, values);
        }

        /* 配列に要素を追加 */
        *((char **)apr_array_push(values)) = apr_pstrdup(r->pool, st);
    }

    return hash;
}

static int output_layout_header(apr_file_t *fp, request_rec *r) {
    char buf[HUGE_STRING_LEN];
    char *lower = NULL;

    while (apr_file_gets(buf, HUGE_STRING_LEN, fp) == APR_SUCCESS) {
        ap_rputs(buf, r);

        /* 挿入ラインを検索 */
        lower = apr_pstrdup(r->pool, buf);
        ap_str_tolower(lower);
        if (apr_fnmatch("*"SUNDOWN_TAG"*", lower, APR_FNM_CASE_BLIND) == 0) {
            return 1;
        }
    }

    return 0;
}

static int output_layout_footer(apr_file_t *fp, request_rec *r) {
    char buf[HUGE_STRING_LEN];

    if (fp == NULL) {
        return 0;
    }

    while (apr_file_gets(buf, HUGE_STRING_LEN, fp) == APR_SUCCESS) {
        ap_rputs(buf, r);
    }
    apr_file_close(fp);

    return 0;
}


/* The sundown content handler */
static int sundown_handler(request_rec *r)
{
    sundown_config_rec *cfg;

    apr_status_t rc;
    apr_file_t *fp = NULL;
    apr_size_t bytes_read;
    char *layout_file = NULL;
    char *layout_filepath = NULL;
    char buf[HUGE_STRING_LEN];

    /* sundown: markdown */
    struct buf *ib, *ob;
    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;
    unsigned int markdown_extensions = 0;

    if (strcmp(r->handler, "sundown")) {
        return DECLINED;
    }

    if (r->header_only) {
        return OK;
    }

    /* open request file */
    rc = apr_file_open(
        &fp, r->filename,
        APR_READ | APR_BINARY | APR_XTHREAD, APR_OS_DEFAULT, r->pool);
    if (rc != APR_SUCCESS) {
        switch (errno) {
            case ENOENT:
                return HTTP_NOT_FOUND;
            case EACCES:
                return HTTP_FORBIDDEN;
            default:
                return HTTP_INTERNAL_SERVER_ERROR;
        }
    }

    /* set contest type */
    r->content_type = SUNDOWN_CONTENT_TYPE;

    /* get parameter */
    if (r->args) {
        apr_hash_t *params = parse_param_from_args(r, r->args);
#ifdef SUNDOWN_RAW_SUPPORT
        if (get_param(params, "raw", 0) != NULL) {
            while (apr_file_gets(buf, HUGE_STRING_LEN, fp) == APR_SUCCESS) {
                ap_rputs(buf, r);
            }
            apr_file_close(fp);
            return OK;
        }
#endif
        layout_file = get_param(params, "layout", 0);
    }

    /* reading everything */
    ib = bufnew(READ_UNIT);
    bufgrow(ib, READ_UNIT);

    do {
        rc = apr_file_read_full(
            fp, ib->data + ib->size, ib->asize - ib->size, &bytes_read);
        if (bytes_read > 0) {
            ib->size += bytes_read;
            bufgrow(ib, ib->size + READ_UNIT);
        }
    } while (rc != APR_EOF);

    apr_file_close(fp);
    fp = NULL;

    /* performing markdown parsing */
    ob = bufnew(OUTPUT_UNIT);

    sdhtml_renderer(&callbacks, &options, 0);

    /* TODO: configure settings */
    //MKDEXT_NO_INTRA_EMPHASIS
    //MKDEXT_AUTOLINK
    //MKDEXT_STRIKETHROUGH
    //MKDEXT_LAX_HTML_BLOCKS
    //MKDEXT_SPACE_HEADERS
    //MKDEXT_SUPERSCRIPT

    //MKDEXT_TABLES => Enabled
    //MKDEXT_FENCED_CODE => Enabled
    markdown_extensions = MKDEXT_TABLES | MKDEXT_FENCED_CODE;

    markdown = sd_markdown_new(markdown_extensions, 16, &callbacks, &options);

    sd_markdown_render(ob, ib->data, ib->size, markdown);
    sd_markdown_free(markdown);

    /* config */
    cfg = ap_get_module_config(r->per_dir_config, &sundown_module);

    //layout をパラメターから受け取る
    if (layout_file == NULL) {
        layout_file = cfg->layout_default;
    }

    if (cfg->layout_path == NULL) {
        //DOCUMENT_ROOT パスを取得
        ap_add_common_vars(r);
        cfg->layout_path = (char *)apr_table_get(
            r->subprocess_env, "DOCUMENT_ROOT");
    }


    layout_filepath = apr_psprintf(
        r->pool, "%s/%s%s", cfg->layout_path, layout_file, cfg->layout_ext);

    //_DEBUG(r, "layout_file:%s", layout_filepath);

    /* output layout header part */
    rc = apr_file_open(
        &fp, layout_filepath,
        APR_READ | APR_BINARY | APR_XTHREAD, APR_OS_DEFAULT, r->pool);
    if (rc == APR_SUCCESS) {
        if (output_layout_header(fp, r) != 1) {
            apr_file_close(fp);
            fp = NULL;
        }
    } else {
        layout_filepath = apr_psprintf(
            r->pool, "%s/%s%s",
            cfg->layout_path, cfg->layout_default, cfg->layout_ext);

        _DEBUG(r, ">> layout_file:%s", layout_filepath);

        rc = apr_file_open(
            &fp, layout_filepath,
            APR_READ | APR_BINARY | APR_XTHREAD, APR_OS_DEFAULT, r->pool);
        if (rc == APR_SUCCESS) {
            if (output_layout_header(fp, r) != 1) {
                apr_file_close(fp);
                fp = NULL;
            }
        }
    }

    /* writing the result */
    ap_rwrite(ob->data, ob->size, r);

    /* cleanup */
    bufrelease(ib);
    bufrelease(ob);

    /* output layout footer part */
    output_layout_footer(fp, r);

    return OK;
}

static void* sundown_create_dir_config(apr_pool_t *p, char *dir)
{
    sundown_config_rec *cfg;

    cfg = apr_pcalloc(p, sizeof(sundown_config_rec));

    memset(cfg, 0, sizeof(sundown_config_rec));

    cfg->layout_path = NULL;
    cfg->layout_default = SUNDOWN_LAYOUT_DEFAULT;
    cfg->layout_ext = SUNDOWN_LAYOUT_EXT;

    return (void *)cfg;
}

static const command_rec sundown_cmds[] = {
    AP_INIT_TAKE1(
        "SundownLayoutPath", ap_set_string_slot,
        (void *)APR_OFFSETOF(sundown_config_rec, layout_path), OR_ALL,
        "sundown layout path"),
    AP_INIT_TAKE1(
        "SundownLayoutDefault", ap_set_string_slot,
        (void *)APR_OFFSETOF(sundown_config_rec, layout_default), OR_ALL,
        "sundown default layout file"),
    AP_INIT_TAKE1(
        "SundownLayoutExtension", ap_set_string_slot,
        (void *)APR_OFFSETOF(sundown_config_rec, layout_ext), OR_ALL,
        "sundown default layout file extension"),
    {NULL}
};

static void sundown_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(sundown_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA sundown_module = {
    STANDARD20_MODULE_STUFF,
    sundown_create_dir_config, /* create per-dir    config structures */
    NULL,                      /* merge  per-dir    config structures */
    NULL,                      /* create per-server config structures */
    NULL,                      /* merge  per-server config structures */
    sundown_cmds,              /* table of config file commands       */
    sundown_register_hooks     /* register hooks                      */
};
