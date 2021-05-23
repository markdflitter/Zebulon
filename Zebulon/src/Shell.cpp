#include "Shell.h"
#include <stdio.h>
#include <string.h>
#include <bsp.h>
#include <string>
#include <list>
#include <vector>
#include <stdlib.h>
#include "FAT.h"
#include "ctype.h"

extern char* __begin;
extern char* __end;
extern char* start;

namespace {

void printHelp (void)
{
	printf ("version\t\t\t - print version\n\r");
	printf ("help\t\t\t - print this help\n\r");
	printf ("exit\t\t\t - exit to monitor\n\r");
	printf ("ident\t\t\t - ident the disk\n\r");
	printf ("readB <block>\t\t - read block from disk\n\r");
	printf ("writeB <block>\t\t - write block to disk\n\r");
	printf ("save <block>\t\t - save code to disk\n\r");
	printf ("format <size>\t\t - format the filing system\n\r");
	printf ("create <name> <size>\t - create a file\n\r");
	printf ("write <name>\t\t - fill file with stuff\n\r");
	printf ("read <name>\t\t - read file from disk\n\r");
	printf ("ls\t\t\t - list files\n\r");
}

void ident ()
{
	DiskInfo info;
	if (__ide_ident (info))
	{
		printf ("model number\t\t\t\t: %s\n\r",info.modelNumber);
		printf ("serial number\t\t\t\t: %s\n\r",info.serialNumber);
		printf ("firmware revision\t\t\t: %s\n\r",info.firmwareRevision);
		printf ("\n\r");

		if (info.fixedDrive) printf ("fixed drive\n\r");
		if (info.removeableCartridgeDrive) printf ("removeable cartridge drive\n\r");
		if (info.hardSectored) printf ("hard sectored\n\r");
		if (info.softSectored) printf ("soft sectored\n\r");
		if (info.LbaSupported) printf ("LBA supported\n\r");
		if (info.DmaSupported) printf ("DMA supported\n\r");
		if (info.notMfmEncoded) printf ("not MFM encoded\n\r");
		if (info.headSwitchTime15uS) printf ("head switch time > 15uS\n\r");
		if (info.transferRateLt5Mbs) printf ("disk transfer rate < 5Mbs\n\r");
		if (info.transferRateLt10Mbs) printf ("disk transfer rate > 5Mbs buyt <- 10Mbs\n\r");
		if (info.transferRateGt10Mbs) printf ("disk transfer rate > 10Mbs\n\r");
		if (info.doubleWordIO) printf ("can perform double word io\n\r");
		if (info.spindleMotorControlOptionImplemented) printf ("spindle motor control option implemented\n\r");
		if (info.dataStrobeOffsetOptionAvailable) printf ("data strobe offset option availabe\n\r");
		if (info.trackOffsetOptionAvailable) printf ("track offset option available\n\r");
		if (info.formatSpeedToleranceGapRequired) printf ("format speed tolerance gap required\n\r");
		printf ("\n\r");

		printf ("number of cylinders\t\t\t: %d\n\r",info.numCylinders);
		printf ("number of heads\t\t\t\t: %d\n\r",info.numHeads);
		printf ("bytes per track\t\t\t\t: %d\n\r",info.numBytesPerTrack);
		printf ("bytes per sector\t\t\t: %d\n\r",info.numBytesPerSector);
		printf ("sectors per track\t\t\t: %d\n\r",info.numSectorsPerTrack);
		printf ("\n\r");

		printf ("buffer type\t\t\t\t: %x\n\r",info.bufferType);
		printf ("buffer size\t\t\t\t: %d\n\r",info.bufferSize);
		printf ("number of ECC bytes\t\t\t: %d\n\r",info.numEccBytes);
		printf ("max rw sectors per interrupt\t\t: %x\n\r",info.maxRwSectorsPerInterrupt);
		printf ("\n\r");

		printf ("PIO mode\t\t\t\t: %d\n\r",info.PioMode);
		printf ("DMA mode\t\t\t\t: %d\n\r",info.DmaMode);	
		printf ("\n\r");

		printf ("number of current cylinders\t\t: %d\n\r",info.numCurrentCylinders);
		printf ("number of current heads\t\t\t: %d\n\r",info.numCurrentHeads);
		printf ("number of current sectors per track\t: %d\n\r",info.numCurrentSectorsPerTrack);
		printf ("current capacity in sectors\t\t: %d\n\r",info.currentCapacityInSectors);
		printf ("total number of user sectors\t\t: %d\n\r",info.totalNumOfUserSectors);
		printf ("\n\r");
		
		printf ("single word DMA modes supported\t\t: %d\n\r",info.singlewordDmaModesSupported);
		printf ("single word DMA modes active\t\t: %d\n\r",info.singlewordDmaModesActive);
		printf ("multi word DMA modes supported\t\t: %d\n\r",info.multiwordDmaModesSupported);
		printf ("multi word DMA modes active\t\t: %d\n\r",info.multiwordDmaModesActive);
	}
	else
		printf ("ERROR: ident failed\n\r");
}

size_t min (size_t l, size_t r)
{
	return l < r ? l : r;
}


void printBuffer (unsigned char* buffer, size_t bufferLen)
{
	unsigned char* p = buffer;

	while (bufferLen > 0)
	{
		size_t rowLen = min (bufferLen, 16);
		for (int c = 0; c < rowLen; c++) printf ("%x",*p++);
		
		size_t pad = 16 -  rowLen;
		while (pad)
		{
			printf ("  ");
			pad--;
		}

		printf (" ");

		p -= rowLen;
		for (int c = 0; c < rowLen; c++)
		{
			if (isprint (*p))
				printf ("%c",*p);
			else
				printf (".");
			p++;
		}
		bufferLen -= rowLen;
		printf ("\n\r");
	}
}


void readB (block_address_t block)
{
	printf ("reading from %d\n\r", block);

	unsigned char data [512];
	
	__ide_read (block, data);
	printBuffer (data, 512);
}

void writeB (block_address_t block)
{
	printf ("writing to %d\n\r", block);

	unsigned char data [] = "The house stood on a slight rise just on the edge of the village. It stood on its own and looked out over a broad spread of West Country farmland. Not a remarkable house by any means—it was about thirty years old, squattish, squarish, made of brick, and had four windows set in the front of a size and proportion which more or less exactly failed to please the eye.  The only person for whom the house was in any way special was Arthur Dent, and that was only because it happened to be the one he lived in.";
	
	__ide_write (block, data);
}

void save (block_address_t startBlock)
{
	block_address_t curBlock = startBlock;

	static char* begin = (char*) &__begin;
	static char* end = (char*) &__end;
	static char* entry = (char*) &start;

	printf ("start 0x%x end 0x%x entry 0x%x\n\r", begin, end, entry);

	file_address_t length = end - begin;
	block_address_t numBlocks = (length / 512) + 1;

	printf ("%d bytes, which is %d blocks\n\r", length, numBlocks);

	for (file_address_t b = 0; b < length;)
	{
		unsigned char block [512];
		if (b == 0)
		{
			memcpy (&block, &begin, 4);
			memcpy (&block [4], &end, 4);
			memcpy (&block [8], &entry, 4);
			memcpy (&block [12], begin, 500);
			b += 500;
		}
		else
		{
			memcpy (&block, begin + b, 512);
			b += 512;
		}

		printf ("writing block %d of %d\n\r", curBlock - startBlock + 1, numBlocks);
		__ide_write (curBlock, block);
		curBlock++;
	}
}


void format (FAT& fat, block_address_t size)
{
	printf ("formatting to %d\n\r", size);	
	fat.format (size);
}

void stat (const File& file)
{
	printf ("%s : %d bytes : ",file.name ().c_str (), file.size ());
	for (std::list<Chunk>::const_iterator i = file.chunks ().begin (); i != file.chunks() .end (); i++)
		printf ("%d -> %d (length %d)\n\r", (*i).m_start, (*i).m_start + (*i).m_length - 1, (*i).m_length);
}

void create (FAT& fat, const std::string& filename, block_address_t size)
{
	printf ("creating file '%s' of size %d\n\r", filename.c_str (), size);
	File file = fat.createFile (filename, size);
	stat (file);
}

void write (FAT& fat, const std::string& filename)
{
/*
	std::list<File>& files = fat.ls ();
	
	for (std::list<File>::iterator i = files.begin (); i != files.end (); i++)
	{
		if ((*i).name () == filename)
		{
			OpenFile f = (*i).open ();

			file_address_t bytesLeftToWrite = (*i).allocSize();

			unsigned char data [] = "Marley was dead: to begin with. There is no doubt whatever about that. The register of his burial was signed by the clergyman, the clerk, the undertaker, and the chief mourner. Scrooge signed it. And Scrooge's name was good upon 'Change, for anything he chose to put his hand to. Old Marley was as dead as a door-nail. Mind! I don't mean to say that I know, of my own knowledge, what there is particularly dead about a door-nail. I might have been inclined, myself, to regard a coffin-nail as the deadest piece of ironmongery in the trade. But the wisdom of our ancestors is in the simile;           ";

			unsigned char* p = data;

			while (bytesLeftToWrite > 0)
			{
				unsigned char buffer [100];
				if (bytesLeftToWrite >= 100)
				{
					memcpy (buffer, p, 100);
					f.write (buffer, 100);
					bytesLeftToWrite -= 100;
					p += 100;
					if (p - data >= 600) p = data;
				}
				else
				{
					memcpy (buffer, p, bytesLeftToWrite);
					f.write (buffer, bytesLeftToWrite);
					bytesLeftToWrite -= bytesLeftToWrite;
				}
			}
		}
	}
*/
}

void read (FAT& fat, const std::string& filename)
{
/*
	std::list<File>& files = fat.ls ();
	
	for (std::list<File>::iterator i = files.begin (); i != files.end (); i++)
	{
		if ((*i).name () == filename)
		{
			file_address_t bytesLeftToRead = (*i).size ();
			
			OpenFile f = (*i).open ();

			while (!f.EOF ())
			{
				unsigned char buffer [480];
				if (bytesLeftToRead >= 480)
				{
					f.read (buffer, 480);
					printBuffer (buffer, 480);
					bytesLeftToRead -= 480;
				}
				else
				{
					f.read (buffer, bytesLeftToRead);
					printBuffer (buffer, bytesLeftToRead);
					bytesLeftToRead -= bytesLeftToRead;
				}	
			}
			break;
		}
	}
*/
}

void ls (FAT& fat)
{
	std::list<File>& files = fat.ls ();
	for (std::list<File>::iterator i = files.begin (); i != files.end (); i++)
		stat (*i);
}

std::vector<std::string> tokenize (const std::string& command)
{
	std::vector<std::string> result;

	const char* str = command.c_str ();
	char buf [255];
	char* cur = buf;
	while (*str != '\0')
	{
		if (*str != ' ')
			*cur++ = *str;
		else
		{
			*cur = '\0';
			result.push_back (std::string (buf));
			cur = buf;
		}
		str++;
	}

	if (cur != buf)
	{
		*cur = '\0';
		result.push_back (std::string (buf));
		cur = buf;
	}

	return result;
}

}

