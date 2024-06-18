SYSROOT=../sysroot
TARGET=i686-elf
CC=$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra --sysroot=$(SYSROOT) -I$(SYSROOT)/usr/include
LDFLAGS=-T ../linker.ld -nostdlib -L$(SYSROOT)/usr/lib
ASS=$(TARGET)-as

LIBC = $(SYSROOT)/usr/lib/libc.a
OBJ_DIR = obj

C_SOURCES = $(wildcard *.c drivers/*.c)
HEADERS = $(wildcard include/kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: kernel.bin

ifndef VERBOSE
.SILENT:
endif

kernel.bin: boot/boot.o kernel.o ${OBJ} $(LIBC)
	$(CC) $(LDFLAGS) -o $@ $^
	grub-file --is-x86-multiboot $@

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) -c $< -o $@

%o: %.S
	$(ASS) $< -o $@

$(LIBC):
	$(MAKE) -C ../libc install

install: install-headers

install-headers:
	mkdir -p $(SYSROOT)/usr/include
	cp -R --preserve=timestamps include/. $(SYSROOT)/usr/include/.

clean:
	rm -rf *.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o

confirm-multiboot:
	cat ../validate-multiboot.sh | sh
