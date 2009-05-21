#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Underground.h"
#include "Surface.h"
#include <vector>
#include "Ant.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "Observer.h"
// stdlib for rand / srand
#include <stdlib.h>

//#ifdef __DEBUG_
#include <stdio.h>
//#endif
#ifdef __PROFILING
	#include "cyg-profile.h"
#endif

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
class GameWorld: public Observer, public Subject
{
	public:
		GameWorld();
		~GameWorld();

		// Link the surface with the underground.
		void linkSurfaceAntUnderground();

		void changeState(int st){ STATE = st; }

		void draw();
		void pickPoint(short x, short y);

		// TODO: This will move the player, and depending on player information will
		//       then move the screen.
// This clearly isn't used anymore, since "ug" was replaced with "curMap"
//		void incX(){ ug->incX(); }
//		void incY(){ ug->incY(); }
//		void decX(){ ug->decX(); }
//		void decY(){ ug->decY(); }

		Underground* getUG(){ return ug; }
		Surface* getSurface(){ return surf; }

		// Get INPUT, everything else will use input as though
		// it is up to date.
		int getInput();

		// Coordinates a step in the game.
		void stepForward(int num);
		// lets each ant know they can move forward one step.
		void stepAntsForward(int num);

		// Camera needs to set itself up.
		void initCam(){	cam->init();
									cam->translateZinc(2.0); }
		void initTex()
			{//	ug->setTextures(tex);
				// surf->setTextures(tex); }
				ug->initTextures();
				surf->setTextures( ug->getTextures() ); }

		// have the camera look at the player!
		// TODO: this is an interesting concept, but it needs some work to... work.
/*
		void placeCamera(){
						cam->render(
									ug->positionX(
											p->getPlayerAnt()->getX()) + p->getPlayerAnt()->getOffsetX()*MODEL_SCALE_INCREMENT ,
									ug->positionY(
											p->getPlayerAnt()->getY()) + p->getPlayerAnt()->getOffsetY()*MODEL_SCALE_INCREMENT		); }
*/
		void placeCamera(){ cam->render(); }
		void setProjection();


		// Event handling: Observer implementation:
		virtual void update(int value);
		
		// #ifdef __DEBUG_
		void printDebugFiveLines();
		// #endif
	private:
		int numAnts;
		// store the picked patch.
		bool doPick;
		Patch *picked;
		bool automove;

		// cache for if the input is new.
		Input *in;

		// GAMEWORLD_STATE_SURFACE, GAMEWORLD_STATE_UNDERGROUND
		short STATE;
		// should use polymorphism so that I don't have to do everything twice with
		// underground and surface.
		MapDraw* curMap;
		Underground *ug;
		Surface *surf;

		Player *p;
		// in most cases this will be true, but there may be cases where
		// I want to follow something else (another ant, spider, etc).
		bool followingPlayer;

		std::vector<Ant*> black;
		std::vector<Ant*> red;

		// Camera
		Camera *cam;

		int held, pressed;
		int touchX, touchY;
		int oldX, oldY;
		int curX, curY;
};

#endif
