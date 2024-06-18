C_SOURCES = $(wildcard *.c libc/*.c kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o}

TARGET=i686-elf
CC=$(TARGET)-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-T linker.ld -nostdlib -ffreestanding -O2 -lgcc

ifndef VERBOSE
.SILENT:
else
.SILENT: confirm-multiboot
endif

all: myos.iso

run: myos.iso
	qemu-system-i386 -cdrom $<

myos.iso: isodir/boot/kernel.bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

isodir/boot/kernel.bin: kernel/kernel.bin
	@mkdir -p isodir/boot
	cp $< $@

isodir/boot/grub/grub.cfg: grub/grub.cfg
	@mkdir -p isodir/boot/grub
	cp $< $@

kernel/kernel.bin: sysroot/usr/lib/libc.a
	$(MAKE) -C ./kernel install
	$(MAKE) -C ./kernel

sysroot/usr/lib/libc.a:
	$(MAKE) -C ./libc install

clean: clean-iso
	rm -rf *.img *.iso

clean-iso:
	rm -rf isodir/boot

confirm-multiboot:
	cat ./validate-multiboot.sh | sh
