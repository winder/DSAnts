#include "Creature.h"
// Include this here to avoid circular dependencies
#include "GameWorld.h"

Creature::Creature()
{
  offsetX = 0;
  offsetY = 0;
  directionX = 0;
  directionY = 20;
  portaled = false;
  failCount = 0;
//  carrying = NOTHING;
  carrying = 5;
  ai = true;
  ACTION=0;
  direction=-1;
  directionOld=-1;
  takePortals=false;
  ACTION = ANT_ACTION_FORAGE;

  hp = 1000;

  visited_index = 0;
   use_visit_memory = true;
}

Creature::Creature(Patch* pat, int loc)
{
  location = loc;
  offsetX = 0;
  offsetY = 0;
  directionX = 0;
  directionY = 20;
  // what if pat already has 2 ants?  ant pile on pat until they move outta the way I guess.
  p=pat;
  portaled = false;
  failCount = 0;
  carrying = NOTHING;
  ai = true;
  ACTION = ANT_ACTION_FORAGE;
  direction=-1;
  directionOld=-1;
  takePortals=false;

  hp = 1000;

  visited_index = 0;
  use_visit_memory = true;
}

Creature::~Creature()
{
  REMOVE_SPOT(p, this);
}

void Creature::pickup(Patch *p)
{
  if (carrying == NOTHING)
    carrying = GameWorldSingleton::getInstance()->pickup(location, p);
}

bool Creature::drop(Patch *p)
{
  if( GameWorldSingleton::getInstance()->drop(location, p, carrying) )
  {
    carrying = NOTHING;
    return true;
  }
  return false;
}

// Checks if there is a portal.  If there is, go through.
bool Creature::handlePortal()
{
  if (getTakePortals())
  {
    // the portaled flag is supposed to stop it from flipping back and forth over and over again.
    if (!p->portal)
    {
      portaled = false;
      return false;
    }
    if (!portaled && p->portal && WALKABLE(p->portal))
    {
      // Force your way through the portal.
      if (!moveTo(p->portal, true))
      {
        portaled = false;
        wentThroughPortal();
        return true;
      }
      offsetX=0;
      offsetY=0;
      portaled = true;
      location = p->location;
      wentThroughPortal();
      return true;
    }
  }
  return false;
}

// Called if we know there is a collision.
// 1) if we haven't waited for WALK_RETRY_NUM, wait
// 2) if we have waited, reset fail counter and re-run AI.
bool Creature::handleCollision()
{
  if(failCount < WALK_RETRY_NUM)
  {
    failCount++;
    // do not re-run AI, do not move ant.  Try again.
    return false;
  }
  else
  {
    return true;
  }
}

// True if there is a collision.
bool Creature::checkCollision(Patch* pat)
{
  return !(AVAILABLE_SPOT(pat));
}

// TODO: might want to have this always return true so the ant will turn around
// if there is a barricade.
// Checks whether can move to the new patch (not full) and moves there.
bool Creature::moveTo(Patch *pat, bool force)
{

  bool t = false;

  // if forcing, don't check if available spot.
  if (force)
  {
    REMOVE_SPOT(p, this);
    t = true;
    p = pat;
    handleFeramone();
  }
  // This is what it looks like with macro's
  else if (AVAILABLE_SPOT(pat))
  {
    t = true;
  }

  // Check if we're doing the move.  If yes, do it.
  if (t)
  {
    REMOVE_SPOT(p, this);
    SET_SPOT(pat, this);

    t = true;
    // move old spot to new spot.
    p = pat;

    // even if we're not using the visited value, keep it updated
    addVisited( pat );

    // if the ant successfully moved to a new patch.... leave a mark.
    handleFeramone();
  }

  return t;
}

// Only ants do this.  Leave a stub in parent class.
void Creature::handleFeramone()
{

}

