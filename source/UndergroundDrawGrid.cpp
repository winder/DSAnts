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
	smoothScrollX+=MODEL_SCALE_INCREMENT;	

	if (smoothScrollX >= MODEL_SCALE)
	{
		underground->moveRight(centerX); 
		smoothScrollX = 0;
	}

}

void UndergroundDrawGrid::decX()
{ 
	smoothScrollX-=MODEL_SCALE_INCREMENT;	
	if (smoothScrollX <= (-1*MODEL_SCALE))
	{
		underground->moveLeft(centerX);
		smoothScrollX = 0;
	}

}

// When drawing a grid, we don't want to pan too shallow.
void UndergroundDrawGrid::incY()
{ 
	if (centerY <= 7) return;
	smoothScrollY+=MODEL_SCALE_INCREMENT;
	if (smoothScrollY >= MODEL_SCALE)
	{
		underground->moveUp(centerY);
		smoothScrollY = 0;
	}

}

void UndergroundDrawGrid::decY()
{ 
	if (centerY == DEPTH-GRID_SIZE-1) return;

	smoothScrollY-=MODEL_SCALE_INCREMENT;
	if (smoothScrollY <= (-1*MODEL_SCALE))
	{
		underground->moveDown(centerY);
		smoothScrollY = 0;
	}

}


// WARNING: COPY / PASTE / TWEAK of UndergroundDrawGrid::draw()
bool UndergroundDrawGrid::pickPoint(int x, int y, Camera &cam)
{
  // This flag makes it so "startCheck" and "endCheck" are called
  // before and after every polygon is drawn
	pickMode = true;


	int viewport[]={0,0,255,191}; // used later for gluPickMatrix()
	//change ortho vs perspective
	glViewport(0,192,0,192); // set the viewport to fullscreen
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix(x,(191-y),4,4,viewport); // render only what is below the cursor
	cam.Perspective();
	glMatrixMode(GL_MODELVIEW);

	// opaque polygons, no culling, this fixes al my problems.
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	// Un-mark the last one.
	if (picked)
		picked->picked = false;

	// pick mode flags will do the magic & return picked patch.
	picked = draw();

	// Mark the new one (which may still be the last one btw)
	if (picked)
		picked->picked = true;

	// Turn off pick mode.
	pickMode = false;

	return endCheck();
}

// WOW there's gotta be something better than this...
bool UndergroundDrawGrid::isVisible(short x, short y)
{
	// out of array bounds.
	if ((x<0) || (y < 0) || (y>=WIDTH) || (x>=WIDTH))
		return false;

	// Out of visible Y values.
	if (	(y > (centerY+GRID_SIZE)) ||
				(y < (centerY-GRID_SIZE)) )
		return false;

	// To get here, y value is good.

	// Basic case
	if (	(x < (centerX+GRID_SIZE)) &&
				(x > (centerX-GRID_SIZE)))
		return true;

	else if (	((WIDTH-1) < (centerX+GRID_SIZE)) &&
				(x < ((centerX+GRID_SIZE)%WIDTH)))
		return true;

	else if (	((centerX-GRID_SIZE) < 0) &&
						(x > (centerX-GRID_SIZE+WIDTH)))
		return true;

	// if none of those then...
	return false;
}

