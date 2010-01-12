#include "GameWorld.h"

GameWorld::GameWorld()
{
  antIndex = 0;
  numBlack = numRed = 0;
  black = (Ant*) malloc ( sizeof(Ant) * 1000 );
  red = (Ant*) malloc ( sizeof(Ant) * 1000 );

  eggTimer = 0;

  in = new Input();

  cam = new Camera();
  //cam->translateZinc(2.2);

  ug = new MapDraw();
  ug->setGrid( new Underground( GAMEWORLD_STATE_UNDERGROUND ) );
  ug->getGrid()->setLoopX();

  eug = new MapDraw();
  eug->setGrid( new Underground( GAMEWORLD_STATE_UNDERGROUND_ENEMY ));
  eug->getGrid()->setLoopX();

  surf = new MapDraw();
  surf->setGrid( new Surface() );

  surf->getGrid()->setLoopY();
  surf->getGrid()->setLoopX();

  // start underground
  STATE = GAMEWORLD_STATE_UNDERGROUND;
  curMap = ug;

  // init picking variables
  picked = '\0';
  doPick = false;
  
  // Create player.
  Ant *tmp = new QueenAnt();
  tmp->setPatch( ug->getGrid()->getPatch(0,2) );
  tmp->setLocation( STATE );
  tmp->setCarrying(PATCH_FOOD1);  
  tmp->setTakePortals( true );
  tmp->setFeramoneOutput(1000);
  tmp->setPheramone(BLACK_FOOD);
  tmp->setSpeed(20);

  // Default to following player:
  followingPlayer = true;
  p = new Player(tmp);

  following = tmp;


  // Create queen(s) and put in a random location.
  tmp = new (&black[numBlack++]) QueenAnt();
  tmp->setPatch( ug->getGrid()->getRandomCleared() );
  tmp->setLocation( STATE );
  tmp->setTakePortals( false );
  // Queen doesn't need an action, she just sits around making eggs
//  black.push_back( tmp );

  // Enemy queen
  tmp = new (&black[numBlack++]) QueenAnt();
  tmp->setPatch( eug->getGrid()->getRandomCleared() );
  tmp->setLocation( GAMEWORLD_STATE_UNDERGROUND_ENEMY );
  tmp->setTakePortals( false );
  // Queen doesn't need an action, she just sits around making eggs
//  black.push_back( tmp );

  // SETUP OBSERVERs:

  // Observing Player: GameWorld.
  p->attach(this);

  // Observing GameWorld: Player.
  attach(p);

  // Observing Input: Player, GameWorld.
  in->attach(p);
  in->attach(this);
  in->attach(cam);

  // This loops through The surface of the underground (both when I add the enemy underground)
  // and links them to the surface.
  linkSurfaceAndUnderground();
}

GameWorld::~GameWorld()
{
  delete ug;
  delete eug;
  delete surf;
  delete p;
  delete in;
  delete cam;

  // loop through "black" and "red" and delete 'em.
  //for (unsigned int i=0; i < black.size(); i++)
//  for (unsigned int i=0; i < numBlack; i++)
//    delete black[i];
//  for (unsigned int i=0; i < numBlack; i++)
//    delete red[i];

  free(black);
  free(red);
}

int GameWorld::pickup(int loc, Patch *pat)
{
  // If it is not something we can interact with, do not pick it up.
  if (OBJECT(pat))
  {
    // Cache the patch's type, and remove it.
    int savet = pat->TYPE;
    getMap(loc)->getGrid()->takeObject( pat );

    // Special handling for food Objects, if it is a stack of food objects,
    // mark the patch as food - 1.
    // TODO: can I assume the enums are declared incrementally?
    if (FOODi(savet))
    {
      switch (savet)
      {
        case PATCH_FOOD1 :
          // Ant is holding 1 food, Patch is empty.  Correct.
          break;

        case PATCH_FOOD2 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD1 );
          break;

        case PATCH_FOOD3 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD2 );
          break;
  
        case PATCH_FOOD4 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD3 );
          break;

        case PATCH_FOOD5 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD4 );
          break;

        case PATCH_FOOD6 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD5 );
          break;

        case PATCH_FOOD7 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD6 );
          break;

        case PATCH_FOOD8 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD7 );
          break;

        case PATCH_FOOD9 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD8 );
          break;

        case PATCH_FOOD10 :
          getMap(loc)->getGrid()->addObject( pat , PATCH_FOOD9 );
          break;

        default:
          // should never get here.  do nothing.
          break;
      }
      // if something picked up food, it only picks up one at a time.
      return PATCH_FOOD1;
    }

    // Object doesn't need special handling.
    else
      return savet;
  }
  // otherwise, nothing.
  return NOTHING;
}

