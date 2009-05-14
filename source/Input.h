#ifndef INPUT_H
#define INPUT_H

#include <nds.h>
#include "Camera.h"

class Input
{
	public:
		int getHeld();
		int getPressed();
		void processInput();
		void processTouch();

		int getTouchX(){ return touchXY.px; }
		int getTouchY(){ return touchXY.py; }
		int getTouchDeltaX(){ return deltax; }
		int getTouchDeltaY(){ return deltay; }

	private:
		bool newInput;

		touchPosition touchXY;
		int held;
		int pressed;

		// current point of cursor
		int touchx, touchy;
		// spot where first touched the screen
		int oldx, oldy;
		// if dragging cursor, this is the difference between old and new;
		int deltax, deltay;
};


#endif
