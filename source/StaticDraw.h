#ifndef STATICDRAW_H
#define STATICDRAW_H

#include <nds.h>
#include "Underground.h"
#include "global.h"

class StaticDraw
{
	public:
		static void draw(Underground* ug, short s, short w, short h);
		static void draw(Patch* p);
		static void Box(float x, float y, float z, float width, float height, float depth);
};

#endif
