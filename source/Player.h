#ifndef PLAYER_H
#define PLAYER_H

#include "Ant.h"
#include "Observer.h"
#include <math.h>
//#ifdef __DEBUG_
#include <stdio.h>
//#endif

// This class handles player interactions with the game world
class Player: public Subject, public Observer
{
	public:
		Player();
		Player(Ant *ip);

		~Player();

		// Handle game state things.
		void stepForward();

		// this will probably only be called from GameWorld, the player ant will need
		// to be removed from whatever list it was taken out of to prevent duplicate drawings.
		// (Or maybe a flag in the ant for if its a player, like setting the AI bool to false)
		void setPlayerAnt(Ant *inp){ p = inp; }
		Ant* getPlayerAnt(){ return p; }

		// Recursive function will be needed here, shouldn't be too bad for memory
		// since a visible screen is only ~20x20
		void setDestination(int x, int y);

		// manual movement:
		// If the player moves to the next tile notify observers.
		void moveUp()
			{
				if (p->moveUp())
					set_val(PLAYER_MOVE_UP);
			}
		void moveDown()
			{
				if (p->moveDown())
					set_val(PLAYER_MOVE_DOWN);
			}
		void moveLeft()
			{
				if (p->moveLeft())
					set_val(PLAYER_MOVE_LEFT);
			}
		void moveRight()
			{
				if (p->moveRight())
					set_val(PLAYER_MOVE_RIGHT);
			}

		// Player interaction with the world: dig dirt, pickup objects, drop objects.
		Patch* dig();
		int pickUp();
		Patch* drop();
		// returns what action the object was used for.
		int use();

		// auto movement:
		void move();

		// the observer method.
		void update(int value);

//#ifdef __DEBUG_
		void printDebug();
//#endif
	private:
		// helper function, check if a patch adjacent to the ant is picked.
		Patch* adjacentPatchPicked();

		// The player is an ant.  This ant is the player.
		Ant *p;

		// destination to move in.
		int destX, destY;
		bool stuck;
		short direction;
		short directionX, directionY;
};

#endif
