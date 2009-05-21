#include "Ant.h"

Ant::Ant()
{
	offsetX = 0;
	offsetY = 0;
	directionX = 0;
	directionY = 20;
	portaled = false;
	carrying = 0;
	ai = true;
	ACTION=0;
	direction=-1;
	directionOld=-1;

	hp = 1000;
}

Ant::Ant(Patch* pat, int loc)
{
	location = loc;
	offsetX = 0;
	offsetY = 0;
	directionX = 0;
	directionY = 20;
	p=pat;
	portaled = false;
	carrying = 0;
	ai = true;
	ACTION=0;
	direction=-1;
	directionOld=-1;
}

// Checks if there is a portal.  If there is, go through.
bool Ant::handlePortal()
{
	// the portaled flag is supposed to stop it from flipping back and forth over and over again.
	if (!p->portal)
	{
		portaled = false;
		return false;
	}

	if (!portaled && p->portal && WALKABLE(p->portal))
	{
		p = p->portal;

		offsetX=0;
		offsetY=0;
		portaled = true;
		location = p->location;
		return true;
	}

	return false;
}

// This is the "simple" way, only move up/down if X is 0, or right/left if Y is 0.
// center the offending piece if they arne't 0.
// I added a couple tweaks:
//		-if the piece is half way between two squares it moves to the next one
//		-if the piece is going the opposite direction its been heading (i.e. a little
//				left then right) then there is no checking.
//				*this piece adds some jitter I think.

// TODO: get rid of the hard coded numbers so that I can tweak the ants speed without
//       changing each one manually.
bool Ant::moveRight()
{
	if (handlePortal()) return true;

	// don't move right if the ant is in the right spot isn't empty.
	if ((offsetX >= 0) && !WALKABLE(p->right)) return false;

	// if Y is off center, the way is not empty, and we're well on our way to the next
	// spot, move towards center Y.
	if 			((offsetX >= 0) && (offsetY > 0) && (p->right && !WALKABLE(p->right->top))) 		decrementOffsetY();
	else if ((offsetX >= 0) && (offsetY < 0) && (p->right && !WALKABLE(p->right->bottom)))	incrementOffsetY();
	else
		incrementOffsetX();

	// if we are half way towards the next way, swap to the next one.
	if (offsetX >= (ANIMATION_SIZE / 2))
	{
		offsetX=-1 * (ANIMATION_SIZE / 2);
		p = Grid::getRight(p);
		return true;
	}
	return false;
}

bool Ant::moveLeft()
{
	if (handlePortal()) return true;

	// if able to move right...
	if ((offsetX <= 0) && !WALKABLE(p->left)) return false;

	// only move left if Y is centered.
	if			((offsetX <= 0) && (offsetY > 0) && (p->left && !WALKABLE(p->left->top))) 		decrementOffsetY();
	else if ((offsetX <= 0) && (offsetY < 0) && (p->left && !WALKABLE(p->left->bottom))) incrementOffsetY();
	else
		decrementOffsetX();
	if (offsetX <= (-1 * (ANIMATION_SIZE / 2)))
	{
		offsetX=(ANIMATION_SIZE / 2);
		p = Grid::getLeft(p);
		return true;
	}
	return false;
}

bool Ant::moveUp()
{
	if (handlePortal()) return true;

	if ((offsetY >= 0) && !WALKABLE(p->top)) return false;

	// Prevent player from going in a weird direction if that way is blocked.
	if 			((offsetY >= 0) && (offsetX > 0) && (p->top && !WALKABLE(p->top->right)))	decrementOffsetX();
	else if ((offsetY >= 0) && (offsetX < 0) && (p->top && !WALKABLE(p->top->left)))	incrementOffsetX();
	else
		incrementOffsetY();
	if (offsetY >= (ANIMATION_SIZE / 2))
	{
		offsetY=-1 * (ANIMATION_SIZE / 2);
		p = Grid::getUp(p);
		return true;
	}
	return false;
}

