#! /bin/bash
nasm -felf32 test2.asm -o test2.o
ld -m elf_i386 test2.o -o test2 /usr/lib/asmlib.a
