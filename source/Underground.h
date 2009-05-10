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
										centerY=50; // equator?
									}
		virtual ~Underground(){ delete underground; }
		virtual void draw() = 0;

		DirtDisk* getDisk()
			{	return underground; }

//TODO: fix this, commented out so that UndergroundDraw could access these...?
//	private:
		std::vector<Ant> inhabitants;
		DirtDisk *underground;
		int centerX, centerY, slice;
		float rotationX, rotationY;
};

#endif
