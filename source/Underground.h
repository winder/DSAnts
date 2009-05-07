#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include <vector>
#include "Ant.h"
#include "DirtDisk.h"

class Underground
{
	public:
		Underground(){ underground = new DirtDisk(); }
		virtual ~Underground(){ delete underground; }

		DirtDisk* getDisk()
			{	return underground; }

	private:
		std::vector<Ant> inhabitants;
		DirtDisk *underground;

};

#endif
