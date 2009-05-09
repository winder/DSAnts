
#include <nds.h>

#include <stdio.h>
#include "UndergroundDraw.h"
#include "StaticDraw.h"
#include "Camera.h"
#include "UndergroundCamera.h"


int main()
{	

	touchPosition touchXY;
	//put 3D on top/bottom
	//lcdMainOnBottom();
	lcdMainOnTop();

	//setup the sub screen for basic printing
	consoleDemoInit();

	// Camera class..
	Camera cam;
	UndergroundCamera ugCam;

	cam.init();
	ugCam.init();
	UndergroundDraw *ug = new UndergroundDraw();

	//keep track of vertex ram usage
	int polygon_count;
	int vertex_count;

	//object 
	int rx = 0, ry = 0;
	int oldx = 0, oldy = 0;

	float deltaPointer = 0;

	//main loop
	while (1) {

		//process input
		scanKeys();

		touchRead(&touchXY);

		
		int held = keysHeld();
		int pressed = keysDown();
		
		// D-Pad to translate.
/*
		if( held & KEY_LEFT) cam.translateXinc(0.1);
		if( held & KEY_RIGHT) cam.translateXinc(-0.1);
		if( held & KEY_UP) cam.translateYinc(0.1);
		if( held & KEY_DOWN) cam.translateYinc(-0.1);
		if( held & KEY_R) cam.translateZinc(0.1);
		if( held & KEY_L) cam.translateZinc(-0.1);
*/
		if( held & KEY_LEFT) ugCam.moveLeft();
		if( held & KEY_RIGHT) ugCam.moveRight();
		if( held & KEY_UP) ugCam.moveUp();
		if( held & KEY_DOWN) ugCam.moveDown();

		if( held & KEY_L) ugCam.zoomIn();
		if( held & KEY_R) ugCam.zoomOut();

//		if( pressed & KEY_UP || pressed & KEY_DOWN ) udButton = 0;
//		if( held & KEY_UP) udButton += 0.1;
//		if( held & KEY_DOWN) udButton -= 0.1;
//		if( held & KEY_L) translate += .1;
//		if( held & KEY_R) translate -= .1;

		//reset x and y when user touches screen
		if( pressed & KEY_TOUCH)  {
			oldx = touchXY.px;
			oldy = touchXY.py;
		}

		//if user drags then grab the delta
		if( held & KEY_TOUCH) {
			rx += touchXY.px - oldx; 
			ry += touchXY.py - oldy;
			oldx = touchXY.px;
			oldy = touchXY.py;
		}
		// I had to swap ry and rx, not exactly sure why ....
		cam.rotateXinc(ry * 0.31);
		cam.rotateYinc(rx * 0.31);
		rx = 0;
		ry = 0;

		//change ortho vs perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(keysHeld() & KEY_B)
			cam.Perspective();
		else 
			cam.Ortho();

		//change cull mode
//		if( held & KEY_A)
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
//		else
//			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT );

		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//handle camera
		//cam.rotate();
		//cam.move();
		//cam.render();
		ugCam.render();
		ug->draw();

		deltaPointer = ry;

		swiWaitForVBlank();

		while (GFX_STATUS & (1<<27)); // wait until the geometry engine is not busy

		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vertex_count);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &polygon_count);

		// Clear console... no more of these crazy \x1b[2J codes
		consoleClear();
		printf("My variables:\n");
		printf("\nCamera location: (%f,", ugCam.getCamLocation().x);
		printf("\n                  %f,", ugCam.getCamLocation().y);
		printf("\n                  %f)", ugCam.getCamLocation().z);
/*
		printf("\nPlayer Position: %f", p1->getPosition());
		printf("\n");
		printf("\nPointer deltas:");
		printf("\n  x-coord: %d", rx);
		printf("\n  y-coord: %f", deltaPointer);

		printf("\n");
		printf("\nBall info:");
		printf("\n  x-coord: %f", ball->getX());
		printf("\n  y-coord: %f", ball->getY());
		printf("\n  Direction vector:");
		printf("\n    x-coord: %f", ball->velX());
		printf("\n    y-coord: %f", ball->velY());
*/
//		printf("\n\nRam usage: Culling none" );
//		printf("\nVertex ram: %i", vertex_count);
//		printf("\nPolygon ram: %i", polygon_count);

		// flush to the screen
		glFlush(0);

	}

	return 0;
}
