#ifndef PATCH_H
#define PATCH_H

#include "global.h"

class Patch
{
	public:
		Patch(short s, short w, short d){ slice=s; width=w; depth=d; }
		Patch* setRight(Patch* p){ right = p; }
		Patch* setLeft(Patch* p){ left = p; }
		Patch* setTop(Patch* p){ top = p; }
		Patch* setBottom(Patch* p){ bottom = p; }
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

#endif
