#include <size_t.h>
#include <stdlib.h>
#include <stdio.h>

void* operator new (size_t n)
{
	return malloc (n);
}

void* operator new[] (size_t n)
{
	return malloc (n);
}


