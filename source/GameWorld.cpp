#include "GameWorld.h"

GameWorld::GameWorld()
{
	cam = new Camera();
	cam->translateZinc(3.5);

	ug = new UndergroundDrawGrid();

	tester = new Ant();
	tester->setXY(0, 2);
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
	int direction;
	// test, move each ant around randomly.
	for (unsigned int i=0; i < black.size(); i++)
	{
		// AI for ant "black[i]->AImove()"
		
		// move in a random direction.
		direction = rand()%4;

		if ((direction == 0) && (black[i]->getPatch()->bottom) && (black[i]->getPatch()->bottom->TYPE == PATCH_EMPTY))
			black[i]->moveDown();
		else if ((direction <= 1) && (black[i]->getPatch()->right) && (black[i]->getPatch()->right->TYPE == PATCH_EMPTY))
			black[i]->moveRight();
		else if ((direction <= 2) && (black[i]->getPatch()->left) && (black[i]->getPatch()->left->TYPE == PATCH_EMPTY))
			black[i]->moveLeft();
		else if ((direction <= 3) && (black[i]->getPatch()->top) && (black[i]->getPatch()->top->TYPE == PATCH_EMPTY))
			black[i]->moveUp();

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
	// add a new ant on press.
	if(pressed & KEY_A) black.push_back(new Ant(0, 2, ug->getGrid()->getPatch(0,2)));

	// TODO: this will be the player ant:
	ug->shiftCenter(p->getPlayerAnt());

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
