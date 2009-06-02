#include "Ant.h"

void Ant::handleFeramone()
{
//  INCREASE_FERAMONE(getPatch(), feramoneOutput);
  SET_FERAMONE(getPatch(), feramoneOutput);
}

// This one should work as follows:
// 0. Do not turn around unless dead end or specified below.
// 1. If in nest with food, drop food then leave.
// 2. If outside, look for feramone, follow.
// 3. If feramone dead end, random direction.
// 4. If food, pick up and turn around.
// 5. If carrying food follow feramone.
//--------------this needs more----------------
//-- New algorithm looks like this:
// Easy to check for cases:
// 1. If carrying food, go home.
// 2.   If in base, drop food.
// 3. If not on surface, wander until on surface.
// 4. else (on surface) check for food, pickup, set feramone output = 1000:
// 5.   Pickup food, mark food spot with feramone, go home
// 6. If no food, set feramone output = 100:
// 7.   If on surface and HOT trail, follow (hot == feramone > 100)
// 8.   If on surface and no HOT trail, go to a spot with no feramone
// 9.     Move to spot with no feramone, Mark feramone (output = 100)
// 10.  If no spot with no feramone, wander
void Ant::forage()
{
  // 1. If carrying food, go home.
  if (FOODi(getCarrying()))
  {
    // 2.   If in base, drop food.

    // if (this->getLocation() == mah base)
    //   drop food some place
    //   not carrying anymore, lower feramone output.
    //   feramoneOutput = 100;
    //goHome();
    wander();
    return;
  }

  // Step 3. Not on surface, wander till on surface.
  takePortals = false;
  // Wander around underground till get to surface
  if (this->getLocation() != GAMEWORLD_STATE_SURFACE)
  {
    // TODO: set these as little as necessary.
    takePortals = true;
    feramoneOutput = 100;
    wander();
    return;
  }
  // If we aren't underground, look for food
  else
  {
    Patch *cache = checkForFood();
    // Found food, pick it up.
    if (cache != '\0')
    {
      // 4. If food, pickup, set feramone output = 1000:
      // 5.   Pickup food, mark food spot with feramone, go home
      pickup( cache );
      feramoneOutput = 1000;
      SET_FERAMONE( cache, feramoneOutput);
      takePortals = true;

      // TODO: go Home.
      //goHome();
      wander();
      return;
    }
  }

  // 6. If no food (feramone should be = 100) - should already be set here.
  cache = getPatch();

  // sort all adjacent patches by chemical level.
  Patch* sort[4];
  int dir[4] = {0,1,2,3};
  sortAdjacentPatchByChem(cache, sort, dir); 


  // 7.   If on surface and HOT trail, follow (hot == feramone > 100)
  // TODO: this, very important, but see if I can get them to find food first.


  // count how many cold trails there are.
  int cold_dirs = 0;
  if (sort[3]->chemLevel < COLD_TRAIL)
  {
    cold_dirs++;
    if (sort[3]->chemLevel < COLD_TRAIL)
    {
      cold_dirs++;
      if (sort[3]->chemLevel < COLD_TRAIL)
      {
        cold_dirs++;
        if (sort[3]->chemLevel < COLD_TRAIL)
        { cold_dirs++; }
      }
    }
  }

  // 10.  If no spot with no feramone, wander
  // note: don't need a check for if the last one was the directionOld
  //       it isn't a cold dir because you just marked it HOT_TRAIL_LIMIT.
  if (cold_dirs == 0)
  {
    wander();
    return;
  }

  // 8.   If on surface and no HOT trail, go to a spot with no feramone
  // 9.     Move to spot with no feramone, Mark feramone (output = 100)

  // if there is only one cold direction, do it to it.
  if (cold_dirs == 1)
  {
    direction = dir[3];
  }

  // at this point I know that there is more than one 'cold_dirs' to pick from.
  int randDir = rand()%cold_dirs;
  direction = dir[(4-cold_dirs)+randDir];
  // We are done, so AI is no longer needed.
  setAI(false);
/*
  int randWiggle = 2;
  int followChem = -1;
  // If it is walkable, and not turned around, see if other criteria are met
  if ((cache->bottom) && directionIsOk(0, directionOld, cache->bottom))
  {
    // if cache is greater than the wiggle value, set the direction.
    if (randWiggle < cache->bottom->chemLevel)
    {
      followChem = cache->bottom->chemLevel;
      direction = 0;
    }
  }
  if ((cache->right) && directionIsOk(1, directionOld, cache->right))
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
*/
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

// adjacent[0] is highest, adjacent[3] is lowest.
// also keeps track of the direction so we don't need extra compares.
void Ant::sortAdjacentPatchByChem(Patch* center, Patch* adjacent[], int direction[])
{
  // setup array
  adjacent[0]=center->bottom;
  adjacent[1]=center->right;
  adjacent[2]=center->left;
  adjacent[3]=center->top;

  int i,j, dirKey;
  Patch* key;
  int array_length=4;
  // Insertion sort:
  for (j = 1; j < array_length; j++) // starts with 1 not 0
  {
    key = adjacent[j];
    dirKey = direction[j];
    for(i = j - 1; (i >= 0) && (adjacent[i]->chemLevel < key->chemLevel); i--)
    {
      adjacent[i+1] = adjacent[i];
      direction[i+1] = direction[i];
    }
    adjacent[i+1] = key;
    direction[i+1] = dirKey;
  }
/* Note: tested this in stand alone program:
void insertion_sort( int array[], int array_length )
{
  int i, j, key;
  for(j = 1; j < array_length; j++)
  {
    key = array[j];
    for(i = j - 1; (i >= 0) && (array[i] < key); i--)
    {
      array[i+1] = array[i];
    }
    array[i+1] = key;
  }
  return;
}
*/
}
