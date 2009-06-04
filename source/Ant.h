#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant: public Creature
{
  public:
    Ant():Creature()
    {
      feramoneOutput = 100;
      location_home = -1;
      portalHome = false;
    }
    Ant(Patch *pat, int location):Creature(pat, location)
    {
      feramoneOutput = 100;
      location_home = -1;
      portalHome = false;
    }
    virtual int getType() = 0;
    virtual void handleFeramone();
    virtual bool handlePortal()
    {
      // will go through any portal, use super class.
      // or if not on surface
      if (!portalHome || (getLocation() != GAMEWORLD_STATE_SURFACE))
        return Creature::handlePortal();
      // if the portal goes home, try to take it.
      else if (getPatch()->portal &&
              (getPatch()->portal->location == location_home))
        return Creature::handlePortal();
      // otherwise, don't take it, and don't try again.
      else
        return false;
    }

    virtual void wentThroughPortal()
      { savedEntrance = getPatch(); }

    virtual void forage();
    virtual void goHome();
    virtual void goHomeCheating();
    // check adjacent Patches for food.
    Patch* checkForFood();


    virtual void setHome(int h){ location_home = h; }
    virtual int getHome(){ return location_home; }

    // return as argument.
    void sortAdjacentPatchByChem(Patch* center, Patch* sort[], int direction[]);

  private:
    short feramoneOutput;
    int location_home;

    // take portal to get home, or for unknown portals?
    // if true, will only take a portal if it goes to the home base.
    bool portalHome;

    // the entrance you come out of is cached, for AI reference point.
    Patch* savedEntrance;
};

#endif
