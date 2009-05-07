#ifndef PATCH_H
#define PATCH_H

class Patch
{
	public:
		Patch(short s, short w, short d){ slice=s; width=w; depth=d; }
		Patch* getRight(){ return right; }
		Patch* getLeft(){ return left; }
		Patch* getTop(){ return top; }
		Patch* getBottom(){ return bottom; }

	private:
		Patch *left;
		Patch *right;
		Patch *top;
		Patch *bottom;
		short slice, width, depth;
};

#endif
