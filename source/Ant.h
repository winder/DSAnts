#ifndef ANT_H
#define ANT_H

#include "Patch.h"
#include "Grid.h"
#include <stdio.h>

class Ant
{
	public:
		Ant();
		Ant(Patch* pat);

		// the ant doesn't draw itself.  the world draws the ants.
//		virtual void draw();

//		void setXY(short inx, short iny)
//			{ x = inx; y = iny; }

		short getX(){ return p->x; }
		short getY(){ return p->y; }

		void setPatch(Patch* inp){ p = inp; }
		Patch* getPatch(){ return p; }

		// move the ant.
		// TODO: This could use more tweaking efficiency wise.
		// return value: true if tile changed.
		bool moveRight();
		bool moveLeft();
		bool moveUp();
		bool moveDown();

		// This lets the ant move on its own accord, influenced by:
		//		-feramone level of adjacent tiles
		//		-current action
		void move();
		void wander();
		void attack();
		void forage();

		void setAction(int a){ ACTION = a; }
		int getAction(){ return ACTION; }

		int getOffsetX(){ return offsetX; }
		int getOffsetY(){ return offsetY; }

		void setAI(bool inai){ ai = inai; }
		bool getAI(){ return ai; }
	private:
		   //-----------------//
		  // AI INFORMATION: //
		 //-----------------//
		// True or false if the AI needs to intervene.
		bool ai;
		// ai will set these after deciding where the ant is heading.
		// TODO: Dijkstra?
		short dest_x, dest_y;
		// Action the ai has decided the ant will perform, ie:
		//	FORAGE, FIGHT, DEFEND, FOLLOW, ...?
		int ACTION;
		// save the direction between steps to prevent re-calculation.
		int direction;
		int directionOld; // ant wants to avoid turning around.

		   //-----------------------//
		  // LOCATION INFORMATION: //
		 //-----------------------//
		// The x/y coordinate the ant is in, for convenient draw checks.
		// these are now stored in the Patch.
//		short x;
//		short y;

		// The patch the ant is in, for convenient adjacentcy checks.
		Patch *p;
		// The offset within the patch
		short offsetX;
		short offsetY;
		
		// Status, health information.
		int status;

		// is the ant carrying something?
		int carrying;
};

#endif
