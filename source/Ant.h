#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant: public Creature
{
  public:
    Ant():Creature(){};
    Ant(Patch *pat, int location):Creature(pat, location){};
    virtual int getType() = 0;
    virtual void handleFeramone();

    virtual void forage();
    // check adjacent Patches for food.
    Patch* checkForFood();


};

#endif
