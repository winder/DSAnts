#include "TextureManager.h"

TextureManager::TextureManager()
{
	callNum = 0;
	textureID = 0;
}

TextureManager::~TextureManager()
{
}

void TextureManager::load( int texBank, u8* image )
{
  glGenTextures(1, &textureID);
  glBindTexture(0, textureID);
  glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, image );
}

void TextureManager::bind( )
{
  glBindTexture(0, textureID);
}

//TODO: there might be a good algorithmic way to do this.
void TextureManager::setTexture()
{
	// reset callNum every 4th call.
	if (callNum >= 4) callNum = 0;

	// Top Left 64 bit texture.
	if (curTex == 0)
	{
		if (callNum == 0)
			GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
		else if (callNum == 1)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
		else if (callNum == 2)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
		else if (callNum == 3)
			GFX_TEX_COORD = (TEXTURE_PACK(0,0));
	}
	// Top Right 64 bit texture.
	else if (curTex == 1)

	{
		if (callNum == 0)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(64) , inttot16(64)));
		else if (callNum == 1)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(128), inttot16(64)));
		else if (callNum == 2)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(128), inttot16(0)));
		else if (callNum == 3)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(64) , inttot16(0)));
	}

	callNum++;
}

// Selects which texture setTexture will return for.
void TextureManager::nextTexture(int ID)
{
	// reset the call count.
	curTex = ID;
	callNum = 0;
}
