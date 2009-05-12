#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include <vector>
#include "Ant.h"
#include "DirtDisk.h"

class Underground
{
	public:
		// TODO: this will need to be slightly restructured to allow multiple nests
		//				so that a huge chunk of memory for multiple "DirtDisks" will not
		//				be needed.
		Underground(){	underground = new DirtDisk(); 
										undergroundSize = 30;
										underground->generateNest(undergroundSize);
										centerX=0;
										slice=0;
										centerY=0;
									}
		virtual ~Underground(){ delete underground; }

		// reference to slice and centerX passed in.
		// This is for increasing the index into the underground grid.
		virtual void incX(){ underground->moveRight(slice, centerX); }
		virtual void decX(){ underground->moveLeft(slice, centerX); }

		virtual void incY(){ underground->moveUp(centerY); }
		virtual void decY(){ underground->moveDown(centerY); }
		DirtDisk* getDisk()
			{	return underground; }

//TODO: fix this, commented out so that UndergroundDraw could access these...?
//	private:
		std::vector<Ant> inhabitants;
		int undergroundSize;
		DirtDisk *underground;
		short centerX, centerY, slice;
		float rotationX, rotationY;
};

#endif
