#include "Ant.h"

// Feramone's "splash"
void Ant::handleFeramone()
{
  Patch* cur = getPatch();
//  INCREASE_FERAMONE(getPatch(), feramoneOutput);
//  // only update feramone if it will be to more than it already is.
//  if (cur->chemLevel < feramoneOutput)
//    INCREASE_FERAMONE(cur, feramoneOutput);
//    INCREASE_FERAMONE(cur, feramoneOutput);

/*
  // if the ant is not hot, don't increase tile too much.
  // and don't do splash.
  if ((feramoneOutput == FERAMONE_LOW) && (cur->chemLevel < (HOT_TRAIL_LIMIT / 2)))
  {
    INCREASE_FERAMONE(cur, feramoneOutput);
    return;
  }
  // ant is hot, don't increase tile too much either.
  else if ((feramoneOutput == FERAMONE_HIGH) && (cur->chemLevel > (HOT_TRAIL_LIMIT * 4)))
    INCREASE_FERAMONE(cur, feramoneOutput);
*/

  // set a max of 5000
  INCREASE_FERAMONE_LIMIT(cur, feramoneOutput, 5000);

  // chemical "splash" to each side as ant moves:
  // * = ants trail
  // - = chemical "splash" once
  // + = chemical "splash" twice
  //  | |-|+|*|-|
  //  |-|*|*|*|-|
  //  |-|*|+|-| |

  int splash = feramoneOutput * 0.5;
  Patch* last = lastVisited(1);

  // can't do this if the ant has no memory (i.e. just spawned).
  if (last == '\0') return;

  int lastDir;
  if (cur->left == last) lastDir = AI_LEFT;
  else if (cur->right == last) lastDir = AI_RIGHT;
  else if (cur->top == last) lastDir = AI_TOP;
  else if (cur->bottom == last) lastDir = AI_DOWN;

  if (lastDir == AI_LEFT)
  {
    INCREASE_FERAMONE_LIMIT(last->top, splash, 5000);
    INCREASE_FERAMONE_LIMIT(last->bottom, splash, 5000);
  }
  else if (lastDir == AI_RIGHT)
  {
    INCREASE_FERAMONE_LIMIT(last->top, splash, 5000);
    INCREASE_FERAMONE_LIMIT(last->bottom, splash, 5000);
  }
  else if (lastDir == AI_TOP)
  {
    INCREASE_FERAMONE_LIMIT(last->left, splash, 5000);
    INCREASE_FERAMONE_LIMIT(last->right, splash, 5000);
  }
  else if (lastDir == AI_DOWN)
  {
    INCREASE_FERAMONE_LIMIT(last->left, splash, 5000);
    INCREASE_FERAMONE_LIMIT(last->right, splash, 5000);
  }
/*
  // If it isn't the current, next node (cur) or last node (last2) splash it.
  if ((last->top!=last2) && (last->top!=cur) && (last->top->chemLevel < 5000))
    INCREASE_FERAMONE(last->top, splash);
  if ((last->bottom!=last2) && (last->bottom!=cur) && (last->bottom->chemLevel < 5000))
    INCREASE_FERAMONE(last->bottom, splash);
  if ((last->left!=last2) && (last->left!=cur) && (last->left->chemLevel < 5000))
    INCREASE_FERAMONE(last->left, splash);
  if ((last->right!=last2) && (last->right!=cur) && (last->right->chemLevel < 5000))
    INCREASE_FERAMONE(last->right, splash);
*/
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
// 7.5.   If following a HOT trail, don't turn unless must turn.
// 8.   If on surface and no HOT trail, go to a spot with no feramone
// 9.     Move to spot with no feramone, Mark feramone (output = 100)
// 10.  If no spot with no feramone, wander

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
    // Found food, pick it up.
    if (cache != '\0')
    {
      // 4. If food, pickup, set feramone output = 1000:
      // 5.   Pickup food, mark food spot with feramone, go home
      pickup( cache );
      set_portaled( false );
      feramoneOutput = FERAMONE_HIGH;

      // set feramone (with new output) where standing.
      handleFeramone();
      // set feramone at food (so ants will move there once the
      // pile is gone).
      INCREASE_FERAMONE( cache, feramoneOutput+1);
      takePortals = true;

      // TODO: clear memory or follow it?
      // finally, we're turning around, so reset the ants memory.
      clearVisited();
      // go Home.
      goHome();
      //wander();
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
  //7.5: If following a HOT trail, don't turn unless must turn.
  // if there is a trail to follow, all is handled.
  if (followTrail(sort, dir, false))
  {
    printf("f");
    return;
  }
  // else... need to follow a cold trail


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
//  Patch* cache = findPortalAdjacent();
//  if (cache != '\0')
//  {
//    setPatch(cache);
//    handlePortal();
//  }

//  if (followTrail())
//    return;
//  else
    // if no hot trail...
  goHomeCheating();
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

// 1. Near Home:
// 1.away - take random hot path, try not to turn
// 1.home - go home.
// 2. if we are far from home, try to head away from home

//
bool Ant::followTrail(Patch* sort[], int dir[], bool home)
{

  // exit early if there is no trail to follow.
  if (sort[0]->chemLevel < HOT_TRAIL_LIMIT) return false;

/*
  // 1. If following a HOT trail, don't turn unless must turn.
  // check if the direction the ant is already
  // heading is HOT and the direction is OK.
  int lastDir = reverseDirection( directionOld );
  for(int i=0; i<4; i++)
    if((dir[i] == lastDir) && ( sort[i]->chemLevel >= HOT_TRAIL_LIMIT ))
      {
        // If the direction we're heading in is "warm" compared to an alternative.. take a hotter one.
        if (i>0)
          // TODO: if there is a good reason to turn, turn.
          if ( true )//(sort[i]->chemLevel) > sort[0]->chemLevel)
          {
            direction = dir[i];
            setAI(false);
            return true;
          }
      }
*/

/*
  // 7.   If on surface and HOT trail, follow (hot == feramone > 100)
  for (int i=0; i< 4 && (sort[i]->chemLevel >= HOT_TRAIL_LIMIT); i++)
  {
    // check if the node has already been visited and is facing away from home.
    if (!checkVisited(sort[i]) && awayFromHome(dir[i]))
    {
      direction = dir[i];
      setAI(false);
      return true;
    }
  }
*/

  int x_dist, y_dist;
  bool up, right;
  wayToHome(x_dist, y_dist, right, up);
  
/*
  // 1. Near Home
  if ((x_dist < 5) && (y_dist < 5))
  {
    if (home)
    {
      
      return;
    }
    else
    {

    }
  }
*/
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
  if (cache->x > savedEntrance->x)
  {
    x_dist = cache->x - savedEntrance->x;
    right = true;
    if ( x_dist > ((savedEntrance->x + WIDTH) - cache->x ))
    {
      x_dist = ((savedEntrance->x + WIDTH) - cache->x );
      right = false;
    }
  }
  else
  {
    x_dist = savedEntrance->x - cache->x;
    right = false;
    if ( x_dist > ((cache->x + WIDTH) - savedEntrance->x ))
    {
      x_dist = ((cache->x + WIDTH) - savedEntrance->x );
      right = true;
    }
  }

  // find Y distance / direction.
  if (cache->y > savedEntrance->y)
  {
    y_dist = cache->y - savedEntrance->y;
    up = true;
    if ( y_dist > ((savedEntrance->y + DEPTH) - cache->y ))
    {
      y_dist = ((savedEntrance->y + DEPTH) - cache->y );
      up = false;
    }
  }
  else
  {
    y_dist = savedEntrance->y - cache->y;
    up = false;
    if ( y_dist > ((cache->y + DEPTH) - savedEntrance->y ))
    {
      y_dist = ((cache->y + DEPTH) - savedEntrance->y );
      up = true;
    }
  }

  // I think the y-axis was originally for depth, so invert this.
  //up = !up;
  right = !right;

  return true;
}
