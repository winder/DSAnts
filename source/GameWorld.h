#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#define UNDERGROUND_S 1
#define SURFACE_S 2

#include "UndergroundDrawGrid.h"
#include <vector>
#include "Ant.h"
#include "Camera.h"
#include "Input.h"


// TODO: this class would make more sense as a "GameSquare" if I decide to go
//			 the original SimAnt route and have multiple colonies spanning a grid.
// TODO: this class should be broken apart from all the camera and other crap
//			 I stuck in here, those should go somewhere else and use this class to
//			 get needed info.

// This class is designed to hold all the pieces of the world together.
// Major pieces include:
//			Underground / Surface world information.
//			Creature (black/red ants, spiders, pill bugs, etc) information.
//			Object (Food, eggs, rocks, etc) information.
class GameWorld
{
	public:
		GameWorld();
		~GameWorld();
		void draw();
		void pickPoint(short x, short y);
		// TODO: This will move the player, and depending on player information will
		//       then move the screen.
		void incX(){ ug->incX(); }
		void incY(){ ug->incY(); }
		void decX(){ ug->decX(); }
		void decY(){ ug->decY(); }

		UndergroundDrawGrid* getUG(){ return ug; }

		// Get INPUT, everything else will use input as though
		// it is up to date.
		int getInput();

		void stepForward();

		// Camera needs to set itself up.
		void init(){	cam->init();
									cam->translateZinc(3.5); }

		void placeCamera(){ cam->render(); }

		void setProjection();
	private:
		// cache for if the input is new.
		bool newInput;
		Input *in;

		short STATE;
		UndergroundDrawGrid *ug;
		//Surface &surf;

		//Player p;
		Ant* tester;
		std::vector<Ant*> black;
		//std::vector<Ant> red;

		// Camera
		Camera *cam;

		int held, pressed;
		int touchX, touchY;
		int oldX, oldY;
		int curX, curY;
};

#endif