// This is the "simple" way, only move up/down if X is 0, or right/left if Y is 0.
// center the offending piece if they arne't 0.
// I added a couple tweaks:
//    -if the piece is half way between two squares it moves to the next one
//    -if the piece is going the opposite direction its been heading (i.e. a little
//        left then right) then there is no checking.
//        *this piece adds some jitter I think.

bool Creature::moveRight()
{
  if (handlePortal()) return true;

  Patch* next = Grid::getRight(p);

  if (checkCollision(next))
    return handleCollision();
  else
    failCount=0;

  // don't move right if the ant is in the right spot isn't empty.
  if ((offsetX >= 0) && !WALKABLE(p->right)) return true;

  // if Y is off center, the way is not empty, and we're well on our way to the next
  // spot, move towards center Y.
  if       ((offsetX >= 0) && (offsetY > 0) && (p->right && !WALKABLE(p->right->top)))     decrementOffsetY();
  else if ((offsetX >= 0) && (offsetY < 0) && (p->right && !WALKABLE(p->right->bottom)))  incrementOffsetY();
  else
    incrementOffsetX();

  // if we are half way towards the next way, swap to the next one.
  if (offsetX >= (ANIMATION_SIZE / 2))
  {
    // Check if there is an empty spot
    if (moveTo(next))
    {
      offsetX=-1 * (ANIMATION_SIZE / 2);
      return true;
    }
  }
  return false;
}

bool Creature::moveLeft()
{
  if (handlePortal()) return true;

  Patch* next = Grid::getLeft(p);
  if (checkCollision(next))
    return handleCollision();
  else
    failCount=0;

  // if able to move right...
  if ((offsetX <= 0) && !WALKABLE(p->left)) return true;

  // only move left if Y is centered.
  if      ((offsetX <= 0) && (offsetY > 0) && (p->left && !WALKABLE(p->left->top)))     decrementOffsetY();
  else if ((offsetX <= 0) && (offsetY < 0) && (p->left && !WALKABLE(p->left->bottom))) incrementOffsetY();
  else
    decrementOffsetX();
  if (offsetX <= (-1 * (ANIMATION_SIZE / 2)))
  {
    // Check if there is an empty spot
    if (moveTo(next))//checkCollision(next))
    {
      offsetX=(ANIMATION_SIZE / 2);
      return true;
    }
  }
  return false;
}

bool Creature::moveUp()
{
  if (handlePortal()) return true;

  Patch* next = Grid::getUp(p);
  if (checkCollision(next))
    return handleCollision();
  else
    failCount=0;

  if ((offsetY >= 0) && !WALKABLE(p->top)) return true;

  // Prevent player from going in a weird direction if that way is blocked.
  if       ((offsetY >= 0) && (offsetX > 0) && (p->top && !WALKABLE(p->top->right)))  decrementOffsetX();
  else if ((offsetY >= 0) && (offsetX < 0) && (p->top && !WALKABLE(p->top->left)))  incrementOffsetX();
  else
    incrementOffsetY();
  if (offsetY >= (ANIMATION_SIZE / 2))
  {
    // Check if there is an empty spot
    if (moveTo(next))//checkCollision(next))
    {
      offsetY=-1 * (ANIMATION_SIZE / 2);
      return true;
    }
  }
  return false;
}

bool Creature::moveDown()
{
  if (handlePortal()) return true;

  Patch* next = Grid::getDown(p);
  if (checkCollision(next))
    return handleCollision();
  else
    failCount=0;


  if ((offsetY <= 0) && !WALKABLE(p->bottom)) return true;

  // Prevent player from going in a weird direction if that way is blocked.
  if       ((offsetY <= 0) && (offsetX > 0) && (p->bottom && !WALKABLE(p->bottom->right)))  decrementOffsetX();
  else if ((offsetY <= 0) && (offsetX < 0) && (p->bottom && !WALKABLE(p->bottom->left)))    incrementOffsetX();
  else
    decrementOffsetY();

  if (offsetY <= (-1 * (ANIMATION_SIZE / 2)))
  {
    // Check if there is an empty spot
    if (moveTo(next))//checkCollision(next))
    {
      offsetY=(ANIMATION_SIZE / 2);
      return true;
    }
  }
  return false;
}

