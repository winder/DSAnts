#include "GameWorld.h"

GameWorld::GameWorld()
{
	followingPlayer = true;

	in = new Input();

	cam = new Camera();
	//cam->translateZinc(2.2);

	ug = new Underground();
	surf = new Surface();
	surf->getGrid()->setLoopY();

	// start underground
	STATE = GAMEWORLD_STATE_UNDERGROUND;
	curMap = ug;

	// init picking variables
	picked = '\0';
	doPick = false;
	
	// Create player.
	Ant *tmp = new Ant();
	tmp->setPatch( ug->getGrid()->getPatch(0,2) );
	tmp->setLocation( STATE );
	tmp->setCarrying(PATCH_FOOD1);	
	p = new Player(tmp);

	// SETUP OBSERVERs:

	// Observing Player: GameWorld.
	p->attach(this);

	// Observing GameWorld: Player.
	attach(p);

	// Observing Input: Player, GameWorld.
	in->attach(p);
	in->attach(this);
	in->attach(cam);

	// This loops through The surface of the underground (both when I add the enemy underground)
	// and links them to the surface.
	linkSurfaceAntUnderground();
}

GameWorld::~GameWorld()
{
	delete ug;
	delete surf;
	delete p;
	delete in;
	delete cam;

	// loop through "black" and "red" and delete 'em.
	for (unsigned int i=0; i < black.size(); i++)
		delete black[i];
	for (unsigned int i=0; i < red.size(); i++)
		delete red[i];
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
		}while (!EMPTY(topleft->portal));

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
	numAnts = 0;
	// if player is offscreen, center screen.
	if (! curMap->isVisible( p->getPlayerAnt()->getX(), p->getPlayerAnt()->getY() ) )
		curMap->setCenter( p->getPlayerAnt()->getX(), p->getPlayerAnt()->getY() );

	curMap->begin();
	// Draw game field.
	curMap->draw();

	// draw the ants			
	for (unsigned int i=0; (numAnts < 80) && (i < black.size()); i++)
		if ( black[i]->getLocation() == STATE )
			if (curMap->drawAnt(black[i], true))
				numAnts++;

//		for (unsigned int i=0; i < red.size(); i++)
//			if ( red[i]->getLocation() == GAMEWORLD_STATE_UNDERGROUND )
//				ug->drawAnt(red[i]);

	curMap->drawAnt(p->getPlayerAnt(), true);

	curMap->end();
	// DO THE PICKING
	// If the touch pad is being touched... see what its touching.
	if (doPick)
	{
		pickPoint(in->getTouchX(), in->getTouchY());
		doPick = false;
	}
}

void GameWorld::pickPoint(short x, short y)
{
	if (curMap->pickPoint(x, y, *cam))
	{
		picked = curMap->getPicked();
		// Disabling automove for now.
		//p->setDestination(picked->x, picked->y);
		//if (WALKABLE(picked))
		//	automove = true;
		//else
		//	automove = false;

		// update intself
		update(PLAYER_PICKED_SOMETHING);
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
		black[i]->stateStep();
		black[i]->move();
	}
}

