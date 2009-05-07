#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include <vector>
#include "Ant.h"
#include "DirtDisk.h"

class Underground
{
	public:
		Underground(){}

	private:
		std::vector<Ant> inhabitants;
		DirtDisk underground;

};

#endif
