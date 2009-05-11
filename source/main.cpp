
#include <nds.h>

#include <stdio.h>
#include "UndergroundDrawSphere.h"
#include "UndergroundDrawGrid.h"
#include "StaticDraw.h"
#include "Camera.h"
#include "UndergroundCamera.h"
#include "memoryStats.h"


void drawXYZaxis()
{
	glBegin(GL_QUADS);

	// X axis.
	glColor3f(1,1,0);
	glVertex3f(-20.0f, 0.01f, 0.01f);
	glVertex3f(20.0f, 0.01f, 0.01f);
	glVertex3f(20.0f, -0.01f, -0.01f);
	glVertex3f(-20.0f, -0.01f, -0.01f);

	// Y axis.
	glColor3f(0,1,1);
	glVertex3f(0.01f, -20.0f, 0.01f);
	glVertex3f(0.01f, 20.0f, 0.01f);
	glVertex3f(-0.01f, 20.0f, -0.01f);
	glVertex3f(-0.01f, -20.0f, -0.01f);
	glEnd();

	// Y axis.
	glColor3f(1,1,1);
	glVertex3f(0.01f, 0.01f, -20.0f);
	glVertex3f(0.01f, 0.01f, 20.0f);
	glVertex3f(-0.01f, -0.01f, 20.0f);
	glVertex3f(-0.01f, -0.1f, -20.0f);
}

int main()
{	

	touchPosition touchXY;
	//put 3D on top/bottom
	//lcdMainOnBottom();
	lcdMainOnTop();

	//setup the sub screen for basic printing
	consoleDemoInit();

	// Exception handler.
	defaultExceptionHandler();

	// Camera class..
	Camera cam;
	UndergroundCamera ugCam;

	cam.init();
	ugCam.init();

	glClearColor(0,0,0,0);
	glClearDepth(0x7FFF);
  glLight(0, RGB15(31,31,31) , 0, floattov10(-1.0), 0); // setup the light


	//UndergroundDrawSphere *ug = new UndergroundDrawSphere();
	UndergroundDrawGrid *ug = new UndergroundDrawGrid();

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
*/
		if( held & KEY_R) cam.translateZinc(0.1);
		if( held & KEY_L) cam.translateZinc(-0.1);

/* // CAMERA control with D-Pad
		if( held & KEY_LEFT) ugCam.moveLeft();
		if( held & KEY_RIGHT) ugCam.moveRight();
		if( held & KEY_UP) ugCam.moveUp();
		if( held & KEY_DOWN) ugCam.moveDown();
*/

		if( held & KEY_LEFT) ug->decX();
		if( held & KEY_RIGHT) ug->incX();
		if( held & KEY_UP) ug->incY();
		if( held & KEY_DOWN) ug->decY();

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

		//change ortho vs perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(keysHeld() & KEY_B)
			cam.Ortho();
		else 
			cam.Perspective();

		//change cull mode
		if( held & KEY_A)
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_FORMAT_LIGHT0);
			//glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT );
		else
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0);
			//glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//handle camera
//		drawXYZaxis();
//		ugCam.render();
		cam.rotate();
		cam.move();
//		cam.render();
		ug->draw();

		deltaPointer = ry;

		swiWaitForVBlank();

		while (GFX_STATUS & (1<<27)); // wait until the geometry engine is not busy

		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vertex_count);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &polygon_count);

		// Clear console... no more of these crazy \x1b[2J codes
		consoleClear();
		printf("My variables:\n");
//		printf("\nCamera location: (%f,", ugCam.getCamLocation().x);
//		printf("\n                  %f,", ugCam.getCamLocation().y);
//		printf("\n                  %f)", ugCam.getCamLocation().z);
		printf("\nGrid slice/X/Y = %i/%i/%i", ug->slice, ug->centerX, ug->centerY);
		printf("\nCamera location: (%f,", cam.getCamLocation().x);
		printf("\n                  %f,", cam.getCamLocation().y);
		printf("\n                  %f)", cam.getCamLocation().z);

		printf("\n\nMemory Statistics:");
		printf("\n\t\tTotal mem: %i", getMemUsed() + getMemFree());
		printf("\n\t\tMem Used: %i", getMemUsed());
		printf("\n\t\tMem Free: %i", getMemFree());

		// flush to the screen
		glFlush(0);

	}

	return 0;
}
