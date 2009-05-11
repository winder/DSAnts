#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include <vector>
#include "Ant.h"
#include "DirtDisk.h"

class Underground
{
	public:
		Underground(){	underground = new DirtDisk(); 
										centerX=0;
										slice=0;
										centerY=0;
									}
		virtual ~Underground(){ delete underground; }
//		virtual void draw() = 0;

		// reference to slice and centerX passed in.
		// This is for increasing the index into the underground grid.
		void incX(){ underground->moveRight(slice, centerX); }
		void decX(){ underground->moveLeft(slice, centerX); }

		void incY(){ underground->moveUp(centerY); }
		void decY(){ underground->moveDown(centerY); }
		DirtDisk* getDisk()
			{	return underground; }

//TODO: fix this, commented out so that UndergroundDraw could access these...?
//	private:
		std::vector<Ant> inhabitants;
		DirtDisk *underground;
		short centerX, centerY, slice;
		float rotationX, rotationY;
};

#endif
