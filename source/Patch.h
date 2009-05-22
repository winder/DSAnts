#ifndef PATCH_H
#define PATCH_H

#include "global.h"


// Simple generic functions:
// Note: I don't really know much about designing macros, it made sense to cascade
// them like this, but it could get confusing.
// TODO: re-evaluate how I'm creating patch comparisons.

#define WALKABLE(X) WALKABLEi(X->TYPE)
#define WALKABLEi(X) ((X == PATCH_EMPTY) || (X == PATCH_ENTRANCE) || (X == PATCH_TOP) || (X == PATCH_EGG) || FOODi(X))

#define FOOD(X) FOODi(X->TYPE)
#define FOODi(X) ((X == PATCH_FOOD1) ||(X == PATCH_FOOD2) ||(X == PATCH_FOOD3) ||(X == PATCH_FOOD4) ||\
(X == PATCH_FOOD5) ||(X == PATCH_FOOD6) ||(X == PATCH_FOOD7) ||(X == PATCH_FOOD8) ||(X == PATCH_FOOD9) ||(X == PATCH_FOOD10)) 

#define OBJECT(X) OBJECTi(X->TYPE)
#define OBJECTi(X) (FOODi(X) || (X == PATCH_EGG))

#define EMPTY(X) EMPTYi(X->TYPE)
#define EMPTYi(X) ((X == PATCH_EMPTY))


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
	// this flag should be moved into a bit of TYPE eventually.
	short location;
	// yeah, I want these in here, sorry memory.
	short x;
	short y;

	// Some patches have a 5th direction, where they portal to another map.
	Patch *portal;

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
