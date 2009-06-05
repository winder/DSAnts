#ifndef PATCH_H
#define PATCH_H

#include "global.h"
// Forward declaration of creature class, because the Patch has a pointer to them.
class Creature;


// Simple generic functions:
// Note: I don't really know much about designing macros, it made sense to cascade
// them like this, but it could get confusing.
// TODO: re-evaluate how I'm creating patch comparisons.
//        -> make these inline functions

// Can walk
// TODO: Seperate AVAILABLE_SPOT from this.
#define WALKABLE(X) (WALKABLEi(X->TYPE))
#define WALKABLEi(X) ((X == PATCH_EMPTY) || (X == PATCH_ENTRANCE) || EGGi(X) || FOODi(X))

// Is food
#define FOOD(X) FOODi(X->TYPE)
#define FOODi(X) ((X == PATCH_FOOD1) ||(X == PATCH_FOOD2) ||(X == PATCH_FOOD3) ||(X == PATCH_FOOD4) || (X == PATCH_FOOD5)\
 ||(X == PATCH_FOOD6) ||(X == PATCH_FOOD7) ||(X == PATCH_FOOD8) ||(X == PATCH_FOOD9) ||(X == PATCH_FOOD10))

#define FOODFULL(X) (FOODFULLi(X->TYPE))
#define FOODFULLi(X) (X == PATCH_FOOD10)

// Is an egg
#define EGG(X) EGGi(X->TYPE)
#define EGGi(X) ((X == PATCH_EGG1) ||(X == PATCH_EGG2) ||(X == PATCH_EGG3) ||(X == PATCH_EGG4) || (X == PATCH_EGG5))

// Is an object
#define OBJECT(X) OBJECTi(X->TYPE)
#define OBJECTi(X) (FOODi(X) || EGGi(X))

// Empty
#define EMPTY(X) (EMPTYi(X->TYPE))
#define EMPTYi(X) ((X == PATCH_EMPTY))

// A Portal
#define PORTAL(X) (X->portal && WALKABLE(cache->portal))

// Collision detection checks
#define NOANTS(X) ((X->occupant_one == '\0') && (X->occupant_two == '\0'))

// swap these to disable collision detection
#define AVAILABLE_SPOT(X) (true)
//#define AVAILABLE_SPOT(X) ((X->occupant_one == '\0') || (X->occupant_two == '\0'))
#define SET_SPOT(X, Y) {if(AVAILABLE_SPOT_ONE(X)) SET_SPOT_ONE(X, Y); else if (AVAILABLE_SPOT_TWO(X)) SET_SPOT_TWO(X, Y);}
#define REMOVE_SPOT(X, Y) {if(SPOT_ONE_IS(X, Y)) SET_SPOT_ONE(X, '\0'); else if (SPOT_TWO_IS(X, Y)) SET_SPOT_TWO(X, '\0');}

#define AVAILABLE_SPOT_ONE(X) (X->occupant_one == '\0')
#define AVAILABLE_SPOT_TWO(X) (X->occupant_two == '\0')

#define SET_SPOT_ONE(X, Y) (X->occupant_one = Y)
#define SET_SPOT_TWO(X, Y) (X->occupant_two = Y)

#define SPOT_ONE_IS(X, Y) (X->occupant_one == Y)
#define SPOT_TWO_IS(X, Y) (X->occupant_two == Y)

#define INCREASE_FERAMONE_ONE(X) (INCREASE_FERAMONE(X, 1))
#define INCREASE_FERAMONE(X, Y) (X->chemLevel+=Y)
#define SET_FERAMONE(X, Y) (X->chemLevel=Y)

struct Patch
{
  Patch *left;
  Patch *right;
  Patch *top;
  Patch *bottom;
  // I'm thinking of making this a long and having lots of flags in addition to
  // just a TYPE, then having a bunch of macro's to convert TYPE into usable parts.
  unsigned short TYPE;
  bool picked;
  // this flag should be moved into a bit of TYPE eventually.
  unsigned short location;
  // yeah, I want these in here, sorry memory.
  unsigned short x;
  unsigned short y;

  // Some patches have a 5th direction, where they portal to another map.
  Patch *portal;

  // Ants can walk over each other, but I don't want big piles.
  Creature* occupant_one;
  Creature* occupant_two;

  // the feramone level the ants use to move around.
  unsigned short chemLevel;
};

#endif
