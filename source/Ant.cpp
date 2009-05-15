#include "Ant.h"

Ant::Ant()
{
	offsetX = 0;
	offsetY = 0;
}

Ant::Ant(Patch* pat, int loc)
{
	location = loc;
	offsetX = 0;
	offsetY = 0;
//	x=inx;
//	y=iny;
	p=pat;
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
	// don't move right if the ant is in the right spot isn't empty.
	if ((offsetX >= 0) &&p->right->TYPE != PATCH_EMPTY) return false;

	// if Y is off center, the way is not empty, and we're well on our way to the next
	// spot, move towards center Y.
	if 			((offsetX >= 0) && (offsetY > 0) && (p->right->top->TYPE != PATCH_EMPTY)) 		offsetY--;
	else if ((offsetX >= 0) && (offsetY < 0) && (p->right->bottom->TYPE != PATCH_EMPTY))	offsetY++;
	// if all is well, move right.
	else
		offsetX++;

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
	// if able to move right...
	if ((offsetX <= 0) && p->left->TYPE != PATCH_EMPTY) return false;

	// only move left if Y is centered.
	if			((offsetX <= 0) && (offsetY > 0) && (p->left->top->TYPE != PATCH_EMPTY)) 		offsetY--;
	else if ((offsetX <= 0) && (offsetY < 0) && (p->left->bottom->TYPE != PATCH_EMPTY)) offsetY++;
	else
		offsetX--;
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
	// no check if we're going opposite the way we're heading.
//	if (offsetY < 0) offsetY++;

	// TODO: This will need special handling to move the ant to the surface.
	if (p->top->TYPE == PATCH_TOP){
		// todo, portal detection in a seperate function called at the beginning of all move calls.
		p = p->top->portal;
		location = GAMEWORLD_STATE_SURFACE;
		return true;
	}
	if ((offsetY >= 0) && p->top->TYPE != PATCH_EMPTY) return false;

	// Prevent player from going in a weird direction if that way is blocked.
	if 			((offsetY >= 0) && (offsetX > 0) && (p->top->right->TYPE != PATCH_EMPTY))	offsetX--;
	else if ((offsetY >= 0) && (offsetX < 0) && (p->top->left->TYPE != PATCH_EMPTY))	offsetX++;
	else
		offsetY++;
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
	// no check if we're going opposite the way we're heading.
//	if (offsetY > 0) offsetY--;

	if ((offsetY <= 0) && p->bottom->TYPE != PATCH_EMPTY) return true;

	// Prevent player from going in a weird direction if that way is blocked.
	if 			((offsetY <= 0) && (offsetX > 0) && (p->bottom->right->TYPE != PATCH_EMPTY))	offsetX--;
	else if ((offsetY <= 0) && (offsetX < 0) && (p->bottom->left->TYPE != PATCH_EMPTY))		offsetX++;
	else
		offsetY--;

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
void Ant::move()
{
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
		if ((direction == 0) && (direction != directionOld) && (getPatch()->bottom) && (getPatch()->bottom->TYPE == PATCH_EMPTY))
			newDir = true;
		else if ((direction == 1) && (direction != directionOld) && (getPatch()->right) && (getPatch()->right->TYPE == PATCH_EMPTY))
			newDir = true;
		else if ((direction == 2) && (direction != directionOld) && (getPatch()->left) && (getPatch()->left->TYPE == PATCH_EMPTY))
			newDir = true;
		else if ((direction == 3) && (direction != directionOld) && (getPatch()->top) && (getPatch()->top->TYPE == PATCH_EMPTY))
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
