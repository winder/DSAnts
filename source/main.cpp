#include <nds.h>

// stdio for printf
#include <stdio.h>
// stdlib for rand / srand
#include <stdlib.h>
#include "GameWorld.h"
//#include "UndergroundDrawSphere.h"
#include "Underground.h"
#include "StaticDraw.h"
#include "Camera.h"
#include "SphereCamera.h"
#include "memoryStats.h"
#include "cpu_usage.h"
#include "Lighting.h"

#include "dirt_one_img_bin.h"
//#include "texture_bin.h"

//TODO: setup the function that implements the VBlank handling
//      put a GameWorld->step() call in there.  That way even
//      if I get to the point where things are too complicated
//      to draw, things still feel like they move in real time.



// FPS calculation.
int frameCounter=0;
int loopCounter=0;
int elapsedFrames=0;
int frameold=0;

void vBlank(void){
   ++elapsedFrames;
   ++frameCounter;
      
   if (frameCounter>=60){
      frameold=loopCounter;
      frameCounter=0;
      loopCounter=0;
   }
}

int main()
{	

	// Set vBlank callback.
	irqSet(IRQ_VBLANK,vBlank);

	time_t t;
	t = time(NULL);
	srand(t);

	//UndergroundDrawGrid *ug = new UndergroundDrawGrid();
	GameWorld *gw = new GameWorld();

	//put 3D on top/bottom
	lcdMainOnBottom();
	//lcdMainOnTop();

	//setup the sub screen for basic printing
	// TODO: this is said to be very heavy weight for basic console needs.
	consoleDemoInit();

	// Exception handler.
	defaultExceptionHandler();

  //set mode 0, enable BG0 and set it to 3D
  videoSetMode(MODE_0_3D);

	// Camera needs to be initialized.
	gw->init();

	//---------------------//
	// INITIALIZE LIGHTING //
	//---------------------//
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

	u32 cpu_percent = 0;
	//keep track of vertex ram usage
	int polygon_count;
	int vertex_count;






  int textureID;
  // initialize gl
  glInit();

  //enable textures
//  glEnable(GL_TEXTURE_2D);

  // enable antialiasing
//  glEnable(GL_ANTIALIAS);

  // setup the rear plane
  glClearColor(0,0,0,31); // BG must be opaque for AA to work
  glClearPolyID(63); // BG must have a unique polygon ID for AA to work
  glClearDepth(0x7FFF);

  //this should work the same as the normal gl call
  glViewport(0,0,255,191);

  vramSetBankA(VRAM_A_TEXTURE);

  glGenTextures(1, &textureID);
  glBindTexture(0, textureID);
  glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)dirt_one_img_bin);
  //glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)texture_bin);


	//main loop
	while (1) {


		// Tell the world to move forward one step.
		// TODO: put this in VBlank override, so that even if
		//			 drawing gets too slow the world will still move forward.
		gw->stepForward();

		// not needed?
		glResetMatrixStack();
		glClearColor(0,0,0,0);
		glClearDepth(0x7FFF);

		// Place lights (this may move to the GameWorld at some point);
		//light0.move(rx, ry, (v10)0.5);
		//light1.move(rx*-1, ry*-1, (v10)-0.5);
		light0.set();
		light1.set();

		glViewport(0,0,255,191); // set the viewport to fullscreen
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gw->setProjection();

		// enable culling.
		//glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT | light0.getPolyFmtFlag() | light1.getPolyFmtFlag()  | POLY_ID(1));
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | light0.getPolyFmtFlag() | light1.getPolyFmtFlag()  | POLY_ID(1));

		// Set the current matrix to be the model matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//--------------//
		// PLACE CAMERA //
		//--------------//
		gw->placeCamera();

		//------------//
		// DRAW SCENE //
		//------------//
		CPU_StartTest(0,0);

		// draw the scene, this does the picking too.
		gw->draw();

		cpu_percent = CPU_EndTest();



    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //move it away from the camera
/*
    glMaterialf(GL_AMBIENT, RGB15(16,16,16));
    glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
    glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
    glMaterialf(GL_EMISSION, RGB15(16,16,16));
*/
	glMaterialf(GL_DIFFUSE, RGB15(1, 11, 31) | BIT(15)); /// Bit 15 enables the diffuse color to act like being set with glColor(), only with lighting support. When not using lighting, this is going to be the default color, just like being set with glColor().
	glMaterialf(GL_AMBIENT, RGB15(15, 15, 15));
	glMaterialf(GL_SPECULAR, RGB15(15, 15, 15)); /// Bit 15 would have to be set here to enable a custom specularity table, instead of the default linear one.
	glMaterialf(GL_EMISSION, RGB15(25, 25, 25));

    //ds uses a table for shinyness..this generates a half-ass one
//    glMaterialShinyness();

    //not a real gl function and will likely change
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);

    glBindTexture(0, textureID);
/*
    //draw the obj
    glBegin(GL_QUAD);
      glNormal(NORMAL_PACK(0,inttov10(-1),0));

      GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(128)));
      glVertex3v16(floattov16(-5.5),  floattov16(-5.5), 0 );

      GFX_TEX_COORD = (TEXTURE_PACK(inttot16(128),inttot16(128)));
      glVertex3v16(floattov16(5.5), floattov16(-5.5), 0 );

      GFX_TEX_COORD = (TEXTURE_PACK(inttot16(128), 0));
      glVertex3v16(floattov16(5.5), floattov16(5.5), 0 );

      GFX_TEX_COORD = (TEXTURE_PACK(0,0));
      glVertex3v16(floattov16(-5.5),  floattov16(5.5), 0 );

    glEnd();
*/
    glPopMatrix(1);

    glFlush(0);

    swiWaitForVBlank();




    loopCounter++; 


		while (GFX_STATUS & (1<<27)); // wait until the geometry engine is not busy

		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vertex_count);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &polygon_count);

		// Clear console... no more of these crazy \x1b[2J codes
		consoleClear();
		printf("My variables:\n");

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
		gw->printDebugFiveLines();

		printf("\n FPS: %i", frameold);

		// flush to the screen

	}

	return 0;
}
