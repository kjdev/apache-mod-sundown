
ACLOCAL_AMFLAGS = -I m4

moddir = @APACHE_MODULEDIR@
mod_LTLIBRARIES = mod_sundown.la

mod_sundown_la_SOURCES = \
   mod_sundown.c \
   sundown/autolink.c \
   sundown/houdini_html_e.c \
   sundown/markdown.c \
   sundown/buffer.c \
   sundown/stack.c \
   sundown/houdini_href_e.c \
   sundown/html.c

mod_sundown_la_CFLAGS = @APACHE_CFLAGS@ @APACHE_INCLUDES@ @CURL_INCLUDES@
mod_sundown_la_CPPFLAGS = @APACHE_CPPFLAGS@ @APACHE_INCLUDES@ @CURL_INCLUDES@
mod_sundown_la_LDFLAGS = -avoid-version -module @APACHE_LDFLAGS@ @CURL_LDFLAGS@
mod_sundown_la_LIBS = @APACHE_LIBS@ @CURL_LIBS@
