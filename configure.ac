#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(mod_sundown, 0.2.0,[])

AC_CONFIG_SRCDIR([mod_sundown.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(mod_sundown, 0.2.0, [NO-DEFINE])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
# AC_PROG_CC
AC_PROG_LIBTOOL
AM_PROG_AR

# Option for sundown debug log level
AC_ARG_WITH(sundown-debug-log,
  AC_HELP_STRING([--with-sundown-debug-log=LEVEL],
    [sundown debug log level [default=no]]),
  [SUNDOWN_DEBUG_LOG="${withval}"],
  [SUNDOWN_DEBUG_LOG=no]
)
AS_IF([test "x${SUNDOWN_DEBUG_LOG}" != xno],
    [AC_DEFINE_UNQUOTED([AP_SUNDOWN_DEBUG_LOG_LEVEL],
      [${SUNDOWN_DEBUG_LOG}], [sundown debug log level])]
)

# Option for sundown extension
AC_ARG_ENABLE(sundown-no_intra_emphasis,
  AC_HELP_STRING([--enable-sundown-no-intra-emphasis],
    [enable sundown extenssion no intra emphasis [[default=no]]]),
  [ENABLED_SUNDOWN_NO_INTRA_EMPHASIS="${enableval:-yes}"],
  [ENABLED_SUNDOWN_NO_INTRA_EMPHASIS=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_NO_INTRA_EMPHASIS}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_NO_INTRA_EMPHASIS], [1], [enable no intra emphasis])]
)

AC_ARG_ENABLE(sundown-autolink,
  AC_HELP_STRING([--enable-sundown-autolink],
    [enable sundown extenssion autolink [default=no]]),
  [ENABLED_SUNDOWN_AUTOLINK="${enableval:-yes}"],
  [ENABLED_SUNDOWN_AUTOLINK=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_AUTOLINK}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_AUTOLINK], [1], [enable autolink])]
)

AC_ARG_ENABLE(sundown-strikethrough,
  AC_HELP_STRING([--enable-sundown-strikethrough],
    [enable sundown extenssion strikethrough [default=no]]),
  [ENABLED_SUNDOWN_STRIKETHROUGH="${enableval:-yes}"],
  [ENABLED_SUNDOWN_STRIKETHROUGH=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_STRIKETHROUGH}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_STRIKETHROUGH], [1], [enable strikethrough])]
)

AC_ARG_ENABLE(sundown-lax-html-blocks,
  AC_HELP_STRING([--enable-sundown-lax-html-blocks],
    [enable sundown extenssion lax html blocks [default=no]]),
  [ENABLED_SUNDOWN_LAX_HTML_BLOCKS="${enableval:-yes}"],
  [ENABLED_SUNDOWN_LAX_HTML_BLOCKS=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_LAX_HTML_BLOCKS}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_LAX_HTML_BLOCKS], [1], [enable lax html blocks])]
)

AC_ARG_ENABLE(sundown-space-headers,
  AC_HELP_STRING([--enable-sundown-space-headers],
    [enable sundown extenssion space headers [default=no]]),
  [ENABLED_SUNDOWN_SPACE_HEADERS="${enableval:-yes}"],
  [ENABLED_SUNDOWN_SPACE_HEADERS=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_SPACE_HEADERS}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_SPACE_HEADERS], [1], [enable space headers])]
)

AC_ARG_ENABLE(sundown-superscript,
  AC_HELP_STRING([--enable-sundown-superscript],
    [enable sundown extenssion superscript [default=no]]),
  [ENABLED_SUNDOWN_SUPERSCRIPT="${enableval:-yes}"],
  [ENABLED_SUNDOWN_SUPERSCRIPT=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_SUPERSCRIPT}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_SUPERSCRIPT], [1], [enable superscript])]
)

AC_ARG_ENABLE(sundown-tables,
  AC_HELP_STRING([--enable-sundown-tables],
    [enable sundown extenssion tables [default=no]]),
  [ENABLED_SUNDOWN_TABLES="${enableval:-yes}"],
  [ENABLED_SUNDOWN_TABLES=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_TABLES}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_TABLES], [1], [enable tables])]
)

AC_ARG_ENABLE(sundown-fenced-code,
  AC_HELP_STRING([--enable-sundown-fenced-code],
    [enable sundown extenssion fenced code [default=no]]),
  [ENABLED_SUNDOWN_FENCED_CODE="${enableval:-yes}"],
  [ENABLED_SUNDOWN_FENCED_CODE=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_FENCED_CODE}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_FENCED_CODE], [1], [enable fenced code])]
)

