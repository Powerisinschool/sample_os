TARGET=i686-elf
CC=$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
AR=$(TARGET)-ar

SYSROOT=../sysroot
INCLUDE_DIR=-Iinclude
SRC_DIR=src
OBJ_DIR=obj

SRC = $(wildcard $(SRC_DIR)/*.c)
# OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

FREEOBJS=\
stdio/clear_screen.o \
stdio/print.o \
stdio/putchar.o \
stdlib/abort.o \
stdlib/atoi.o \
stdlib/free.o \
stdlib/itoa.o \
stdlib/malloc.o \
string/memcpy.o \
string/memset.o \
string/strcmp.o \
string/strcpy.o \
string/strlen.o \
# $(ARCH_FREEOBJS) \


OBJS = $(FREEOBJS:.o=.libc.o)

.SUFFIXES: .o .libc.o .c

all: $(SYSROOT)/usr/lib/libc.a

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(OBJ_DIR)
# 	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

.c.libc.o:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(SYSROOT)/usr/lib/libc.a: $(OBJS)
	@mkdir -p $(SYSROOT)/usr/lib
	$(AR) rcs $@ $^

install: install-headers $(SYSROOT)/usr/lib/libc.a

install-headers:
	mkdir -p $(SYSROOT)/usr/include
	cp -R --preserve=timestamps include/. $(SYSROOT)/usr/include/.

# install-libs: $(BINARIES)
# 	mkdir -p $(SYSROOT)/usr/lib
# 	cp libc.a $(DESTDIR)$(LIBDIR)

# install: $(SYSROOT)/usr/include/stdlib.h $(SYSROOT)/usr/include/string.h $(SYSROOT)/usr/include/io.h

# $(SYSROOT)/usr/include/stdlib.h: include/stdlib.h
# 	@mkdir -p $(SYSROOT)/usr/include
# 	cp $< $@

# $(SYSROOT)/usr/include/string.h: include/string.h
# 	@mkdir -p $(SYSROOT)/usr/include
# 	cp $< $@

# $(SYSROOT)/usr/include/io.h: include/io.h
# 	@mkdir -p $(SYSROOT)/usr/include
# 	cp $< $@

clean:
	rm -f $(OBJ_DIR) $(SYSROOT)/usr/lib/libc.a

.PHONY: all install clean
