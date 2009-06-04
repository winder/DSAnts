#ifndef QUEENANT_H
#define QUEENANT_H

#include "Ant.h"

class QueenAnt: public Ant
{
  public:
    QueenAnt():Ant(){ gestation = 0; };
    QueenAnt(Patch *pat, int location):Ant(pat, location){ gestation = 0; };
    virtual int getType(){ return ANT_QUEEN; }
    

    virtual void stateStep(int num);
    virtual void moveAI();
    //virtual void wander();
    //virtual void attack();
    //virtual void forage();

  private:
    short gestation;
};

#endif
