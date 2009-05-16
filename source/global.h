#ifndef GLOBAL_H
#define GLOBAL_H

// GameWorld states
#define GAMEWORLD_STATE_UNDERGROUND 0
#define GAMEWORLD_STATE_SURFACE 1
#define GAMEWORLD_STATE_UNDERGROUND_ENEMY 2

// Map tile size
#define WIDTH 60
#define DEPTH 45

// Ant ACTION's
#define ANT_ACTION_WANDER 0
#define ANT_ACTION_FORAGE 1
#define ANT_ACTION_FOLLOW 2
#define ANT_ACTION_ATTACK 3
#define ANT_ACTION_IDLE 4
#define ANT_ACTION_GOHOME 5

// Patch TYPE values
#define PATCH 0
#define PATCH_DIRT 1
#define PATCH_EMPTY 2
#define PATCH_BARRIER 3
#define PATCH_FOOD 4
#define PATCH_EGG 5
#define PATCH_TOP 6
// entrance to underground (hill)
#define PATCH_ENTRANCE 7

// Draw distance.
#define UNDERGROUND_GRID_DRAW 7
#define GRID_SIZE 13

// Draw scale and animation between grid stuff.
// ANIMATION_SIZE must be divisible by 2.
#define ANIMATION_SIZE 20
#define MODEL_SCALE 0.2
#define MODEL_SCALE_INCREMENT 0.01
// there is a built in vector: "GLvector" but it uses ints...
// I'm not that good yet.
typedef struct {
	float x, y, z, d;
} VectorF;



// Observer states:
#define PLAYER_MOVE_UP		0
#define PLAYER_MOVE_DOWN	1
#define PLAYER_MOVE_RIGHT	2
#define PLAYER_MOVE_LEFT	3

#define PLAYER_HELD_LEFT	4
#define PLAYER_HELD_RIGHT	5
#define PLAYER_HELD_UP		6
#define PLAYER_HELD_DOWN	7

#define PLAYER_PRESSED_LEFT		4
#define PLAYER_PRESSED_RIGHT	5
#define PLAYER_PRESSED_UP			6
#define PLAYER_PRESSED_DOWN		7
#endif
