#include "GameWorld.h"

GameWorld::GameWorld()
{
	cam = new Camera();
	cam->translateZinc(3.5);

	ug = new Underground();

	tester = new Ant();
//	tester->setXY(0, 2);
	tester->setPatch( ug->getGrid()->getPatch(0,2) );
	p = new Player(tester);

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
			// Draw game field.
			ug->draw();
			//ug->drawAnts(black, red);

			// draw the ants			
			for (unsigned int i=0; i < black.size(); i++)
				ug->drawAnt(black[i]);

//			for (unsigned int i=0; i < red.size(); i++)
//				ug->drawAnt(red[i]);

			ug->drawAnt(p->getPlayerAnt());
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
	{
		if (ug->pickPoint(x, y, *cam))
		{
			picked = ug->getPicked();
			p->setDestination(picked->x, picked->y);
			if (picked->TYPE == PATCH_EMPTY)
				automove = true;
			else
				automove = false;
		}
	}
	else if (STATE == SURFACE_S)
	{
		//surf->pickPoint();
	}
}

int GameWorld::getInput()
{
	return in->getPressed();
}

void GameWorld::stepAntsForward()
{
	// test, move each ant around randomly.
	for (unsigned int i=0; i < black.size(); i++)
	{
		// AI for ant "black[i]->AImove()"
		black[i]->move();
	}
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
	if( held & KEY_LEFT)
	{
		p->moveLeft();
		automove = false;
	}
	if( held & KEY_RIGHT)
	{
		p->moveRight();
		automove = false;
	}
	if( held & KEY_UP)
	{
		p->moveUp();
		automove = false;
	}
	if( held & KEY_DOWN)
	{
		p->moveDown();
		automove = false;
	}

	// Pressing the D-Pad cancels automove.
	// automove is set by touching an empty spot.
	if (automove)
	{
		p->move();
	}

	if(pressed & KEY_A)
	{
		// add a new ant on press.
		Ant *t = new Ant(ug->getGrid()->getPatch(0,2));
		t->setAction( ANT_ACTION_WANDER );
		black.push_back(t);
	}

	// TODO: this will be the player ant:
	ug->shiftCenter(p->getPlayerAnt());

	// no draw here, it is handled elsewhere so that things will be able
	// to move forward if things start to lag.

	touchPosition touchXY;
	// snag touch info from hardware.
	touchRead(&touchXY);

	curX = touchXY.px;
	curY = touchXY.py;

/*
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
*/
	// If it is pressed, see if we can DIG IT.
	if( pressed & KEY_TOUCH)
		p->dig();


	// send everyone on their way.
	stepAntsForward();
}


void GameWorld::setProjection()
{
	//change ortho vs perspective
	if(held & KEY_B)
		cam->Ortho();
	else 
		cam->Perspective();
}
