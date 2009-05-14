#ifndef PATCH_H
#define PATCH_H

#include "global.h"

struct Patch
{
	Patch *left;
	Patch *right;
	Patch *top;
	Patch *bottom;
	// I'm thinking of making this a long and having lots of flags in addition to
	// just a TYPE, then having a bunch of macro's to convert TYPE into usable parts.
	short TYPE;
	bool picked;
	// yeah, I want these in here, sorry memory.
	short x;
	short y;

// something like this will be useful at some point, these could be used as a TYPE though.
//	Ant* occupant?
//	bool occupied?
};

/*
class Patch
{
	public:
		Patch(short s, short w, short d){ slice=s; width=w; depth=d; }
		void setRight(Patch* p){ right = p; }
		void setLeft(Patch* p){ left = p; }
		void setTop(Patch* p){ top = p; }
		void setBottom(Patch* p){ bottom = p; }
		Patch* getRight(){ return right; }
		Patch* getLeft(){ return left; }
		Patch* getTop(){ return top; }
		Patch* getBottom(){ return bottom; }

		short getSlice(){ return slice; }
		short getWidth(){ return width; }
		short getDepth(){ return depth; }

		virtual int getType(){ return PATCH; }

	private:
		Patch *left;
		Patch *right;
		Patch *top;
		Patch *bottom;
		short slice, width, depth;
};
*/
#endif
