#ifndef PATCH_H
#define PATCH_H

#include "global.h"
//#include "Creature.h"


// Simple generic functions:
// Note: I don't really know much about designing macros, it made sense to cascade
// them like this, but it could get confusing.
// TODO: re-evaluate how I'm creating patch comparisons.

// Can walk
#define WALKABLE(X) WALKABLEi(X->TYPE)
#define WALKABLEi(X) ((X == PATCH_EMPTY) || (X == PATCH_ENTRANCE) || EGGi(X) || FOODi(X))

// Is food
#define FOOD(X) FOODi(X->TYPE)
#define FOODi(X) ((X == PATCH_FOOD1) ||(X == PATCH_FOOD2) ||(X == PATCH_FOOD3) ||(X == PATCH_FOOD4) || (X == PATCH_FOOD5)\
 ||(X == PATCH_FOOD6) ||(X == PATCH_FOOD7) ||(X == PATCH_FOOD8) ||(X == PATCH_FOOD9) ||(X == PATCH_FOOD10))

// Is an egg
#define EGG(X) EGGi(X->TYPE)
#define EGGi(X) ((X == PATCH_EGG1) ||(X == PATCH_EGG2) ||(X == PATCH_EGG3) ||(X == PATCH_EGG4) || (X == PATCH_EGG5))

// Is an object
#define OBJECT(X) OBJECTi(X->TYPE)
#define OBJECTi(X) (FOODi(X) || EGGi(X))

// Empty
#define EMPTY(X) EMPTYi(X->TYPE)
#define EMPTYi(X) ((X == PATCH_EMPTY))

// Collision detection checks
#define NOANTS(X) ((X->occupant_one == '\0') && (X->occupant_two == '\0'))
#define AVAILABLE_SPOT(X) ((X->occupant_one == '\0') || (X->occupant_two == '\0'))

struct Patch
{
	Patch *left;
	Patch *right;
	Patch *top;
	Patch *bottom;
	// I'm thinking of making this a long and having lots of flags in addition to
	// just a TYPE, then having a bunch of macro's to convert TYPE into usable parts.
	short TYPE;
	bool picked;
	// this flag should be moved into a bit of TYPE eventually.
	short location;
	// yeah, I want these in here, sorry memory.
	short x;
	short y;

	// Some patches have a 5th direction, where they portal to another map.
	Patch *portal;

	// Ants can walk over each other, but I don't want big piles.
//	Creature* occupant_one;
//	Creature* occupant_two;
};

#endif