// This is the Creature's AI.
// Each ant knows what direction its heading in,
// it will head in that direction until a tile change
// at which point it will keep going till in the center
// of a tile, then call the AI function according to its
// ACTION.
void Creature::stateStep(int num)
{
  // health goes down every step.
  if (hp > (num-1))
    hp-=num;
  else
    hp=0;

  if ((hp < 10) && FOODi(carrying))
    use();
}

// because of tile changes/animation, need to call move multiple times or the move
// logic will get crazy.  In any case, AI only runs on a tile change so this isn't
// a very expensive addition.
void Creature::move(int num)
{
  for(int i=num; i>0; i--)
    move();
}
void Creature::move()
{
  // they are dead.  will be deleted from the list at some interval.
//  if (hp <= 0) return;

  // Run without checking, AI figured everything out until the tile changes.
  if (!getAI())
  {
    if (direction == 0)
      setAI(moveDown());
    else if (direction == 1)
      setAI(moveRight());
    else if (direction == 2)
      setAI(moveLeft());
    else if (direction == 3)
      setAI(moveUp());
    // if couldn't move in desired direction, reset.  This shouldn't happen.
    else
    {
      directionOld = direction;
      direction = -1;
      setAI(true);
    }
    return;
  }
/*
  // if AI needs to be run, but it hasn't gotten to the center yet, keep moving.
  if ((direction == 0) && (getOffsetY() != 0))
  {
    moveDown();
    return;
  }
  else if ((direction == 1) && (getOffsetX() != 0))
  {
    moveRight();
    return;
  }
  else if ((direction == 2) && (getOffsetX() != 0))
  {
    moveLeft();
    return;
  }
  else if ((direction == 3) && (getOffsetY() != 0))
  {
    moveUp();
    return;
  }
*/
  // if the tile changed, reset.
  if (getAI())
  {
    directionOld = direction;
    direction = -1;
  }

  // if the direction has not reset, exit.
  if (direction != -1) return;

  // DirectionOld to reverse dir.
  if (directionOld == 0) directionOld = 3;
  else if (directionOld == 1) directionOld = 2;
  else if (directionOld == 2) directionOld = 1;
  else if (directionOld == 3) directionOld = 0;

  moveAI();
}

void Creature::moveAI()
{
  // If it gets here need to decide where to go.
  // Select AI pattern based on ACTION.
  if (ACTION == ANT_ACTION_WANDER)
  {
    wander();
  }
  else if (ACTION == ANT_ACTION_FORAGE)
  {
    forage();
  }
  else if (ACTION == ANT_ACTION_ATTACK)
    attack();
}

// 0. Do not turn around unless dead end or specified below.
// 1. If enemy is within 2 blocks, move towards.
// 2. if enemy is in block, mortal kombat.
// 3. wander.
void Creature::attack()
{

  // if nothing to fight, wander.
  wander();
}