AC_ARG_ENABLE(sundown-skip-linebreak,
  AC_HELP_STRING([--enable-sundown-skip-linebreak],
    [enable sundown extenssion skip linebreak [default=no]]),
  [ENABLED_SUNDOWN_SKIP_LINEBREAK="${enableval:-yes}"],
  [ENABLED_SUNDOWN_SKIP_LINEBREAK=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_SKIP_LINEBREAK}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_SKIP_LINEBREAK], [1], [enable skip linebreak])]
)

AC_ARG_ENABLE(sundown-special-attributes,
  AC_HELP_STRING([--enable-sundown-special-attributes],
    [enable sundown extenssion special attributes [default=no]]),
  [ENABLED_SUNDOWN_SPECIAL_ATTRIBUTES="${enableval:-yes}"],
  [ENABLED_SUNDOWN_SPECIAL_ATTRIBUTES=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_SPECIAL_ATTRIBUTES}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_SPECIAL_ATTRIBUTES], [1], [enable special attributes])]
)

AC_ARG_ENABLE(sundown-xhtml,
  AC_HELP_STRING([--enable-sundown-xhtml],
    [enable sundown extenssion xhtml [default=no]]),
  [ENABLED_SUNDOWN_XHTML="${enableval:-yes}"],
  [ENABLED_SUNDOWN_XHTML=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_XHTML}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_XHTML], [1], [enable xhtml])]
)

AC_ARG_ENABLE(sundown-toc,
  AC_HELP_STRING([--enable-sundown-toc],
    [enable sundown extenssion toc [default=no]]),
  [ENABLED_SUNDOWN_TOC="${enableval:-yes}"],
  [ENABLED_SUNDOWN_TOC=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_TOC}" = xyes],
    [AC_DEFINE([SUNDOWN_USE_TOC], [1], [enable xhtml])]
)

# Option for sundown raw support
AC_ARG_ENABLE(sundown-raw-support,
  AC_HELP_STRING([--enable-sundown-raw-support],
    [enable sundown raw support [default=no]]),
  [ENABLED_SUNDOWN_RAW_SUPPORT="${enableval:-yes}"],
  [ENABLED_SUNDOWN_RAW_SUPPORT=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_RAW_SUPPORT}" = xyes],
    [AC_DEFINE([SUNDOWN_RAW_SUPPORT], [1], [enable raw support])]
)

# Option for sundown toc support
AC_ARG_ENABLE(sundown-toc-support,
  AC_HELP_STRING([--enable-sundown-toc-support],
    [enable sundown toc support [default=no]]),
  [ENABLED_SUNDOWN_TOC_SUPPORT="${enableval:-yes}"],
  [ENABLED_SUNDOWN_TOC_SUPPORT=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_TOC_SUPPORT}" = xyes],
    [AC_DEFINE([SUNDOWN_TOC_SUPPORT], [1], [enable toc support])]
)

# Checks for apxs.
AC_ARG_WITH(apxs,
  [AC_HELP_STRING([--with-apxs=PATH], [apxs path [default=yes]])],
  [APXS_PATH="$withval"],
  [APXS_PATH="/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin"]
)

AC_PATH_PROG(APXS, apxs, no, ${APXS_PATH})

AS_IF([test "x${APXS}" = "xno"], AC_MSG_ERROR([apxs not found]),
  [test "x${APXS}" = "x"], AC_MSG_ERROR([apxs not found])
)

APXS_CFLAGS=`${APXS} -q CFLAGS 2> /dev/null`
APXS_CPPFLAGS=`${APXS} -q CPPFLAGS 2> /dev/null`
APXS_LDFLAGS=`${APXS} -q LDFLAGS 2> /dev/null`
APXS_LIBS=`${APXS} -q LIBS 2> /dev/null`
APXS_LIBEXECDIR=`${APXS} -q LIBEXECDIR 2> /dev/null`

APXS_INCLUDEDIR=`${APXS} -q INCLUDEDIR 2> /dev/null`
APXS_INCLUDES="-I${APXS_INCLUDEDIR}"

# Checks for apr.
AC_ARG_WITH(apr,
  [AC_HELP_STRING([--with-apr=PATH], [apr config path [default=yes]])],
  [APR_CONFIG="$withval"],
  [with_apr=yes]
)

AC_MSG_CHECKING([whether apr])
AS_IF([test "x$with_apr" != "xno"],
  [
   if test ! -x "${APR_CONFIG}" -o -z "${APR_CONFIG}"; then
     APR_PATH=`${APXS} -q APR_BINDIR 2> /dev/null`
     APR_CONFIG="${APR_PATH}/apr-1-config"
     if test ! -x "${APR_CONFIG}"; then
       APR_CONFIG="${APR_PATH}/apr-config"
     fi
   fi
   APR_INCLUDES=`${APR_CONFIG} --includes 2> /dev/null`
   APR_CFLAGS=`${APR_CONFIG} --cflags 2> /dev/null`
   APR_CPPFLAGS=`${APR_CONFIG} --cppflags 2> /dev/null`
   APR_LDFLAGS=`${APR_CONFIG} --ldflags 2> /dev/null`
   APR_LIBS=`${APR_CONFIG} --libs 2> /dev/null`
   AC_MSG_RESULT(yes)
  ],
  AC_MSG_ERROR(apr not found)
)

