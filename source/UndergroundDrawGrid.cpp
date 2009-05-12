#include "UndergroundDrawGrid.h"

UndergroundDrawGrid::UndergroundDrawGrid()
{
	smoothScrollX = smoothScrollY = 0;
	boxSide = 0.1;
	picked = '\0';
}


// increment a scene shift some amount before scrolling the tiles.
void UndergroundDrawGrid::incX()
{
	smoothScrollX+=0.01;	

	if (smoothScrollX >= 0.2)
	{
		underground->moveRight(slice, centerX); 
		smoothScrollX = 0;
	}
}

void UndergroundDrawGrid::decX()
{ 
	smoothScrollX-=0.01;	
	if (smoothScrollX <= -0.2)
	{
		underground->moveLeft(slice, centerX);
		smoothScrollX = 0;
	}
}

void UndergroundDrawGrid::incY()
{ 
	if (centerY == 0) return;
	smoothScrollY+=0.01;
	if (smoothScrollY >= 0.2)
	{
		underground->moveUp(centerY);
		smoothScrollY = 0;
	}
}

void UndergroundDrawGrid::decY()
{ 
	if (centerY == 44) return;

	smoothScrollY-=0.01;
	if (smoothScrollY <= -0.2)
	{
		underground->moveDown(centerY);
		smoothScrollY = 0;
	}
}


// WARNING: COPY / PASTE / TWEAK of UndergroundDrawGrid::draw()
bool UndergroundDrawGrid::pickPoint(int x, int y, Camera &cam)
{
glPushMatrix();
	pickMode = true;


	int viewport[]={0,0,255,191}; // used later for gluPickMatrix()
		//change ortho vs perspective
		glViewport(0,192,0,192); // set the viewport to fullscreen
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPickMatrix(x,(191-y),4,4,viewport); // render only what is below the cursor
		cam.Perspective();
		glMatrixMode(GL_MODELVIEW);

	if (picked)
		picked->picked = false;

	// pick mode flags will do the magic & return picked patch.
	draw();

	if (picked)
		picked->picked = true;

	pickMode = false;

glPopMatrix(1);
	return endCheck();
}
Patch* UndergroundDrawGrid::draw()
{
	int x,y, temp;
	Patch* bottomLeft = '\0';
	Patch* tp = '\0';

	// Eventually this call will be made to whatever generic character the
	// map should be centered on.  i.e. the player, or an enemy ant.
	bottomLeft = underground->getPatch(slice, centerX, centerY);

	// Shift from the center to the bottomLeft.
	// shift left a bunch of times.
	for (temp=0; (bottomLeft && (temp < GRID_SIZE)) ; temp++)
		bottomLeft = bottomLeft->left;

	// shift up a bunch of times or until we hit the surface.
	for (temp=GRID_SIZE; ((bottomLeft) && (bottomLeft->bottom) && (temp < GRID_SIZE)); temp++)
		bottomLeft = bottomLeft->bottom;

	tp=bottomLeft;

	// Setup for drawing.
	glBegin(GL_QUADS);

	for (y=GRID_SIZE*-1; (bottomLeft && (y < (GRID_SIZE+1))); y+=1)
	{
		tp=bottomLeft;
		for (x=GRID_SIZE*-1; x < (GRID_SIZE+1); x+=1)
		{
			tp=tp->right;
			if (pickMode)
				startCheck();
			// scale by *0.2 then "translate" by adding the smooth scroll factors.
			drawPatch(x*0.2-smoothScrollX, y*0.2-smoothScrollY, tp);
			if (pickMode)
			{
				if (endCheck())
				{
//					tp->picked = true;
					picked = tp;
					picked->picked = true;
					glEnd();
					return tp;
				}
			}
		}
		if (bottomLeft)
			bottomLeft = bottomLeft->top;
	}

	// Finished drawing.
	glEnd();
}

