CC ?= cc
CFLAGS ?= -O2 -pipe -std=c11 -Wall -Wextra
PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
LIBS ?= $(PKG_LIBS) -lGL -ldl -lm -lpthread

all: luna-fm

luna-fm: luna-fm.c luna-ui.h stb_truetype.h stb_image.h stb_image_write.h cssparser.h
	$(CC) $(CFLAGS) $(PKG_CFLAGS) -o $@ luna-fm.c $(LIBS)

clean:
	rm -f luna-fm

.PHONY: all clean
