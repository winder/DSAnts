#include "Underground.h"

Underground::Underground()
{
  tunnelSize = 160;
  //setCenter(0, 0);
  initGrid( GAMEWORLD_STATE_UNDERGROUND );
}
Underground::Underground(int locName)
{
  location = locName;
  tunnelSize = 160;
  initGrid(locName);
}

void Underground::initGrid(int location)
{
  // move this function to this class
  setupUnderground(location);
  // move this function to this class
  generateNest(tunnelSize);
}

void Underground::setupUnderground(int location)
{
  int x,y;
  for (x=0; x < WIDTH; x++)
    for (y=0; y < DEPTH; y++)
    {
      dd[x][y].location = location;

      // Unpassable patch at the deepest level.
      if (y == DEPTH-1)
        dd[x][y].TYPE = PATCH_BARRIER;
      // Sky / trigger to enter surface world at the top.  This patch will
      // have a coordinate to where on the surface the hole emerges.
      else if ( y == 0 )
        dd[x][y].TYPE = PATCH_TOP;
      // Everything else is dirt.
      else
        dd[x][y].TYPE = PATCH_DIRT;
    }
}

// This algorithm works, it does what its supposed to, but it doesn't make a
// very convincing nest.
void Underground::generateNest(int size)
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
//      else if (start->top && (start->top->TYPE == PATCH_EMPTY))
//        start = start->top;

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
