#include "DirtDisk.h"

DirtDisk::DirtDisk()
{
	int x,y,z;

	// Create the objects.
	for (x=0; x < SLICES; x++)
		for (y=0; y < WIDTH; y++)
			for (z=0; z < DEPTH; z++)
			{
				dd[x][y][z] = new Patch();
			if (z == DEPTH-1)
				dd[x][y][z]->TYPE = PATCH_BARRIER;
			else if ( z == 0 )
				dd[x][y][z]->TYPE = PATCH_TOP;
			else if (y%2 == 1)
				dd[x][y][z]->TYPE = PATCH_EMPTY;
			else
				dd[x][y][z]->TYPE = PATCH_DIRT;
			}

	// Connect the objects.
	for (x=0; x < SLICES; x++)
		for (y=0; y < WIDTH; y++)
			for (z=0; z < DEPTH; z++)
			{
				dd[x][y][z]->top = getTop(x,y,z);
				dd[x][y][z]->bottom = getBottom(x,y,z);
				dd[x][y][z]->left = getLeft(x,y,z);
				dd[x][y][z]->right = getRight(x,y,z);
			}
}

void DirtDisk::moveRight(short &slice, short &x)
{
	// Slice / Width roll-over
	if ( x != WIDTH - 1 )
	{
//		slice = slice;
		x = x + 1;
	}
	else
	{
		x = 0;
		// Slice roll-over
		if ( slice != SLICES - 1 )
			slice = slice + 1;
		else
			slice = 0;
	}
}

void DirtDisk::moveLeft(short &slice, short &x)
{
	// Slice / Width roll-over
	if ( x >= 1 )
	{
		slice = slice;
		x = x - 1;
	}
	else
	{
		x = WIDTH - 1;
		// Slice roll-over
		if ( slice != 0 )
			slice = slice - 1;
		else
			slice = SLICES - 1;
	}
}

bool DirtDisk::moveUp(short &y)
{
	// SLICE and WIDTH don't change.
	if ( y == 0 )
		return false;
	y--;
	return true;
}

bool DirtDisk::moveDown(short &y)
{
	// SLICE and WIDTH don't change.
	if ( y == DEPTH - 1 )
		return false;
	y++;
	return true;
}

// use the reference methods above to take a pointer and get the one left to it.
Patch* DirtDisk::getRight(int x, int y, int z)
{
	short s;
	short w;
	s = x;
	w = y;

	moveRight(s, w);
	// Depth max/min doesn't change moving right/left
	return dd[s][w][ z ];
}

Patch* DirtDisk::getLeft(int x, int y, int z)
{
	short s, w;
	s = x;
	w = y;
	moveLeft(s, w);

	// Depth max/min doesn't change moving right/left
	return dd[s][w][ z ];
}
Patch* DirtDisk::getTop(int x, int y, int z)
{
	short d = z;
	if (!moveUp(d)) return '\0';

	return dd[ x ][ y ][ d ];
}
Patch* DirtDisk::getBottom(int x, int y, int z)
{
	short d = z;
	if (!moveDown(d)) return '\0';


	return dd[ x ][ y ][ d ];
}
