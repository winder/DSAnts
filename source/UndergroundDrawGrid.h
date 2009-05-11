#ifndef UNDERGROUNDDRAWGRID_H
#define UNDERGROUNDDRAWGRID_H

#include "Underground.h"
#include "StaticDraw.h"
#include "Patch.h"

class UndergroundDrawGrid: public Underground
{
	public:
		UndergroundDrawGrid();
		void draw();
		void drawBox(float x, float y, Patch *p);

		int getLeftIndex(){ return centerX - GRID_SIZE; }
		int getRightIndex(){ return centerX + GRID_SIZE; }

		// increment a scene shift some amount before scrolling the tiles.
		virtual void incX(); 
		virtual void decX(); 

		virtual void incY(); 
		virtual void decY(); 

	private:
		float smoothScrollX, smoothScrollY;
		float boxSide;
};

#endif
