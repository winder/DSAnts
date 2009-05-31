#ifndef GLOBAL_H
#define GLOBAL_H

// TODO: I should be using "enum { FIRST, SECOND, THIRD, ... };"
//       or "enum { WIDTH=45, DEPTH=50 };"
// GameWorld states
enum Maps
{
  GAMEWORLD_STATE_SURFACE,
  GAMEWORLD_STATE_UNDERGROUND,
  GAMEWORLD_STATE_UNDERGROUND_ENEMY
};

// Map tile size
#define WIDTH 60
#define DEPTH 45

// Ant ACTION's
enum Ant_AI
{
  ANT_ACTION_WANDER,
  ANT_ACTION_FORAGE,
  ANT_ACTION_FOLLOW,
  ANT_ACTION_ATTACK,
  ANT_ACTION_IDLE,
  ANT_ACTION_GOHOME
};


// Patch TYPE values
enum PATCH_types
{
  NOTHING,
  PATCH,
  PATCH_DIRT,
  PATCH_EMPTY,
  PATCH_BARRIER,
  PATCH_FOOD1,
  PATCH_FOOD2,
  PATCH_FOOD3,
  PATCH_FOOD4,
  PATCH_FOOD5,
  PATCH_FOOD6,
  PATCH_FOOD7,
  PATCH_FOOD8,
  PATCH_FOOD9,
  PATCH_FOOD10, // food stacks.
  PATCH_EGG1,
  PATCH_EGG2,
  PATCH_EGG3,
  PATCH_EGG4,
  PATCH_EGG5,
  PATCH_TOP,
  // entrance to underground (hill)
  PATCH_ENTRANCE
};

// Draw distance.
#define UNDERGROUND_GRID_DRAW 7
#define GRID_SIZE 10

// Number of ticks the ant will try to walk in a direction that is not clear
#define WALK_RETRY_NUM 100

// Draw scale and animation between grid stuff.
// ANIMATION_SIZE must be divisible by 2, it determines how many frames it takes
// for an ant to move 1 tile.
#define ANIMATION_SIZE 20
#define MODEL_SCALE 0.2
#define MODEL_SCALE_INCREMENT MODEL_SCALE / ANIMATION_SIZE
// there is a built in vector: "GLvector" but it uses ints...
// I'm not that good yet.
typedef struct {
  float x, y, z, d;
} VectorF;

// These are the physical inputs sent out from the Input class.
enum Observer_states
{
  // Player sourced states
  PLAYER_MOVE_UP,
  PLAYER_MOVE_DOWN,
  PLAYER_MOVE_RIGHT,
  PLAYER_MOVE_LEFT,
  // note: this does not actually originate from the player,
  //       and is not an observer message.  But conceptually it
  //       is from the player, and the GameWorld notifies itself
  //       through its update message.
  PLAYER_PICKED_SOMETHING,

  // Input sourced states.
  // D-PAD.
  PLAYER_HELD_LEFT,
  PLAYER_HELD_RIGHT,
  PLAYER_HELD_UP,
  PLAYER_HELD_DOWN,
  PLAYER_PRESSED_LEFT,
  PLAYER_PRESSED_RIGHT,
  PLAYER_PRESSED_UP,
  PLAYER_PRESSED_DOWN,
  // TOUCHPAD
  PLAYER_PRESSED_TOUCHPAD,
  PLAYER_HELD_TOUCHPAD,
    // this one isn't from hardware, but I detect it manually from the Input class.
  PLAYER_RELEASED_TOUCHPAD,
  // BUTTONS
  PLAYER_HELD_A,
  PLAYER_HELD_B,
  PLAYER_HELD_X,
  PLAYER_HELD_Y,
  PLAYER_PRESSED_A,
  PLAYER_PRESSED_B,
  PLAYER_PRESSED_X,
  PLAYER_PRESSED_Y,
  // SHOULDERS
  PLAYER_HELD_SHOULDER_R,
  PLAYER_HELD_SHOULDER_L,
  PLAYER_PRESSED_SHOULDER_R,
  PLAYER_PRESSED_SHOULDER_L
  // SELECT, START
};

// Different creatures.
enum Creature_TYPE
{
  ANT_WORKER,
  ANT_SOLDIER,
  ANT_BREEDER,
  ANT_QUEEN,
  ENEMY_SPIDER,
  ENEMY_PILL_BUG,
  ENEMY_WORM
};

#endif
