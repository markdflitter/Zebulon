#include "../include/start.h"
#include <bsp.h>
#include <stdio.h>

extern int main (void);

typedef void (*f) (void);

extern char __bss_begin;
extern char __bss_end;

void clear_bss ()
{
	for (char* p = &__bss_begin; p != &__bss_end; p++)
		*p = 0;
}

extern f __ctors_begin;
extern f __ctors_end;

void call_static_constructors ()
{
	for (f* cur = &__ctors_begin; cur != &__ctors_end; cur++)
		(*cur)();
}

extern "C" int __main (void)
{
	asm ("MOVE.B #5,0xE00001");
	clear_bss();
	asm ("MOVE.B #6,0xE00001");
	
	__init ();
	asm ("MOVE.B #7,0xE00001");
	__putstr ("bsp initialised\n\r");

	__putstr ("calling static constructors\n\r");
  	call_static_constructors ();
	asm ("MOVE.B #8,0xE00001");
	
	__putstr ("handing off to main()\n\r");
	return main ();
}