bool GameWorld::drop(int loc, Patch* pat, int Ob)
{
  // Dropping food onto food.
  if (FOODi(Ob))
  {
    switch (pat->TYPE)
    {
      case PATCH_EMPTY :
        pat->TYPE = PATCH_FOOD1;
        break;

      case PATCH_FOOD1 :
        pat->TYPE = PATCH_FOOD2;
        break;

      case PATCH_FOOD2 :
        pat->TYPE = PATCH_FOOD3;
        break;

      case PATCH_FOOD3 :
        pat->TYPE = PATCH_FOOD4;
        break;

      case PATCH_FOOD4 :
        pat->TYPE = PATCH_FOOD5;
        break;

      case PATCH_FOOD5 :
        pat->TYPE = PATCH_FOOD6;
        break;

      case PATCH_FOOD6 :
        pat->TYPE = PATCH_FOOD7;
        break;

      case PATCH_FOOD7 :
        pat->TYPE = PATCH_FOOD8;
        break;

      case PATCH_FOOD8 :
        pat->TYPE = PATCH_FOOD9;
        break;

      case PATCH_FOOD9 :
        pat->TYPE = PATCH_FOOD10;
        break;

      // only case that should hit this is PATCH_FOOD10
      default:
        return false;
    }
    return true;
  }
  // Dropping anything with no special handling..
  else if( EMPTY(pat) )
  {
    pat->TYPE = Ob;
    return true;
  }

  return false;
}


void GameWorld::linkSurfaceAndUnderground()
{
  int randX, randY;
  // loop across the top layer of the underground, link to random areas of the surface.
  // ug is the black ants, in the left side of the map.
  Patch* topleft = ug->getGrid()->getPatch(0,1);
  do
  {
    // find random locations until we get one that hasn't already been picked.
    do
    {
      randX = rand() % (WIDTH / 2);
      randY = rand() % DEPTH;
      topleft->portal = surf->getGrid()->getPatch(randX, randY);
    }while (!EMPTY(topleft->portal));

    topleft->portal->TYPE = PATCH_ENTRANCE;

    // make it two-way.
    // NOTE: the surface points to just below the top level of the underground
    topleft->portal->portal = topleft;
    // move right
    topleft = Grid::getRight( topleft );
    // until we loop all the way around.
  } while(topleft->x != 0);

  // TODO: Make this generic so I don't copy/paste for the enemy underground
  topleft = eug->getGrid()->getPatch(0,1);
  do
  {
    // find random locations until we get one that hasn't already been picked.
    do
    {
      // This is really the only different part,
      // make it on the right half of the map.
      randX = (rand() % (WIDTH / 2)) + (WIDTH / 2);
      randY = rand() % DEPTH;
      topleft->portal = surf->getGrid()->getPatch(randX, randY);
    }while (!EMPTY(topleft->portal));

    topleft->portal->TYPE = PATCH_ENTRANCE;

    // make it two-way.
    // NOTE: the surface points to just below the top level of the underground
    topleft->portal->portal = topleft;
    // move right
    topleft = Grid::getRight( topleft );
    // until we loop all the way around.
  } while(topleft->x != 0);

}

void GameWorld::draw()
{
  numAnts = 0;
  // if player is offscreen, center screen.
  if (! curMap->isVisible( following->getX(), following->getY() ) )
    curMap->setCenter( following->getX(), following->getY() );

  curMap->doMapShift();
  curMap->beginQuads();
  // Draw game field.
  curMap->draw();

  // draw the ants      
  //for (unsigned int i=0; (numAnts < 80) && (i < black.size()); i++)
  for (int i=0; (numAnts < 80) && (i < numBlack); i++)
    if ( black[i].getLocation() == STATE )
      if (curMap->drawAnt(&black[i], true))
        numAnts++;

//    for (unsigned int i=0; i < red.size(); i++)
//      if ( red[i]->getLocation() == GAMEWORLD_STATE_UNDERGROUND )
//        ug->drawAnt(red[i]);

  curMap->drawAnt(p->getPlayerAnt(), true);

  // DO THE PICKING
  // If the touch pad is being touched... see what its touching.
  if (doPick)
  {
    pickPoint(in->getTouchX(), in->getTouchY());
    doPick = false;
  }

  curMap->end();
}
void GameWorld::setPicked( Patch* p)
{
    picked = p;
}

