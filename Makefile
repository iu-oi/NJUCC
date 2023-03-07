VERSION = dbg
CC = gcc
FLEX = flex
BISON = bison
LFLAGS = -lfl -ly
TARGET = njucc

ifeq ($(VERSION), dbg)
CFLAGS = -g -std=gnu99 -Werror -O0 -I./include
else
CFLAGS = -std=gnu99 -Werror -O3 -I./include
endif

CFILES = $(shell find ./src -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./syntax -name "*.l")
YFILE = $(shell find ./syntax -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/'[^/]*\.l'/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/'[^/]*\.y'/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

njucc: syntax $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(YFO) $(LFLAGS)

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO) $(CFLAGS)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

.PHONY: clean
clean:
	rm -f $(LFO) $(YFO) $(OBJS) $(TARGET)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h) $(YFC:.tab.c=.output)
