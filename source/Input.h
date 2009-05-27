#ifndef INPUT_H
#define INPUT_H

#include <nds.h>
#include "Observer.h"
#include "global.h"
//#include "Camera.h"

class Input: public Subject
{
  public:
    int getHeld();
    int getPressed();
    void process();
//    void processInput();
//    void processTouch();

    int getTouchX(){ return touchXY.px; }
    int getTouchY(){ return touchXY.py; }
//    int getTouchDeltaX(){ return deltax; }
//    int getTouchDeltaY(){ return deltay; }

  private:
    bool newInput;
    bool touchReleaseSent;

    // current touch x/y
    touchPosition touchXY;
    int held;
    int pressed;

    // spot where first touched the screen
    //int oldx, oldy;
    // if dragging cursor, this is the difference between old and new;
    //int deltax, deltay;
};


#endif
