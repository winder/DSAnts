#include "Surface.h"

Surface::Surface()
{
	initGrid();
}

void Surface::initGrid()
{
	getGrid()->setupSurface();

	// Add a bunch of food in a cluster on the grid.
	makeFood();
}


void Surface::makeFood()
{
	// snag a reference to the grid.
	Grid* g = getGrid();

	Patch* p;
	Patch* tl;
	// First find a random empty spot on the surface.  getRandomCleared should
	// return an empty patch on the first check, but there is little harm in being safe.
	do {
		tl = g->getRandomCleared();
	} while (!EMPTY(tl));

	// move it up and to the left.
	for (int i=0; i<3; i++)
	{
		tl = Grid::getLeft(tl);
		tl = Grid::getUp(tl);
	}
	p = tl;
	for (int x=0; x<=6; x++)
	{
		for (int y=0; y<=6; y++)
		{
			g->addObject(p, PATCH_FOOD10);
			p = Grid::getRight(p);
		}
		tl = Grid::getDown(tl);
		p = tl;
	}

}
