#include "GameWorld.h"

GameWorld::GameWorld()
{
	tester = new Ant();
	tester->setXY(0, 2);
	ug = new UndergroundDrawGrid();
	STATE = UNDERGROUND_S;
}

GameWorld::~GameWorld()
{
	delete ug;
}

void GameWorld::draw()
{
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
}

void GameWorld::pickPoint(short x, short y, Camera cam)
{
	if (STATE == UNDERGROUND_S)
		ug->pickPoint(x, y, cam);
	else if (STATE == SURFACE_S)
	{
		//surf->pickPoint();
	}
}
