#include "QueenAnt.h"

#define GESTATION_PERIOD 100

void QueenAnt::stateStep(int num)
{
  if ((gestation >= GESTATION_PERIOD) && getAI())
  {
    Patch* pat = findEmptyAdjacent();
    if ((pat != '\0') && (pat->portal == '\0'))
    {
      // lay egg.
      pat->TYPE = PATCH_EGG1;
      gestation = 0;
    }
  }
  else
    gestation++;
}

void QueenAnt::moveAI()
{
  // TODO: make wander not go through portals.
  if (gestation >= GESTATION_PERIOD)
    if (findEmptyAdjacent() == '\0')
      wander();
//  if (findEmptyAdjacent() == '\0')
//  if (gestation >= GESTATION_PERIOD)
//    wander();
}

