#include "Underground.h"

Underground::Underground()
{
  tunnelSize = 160;
  //setCenter(0, 0);
  initGrid();
}

void Underground::initGrid()
{
  // move this function to this class
  setupUnderground();
  // move this function to this class
  generateNest(tunnelSize);
}

void Underground::setupUnderground()
{
  int x,y;
  for (x=0; x < WIDTH; x++)
    for (y=0; y < DEPTH; y++)
    {
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
}
