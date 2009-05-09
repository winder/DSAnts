#ifndef UNDERGROUNDDRAW_H
#define UNDERGROUNDDRAW_H

#include "Underground.h"
#include "StaticDraw.h"

class UndergroundDraw: public Underground
{
	public:
		UndergroundDraw(): Underground()
		{
			cached = false;
			computeCircles();
		}

		void draw();
		void drawSegment(int x, int y, int rightx, int upy);
		void computeCircles();

	private:
		bool cached;
//		const static int lats = 50;
//		const static int longs = 15;
		const static int lats = 50;
		const static int longs = 50;
		VectorF inner[lats+1][longs+1];
		VectorF outer[lats+1][longs+1];
};

#endif
