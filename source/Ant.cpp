#include "Ant.h"

// Feramone's "splash"
void Ant::handlePheramone()
{
  Patch* cur = getPatch();
   if (cur)
      INCREASE_FERAMONE(cur, feramoneOutput, currentPheramone);
return;
/*
  // if chemLevel is very high, don't put down too much
  if ((feramoneOutput > FERAMONE_LOW) && GET_PHERAMONE(cur, currentPheramone) > FERAMONE_MAX-feramoneOutput)
    SET_FERAMONE(cur, FERAMONE_MAX, currentPheramone);
  else if (feramoneOutput > FERAMONE_LOW)
    INCREASE_FERAMONE(cur, feramoneOutput, currentPheramone);
  // if feramone output is low, and tile level is low, set.
  else if ((feramoneOutput == FERAMONE_LOW) && (GET_PHERAMONE(cur, currentPheramone) <= COLD_TRAIL))
    SET_FERAMONE(cur, feramoneOutput, currentPheramone);
*/

   // chemical "splash" to each side as ant moves:
   // * = ants trail
   // - = chemical "splash" once
   // + = chemical "splash" twice
   //  | |-|+|*|-|
   //  |-|*|*|*|-|
   //  |-|*|+|-| |
 
   Patch* last = lastVisited(1);
   Patch* last2 = lastVisited(2);
 
   int splash = feramoneOutput * 0.5;
 
   // If it isn't the current, next node (cur) or last node (last2) splash it.
   if ((last->top!=last2) && (last->top!=cur))
     INCREASE_FERAMONE(last->top, splash, currentPheramone);
   if ((last->bottom!=last2) && (last->bottom!=cur))
     INCREASE_FERAMONE(last->bottom, splash, currentPheramone);
   if ((last->left!=last2) && (last->left!=cur))
     INCREASE_FERAMONE(last->left, splash, currentPheramone);
   if ((last->right!=last2) && (last->right!=cur))
     INCREASE_FERAMONE(last->right, splash, currentPheramone);

}

//-- algorithm looks like this:
// 1. If carrying food, go home.
// 2.   If in base, drop food.
// 3. If not on surface, wander until on surface.
// 4. else (on surface) check for food, pickup, set feramone output = 1000:
// 5.   Pickup food, mark food spot with feramone, go home
// 6. If no food, set feramone output = 100:
// 7.   If on surface and HOT trail, follow (hot == feramone > 100)
// 8.   If on surface and no HOT trail, go to a spot with no feramone
// 9.     Move to spot with no feramone, Mark feramone (output = 100)
// 10.  If no spot with no feramone, wander away from home.

