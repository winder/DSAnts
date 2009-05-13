#ifndef ANT_H
#define ANT_H

#include "Patch.h"
#include "DirtDisk.h"

class Ant
{
	public:
		// the ant doesn't draw itself.  the world draws the ants.
//		virtual void draw();

		void setXY(short inx, short iny)
			{ x = inx; y = iny; }

		short getX(){ return x; }
		short getY(){ return y; }

		void setPatch(Patch* inp){ p = inp; }
		Patch* getPatch(){ return p; }

		// move the ant.
		void moveRight();
		void moveLeft();
		void moveUp();
		void moveDown();

	private:
		   //-----------------//
		  // AI INFORMATION: //
		 //-----------------//
		// True or false if the AI needs to intervene.
		bool ai;
		// ai will set these after deciding where the ant is heading.
		short dest_x, dest_y;
		// Action the ai has decided the ant will perform, ie:
		//	FORAGE, FIGHT, DEFEND, FOLLOW, ...?
		int ACTION;

		   //-----------------------//
		  // LOCATION INFORMATION: //
		 //-----------------------//
		// The x/y coordinate the ant is in, for convenient draw checks.
		short x;
		short y;
		// The patch the ant is in, for convenient adjacentcy checks.
		Patch *p;
		// The offset within the patch
		short offsetX;
		short offsetY;
		
};

#endif
