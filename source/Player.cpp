#include "Player.h"
// Include this here to avoid circular dependencies
#include "GameWorld.h"

Player::Player()
{
  statespeed=1;
}

Player::Player(Ant* inp)
{
  p = inp;
}

Player::~Player()
{
  // this should always be passed in to the player...
  delete p;
}

void Player::stepForward(int num)
{
  // cache num, for player input step speed.
  statespeed = num;

  if (p->getHP() <= 0)
  {
//    set_val(PLAYER_HAS_DIED);
    return;
  }

/*
  // Pressing the D-Pad cancels automove.
  // automove is set by touching an empty spot.
  if (automove)
  {
    p->move();
  }
*/

  // update the ants status (hp)
  p->stateStep(num);
}

void Player::setDestination(int x, int y)
{
  // reset stuck flag
  stuck = false;

  destX = x;
  destY = y;

  // init to no direction.
  direction=-1;

  // we are "deeper" than the destination...
  if (p->getY() > destY)
  {
    directionY = 1;
    direction = 1;
  }
  else
  {
    directionY = -1;
    direction = 3;
  }

  
  // Pretend there's no roll-over
  if (p->getX() < x)
    directionX = 1;
  else
    directionX = -1;

  // if the difference is very large, there is rollover involved, so inverse directions.
  if ( fabs(x - p->getX()) > WIDTH/2)
      directionX *= -1;

}
// Like most games, this is not going to be a shortest path algorithm,
// it will attempt to go straight and will turn just a couple times.

// This algorithm is very dumb, I'm enumerating everything...
void Player::move()
{
  // don't do anything if stuck. stuck also means done.
  if (stuck) return;


  if ((p->getX() != destX) && (direction == 0) && (p->getPatch()->left->TYPE != PATCH_DIRT))// && !(p->getOffsetX() >= 0))
  {
    moveLeft();
    return;
  }
  else if ((p->getX() != destX) && (direction == 2) && (p->getPatch()->right->TYPE != PATCH_DIRT))// && !(p->getOffsetX() <= 0))
  {
    moveRight();
    return;
  }
  else if ((p->getY() != destY) && (direction == 1) && (p->getPatch()->top->TYPE != PATCH_DIRT))// && !(p->getOffsetY() >= 0))
  {
    moveUp();
    return;
  }
  else if ((p->getY() != destY) && (direction == 3) && (p->getPatch()->bottom->TYPE != PATCH_DIRT))// && !(p->getOffsetY() <= 0))
  {
    moveDown();
    return;
  }


  // Final stretch.
    if (destY == p->getY() && (p->getOffsetY() != 0))
    {
      if (direction == 3) moveDown();
      else                moveUp();

      return;
    }
    else if (destX == p->getX() && (p->getOffsetX() != 0))
    {
      if (direction == 0) moveLeft();
      else                moveRight();

      return;
    }

    // if we get here and this is true, we're there.
    if ((destY == p->getY()) && (destX == p->getX()))
    {
      directionX = -1;
      directionY = -1;
      stuck = true;
    }
  
  // If we get here there is more work to be done.
  // check for dead end.
  bool deadEndX = false;
  bool deadEndY = false;
  // in X direction
  if ( (directionX==1) && (p->getPatch()->right->TYPE == PATCH_DIRT))
    deadEndX = true;
  else if ( (directionX==-1) && (p->getPatch()->left->TYPE == PATCH_DIRT))
    deadEndX = true;
  // in Y direction
  if ( (directionY==1) && (p->getPatch()->top->TYPE == PATCH_DIRT))
    deadEndY = true;
  else if ( (directionY==-1) && (p->getPatch()->bottom->TYPE == PATCH_DIRT))
    deadEndY = true;

  // TODO: test
  if (deadEndX && deadEndY)
  {
    stuck = true;
    return;
  }

  // if we got here we hit a dead end but still have a way to go.
  if (!deadEndX)
  {
    if (directionX == -1) direction = 0;
    else                   direction = 2;
  }
  else
  {
    if (directionY == -1) direction = 3;
    else                  direction = 1;
  }

  // recurse, but this will only go once, as it returns after moving.
//  move();
}

// If it can dig, return the spot.
Patch* Player::dig()
{
  Patch* t = adjacentPatchPicked();
  if (t && (t->TYPE == PATCH_DIRT))
    return t;
  return '\0';
}

Patch* Player::pickUp()
{ 
  Patch* t = adjacentPatchPicked();
  if (t && (p->getCarrying() == NOTHING))
  {
    p->pickup( t );
    // try using it immediately.  normal ants wait until the next step to use.
    int useAction = use();
    // if the player used an object, notify listeners so actions may be performed.
    // i.e. sound effect.
    if (useAction)
      set_val(useAction);

    return t;
  }
  // Not adjacent, can't do anything with it.
  return '\0';
}

Patch* Player::drop()
{
  Patch* t = adjacentPatchPicked();
  if (t)
    if ( p->drop( t ) )
      return t;
  return '\0';
}

