#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <nds.h>

class TextureManager
{
	public:
		TextureManager();

		void load( u8* image );
		void bind( int texBank );

	private:
		int textureID;
};

#endif