Shell::Shell (unsigned int& tick) : m_tick (tick)
{
}

void Shell::run () const
{
	FAT fat;

	const char* version = "Z-Shell V1.14";
	printf ("\n\r");
	printf ("%s\n\r",version);
	printf ("type help for help\n\r");

	char buf [255];
	int exit = 0;
	while (!exit)
	{
		printf ("$ ");
		char* p = gets (buf);
		*p = '\0';
		printf ("\n\r");

		std::string command (buf);
		std::vector<std::string> tokens = tokenize (command);

		if (tokens.size () > 0)
		{
			if (tokens [0] == "version") printf ("%s\n\r",version);
			if (tokens [0] == "help") printHelp ();
			if (tokens [0] == "exit") exit = 1;
			if (tokens [0] == "ident") ident ();
			if (tokens [0] == "readB" && tokens.size () > 1) 
			{
				block_address_t block = atol (tokens [1].c_str ());
				readB (block);
			}
			if (tokens [0] == "writeB" && tokens.size () > 1)
			{
				block_address_t block = atol (tokens [1].c_str ());
				writeB (block);
			}
			if (tokens [0] == "save" && tokens.size () > 1)
			{
				block_address_t block = atol (tokens [1].c_str ());			
				save (block);
			}
			if (tokens [0] == "format" && tokens.size () > 1)
			{
				block_address_t size = atol (tokens [1].c_str ());			
				format (fat, size);
			}
			if (tokens [0] == "create" && tokens.size () > 2)
			{
				std::string filename (tokens [1].c_str ());
				block_address_t size = atol (tokens [2].c_str ());
				create (fat, filename, size);
			}
			if (tokens [0] == "write" && tokens.size () > 1)
			{
				std::string filename (tokens [1].c_str ());
				write (fat, filename);
			}
			if (tokens [0] == "read" && tokens.size () > 1)
			{
				std::string filename (tokens [1].c_str ());
				read (fat, filename);
			}
			if (tokens [0] == "ls") ls (fat);
		}
	}

	return ;
}
