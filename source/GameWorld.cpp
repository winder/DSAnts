#include "GameWorld.h"

GameWorld::GameWorld()
{
	// start underground
	STATE = GAMEWORLD_STATE_UNDERGROUND;

	cam = new Camera();
	cam->translateZinc(3.5);

	ug = new Underground();
	surf = new Surface();
	surf->getGrid()->loopY();

	
	tester = new Ant();
	tester->setPatch( ug->getGrid()->getPatch(0,2) );
	p = new Player(tester);


	// This loops through The surface of the underground (both when I add the enemy underground)
	// and links them to the surface.
	linkSurfaceAntUnderground();
}

GameWorld::~GameWorld()
{
	delete ug;
	delete surf;

	// loop through "black" and "red" and delete 'em.
}

void GameWorld::linkSurfaceAntUnderground()
{
	int randX, randY;
	// loop across the top layer of the underground, link to random areas of the surface.
	// ug is the black ants, in the left side of the map.
	Patch* topleft = ug->getGrid()->getPatch(0,0);
	do
	{
		// find random locations until we get one that hasn't already been picked.
		do
		{
			randX = rand() % (WIDTH / 2);
			randY = rand() % DEPTH;
			topleft->portal = surf->getGrid()->getPatch(randX, randY);
		}while (topleft->portal->TYPE == PATCH_ENTRANCE);

		topleft->portal->TYPE = PATCH_ENTRANCE;

		// make it two-way.
		// NOTE: the surface points to just below the top level of the underground
		topleft->portal->portal = topleft->bottom;
		// move right
		topleft = Grid::getRight( topleft );
		// until we loop all the way around.
	} while(topleft->x != 0);
	
}

void GameWorld::draw()
{
	// Draw the SCENE
	if (STATE == GAMEWORLD_STATE_UNDERGROUND)
	{
		// if player is offscreen, center screen.
//		if (! ug->isVisible( p->getPlayerAnt()->getX(), p->getPlayerAnt()->getY() ) )
//			ug->setCenter( p->getPlayerAnt()->getX(), p->getPlayerAnt()->getY() );

		// Draw game field.
		ug->draw();

		// draw the ants			
		for (unsigned int i=0; i < black.size(); i++)
			if ( black[i]->getLocation() == GAMEWORLD_STATE_UNDERGROUND )
				ug->drawAnt(black[i]);

//			for (unsigned int i=0; i < red.size(); i++)
//				if ( red[i]->getLocation() == GAMEWORLD_STATE_UNDERGROUND )
//					ug->drawAnt(red[i]);

		ug->drawAnt(p->getPlayerAnt());
	}
	else if (STATE == GAMEWORLD_STATE_SURFACE)
	{
//		if (! surf->isVisible( p->getPlayerAnt()->getX(), p->getPlayerAnt()->getY() ) )
//			surf->setCenter( p->getPlayerAnt()->getX(), p->getPlayerAnt()->getY() );

		// Draw the game field.
		surf->draw();

		// draw the ants			
		for (unsigned int i=0; i < black.size(); i++)
			if ( black[i]->getLocation() == GAMEWORLD_STATE_SURFACE)
				surf->drawAnt(black[i]);

		surf->drawAnt(p->getPlayerAnt());
	}

	// DO THE PICKING
	// If the touch pad is being touched... see what its touching.
	if( held & KEY_TOUCH)
		pickPoint(curX, curY);
}

void GameWorld::pickPoint(short x, short y)
{
	if (STATE == GAMEWORLD_STATE_UNDERGROUND)
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
	else if (STATE == GAMEWORLD_STATE_SURFACE)
	{

		if (surf->pickPoint(x, y, *cam))
		{
			picked = ug->getPicked();
			p->setDestination(picked->x, picked->y);
			if (picked->TYPE == PATCH_EMPTY)
				automove = true;
			else
				automove = false;
		}
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
/*
	// Manual swap from Surface to Underground?
	if(pressed & KEY_X)
	{
		if (STATE == GAMEWORLD_STATE_SURFACE)
			STATE = GAMEWORLD_STATE_UNDERGROUND;
		else if (STATE == GAMEWORLD_STATE_UNDERGROUND)
			STATE = GAMEWORLD_STATE_SURFACE;
	}
*/
	STATE = p->getPlayerAnt()->getLocation();

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

	// Hold A to spam ants,
	// Press B to spawn 1 at a time.
	if((held & KEY_A) || (pressed & KEY_B))
	{
		// add a new ant on press.
		Ant *t = new Ant(ug->getGrid()->getPatch(0,2), GAMEWORLD_STATE_UNDERGROUND);
		t->setAction( ANT_ACTION_WANDER );
		black.push_back(t);
	}

	// follow the player.
	if (STATE == GAMEWORLD_STATE_UNDERGROUND)
		ug->shiftCenter(p->getPlayerAnt());
	else if (STATE == GAMEWORLD_STATE_SURFACE)
		surf->shiftCenter(p->getPlayerAnt());

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
		ug->getGrid()->clear(p->dig());


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

//#ifdef __DEBUG_
void GameWorld::printDebugFiveLines()
{
	// Interesting stats:
	//    Number of ants
	//    Map / Map coordinate
	//		Spot touched
	//		Player info
	//		player automove?
	printf("\nGAME WORLD");
	printf("\nNum Ants, black(%i), red(%i)", black.size(), red.size());
	if (STATE == GAMEWORLD_STATE_UNDERGROUND)
		printf("\nCurrent map: underground");
	else if (STATE == GAMEWORLD_STATE_SURFACE)
		printf("\nCurrent map: surface");
	printf("\nMap Center: (%i, %i)", ug->getCenterX(), ug->getCenterY());
//	printf("\nTouch coord: (%i, %i)", curX, curY);
	p->printDebug();


}
//#endif
