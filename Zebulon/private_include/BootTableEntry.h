#ifndef BOOTTABLEENTRY_H
#define BOOTTABLEENTRY_H

#include <shared_ptr>
#include <string>

namespace Zebulon
{

struct BootTableEntry
{
	typedef mdf::shared_ptr <BootTableEntry> Ptr;
	typedef mdf::shared_ptr <const BootTableEntry> ConstPtr;

	BootTableEntry () : empty (true)
	{
		shortName.resize (20, ' ');
	}
	BootTableEntry (const std::string& name,
		unsigned int _length, unsigned int _loadAddress, 
		unsigned int _goAddress, unsigned long _startBlock)
		: shortName (name), length (_length), loadAddress (_loadAddress),
		goAddress (_goAddress), startBlock (_startBlock), empty (false)
	{
		shortName.resize (20,' ');
	}

	bool empty;					
	std::string shortName;
	unsigned int length;
	unsigned int loadAddress;
	unsigned int goAddress;
	unsigned long startBlock;
};

}
#endif