void UndergroundDrawGrid::drawPatch(float x, float y, Patch *p)
{
// float boxSide = 0.1;
	float s = boxSide;

	if (pickMode)
	{
		drawRect(x, y, 0, s, s);
		// if in pick mode, draw a simple rectangle simply for identifying the location.
		return;
	}
	else if (p->picked)
	{
		// if the box was picked, identify it by coloring red..
		material(31,1,1);
	}
// glColor can't work with glNormal, so use materials.
	else if (p->TYPE == PATCH_DIRT)
	{
		material(31,31,31);
		// Draw filled dirt.
	}
	else if (p->TYPE == PATCH_EMPTY)
	{
		material(15,15,15);
		// Check top, bottom, left, right and draw
		// empty patch with paths that can link to
		// whichever are missing, this will either be a bunch of call lists
		// or, more likely, something I do procedurally:
		//     EMPTY_LEFT
		//		 EMPTY_RIGHT
		//     EMPTY_UP
		//     EMPTY_DOWN
		//     EMPTY_LEFT_RIGHT
		//     EMPTY_LEFT_UP
		//     EMPTY_LEFT_DOWN
		//     EMPTY_RIGHT_UP
		//     EMPTY_RIGHT_DOWN
		//     EMPTY_UP_DOWN
		//     EMPTY_LEFT_RIGHT_UP
		//     EMPTY_LEFT_RIGHT_DOWN
		//     EMPTY_LEFT_UP_DOWN
		//     EMPTY_UP_RIGHT_DOWN
		//     EMPTY_UP_RIGHT_LEFT_DOWN
	}
	else if (p->TYPE == PATCH_BARRIER)
	{
		material(30,30,0);
		// Draw a barrier, rock, root, whateva, I may need to break
		// barrier into several different things, but we'll see.
		// This may also need something like the empty, where barriers
		// mold into a bigger thing instead of a bunch of little ones
	}
	else if (p->TYPE == PATCH_TOP)
	{
		material(1,1,30);
		// Draw the sky, this will probably be some sort of tile that keeps going up.
	}
	else
	{
		// if its unidentified, draw a random red box.
		material(31,1,1);
		drawBox(x, y, 0, s, s, s);
	}

	// for now, draw a box upon completion. each clause above will return early
	// if it does its business.
	drawBox(x, y, 0, s, s, s);
}
void UndergroundDrawGrid::drawBox(float x, float y, float z, float width, float height, float depth)
{

	//z face
	glNormal3f(0,0,1);
	glVertex3f(x	,y	,z);
	glVertex3f(x+width,y	,z);
	glVertex3f(x+width,y+height,z);
	glVertex3f(x	,y+height,z);

	//z+ face
	glNormal3f(0,0,-1);
	glVertex3f(x	,y	,z+depth);
	glVertex3f(x	,y+height,z+depth);
	glVertex3f(x+width,y+height,z+depth);
	glVertex3f(x+width,y	,z+depth);
	
	//x  face
	glNormal3f(1,0,0);
	glVertex3f(x,y		,z);
	glVertex3f(x,y+height	,z);
	glVertex3f(x,y+height	,z+depth);
	glVertex3f(x,y		,z+depth);

	//x + width face
	glNormal3f(-1,0,0);
	glVertex3f(x+width,y 	,z);
	glVertex3f(x+width,y 	,z+depth);
	glVertex3f(x+width,y+height,z+depth);
	glVertex3f(x+width,y+height,z);

	//y  face
	glNormal3f(0,-1,0);
	glVertex3f(x 	,y,z);
	glVertex3f(x 	,y,z+depth);
	glVertex3f(x+width,y,z+depth);
	glVertex3f(x+width,y,z);

	//y  + height face
	glNormal3f(0,1,0);
	glVertex3f(x 	,y+height,z);
	glVertex3f(x+width,y+height,z);
	glVertex3f(x+width,y+height,z+depth);
	glVertex3f(x 	,y+height,z+depth);
}

void UndergroundDrawGrid::material(int r, int g, int b)
{
// This seems to work alright.
	glMaterialf(GL_DIFFUSE, RGB15(r, g, b) | BIT(15)); /// Bit 15 enables the diffuse color to act like being set with glColor(), only with lighting support. When not using lighting, this is going to be the default color, just like being set with glColor().
	glMaterialf(GL_AMBIENT, RGB15(4, 4, 5));
	glMaterialf(GL_SPECULAR, RGB15(0, 0, 0)); /// Bit 15 would have to be set here to enable a custom specularity table, instead of the default linear one.
	glMaterialf(GL_EMISSION, RGB15(0, 0, 0));
}

void UndergroundDrawGrid::drawRect(float x, float y, float z, float width, float height)
{
	glVertex3f(x	,y	,z);
	glVertex3f(x+width,y	,z);
	glVertex3f(x+width,y+height,z);
	glVertex3f(x	,y+height,z);
}

void UndergroundDrawGrid::startCheck()
{
	while(PosTestBusy()); // wait for the position test to finish
	while(GFX_BUSY); // wait for all the polygons from the last object to be drawn
	PosTest_Asynch(0,0,0); // start a position test at the current translated position
	polyCount = GFX_POLYGON_RAM_USAGE; // save the polygon count
}

bool UndergroundDrawGrid::endCheck()
{
	while(GFX_BUSY); // wait for all the polygons to get drawn
	while(PosTestBusy()); // wait for the position test to finish
	if(GFX_POLYGON_RAM_USAGE>polyCount) // if a polygon was drawn
	{
		{
			// this is currently the closest object under the cursor!
			closeW=PosTestWresult();
			return true;
		}
	}
	return false;
}
