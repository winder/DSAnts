#include "MapDraw.h"

MapDraw::MapDraw()
{
	smoothScrollX = smoothScrollY = 0;
	boxSide = 0.1;
	picked = '\0';
	pickMode = false;
}


// increment a scene shift some amount before scrolling the tiles.
void MapDraw::incX()
{
	smoothScrollX+=MODEL_SCALE_INCREMENT;	

	if (smoothScrollX >= MODEL_SCALE)
	{
		moveCenterRight();
		smoothScrollX = 0;
	}

}

void MapDraw::decX()
{ 
	smoothScrollX-=MODEL_SCALE_INCREMENT;	
	if (smoothScrollX <= (-1*MODEL_SCALE))
	{
		moveCenterLeft();
		smoothScrollX = 0;
	}

}

// When drawing a grid, we don't want to pan too shallow.
void MapDraw::incY()
{ 
// not sure this is needed anymore
//	if (getCenterY() <= 7) return;
	smoothScrollY+=MODEL_SCALE_INCREMENT;

	if (smoothScrollY >= MODEL_SCALE)
	{
		moveCenterUp();
		smoothScrollY = 0;
	}

}

void MapDraw::decY()
{ 
//	if (getCenterY() == DEPTH-GRID_SIZE-1) return;

	smoothScrollY-=MODEL_SCALE_INCREMENT;
	if (smoothScrollY <= (-1*MODEL_SCALE))
	{
		moveCenterDown();
		smoothScrollY = 0;
	}

}


// WARNING: COPY / PASTE / TWEAK of MapDraw::draw()
bool MapDraw::pickPoint(int x, int y, Camera &cam)
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

	// opaque polygons, no culling, this fixes my problems.
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
bool MapDraw::isVisible(short x, short y)
{
// Come on Owen, you have their X / Y.

	// out of array bounds.
	if ((x<0) || (y < 0) || (y>=WIDTH) || (x>=WIDTH))
		return false;

	float pos = positionY( y );
	if (fabs(pos) > (GRID_SIZE-2))
		return false;

	pos = positionX( x );
	if (fabs(pos) > (GRID_SIZE-2))
		return false;

	return true;
}

// This is complicated because there's a circular array...
float MapDraw::positionX(short x)
{
	// normal bounds.
	if (	(x < (getCenterX()+GRID_SIZE)) && (x > (getCenterX()-GRID_SIZE)))
		return ((getCenterX() - x + 1) * -1);
	// over bounds.
	else if (	((WIDTH-1) < (getCenterX()+GRID_SIZE)) && (x < ((getCenterX()+GRID_SIZE)%WIDTH)))
		return ((getCenterX() - (x+WIDTH) + 1) * -1);
	// below bounds.
	else if (	((getCenterX()-GRID_SIZE) < 0) && (x > (getCenterX()-GRID_SIZE+WIDTH)))
		return ((getCenterX() - (x-WIDTH) + 1) * -1);


	//TODO: throw exception?
	return 1215;
}
float MapDraw::positionY(short y)
{
//	return (getCenterY() - y) * MODEL_SCALE;

	// normal bounds.
	if ( ( y < (getCenterY()+GRID_SIZE)) && ( y > (getCenterY()-GRID_SIZE)))
		return ((getCenterY() - y));
	// over bounds.
	else if ( ((DEPTH-1) < (getCenterY() + GRID_SIZE)) && ( y < ((getCenterY() + GRID_SIZE)%DEPTH)))
		return ((getCenterY() - (y+DEPTH)));
	// below bounds.
	else if (	((getCenterY()-GRID_SIZE) < 0) && (y > (getCenterY()-GRID_SIZE+DEPTH)))
		return (getCenterY() - (y-DEPTH));

	//TODO: throw exception?
	return 1215;
}

// Shift the center if the player is too far to the side of the map.
void MapDraw::shiftCenter(Ant *p)
{
	float pos = positionY(p->getY());

	// if the ant is GRID_SIZE/3 boxes away from the center, start trying to follow it.
	if (pos < (-1* (GRID_SIZE / 3)) )
		decY();
	else if (pos > (GRID_SIZE / 3)) 
		incY();

	pos = positionX(p->getX());
	if (pos < (-1 * (GRID_SIZE / 3)))
		decX();
	else if (pos > (GRID_SIZE / 3)) 
		incX();
}