void GameWorld::pickPoint(short x, short y)
{
  if (curMap->pickPoint(x, y, *cam))
  {
    setPicked( curMap->getPicked() );
    // Disabling automove for now.
    //p->setDestination(picked->x, picked->y);
    //if (WALKABLE(picked))
    //  automove = true;
    //else
    //  automove = false;

    // update intself
    update(PLAYER_PICKED_SOMETHING);
  }
}

void GameWorld::stepAntsForward(int num)
{
  // test, move each ant around randomly.
  //for (unsigned int i=0; i < black.size(); i++)
  int i;
  for (i=antIndex; (i < numBlack) && ((i-antIndex) < ANT_STEPS_PER_FRAME); i++)
  {
    // AI for ant "black[i]->AImove()"
    black[i].stateStep(num *  numBlack / ANT_STEPS_PER_FRAME);
    black[i].move(num * numBlack / ANT_STEPS_PER_FRAME);
  }

  if (i >= numBlack) antIndex = 0;
  else antIndex += ANT_STEPS_PER_FRAME;
}

void GameWorld::stepEggsForward(MapDraw* md)
{
  // Counter for eggs:
  eggTimer++;
  if (eggTimer >= 500)
  {
    eggTimer = 0;
    for (unsigned int i=0; i < md->getGrid()->getCleared().size(); i++)
    {
      if (EGG(md->getGrid()->getCleared()[i]))
      {
        switch(md->getGrid()->getCleared()[i]->TYPE)
        {
          case PATCH_EGG1:
            md->getGrid()->getCleared()[i]->TYPE = PATCH_EGG2;
            break;
          case PATCH_EGG2:
            md->getGrid()->getCleared()[i]->TYPE = PATCH_EGG3;
            break;
          case PATCH_EGG3:
            md->getGrid()->getCleared()[i]->TYPE = PATCH_EGG4;
            break;
          case PATCH_EGG4:
            md->getGrid()->getCleared()[i]->TYPE = PATCH_EGG5;
            break;
          case PATCH_EGG5:
            md->getGrid()->getCleared()[i]->TYPE = PATCH_EMPTY;
            createAnt( md->getGrid()->getCleared()[i], md->getGrid()->getLocation() );
            break;
        }
      }
    }
  }
}

// no draw here, it is handled elsewhere so that things will be able
// to move forward if things start to lag.
void GameWorld::stepForward(int num)
{
  // The "Player" doesn't keep track of its location, so I can't do this through observer.
  if (STATE != following->getLocation())
  {
    STATE = following->getLocation();
    curMap = getMap(STATE);
  }

  // update the maps.
  ug->gameTick(num);
  eug->gameTick(num);
  surf->gameTick(num);

  // Shift the map to center the player (normal circumstances)
    for (int i = num; i > 0; i--)
    {
      curMap->shiftCenter(following);
      stepEggsForward(ug);
      stepEggsForward(eug);
    }

  // process user input.
  in->process();

  // send everyone on their way.
  stepAntsForward(num);
  p->stepForward(num);
}

// TODO: population ratio's (i.e. 50% workers, 30% breeders, 20% soldiers)
// TODO: createRedAnt / createBlackAnt ?
void GameWorld::createAnt( Patch* pat, int location )
{
    Ant *t = new (&black[numBlack++]) WorkerAnt( pat, location);
    t->setAction( ANT_ACTION_WANDER );
    // set home to the location:
    t->setHome( location );

    t->setSpeed( 20 );

    // find which to use based on location
//    black.push_back(t);
}

void GameWorld::setProjection()
{
  //change ortho vs perspective
//  if(held & KEY_B)
//    cam->Ortho();
//  else 
    cam->Perspective();
}

