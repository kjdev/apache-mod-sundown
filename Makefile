##
##  Makefile -- Apache sundown module
##

TARGET = mod_sundown.so
SRCS = \
	mod_sundown.c \
	sundown/autolink.c \
	sundown/houdini_html_e.c \
	sundown/markdown.c \
	sundown/buffer.c \
	sundown/stack.c \
	sundown/houdini_href_e.c \
	sundown/html.c
# sundown/html_smartypants.c
OBJS = $(SRCS:.c=.o)

## the used tools
APXS = apxs
APR = apr-1-config

APXS_INCLUDEDIR = `$(APXS) -q INCLUDEDIR`
APXS_CFLAGS = `$(APXS) -q CFLAGS`
APXS_LDFLAGS = `$(APXS) -q LDFLAGS`
APXS_LIBS = `$(APXS) -q LIBS`

APR_INCLUDEDIR = `$(APR) --includedir`
APR_CFLAGS = `$(APR) --cppflags`
APR_LDFLAGS = `$(APR) --ldflags`
APR_LIBS = `$(APR) --libs`

CC = gcc
INCLUDES = -I$(APXS_INCLUDEDIR) -I$(APR_INCLUDEDIR) -I.
CFLAGS = -Wall -fPIC -DAPR_POOL_DEBUG $(APXS_CFLAGS) $(APR_CFLAGS)
LDFLAGS = $(APXS_LDFLAGS) $(APR_LDFLAGS)
LIBS = $(APXS_LIBS) $(APR_LIBS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -shared -o $@ $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -c -o $@ $<

install: all
	$(APXS) -i -n 'sundown' mod_sundown.so

clean:
	find . -name \*.o | xargs rm -f
	-rm -f $(TARGET)
