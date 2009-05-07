#ifndef DIRTDISK_H
#define DIRTDISK_H

#include "global.h"
#include "Patch.h"
#include "Dirt.h"
//#include "Empty.h"
#include "Barrier.h"

class DirtDisk
{
	public:
		DirtDisk();
		Patch* getPatch(short slice, short width, short depth)
			{ return dd[slice][width][depth]; }

		Patch* getRight(Patch* p);
		Patch* getLeft(Patch* p);
		Patch* getTop(Patch* p);
		Patch* getBottom(Patch* p);
	private:
		Patch* dd[SLICES][WIDTH][DEPTH];
};

#endif
