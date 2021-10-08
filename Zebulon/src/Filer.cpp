#include "Filer.h"
#include <stdio.h>

namespace Zebulon
{

Filer::Filer ()
{
	printf ("Filer::Filer()");
}


void Filer::load ()
{
	m_FAT.load ();
}


int Filer::format (int diskSize)
{
  	return m_FAT.initialise (diskSize);
}

}
