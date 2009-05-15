#ifndef PLAYER_H
#define PLAYER_H

#include "Ant.h"
#include <math.h>

// This class handles player interactions with the game world
class Player
{
	public:
		Player();
		Player(Ant *ip);

		// this will probably only be called from GameWorld, the player ant will need
		// to be removed from whatever list it was taken out of to prevent duplicate drawings.
		// (Or maybe a flag in the ant for if its a player, like setting the AI bool to false)
		void setPlayerAnt(Ant *inp){ p = inp; }
		Ant* getPlayerAnt(){ return p; }

		// Recursive function will be needed here, shouldn't be too bad for memory
		// since a visible screen is only ~20x20
		void setDestination(int x, int y);

		// manual movement:
		void moveUp(){ p->moveUp(); }
		void moveDown(){ p->moveDown(); }
		void moveLeft(){ p->moveLeft(); }
		void moveRight(){ p->moveRight(); }

		// If player has moved adjacent to a picked DIRT box, dig it.
		Patch* dig();

		// auto movement:
		void move();

	private:
		// The player is an ant.  This ant is the player.
		Ant *p;

		// destination to move in.
		int destX, destY;
		bool stuck;
		short direction;
		short directionX, directionY;
};

#endif
