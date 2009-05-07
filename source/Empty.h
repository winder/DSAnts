#ifndef EMPTY_H
#define EMPTY_H

#include "Patch.h"
#include "global.h"

class Empty: public Patch
{
	public:
		Empty(short x, short y, short z): Patch(x,y,z) { }
		virtual int getType(){ return PATCH_EMPTY; }
};

#endif
