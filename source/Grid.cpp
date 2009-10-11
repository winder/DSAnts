#include "Grid.h"

Grid::Grid()
{
  // this is always true... for now.
  loopX = true;
  int x,y;

  // Create the objects.
  for (x=0; x < WIDTH; x++)
    for (y=0; y < DEPTH; y++)
    {
      dd[x][y] = new Patch(x, y);
    }

  // Connect the objects.
  for (x=0; x < WIDTH; x++)
    for (y=0; y < DEPTH; y++)
    {
      dd[x][y]->top = getTop(x,y);
      dd[x][y]->bottom = getBottom(x,y);
      dd[x][y]->left = getLeft(x,y);
      dd[x][y]->right = getRight(x,y);
    }
}

void Grid::setLoopY()
{
  int x = 0;
  for(x=0; x < WIDTH;  x++)
  {
    dd[x][0]->top           = dd[x][DEPTH-1];
    dd[x][DEPTH-1]->bottom  = dd[x][0];
  }
  loopY = true;
}

bool Grid::moveRight(short &x)
{
  // Width roll-over
  if ( x != WIDTH - 1 )
    x = x + 1;
  else if (loopX)
    x = 0;
  else
    return false;
  return true;
}

bool Grid::moveLeft(short &x)
{
  // Width roll-over
  if ( x >= 1 )
    x = x - 1;
  else if (loopX)
    x = WIDTH - 1;
  else
    return false;
  return true;
}

bool Grid::moveUp(short &y)
{
  // SLICE and WIDTH don't change.
  if ( y != 0 )
    y--;
  else if (loopY)
    y = DEPTH-1;
  else
    return false;

  return true;
}

bool Grid::moveDown(short &y)
{
  // SLICE and WIDTH don't change.
  if ( y != DEPTH - 1 )
    y++;
  else if (loopY)
    y = 0;
  else
    return false;

  return true;
}

Patch* Grid::getRight(Patch* p)
{
  return p->right;
}

Patch* Grid::getLeft(Patch* p)
{
  return p->left;
}

Patch* Grid::getUp(Patch* p)
{
  return p->top;
}

Patch* Grid::getDown(Patch* p)
{
  return p->bottom;
}

// use the reference methods above to take a pointer and get the one left to it.
Patch* Grid::getRight(int x, int y)
{
  short w = x;
  moveRight(w);

  // Depth max/min doesn't change moving right/left
  return dd[w][y];
}

Patch* Grid::getLeft(int x, int y)
{
  short w = x;
  moveLeft(w);

  // Depth max/min doesn't change moving right/left
  return dd[w][y];
}
Patch* Grid::getTop(int x, int y)
{
  short d = y;
  if (!moveUp(d)) return '\0';

  return dd[x][d];
}
Patch* Grid::getBottom(int x, int y)
{
  short d = y;
  if (!moveDown(d)) return '\0';

  return dd[x][d];
}

void Grid::clear(Patch* p)
{
  if (p == '\0') return;

  if (p->TYPE == PATCH_DIRT)
  {
    p->TYPE = PATCH_EMPTY;
    cleared.push_back(p);
  }
}

void Grid::takeObject(Patch* p)
{
  if (p == '\0') return;

  if (OBJECT(p))
  {
    p->TYPE = PATCH_EMPTY;
    // searth through objects for the object being taken.
    // When (if?) found, remove it from the list as it is now empty.
    // TODO: I can probably get around to doing this only periodically, since
    //        I can always just check if the patch is an object first.
    for(unsigned int i=0; i < objects.size(); i++)
      if ( objects[i] == p )
        objects.erase( objects.begin() + i, objects.begin() + i + 1);
  }
}

void Grid::addObject(Patch* p, int Ob)
{
  if (p == '\0') return;

  if (EMPTY(p) && OBJECTi(Ob))
  {
    p->TYPE = Ob;
    objects.push_back(p);
  }
}

void Grid::gameTick(int num)
{
  chemicalTick += num;
  if (chemicalTick > CHEMICAL_DECAY_FREQUENCY)
  {
    chemicalTick -= CHEMICAL_DECAY_FREQUENCY;
    chemicalDecay(0.9);
  }
}

void Grid::chemicalDecay(float f)
{
  for (int x=0; x < WIDTH; x++)
    for (int y=0; y < DEPTH; y++)
    {
      DECAY_FERAMONE( dd[x][y] , f);
    }
}

#include <stdio.h>
// Figure out the distance between two points.
int Grid::distanceBetween(int x1, int y1, int x2, int y2)
{
  // out of array bounds.
  if ((x1 < 0) || (y1 < 0) || (y1 >= DEPTH) || (x1 >= WIDTH) ||
      (x2 < 0) || (y2 < 0) || (y2 >= DEPTH) || (x2 >= WIDTH))
    return -1;

  // Find x minimum
  int xmin = abs(x1-x2);
  if (loopX && ((WIDTH - xmin) < xmin))
    xmin = WIDTH - xmin + 1;

  // Find y minimum
  int ymin = abs(y1-y2);
  if (loopY && ((DEPTH - ymin) < ymin))
    ymin = DEPTH - ymin + 1;

  return (xmin > ymin) ? xmin : ymin;
}

int Grid::distanceBetweenStatic(int x1, int y1, int x2, int y2)
{
  // out of array bounds.
  if ((x1 < 0) || (y1 < 0) || (x2 < 0) || (y2 < 0))
    return -1;

  // Find x minimum
  int xmin = abs(x1-x2);

  // Find y minimum
  int ymin = abs(y1-y2);

  return (xmin > ymin) ? xmin : ymin;
}

int Grid::directionTo(int fromx, int fromy, int tox, int toy)
{
  // out of array bounds.
  if ((fromx < 0) || (fromy < 0) || (tox < 0) || (toy < 0))
    return -1;

  // Right/Left
  if (rand()%2 == 1)
  {
    if ((fromx - tox) < 0) return AI_RIGHT;
    if ((fromx - tox) > 0) return AI_LEFT;
  }
  if ((fromy - toy) < 0) return AI_DOWN;
  if ((fromy - toy) > 0) return AI_TOP;

  return -1;
}
