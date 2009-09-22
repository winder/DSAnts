#include "Patch.h"


Patch::Patch()
{
  // setting it to this just so I can use the "clear" method without anything special.
  TYPE = PATCH_DIRT;
  portal = '\0';
  chemLevel[0] = 0;
  chemLevel[1] = 0;
  chemLevel[2] = 0;
  chemLevel[3] = 0;
}

Patch::Patch(int inx, int iny)
{
  Patch();
  x = inx;
  y = iny;
}
