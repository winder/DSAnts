#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <nds.h>

class TextureManager
{
	public:
		TextureManager();
		~TextureManager();

		void load( int texBank, u8* image );
		void bind( );

		void setTexture();

	private:
		int textureID;
		int callNum;
};

#endif
