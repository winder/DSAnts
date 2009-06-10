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

  // if chemLevel is very high, don't put down too much
  if (cur->chemLevel > FERAMONE_MAX-feramoneOutput)
    SET_FERAMONE(cur, FERAMONE_MAX);
  else
    INCREASE_FERAMONE(cur, feramoneOutput);
return;
  // chemical "splash" to each side as ant moves:
  // * = ants trail
  // - = chemical "splash" once
  // + = chemical "splash" twice
  //  | |-|+|*|-|
  //  |-|*|*|*|-|
  //  |-|*|+|-| |

  int splash = feramoneOutput * 0.3;
  int splashLow = feramoneOutput * 0.1;
  Patch* last = lastVisited(1);

  // can't do this if the ant has no memory (i.e. just spawned).
  if (last == '\0') return;

  int lastDir = getLastDirection();

  // if LEFT or RIGHT, mark top and bottom of last.
  if ((lastDir == AI_LEFT) || (lastDir == AI_RIGHT))
  {
    if ((last->top != '\0') && (last->top->chemLevel < 5000))
      INCREASE_FERAMONE(last->top, splash);
    else if (last->top->chemLevel < 10000)
      INCREASE_FERAMONE(last->top, splashLow);

    if ((last->bottom != '\0') && (last->bottom->chemLevel < 5000))
      INCREASE_FERAMONE(last->bottom, splash);
    else if (last->left->chemLevel < 10000)
      INCREASE_FERAMONE(last->bottom, splashLow);
  }
  // if TOP or BOTTOM, mark left and right of last
  else if ((lastDir == AI_TOP) || (lastDir == AI_DOWN))
  {
    if ((last->left != '\0') && (last->left->chemLevel < 5000))
      INCREASE_FERAMONE(last->left, splash);
    else if (last->left->chemLevel < 10000)
      INCREASE_FERAMONE(last->left, splashLow);

    if ((last->right != '\0') && (last->right->chemLevel < 5000))
      INCREASE_FERAMONE(last->right, splash);
    else if (last->right->chemLevel < 10000)
      INCREASE_FERAMONE(last->right, splashLow);
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
      // "follow trail" towards home.
      followTrail(true);
      //goHome();
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
      // Note: this might not work, since it will create a big block of
      //        high if there are adjacent food pieces.
//      INCREASE_FERAMONE( cache, feramoneOutput+1);
      takePortals = true;

      // TODO: clear memory or follow it?
      // finally, we're turning around, so reset the ants memory.
      clearVisited();

      // go Home.
      //goHome();
      //wander();

      // turn around!!
      direction = directionOld;
      setAI(false);
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

  // followTrail now calls goHome, so goHome is goHomeCheating
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

// TODO: Implement all situations, the "followTrail" algorithm turns out to be the most complex!

// Drew out a new algorithm... there are a lot of cases that need to be handled so here it goes:
// Variables to keep in mind:
//    Turning?  does the ant need to turn.  Binary (yes/no)
//    Multiple choices?  how many directions can the ant move in? 1-3, ant never turns around unless necessary

// So there are 8 cases. Using these, but I decided on 5 important ones to handle specific situations.
//    One other thing to consider is the ants "memory"
//      Which brings it up to much more than 8 cases, but these may be generalized somehow.
//
// Key:
//  *, ant hill
//  =, high chemical level
//  -, medium chemical level
//  0, ant
//  ., ants last location, 2 dots so I can use the same pic for 'TO_HOME' and 'FROM_HOME'
//  new, keyword, means square is not in memory.
//
// Situation 1: the ant has 3 new directions to move in, and 4 hot ways
//
// | | | | | | | |=|
// | | | | | | | |=|
// | | |=|.|=|=|=|=|
// | | |=|0|=|=| | |
// | | | |.| | | | |
// | | | |*| | | | |
// | | | | | | | | |
//
//  solution: take hottest trail without turning around, the way you *were* going should
//            be opposite the way your heading so don't need to consider 'bool home'
//
//  possible problem: loop?  If we take the hottest trail, will hopefully be more than the rest.
//
//  |=|=|=|=|
//  |=|=|=|=|
//  |=|=|=|=|
//  |=|=|=|=|
//

// Situation 2: 2 new ways to go, 3 hot ways
//
// | | | | | | | |=|
// | | | | | | | |=|
// | | |=|0|.|=|=|=|
// | | |=|.|=|=| | |
// | | | |=| | | | |
// | | | |*| | | | |
// | | | | | | | | |
//
//  solution: take hottest trail
//  possible problem: in above, the ant goes left, that shouldn't happen because
//                    that direction wont go anywhere, but using memory in dead end
//                    scenarios should get it out of it.

// Situation 2: 2 new ways to go, 3 hot ways, actually this is covered above.
//
// | | | | | | | |=|
// | | | | | | | |=|
// | | |=|.|0|.|=|=|
// | | |=|=|=|=| | |
// | | | |=| | | | |
// | | | |*| | | | |
// | | | | | | | | |

// Situation 3: 1 new ways to go, 2 hot ways.
//
// | | | | | | | |=|
// | | | | | | | |=|
// | | |=|=|=|.|0|.|
// | | |=|=|=|=| | |
// | | | |=| | | | |
// | | | |*| | | | |
// | | | | | | | | |
//
// solution: take new way.
// possible problem: dead end?  See Situation 7
//

// Situation 3: 1 new way, 2 hot ways.
//
// | | | | | | | |=|
// | | |0|.|=|=|=|=|
// | | |=|=|=|=| | |
// | | | |=| | | | |
//
// solution: see above, memory will make the ant go down, right, right:
// | | | | | | | |=|
// | | |.|.|=|=|=|=|
// | | |.|0|=|=| | |
// | | | |.| | | | |

// Situation 4: 2 new ways to go, 2 hot ways.
//
// | | | | | | | |
// |=|=|=|=|0|=|*|
// | | | | |.| | |
//
// solution: head towards 'bool home'.
//           this is the one case where we need to use the "cheat"
//           but I will attribute this to "talking" since ant can check to see
//           what direction other nearby ants are going and what their goals are.
// problem: neither way is toward home, or neither way is away from home.
//
// | | | | | |     | |*| | |=| |
// |*| |.|0|=|     | | | | |=| |
// | | | |=| |     | | |=|=|0|.|
//
// solution: head random way in this case.


// Situation 5: 0 new ways to go, 2 hot ways to go.
//
// | | | | | | | |=|
// | | |.|.|=|=|=|=|
// | | |0|.|=|=| | |
// | | | |.| | | | |
//
// solution: don't turn around.  in above, goes right.

// Situation 6: 0 new ways to go, 1 old way.
// | | | | | | | |
// |=|=|=|.|0| |*|
// | | | | | | | |
//
// solution: call "goHome"
bool Ant::followTrail(Patch* sort[], int dir[], bool home)
{

  // 1. If there is no trail to follow, return false.
  if (sort[0]->chemLevel < HOT_TRAIL_LIMIT) return false;

  int next = -1;

  // We are only interested in the hot trails, get rid of the rest
  int numHot = 0;
  for(int i=0; i<4; i++)
    if (sort[i]->chemLevel >= HOT_TRAIL_LIMIT)
      numHot++;
    else
      sort[i] = '\0';

  int numNew = 0;
  for(int i=numHot; i>=0; i--)
    if (! (checkVisited(sort[i])) )
      numNew++;

  // now we have the variables:
  //  *number new directions.
  //  *number hot directions.
  //  *"wayToHome" variables.

  // Situation 1: the ant has 3 new directions to move in, and 4 hot ways
  // Situation 2: 2 new ways to go, 3 hot ways
  //    solution: take hottest trail without turning around, the way you *were* going should
  //              be opposite the way your heading so don't need to consider 'bool home'

  // Situation 3: 1 new ways to go, 2 hot ways.
  //    solution: take new way (followHotNotVisited will pick the new way).
  if ((numHot >= 2) && (numNew >= 1))
  {
    next = followHotNotVisited(sort);
    if (next != -1)
    {
      direction = dir[next];
      setAI(false);
      return true;
    }
    else
    {
      //TODO: error?
    }
  }

  // Situation 5: 0 new ways to go, 2 hot ways to go.
  // solution: don't turn around.  in above, goes right.
  if ((numHot == 2) && (numNew == 0))
  {
    if (sort[0] == lastVisited(1))
    {
      direction=dir[1];
      setAI(false);
      return true;
    }
    else if (sort[1] == lastVisited(1))
    {
      direction=dir[0];
      setAI(false);
      return true;
    }
  }

  // Situation 6: 0 new ways to go, 1 old way.
  // solution: call "goHome"
  if (numHot <= 1)
  {
printf("gh");
    if (home)
      goHome();
    else
      wander();
    return true;
  }
  

  int x_dist, y_dist;
  bool up, right;
  wayToHome(x_dist, y_dist, right, up);
  // goHome, home = true, right & up = direction home is in
  // otherwise, opposite of that.

  // Note: this comes last because it is the most uncommon and most costly.
  // Situation 4: 2 new ways to go, 2 hot ways.
  // solution: head towards 'bool home'.
  if ((numHot == 2) && (numNew == 2))
  {
    // cases:                                      | |.| |
    // |.|0|=|   | |=| |    | |=| |    |=|0|.|     |=|0|=|
    // | |=| |   |.|0|=|    |=|0|.|    | |=| |     | |.| |

    bool dir0 = false;
    bool dir1 = false;

    // if dir[0] is in the right direction to go home
    if (((dir[0] == AI_RIGHT) &&  right) || ((dir[0] == AI_TOP)  &&  up) ||
        ((dir[0] == AI_LEFT)  && !right) || ((dir[0] == AI_DOWN) && !up))
      dir0 = true;

    // if dir[1] is in the right direction to go home
    if (((dir[1] == AI_RIGHT) &&  right) || ((dir[1] == AI_TOP)  &&  up) ||
        ((dir[1] == AI_LEFT)  && !right) || ((dir[1] == AI_DOWN) && !up))
      dir1 = true;

    // if we're not going home, invert dir0 and dir1
    if (!home)
    {
      dir0 = !dir0;
      dir1 = !dir1;
    }

    // pick the direction we're heading in.
    if (dir0 && !dir1)
    {
      direction=dir[0];
      setAI(false);
      return true;
    }
    else if (dir1 && !dir0)
    {
      direction=dir[1];
      setAI(false);
      return true;
    }
    // Note: if both are in the right direction, or neither are, pick at random
    // TODO: might want to handle this differently
    //else if (dir1 && dir0)
    else
    {
      direction=dir[rand()%2];
      setAI(false);
      return true;
    }
  }

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
