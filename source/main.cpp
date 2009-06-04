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



#ifdef PATATER_TUTORIAL_BACKGROUNDS
// This is from Patater's tutorial, will setup video and stuff using backgrounds.

#include "starField.h"
#include "planet.h"
#include "splash.h"

static const int DMA_CHANNEL = 3;

void displayStarField() {
    dmaCopyHalfWords(DMA_CHANNEL,
                     starFieldBitmap, /* This variable is generated for us by
                                       * grit. */
                     (uint16 *)BG_BMP_RAM(0), /* Our address for main
                                               * background 3 */
                     starFieldBitmapLen); /* This length (in bytes) is generated
                                           * from grit. */
}

void displayPlanet() {
    dmaCopyHalfWords(DMA_CHANNEL,
                     planetBitmap, /* This variable is generated for us by
                                    * grit. */
                     (uint16 *)BG_BMP_RAM(8), /* Our address for main
                                               * background 2 */
                     planetBitmapLen); /* This length (in bytes) is generated
                                        * from grit. */
}

void displaySplash() {
    dmaCopyHalfWords(DMA_CHANNEL,
                     splashBitmap, /* This variable is generated for us by
                                    * grit. */
                     (uint16 *)BG_BMP_RAM_SUB(0), /* Our address for sub
                                                   * background 3 */
                     splashBitmapLen); /* This length (in bytes) is generated
                                        * from grit. */
}
            

void initVideo() {
    /*
     *  Map VRAM to display a background on the main and sub screens.
     * 
     *  The vramSetMainBanks function takes four arguments, one for each of the
     *  major VRAM banks. We can use it as shorthand for assigning values to
     *  each of the VRAM bank's control registers.
     *
     *  We map banks A and B to main screen  background memory. This gives us
     *  256KB, which is a healthy amount for 16-bit graphics.
     *
     *  We map bank C to sub screen background memory.
     *
     *  We map bank D to LCD. This setting is generally used for when we aren't
     *  using a particular bank.
     */
    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_LCD);

    /*  Set the video mode on the main screen. */
    videoSetMode(MODE_5_2D | // Set the graphics mode to Mode 5
                 DISPLAY_BG2_ACTIVE | // Enable BG2 for display
                 DISPLAY_BG3_ACTIVE); //Enable BG3 for display

    /*  Set the video mode on the sub screen. */
    videoSetModeSub(MODE_5_2D | // Set the graphics mode to Mode 5
                    DISPLAY_BG3_ACTIVE); // Enable BG3 for display
}

void initBackgrounds() {
    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /*  Set up affine background 2 on main as a 16-bit color background. */
    REG_BG2CNT = BG_BMP16_128x128 |
                 BG_BMP_BASE(8) | // The starting place in memory
                 BG_PRIORITY(2);  // A higher priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG2PA = 1 << 8;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 1 << 8;

    /*  Place main screen background 2 in an interesting place. */
    REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
    REG_BG2Y = -32 << 8;

    /*  Set up affine background 3 on the sub screen as a 16-bit color
     *  background.
     */
    REG_BG3CNT_SUB = BG_BMP16_256x256 |
                     BG_BMP_BASE(0) | // The starting place in memory
                     BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the sub screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PB_SUB = 0;
    REG_BG3PC_SUB = 0;
    REG_BG3PD_SUB = 1 << 8;

    /*
     *  Place main screen background 3 at the origin (upper left of the screen)
     */
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 0;
}
#endif


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

#ifdef PATATER_TUTORIAL_BACKGROUNDS
    /*  Turn on the 2D graphics core. */
    powerOn(POWER_ALL_2D);

    /*  Configure the VRAM and background control registers. */
    lcdMainOnBottom(); // Place the main screen on the bottom physical screen
    initVideo(); 
    initBackgrounds(); 

    /*  Display the backgrounds. */
    displayStarField(); 
//    displayPlanet();
    displaySplash();
#endif




  // Set vBlank callback.
  irqSet(IRQ_VBLANK,vBlank);


  // seed rand.
  srand(time(NULL));

  //UndergroundDrawGrid *ug = new UndergroundDrawGrid();
//  GameWorld *gw = new GameWorld();
  GameWorld *gw = GameWorldSingleton::getInstance();

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
