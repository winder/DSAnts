#ifndef DIRT_H
#define DIRT_H

#include "Patch.h"
#include "global.h"

class Dirt: public Patch
{
	public:
		Dirt(short x, short y, short z): Patch(x,y,z) { }
		virtual int getType(){ return PATCH_DIRT; }
};

#endif