bool Ant::moveDown()
{
	if (handlePortal()) return true;

	if ((offsetY <= 0) && !WALKABLE(p->bottom)) return true;

	// Prevent player from going in a weird direction if that way is blocked.
	if 			((offsetY <= 0) && (offsetX > 0) && (p->bottom && !WALKABLE(p->bottom->right)))	decrementOffsetX();
	else if ((offsetY <= 0) && (offsetX < 0) && (p->bottom && !WALKABLE(p->bottom->left)))		incrementOffsetX();
	else
		decrementOffsetY();

	if (offsetY <= (-1 * (ANIMATION_SIZE / 2)))
	{
		offsetY=(ANIMATION_SIZE / 2);
		p = Grid::getDown(p);
		return true;
	}
	return false;
}

// This is the Ant's AI.
// Each ant knows what direction its heading in,
// it will head in that direction until a tile change
// at which point it will keep going till in the center
// of a tile, then call the AI function according to its
// ACTION.
void Ant::stateStep(int num)
{
	// health goes down every step.
	if (hp > (num-1))
		hp-=num;
	else
		hp=0;

	if ((hp < 10) && FOODi(carrying))
		use();

	// if they are dead, no longer move.
	if (hp <= 0)
		return;
}

// because of tile changes/animation, need to call move multiple times or the move
// logic will get crazy.  In any case, AI only runs on a tile change so this isn't
// a very expensive addition.
void Ant::move(int num)
{
	for(int i=num; i>0; i--)
		move();
}
void Ant::move()
{
	// they are dead.  will be deleted from the list at some interval.
	if (hp <= 0) return;
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

	// If it gets here need to decide where to go.
	// Select AI pattern based on ACTION.
	if (ACTION == ANT_ACTION_WANDER)
		wander();
	else if (ACTION == ANT_ACTION_FORAGE)
		forage();
	else if (ACTION == ANT_ACTION_ATTACK)
		attack();
}

// 0. Do not turn around unless dead end or specified below.
// 1. If enemy is within 2 blocks, move towards.
// 2. if enemy is in block, mortal kombat.
// 3. wander.
void Ant::attack()
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
void Ant::forage()
{

}

// This algorithm works remarkably well in tests. The first rule makes it follow
// tunnels very well.
// Rules:
// 1. The ant will not turn around unless it hits a dead end.
// 2. The ant will not stop.
void Ant::wander()
{
	// move about randomly.
	direction = rand()%4;
	if (direction == directionOld)
		direction = (direction+1)%4;

	bool newDir = false;

	for (int four=0; !newDir; four++ )
	{
		if ((direction == 0) && (direction != directionOld) && (getPatch()->bottom) && WALKABLE(getPatch()->bottom))
			newDir = true;
		else if ((direction == 1) && (direction != directionOld) && (getPatch()->right) && WALKABLE(getPatch()->right))
			newDir = true;
		else if ((direction == 2) && (direction != directionOld) && (getPatch()->left) && WALKABLE(getPatch()->left))
			newDir = true;
		else if ((direction == 3) && (direction != directionOld) && (getPatch()->top) && WALKABLE(getPatch()->top))
			newDir = true;

		if (!newDir)
			direction = (direction+1)%4;

		// dead end, forced to turn around.
		if (four == 4)
		{
			direction = directionOld;
			setAI(false);
			return;
		}
	}
	setAI(false);
}

void Ant::clampDirections()
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

int Ant::use()
{
	if ((hp <= 10) && FOODi(carrying))
	{
		hp = 1000;
		carrying = 0;
		return 1; // TODO: object action enum, i.e. PLAYER_USED_FOOD
	}
	// if not usable.
	return 0;
}

void Ant::incrementOffsetX()
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
void Ant::decrementOffsetX()
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
void Ant::incrementOffsetY()
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
void Ant::decrementOffsetY()
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
