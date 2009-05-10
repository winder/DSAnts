#include "DirtDisk.h"

DirtDisk::DirtDisk()
{
	int x,y,z;

	// Create the objects.
	for (x=0; x < SLICES; x++)
		for (y=0; y < WIDTH; y++)
			for (z=0; z < DEPTH; z++)
			{
			if (z == DEPTH-1)
				dd[x][y][z] = new Barrier(x,y,z);
			else
					dd[x][y][z] = new Dirt(x,y,z);
			}

	// Connect the objects.
	for (x=0; x < SLICES; x++)
		for (y=0; y < WIDTH; y++)
			for (z=0; z < DEPTH; z++)
			{
				dd[x][y][z]->setTop( getTop( dd[x][y][z]) );
				dd[x][y][z]->setBottom( getBottom( dd[x][y][z]) );
				dd[x][y][z]->setLeft( getLeft( dd[x][y][z]) );
				dd[x][y][z]->setRight( getRight( dd[x][y][z]) );

				// TODO: handle this differently.
				dd[x][y][z]->setColor(0.5, 0.5, 0.5);
			}
}

Patch* DirtDisk::getRight(Patch* p)
{
	short s,w;


	// Slice / Width roll-over
	if ( p->getWidth() != WIDTH - 1 )
	{
		s = p->getSlice();
		w = p->getWidth() + 1;
	}
	else
	{
		w = 0;
		// Slice roll-over
		if ( p->getSlice() != SLICES - 1 )
			s = p->getSlice() + 1;
		else
			s = 0;
	}

	// Depth max/min doesn't change moving right/left
	return dd[s][w][ p->getDepth() ];
}


Patch* DirtDisk::getLeft(Patch* p)
{
	short s,w;


	// Slice / Width roll-over
	if ( p->getWidth() >= 1 )
	{
		s = p->getSlice();
		w = p->getWidth() - 1;
	}
	else
	{
		w = WIDTH - 1;
		// Slice roll-over
		if ( p->getSlice() != 0 )
			s = p->getSlice() - 1;
		else
			s = SLICES - 1;
	}

	// Depth max/min doesn't change moving right/left

	return dd[s][w][ p->getDepth() ];
}
Patch* DirtDisk::getTop(Patch* p)
{
	// SLICE and WIDTH don't change.
	if ( p->getDepth() == 0 )
		return '\0';

	return dd[ p->getSlice() ][ p->getWidth() ][ p->getDepth() - 1 ];
}
Patch* DirtDisk::getBottom(Patch* p)
{
	// SLICE and WIDTH don't change.
	if ( p->getDepth() == DEPTH - 1 )
		return '\0';

	return dd[ p->getSlice() ][ p->getWidth() ][ p->getDepth() + 1 ];
}
