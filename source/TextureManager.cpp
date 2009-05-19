#include "TextureManager.h"

TextureManager::TextureManager()
{
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

void TextureManager::setTexture()
{
	if (callNum >= 4) callNum = 0;
//	if ( currentTexture == WHATEVER )

		if (callNum == 0)
			GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(63)));
		else if (callNum == 1)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63),inttot16(63)));
		else if (callNum == 2)
			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(63), 0));
		else if (callNum == 2)
			GFX_TEX_COORD = (TEXTURE_PACK(0,0));
			
}
