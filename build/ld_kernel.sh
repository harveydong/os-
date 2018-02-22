#!/bin/sh
ld -Ttext 0xc0001500 -e main -m elf_i386 -o kernel.bin main.o init.o int.o low_int.o timer.o  memory.o bitmap.o string.o  debug.o print.o
cp kernel.bin ../../
