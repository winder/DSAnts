#include "Underground.h"

Underground::Underground()
{
	tunnelSize = 60;
	setCenter(0, 0);
	initGrid();
}

void Underground::initGrid()
{
	getGrid()->generateNest(tunnelSize);
}
