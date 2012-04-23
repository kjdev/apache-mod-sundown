# mod_sundown #

mod_sundown is Markdown handler module for Apache HTTPD Server.

## Dependencies ##

* [sundown](https://github.com/tanoku/sundown.git)

## Build ##

    % ./autogen.sh (or autoreconf -i)
    % ./configure [OPTION]
    % make
    % make install

### Build Options ###

sundown library extensions (MKDEXT_).

* --enable-sundown-no-intra-emphasis
* --enable-sundown-autolink
* --enable-sundown-strikethrough
* --enable-sundown-lax-html-blocks
* --enable-sundown-space-headers
* --enable-sundown-superscript
* --enable-sundown-tables
* --enable-sundown-fenced-code

markdown raw print.

* --enable-sundown-raw-support

    http://localhot/index.md?raw

apache path.

* --with-apxs=PATH
* --with-apr=PATH

## Configration ##

httpd.conf:

    LoadModule sundown_module modules/mod_sundown.so
    <Location /sundown>
        AddHandler sundown .md
    </Location>

## Layout ##

/var/www/layout/layout.html:

    <!DOCTYPE html>
    <html>
    <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title>Markdown Layout</title>
    </head>
    <body>
    </body>
    </html>

httpd.conf:

    LoadModule sundown_module modules/mod_sundown.so
    <Location /sundown>
        AddHandler sundown .md
        SundownLayoutPath      /var/www/layout
        SundownLayoutDefault   layout
        SundownLayoutExtension .html
    </Location>

This will expand the markdown file next to the line with the "<body>" of
layout.html.

## Multiple ##

* /var/www/layout/layout.html
* /var/www/layout/layout-2.html

You can change the layout file by specifying the layout parameters.

    http://localhot/readme.md?layout=layout
    http://localhot/readme.md?layout=layout-2
