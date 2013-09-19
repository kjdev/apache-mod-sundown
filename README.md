# mod_sundown #

mod_sundown is Markdown handler module for Apache HTTPD Server.

## Dependencies ##

* [sundown](https://github.com/kjdev/sundown)

## Build ##

    % ./autogen.sh (or autoreconf -i)
    % ./configure [OPTION]
    % make
    % make install

### Build Options ###

sundown library extensions.

* --enable-sundown-no-intra-emphasis
* --enable-sundown-autolink
* --enable-sundown-strikethrough
* --enable-sundown-lax-html-blocks
* --enable-sundown-space-headers
* --enable-sundown-superscript
* --enable-sundown-tables
* --enable-sundown-fenced-code
* --enable-sundown-special-attributes

* --enable-sundown-skip-linebreak
* --enable-sundown-xhtml
* --enable-sundown-toc
* --enable-sundown-task-list

markdown raw print.

* --enable-sundown-raw-support

    http://localhot/index.md?raw

markdown toc support.

* --enable-sundown-toc-support

    http://localhot/index.md?toc
    http://localhot/index.md?toc=3
    http://localhot/index.md?toc=:3
    http://localhot/index.md?toc=2:4

apache path.

* --with-apxs=PATH
* --with-apr=PATH
* --with-apreq2=PATH

## Configration ##

httpd.conf:

    LoadModule sundown_module modules/mod_sundown.so
    <Location /markdown>
        SetHandler sundown
    </Location>

## Style ##

/var/www/style/default.html:

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
    <Location /markdown>
        SetHandler            sundown
        SundownStylePath      /var/www/style
        SundownStyleDefault   default
        SundownStyleExtension .html
    </Location>

This will expand the markdown file next to the line
with the "<body>" of style.html.

### Multiple Style ##

* /var/www/style/style.html
* /var/www/style/style-2.html

You can change the layout file by specifying the layout parameters.

    http://localhot/markdown/readme.md?style=style
    http://localhot/markdown/readme.md?style=style-2

## URL ##

You can also get the file from an external source
by specifying the Markdown file to URL parameter.

    http://localhot/markdown?url=https://raw.github.com/kjdev/apache-mod-sundown/master/README.md

## Markdown ##

You can also send a markdown Markdown content parameter. (Send to POST)

    POST http://localhot/markdown

form:

    <form action="/markdown" method="post">
        <textarea name="markdown"></textarea>
        <input type="submit" />
    </form>

## Order ##

Load the content in order.

1. A local file
2. Markdown Parameters
3. URL parameters

Output together.
