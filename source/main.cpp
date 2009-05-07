
#include <nds.h>

#include <stdio.h>


//---------------------------------------------------------------------------------
int main() {	
//---------------------------------------------------------------------------------

	touchPosition touchXY;

	//put 3D on bottom
	lcdMainOnBottom();

	//setup the sub screen for basic printing
	consoleDemoInit();

	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	// Set our view port to be the same size as the screen
	glViewport(0,0,255,191);
	
	//camera
//	float rotX = 0, rotY = 0;


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
		
//		if( held & KEY_LEFT) rotY++;
//		if( held & KEY_RIGHT) rotY--;
//		if( pressed & KEY_UP || pressed & KEY_DOWN ) udButton = 0;
//		if( held & KEY_UP) udButton ++;
//		if( held & KEY_DOWN) udButton --;
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
			gluPerspective(70, 256.0 / 192.0, 0.1, 10);
		else 
			glOrtho(-4,4,-3,3,0.1,10);	

		//change cull mode
		if( held & KEY_A)
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
		else
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT );

		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//handle camera
//		glRotateY(rotY);
//		glRotateX(rotX);
//		glTranslatef(0,0,translate);

		deltaPointer = ry;

		swiWaitForVBlank();

		while (GFX_STATUS & (1<<27)); // wait until the geometry engine is not busy

		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vertex_count);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &polygon_count);

		// Clear console... no more of these crazy \x1b[2J codes
		consoleClear();
		printf("My variables:\n");
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
