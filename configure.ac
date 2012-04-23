#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(mod_sundown, 0.0.1)

AC_CONFIG_SRCDIR([mod_sundown.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

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

# Option for sundown raw support
AC_ARG_ENABLE(sundown-raw-support,
  AC_HELP_STRING([--enable-sundown-raw-support],
    [enable sundown extenssion fenced code [default=no]]),
  [ENABLED_SUNDOWN_RAW_SUPPORT="${enableval:-yes}"],
  [ENABLED_SUNDOWN_RAW_SUPPORT=no]
)
AS_IF([test "x${ENABLED_SUNDOWN_RAW_SUPPORT}" = xyes],
    [AC_DEFINE([SUNDOWN_RAW_SUPPORT], [1], [enable raw support])]
)

# Checks for apxs
AC_ARG_WITH(apxs,
  [AC_HELP_STRING([--with-apxs=PATH], [apxs path [default=yes]])],
  [APXS_PATH="$withval"],
  [APXS_PATH="/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin"]
)

AC_PATH_PROG(APXS, apxs, no, ${APXS_PATH})

AS_IF([test "x${APXS}" = xno], AC_MSG_ERROR([apxs not found]),
  [test "x${APXS}" = x], AC_MSG_ERROR([apxs not found])
)

APXS_CFLAGS=`${APXS} -q CFLAGS 2> /dev/null`
APXS_CPPFLAGS=`${APXS} -q CPPFLAGS 2> /dev/null`
APXS_LDFLAGS=`${APXS} -q LDFLAGS 2> /dev/null`
APXS_LIBS=`${APXS} -q LIBS 2> /dev/null`
APXS_LIBEXECDIR=`${APXS} -q LIBEXECDIR 2> /dev/null`

APXS_INCLUDEDIR=`${APXS} -q INCLUDEDIR 2> /dev/null`
APXS_INCLUDES="-I${APXS_INCLUDEDIR}"

# Checks for apr
AC_ARG_WITH(apr,
  [AC_HELP_STRING([--with-apr=PATH], [apr config path [default=yes]])],
  [APR_CONFIG="$withval"],
  [with_apr=yes]
)

AC_MSG_CHECKING([whether apr])
AS_IF([test "x$with_apr" != xno],
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
   AC_MSG_RESULT(enable)
  ],
  AC_MSG_RESULT(disable)
)

# Checks for apu
AC_ARG_WITH(apu,
  [AC_HELP_STRING([--with-apu=PATH], [apu config path [default=no]])],
  [APU_CONFIG="$withval"],
  [with_apu=no]
)

AC_MSG_CHECKING([whether apu])
AS_IF([test "x$with_apu" != xno],
  [
   if test ! -x "${APU_CONFIG}" -o -z "${APU_CONFIG}"; then
     APU_PATH=`${APXS} -q APU_BINDIR 2> /dev/null`
     APU_CONFIG="${APU_PATH}/apu-1-config"
     if test ! -x "${APU_CONFIG}"; then
       APU_CONFIG="${APU_PATH}/apu-config"
     fi
   fi
   APU_INCLUDES=`${APU_CONFIG} --includes 2>/dev/null`
   APU_CFLAGS=`${APU_CONFIG} --cflags 2> /dev/null`
   APU_CPPFLAGS=`${APU_CONFIG} --cppflags 2> /dev/null`
   APU_LDFLAGS=`${APU_CONFIG} --ldflags 2> /dev/null`
   APU_LIBS=`${APU_CONFIG} --libs 2> /dev/null`
   AC_MSG_RESULT(enable)
  ],
  AC_MSG_RESULT(disable)
)

# Apache libraries
APACHE_MODULEDIR="${APXS_LIBEXECDIR}"
APACHE_INCLUDES="${APXS_INCLUDES} ${APR_INCLUDES} ${APU_INCLUDES}"
APACHE_CFLAGS="${APXS_CFLAGS} ${APR_CFLAGS} ${APU_CFLAGS}"
APACHE_CPPFLAGS="${APXS_CPPFLAGS} ${APR_CPPFLAGS} ${APU_CPPFLAGS}"
APACHE_LDFLAGS="${APXS_LDFLAGS} ${APR_LDFLAGS} ${APU_LDFLAGS}"
APACHE_LIBS="${APXS_LIBS} ${APR_LIBS} ${APU_LIBS}"

AC_SUBST(APACHE_MODULEDIR)
AC_SUBST(APACHE_INCLUDES)
AC_SUBST(APACHE_CFLAGS)
AC_SUBST(APACHE_CPPFLAGS)
AC_SUBST(APACHE_LDFLAGS)
AC_SUBST(APACHE_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