bool MapDraw::drawAnt(Ant* a, bool animate)
{
		material(3,3,3);
	// exit early if not visible.
	if (! isVisible(a->getX(), a->getY()))
		return false;

	// find X/Y location by finding offset from center.
	// Then add the offset for smoothly moving between 2 squares.
	float x = (positionX(a->getX())*MODEL_SCALE) + (a->getOffsetX()*MODEL_SCALE_INCREMENT);
	float y = (positionY(a->getY())*MODEL_SCALE) + (a->getOffsetY()*MODEL_SCALE_INCREMENT);

	if (!animate)
	{
		drawRect(x, y, MODEL_SCALE*0.2, MODEL_SCALE*.4, MODEL_SCALE*0.9);
		return true;
	}
	// draw at x, y.

	// slightly off the background to get rid of overlaping.
	glPushMatrix();
	// if X has no influence, facing right or left.
	// convert facing direction to measurement of 90 degrees ((-20:20) * 2.25).
	// (angle is -32768 to 32767) 
	float xInfluence = ((a->getFacingX() * 2.25f));
	// default facing up.
	float yInfluence = ((a->getFacingY() * 2.25f));

	float rotation = 0;

	// this nonsense sets the ants rotation to face the direction its walking.
	// TODO: figure out how to convert this to glRotatef32i.
	// pointing top-left
	if (( a->getFacingX() <= 0 ) && (a->getFacingY() >= 0))
		rotation = (-315) - (yInfluence+xInfluence);
	// pointing down-left
	else if (( a->getFacingX() <= 0 ) && (a->getFacingY() <= 0))
		rotation = (-225) - (yInfluence-xInfluence);
	// pointing top-right
	else if (( a->getFacingX() > 0 ) && (a->getFacingY() >= 0))
		rotation = (-45) + (yInfluence-xInfluence);
	// pointing down-right
	else if (( a->getFacingX() > 0 ) && (a->getFacingY() <= 0))
		rotation = (-135) + (yInfluence+xInfluence);

	// translate to the center point.
	glTranslatef(x, y, 0.01);
	// rotate so the ant faces in the correct direction.
	glRotatef( rotation			, 0, 0, 1);
	//drawBox(0, 0, 0, MODEL_SCALE*.4, MODEL_SCALE*0.9, MODEL_SCALE*0.4);
	drawRect(0, 0, MODEL_SCALE*0.2, MODEL_SCALE*.4, MODEL_SCALE*0.9);


	// Check if the ant is carrying anything.  If it is, draw it..
	if (a->getCarrying())
	{
		glTranslatef(0,MODEL_SCALE*0.50, MODEL_SCALE*0.1);

		// TODO: different types of objects: drawObject (scaled down if being carried).
		material(3,25,3);
		drawBox(0, 0, 0, MODEL_SCALE*.5, MODEL_SCALE*0.5, MODEL_SCALE*0.5);
	}
	glPopMatrix(1);
	return true;
}
Patch* MapDraw::draw()
{

	// Adding this saves me several hundred additions / subtractions.  Cool beans.
	glTranslatef(smoothScrollX*-1, smoothScrollY*-1, 0);
	// TODO: Fit this one in too so I don't have to to all the MODEL_SCALE multiplications.
//	glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);

	int x,y, temp;
	Patch* bottomLeft = '\0';
	Patch* tp = '\0';

	// Eventually this call will be made to whatever generic character the
	// map should be centered on.  i.e. the player, or an enemy ant.
	bottomLeft = getGrid()->getPatch(getCenterX(), getCenterY());

	// Shift from the center to the bottomLeft.
	// shift left a bunch of times.
	for (temp=0; (bottomLeft && (temp < GRID_SIZE)) ; temp++)
		bottomLeft = Grid::getLeft(bottomLeft);

	// shift up a bunch of times or until we hit the surface.
//	for (temp=GRID_SIZE; ((bottomLeft) && (bottomLeft->bottom) && (temp < GRID_SIZE)); temp++)
	for (temp=0; ((bottomLeft) && (Grid::getDown(bottomLeft)) && (temp < GRID_SIZE)); temp++)
		bottomLeft = Grid::getDown(bottomLeft);

	tp=bottomLeft;

	// Setup for drawing.
//	glBegin(GL_QUADS);

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
			drawPatch(x*MODEL_SCALE, y*MODEL_SCALE, tp);
			if (pickMode)
				if (endCheck())
				{
					//glEnd();
					return tp;
				}
		}
		if (bottomLeft)
			bottomLeft = Grid::getUp(bottomLeft);
	}

	// Finished drawing.
	//glEnd();
	return '\0';
}

