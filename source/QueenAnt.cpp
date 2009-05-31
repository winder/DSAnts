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
    wander();
//  if (findEmptyAdjacent() == '\0')
//  if (gestation >= GESTATION_PERIOD)
//    wander();
}

// I'm seeing a lot of repeat code doing this, a helper function will reduce
// chance of errors later on.
Patch* QueenAnt::findEmptyAdjacent()
{
  if( EMPTY(getPatch()) )
    return getPatch();
  else if( EMPTY(getPatch()->right) )
    return getPatch()->right;
  else if( EMPTY(getPatch()->left) )
    return getPatch()->left;
  else if( getPatch()->top && EMPTY(getPatch()->top) )
      return getPatch()->top;
  else if( getPatch()->bottom && EMPTY(getPatch()->bottom) )
    return getPatch()->bottom;
  // top right
  else if ( (EMPTY(getPatch()->right) || EMPTY(getPatch()->top)) && EMPTY(getPatch()->right->top) )
    return getPatch()->right->top;
  // bottom right
  else if ( (EMPTY(getPatch()->right) || EMPTY(getPatch()->bottom)) && EMPTY(getPatch()->right->bottom) )
    return getPatch()->right->bottom;
  // top left
  else if ( (EMPTY(getPatch()->left) || EMPTY(getPatch()->top)) && EMPTY(getPatch()->left->top) )
    return getPatch()->left->top;
  // bottom left
  else if ( (EMPTY(getPatch()->left) || EMPTY(getPatch()->bottom)) && EMPTY(getPatch()->left->bottom) )
    return getPatch()->left->bottom;
  return '\0';
}
