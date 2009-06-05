#ifndef MAP_H
#define MAP_H

#include "Creature.h"
#include "Grid.h"

class Map
{
  public:
    // This is structured to make a tunnel.  Need to change it to allow
    // surface maps as well.
    // TODO: this will need to be slightly restructured to allow multiple nests
    //        so that a huge chunk of memory for multiple "Grid's" will not
    //        be needed.
    Map()
      {
        grid = new Grid(); 
        centerX=0;
        centerY=0;
      }
    Map(Grid *g)
      {
        grid = g;
        centerX=0;
        centerY=0;
      }
    virtual ~Map(){ delete grid; }

    void gameTick(int num){ grid->gameTick(num); }
    //virtual void initGrid() = 0;

    // Adds an ant in a random location.
    void addAnt(Creature *a);

    void addAnt(Creature *a, int x, int y);

    // This is for increasing the index into the underground grid.
    // Used primarily for determining where the map should be pointing.
    virtual void incX(){ grid->moveRight(centerX); }
    virtual void decX(){ grid->moveLeft(centerX); }

    virtual void incY(){ grid->moveUp(centerY); }
    virtual void decY(){ grid->moveDown(centerY); }

    void setGrid(Grid *g){ grid = g; };

    Grid* getGrid()
      {  return grid; }

    void setCenter(short x, short y){ centerX=x; centerY=y; }
    void setCenterX(short x){ centerX=x; }
    void setCenterY(short y){ centerY=y; }
    short getCenterX(){ return centerX; }
    short getCenterY(){ return centerY; }

    // Move the center.
    bool moveCenterRight(){ grid->moveRight(centerX); return true; }
    bool moveCenterLeft(){ grid->moveLeft(centerX); return true; }
    bool moveCenterUp(){ return grid->moveUp(centerY); }
    bool moveCenterDown(){ return grid->moveDown(centerY); }

  private:
    Grid *grid;
    short centerX, centerY;
    // not sure what these were for.
    //float rotationX, rotationY;
};

#endif