void GameWorld::stepForward()
{
	// The "Player" doesn't keep track of its location, so I can't do this through observer.
	if (STATE != p->getPlayerAnt()->getLocation())
	{
		STATE = p->getPlayerAnt()->getLocation();
		if (STATE == GAMEWORLD_STATE_UNDERGROUND)
			curMap = ug;
		else if (STATE == GAMEWORLD_STATE_SURFACE)
			curMap = surf;
		//else if (STATE == GAMEWORLD_STATE_ENEMY_UNDERGROUND)
		//	curMap = eug;
	}

	// The map needs to follow the player.
	// This needs to be done every frame rather than on player move event,
	// otherwise panning is jerky.
	if (followingPlayer)
		curMap->shiftCenter(p->getPlayerAnt());


	// no draw here, it is handled elsewhere so that things will be able
	// to move forward if things start to lag.

	// process user input.
	// TODO: if this is moved into the VBlank, will the input registers be valid?
	// note: vBlank didn't work.
	in->process();

	// send everyone on their way.
	stepAntsForward();
	p->stepForward();

/*
	if( held & KEY_LEFT)
	{
		//p->moveLeft();
		set_val(PLAYER_HELD_LEFT);
		automove = false;
	}
	if( held & KEY_RIGHT)
	{
		//p->moveRight();
		set_val(PLAYER_HELD_RIGHT);
		automove = false;
	}
	if( held & KEY_UP)
	{
		//p->moveUp();
		set_val(PLAYER_HELD_UP);
		automove = false;
	}
	if( held & KEY_DOWN)
	{
		//p->moveDown();
		set_val(PLAYER_HELD_DOWN);
		automove = false;
	}

	// Pressing the D-Pad cancels automove.
	// automove is set by touching an empty spot.
	if (automove)
	{
		p->move();
	}
*/

}


void GameWorld::setProjection()
{
	//change ortho vs perspective
//	if(held & KEY_B)
//		cam->Ortho();
//	else 
		cam->Perspective();
}

void GameWorld::update(int value)
{
	// TODO: do follow cursor on hold?

	// do picking on PRESS
	if (value == PLAYER_PRESSED_TOUCHPAD)
		doPick = true;
	else if (value == PLAYER_PICKED_SOMETHING)
	{
		// If it is dirt, see if we can DIG IT.
		if (picked->TYPE == PATCH_DIRT)
			curMap->getGrid()->clear(p->dig());
		// If your carrying something, and the spot is empty, drop.
		else if (EMPTY(picked) && p->getPlayerAnt()->getCarrying())
		{
			// "drop" clears out what its carring, so need to store it for a moment.
			int saveO = p->getPlayerAnt()->getCarrying();
			curMap->getGrid()->addObject( p->drop(), saveO );
		}
		// if it is an object, see if we can PICK IT.
		else if (OBJECT(picked))
		{
			int pickupAction = p->pickUp();
			curMap->getGrid()->takeObject( picked );
		}
	}
	else if (value == PLAYER_RELEASED_TOUCHPAD)
	{
		if (picked)
			picked->picked = false;
	}


	// Hold A to spawn ants,
	else if(value == PLAYER_HELD_B)
	{
		// add a new ant on press.
		Ant *t = new Ant(ug->getGrid()->getPatch(0,2), GAMEWORLD_STATE_UNDERGROUND);
		t->setAction( ANT_ACTION_WANDER );
		black.push_back(t);
	}

	else if (value == PLAYER_PRESSED_X);
	{
		#ifdef __PROFILING
			cygprofile_end();
		#endif
	}
}

//#ifdef __DEBUG
void GameWorld::printDebugFiveLines()
{
	// Interesting stats:
	//    Number of ants
	//    Map / Map coordinate
	//		Spot touched
	//		Player info
	//		player automove?
//	printf("\nWORLD, Ants: black(%i), red(%i)", black.size(), red.size());
	if (STATE == GAMEWORLD_STATE_UNDERGROUND)
		printf("\nCurrent map: underground");
	else if (STATE == GAMEWORLD_STATE_SURFACE)
		printf("\nCurrent map: surface");
	else
		printf("\nCurrent map: unknown");
//	printf("\nMap Stats: <UG> <Surf>");
//	printf("\n  Cleared: %5d %5d", ug->getGrid()->numCleared(), surf->getGrid()->numCleared());
//	printf("\n  Objects: %5d %5d", ug->getGrid()->numObjects(), surf->getGrid()->numObjects());
	printf("\nMap Center: (%i, %i)", curMap->getCenterX(), curMap->getCenterY());
	printf("\nAnts: drawn(%i)\n black(%i), red(%i)", numAnts, black.size(), red.size());
//	printf("\nCamera distance: %f", cam->getCamLocation().z);
//	printf("\nTouch coord: (%i, %i)", curX, curY);
	p->printDebug();


}
//#endif
