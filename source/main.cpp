
#include <nds.h>

#include <stdio.h>
#include "UndergroundDrawSphere.h"
#include "UndergroundDrawGrid.h"
#include "StaticDraw.h"
#include "Camera.h"
#include "UndergroundCamera.h"
#include "memoryStats.h"
#include "cpu_usage.h"
#include "Lighting.h"



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
	lcdMainOnBottom();
	//lcdMainOnTop();

	//setup the sub screen for basic printing
	// TODO: this is said to be very heavy weight for basic console needs.
	consoleDemoInit();

	// Exception handler.
	defaultExceptionHandler();

	// I threw this in here for testing.
	// enable edge outlining, this will be used to show which object is selected
	glEnable(GL_OUTLINE);
	//set the first outline color to white
	glSetOutlineColor(0,RGB15(31,31,31));

	// Camera class..
	Camera cam;
	UndergroundCamera ugCam;

	cam.init();
	ugCam.init();
	// Lights are as follows:
	// light0 = left->right, bottom->top, rear->front.
	// light1 = right->left, top->bottom, front->rear.
	Lighting light0;
	light0.setLight(0);
	// From cam perspective lights up: RIGHT, TOP, FRONT planes
	light0.move(0.7f, 0.5f, 0.8f);
	light0.color(31, 1, 1);
	light0.color(31, 31, 31);
	light0.enable();

	Lighting light1;
	light1.setLight(1);
	// From cam perspective lights up: LEFT, BOTTOM, REAR planes
	light1.move(-0.7f, -0.2f, -0.8f);
	light1.color(1, 1, 31);
	light1.color(31, 31, 31);
	light1.enable();
	glClearColor(0,0,0,0);
	glClearDepth(0x7FFF);

	light0.set();
	light1.set();

	glSetOutlineColor(0,RGB15(31,31,31));


	//UndergroundDrawSphere *ug = new UndergroundDrawSphere();
	UndergroundDrawGrid *ug = new UndergroundDrawGrid();

	u32 cpu_percent = 0;
	//keep track of vertex ram usage
	int polygon_count;
	int vertex_count;

	//object 
	// v10, this seems to work great but could have issues with resolution.
	v10 rx = 0, ry = 0;
	int oldx = 0, oldy = 0;

	float deltaPointer = 0;
	bool attempt, at;

	//main loop
	while (1) {
		//process input
		scanKeys();
		int held = keysHeld();
		int pressed = keysDown();
		
		if( held & KEY_R) cam.translateZinc(0.1);
		if( held & KEY_L) cam.translateZinc(-0.1);

		// D-Pad to translate
		if( held & KEY_LEFT) ug->decX();
		if( held & KEY_RIGHT) ug->incX();
		if( held & KEY_UP) ug->incY();
		if( held & KEY_DOWN) ug->decY();

		if( held & KEY_L) ugCam.zoomIn();
		if( held & KEY_R) ugCam.zoomOut();

		touchRead(&touchXY);
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

	glClearColor(0,0,0,0);
	glClearDepth(0x7FFF);
//	light0.move(rx, ry, (v10)0.5);
//	light1.move(rx*-1, ry*-1, (v10)-0.5);
	light0.set();
	light1.set();
	glSetOutlineColor(0,RGB15(31,31,31));

		glViewport(0,0,255,191); // set the viewport to fullscreen
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//change ortho vs perspective
		if(keysHeld() & KEY_B)
			cam.Ortho();
		else 
			cam.Perspective();

		//change cull mode
		if( held & KEY_A)
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | light0.getPolyFmtFlag() | light1.getPolyFmtFlag()  | POLY_ID(1));
		else
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT | light0.getPolyFmtFlag() | light1.getPolyFmtFlag()  | POLY_ID(1));

		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//--------------//
		// PLACE CAMERA //
		//--------------//
		cam.render();

		//------------//
		// DRAW SCENE //
		//------------//
		CPU_StartTest(0,0);

		ug->draw();
		// If the touch pad is being touched... see what its touching.
		if( held & KEY_TOUCH)
		{ at = true;
			attempt = ug->pickPoint(touchXY.px, touchXY.py, cam);
		} else at = false;

		cpu_percent = CPU_EndTest();

		deltaPointer = ry;

		swiWaitForVBlank();

		while (GFX_STATUS & (1<<27)); // wait until the geometry engine is not busy

		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vertex_count);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &polygon_count);

		// Clear console... no more of these crazy \x1b[2J codes
		consoleClear();
		printf("My variables:\n");
		if (at){ printf("\n TRIED TO PICK Result: ");
				if (attempt) printf("HIT");
				else printf("MISS");
		} else printf("\n DID NOT PICK");
//		printf("\nCamera location: (%f,", ugCam.getCamLocation().x);
//		printf("\n                  %f,", ugCam.getCamLocation().y);
//		printf("\n                  %f)", ugCam.getCamLocation().z);
		printf("\nGrid slice/X/Y = %i/%i/%i", ug->slice, ug->centerX, ug->centerY);
		printf("\nTouching: (%i, %i)", touchXY.px, touchXY.py);
		printf("\nCamera location: (%f,", cam.getCamLocation().x);
		printf("\n                  %f,", cam.getCamLocation().y);
		printf("\n                  %f)", cam.getCamLocation().z);

		printf("\n\nMemory Statistics:");
		printf("\n\tTotal mem: %i", getMemUsed() + getMemFree());
		float pc_free = (float)getMemFree() / (float)(getMemUsed()+getMemFree()) * 100.0;
		float pc_used = (float)getMemUsed() / (float)(getMemUsed()+getMemFree()) * 100.0;
		printf("\n\tMem Used: %i (%.1f%%)", getMemUsed(), pc_used);
		printf("\n\tMem Free: %i (%.1f%%)", getMemFree(), pc_free);
//		printf("\n\nCPU Percent for drawing: %i%", cpu_percent);
		printf("\n\n");
		PrintMemoryStatistics();
		printf("\nVideo info: Vert Ram = %i", vertex_count);
		printf("\n            Poly Count = %i", polygon_count);

		// flush to the screen
		glFlush(0);

	}

	return 0;
}