// This one should work as follows:
// 0. Do not turn around unless dead end or specified below.
// 1. If in nest with food, drop food then leave.
// 2. If outside, look for feramone, follow.
// 3. If feramone dead end, random direction.
// 4. If food, pick up and turn around.
// 5. If carrying food follow feramone.
//--------------this needs more----------------
// 1. If not on surface, wander until on surface.
// 2. If no food, set feramone output = 100:
// 3.   If on surface and HOT trail, follow (hot == feramone > 100)
// 4.   If on surface and no HOT trail, go to a spot with no feramone
// 5.     Move to spot with no feramone, Mark feramone (output = 100)
// 6.   If no spot with no feramone, wander
// 7. If food, set feramone output = 1000:
// 8.   Pickup food, mark food spot with feramone
// 9.   Go Home
void Creature::forage()
{

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
void Creature::goHome()
{

}

// This algorithm works remarkably well in tests. The first rule makes it follow
// tunnels very well.
// Rules:
// 1. The ant will not turn around unless it hits a dead end.
// 2. The ant will not stop.
void Creature::wander()
{
  // move about randomly.
  direction = rand()%4;
  if (direction == directionOld)
    direction = (direction+1)%4;

  bool newDir = false;
  // cache getPatch() so that it doesn't need a read each time.
  Patch* cache = getPatch();

  for (int four=0; !newDir; four++)
  {
    if ((direction == 0) && directionIsOk(direction, directionOld, cache->bottom))
      newDir = true;
    else if ((direction == 1) && directionIsOk(direction, directionOld, cache->right))
      newDir = true;
    else if ((direction == 2) && directionIsOk(direction, directionOld, cache->left))
      newDir = true;
    else if ((direction == 3) && directionIsOk(direction, directionOld, cache->top))
      newDir = true;

    if (!newDir)
    {
      direction += 1;
      if (direction == 4)
        direction = 0;
    }

    // dead end, forced to turn around.
    if (four == 4)
    {
      // SPECIAL CASE: if ants are remembering recent tiles, check if we are
      //               stuck because of it. 
      // check if we can't go because they are all visited
      if ( use_visit_memory &&
           (!WALKABLE(cache->top)    || checkVisited(cache->top))  &&
           (!WALKABLE(cache->right)  || checkVisited(cache->right)) &&
           (!WALKABLE(cache->left)   || checkVisited(cache->left))  &&
           (!WALKABLE(cache->bottom) || checkVisited(cache->bottom)) )
      {
        clearVisited();
        wander();
        return;
      }

      // Otherwise, just turn around.
      // this case will never be true unless there are a bunch of ants blocking
      // the way.
      direction = directionOld;
      setAI(false);
      return;
    }
  }
  setAI(false);
}

void Creature::clampDirections()
{
  if (directionX > 20)
    directionX = 20;
  else if (directionX < -20)
    directionX = -20;

  if (directionY > 20)
    directionY = 20;
  else if (directionY < -20)
    directionY = -20;
}

int Creature::use()
{
  if ((hp <= 10) && FOODi(carrying))
  {
    hp = 1000;
    carrying = NOTHING;
    return 1; // TODO: object action enum, i.e. PLAYER_USED_FOOD
  }
  // if not usable.
  return 0;
}

void Creature::incrementOffsetX()
{
  offsetX++;

  if ( directionX < 20 )
    directionX+=3;

  // center Y
  if ( directionY < 0 )
    directionY++;
  else if (directionY > 0)
    directionY--;

  clampDirections();
}
void Creature::decrementOffsetX()
{
  offsetX--;
  if ( directionX > -20 )
    directionX-=3;

  // center Y
  if ( directionY < 0 )
    directionY++;
  else if (directionY > 0)
    directionY--;

  clampDirections();
}
void Creature::incrementOffsetY()
{
  offsetY++;
  if ( directionY < 20 )
    directionY+=3;

  // center X
  if ( directionX < 0 )
    directionX++;
  else if (directionX > 0)
    directionX--;

  clampDirections();
}
void Creature::decrementOffsetY()
{
  offsetY--;
  if ( directionY > -20 )
    directionY-=3;

  // center X
  if ( directionX < 0 )
    directionX++;
  else if (directionX > 0)
    directionX--;

  clampDirections();
}

//#ifdef __DEBUG
void Creature::printDebug()
{
  printf("AI (");
  if (ai)
    printf("X)");
  else
    printf(" )");

  printf(" failcount(%i)", failCount);
  printf("\n portaled(");
  if (portaled)
    printf("X)");
  else
    printf(" )");

   printf(" ACTION = %i", ACTION);
  
}
//#endif
