#include "Input.h"

int Input::getHeld()
{
	return held;
}

int Input::getPressed()
{
	return pressed;
}

void Input::processTouch()
{
	// snag touch info from hardware.
	touchRead(&touchXY);

	//reset x and y when user touches screen
	if( pressed & KEY_TOUCH)  {
		oldx = touchXY.px;
		oldy = touchXY.py;
	}

	//if user drags then grab the delta
	if( held & KEY_TOUCH) {
		deltax += touchXY.px - oldx; 
		deltay += touchXY.py - oldy;
		oldx = touchXY.px;
		oldy = touchXY.py;
	}
}

void Input::processInput()
{
		//process input
		scanKeys();
		int held = keysHeld();
		int pressed = keysDown();
}
