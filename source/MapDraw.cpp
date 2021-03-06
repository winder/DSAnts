#include "MapDraw.h"

MapDraw::MapDraw()
{
  showPheramone = 0;
  smoothScrollX = smoothScrollY = 0;
  boxSide = 0.1;
  picked = '\0';
  pickMode = false;
  //tm = '\0';
  TextureManager *tm = new TextureManager();
  tm->load( 0, (u8*)dirt_one_img_bin );
//  tm->bind( );
}

MapDraw::MapDraw( Grid *g )
{
  setGrid(g);
}

void MapDraw::initTextures()
{
  tm = new TextureManager();
  tm->load( 0, (u8*)dirt_one_img_bin );
  tm->bind( );
}

// TODO: textures for one level loaded at a time.
void MapDraw::destroyTextures()
{
  // texture manager unloads everything upon destruction.
  delete tm;
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
//  if (getCenterY() <= 7) return;
  smoothScrollY+=MODEL_SCALE_INCREMENT;

  if (smoothScrollY >= MODEL_SCALE)
  {
    moveCenterUp();
    smoothScrollY = 0;
  }

}

void MapDraw::decY()
{ 
//  if (getCenterY() == DEPTH-GRID_SIZE-1) return;

  smoothScrollY-=MODEL_SCALE_INCREMENT;
  if (smoothScrollY <= (-1*MODEL_SCALE))
  {
    moveCenterDown();
    smoothScrollY = 0;
  }

}

// Setup viewport to a 1 pixel window where the user clicked.
// Set the "pickMode" flag and call draw to find out what piece of the grid was drawn.
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

bool MapDraw::isVisible(short x, short y)
{
  int distFromCenter = 
    getGrid()->distanceBetween(x, y, getCenterX(), getCenterY());

  if (distFromCenter < (GRID_SIZE - 2 ))
    return true;
  else
    return false;
/*
// Come on Owen, you have their X / Y.

  // out of array bounds.
  if ((x<0) || (y < 0) || (y>=DEPTH) || (x>=WIDTH))
    return false;

  float pos = positionY( y );
  if (fabs(pos) > (GRID_SIZE-2))
    return false;

  pos = positionX( x );
  if (fabs(pos) > (GRID_SIZE-2))
    return false;
  return true;
*/
}

// This is complicated because there's a circular array...
float MapDraw::positionX(short x)
{
  // normal bounds.
  if (  (x < (getCenterX()+GRID_SIZE)) && (x > (getCenterX()-GRID_SIZE)))
    return ((getCenterX() - x + 1) * -1);
  // over bounds.
  else if (  ((WIDTH-1) < (getCenterX()+GRID_SIZE)) && (x < ((getCenterX()+GRID_SIZE)%WIDTH)))
    return ((getCenterX() - (x+WIDTH) + 1) * -1);
  // below bounds.
  else if (  ((getCenterX()-GRID_SIZE) < 0) && (x > (getCenterX()-GRID_SIZE+WIDTH)))
    return ((getCenterX() - (x-WIDTH) + 1) * -1);


  //TODO: throw exception?
  return 1215;
}
float MapDraw::positionY(short y)
{
  // normal bounds.
  if ( ( y < (getCenterY()+GRID_SIZE)) && ( y > (getCenterY()-GRID_SIZE)))
    return ((getCenterY() - y));
  // over bounds.
  else if ( ((DEPTH-1) < (getCenterY() + GRID_SIZE)) && ( y < ((getCenterY() + GRID_SIZE)%DEPTH)))
    return ((getCenterY() - (y+DEPTH)));
  // below bounds.
  else if (  ((getCenterY()-GRID_SIZE) < 0) && (y > (getCenterY()-GRID_SIZE+DEPTH)))
    return (getCenterY() - (y-DEPTH));

  //TODO: throw exception?
  return 1215;
}

// Shift the center if the player is too far to the side of the map.
void MapDraw::shiftCenter(Creature *p)
{
  float pos = positionY(p->getY());
  int DISTANCE = GRID_SIZE / 3;

  // if the ant is GRID_SIZE/3 boxes away from the center, start trying to follow it.
  if (pos < (-1* DISTANCE))
    decY();
  else if (pos > DISTANCE) 
    incY();

  pos = positionX(p->getX());
  if (pos < (-1 * DISTANCE))
    decX();
  else if (pos > DISTANCE) 
    incX();
}

