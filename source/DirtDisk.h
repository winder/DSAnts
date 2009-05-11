#ifndef DIRTDISK_H
#define DIRTDISK_H

#include "global.h"
#include "Patch.h"
//#include "Dirt.h"
//#include "Empty.h"
//#include "Barrier.h"

class DirtDisk
{
	public:
		DirtDisk();
		Patch* getPatch(short slice, short width, short depth)
			{ return dd[slice][width][depth]; }

		void moveRight(short &slice, short &x);
		void moveLeft(short &slice, short &x);
		bool moveUp(short &y);
		bool moveDown(short &y);

		Patch* getRight(int x, int y, int z);
		Patch* getLeft(int x, int y, int z);
		Patch* getTop(int x, int y, int z);
		Patch* getBottom(int x, int y, int z);

	private:
		Patch* dd[SLICES][WIDTH][DEPTH];
};

#endif
