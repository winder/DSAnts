#include "Input.h"

int Input::getHeld()
{
	return held;
}

int Input::getPressed()
{
	return pressed;
}

void Input::process()
{
		//process input
		scanKeys();
		held = keysHeld();
		pressed = keysDown();

	// snag touch info from hardware.
	touchRead(&touchXY);
/*
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
*/


	// D-PAD:
	if( held & KEY_LEFT)
	{
		set_val(PLAYER_HELD_LEFT);
//		automove = false;
	}
	if( held & KEY_RIGHT)
	{
		set_val(PLAYER_HELD_RIGHT);
//		automove = false;
	}
	if( held & KEY_UP)
	{
		set_val(PLAYER_HELD_UP);
//		automove = false;
	}
	if( held & KEY_DOWN)
	{
		set_val(PLAYER_HELD_DOWN);
//		automove = false;
	}

	// TOUCH-PAD
	if( held & KEY_TOUCH)
	{
		set_val(PLAYER_HELD_TOUCHPAD);
		touchReleaseSent = false;
	}
	if( pressed & KEY_TOUCH)
	{
		set_val(PLAYER_PRESSED_TOUCHPAD);
		touchReleaseSent = false;
	}

	if( !(held & KEY_TOUCH) && !(pressed & KEY_TOUCH) && !touchReleaseSent)
	{
		set_val(PLAYER_RELEASED_TOUCHPAD);
		touchReleaseSent = true;
	}

	// BUTTONS
	if( held & KEY_A )
		set_val(PLAYER_HELD_A);

	if( held & KEY_B )
		set_val(PLAYER_HELD_B);

	// SHOULDER BUTTONS
	if( held & KEY_R)
		set_val(PLAYER_HELD_SHOULDER_R);
	if( held & KEY_L)
		set_val(PLAYER_HELD_SHOULDER_L);
}
