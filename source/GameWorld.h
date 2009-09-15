#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Singleton.h"
// typedef after class definition
//typedef Singleton<GameWorld> GameWorldSingleton; // global declaration
#include "Underground.h"
#include "Surface.h"
#include <vector>
#include "Ant.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "Observer.h"

// Different types of ants.
#include "WorkerAnt.h"
#include "QueenAnt.h"

// stdlib for rand / srand
#include <stdlib.h>

//#ifdef __DEBUG_
#include <stdio.h>
//#endif
#ifdef __PROFILING
  #include "cyg-profile.h"
#endif

// TODO: this class would make more sense as a "GameSquare" if I decide to go
//       the original SimAnt route and have multiple colonies spanning a grid.
// TODO: this class should be broken apart from all the camera and other crap
//       I stuck in here, those should go somewhere else and use this class to
//       get needed info.

// This class is designed to hold all the pieces of the world together.
// Major pieces include:
//      Underground / Surface world information.
//      Creature (black/red ants, spiders, pill bugs, etc) information.
//      Object (Food, eggs, rocks, etc) information.
class GameWorld: public Observer, public Subject
{
  public:
    GameWorld();
    ~GameWorld();

     // Modifies the patch, picks up the object and returns it.
     int pickup(int loc, Patch* pat);
    bool drop(int loc, Patch* pat, int Ob);

     MapDraw* getMap(int st)
     {  
       if (STATE == GAMEWORLD_STATE_UNDERGROUND)
         return ug;
       else if (STATE == GAMEWORLD_STATE_SURFACE)
         return surf;
       //else if (STATE == GAMEWORLD_STATE_ENEMY_UNDERGROUND)
       //  return eug;
      else
        return '\0';
     }

    // Link the surface with the underground.
    void linkSurfaceAndUnderground();

    void changeState(int st){ STATE = st; }

    // tell classes to draw themselves
    void draw();
    void pickPoint(short x, short y);

    // getters
    MapDraw* getUG(){ return ug; }
    MapDraw* getSurface(){ return surf; }

    // Coordinates a step in the game.
    void stepForward(int num);
    void stepEggsForward();
    // lets each ant know they can move forward one step.
    void stepAntsForward(int num);

    // Creates an ant based on population ratio settings.
    void createAnt( Patch* pat, int location );

    // Camera needs to set itself up.
    void initCam(){  cam->init();
                  cam->translateZinc(2.0); }
    void initTex()
      {//  ug->setTextures(tex);
        // surf->setTextures(tex); }
        ug->initTextures();
        surf->setTextures( ug->getTextures() ); }

    void placeCamera(){ cam->render(); }
    void setProjection();

    // Event handling: Observer implementation:
    virtual void update(int value);
    
    // #ifdef __DEBUG_
    void printDebugFiveLines();
    // #endif
  private:
    unsigned int numAnts;
    unsigned short eggTimer;

    // Player, Player Input, Input class, picking, touch-movement
    // store the picked patch.
    Player *p;
    bool doPick;
    Patch *picked;
    bool automove;
    Input *in;

    // GAMEWORLD_STATE_SURFACE, GAMEWORLD_STATE_UNDERGROUND
    short STATE;
    MapDraw* curMap;
    MapDraw* ug;
    MapDraw* surf;

    // in most cases this will be true, but there may be cases where
    // I want to follow something else (another ant, spider, etc).
    bool followingPlayer;
    Creature *following;
    // Camera
    Camera *cam;

    std::vector<Ant*> black;
    std::vector<Ant*> red;
};

typedef Singleton<GameWorld> GameWorldSingleton; // global declaration

#endif
