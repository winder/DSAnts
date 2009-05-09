#ifndef STATICDRAW_H
#define STATICDRAW_H

#include <nds.h>
#include "Underground.h"
#include "global.h"
#include <math.h>

		 typedef struct{
			float x,y,z;
			} verts;
class StaticDraw
{
	public:
		StaticDraw(){ cache = false; }
		void draw(Underground* ug, short s, short w, short h);
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
