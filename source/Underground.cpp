#include "Underground.h"

Underground::Underground()
{
  tunnelSize = 160;
  setCenter(0, 0);
  initGrid();
}

void Underground::initGrid()
{
  getGrid()->setupUnderground();
  getGrid()->generateNest(tunnelSize);
}
