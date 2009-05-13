#include "Ant.h"

void Ant::moveRight()
{
	// if able to move right...
	if (p->right->TYPE != PATCH_EMPTY) return;

	offsetX++;
	if (offsetX >= 20)
	{
		offsetX=0;
		DirtDisk::moveRight(x);
		p = p->right;
	}
}

void Ant::moveLeft()
{
	// if able to move right...
	if (p->left->TYPE != PATCH_EMPTY) return;

	offsetX--;
	if (offsetX <= -20)
	{
		offsetX=0;
		DirtDisk::moveLeft(x);
		p = p->left;
	}
}

void Ant::moveUp()
{
	// TODO: This will need special handling to move the ant to the surface.
	if (p->top->TYPE == PATCH_TOP) return;
	if (p->top->TYPE != PATCH_EMPTY) return;

	offsetY++;
	if (offsetY >= 20)
	{
		offsetY=0;
		DirtDisk::moveUp(y);
		p = p->top;
	}
}

void Ant::moveDown()
{
	if (p->bottom->TYPE != PATCH_EMPTY) return;

	offsetY--;
	if (offsetY <= -20)
	{
		offsetY=0;
		DirtDisk::moveDown(y);
		p = p->bottom;
	}
}
