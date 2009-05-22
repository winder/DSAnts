#ifndef CREATURE_H
#define CREATURE_H

#include "Patch.h"
#include "Grid.h"
#include <stdio.h>

class Creature
{
	public:
		Creature();
		Creature(Patch* pat, int location);

		// template class.
		virtual int getType() = 0;

		// move the ant.
		// return value: true if tile changed.
		bool moveRight();
		bool moveLeft();
		bool moveUp();
		bool moveDown();
		// If moving into a portal from any direction, need to handle it the same way.
		// returns true if it needed to be handled and the move needed to be stopped.
		virtual bool handlePortal();

		// This lets the ant move on its own accord, influenced by:
		//		-feramone level of adjacent tiles
		//		-current action
		virtual void stateStep(int num);

		// move is broken into several parts.
		virtual void move(int num);
		virtual void move();
		virtual void moveAI();
		virtual void wander();
		virtual void attack();
		virtual void forage();

		// use the carrying object (i.e. eat food / egg)
		virtual int use();

		// True if will pickup.  False otherwise.
		bool pickup(Patch *p) {
														if ((carrying == NOTHING) && OBJECT(p))
														{
															carrying = p->TYPE;
															return true;
															// The Grid must modify the type
															//p->TYPE = PATCH_EMPTY;
														}
														return false;
													}

		// True if it can drop, false otherwise.
		bool drop(Patch *p)	{
													if (EMPTY(p))
													{
														carrying = NOTHING;
														// the grid must modify the type.
														//p->TYPE = carrying;
														return true;
													}
													return false;
												}

		//-----------------------//
		// Accessors and Getters //
		//-----------------------//
		void setAction(int a){ ACTION = a; }
		int getAction(){ return ACTION; }

		int getOffsetX(){ return offsetX; }
		int getOffsetY(){ return offsetY; }

		void setAI(bool inai){ ai = inai; }
		bool getAI(){ return ai; }

		short getFacingX(){ return directionX; }
		short getFacingY(){ return directionY; }

		int getCarrying(){ return carrying; }
		void setCarrying(int x){ carrying = x; }

		void setHP(short h){ hp=h; }
		short getHP(){ return hp; }

		short getX(){ return p->x; }
		short getY(){ return p->y; }

		void setPatch(Patch* inp){ p = inp; }
		Patch* getPatch(){ return p; }

		int getLocation(){ return location; }
		void setLocation(int l){ location = l; }

	private:
		// these are used to change offsetX / offsetY and keep the direction correct.
		void incrementOffsetX();
		void decrementOffsetX();
		void incrementOffsetY();
		void decrementOffsetY();
		void clampDirections();

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
		// UNDERGROUND, SURFACE, ENEMY_UNDERGROUND.
		int location;

		// The patch the ant is in, for convenient adjacentcy checks.
		Patch *p;
		// flag so that we don't go back and forth through a portal infinitly.
		bool portaled;

		// The offset within the patch
		short offsetX;
		short offsetY;

		// These are needed so that the ant is rotated in the right direction.
		short directionX;
		short directionY;
		
		// Status, health information.
		short hp;
		// what type of creature is it (ANT_WORKER, ANT_QUEEN, SPIDER, etc)
		short TYPE;

		// is the ant carrying something?
		int carrying;
};

#endif
