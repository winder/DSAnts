#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#define UNDERGROUND_S 1
#define SURFACE_S 2

#include "UndergroundDrawGrid.h"
#include <vector>
#include "Ant.h"
#include "Camera.h"

class GameWorld
{
	public:
		GameWorld();
		~GameWorld();
		void draw();
		void pickPoint(short x, short y, Camera cam);
		// TODO: This will move the player, and depending on player information will
		//       then move the screen.
		void incX(){ ug->incX(); }
		void incY(){ ug->incY(); }
		void decX(){ ug->decX(); }
		void decY(){ ug->decY(); }

		UndergroundDrawGrid* getUG(){ return ug; }
	private:
		short STATE;
		UndergroundDrawGrid *ug;
		//Surface &surf;

		//Player p;
		Ant* tester;
		std::vector<Ant*> black;
		//std::vector<Ant> red;
};

#endif
