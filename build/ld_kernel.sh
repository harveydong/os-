#!/bin/sh
ld -Ttext 0xc0001500 -e main -m elf_i386 -o kernel.bin main.o init.o low_int.o timer.o console.o keyboard.o lock.o int.o thread.o switch.o  list.o  memory.o bitmap.o string.o  debug.o print.o
cp kernel.bin ../../
