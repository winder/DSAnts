#include "Grid.h"

Grid::Grid()
{
  int x,y;

  // Create the objects.
  for (x=0; x < WIDTH; x++)
    for (y=0; y < DEPTH; y++)
    {
      dd[x][y] = new Patch();

      // setting it to this just so I can use the "clear" method without anything special.
      dd[x][y]->TYPE = PATCH_DIRT;
      dd[x][y]->x = x;
      dd[x][y]->y = y;
      dd[x][y]->portal = '\0';
      dd[x][y]->chemLevel = 0;
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
    dd[x][0]->top            = dd[x][DEPTH-1];
    dd[x][DEPTH-1]->bottom  = dd[x][0];
  }
  loopY = true;
}

void Grid::moveRight(short &x)
{
  // Width roll-over
  if ( x != WIDTH - 1 )
    x = x + 1;
  else
    x = 0;
}

void Grid::moveLeft(short &x)
{
  // Width roll-over
  if ( x >= 1 )
    x = x - 1;
  else
    x = WIDTH - 1;
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
