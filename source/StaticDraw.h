#ifndef STATICDRAW_H
#define STATICDRAW_H

#include <nds.h>
#include <math.h>
#include "MapDraw.h"
#include "TextureManager.h"
#include "global.h"

//#ifdef __DEBUG
#include <stdio.h>
//#endif

typedef struct{
  float x,y,z;
} verts;

class StaticDraw
{
  public:
    //void draw(Patch* p);
    static float d2r(float degrees){
        const double conversion = 3.1416f/180.0f;
        return degrees * conversion; }

    // Moved over from MapDraw:
    // Cubes
    static void drawBox(float x, float y, float z, float width, float height, float depth);
    static void drawBox(float x, float y, float z, float width, float height, float depth, TextureManager *tm);

    // Squares
    static void drawRect(float x, float y, float z, float width, float height);
    static void drawRect(float x, float y, float z, float width, float height, TextureManager *tm);
};

#endif
