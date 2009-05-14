#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include "Ant.h"
#include "Grid.h"
#include "MapDraw.h"

class Underground: public MapDraw
{
	public:
		// This is structured to make a tunnel.  Need to change it to allow
		// surface maps as well.
		// TODO: this will need to be slightly restructured to allow multiple nests
		//				so that a huge chunk of memory for multiple "Grid's" will not
		//				be needed.
		Underground(){	tunnelSize = 60;
										setCenter(0, 0);
										initGrid();
									}
		virtual ~Underground(){ /* superclass destructure deletes */}

		virtual void initGrid()
		{
			getGrid()->generateNest(tunnelSize);
		}

		// Adds an ant in a random location.
		void addAnt(Ant *a);

		void addAnt(Ant *a, int x, int y);

	private:
		int tunnelSize;
};

#endif
