#include "TextureManager.h"

TextureManager::TextureManager()
{
	textureID = 0;
}


void TextureManager::load( u8* image )
{
  glGenTextures(1, &textureID);
  glBindTexture(0, textureID);
  glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, image );
}

void TextureManager::bind( int texBank )
{
	glBindTexture(texBank, textureID);
}
