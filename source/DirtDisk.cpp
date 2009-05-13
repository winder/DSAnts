#include "DirtDisk.h"

DirtDisk::DirtDisk()
{
	int x,y,z;

	// Create the objects.
	for (x=0; x < WIDTH; x++)
		for (y=0; y < DEPTH; y++)
		{
			dd[x][y] = new Patch();
			// Unpassable patch at the deepest level.
			if (y == DEPTH-1)
				dd[x][y]->TYPE = PATCH_BARRIER;
			// Sky / trigger to enter surface world at the top.  This patch will
			// have a coordinate to where on the surface the hole emerges.
			else if ( y == 0 )
				dd[x][y]->TYPE = PATCH_TOP;
			// This was for making something to look at.
//			else if (y%2 == 1)
//				dd[x][y][z]->TYPE = PATCH_EMPTY;
			// Everything else is dirt.
			else
				dd[x][y]->TYPE = PATCH_DIRT;
			}

	// Connect the objects.
	for (x=0; x < WIDTH; x++)
		for (y=0; y < DEPTH; y++)
		{
			dd[x][y]->top = getTop(x,y);
			dd[x][y]->bottom = getBottom(x,y);
			dd[x][y]->left = getLeft(x,y);
			dd[x][y]->right = getRight(x,y);
		}
}

void DirtDisk::moveRight(short &x)
{
	// Width roll-over
	if ( x != WIDTH - 1 )
		x = x + 1;
	else
		x = 0;
}

void DirtDisk::moveLeft(short &x)
{
	// Width roll-over
	if ( x >= 1 )
		x = x - 1;
	else
		x = WIDTH - 1;
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
Patch* DirtDisk::getRight(int x, int y)
{
	short w = x;
	moveRight(w);

	// Depth max/min doesn't change moving right/left
	return dd[w][y];
}

Patch* DirtDisk::getLeft(int x, int y)
{
	short w = x;
	moveLeft(w);

	// Depth max/min doesn't change moving right/left
	return dd[w][y];
}
Patch* DirtDisk::getTop(int x, int y)
{
	short d = y;
	if (!moveUp(d)) return '\0';

	return dd[x][d];
}
Patch* DirtDisk::getBottom(int x, int y)
{
	short d = y;
	if (!moveDown(d)) return '\0';

	return dd[x][d];
}

// This algorithm works, it does what its supposed to, but it doesn't make a
// very convincing nest.
void DirtDisk::generateNest(int size)
{
	Patch *start = getPatch(0,1);

	// dig down a little and set them as empty spot
	start->TYPE = PATCH_EMPTY;
	start = start->bottom;
	start->TYPE = PATCH_EMPTY;

	time_t t;
	t = time(NULL);
	srand(t);

	int direction = 0;
	int x = 0;
	while (x++ < size)
	{
		direction = rand()%4;

		// hard code direction = down.
//		direction = 0;

		// for every check afte the first use <=, in case the prior one
		// failed due to existing patch.
		// depending on the direction, make a hole in a random direction.
		if ((direction == 0) && (start->bottom) && (start->bottom->TYPE == PATCH_DIRT))
			start = start->bottom;
		else if ((direction <= 1) && (start->right) && (start->right->TYPE == PATCH_DIRT))
			start = start->right;
		else if ((direction <= 2) && (start->left) && (start->left->TYPE == PATCH_DIRT))
			start = start->left;
		else if ((direction <= 3) && (start->top) && (start->top->TYPE == PATCH_DIRT))
		{
			// I don't want a bunch of holes along the surface, so don't go up unless
			// it isn't next to the surface.
			if (start->top->top && (start->top->top->TYPE != PATCH_TOP))
				start = start->top;
			else
				direction = -1;
		}
		// if nothing was empty move up or down, un-increment x (decrement) and try again.	
		else
		{
			direction = -1;
		}

		if (direction == -1)
		{
			if (start->bottom && (start->bottom->TYPE == PATCH_EMPTY))
				start = start->bottom;
			else if (start->top && (start->top->TYPE == PATCH_EMPTY))
				start = start->top;

			x--;
		}

		start->TYPE = PATCH_EMPTY;
	}
	
}