// This is complicated because there's a circular array...
float UndergroundDrawGrid::positionX(short x)
{
	if (	(x < (centerX+GRID_SIZE)) &&
				(x > (centerX-GRID_SIZE)))
		return ((centerX - x + 1) * -1) * MODEL_SCALE;

	else if (	((WIDTH-1) < (centerX+GRID_SIZE)) &&
				(x < ((centerX+GRID_SIZE)%WIDTH)))
		return ((centerX - (x+WIDTH) + 1) * -1) * MODEL_SCALE;

	else if (	((centerX-GRID_SIZE) < 0) &&
						(x > (centerX-GRID_SIZE+WIDTH)))
		return ((centerX - (x-WIDTH) + 1) * -1) * MODEL_SCALE;
}
float UndergroundDrawGrid::positionY(short y)
{
	return (centerY - y) * MODEL_SCALE;
}
void UndergroundDrawGrid::drawAnt(Ant* a)
{
		material(3,3,3);
	// exit early if not visible.
	if (! isVisible(a->getX(), a->getY()))
		return;

	// can easily find X/Y location by finding offset from center.
	float x = positionX(a->getX());
	float y = positionY(a->getY());

	// draw at x, y.

	drawBox(x-smoothScrollX, y-smoothScrollY, 0, MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
}
Patch* UndergroundDrawGrid::draw()
{
	int x,y, temp;
	Patch* bottomLeft = '\0';
	Patch* tp = '\0';

	// Eventually this call will be made to whatever generic character the
	// map should be centered on.  i.e. the player, or an enemy ant.
	bottomLeft = underground->getPatch(centerX, centerY);

	// Shift from the center to the bottomLeft.
	// shift left a bunch of times.
	for (temp=0; (bottomLeft && (temp < GRID_SIZE)) ; temp++)
		bottomLeft = bottomLeft->left;

	// shift up a bunch of times or until we hit the surface.
//	for (temp=GRID_SIZE; ((bottomLeft) && (bottomLeft->bottom) && (temp < GRID_SIZE)); temp++)
	for (temp=0; ((bottomLeft) && (bottomLeft->bottom) && (temp < GRID_SIZE)); temp++)
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
			// scale by *MODEL_SCALE then "translate" by adding the smooth scroll factors.
			// I really have no idea why I was scaling this thing, but I think this is screwing
			// up tile alignment.
			// OK, well if x and y get too big I get weird graphical issues with boxes wrapping
			// around the screen, so lets keep things small for now...
			drawPatch(x*MODEL_SCALE-smoothScrollX, y*MODEL_SCALE-smoothScrollY, tp);
			if (pickMode)
				if (endCheck())
				{
					glEnd();
					return tp;
				}
		}
		if (bottomLeft)
			bottomLeft = bottomLeft->top;
	}

	// Finished drawing.
	glEnd();
	return '\0';
}

void UndergroundDrawGrid::drawPatch(float x, float y, Patch *p)
{
	float s = boxSide;
	// make it take up all the space.
	s = MODEL_SCALE;

	// WARNING: this optomization makes it so picking only works from one side of the X/Y plane.
	// (when cam location z > 0)
	if (pickMode)
	{
		drawRect(x, y, s, s, s);
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
	{ // brown.
		material(19,10,5);
		// Draw filled dirt.

		// if it is completely enclosed.... only draw the front.
		// THIS FIXES THE WEIRD BOXY PROBLEM, WHEN I IMPLEMENT THE EMPTY
		// CODE STUFF I WILL BE ABLE TO ONLY DRAW THE RECTANGLE ALL THE TIME
		// WHICH WILL FIX THE GRAPHICAL ERROR ALL THE TIME.
		if (p->top->TYPE == PATCH_DIRT)
		if (p->left->TYPE == PATCH_DIRT)
		if (p->right->TYPE == PATCH_DIRT)
		if (p->bottom->TYPE == PATCH_DIRT)
		{
			// this assumes I'm going to be looking at it from the wrong side...
			drawRect(x, y, s, s, s);
			return;
		}
	}
	else if (p->TYPE == PATCH_EMPTY)
	{
		material(5,5,5);
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

		// this rectangle is assuming I will go with the looking-at-wrong-side approach.
		drawRect(x, y, 0, s, s);

		// special drawing for this now....
		return;
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
		// Draw the sky, this will probably be some sort flat texture that gets tiled and keeps going up.

		
		// this rectangle is assuming I will go with the looking-at-wrong-side approach.

		material(1,31,10); // Greenish
		drawRect(x, y, 0, s, s);
		material(5,5,28); // Bluish
		drawRect(x, y+s, 0, s, 1);
		drawRect(x, y+(s*2), 0, s, 1);
		drawRect(x, y+(s*3), 0, s, 1);

		// special drawing for this now...
		return;
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
	glNormal3f(0,0,-1);
	glVertex3f(x	,y	,z);
	glVertex3f(x	,y+height,z);
	glVertex3f(x+width,y+height,z);
	glVertex3f(x+width,y	,z);
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
