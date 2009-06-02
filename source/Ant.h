#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant: public Creature
{
  public:
    Ant():Creature(){ feramoneOutput = 100; }
    Ant(Patch *pat, int location):Creature(pat, location)
      { feramoneOutput = 100; }
    virtual int getType() = 0;
    virtual void handleFeramone();

    virtual void forage();
    // check adjacent Patches for food.
    Patch* checkForFood();


    // return as argument.
    void sortAdjacentPatchByChem(Patch* center, Patch* sort[], int direction[]);

  private:
    short feramoneOutput;

};

#endif
