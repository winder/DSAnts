#ifndef WORKER_ANT
#define WORKER_ANT

#include "Ant.h"

class WorkerAnt: public Ant
{
  public:
    WorkerAnt():Ant(){};
    WorkerAnt(Patch *pat, int location):Ant(pat, location){};
    virtual int getType(){ return ANT_WORKER; }

};

#endif
