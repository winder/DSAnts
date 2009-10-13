#include "StaticDraw.h"

void StaticDraw::drawBox(float x, float y, float z, float width, float height, float depth)
{
  float w2 = width * 0.5;
  float h2 = height * 0.5;

  //z  face
  glNormal3f(0,0,1);
  glVertex3f(x-w2,y-h2,z);
  glVertex3f(x-w2,y+h2,z);
  glVertex3f(x+w2,y+h2,z);
  glVertex3f(x+w2,y-h2,z);

  //z + depth face
  glNormal3f(0,0,-1);
  glVertex3f(x-w2,y-h2,z+depth);
  glVertex3f(x+w2,y-h2,z+depth);
  glVertex3f(x+w2,y+h2,z+depth);
  glVertex3f(x-w2,y+h2,z+depth);

  //x  face
  glNormal3f(1,0,0);
  glVertex3f(x-w2,y-h2,z);
  glVertex3f(x-w2,y-h2,z+depth);
  glVertex3f(x-w2,y+h2,z+depth);
  glVertex3f(x-w2,y+h2,z);

  //x + width face
  glNormal3f(-1,0,0);
  glVertex3f(x+w2,y-h2,z);
  glVertex3f(x+w2,y+h2,z);
  glVertex3f(x+w2,y+h2,z+depth);
  glVertex3f(x+w2,y-h2,z+depth);

  //y  face
  glNormal3f(0,-1,0);
  glVertex3f(x-w2,y-h2,z);
  glVertex3f(x+w2,y-h2,z);
  glVertex3f(x+w2,y-h2,z+depth);
  glVertex3f(x-w2,y-h2,z+depth);

  //y  + height face
  glNormal3f(0,1,0);
  glVertex3f(x-w2,y+h2,z);
  glVertex3f(x-w2,y+h2,z+depth);
  glVertex3f(x+w2,y+h2,z+depth);
  glVertex3f(x+w2,y+h2,z);

}

void StaticDraw::drawBox(float x, float y, float z, float width, float height, float depth, TextureManager* tm)
{
//TODO: um.. why doesn't this work?
  // if this was called with a null texture, draw w/o texture.
//  if (tm == '\0')
//  {
//    drawBox(x,y,z,width,height,depth);
//    return;
//  }

  float w2 = width * 0.5;
  float h2 = height * 0.5;

  //z  face
  glNormal3f(0,0,1);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
  glVertex3f(x-w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
  glVertex3f(x-w2,y+h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
  glVertex3f(x+w2,y+h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0,0));
  glVertex3f(x+w2,y-h2,z);

  //z + depth face
  glNormal3f(0,0,-1);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
  glVertex3f(x-w2,y-h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
  glVertex3f(x+w2,y-h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
  glVertex3f(x+w2,y+h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0,0));
  glVertex3f(x-w2,y+h2,z+depth);

  //x  face
  glNormal3f(1,0,0);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
  glVertex3f(x-w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
  glVertex3f(x-w2,y-h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
  glVertex3f(x-w2,y+h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0,0));
  glVertex3f(x-w2,y+h2,z);

  //x + width face
  glNormal3f(-1,0,0);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
  glVertex3f(x+w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
  glVertex3f(x+w2,y+h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
  glVertex3f(x+w2,y+h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0,0));
  glVertex3f(x+w2,y-h2,z+depth);

  //y  face
  glNormal3f(0,-1,0);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
  glVertex3f(x-w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
  glVertex3f(x+w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
  glVertex3f(x+w2,y-h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0,0));
  glVertex3f(x-w2,y-h2,z+depth);

  //y  + height face
  glNormal3f(0,1,0);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
  glVertex3f(x-w2,y+h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
  glVertex3f(x-w2,y+h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
  glVertex3f(x+w2,y+h2,z+depth);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(0,0));
  glVertex3f(x+w2,y+h2,z);

}

void StaticDraw::drawRect(float x, float y, float z, float width, float height, TextureManager *tm)
{
//TODO: um.. why doesn't this work?
  // if this was called with a null texture, draw w/o texture.
//  if (tm == '\0')
//  {
//    drawRect(x,y,z,width,height);
//    return;
//  }
  float w2 = width * 0.5;
  float h2 = height * 0.5;

  //z + depth face
  glNormal3f(0,0,-1);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(64) , inttot16(64)));
  glVertex3f(x-w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(128), inttot16(64)));
  glVertex3f(x+w2,y-h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(128), inttot16(0)));
  glVertex3f(x+w2,y+h2,z);
  tm->setTexture();
//GFX_TEX_COORD = (TEXTURE_PACK(inttot16(64) , inttot16(0)));
  glVertex3f(x-w2,y+h2,z);
}

void StaticDraw::drawRect(float x, float y, float z, float width, float height)
{
  float w2 = width * 0.5;
  float h2 = height * 0.5;

  glNormal3f(0,0,-1);
  glVertex3f(x-w2,y-h2,z);
  glVertex3f(x+w2,y-h2,z);
  glVertex3f(x+w2,y+h2,z);
  glVertex3f(x-w2,y+h2,z);
}

void StaticDraw::drawTri(float x, float y, float z, float width, float height, bool topBottom)
{
glEnd();
glBegin(GL_TRIANGLES);
  float w2 = width * 0.5;
  float h2 = height * 0.5;

  glNormal3f(0,0,-1);

  if (topBottom)
  {
    glVertex3f(x-w2,y-h2,z);
    glVertex3f(x+w2,y-h2,z);
    glVertex3f(x+w2,y+h2,z);
  }
  else
  {
    glVertex3f(x+w2,y+h2,z);
    glVertex3f(x-w2,y+h2,z);
    glVertex3f(x-w2,y-h2,z);
  }

glEnd();
glBegin(GL_QUADS);
}

void StaticDraw::drawTri(float x, float y, float z, float width, float height, bool topBottom, TextureManager *tm)
{
glEnd();
glBegin(GL_TRIANGLES);
  float w2 = width * 0.5;
  float h2 = height * 0.5;

  tm->resetTexture();

  //z + depth face
  glNormal3f(0,0,-1);

  if (topBottom)
  {
    tm->setTexture();
    glVertex3f(x-w2,y-h2,z);
    tm->setTexture();
    glVertex3f(x+w2,y-h2,z);
    tm->setTexture();
    glVertex3f(x+w2,y+h2,z);
  }
  else
  {
    tm->setTexture();
    glVertex3f(x+w2,y+h2,z);
    tm->setTexture();
    glVertex3f(x-w2,y+h2,z);
    tm->setTexture();
    glVertex3f(x-w2,y-h2,z);
  }

glEnd();
glBegin(GL_QUADS);
}
