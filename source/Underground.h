#ifndef UNDERGROUND_H
#define UNDERGROUND_H

//#include "Ant.h"
#include "Grid.h"
#include "MapDraw.h"

class Underground: public Grid
{
  public:
    // This is structured to make a tunnel.  Need to change it to allow
    // surface maps as well.
    // TODO: this will need to be slightly restructured to allow multiple nests
    //        so that a huge chunk of memory for multiple "Grid's" will not
    //        be needed.
    Underground();
    Underground(int locName);
    virtual ~Underground(){ /* superclass destructure deletes */}

    virtual void initGrid(int location);

    // Sets up the Patches for the underground.
    void setupUnderground(int location);

    // This will generate a random nest with 'size' connected empty spaces
    // It will have one entrance which is always at (0,20,0)
    void generateNest(int size);

    // Adds an ant in a random location.
//    void addAnt(Ant *a);

//    void addAnt(Ant *a, int x, int y);

  private:
    int tunnelSize;
};

#endif
