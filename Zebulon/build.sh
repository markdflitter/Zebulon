#!/bin/bash
m68k-elf-gcc Zebulon.c -o Zebulon.out -I/usr/local/crossgcc/include -L/usr/local/crossgcc/lib -lc -lbsp -nostartfiles -nostdlib -Wl,--script=Zebulon.ld -Wl,--entry=main -Wl,--print-map -march=68000
m68k-elf-objcopy -O srec Zebulon.out Zebulon.S68
