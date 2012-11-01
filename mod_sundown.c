/*
**  mod_sundown.c -- Apache sundown module
**
**  Then activate it in Apache's httpd.conf file:
**
**    # httpd.conf
**    LoadModule sundown_module modules/mod_sundown.so
**    AddHandler sundown .md
**    SundownLayoutPath      /var/www/html
**    SundownLayoutDefault   layout
**    SundownLayoutExtension .html
*/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

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

    /* create hash */
    hash = apr_hash_make(r->pool);

    /* parse '&' */
    for (items = apr_strtok(args, delim, &last);
         items != NULL;
         items = apr_strtok(NULL, delim, &last)) {
        /* convert space */
        for (st = items; *st; ++st) {
            if (*st == '+') {
                *st = ' ';
            }
        }

        /* parse key, value */
        st = strchr(items, '=');
        if (st) {
            *st++ = '\0';
            ap_unescape_url(items);
            ap_unescape_url(st);
        } else {
            st = "";
            ap_unescape_url(items);
        }

        /* check hash */
        values = apr_hash_get(hash, items, APR_HASH_KEY_STRING);
        if (values == NULL) {
            /* init apr_array_header_t */
            values = apr_array_make(r->pool, 1, sizeof(char*));
            /* add hash */
            apr_hash_set(hash, items, APR_HASH_KEY_STRING, values);
        }

        /* add array */
        *((char **)apr_array_push(values)) = apr_pstrdup(r->pool, st);
    }

    return hash;
}

static int output_layout_header(request_rec *r, apr_file_t *fp) {
    char buf[HUGE_STRING_LEN];
    char *lower = NULL;

    while (apr_file_gets(buf, HUGE_STRING_LEN, fp) == APR_SUCCESS) {
        ap_rputs(buf, r);

        lower = apr_pstrdup(r->pool, buf);
        ap_str_tolower(lower);
        if (apr_fnmatch("*"SUNDOWN_TAG"*", lower, APR_FNM_CASE_BLIND) == 0) {
            return 1;
        }
    }

    return 0;
}

static apr_file_t* layout_header(request_rec *r, char *filename) {
    apr_status_t rc = -1;
    apr_file_t *fp = NULL;
    char *layout_filepath = NULL;
    sundown_config_rec *cfg;

    cfg = ap_get_module_config(r->per_dir_config, &sundown_module);

    if (filename == NULL && cfg->layout_default != NULL) {
        filename = cfg->layout_default;
    }

    if (filename != NULL) {
        if (cfg->layout_path == NULL) {
            ap_add_common_vars(r);
            cfg->layout_path = (char *)apr_table_get(
                r->subprocess_env, "DOCUMENT_ROOT");
        }

        layout_filepath = apr_psprintf(
            r->pool, "%s/%s%s", cfg->layout_path, filename, cfg->layout_ext);

        rc = apr_file_open(
            &fp, layout_filepath,
            APR_READ | APR_BINARY | APR_XTHREAD, APR_OS_DEFAULT, r->pool);
        if (rc == APR_SUCCESS) {
            if (output_layout_header(r, fp) != 1) {
                apr_file_close(fp);
                fp = NULL;
            }
        } else {
            layout_filepath = apr_psprintf(
                r->pool, "%s/%s%s",
                cfg->layout_path, cfg->layout_default, cfg->layout_ext);

            rc = apr_file_open(
                &fp, layout_filepath,
                APR_READ | APR_BINARY | APR_XTHREAD, APR_OS_DEFAULT, r->pool);
            if (rc == APR_SUCCESS) {
                if (output_layout_header(r, fp) != 1) {
                    apr_file_close(fp);
                    fp = NULL;
                }
            }
        }
    }

    if (rc != APR_SUCCESS) {
        ap_rputs("<!DOCTYPE html>\n", r);
        ap_rputs("<html>\n", r);
        ap_rputs("<head>\n", r);
        ap_rputs("<title>Markdown</title>\n", r);
        ap_rputs("</head>\n", r);
        ap_rputs("<body>\n", r);
    }

    return fp;
}

static int layout_footer(request_rec *r, apr_file_t *fp) {
    char buf[HUGE_STRING_LEN];

    if (fp != NULL) {
        while (apr_file_gets(buf, HUGE_STRING_LEN, fp) == APR_SUCCESS) {
            ap_rputs(buf, r);
        }
        apr_file_close(fp);
    } else {
        ap_rputs("</body>\n", r);
        ap_rputs("</html>\n", r);
    }

    return 0;
}


/* The sundown content handler */
static int sundown_handler(request_rec *r)
{
    apr_status_t rc;
    apr_file_t *fp = NULL;
    apr_size_t bytes_read;
    char *layout_file = NULL;

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
        char buf[HUGE_STRING_LEN];
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

    /* extensionss */
    markdown_extensions = 0;
#ifdef SUNDOWN_USE_FENCED_CODE
    markdown_extensions = markdown_extensions | MKDEXT_FENCED_CODE;
#endif
#ifdef SUNDOWN_USE_NO_INTRA_EMPHASIS
    markdown_extensions = markdown_extensions | MKDEXT_NO_INTRA_EMPHASIS;
#endif
#ifdef SUNDOWN_USE_AUTOLINK
    markdown_extensions = markdown_extensions | MKDEXT_AUTOLINK;
#endif
#ifdef SUNDOWN_USE_STRIKETHROUGH
    markdown_extensions = markdown_extensions | MKDEXT_STRIKETHROUGH;
#endif
#ifdef SUNDOWN_USE_LAX_HTML_BLOCKS
    markdown_extensions = markdown_extensions | MKDEXT_LAX_HTML_BLOCKS;
#endif
#ifdef SUNDOWN_USE_SPACE_HEADERS
    markdown_extensions = markdown_extensions | MKDEXT_SPACE_HEADERS;
#endif
#ifdef SUNDOWN_USE_SUPERSCRIPT
    markdown_extensions = markdown_extensions | MKDEXT_SUPERSCRIPT;
#endif
#ifdef SUNDOWN_USE_FENCED_CODE
    markdown_extensions = markdown_extensions | MKDEXT_FENCED_CODE;
#endif
#ifdef SUNDOWN_USE_TABLES
    markdown_extensions = markdown_extensions | MKDEXT_TABLES;
#endif

    markdown = sd_markdown_new(markdown_extensions, 16, &callbacks, &options);

    sd_markdown_render(ob, ib->data, ib->size, markdown);
    sd_markdown_free(markdown);

    /* output layout header */
    fp = layout_header(r, layout_file);

    /* writing the result */
    ap_rwrite(ob->data, ob->size, r);

    /* cleanup */
    bufrelease(ib);
    bufrelease(ob);

    /* output layout footer */
    layout_footer(r, fp);

    return OK;
}

static void* sundown_create_dir_config(apr_pool_t *p, char *dir)
{
    sundown_config_rec *cfg;

    cfg = apr_pcalloc(p, sizeof(sundown_config_rec));

    memset(cfg, 0, sizeof(sundown_config_rec));

    cfg->layout_path = NULL;
    cfg->layout_default = NULL;
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
