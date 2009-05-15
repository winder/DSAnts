#include "Grid.h"

Grid::Grid()
{
	int x,y;

	// Create the objects.
	for (x=0; x < WIDTH; x++)
		for (y=0; y < DEPTH; y++)
		{
			dd[x][y] = new Patch();

			dd[x][y]->x = x;
			dd[x][y]->y = y;
			dd[x][y]->location = GAMEWORLD_STATE_UNDERGROUND;

			// Unpassable patch at the deepest level.
			if (y == DEPTH-1)
				dd[x][y]->TYPE = PATCH_BARRIER;
			// Sky / trigger to enter surface world at the top.  This patch will
			// have a coordinate to where on the surface the hole emerges.
			else if ( y == 0 )
				dd[x][y]->TYPE = PATCH_TOP;
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

// Makes everything empty.  Ant hills, barriers, etc need to be setup later.
void Grid::setupSurface()
{
	// Connect the objects.
	for (int x=0; x < WIDTH; x++)
		for (int y=0; y < DEPTH; y++)
		{
			dd[x][y]->TYPE = PATCH_EMPTY;
			dd[x][y]->location = GAMEWORLD_STATE_SURFACE;
		}
}
void Grid::moveRight(short &x)
{
	// Width roll-over
	if ( x != WIDTH - 1 )
		x = x + 1;
	else
		x = 0;
}

void Grid::moveLeft(short &x)
{
	// Width roll-over
	if ( x >= 1 )
		x = x - 1;
	else
		x = WIDTH - 1;
}

bool Grid::moveUp(short &y)
{
	// SLICE and WIDTH don't change.
	if ( y == 0 )
		return false;
	y--;
	return true;
}

bool Grid::moveDown(short &y)
{
	// SLICE and WIDTH don't change.
	if ( y == DEPTH - 1 )
		return false;
	y++;
	return true;
}

Patch* Grid::getRight(Patch* p)
{
	return p->right;
}

Patch* Grid::getLeft(Patch* p)
{
	return p->left;
}

Patch* Grid::getUp(Patch* p)
{
	return p->top;
}

Patch* Grid::getDown(Patch* p)
{
	return p->bottom;
}

// use the reference methods above to take a pointer and get the one left to it.
Patch* Grid::getRight(int x, int y)
{
	short w = x;
	moveRight(w);

	// Depth max/min doesn't change moving right/left
	return dd[w][y];
}

Patch* Grid::getLeft(int x, int y)
{
	short w = x;
	moveLeft(w);

	// Depth max/min doesn't change moving right/left
	return dd[w][y];
}
Patch* Grid::getTop(int x, int y)
{
	short d = y;
	if (!moveUp(d)) return '\0';

	return dd[x][d];
}
Patch* Grid::getBottom(int x, int y)
{
	short d = y;
	if (!moveDown(d)) return '\0';

	return dd[x][d];
}

// This algorithm works, it does what its supposed to, but it doesn't make a
// very convincing nest.
void Grid::generateNest(int size)
{
	Patch *start = getPatch(0,1);

	// dig down a little and set them as empty spot
	clear(start);
	start = start->bottom;
	clear(start);

	time_t t;
	t = time(NULL);
	srand(t);

	int direction = 0;
	int x = 0;
	while (x++ < size)
	{
		// move in a random direction.
		direction = rand()%4;

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
//			else if (start->top && (start->top->TYPE == PATCH_EMPTY))
//				start = start->top;

			x--;
		}

		clear(start);
	}
/*
	// Make a big cross for testing.
	int var;
	start = getPatch(0,1);

	for (var=0; var < 30; var++)
	{
		start->TYPE = PATCH_EMPTY;
		start = start->bottom;
	}

	start = getPatch(15, 15);
	for (var=0; var < 30; var++)
	{
		start->TYPE = PATCH_EMPTY;
		start = start->left;
	}
*/
}

void Grid::clear(Patch* p)
{
	if (p == '\0') return;

	if (p->TYPE == PATCH_DIRT)
	{
		p->TYPE = PATCH_EMPTY;
		cleared.push_back(p);
	}
}
