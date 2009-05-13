#include "GameWorld.h"

GameWorld::GameWorld()
{
	cam = new Camera();
	cam->translateZinc(3.5);

	ug = new UndergroundDrawGrid();

	tester = new Ant();
	tester->setXY(0, 2);
	tester->setPatch( ug->getDisk()->getPatch(0,2) );

	STATE = UNDERGROUND_S;
}

GameWorld::~GameWorld()
{
	delete ug;
}

void GameWorld::draw()
{
	// Draw the SCENE
	if (STATE == UNDERGROUND_S)
	{
			ug->draw();
			//ug->drawAnts(black, red);
			ug->drawAnt(tester);
	}
	else if (STATE == SURFACE_S)
	{
		//surf->draw();
	}

	// DO THE PICKING
	if (STATE == UNDERGROUND_S)
	{
		// If the touch pad is being touched... see what its touching.
		if( held & KEY_TOUCH)
			pickPoint(curX, curY);
	}
	else if (STATE == SURFACE_S)
	{
	}
}

void GameWorld::pickPoint(short x, short y)
{
	if (STATE == UNDERGROUND_S)
		ug->pickPoint(x, y, *cam);
	else if (STATE == SURFACE_S)
	{
		//surf->pickPoint();
	}
}

int GameWorld::getInput()
{
	return in->getPressed();
}

void GameWorld::stepForward()
{
	// GET INPUT and STORE IT
	scanKeys();
	held = keysHeld();
	pressed = keysDown();

	// Camera zoom with triggers
	if( held & KEY_R) cam->translateZinc(0.5);
	if( held & KEY_L) cam->translateZinc(-0.5);

	// move the player
/* // Move map:
	if( held & KEY_LEFT)	ug->decX();
	if( held & KEY_RIGHT)	ug->incX();
	if( held & KEY_UP)		ug->incY();
	if( held & KEY_DOWN)	ug->decY();
*/
	if( held & KEY_LEFT)	tester->moveLeft();
	if( held & KEY_RIGHT)	tester->moveRight();
	if( held & KEY_UP)		tester->moveUp();
	if( held & KEY_DOWN)	tester->moveDown();

	// TODO: this will be the player ant:
	ug->shiftCenter(tester);

	// no draw here, it is handled elsewhere so that things will be able
	// to move forward if things start to lag.

	touchPosition touchXY;
	// snag touch info from hardware.
	touchRead(&touchXY);

	curX = touchXY.px;
	curY = touchXY.py;

int oldX, oldY;
	//reset x and y when user touches screen
	if( pressed & KEY_TOUCH)  {
		oldX = touchXY.px;
		oldY = touchXY.py;
	}

	//if user drags then grab the delta
	if( held & KEY_TOUCH) {
		touchY += touchXY.px - oldX; 
		touchX += touchXY.py - oldY;
		oldX = touchXY.px;
		oldY = touchXY.py;
	}

}


void GameWorld::setProjection()
{
	//change ortho vs perspective
	if(held & KEY_B)
		cam->Ortho();
	else 
		cam->Perspective();
}
