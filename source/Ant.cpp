#include "Ant.h"

Ant::Ant()
{
	offsetX = 0;
	offsetY = 0;
}

Ant::Ant(int inx, int iny, Patch* pat)
{
	offsetX = 0;
	offsetY = 0;
	x=inx;
	y=iny;
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
void Ant::moveRight()
{
	// don't move right if the ant is in the right spot isn't empty.
	if ((offsetX >= 0) &&p->right->TYPE != PATCH_EMPTY) return;

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
		Grid::moveRight(x);
		p = p->right;
	}
}

void Ant::moveLeft()
{
	// if able to move right...
	if ((offsetX <= 0) && p->left->TYPE != PATCH_EMPTY) return;

	// only move left if Y is centered.
	if			((offsetX <= 0) && (offsetY > 0) && (p->left->top->TYPE != PATCH_EMPTY)) 		offsetY--;
	else if ((offsetX <= 0) && (offsetY < 0) && (p->left->bottom->TYPE != PATCH_EMPTY)) offsetY++;
	else
		offsetX--;
	if (offsetX <= (-1 * (ANIMATION_SIZE / 2)))
	{
		offsetX=(ANIMATION_SIZE / 2);
		Grid::moveLeft(x);
		p = p->left;
	}
}

void Ant::moveUp()
{
	// no check if we're going opposite the way we're heading.
//	if (offsetY < 0) offsetY++;

	// TODO: This will need special handling to move the ant to the surface.
//	if (p->top->TYPE == PATCH_TOP) return;
	if ((offsetY >= 0) && p->top->TYPE != PATCH_EMPTY) return;

	// Prevent player from going in a weird direction if that way is blocked.
	if 			((offsetY >= 0) && (offsetX > 0) && (p->top->right->TYPE != PATCH_EMPTY))	offsetX--;
	else if ((offsetY >= 0) && (offsetX < 0) && (p->top->left->TYPE != PATCH_EMPTY))	offsetX++;
	else
		offsetY++;
	if (offsetY >= (ANIMATION_SIZE / 2))
	{
		offsetY=-1 * (ANIMATION_SIZE / 2);
		Grid::moveUp(y);
		p = p->top;
	}
}

void Ant::moveDown()
{
	// no check if we're going opposite the way we're heading.
//	if (offsetY > 0) offsetY--;

	if ((offsetY <= 0) && p->bottom->TYPE != PATCH_EMPTY) return;

	// Prevent player from going in a weird direction if that way is blocked.
	if 			((offsetY <= 0) && (offsetX > 0) && (p->bottom->right->TYPE != PATCH_EMPTY))	offsetX--;
	else if ((offsetY <= 0) && (offsetX < 0) && (p->bottom->left->TYPE != PATCH_EMPTY))		offsetX++;
	else
		offsetY--;

	if (offsetY <= (-1 * (ANIMATION_SIZE / 2)))
	{
		offsetY=(ANIMATION_SIZE / 2);
		Grid::moveDown(y);
		p = p->bottom;
	}
}

/*
// This stuff is clever and all, but it is too complicated, there is
// noticable slowdown between the "nocheck" and other cases.
// ALSO, it doesn't work quite right. (the -19 -> 1 part jumps)
void Ant::moveRight()
{
	// no check if we're going opposite the way we're heading.
	if (offsetX < 0) offsetX++;

	// if able to move right...
	if (p->right->TYPE != PATCH_EMPTY) return;

	// Prevent player from going in a weird direction if that way is blocked.
	if ((offsetY > 0) && (p->right->top->TYPE != PATCH_EMPTY)) offsetY--;
	else if ((offsetY < 0) && (p->right->bottom->TYPE != PATCH_EMPTY)) offsetY++;
	else
		offsetX++;

	if (offsetX >= 1)
	{
		offsetX=-19;
		Grid::moveRight(x);
		p = p->right;
	}
}

void Ant::moveLeft()
{
	// no check if we're going opposite the way we're heading.
	if (offsetX > 0) offsetX--;

	// if able to move right...
	if (p->left->TYPE != PATCH_EMPTY) return;

	// Prevent player from going in a weird direction if that way is blocked.
	if ((offsetY > 0) && (p->left->top->TYPE != PATCH_EMPTY)) offsetY--;
	else if ((offsetY < 0) && (p->left->bottom->TYPE != PATCH_EMPTY)) offsetY++;
	else
		offsetX--;
	if (offsetX <= -1)
	{
		offsetX=19;
		Grid::moveLeft(x);
		p = p->left;
	}
}

void Ant::moveUp()
{
	// no check if we're going opposite the way we're heading.
	if (offsetY < 0) offsetY++;

	// TODO: This will need special handling to move the ant to the surface.
//	if (p->top->TYPE == PATCH_TOP) return;
	if (p->top->TYPE != PATCH_EMPTY) return;

	// Prevent player from going in a weird direction if that way is blocked.
	if ((offsetX > 0) && (p->top->right->TYPE != PATCH_EMPTY)) offsetX--;
	else if ((offsetX < 0) && (p->top->left->TYPE != PATCH_EMPTY)) offsetX++;
	else
		offsetY++;
	if (offsetY >= 1)
	{
		offsetY=-19;
		Grid::moveUp(y);
		p = p->top;
	}
}

void Ant::moveDown()
{
	// no check if we're going opposite the way we're heading.
	if (offsetY > 0) offsetY--;

	if (p->bottom->TYPE != PATCH_EMPTY) return;

	// Prevent player from going in a weird direction if that way is blocked.
	if ((offsetX > 0) && (p->bottom->right->TYPE != PATCH_EMPTY)) offsetX--;
	else if ((offsetX < 0) && (p->bottom->left->TYPE != PATCH_EMPTY)) offsetX++;
	else
		offsetY--;

	if (offsetY <= -1)
	{
		offsetY=19;
		Grid::moveDown(y);
		p = p->bottom;
	}
}
*/
