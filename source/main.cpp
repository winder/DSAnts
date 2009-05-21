#include <nds.h>

// stdio for printf
#include <stdio.h>
// fat for disk I/O
#include <fat.h>
// stdlib for rand / srand
#include <stdlib.h>

#include "GameWorld.h"
#include "Underground.h"
#include "StaticDraw.h"
#include "Camera.h"
#include "memoryStats.h"
#include "cpu_usage.h"
#include "Lighting.h"
#include "TextureManager.h"
#include "dirt_one_img_bin.h"

#ifdef __PROFILING
#define __cplusplus
#include "cyg-profile.h"
#endif

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

	// even though this is used for profiling, put it out so cyg-profile.c doesn't complain
	int hblanks = 0;
#ifdef __PROFILING
	bool enabled = false;
	static void hblankCount (void)
	__attribute__ ((no_instrument_function));

	void hblankCount(){
		hblanks++;
	}
#endif

int main()
{	

	// Set vBlank callback.
	irqSet(IRQ_VBLANK,vBlank);


	// seed rand.
	srand(time(NULL));

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

	// initialize libfat.
	fatInitDefault();

	#ifdef __PROFILING
	irqEnable(IRQ_HBLANK);
	irqSet(IRQ_HBLANK, hblankCount);
	if (!enabled)
	{
		cygprofile_begin();
		cygprofile_enable();
		enabled = true;
	}
	#endif

/* // file write test.  file isn't written until fclose is called.
	// init lib fat
	if (fatInitDefault())
	{
		printf("lib fat init success.\n");
    FILE *fp;
    fp = fopen("/results.dat", "w");
		if (fp)
		{
			printf("opened file /results.dat\n");
			for (int i=0; i<=10; ++i)
				fprintf(fp, "%d, %d\n", i, i*i);
			printf("wrote to file.\n");
			fclose(fp);
			printf("closed the file.\n");
		}
		else
			printf("failed to open file.");
		while(true){};
	}
	else 
	{
		printf("lib fat init failure.");
		while(true){};
	}
*/

  //set mode 0, enable BG0 and set it to 3D
  videoSetMode(MODE_0_3D);

	// Camera needs to be initialized.
	gw->initCam();

	//---------------------//
	// INITIALIZE LIGHTING //
	//---------------------//
	Lighting light0;
	light0.setLight(0);
	// From cam perspective lights up: RIGHT, TOP, FRONT planes
//	light0.move(0.7f, 0.5f, 0.8f);
	light0.move(0.5f, -0.5f, 1.0f);
//	light0.move(1.0f, 1.0f, 1.0f);
	light0.color(19, 19, 19);
	light0.enable();

// Light 1, not needed anymore (can't be used, rather)
	Lighting light1;
	light1.setLight(1);
	light1.move(-0.7f, -0.2f, 0.8f);
//	light1.move(-1.0f, -1.0f, -1.0f);
	light1.color(11, 11, 11);
	light1.enable();
	light1.disable();
	glClearColor(0,0,0,0);
	glClearDepth(0x7FFF);

	light0.set();
	light1.set();

	glSetOutlineColor(0,RGB15(31,31,31));

	u32 cpu_percent = 0;
	//keep track of vertex ram usage
	int polygon_count;
	int vertex_count;

  // initialize gl
  glInit();

  //enable textures
  glEnable(GL_TEXTURE_2D);

  // enable antialiasing
//  glEnable(GL_ANTIALIAS);

  // setup the rear plane
  glClearColor(0,0,0,31); // BG must be opaque for AA to work
  glClearPolyID(63); // BG must have a unique polygon ID for AA to work
  glClearDepth(0x7FFF);

  //this should work the same as the normal gl call
  glViewport(0,0,255,191);

  vramSetBankA(VRAM_A_TEXTURE);

	// Init textures
	gw->initTex();

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

    glFlush(0);

		// Wait for next frame before doing any more.
    swiWaitForVBlank();

		// count the frame.
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
