#include "Surface.h"

Surface::Surface()
{
  chemicalTick = 0;
  initGrid();
}

void Surface::initGrid()
{
  setupSurface();

  // Add a bunch of food in a cluster on the grid.
  makeFood();
}

// Makes everything empty.  Ant hills, barriers, etc need to be setup later.
void Surface::setupSurface()
{
  // Connect the objects.
  for (int x=0; x < WIDTH; x++)
    for (int y=0; y < DEPTH; y++)
    {
      dd[x][y]->TYPE = PATCH_EMPTY;
      // If for some reason this becomes necessary, cross the bridge.
      // Otherwise a vector of thousands of patches doesn't seem too useful.
      //clear(dd[x][y]);
      dd[x][y]->location = GAMEWORLD_STATE_SURFACE;
    }
}

void Surface::makeFood()
{
  // snag a reference to the grid.
//  Grid* g = getGrid();

  Patch* p;
  Patch* tl;
  // First find a random empty spot on the surface.  getRandomCleared should
  // return an empty patch on the first check, but there is little harm in being safe.
  int randX, randY;
  // find random locations until we get one that hasn't already been picked.
  do
  {
    randX = rand() % (WIDTH / 2);
    randY = rand() % DEPTH;
    tl = getPatch(randX, randY);
  }while (!EMPTY(tl));

/*
  // move it up and to the left.
  for (int i=0; i<3; i++)
  {
    tl = Grid::getLeft(tl);
    tl = Grid::getUp(tl);
  }
*/
  p = tl;
  for (int x=0; x<=6; x++)
  {
    for (int y=0; y<=6; y++)
    {
      addObject(p, PATCH_FOOD10);
      p = Grid::getRight(p);
    }
    tl = Grid::getDown(tl);
    p = tl;
  }

}


