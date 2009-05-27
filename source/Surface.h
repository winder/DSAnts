#ifndef SURFACE_H
#define SURFACE_H

#include "MapDraw.h"

class Surface: public MapDraw
{
  public:
    // The surface is pretty much empty.
    // TODO: remove X wrapping or add Y wrapping for the surface.
    Surface();
    virtual ~Surface(){ /* superclass deletes structure */ }

    // Sets up the initial state of the grid.
    void initGrid();

    // Creates a patch of food.
    void makeFood();

    // Note: Don't want to call this one too often because it scrolls through the whole map.
    //       Its the price I pay for not having the overhead to make extra lists of everything.
    void checkFood();
};

#endif
