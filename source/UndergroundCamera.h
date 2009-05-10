#ifndef UNDERGROUNDCAMERA_H
#define UNDERGROUNDCAMERA_H

#include <nds.h>
#include <math.h>

#include "global.h"
#include "Camera.h"
#include "StaticDraw.h"


// This class is special because it runs along a circular track around the
// origin, looking inward.
class UndergroundCamera:public Camera
{
	public:
		UndergroundCamera():Camera(){ cached=false; radius=6; track=0; lat=0;}

		void moveRight(){ track+=2.5; cached=false; }
		void moveLeft(){ track-=2.5; cached=false; }
		void moveUp(){	lat+=2.5; cached=false; }
		void moveDown(){ lat-=2.5; cached=false; }
		void zoomIn(){ radius+=0.1; cached=false; }
		void zoomOut(){ radius-=0.1; cached=false; }
		
		void setTrack(float x){ track=x; cached=false; }

		virtual void render();
		void calcPosition();
		VectorF getCamLocation(){ return camLoc; }

	private:
		float radius;
		float track;
		float lat;
		VectorF camLoc;
		bool cached;
};

#endif