// TODO: break each type of patch into its own method.
void MapDraw::drawPatch(float x, float y, Patch *p)
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

/*// this is getting in the way for now.
	// If this box is also picked, make a visible highlight in addition to whatever is below..
	if (p->picked)
	{
		// TODO: set alpha level
		// if the box was picked, identify it by coloring red..
		material(31,31,1);
		drawBox(x, y, 0.1, s, s, 0.1);
	}
*/

// glColor can't work with glNormal, so use materials.
	if (p->TYPE == PATCH_DIRT)
	{ // brown.
		material(0,0,0);
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
			drawTextureRect(x, y, s, s, s);
			return;
		}

		// Otherwise we need to do a whole cube
		drawTextureCenteredBox(x, y, 0, s, s, s);
		return;
	}
	else if (p->TYPE == PATCH_EMPTY)
	{
		material(31,31,31); // make it easier to see ants...
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
	else if (p->TYPE == PATCH_ENTRANCE)
	{
		// If the spot isn't empty, this is a dormant hole...
		// this will probably get confusing at some point, will need to always check
		// if portal->TYPE is empty or not.
		//if (p->portal && p->portal->bottom && p->portal->bottom->TYPE != PATCH_EMPTY)
		if (WALKABLE(p->portal))
		{
			material(1,1,1);
			drawRect(x, y, 0, s, s);
			// special drawing for this now....
			return;
		}
		else
		{
		// TODO: call the empty surface method.
			material(1,31,1); // make it easier to see ants...
			drawRect(x, y, 0, s, s);
			return;
		}
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
		drawRect(x, y+MODEL_SCALE, 0, s, s);
		drawRect(x, y+2*MODEL_SCALE, 0, s, s);
		drawRect(x, y+2*MODEL_SCALE, 0, s, s);

		// special drawing for this now...
		return;
	}
	else
	{
		// if its unidentified, draw a random red box.
		material(31,1,1);
		//drawBox(x, y, 0, s, s, s);
	}

	// for now, draw a box upon completion. each clause above will return early
	// if it does its business.
	drawBox(x, y, 0, s*0.5, s*0.5, s*0.5);
}
void MapDraw::drawBox(float x, float y, float z, float width, float height, float depth)
{
	drawCenteredBox(x,y,z,width,height,depth);
	return;
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

void MapDraw::drawCenteredBox(float x, float y, float z, float width, float height, float depth)
{
	float w2 = width * 0.5;
	float h2 = height * 0.5;

	//z  face
	glNormal3f(0,0,1);
	glVertex3f(x-w2,y-h2,z);
	glVertex3f(x-w2,y+h2,z);
	glVertex3f(x+w2,y+h2,z);
	glVertex3f(x+w2,y-h2,z);

	//z + depth face
	glNormal3f(0,0,-1);
	glVertex3f(x-w2,y-h2,z+depth);
	glVertex3f(x+w2,y-h2,z+depth);
	glVertex3f(x+w2,y+h2,z+depth);
	glVertex3f(x-w2,y+h2,z+depth);

	//x  face
	glNormal3f(1,0,0);
	glVertex3f(x-w2,y-h2,z);
	glVertex3f(x-w2,y-h2,z+depth);
	glVertex3f(x-w2,y+h2,z+depth);
	glVertex3f(x-w2,y+h2,z);

	//x + width face
	glNormal3f(-1,0,0);
	glVertex3f(x+w2,y-h2,z);
	glVertex3f(x+w2,y+h2,z);
	glVertex3f(x+w2,y+h2,z+depth);
	glVertex3f(x+w2,y-h2,z+depth);

	//y  face
	glNormal3f(0,-1,0);
	glVertex3f(x-w2,y-h2,z);
	glVertex3f(x+w2,y-h2,z);
	glVertex3f(x+w2,y-h2,z+depth);
	glVertex3f(x-w2,y-h2,z+depth);

	//y  + height face
	glNormal3f(0,1,0);
	glVertex3f(x-w2,y+h2,z);
	glVertex3f(x-w2,y+h2,z+depth);
	glVertex3f(x+w2,y+h2,z+depth);
	glVertex3f(x+w2,y+h2,z);

}

void MapDraw::drawTextureCenteredBox(float x, float y, float z, float width, float height, float depth)
{
	float w2 = width * 0.5;
	float h2 = height * 0.5;

	//z  face
	glNormal3f(0,0,1);
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
	glVertex3f(x-w2,y-h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
	glVertex3f(x-w2,y+h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
	glVertex3f(x+w2,y+h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	glVertex3f(x+w2,y-h2,z);

	//z + depth face
	glNormal3f(0,0,-1);
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
	glVertex3f(x-w2,y-h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
	glVertex3f(x+w2,y-h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
	glVertex3f(x+w2,y+h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	glVertex3f(x-w2,y+h2,z+depth);

	//x  face
	glNormal3f(1,0,0);
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
	glVertex3f(x-w2,y-h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
	glVertex3f(x-w2,y-h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
	glVertex3f(x-w2,y+h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	glVertex3f(x-w2,y+h2,z);

	//x + width face
	glNormal3f(-1,0,0);
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
	glVertex3f(x+w2,y-h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
	glVertex3f(x+w2,y+h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
	glVertex3f(x+w2,y+h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	glVertex3f(x+w2,y-h2,z+depth);

	//y  face
	glNormal3f(0,-1,0);
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
	glVertex3f(x-w2,y-h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
	glVertex3f(x+w2,y-h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
	glVertex3f(x+w2,y-h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	glVertex3f(x-w2,y-h2,z+depth);

	//y  + height face
	glNormal3f(0,1,0);
	GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
	glVertex3f(x-w2,y+h2,z);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
	glVertex3f(x-w2,y+h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
	glVertex3f(x+w2,y+h2,z+depth);
	GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	glVertex3f(x+w2,y+h2,z);

}

void MapDraw::material(int r, int g, int b)
{
// This seems to work alright.
	glMaterialf(GL_DIFFUSE, RGB15(r, g, b) | BIT(15)); /// Bit 15 enables the diffuse color to act like being set with glColor(), only with lighting support. When not using lighting, this is going to be the default color, just like being set with glColor().
	glMaterialf(GL_AMBIENT, RGB15(r, g, b));
	glMaterialf(GL_SPECULAR, RGB15(0, 0, 0)); /// Bit 15 would have to be set here to enable a custom specularity table, instead of the default linear one.
	glMaterialf(GL_EMISSION, RGB15(31, 31, 31));

  //ds uses a table for shinyness..this generates a half-ass one
	// NOTE: do not use this, at least not here, it slows shit down a lot.
//  glMaterialShinyness();

}

void MapDraw::drawTextureRect(float x, float y, float z, float width, float height)
{

	float w2 = width * 0.5;
	float h2 = height * 0.5;

//TODO: fix this.
	glNormal3f(0,0,-1);
  GFX_TEX_COORD = (TEXTURE_PACK(inttot16(65), inttot16(5)));
	glVertex3f(x-w2,y-h2,z);
  GFX_TEX_COORD = (TEXTURE_PACK(inttot16(65),inttot16(5)));
	glVertex3f(x+w2,y-h2,z);
  GFX_TEX_COORD = (TEXTURE_PACK(inttot16(65), inttot16(5)));
	glVertex3f(x+w2,y+h2,z);
  GFX_TEX_COORD = (TEXTURE_PACK(inttot16(65),inttot16(5)));
	glVertex3f(x-w2,y+h2,z);
}

void MapDraw::drawRect(float x, float y, float z, float width, float height)
{
	drawTextureRect(x,y,z,width,height);
	float w2 = width * 0.5;
	float h2 = height * 0.5;

	glNormal3f(0,0,-1);
	glVertex3f(x-w2,y-h2,z);
	glVertex3f(x+w2,y-h2,z);
	glVertex3f(x+w2,y+h2,z);
	glVertex3f(x-w2,y+h2,z);
}
void MapDraw::startCheck()
{
	while(PosTestBusy()); // wait for the position test to finish
	while(GFX_BUSY); // wait for all the polygons from the last object to be drawn
	PosTest_Asynch(0,0,0); // start a position test at the current translated position
	polyCount = GFX_POLYGON_RAM_USAGE; // save the polygon count
}

bool MapDraw::endCheck()
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
