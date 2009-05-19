#ifndef MAPDRAW_H
#define MAPDRAW_H

#include "Map.h"
#include "StaticDraw.h"
#include "Patch.h"
#include "Camera.h"
#include <vector>
#include "TextureManager.h"


// Textures:
#include "dirt_one_img_bin.h"
// need this for picking.
#include <nds/arm9/postest.h>

extern int textureID;

class MapDraw: public Map
{
	public:
		MapDraw();

		// this is an abstract class too.
		void initGrid() = 0;

		//---------------//
		// TEXTURE STUFF //
		//---------------//
		// Loads the textures.
		void initTextures();
		// in case of shared textures.
		// TODO: remove "setTextures" and "getTextures" ?
		// because of "setTextures" and "getTextures" this object may or may not own the texture.
		void setTextures(TextureManager* t){ tm = t; }
		TextureManager* getTextures(){ return tm; }
		void destroyTextures();

		// Drawing functions.
		Patch* draw();
		void drawAnts(std::vector<Ant*> &black, std::vector<Ant*> &red);
		bool drawAnt(Ant* a, bool animate);
		void drawPatch(float x, float y, Patch *p);

		void drawBox(float x, float y, float z, float width, float height, float depth);

		void drawCenteredBox(float x, float y, float z, float width, float height, float depth);
		void drawTextureCenteredBox(float x, float y, float z, float width, float height, float depth);

		void drawRect(float x, float y, float z, float width, float height);
		void drawTextureRect(float x, float y, float z, float width, float height);

		// This function does picking to find out where in the grid the touch occurred.
		bool pickPoint(int x, int y, Camera &cam);

		// returns NULL if nothing is picked.
		Patch* getPicked(){ return picked; }
		// returns true if the x/y coordinate is in the drawing range.
		bool isVisible(short x, short y);
		// if an object is visible (should be checked seperately) these will return the
		// MODELVIEW x / y coordinates.
		float positionX(short x);
		float positionY(short y);

		void shiftCenter(Ant *p);

		// Used for determining the center of the map.
		// TODO: increment a scene shift some amount before scrolling the tiles.
		virtual void incX(); 
		virtual void decX(); 
		virtual void incY(); 
		virtual void decY(); 

		void begin(){ glBegin(GL_QUADS); }
		void end(){ glEnd(); }
	private:
		void material(int r, int g, int b);

		// Note: these two functions and the rediculously clever picking method
		// was taken directly from Gabe Ghearing's Feb 2007 picking example.
		// 	I added a minor tweak to have the endCheck return a bool rather than store the value.
		// run before starting to draw an object while picking
		void startCheck();
		// run afer drawing an object while picking
		bool endCheck();

		float smoothScrollX, smoothScrollY;
		float boxSide;

		// the following are needed for picking.
		float closeW;
		int polyCount;
		bool pickMode;
		// Storage for picking.
		Patch *picked;

		TextureManager *tm;
};

#endif
