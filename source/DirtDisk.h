#ifndef DIRTDISK_H
#define DIRTDISK_H

#include "global.h"
#include "Patch.h"
#include "Dirt.h"
//#include "Empty.h"
//#include "Barrier.h"

class DirtDisk
{
	public:
		DirtDisk();
//		Patch* getPatch()
	private:
		Patch* dd[SLICES][WIDTH][DEPTH];
};

#endif
