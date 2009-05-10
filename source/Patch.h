#ifndef PATCH_H
#define PATCH_H

#include "global.h"

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

		// TODO: fix this stuff
		void setColor(float r, float g, float b)
			{ red=r; green=g; blue=b; }
		float getR(){ return red; }
		float getG(){ return green; }
		float getB(){ return blue; }

	private:
		Patch *left;
		Patch *right;
		Patch *top;
		Patch *bottom;
		short slice, width, depth;

		float red,green,blue;
};

#endif