bool MapDraw::drawAnt(Creature* a, bool animate)
{
  // top-right - white
  tm->nextTexture(1);

  // ant dead?
  if (a->getHP() <= 0)
    material( 15,10,10 );
  else
    material(3,3,3);

  // exit early if not visible.
  if (! isVisible(a->getX(), a->getY()))
    return false;

  // find X/Y location by finding offset from center.
  // Then add the offset for smoothly moving between 2 squares.
//TODO: Why do I need to add 1 to a->getX() ?!?
  float x = (positionX(a->getX()+1)*MODEL_SCALE) + (a->getOffsetX()*MODEL_SCALE_INCREMENT);
  float y = (positionY(a->getY())*MODEL_SCALE) + (a->getOffsetY()*MODEL_SCALE_INCREMENT);

  if (!animate)
  {
    StaticDraw::drawRect(x, y, MODEL_SCALE*0.2, MODEL_SCALE*.4, MODEL_SCALE*0.9, tm);
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
  glRotatef( rotation      , 0, 0, 1);

  if (a->getType() == ANT_WORKER)
    drawWorker( a->getCarrying() );
  else if (a->getType() == ANT_QUEEN)
    drawQueen( a->getCarrying() ); // queens don't carry anything, but whatev.

  glPopMatrix(1);
  return true;
}

void MapDraw::drawWorker(int carry)
{
  StaticDraw::drawRect(0, 0, MODEL_SCALE*0.15, MODEL_SCALE*.35, MODEL_SCALE*0.9, tm);

  // Check if the ant is carrying anything.  If it is, draw it..
  if (carry != NOTHING)
  {
    glTranslatef(0,MODEL_SCALE*0.50, MODEL_SCALE*0.1);

    // TODO: different types of objects: drawObject (scaled down if being carried).
    if (FOODi(carry))
      material(3,25,3);
    else
      material(31,2,3); // unknown object, red.

    StaticDraw::drawBox(0, 0, 0, MODEL_SCALE*.5, MODEL_SCALE*0.5, MODEL_SCALE*1.0, tm);
  }
}

void MapDraw::drawQueen(int carry)
{
  //StaticDraw::drawRect(0, 0, MODEL_SCALE*0.15, MODEL_SCALE*.35, MODEL_SCALE*0.9, tm);
  // drawRect(x, y, z, width, height, texture);
  StaticDraw::drawRect(0, 0, MODEL_SCALE*0.15, MODEL_SCALE*.45, MODEL_SCALE*1.5, tm);

  // Check if the ant is carrying anything.  If it is, draw it..
  if (carry)
  {
    glTranslatef(0,MODEL_SCALE*0.50, MODEL_SCALE*0.1);

    // TODO: different types of objects: drawObject (scaled down if being carried).
    if (FOODi(carry))
      material(3,25,3);
    else
      material(31,2,3); // unknown object, red.

    StaticDraw::drawBox(0, 0, 0, MODEL_SCALE*.5, MODEL_SCALE*0.5, MODEL_SCALE*0.5, tm);
  }
}

Patch* MapDraw::draw()
{
  // TODO: Fit this one in too so I don't have to to all the MODEL_SCALE multiplications.
//  glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);

  int x,y, temp;
  Patch* bottomLeft = '\0';
  Patch* tp = '\0';

  // Eventually this call will be made to whatever generic character the
  // map should be centered on.  i.e. the player, or an enemy ant.
  bottomLeft = getGrid()->getPatch(getCenterX(), getCenterY());

  // Shift from the center to the bottomLeft.
  // shift left a bunch of times or until we hit the left wall.
  for (temp=0; ((bottomLeft) && (Grid::getLeft(bottomLeft)) && (temp < GRID_SIZE)); temp++)
    bottomLeft = Grid::getLeft(bottomLeft);

  int offsetx = 0;
  // At the bottom, offset...
  if (temp < GRID_SIZE)
    offsetx = GRID_SIZE - temp;

  // shift up a bunch of times or until we hit the top wall.
  for (temp=0; ((bottomLeft) && (Grid::getDown(bottomLeft)) && (temp < GRID_SIZE)); temp++)
    bottomLeft = Grid::getDown(bottomLeft);

  int offsety = 0;
  // At the bottom, offset...
  if (temp < GRID_SIZE)
    offsety = GRID_SIZE - temp;
    //printf("EXTREME");

  tp=bottomLeft;

  // Setup for drawing.
//  glBegin(GL_QUADS);

  for (y=GRID_SIZE*-1 + (offsety); (bottomLeft && (y < (GRID_SIZE+1))); y+=1)
  {
    tp=bottomLeft;
    for (x=GRID_SIZE*-1 + (offsetx); x < (GRID_SIZE+1); x+=1)
    {
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
      tp=tp->right;
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

  // default to top-right (white)
  tm->nextTexture(1);

  // WARNING: this optomization makes it so picking only works from one side of the X/Y plane.
  // (when cam location z > 0)
  if (pickMode)
  {
    StaticDraw::drawRect(x, y, s, s, s, tm);
    // if in pick mode, draw a simple rectangle simply for identifying the location.
    return;
  }

// this is getting in the way for now.
  // If this box is also picked, make a visible highlight in addition to whatever is below..
  if (p->picked)
  {
    // TODO: set alpha level
    // if the box was picked, identify it by coloring red..
    material(31,31,1);
    StaticDraw::drawBox(x, y, 0.1, s, s, 0.1, tm);
  }


// glColor can't work with glNormal, so use materials.
  if (p->TYPE == PATCH_DIRT)
  {
    // dirt texture, top-left
    tm->nextTexture(0);
    // texture "brightness"
    material(31,31,31);
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
      StaticDraw::drawRect(x, y, s, s, s, tm);
      return;
    }

    // Otherwise we need to do a whole cube
    StaticDraw::drawBox(x, y, 0, s, s, s, tm);
    return;
  }
  else if (EGG(p))
  {
    drawEggPatch(x, y, p);
    return;
  }
  else if (FOOD(p))
  {
    drawFoodPatch(x, y, p);
    return;
  }
  else if (EMPTY(p))
  {
    drawGroundPatch(x, y, p);
    // special drawing for this now....
    return;
  }
  else if (p->TYPE == PATCH_ENTRANCE)
  {
    // If the spot isn't empty, this is a dormant hole...
    // this will probably get confusing at some point, will need to always check
    // if portal->TYPE is empty or not.
    if (WALKABLE(p->portal))
    {
      material(1,1,1);
      StaticDraw::drawRect(x, y, 0, s, s, tm);
      // special drawing for this now....
      return;
    }
    else
    {
      drawGroundPatch(x, y, p);
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
    // this rectangle is assuming I will go with the looking-at-wrong-side approach.

    material(1,31,1); // Greenish
    StaticDraw::drawRect(x, y, 0, s, s, tm);
    material(1,1,28); // Bluish
    StaticDraw::drawRect(x, y+MODEL_SCALE, 0, s, s, tm);
    StaticDraw::drawRect(x, y+2*MODEL_SCALE, 0, s, s, tm);
    StaticDraw::drawRect(x, y+2*MODEL_SCALE, 0, s, s, tm);

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
  StaticDraw::drawBox(x, y, 0, s*0.5, s*0.5, s*0.5, tm);
}

void MapDraw::drawEggPatch(float x, float y, Patch *p)
{
  // draw empty at bottom.
  drawGroundPatch(x, y, p);

  material(31,21,10);
  int factor = 1;
  switch(p->TYPE)
  {
    case PATCH_EGG1 :
      factor = 1;
      break;

    case PATCH_EGG2 :
      factor = 2;
      break;

    case PATCH_EGG3 :
      factor = 3;
      break;

    case PATCH_EGG4 :
      factor = 4;
      break;

    case PATCH_EGG5 :
      factor = 5;
      break;

    default:
      factor = 1;
      break;
  }
  
  StaticDraw::drawBox(x, y, 0, (2+factor)*(MODEL_SCALE/10), (2+factor)*(MODEL_SCALE/6), (MODEL_SCALE/3), tm);
}


static float sizes[10] = {
  (3) * (MODEL_SCALE/14),
  (4) * (MODEL_SCALE/14),
  (5) * (MODEL_SCALE/14),
  (6) * (MODEL_SCALE/14),
  (7) * (MODEL_SCALE/14),
  (8) * (MODEL_SCALE/14),
  (9) * (MODEL_SCALE/14),
  (10) * (MODEL_SCALE/14),
  (11) * (MODEL_SCALE/14),
  (12) * (MODEL_SCALE/14) };
// This draws all the food types, the green cube is bigger depending
// on the size of the pile.
void MapDraw::drawFoodPatch(float x, float y, Patch *p)
{
  // draw empty at bottom.
  //drawGroundPatch(x, y, p);

  material(1,31,1);
  StaticDraw::drawRect(x, y, 0, MODEL_SCALE, MODEL_SCALE, tm);
return;
  material(1,31,1);
  int factor = 1;
  switch(p->TYPE)
  {
    case PATCH_FOOD1 :
      factor = 1;
      break;

    case PATCH_FOOD2 :
      factor = 2;
      break;

    case PATCH_FOOD3 :
      factor = 3;
      break;

    case PATCH_FOOD4 :
      factor = 4;
      break;

    case PATCH_FOOD5 :
      factor = 5;
      break;

    case PATCH_FOOD6 :
      factor = 6;
      break;

    case PATCH_FOOD7 :
      factor = 7;
      break;

    case PATCH_FOOD8 :
      factor = 8;
      break;

    case PATCH_FOOD9 :
      factor = 9;
      break;

    case PATCH_FOOD10 :
      factor = 10;
      break;

    default:
      factor = 1;
      break;
  }
  
  StaticDraw::drawBox(x, y, 0, sizes[factor-1], sizes[factor-1], (MODEL_SCALE/15), tm);
}

void MapDraw::drawGroundPatch(float x, float y, Patch *p)
{
  unsigned int cl = 0;

  // if it is low, make it 10 to it so we can see something
  {
//      cl = ((int)(p->chemLevel[showPheramone] / (FERAMONE_MAX / 31)));
      cl = ((int)(p->chemLevel[BLACK_HOME] / (FERAMONE_MAX / 31)));
      if (cl > 31) cl = 31;
  }

  material(31,31-cl,31-cl); // make it easier to see ants...
  // whichever are missing, this will either be a bunch of call lists
  // or, more likely, something I do procedurally:
  //     EMPTY_LEFT
  //     EMPTY_RIGHT
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
  //StaticDraw::drawRect(x, y, 0, MODEL_SCALE, MODEL_SCALE, tm);
  StaticDraw::drawTri(x, y, 0, MODEL_SCALE, MODEL_SCALE, true, tm);

  {
//      cl = ((int)(p->chemLevel[showPheramone] / (FERAMONE_MAX / 31)));
      cl = ((int)(p->chemLevel[BLACK_FOOD] / (FERAMONE_MAX / 31)));
      if (cl > 31) cl = 31;
  }

  material(31-cl,31,31); // make it easier to see ants...
  StaticDraw::drawTri(x, y, 0, MODEL_SCALE, MODEL_SCALE, false, tm);
}

void MapDraw::material(int r, int g, int b)
{
// This seems to work alright.
  //glMaterialf(GL_DIFFUSE, RGB15(r, g, b) | BIT(15)); /// Bit 15 enables the diffuse color to act like being set with glColor(), only with lighting support. When not using lighting, this is going to be the default color, just like being set with glColor().
  glMaterialf(GL_DIFFUSE, RGB15(r, g, b));
  glMaterialf(GL_AMBIENT, RGB15(r, g, b));
  glMaterialf(GL_SPECULAR, RGB15(20, 20, 20)| BIT(15)); /// Bit 15 would have to be set here to enable a custom specularity table, instead of the default linear one.
  glMaterialf(GL_EMISSION, RGB15(0, 0, 0));

  //ds uses a table for shinyness..this generates a half-ass one
  // NOTE: do not use this, at least not here, it slows shit down a lot.
//  glMaterialShinyness();

}


// Picking algorithms, set the polyCount before drawing, check it after.
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
