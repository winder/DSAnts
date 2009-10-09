#ifndef DIRTDISK_H
#define DIRTDISK_H


//#include <nds.h>
#include <time.h>
#include <vector>
// stdlib for rand / srand
#include <stdlib.h>

#include "global.h"
#include "Patch.h"
//#include "Dirt.h"
//#include "Empty.h"
//#include "Barrier.h"

class Grid
{
  public:
    // Default initializes Grid to underground.
    Grid();

    // if the map needs to keep track of the game speed.
    virtual void gameTick(int num){ }

    // Make the grid loop on the Y axis.
    void setLoopY();
    void setLoopX();

    // clamp the values just in case.
    Patch* getPatch(short width, short depth)
      {  if (width<0) width=0; if (depth<0) depth=0;
        return dd[width%WIDTH][depth%DEPTH]; }

    // move the x/y coord around.
    bool moveRight(short &x);
    bool moveLeft(short &x);
    bool moveUp(short &y);
    bool moveDown(short &y);

    // move the Patch around.
    static Patch* getRight(Patch* p);
    static Patch* getLeft(Patch* p);
    static Patch* getUp(Patch* p);
    static Patch* getDown(Patch* p);
    static Patch* getUpLeft(Patch* p){ return Grid::getUp( Grid::getLeft(p)); }
    static Patch* getUpRight(Patch* p){ return Grid::getUp( Grid::getRight(p)); }
    static Patch* getDownLeft(Patch* p){ return Grid::getDown( Grid::getLeft(p)); }
    static Patch* getDownRight(Patch* p){ return Grid::getDown( Grid::getRight(p)); }

    Patch* getRight(int x, int y);
    Patch* getLeft(int x, int y);
    Patch* getTop(int x, int y);
    Patch* getBottom(int x, int y);

    ///////////////////
    // Modify Tiles: //
    ///////////////////

    // Keep track of cleared tiles in a vector.
    void clear(Patch* p);

    // Keep track of food tiles.
    void addObject(Patch* p, int Ob);
    void takeObject(Patch* p);

    // Keep STL structure abstracted from other classes.
    // sorry, I need it.
    std::vector<Patch*> getCleared(){ return cleared; }
    //std::vector<Patch*> getObjects(){ return objects; }

    ////////////////
    // Accessors: //
    ////////////////
    int numCleared(){ return cleared.size(); }
    int numObjects(){ return objects.size(); }

    // the whole reason of maintaining these lists is so we can get a random one
    // without searching the entire grid.
    Patch* getRandomCleared(){ return cleared[ rand()%(cleared.size()) ]; }
    Patch* getRandomObject(){ return objects[ rand()%(objects.size()) ]; }

    // Figure out the distance between two points.
    int distanceBetween(int x1, int y1, int x2, int y2);
    static int distanceBetweenStatic(int x1, int y1, int x2, int y2);
    static int directionTo(int fromx, int fromy, int tox, int toy);

    int getLocation(){ return location; }

  // This is available to subclasses.
  protected:
    Patch* dd[WIDTH][DEPTH];
    int location;

  private:
    // Grids can optionally be set to loop.
    bool loopX;
    bool loopY;

    // a list of certain tiles that I may want to information about.
    std::vector<Patch*> cleared;
    std::vector<Patch*> objects;
};

#endif
