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

void DrawLine(u16* framebuffer, int x1, int y1, int x2, int y2, unsigned short color)
{
    int yStep = SCREEN_WIDTH;
    int xStep = 1;      
    int xDiff = x2 - x1;
    int yDiff = y2 - y1;
 
    int errorTerm = 0;
    int offset = y1 * SCREEN_WIDTH + x1; 
    int i; 
    
    //need to adjust if y1 > y2
    if (yDiff < 0)       
    {                  
       yDiff = -yDiff;   //absolute value
       yStep = -yStep;   //step up instead of down   
    }
    
    //same for x
    if (xDiff < 0) 
    {           
       xDiff = -xDiff;            
       xStep = -xStep;            
    }        
 
    //case for changes more in X than in Y   
    if (xDiff > yDiff) 
    {                            
       for (i = 0; i < xDiff + 1; i++)
       {                           
          framebuffer[offset] = color;  
          framebuffer[offset+1] = color;  
          framebuffer[offset+2] = color;  
 
          offset += xStep;           
 
          errorTerm += yDiff;     
 
          if (errorTerm > xDiff) 
          {  
             errorTerm -= xDiff;     
             offset += yStep;        
          }
       }
    }//end if xdiff > ydiff
    //case for changes more in Y than in X
    else 
    {                       
       for (i = 0; i < yDiff + 1; i++) 
       {  
          framebuffer[offset] = color;  
          framebuffer[offset+1] = color;  
          framebuffer[offset+2] = color;  
 
          offset += yStep;           
 
          errorTerm += xDiff;    
 
          if (errorTerm > yDiff) 
          {     
             errorTerm -= yDiff;  
             offset += xStep;     
 
          }
       }
    }
 
}

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
  GameWorld *gw = GameWorldSingleton::getInstance();

  //put 3D on top/bottom
  lcdMainOnBottom();
  //lcdMainOnTop();

  //setup the sub screen for basic printing
  // TODO: this is said to be very heavy weight for basic console needs.
  consoleDemoInit();

/*
  //set mode 0, enable BG0 and set it to 3D
  videoSetMode(MODE_0_3D);

//  videoSetModeSub(MODE_5_2D);
  videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);
  vramSetBankC(VRAM_C_SUB_BG);
//  vramSetBankH(VRAM_H_SUB_BG);
//  vramSetBankI(VRAM_I_SUB_BG_0x06208000);
  //initialize the sub background
  BACKGROUND_SUB.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0);
  
  BACKGROUND_SUB.bg3_rotation.xdy = 0;
  BACKGROUND_SUB.bg3_rotation.xdx = 1 << 8;
  BACKGROUND_SUB.bg3_rotation.ydx = 0;
  BACKGROUND_SUB.bg3_rotation.ydy = 1 << 8;

  u16* video_buffer_sub = (u16*)BG_BMP_RAM_SUB(0);
  for(int y=0; y < 256; y++)
  {
     for(int x=0; x < 256; x++)
        video_buffer_sub[x + y * 256] = RGB15(0,0,x%31) | BIT(15);
  }
  //paint the sub screen blue
//  for(int i = 0; i < 256 * 256; i++)
//    video_buffer_sub[i] = RGB15(0,0,31) | BIT(15);

  DrawLine(video_buffer_sub, 0, 0, 250, 192, rand());       
  DrawLine(video_buffer_sub, 250, 0, 0, 192, rand());       
*/
/*
  int SubPtr = bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
  for(int i=0; i < 256; i++)
  {
     BG_PALETTE_SUB[i] = RGB15(i & 0x1F, i & 0x1F, i & 0x1F);
  }

  u16 *SubData = bgGetGfxPtr(SubPtr);
  for(int y=0; y < 128; y++)
  {
     for(int x=0; x < 192; x++)
        SubData[x + y * 192] = ((x * 2) & 0xFF) | (((x * 2 + 1) << 8) & 0xFF00);

      DrawLine(SubData, 0, 0, 250, 192, rand());       
      DrawLine(SubData, 250, 0, 0, 192, rand());       
  }
*/

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

  // Camera needs to be initialized.
  gw->initCam();

  //---------------------//
  // INITIALIZE LIGHTING //
  //---------------------//
  Lighting light0;
  light0.setLight(0);
  // From cam perspective lights up: RIGHT, TOP, FRONT planes
//  light0.move(0.7f, 0.5f, 0.8f);
  light0.move(0.5f, -0.5f, 1.0f);
//  light0.move(1.0f, 1.0f, 1.0f);
  light0.color(19, 19, 19);
  light0.enable();

// Light 1, not needed anymore (can't be used, rather)
  Lighting light1;
  light1.setLight(1);
  light1.move(-0.7f, -0.2f, 0.8f);
//  light1.move(-1.0f, -1.0f, -1.0f);
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
  vramSetBankA(VRAM_A_TEXTURE);

  // enable antialiasing
//  glEnable(GL_ANTIALIAS);

  // setup the rear plane
  glClearColor(0,0,0,31); // BG must be opaque for AA to work
  glClearPolyID(63); // BG must have a unique polygon ID for AA to work
  glClearDepth(0x7FFF);

  //this should work the same as the normal gl call
  glViewport(0,0,255,191);


  // Init textures
  gw->initTex();

  //main loop
  while (1) {
    // Tell the world to move forward one step.
    // TODO: put this in VBlank override, so that even if
    //       drawing gets too slow the world will still move forward.
    // VBlank didn't seem to work, so calculate steps based on framerate
    // I'm told that the way I calculate framerate is bad, so implement timer at
    // some point.
    if (frameold == 60)
      gw->stepForward(1);
    else if (frameold >= 30)
      gw->stepForward(2);
    else if (frameold >= 20)
      gw->stepForward(3);

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

    printf("\nMemory Statistics:");
    printf("\n\tTotal mem: %i", getMemUsed() + getMemFree());
    float pc_free = (float)getMemFree() / (float)(getMemUsed()+getMemFree()) * 100.0;
    float pc_used = (float)getMemUsed() / (float)(getMemUsed()+getMemFree()) * 100.0;
    printf("\n\tMem Used: %i (%.1f%%)", getMemUsed(), pc_used);
    printf("\n\tMem Free: %i (%.1f%%)", getMemFree(), pc_free);
//    printf("\n\nCPU Percent for drawing: %i%", cpu_percent);
    printf("\n");
    PrintMemoryStatistics();
//    printf("\nVideo info: Vert Ram = %i", vertex_count);
//    printf("\n            Poly Count = %i", polygon_count);
    gw->printDebugFiveLines();

    printf("\n FPS: %i", frameold);

    // flush to the screen

  }

  return 0;
}
