#ifndef DIRT_H
#define DIRT_H

#include "Patch.h"

class Dirt: public Patch
{
	public:
		Dirt(short x, short y, short z): Patch(x,y,z) { }
};

#endif
