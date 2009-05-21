#ifndef WORKER_ANT
#define WORKER_ANT

#include "Ant.h"

class WorkerAnt: public Ant
{
	public:
		virtual int getType(){ return ANT_WORKER; }

};

#endif
