#include "StaticDraw.h"

void StaticDraw::draw(Underground *ug, short slice, short width, short height)
{
	int x, y;
	Patch *topLeft = ug->getDisk()->getPatch(slice, width, height);
	Patch *t;

//	Box(-3.5, 0, -7, 0.2, 1.5, 1);

	// Shift to the left a little so the screen will be centered.
	for(x = 0; x < (UNDERGROUND_GRID_DRAW/2); x++)
		topLeft = topLeft->getLeft();

	t = topLeft;

	// Draw the box 3 to the right and 3 down.
	y = -6;
	do {
		for (x = -6; x < UNDERGROUND_GRID_DRAW; x++)
		{
			Box(x, y, -7,  0.1, 0.1, 1);
			if (t)
				t = t->getRight();
		}
		if (topLeft)
		{
			topLeft = topLeft->getBottom();
			t = topLeft;
		}
	} while (y++, y < UNDERGROUND_GRID_DRAW);
}

void StaticDraw::draw(Patch *p)
{

}

void StaticDraw::Box(float x, float y, float z, float width, float height, float depth)
{
	float w2 = width * 0.5;
	float h2 = height * 0.5;
	float d2 = depth * 0.5;

	glBegin(GL_QUADS);
	//z  face
	glColor3f(1,0,0);
	glVertex3f(x-w2 , y-h2 , z-d2 );
	glVertex3f(x + w2, y-h2 , z-d2 );
	glVertex3f(x + w2, y + h2, z-d2 );
	glVertex3f(x-w2 , y + h2, z-d2 );

	//z + depth face
	glColor3f(0,0,1);
	glVertex3f(x-w2 , y-h2 , z + d2);
	glVertex3f(x-w2 , y + h2, z + d2);
	glVertex3f(x + w2, y + h2, z + d2);
	glVertex3f(x + w2, y-h2 , z + d2);

	//x  face
	glColor3f(1,1,0);
	glVertex3f(x-w2 , y-h2 , z-d2 );
	glVertex3f(x-w2 , y + h2, z-d2 );
	glVertex3f(x-w2 , y + h2, z + d2);
	glVertex3f(x-w2 , y-h2 , z + d2);

	//x + width face
	glColor3f(1,1,0);
	glVertex3f(x + w2, y-h2 , z-d2 );
	glVertex3f(x + w2, y-h2 , z + d2);
	glVertex3f(x + w2, y + h2, z + d2);
	glVertex3f(x + w2, y + h2, z-d2 );

	//y  face
	glColor3f(1,1,0);
	glVertex3f(x - w2       , y-h2        , z-d2 );
	glVertex3f(x - w2       , y-h2 - 0.02 , z + d2);
	glVertex3f(x + w2 + 0.05, y-h2 - 0.02 , z + d2);
	glVertex3f(x + w2       , y-h2        , z-d2 );

	//y  + height face
	glColor3f(1,1,0);
	glVertex3f(x - w2       , y + h2       , z-d2 );
	glVertex3f(x + w2       , y + h2       , z-d2 );
	glVertex3f(x + w2 + 0.05, y + h2 + 0.03, z + d2);
	glVertex3f(x - w2       , y + h2 + 0.03, z + d2);

	glEnd();
}

