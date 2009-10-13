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

    // This is the main function, it is called during the OpenGL calls to setup the texture coordinates.
    void setTexture();
    // Selects which texture setTexture will return for.
    void nextTexture(int ID);

    // Reset texture
    void resetTexture();

  private:
    int textureID;
    int callNum;
    int curTex;
};

#endif
