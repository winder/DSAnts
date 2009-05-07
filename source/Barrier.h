#ifndef BARRIER_H
#define BARRIER_H

#include "Patch.h"
#include "global.h"

class Barrier: public Patch
{
	public:
		Barrier(short x, short y, short z): Patch(x,y,z) { }
		virtual int getType(){ return PATCH_BARRIER; }
};

#endif