# Checks for apreq2.
AC_ARG_WITH(apreq2,
  [AC_HELP_STRING([--with-apreq2=PATH], [apreq2 config path [default=yes]])],
  [APREQ2_CONFIG="$withval"],
  [with_apreq2=yes]
)

AC_MSG_CHECKING([whether apreq2])
AS_IF([test "x$with_apreq2" != "xno"],
  [
   if test ! -x "${APREQ2_CONFIG}" -o -z "${APREQ2_CONFIG}"; then
     SEARCH_PATH="/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin"
     AC_PATH_PROG(APREQ2_CONFIG, apreq2-config, no, ${SEARCH_PATH})
     AS_IF(
       [test "x${APREQ2_CONFIG}" = "xno"], AC_MSG_ERROR([apreq2-config not found]),
       [test "x${APREQ2_CONFIG}" = "x"], AC_MSG_ERROR([apreq2-config not found])
     )
   fi
   APREQ2_INCLUDES=`${APREQ2_CONFIG} --includes 2> /dev/null`
   APREQ2_LDFLAGS=`${APREQ2_CONFIG} --ldflags 2> /dev/null`
   APREQ2_LIBS=`${APREQ2_CONFIG} --libs 2> /dev/null`
   AC_MSG_RESULT(yes)
  ],
  AC_MSG_ERROR(apreq2 not found)
)

# Apache libraries.
APACHE_MODULEDIR="${APXS_LIBEXECDIR}"
APACHE_INCLUDES="${APXS_INCLUDES} ${APR_INCLUDES} ${APREQ2_INCLUDES}"
APACHE_CFLAGS="${APXS_CFLAGS} ${APR_CFLAGS}"
APACHE_CPPFLAGS="${APXS_CPPFLAGS} ${APR_CPPFLAGS}"
APACHE_LDFLAGS="${APXS_LDFLAGS} ${APR_LDFLAGS} ${APREQ2_LDFLAGS}"
APACHE_LIBS="${APXS_LIBS} ${APR_LIBS} ${APREQ2_LIBS}"

AC_SUBST(APACHE_MODULEDIR)
AC_SUBST(APACHE_INCLUDES)
AC_SUBST(APACHE_CFLAGS)
AC_SUBST(APACHE_CPPFLAGS)
AC_SUBST(APACHE_LDFLAGS)
AC_SUBST(APACHE_LIBS)


# Checks for curl.
AC_ARG_WITH(curl,
  [AC_HELP_STRING([--with-curl=PATH], [curl path [default=/usr/include]])],
  [CURL_PATH="$withval"],
  [CURL_PATH="/usr/include"]
)

AC_CHECK_FILE(
  "${CURL_PATH}/curl/curl.h",
  [CURL_INCLUDEDIR=${CURL_PATH}],
  [AC_MSG_ERROR([Missing required curl header.])]
)

AC_ARG_WITH(curl-lib,
  [AC_HELP_STRING([--with-curl-lib=PATH], [curl library path [default=no]])],
  [CURL_LIBPATH="$withval"],
  [CURL_LIBPATH="no"]
)

AS_IF([test "x${CURL_LIBPATH}" != "xno"],
  CURL_LDFLAGS="-L${CURL_LIBPATH}"
)

AC_MSG_CHECKING([for curl library])
SAVED_CFLAGS=$CFLAGS
SAVED_LDFLAGS=$LDFLAGS
CFLAGS="$CFLAGS -I ${CURL_INCLUDEDIR}"
LDFLAGS="$LDFLAGS ${CURL_LDFLAGS} -lcurl"
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
    [#include "curl/curl.h"],
    [CURL *curl])],
  [CURL_LIBS="-lcurl"],
  [AC_MSG_ERROR([Missing required curl library.])]
)
AC_MSG_RESULT(yes)
CFLAGS=$SAVED_CFLAGS
LDFLAGS=$SAVED_LDFLAGS

# curl libraries.
CURL_CFLAGS="-I ${CURL_INCLUDES}"
CURL_CPPFLAGS="${CURL_CFLAGS}"
CURL_LDFLAGS="${CURL_LDFLAGS} ${CURL_LIBS}"

AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_CPPFLAGS)
AC_SUBST(CURL_LDFLAGS)
AC_SUBST(CURLLIBS)


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
