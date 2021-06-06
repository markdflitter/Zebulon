#ifndef ZEBULON_H
#define ZEBULON_H

namespace Zebulon
{

inline unsigned int _zebulon_time ()
{
	volatile unsigned int result;
	volatile register unsigned int* pResult asm("a0") = &result;

	asm ("trap #0" : : "a" (pResult));

	return result;
}

inline void _zebulon_putch (char c)
{
	volatile char cc = c;
	volatile char* p = &cc;

	asm ("moveb #0,%%d0\n\t"
		 "movel %0,%%a0\n\t"
		 "trap #1\n\t" : : "m" (p) : "a0","d0");
}

inline char _zebulon_getch ()
{
	volatile char result;
	volatile register char* p asm("a0") = &result;

	asm ("move.b #1,%%d0\n\t"
		 "trap #1\n\t" : : "a" (p) : "d0");

	return result;
}	

}

#endif

