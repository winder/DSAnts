#ifndef GLOBAL_H
#define GLOBAL_H

#define SLICES 15
#define WIDTH 100
#define DEPTH 45

#define PATCH 0
#define PATCH_DIRT 1
#define PATCH_EMPTY 2
#define PATCH_BARRIER 3
#define PATCH_FOOD 4
#define PATCH_EGG 5

// Draw distance.
#define UNDERGROUND_GRID_DRAW 8

// there is a built in vector: "GLvector" but it uses ints...
// I'm not that good yet.
typedef struct {
	float x, y, z;
} VectorF;

#endif
