#include "Ant.h"

void Ant::handleFeramone()
{
  INCREASE_FERAMONE(getPatch(), 1);
}

void Ant::forage()
{
  Patch *cache = checkForFood();

  // Found food, pick it up.
  if (cache != '\0')
  {
    pickup( cache );
    // go Home.
    //goHome();
//   wander();
  return;
  }
  
//  this->setTakePortals( false );
  takePortals = false;
  // Wander around underground till get to surface
  if (this->getLocation() != GAMEWORLD_STATE_SURFACE)
  {
//    this->setTakePortals( true );
    takePortals = true;
    wander();
    return;
  }


  // This is the wiggle room, if two paths are within this number of each other,
  // go randomly between them.  This way the ants don't follow each other without
  // a good reason.
  int randWiggle = 2;
  int followChem = -1;

  cache = getPatch();

  // If it is walkable, and not turned around, see if other criteria are met
  if ((cache->bottom) && directionIsOk(0, directionOld, cache->bottom))
//WALKABLE(cache->bottom) && (directionOld != 0))
  {
    // if cache is greater than the wiggle value, set the direction.
    if (randWiggle < cache->bottom->chemLevel)
    {
      followChem = cache->bottom->chemLevel;
      direction = 0;
    }
  }
  if ((cache->right) && directionIsOk(1, directionOld, cache->right))
//WALKABLE(cache->right) && (directionOld != 1))
  {
    // if this chemLevel is greater than the current trail.
    if (cache->right->chemLevel > followChem)
    {
      if (randWiggle < cache->right->chemLevel)
      {
        followChem = cache->right->chemLevel;
        direction = 1;
      }
    }
  }
  if ((cache->left) && directionIsOk(2, directionOld, cache->left))
//WALKABLE(cache->left) && (directionOld != 2))
  {
    if (cache->left->chemLevel > followChem)
    {
      if (randWiggle < cache->left->chemLevel)
      {
        followChem = cache->left->chemLevel;
        direction = 2;
      }
    }
  }
  if ((cache->top) && directionIsOk(3, directionOld, cache->top))
//WALKABLE(cache->top) && (directionOld != 3))
  {
    if (cache->top->chemLevel > followChem)
    {
      if (randWiggle < cache->top->chemLevel)
      {
        followChem = cache->top->chemLevel;
        direction = 3;
      }
    }
  }

  // if no trail to follow, go random.
  if (followChem == -1)
    wander();
  else
    setAI(false);
}

Patch* Ant::checkForFood()
{

  Patch* cache = getPatch();

  // If it is walkable, and not turned around, see if other criteria are met
  if ((cache->bottom) && FOOD(cache->bottom))
  {
    return cache->bottom;
  }
  else if ((cache->right) && FOOD(cache->right))
  {
    return cache->right;
  }
  else if ((cache->left) && FOOD(cache->left))
  {
    return cache->left;
  }
  else if ((cache->top) && FOOD(cache->top))
  {
    return cache->top;
  }

  return '\0';
}
