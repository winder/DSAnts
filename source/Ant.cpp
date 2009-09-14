#include "Ant.h"

// Feramone's "splash"
void Ant::handleFeramone()
{
  Patch* cur = getPatch();

  // if chemLevel is very high, don't put down too much
  if ((feramoneOutput > FERAMONE_LOW) && cur->chemLevel > FERAMONE_MAX-feramoneOutput)
    SET_FERAMONE(cur, FERAMONE_MAX);
  else if (feramoneOutput > FERAMONE_LOW)
    INCREASE_FERAMONE(cur, feramoneOutput);
  // if feramone output is low, and tile level is low, set.
  else if ((feramoneOutput == FERAMONE_LOW) && (cur->chemLevel <= COLD_TRAIL))
    SET_FERAMONE(cur, feramoneOutput);

  // no splash, it seems to confuse things more than it helps.
  return;
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
        feramoneOutput = FERAMONE_HIGH;

        // set feramone (with new output) where standing.
        handleFeramone();

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
  if (sort[3]->chemLevel < COLD_TRAIL)
  {
    cold_dirs++;
    if (sort[2]->chemLevel < COLD_TRAIL)
    {
      cold_dirs++;
      if (sort[1]->chemLevel < COLD_TRAIL)
      {
        cold_dirs++;
        if (sort[0]->chemLevel < COLD_TRAIL)
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
// solution: no trail to follow, return false


// Problem: Got a shape like the following and the ants follow around the perimiter forever.
// | | | | | | |
// | | | |=|=| |
// | |=|=|=|=| |
// | |=|=|=|=| |
// | | | | | | |
//
// Generalized: they have trouble getting off the trail when they need to
//
// Solution: if there are 3 or more ways, figure out if we're at a crossroad,
//           along the edge, or in the middle of everything:
// crossroad: | |=| | edge:| |=|=| | middle:|=|=|=| corner: | |=|=| |
//            |=|0|=|      | |=|0| |        |=|0|=| (edge)  |=|0|=| |
//            | |.| |      | |=|.| |        |=|.|=|         |=|=|0| |
//
//  crossroads work great.
//  edges need to be treated as a single path (ignore left in above)
//  middle needs to move in the direction they're heading and find an edge


bool Ant::followTrail(Patch* sort[], int dir[], bool home)
{
  // 1. If there is no trail to follow, return false.
  if (sort[0]->chemLevel < HOT_TRAIL_LIMIT) return false;

  // Figure out how many trails are hot, and how many of those are new.
  int numHot = 0;
  int numNew = 0;
  for(int i=0; i<4; i++)
    if (sort[i]->chemLevel >= HOT_TRAIL_LIMIT)
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
    if(Grid::getLeft(cache)->top && (Grid::getLeft(cache)->top->chemLevel >= HOT_TRAIL_LIMIT))
      numDiag++;
    if(Grid::getLeft(cache)->bottom && (Grid::getLeft(cache)->bottom->chemLevel >= HOT_TRAIL_LIMIT))
      numDiag++;
  }
  if (Grid::getRight(cache))
  {
    if(Grid::getRight(cache)->top && (Grid::getRight(cache)->top->chemLevel >= HOT_TRAIL_LIMIT))
      numDiag++;
    if(Grid::getRight(cache)->bottom && (Grid::getRight(cache)->bottom->chemLevel >= HOT_TRAIL_LIMIT))
      numDiag++;
  }

  // now we have the variables:
  //  *number new directions.
  //  *number hot directions.
  //  *"wayToHome" variables.


  // Situation 0: on a simple trail, stay on it.
  if ((numHot == 2) && (numNew == 1))
  {
    if (checkVisited(sort[0]))
      direction = dir[1];
    else
      direction = dir[0];

    setAI(false);
    return true;
  }


  // Situation 6: 0 new ways to go, 1 old way.
  // solution: nothing to follow, let the caller do its thing
  if ((numHot == 1) && (numNew == 0))
  {
    return false;
  }

// Problem: Got a shape like the following and the ants follow around the perimiter forever.
// | | | | | | |
// | | | |=|=| |
// | |=|=|=|=| |
// | |=|=|=|=| |
// | | | | | | |
//
// Generalized: they have trouble getting off the trail when they need to
//
// Solution: if there are 3 or more ways, figure out if we're at a crossroad,
//           along the edge, or in the middle of everything:
// crossroad: | |=| | edge:| |=|=| | middle:|=|=|=| corner: | |=|=| |
//            |=|0|=|      | |=|0| |        |=|0|=| (edge)  |=|0|=| |
//            | |.| |      | |=|.| |        |=|.|=|         |=|=|0| |
//                                                          | | | | |
//  crossroads work great.
//  edges need to be treated as a single path (ignore left in above)
//  middle needs to move in the direction they're heading and find an edge


  //-----------------------------//
  // FIGURE OUT IF ON EDGE TRAIL //
  //   if not, prune till it is  //
  //-----------------------------//
/*
  // Edge, want to continue forward.
  if ((numDiag == 2) && (numHot == 3) && (numNew == 2))
  {
    bool ud_edge = false;
    bool lr_edge = false;
    // Up-Down edge
    if ( ((Grid::getUpRight(cache)->chemLevel > HOT_TRAIL_LIMIT) && (Grid::getDownRight(cache)->chemLevel > HOT_TRAIL_LIMIT)) ||
         ((Grid::getUpLeft(cache)->chemLevel > HOT_TRAIL_LIMIT)  && (Grid::getDownLeft(cache)->chemLevel > HOT_TRAIL_LIMIT)) )
      ud_edge=true;

    // Left-Right edge
    else if ( ((Grid::getUpRight(cache)->chemLevel > HOT_TRAIL_LIMIT)   && (Grid::getUpLeft(cache)->chemLevel > HOT_TRAIL_LIMIT)) ||
              ((Grid::getDownRight(cache)->chemLevel > HOT_TRAIL_LIMIT) && (Grid::getDownLeft(cache)->chemLevel > HOT_TRAIL_LIMIT)) )
      lr_edge=true;

    // if neither of those, moving diagonal
    // | |=|=|
    // | |0| |
    // |=|=| |

    if (lr_edge || ud_edge)
    {
      // we're removing *one* hot value, so when we find it, just swap it with the last one.
      for(int i=0; (i < (numHot-1)) && (lr_edge || ud_edge); i++)
      {
        if (lr_edge && ((dir[i] == AI_TOP) || (dir[i] == AI_DOWN)))
        {
          sort[i] = sort[2];
          dir[i] = dir[2];
          lr_edge = false;
        }
        else if (ud_edge && ((dir[i] == AI_RIGHT) || (dir[i] == AI_DOWN)))
        {
          sort[i] = sort[2];
          dir[i] = dir[2];
          ud_edge = false;
        }
      }
    
      printf("PRUNED");
      sort[2] = '\0';
      numHot-=1;
      numNew-=1;
    }
  }
*/
/*
  // remove prior direction (stick it at end):
  for(int i=0; i<3; i++)
    // swap with next
    if (dir[i] == directionOld)
    {
      cache=sort[i];
      next=dir[i];

      sort[i] = sort[i+1];
      dir[i] = dir[i+1];

      sort[i+1] = cache;
      dir[i+1] = next;
    }
*/      
  next = -1;
  cache = getPatch();

/*
  // TODO: does it make sense to do this again?
  // do this again in case pruning was involved.
  // Situation 0: not on a trail, but see one
  if ((numHot == 2) && (numNew == 1))
  {
    direction = dir[0];
    setAI(false);
    return true;
  }
*/

  //---------------------------//
  // BELOW, SINGLE LINE STUFF: // (know a single line because "numDiag <= 1"
  //---------------------------//
  // i.e. | | |=|=|
  //      | | |=| |
  //      |=|=|=| |
  //      |=| | | |

  // Situation 1: the ant has 3 new directions to move in, and 4 hot ways
  // Situation 2: 2 new ways to go, 3 hot ways
  //    solution: take hottest trail without turning around, the way you *were* going should
  //              be opposite the way your heading so don't need to consider 'bool home'

  // Situation 3: 1 new ways to go, 2 hot ways.
  //    solution: take new way (followHotNotVisited will pick the new way).

  if ((numHot >= 2) && (numNew >= 1) && (numDiag !=4))
  {
    int numEqual = 1;
    next = followHotNotVisited(sort);

    // If they are equal, head towards 'bool home'
    for(int i=0; i<=numNew-1; i++)
      if (sort[i]->chemLevel == sort[i+1]->chemLevel)
        numEqual++;
      
    if ((next != -1) && (numEqual < 2))
    {
      direction = dir[next];
      setAI(false);
      return true;
    }
  }

  // Situation 5: 0 new ways to go, 2 hot ways to go.
  // solution: don't turn around.  in above, goes right.
  if ((numHot == 2) && (numNew == 0))
  {
//printf("shouldn't be here");
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

  //----------------------------------------------//
  // Finally: if all else fails,                  //
  //          take the direction that heads home. //
  //----------------------------------------------//

  int x_dist, y_dist;
  bool up, right;
  wayToHome(x_dist, y_dist, right, up);
  // goHome, home = true, right & up = direction home is in
  // otherwise, opposite of that.


  bool dirX[numHot];
  int num = 0;
  for(int i=0; i<numHot; i++)
  {
    if (((dir[i] == AI_RIGHT) &&  right) || ((dir[i] == AI_TOP)  &&  up) ||
        ((dir[i] == AI_LEFT)  && !right) || ((dir[i] == AI_DOWN) && !up))
    {
      // don't want to turn around (!home because it will be inverted for normal cases):
      if ((dir[i] != directionOld) || !home)
      {
        dirX[i] = true;
        num++;
      }
    }
  }
  // if not going home, invert:
  if (!home)
  {
    // invert for each direction
    for(int i=0; i<numHot; i++)
      dirX[i] = !dirX[i];
    // invert count
    num = numHot - num;
  }

  // one choice, take it.
  if (num == 1)
  {
    for(int i=0; i<numHot; i++)
      if(dirX[i])
      {
        direction=dir[i];
        setAI(false);
        return true;
      }
  }
  // multiple choices, pick one at random
  else
  {
    int choice=rand()%num;
    for(int i=0; i<numHot; i++)
    {
      if (dirX[i] && (choice!=0))
        choice--;
      else if (dirX[i] && (choice==0))
      {
        direction=dir[i];
        setAI(false);
        return true;
      }
    }
  }

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
    for(i = j - 1; (i >= 0) && (adjacent[i]->chemLevel < key->chemLevel); i--)
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
