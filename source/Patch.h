#ifndef PATCH_H
#define PATCH_H

#include "global.h"
// Forward declaration of creature class, because the Patch has a pointer to them.
class Creature;
struct Patch
{
  // constructor.
  Patch();
  Patch(int, int);

  // Linked in a grid.
  Patch *left;
  Patch *right;
  Patch *top;
  Patch *bottom;

  // Optional 5th dimension (portal to another map).
  Patch *portal;

  // I'm thinking of making this a long and having lots of flags in addition to
  // just a TYPE, then having a bunch of macro's to convert TYPE into usable parts.
  unsigned short TYPE;

  // Having this flag doesn't make any logical sense, as there will only be
  // one picked at any given time.
  bool picked;

  // location flag could be moved into a bit of TYPE eventually.
  unsigned short location;
  // yeah, I want these in here, sorry memory.
  unsigned short x;
  unsigned short y;


  // Ants can walk over each other, but I don't want big piles.
  Creature* occupant_one;
  Creature* occupant_two;
  Creature* occupant_three;
  Creature* occupant_four;

  // In the near future I will want 2 chemicals for each team,
  // One that is HIGH near food, one that is HIGH near an entrance.
  unsigned int chemLevel[4];
  //unsigned int *black = &chemLevel[0]
  //unsigned int *red   = &chemLevel[2]
};

// Simple generic functions:
// Note: I don't really know much about designing macros, it made sense to cascade
// them like this, but it could get confusing.
// TODO: re-evaluate how I'm creating patch comparisons.
//        -> make these inline functions

static inline bool FOODi(unsigned short X)
{
  return (  (X == PATCH_FOOD1) ||
            (X == PATCH_FOOD2) ||
            (X == PATCH_FOOD3) ||
            (X == PATCH_FOOD4) ||
            (X == PATCH_FOOD5) ||
            (X == PATCH_FOOD6) ||
            (X == PATCH_FOOD7) ||
            (X == PATCH_FOOD8) ||
            (X == PATCH_FOOD9) ||
            (X == PATCH_FOOD10) );
}

static inline bool FOOD(Patch* X)
{
  return FOODi(X->TYPE);
}

static inline bool FOODFULL(Patch* X)
{
  return (X->TYPE == PATCH_FOOD10);
}

static inline bool EGGi(short unsigned X)
{
  return (  (X == PATCH_EGG1) ||
            (X == PATCH_EGG2) ||
            (X == PATCH_EGG3) ||
            (X == PATCH_EGG4) ||
            (X == PATCH_EGG5)  );
}

static inline bool EGG(Patch* X)
{
  return EGGi(X->TYPE);
}

static inline bool OBJECT(Patch* X)
{
  return (FOOD(X) || EGG(X));
}

static inline bool OBJECTi(short unsigned X)
{
  return (FOODi(X) || EGGi(X));
}


static inline bool EMPTY(Patch* X)
{
  return (X->TYPE == PATCH_EMPTY);
}

// Can walk
static inline bool WALKABLE(Patch* X)
{
  return ( ( X->TYPE == PATCH_EMPTY    ) ||
           ( X->TYPE == PATCH_ENTRANCE ) ||
           ( EGG(X)              ) ||
           ( FOOD(X)             )  );
}

static inline bool PORTAL(Patch* X)
{
  return (  (X->portal) &&
            (WALKABLE(X->portal)) );
}


// Collision detection checks
static inline bool NOANTS(Patch* X)
{
  return( (X->occupant_one   == '\0') &&
          (X->occupant_two   == '\0') &&
          (X->occupant_three == '\0') &&
          (X->occupant_four  == '\0'));
}

// swap these to disable collision detection
#define AVAILABLE_SPOT(X) (true)
/*
static inline bool AVAILABLE_SPOT(Patch* X)
{
  return( (X->occupant_one   == '\0') ||
          (X->occupant_two   == '\0') ||
          (X->occupant_three == '\0') ||
          (X->occupant_four  == '\0'))
}
*/

// TODO: Make this use an array with MACRO defined max instead of ... this.
#define SET_SPOT(X, Y) {if      (AVAILABLE_SPOT_ONE(X)) SET_SPOT_ONE(X, Y);\
                        else if (AVAILABLE_SPOT_TWO(X)) SET_SPOT_TWO(X, Y);\
                        else if (AVAILABLE_SPOT_THREE(X)) SET_SPOT_THREE(X, Y);\
                        else if (AVAILABLE_SPOT_FOUR(X)) SET_SPOT_FOUR(X, Y);}
#define REMOVE_SPOT(X, Y) {if(SPOT_ONE_IS((X), (Y)))         SET_SPOT_ONE  ((X), '\0');\
                           else if (SPOT_TWO_IS((X), (Y)))   SET_SPOT_TWO  ((X), '\0');\
                           else if (SPOT_THREE_IS((X), (Y))) SET_SPOT_THREE((X), '\0');\
                           else if (SPOT_FOUR_IS((X), (Y)))  SET_SPOT_FOUR ((X), '\0');}

#define AVAILABLE_SPOT_ONE(X)   ((X)->occupant_one   == '\0')
#define AVAILABLE_SPOT_TWO(X)   ((X)->occupant_two   == '\0')
#define AVAILABLE_SPOT_THREE(X) ((X)->occupant_three == '\0')
#define AVAILABLE_SPOT_FOUR(X)  ((X)->occupant_four  == '\0')

#define SET_SPOT_ONE(X, Y)   ((X)->occupant_one   = (Y))
#define SET_SPOT_TWO(X, Y)   ((X)->occupant_two   = (Y))
#define SET_SPOT_THREE(X, Y) ((X)->occupant_three = (Y))
#define SET_SPOT_FOUR(X, Y)  ((X)->occupant_four  = (Y))

#define SPOT_ONE_IS(X, Y)   ((X)->occupant_one   == (Y))
#define SPOT_TWO_IS(X, Y)   ((X)->occupant_two   == (Y))
#define SPOT_THREE_IS(X, Y) ((X)->occupant_three == (Y))
#define SPOT_FOUR_IS(X, Y)  ((X)->occupant_four  == (Y))


// Fermamone stuff....
//#define INCREASE_FERAMONE(X, Y) ((X)->chemLevel[0]+=(Y))
//#define INCREASE_FERAMONE_LIMIT(X, Y, MAX) (((X)->chemLevel[0]<MAX)?(X)->chemLevel[0]+=(Y):(X)->chemLevel[0]=MAX)
//#define SET_FERAMONE(X, Y) ((X)->chemLevel[0]=(Y))

static inline void
INCREASE_FERAMONE(Patch* X, unsigned int Amount, unsigned int pheramone)
{
  if (X)
    X->chemLevel[pheramone] += Amount;
}

static inline void
DECAY_FERAMONE(Patch* X, float percentage)
{
  X->chemLevel[BLACK_FOOD] *= percentage;
  X->chemLevel[BLACK_HOME] *= percentage;
  X->chemLevel[RED_FOOD] *= percentage;
  X->chemLevel[RED_HOME] *= percentage;
}

static inline void
SET_FERAMONE(Patch* X, unsigned int Amount, unsigned int pheramone)
{
  X->chemLevel[pheramone] = Amount;
}

static inline unsigned int
GET_PHERAMONE(Patch* X, unsigned int pheramone)
{
  return X->chemLevel[pheramone];
}

#endif
