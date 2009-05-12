#ifndef UNDERGROUNDDRAWGRID_H
#define UNDERGROUNDDRAWGRID_H

#include "Underground.h"
#include "StaticDraw.h"
#include "Patch.h"
#include "Camera.h"

// need this for picking.
#include <nds/arm9/postest.h>

class UndergroundDrawGrid: public Underground
{
	public:
		UndergroundDrawGrid();
		Patch* draw();
		void drawPatch(float x, float y, Patch *p);
		void drawBox(float x, float y, float z, float width, float height, float depth);
		void drawRect(float x, float y, float z, float width, float height);

		// This function does picking to find out where in the grid the touch occurred.
		bool pickPoint(int x, int y, Camera &cam);

		int getLeftIndex(){ return centerX - GRID_SIZE; }
		int getRightIndex(){ return centerX + GRID_SIZE; }

		// increment a scene shift some amount before scrolling the tiles.
		virtual void incX(); 
		virtual void decX(); 

		virtual void incY(); 
		virtual void decY(); 

	private:
		void material(int r, int g, int b);

		// Note: these two functions and the rediculously clever picking method
		// was taken directly from Gabe Ghearing's Feb 2007 picking example.
		// 	I added a minor tweak to have the endCheck return a bool rather than store the value.
		// run before starting to draw an object while picking
		void startCheck();
		// run afer drawing an object while picking
		bool endCheck();

		float smoothScrollX, smoothScrollY;
		float boxSide;

		// the following are needed for picking.
		float closeW;
		int polyCount;
		bool pickMode;
		// Storage for picking.
		Patch *picked;
};

#endif
