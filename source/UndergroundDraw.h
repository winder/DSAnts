#ifndef UNDERGROUNDDRAW_H
#define UNDERGROUNDDRAW_H

#include "Underground.h"
#include "StaticDraw.h"
#include "Patch.h"

class UndergroundDraw: public Underground
{
	public:
		UndergroundDraw(): Underground()
		{
			cached = false;
			computeCircles();
		}

		void draw();
		void drawSegment(int x, int y, int rightx, int upy, Patch* p);
		void computeCircles();

	private:
		bool cached;
		const static float innerR = 2;
		const static float outerR = 2.1;
		const static int lats = 100;
		const static int longs = 100;
		VectorF inner[lats][longs];
		VectorF outer[lats][longs];

		// need some sort of storage to know where to map the patches to the sphere.
		int centerPointX, centerPointY;
};

#endif
