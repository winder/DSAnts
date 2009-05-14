#ifndef GLOBAL_H
#define GLOBAL_H

// Underground tile size
#define WIDTH 60
#define DEPTH 45

// Patch TYPE values
#define PATCH 0
#define PATCH_DIRT 1
#define PATCH_EMPTY 2
#define PATCH_BARRIER 3
#define PATCH_FOOD 4
#define PATCH_EGG 5
#define PATCH_TOP 6

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

#endif
