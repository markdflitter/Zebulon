#!/bin/bash

cp include/stdio.h /usr/local/crossgcc/include
cp include/string.h /usr/local/crossgcc/include
cp include/stdlib.h /usr/local/crossgcc/include

cp include/libgloss.h /usr/local/crossgcc/include

cp bld/libc.a /usr/local/crossgcc/lib

