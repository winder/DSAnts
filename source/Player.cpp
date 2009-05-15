#include "Player.h"

Player::Player()
{
}

Player::Player(Ant* inp)
{
	p = inp;
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
			else								moveUp();

			return;
		}
		else if (destX == p->getX() && (p->getOffsetX() != 0))
		{
			if (direction == 0) moveLeft();
			else								moveRight();

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
		else 									direction = 2;
	}
	else
	{
		if (directionY == -1) direction = 3;
		else									direction = 1;
	}

	// recurse, but this will only go once, as it returns after moving.
//	move();
}

// If it can dig, return the spot.
Patch* Player::dig()
{
	if( (p->getPatch()->right->TYPE == PATCH_DIRT) && p->getPatch()->right->picked )
		return p->getPatch()->right;
	else if( (p->getPatch()->left->TYPE == PATCH_DIRT) && p->getPatch()->left->picked )
		return p->getPatch()->left;
	// TOP could potentially be NULL (actually I don't think so.)
	else if( (p->getPatch()->top) && (p->getPatch()->top->TYPE == PATCH_DIRT) && p->getPatch()->top->picked )
		return p->getPatch()->top;
	// BOTTOM will not be NULL, it always has a barrier along the bottom.
	else if( (p->getPatch()->bottom->TYPE == PATCH_DIRT) && p->getPatch()->bottom->picked )
		return p->getPatch()->bottom;

	return '\0';
}

void Player::printDebug()
{
	printf("\nPlayer loc: (%i, %i)",getPlayerAnt()->getPatch()->x, getPlayerAnt()->getPatch()->y); 
	printf("\nPatch type: ");
	int type = getPlayerAnt()->getPatch()->TYPE;
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

}
