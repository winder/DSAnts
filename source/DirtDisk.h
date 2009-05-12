#ifndef DIRTDISK_H
#define DIRTDISK_H

// stdlib for rand / srand
#include <stdlib.h>

//#include <nds.h>
#include <time.h>

#include "global.h"
#include "Patch.h"
//#include "Dirt.h"
//#include "Empty.h"
//#include "Barrier.h"

class DirtDisk
{
	public:
		DirtDisk();

		// clamp the values just in case.
		Patch* getPatch(short slice, short width, short depth)
			{	if (slice<0) slice=0; if (width<0) width=0; if (depth<0) depth=0;
				return dd[slice%SLICES][width%WIDTH][depth%DEPTH]; }

		void moveRight(short &slice, short &x);
		void moveLeft(short &slice, short &x);
		bool moveUp(short &y);
		bool moveDown(short &y);

		Patch* getRight(int x, int y, int z);
		Patch* getLeft(int x, int y, int z);
		Patch* getTop(int x, int y, int z);
		Patch* getBottom(int x, int y, int z);

		// This will generate a random nest with 'size' connected empty spaces
		// It will have one entrance which is always at (0,20,0)
		void generateNest(int size);

	private:
		Patch* dd[SLICES][WIDTH][DEPTH];
};

#endif