// TODO: just call forage recursively when the state changes, there is a little repeat code in there right now.
void Ant::forage()
{
  Patch *cache;

  // 1. If carrying food, go home.
  if (FOODi(getCarrying()))
  {
    // 2. If in home base, drop food or look for a spot to drop food.
    if (getLocation() == getHome())
    {
      // Have food, in Base, don't take a portal until the food is dropped.
      takePortals=false;

      // if we are home, look for a spot to drop the food.
      cache = findFoodDropAdjacent();
      if ( cache != '\0' )
      {
        // drop food, setup to go back on the prowl.
        if(drop(cache))
        {
          feramoneOutput = FERAMONE_LOW;
          set_portaled( false );
          takePortals = true;
          if ( getPheramone() == BLACK_FOOD )
            setPheramone( BLACK_HOME );
          else
            setPheramone( RED_HOME );
        }
      }
      // if there was no place to put the food, wander till there is.
      else
      {
        wander();
        return;
      }
    }
    // otherwise we are on the surface with food, so go home.
    else
    {
      goHome();
      return;
    }
  }

  // Step 3. Not on surface, wander till on surface.
  // Wander around underground till get to surface
  if (this->getLocation() != GAMEWORLD_STATE_SURFACE)
  {
    // TODO: set these as little as necessary.
    takePortals = true;
    feramoneOutput = FERAMONE_LOW;

    // TODO: wander away from home.
    wander();
    return;
  }
  // If we aren't underground, look for food
  else
  {
    takePortals = false;
    // sometimes the Ant will eat the food before it gets home, so need to
    // make sure the feramone is set correctly.
    feramoneOutput = FERAMONE_LOW;
    cache = checkForFood();
    // Found nearby food, get over there.
    if (cache != '\0')
    {
      // if the patch IS NOT food, head towards it.
      if (!FOOD(cache))
      {
        direction = getDirectionPatch( cache );
        setAI(false);
        return;
      }
      // otherwise it is food, pick it up.
      else
      {
        // 4. If food, pickup, set feramone output = 1000:
        // 5.   Pickup food, mark food spot with feramone, go home
        pickup( cache );
        set_portaled( false );
        if ( getPheramone() == BLACK_HOME )
          setPheramone( BLACK_FOOD );
        else
          setPheramone( RED_FOOD );

        feramoneOutput = FERAMONE_HIGH;

        // set feramone (with new output) where standing.
        handlePheramone();

        takePortals = true;

        // TODO: clear memory or follow it?
        // finally, we're turning around, so reset the ants memory.
        clearVisited();

        // turn around!!
        direction = directionOld;
        setAI(false);
        return;
      }
    }
  }

  // 6. If no food (feramone should be = 100) - should already be set here.
  cache = getPatch();

  // sort all adjacent patches by chemical level.
  Patch* sort[4];
  int dir[4] = {0,1,2,3};
  sortAdjacentPatchByChem(cache, sort, dir); 


  // 7.   If on surface and HOT trail, follow (hot == feramone > 100)
  //7.5: If following a HOT trail, don't turn unless must turn.
  // if there is a trail to follow, all is handled.
  if (followTrail(sort, dir, false))
  {
    return;
  }
  // else... need to follow a cold trail

  // count how many cold trails there are.
  int cold_dirs = 0;
  for (int i = 3; i >= 0 ; i--)
  {
    if ( GET_PHERAMONE(sort[i], currentPheramone) < COLD_TRAIL )
      cold_dirs++;
  }

/* // this is dumb... above should be the same thing.
  if (sort[3]->chemLevel[0] < COLD_TRAIL)
  {
    cold_dirs++;
    if (sort[2]->chemLevel[0] < COLD_TRAIL)
    {
      cold_dirs++;
      if (sort[1]->chemLevel[0] < COLD_TRAIL)
      {
        cold_dirs++;
        if (sort[0]->chemLevel[0] < COLD_TRAIL)
        { cold_dirs++; }
      }
    }
  }
*/
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
}

// Two possible ways to do this...
// Desperate way, cheat:
// 1. Ant remembers X / Y coordinate of ant hill,
// 2.   follows a feramone path to get there, or just b-lines it that way.
// An accurate way?
// 1. Use "recent memory" to go backwards last 5 steps.
// 2. Follow feramone trail rest of way, other than the stuff we're leaving behind.
// Maybe a hybrid,
// 1. Use "recent memory" to go backwards last 5 steps.
// 2. Follow feramone, if get stuck in loop
// 3.   remember general direction and walk that way
void Ant::goHome()
{
  // check for nearby ant hill
  Patch* cache = checkForPortal();
  if (cache != '\0')
  {
    // if found, move towards it.
    direction = getDirectionPatch(cache);
    if (direction != -1)
    {
      setAI(false);
      return;
    }
  }

  // try to follow a trail home.
  if( !followTrail(true) )
  {
    // if no hot trail...
    goHomeCheating();

    // TODO: go home without cheating
  }
}

// 1. Need to sort the adjacent tiles to see what we're dealing with,
// 2. Call 2nd follow method.
bool Ant::followTrail(bool home)
{
  Patch* sort[4];
  int dir[4] = {0,1,2,3};
  sortAdjacentPatchByChem(getPatch(), sort, dir); 
  return followTrail(sort, dir, home);
}