void GameWorld::update(int value)
{
  // TODO: do follow cursor on hold?

  // do picking on PRESS
  if (value == PLAYER_PRESSED_TOUCHPAD)
    doPick = true;
  else if (value == PLAYER_PICKED_SOMETHING)
  {
    // If it is dirt, see if we can DIG IT.
    if (picked->TYPE == PATCH_DIRT)
      curMap->getGrid()->clear(p->dig());
    // If your carrying something, drop.  Note: the spot doesn't always need to be empty.
    else if ( p->getPlayerAnt()->getCarrying() )
    {
      p->drop();
    }
    // if it is an object, see if we can PICK IT.
    else if (OBJECT(picked))
    {
      p->pickUp();
    }
  }
  else if (value == PLAYER_RELEASED_TOUCHPAD)
  {
    if (picked)
      picked->picked = false;
  }


  // Hold A to spawn ants,
  else if(value == PLAYER_HELD_B)
  {
    // add a new ant on press.
    Ant *t = new (&black[numBlack++]) WorkerAnt(ug->getGrid()->getPatch(0,2), GAMEWORLD_STATE_UNDERGROUND);
    t->setHome( GAMEWORLD_STATE_UNDERGROUND );
    t->setTakePortals(true);
//    t->setAction( ANT_ACTION_WANDER );
    t->setAction( ANT_ACTION_FORAGE );
    t->setTarget( p->getPlayerAnt() );
//    t->setAction( ANT_ACTION_FOLLOW );
    t->setSpeed( 20 );
//    black.push_back(t);

    // Can change following to any creature at any time...
    // following = t;
  }

  else if (value == PLAYER_HELD_A)
  {
    Ant *t = new (&black[numBlack++]) QueenAnt(ug->getGrid()->getPatch(0,2), GAMEWORLD_STATE_UNDERGROUND);
    t->setAction( ANT_ACTION_WANDER );
//    black.push_back(t);
  }
  else if (value == PLAYER_PRESSED_X)
  {
    #ifdef __PROFILING
      cygprofile_end();
    #endif
  }
}

//#ifdef __DEBUG
void GameWorld::printDebugFiveLines()
{
  //printf("Ant=%d, WorkerAnt=%d, QueenAnt=%d, int=%d", sizeof(Ant), sizeof(WorkerAnt), sizeof(QueenAnt), sizeof(int));
  // Interesting stats:
  //    Number of ants
  //    Map / Map coordinate
  //    Spot touched
  //    Player info
  //    player automove?
//  printf("\nWORLD, Ants: black(%i), red(%i)", black.size(), red.size());
  if (STATE == GAMEWORLD_STATE_UNDERGROUND)
    printf("\nCurrent map: underground");
  else if (STATE == GAMEWORLD_STATE_SURFACE)
    printf("\nCurrent map: surface");
  else if (STATE == GAMEWORLD_STATE_UNDERGROUND_ENEMY)
    printf("\nCurrent map: enemy underground");
  else
    printf("\nCurrent map: unknown");
//  printf("\nMap Stats: <UG> <Surf>");
//  printf("\n  Cleared: %5d %5d", ug->getGrid()->numCleared(), surf->getGrid()->numCleared());
//  printf("\n  Objects: %5d %5d", ug->getGrid()->numObjects(), surf->getGrid()->numObjects());
//  printf("\nMap Center: (%i, %i)", curMap->getCenterX(), curMap->getCenterY());

//  printf("\nAnts: drawn(%i)\n black(%i), red(%i)", numAnts, black.size(), red.size());
  printf("\nAnts: drawn(%i)\n black(%i), red(%i)", numAnts, numBlack, numRed);

//  printf("\nCamera distance: %f", cam->getCamLocation().z);
//  printf("\nTouch coord: (%i, %i)", curX, curY);
  p->printDebug();

  if (picked)
  {
    printf("\nPatch: level(%i)", picked->chemLevel[0]);
    printf("\nPatch bh(%i) bf(%i)\n\t rh(%i) rf(%i)",
        GET_PHERAMONE(picked, BLACK_HOME), GET_PHERAMONE(picked, BLACK_FOOD),
        GET_PHERAMONE(picked, RED_HOME), GET_PHERAMONE(picked, RED_FOOD));

/*
    printf("\n Occupants: ");
    if (picked->occupant_one)
      printf("one(X) ");
    else
      printf("one( ) ");

    if (picked->occupant_two)
      printf("two(X) ");
    else
      printf("two( ) ");

    printf("\n portal: ");
    if (picked->portal)
      printf("X");


    if (picked->occupant_one)
    {
      printf("\n one: \n");
      picked->occupant_one->printDebug();
    }
    if (picked->occupant_two)
    {
      printf("\n two: \n");
      picked->occupant_two->printDebug();
    }
*/
  }
}
//#endif
