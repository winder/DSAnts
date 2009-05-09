#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include <vector>
#include "Ant.h"
#include "DirtDisk.h"

class Underground
{
	public:
		Underground(){ underground = new DirtDisk(); }
		virtual ~Underground(){ delete underground; }
		virtual void draw() = 0;

		DirtDisk* getDisk()
			{	return underground; }

	private:
		std::vector<Ant> inhabitants;
		DirtDisk *underground;
		int centerX, centerY;
		float rotationX, rotationY;
};

#endif