bool Ant::followTrail(Patch* sort[], int dir[], bool home)
{
  // 1. If there is no trail to follow, return false.
  if (GET_PHERAMONE(sort[0], currentPheramone) < HOT_TRAIL_LIMIT) return false;

  // Figure out how many trails are hot, and how many of those are new.
  int numHot = 0;
  int numNew = 0;
  for(int i=0; i<4; i++)
    if (GET_PHERAMONE(sort[i], currentPheramone) >= HOT_TRAIL_LIMIT)
    {
      numHot++;
      if (!checkVisited(sort[i]))
        numNew++;
    }

  int next;
  Patch *cache;

  next = -1;
  cache = getPatch();

  // Figure out how many diagonals are hot so we know if we're in the middle of something or along a path.
  int numDiag = 0;
  if (Grid::getLeft(cache))
  {
    if(Grid::getLeft(cache)->top && (GET_PHERAMONE( Grid::getLeft(cache)->top, currentPheramone ) >= HOT_TRAIL_LIMIT))
      numDiag++;
    if(Grid::getLeft(cache)->bottom && (GET_PHERAMONE( Grid::getLeft(cache)->bottom, currentPheramone ) >= HOT_TRAIL_LIMIT))
      numDiag++;
  }
  if (Grid::getRight(cache))
  {
    if(Grid::getRight(cache)->top && (GET_PHERAMONE( Grid::getRight(cache)->top, currentPheramone ) >= HOT_TRAIL_LIMIT))
      numDiag++;
    if(Grid::getRight(cache)->bottom && (GET_PHERAMONE( Grid::getRight(cache)->bottom, currentPheramone ) >= HOT_TRAIL_LIMIT))
      numDiag++;
  }

  // now we have the variables:
  //  *number new directions.
  //  *number hot directions.
  //  *number hot diagonals.
  //  *"wayToHome" variables.


  return false;
}

// This way is cheating, because it uses a memory of where the entrance is.
void Ant::goHomeCheating()
{
  // Cheating:
  int x_dist, y_dist;
  bool up, right;

  wayToHome(x_dist, y_dist, right, up);

  // don't need to check that the other != 0, because if its 0,0 the portal will be taken.
  if (y_dist == 0)
  {
    if (right)
      direction=1;
    else
      direction=2;
  }
  else if (x_dist == 0)
  {
    if (up)
      direction=3;
    else
      direction=0;
  }
  // need to move in both directions, pick one at random.
  else
  {
    int dir = rand()%2;
    if (dir == 0)
    {
      if (up)
        direction=3;
      else
        direction=0;
    }
    else
    {
      if (right)
        direction=1;
      else
        direction=2;
    }
  }

  // done with AI, do it to it.
  setAI(false);
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
    for(i = j - 1; (i >= 0) && ( GET_PHERAMONE( adjacent[i], currentPheramone ) < GET_PHERAMONE( key, currentPheramone )); i--)
    {
      adjacent[i+1] = adjacent[i];
      direction[i+1] = direction[i];
    }
    adjacent[i+1] = key;
    direction[i+1] = dirKey;
  }
}

bool Ant::awayFromHome(int dir)
{
  int x_dist, y_dist;
  bool right, up;
  wayToHome(x_dist, y_dist, right, up);

//  if ((x_dist < 5) || (y_dist < 5))
//    return true;

  // if Walking away in X dir:
  if ( ((dir == AI_RIGHT) && !right) || ((dir == AI_LEFT) && right) ||
  ((dir == AI_TOP) && !up) || ((dir == AI_DOWN) && up) )
    return true;
  return false;
}

bool Ant::wayToHome(int &x_dist, int &y_dist, bool &right, bool &up)
{
  Patch* cache = getPatch();

  // left or right to get there?
  // find X distance / direction.
  if (cache->x > lastPortal->x)
  {
    x_dist = cache->x - lastPortal->x;
    right = true;
    if ( x_dist > ((lastPortal->x + WIDTH) - cache->x ))
    {
      x_dist = ((lastPortal->x + WIDTH) - cache->x );
      right = false;
    }
  }
  else
  {
    x_dist = lastPortal->x - cache->x;
    right = false;
    if ( x_dist > ((cache->x + WIDTH) - lastPortal->x ))
    {
      x_dist = ((cache->x + WIDTH) - lastPortal->x );
      right = true;
    }
  }

  // find Y distance / direction.
  if (cache->y > lastPortal->y)
  {
    y_dist = cache->y - lastPortal->y;
    up = true;
    if ( y_dist > ((lastPortal->y + DEPTH) - cache->y ))
    {
      y_dist = ((lastPortal->y + DEPTH) - cache->y );
      up = false;
    }
  }
  else
  {
    y_dist = lastPortal->y - cache->y;
    up = false;
    if ( y_dist > ((cache->y + DEPTH) - lastPortal->y ))
    {
      y_dist = ((cache->y + DEPTH) - lastPortal->y );
      up = true;
    }
  }

  // I think the y-axis was originally for depth, so invert this.
  //up = !up;
  right = !right;

  return true;
}
