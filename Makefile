C_SOURCES = $(wildcard kernel/*.c drivers/*.c *.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o}

CFLAGS = -g

ifndef VERBOSE
.SILENT:
else
.SILENT: confirm-multiboot
endif

all: myos.iso

run: myos.iso
	qemu-system-i386 -cdrom $<

myos.iso: isodir/boot/myos.bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

isodir:
	mkdir -p isodir/boot/grub

isodir/boot/myos.bin: myos.bin
	cp $< $@

isodir/boot/grub/grub.cfg: grub/grub.cfg
	cp $< $@

myos.bin: boot/boot.o kernel/kernel.o ${OBJ}
	i686-elf-gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

%.o: %.c ${HEADERS}
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%o: %.s
	i686-elf-as $< -o $@

# Intentionally left out isodir so we can clean and easily
# recreate our iso without recompiling everything
clean:
	rm -rf *.bin *.img *.dis *.o *.elf *.iso
	rm -rf kernel/*.o boot/*.bin drivers/*.o

confirm-multiboot:
	cat ./validate-multiboot.sh | sh
