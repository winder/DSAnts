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
//	if (tm == '\0')
//	{
//		drawBox(x,y,z,width,height,depth);
//		return;
//	}

	float w2 = width * 0.5;
	float h2 = height * 0.5;

	//z  face
	glNormal3f(0,0,1);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z);

	//z + depth face
	glNormal3f(0,0,-1);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z+depth);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z+depth);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z+depth);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z+depth);

	//x  face
	glNormal3f(1,0,0);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z+depth);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z+depth);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z);

	//x + width face
	glNormal3f(-1,0,0);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z+depth);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z+depth);

	//y  face
	glNormal3f(0,-1,0);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z+depth);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z+depth);

	//y  + height face
	glNormal3f(0,1,0);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z+depth);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z+depth);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z);

}

void StaticDraw::drawRect(float x, float y, float z, float width, float height, TextureManager *tm)
{
//TODO: um.. why doesn't this work?
	// if this was called with a null texture, draw w/o texture.
//	if (tm == '\0')
//	{
//		drawRect(x,y,z,width,height);
//		return;
//	}
	float w2 = width * 0.5;
	float h2 = height * 0.5;

	//z + depth face
	glNormal3f(0,0,-1);
	tm->setTexture();
	glVertex3f(x-w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y-h2,z);
	tm->setTexture();
	glVertex3f(x+w2,y+h2,z);
	tm->setTexture();
	glVertex3f(x-w2,y+h2,z);
}

void StaticDraw::drawRect(float x, float y, float z, float width, float height)
{
	float w2 = width * 0.5;
	float h2 = height * 0.5;

	glNormal3f(0,0,1);
	glVertex3f(x-w2,y-h2,z);
	glVertex3f(x+w2,y-h2,z);
	glVertex3f(x+w2,y+h2,z);
	glVertex3f(x-w2,y+h2,z);
}

