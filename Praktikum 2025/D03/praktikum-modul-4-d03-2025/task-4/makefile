prepare: 
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader: 
	nasm -f bin src/bootloader.asm -o bin/bootloader.bin

stdlib: 
	bcc -ansi -Iinclude -c src/std_lib.c -o bin/std_lib.o


kernel:
	bcc -ansi -Iinclude -c src/kernel.c -o bin/kernel.o
	nasm -f as86 src/kernel.asm -o bin/kernel-asm.o

link: 
	ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel-asm.o bin/std_lib.o

build: prepare bootloader stdlib kernel link
	dd if=bin/bootloader.bin of=bin/floppy.img bs=512 count=1 conv=notrunc
	dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 count=15 conv=notrunc

run: 
	bochs -f bochsrc.txt