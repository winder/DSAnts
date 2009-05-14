#ifndef STATICDRAW_H
#define STATICDRAW_H

#include <nds.h>
#include <math.h>
#include "MapDraw.h"
#include "global.h"

typedef struct{
	float x,y,z;
} verts;

class StaticDraw
{
	public:
		StaticDraw(){ cache = false; }
//		void draw(MapDraw* ug, short s, short w, short h);
		void draw(Patch* p);
		void Box(float x, float y, float z, float width, float height, float depth);
		void Sphere(float x, float y, float z, float radius);
		static float d2r(float degrees){
        const double conversion = 3.1416f/180.0f;
        return degrees * conversion; }

	private:
		verts v[7][7];

		bool cache;
};

#endif