int Player::use()
{
  return p->use();
}

// the observer method.
void Player::update(int value)
{
  // If the user gives input while the ant is dead, don't do anything.
  // The Player object should notify the world that it needs to respawn shortly.
  if (p->getHP() <= 0)
  {
//    return;
  }
  if (value == PLAYER_HELD_LEFT)
  {
    moveLeft();
//    automove = false;
  }
  else if (value == PLAYER_HELD_RIGHT)
  {
    moveRight();
//    automove = false;
  }
  else if (value == PLAYER_HELD_UP)
  {
    moveUp();
//    automove = false;
  }
  else if (value == PLAYER_HELD_DOWN)
  {
    moveDown();
//    automove = false;
  }

  else if (value == PLAYER_PRESSED_X)
  {
    if (p->getFeramoneOutput() == 0)
      p->setFeramoneOutput(100);
    else
      p->setFeramoneOutput(0);
  }
  else if (value == PLAYER_PRESSED_Y)
  {
    // Pick in front of player
    Patch* facing = getPlayerAnt()->getPatch();
    switch(getPlayerAnt()->getDirectionFacing())
    {
      case AI_LEFT:
        facing = facing->left;
        break;
      case AI_RIGHT:
        facing = facing->right;
        break;
      case AI_TOP:
        facing = facing->top;
        break;
      case AI_DOWN:
        facing = facing->bottom;
        break;
    }

    facing->picked = true;
    GameWorldSingleton::getInstance()->setPicked(facing);
    // hack.. GameWorld isn't watching player?
    GameWorldSingleton::getInstance()->update(PLAYER_PICKED_SOMETHING);
    facing->picked = false;
  }
  
}


void Player::printDebug()
{
//  printf("\nPlayer loc: (%i, %i)",p->getPatch()->x, p->getPatch()->y); 
  printf("\nPatch type: ");
  int type = p->getPatch()->TYPE;
  if ( type == PATCH_DIRT )
    printf("PATCH_DIRT");
  else if ( type == PATCH_EMPTY )
    printf("PATCH_EMPTY");
  else if ( type == PATCH_BARRIER )
    printf("PATCH_BARRIER");
  else if ( type == PATCH_TOP )
    printf("PATCH_TOP");
  else if ( type == PATCH_ENTRANCE )
    printf("PATCH_ENTRANCE");
//  printf("\nFacing: x=%i, y=%i", p->getFacingX(), p->getFacingY());
  printf("\nAnt HP: %i", p->getHP());
  printf("\nAnt Carrying: %i", p->getCarrying());
  printf("\nPlayer Coord: (%i, %i)", p->getX(), p->getY());
  printf("\nLocation near FOOD(%s) PORTAL(%s)", p->checkForFood()?"X":" ",
                                                p->checkForPortal()?"X":" ");

/*
  printf("\nAI ");
  switch(p->getAction())
  {
    case ANT_ACTION_FORAGE:
      printf("(FORAGE)");
      break;
    case ANT_ACTION_WANDER:
      printf("(WANDER)");
      break;
  }
  printf(" = ");
  p->moveAI();
  switch(p->getDirection())
  {
    case AI_LEFT:
        printf("LEFT");
        break;
    case AI_RIGHT:
        printf("RIGHT");
        break;
    case AI_TOP:
        printf("TOP");
        break;
    case AI_DOWN:
        printf("DOWN");
        break;
  }
*/
}

// I'm seeing a lot of repeat code doing this, a helper function will reduce
// chance of errors later on.
Patch* Player::adjacentPatchPicked()
{
  if( p->getPatch()->picked )
    return p->getPatch();
  else if( p->getPatch()->right->picked )
    return p->getPatch()->right;
  else if( p->getPatch()->left->picked )
    return p->getPatch()->left;
  else if( p->getPatch()->top && p->getPatch()->top->picked )
      return p->getPatch()->top;
  else if( p->getPatch()->bottom && p->getPatch()->bottom->picked )
    return p->getPatch()->bottom;

  // if they are accessible, diagonals are OK as well.
  // TODO: this *could* look weird depending on where in the box the player is, so consider checking
  //       the players offset as well.
  // top right
  else if ( (EMPTY(p->getPatch()->right) || EMPTY(p->getPatch()->top)) && p->getPatch()->right->top->picked )
    return p->getPatch()->right->top;
  // bottom right
  else if ( (EMPTY(p->getPatch()->right) || EMPTY(p->getPatch()->bottom)) && p->getPatch()->right->bottom->picked )
    return p->getPatch()->right->bottom;
  // top left
  else if ( (EMPTY(p->getPatch()->left) || EMPTY(p->getPatch()->top)) && p->getPatch()->left->top->picked )
    return p->getPatch()->left->top;
  // bottom left
  else if ( (EMPTY(p->getPatch()->left) || EMPTY(p->getPatch()->bottom)) && p->getPatch()->left->bottom->picked )
    return p->getPatch()->left->bottom;
  return '\0';
}
