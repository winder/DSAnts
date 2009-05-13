#ifndef ANT_H
#define ANT_H

#include "Patch.h"

class Ant
{
	public:
		// the ant doesn't draw itself.  the world draws the ants.
//		virtual void draw();

		void moveRight();
		void moveLeft();
		void moveUp();
		void moveDown();

	private:
		// The patch the ant is in.
		Patch *p;

		// The offset within the patch
		int offsetX;
		int offsetY;
		
};

#endif
