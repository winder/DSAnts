#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant: public Creature
{
  public:
    Ant():Creature()
    {
      feramoneOutput = 100;
      currentPheramone = BLACK_HOME;
      location_home = -1;
      portalHome = false;
    }
    Ant(Patch *pat, int location):Creature(pat, location)
    {
      feramoneOutput = 100;
      currentPheramone = BLACK_HOME;
      location_home = -1;
      portalHome = false;
    }
    virtual int getType() = 0;
    virtual void handlePheramone();
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

//    virtual void wentThroughPortal()
//      { savedEntrance = getPatch(); }

    virtual void forage();
    // Attempts to follow a chemical trail
    // returns true/false if there is a trail to follow
    virtual bool followTrail(bool h);
    // because the sorted patch surrounding the ant may have
    // been calculated already, a version of the function will accept
    // that array.
    virtual bool followTrail(Patch* sort[], int direction[], bool h);
    virtual void goHome();
    virtual void goHomeCheating();

    virtual void setHome(int h){ location_home = h; }
    virtual int getHome(){ return location_home; }

    void setFeramoneOutput(int f){ feramoneOutput = f; }
    unsigned int getFeramoneOutput(){ return feramoneOutput; }

    void setPheramone(int p){ currentPheramone = p; }
    int getPheramone(){ return currentPheramone; }

    // return as argument.
    void sortAdjacentPatchByChem(Patch* center, Patch* sort[], int direction[]);

    bool wayToHome(int &x_dist, int &y_dist, bool &right, bool &up);
    bool awayFromHome(int dir);

    // assumes they are sorted by chemical level.
    // returns the index for the array.
    int followHotNotVisited(Patch* choices[])
      {
        for(int i=0; i<4; i++)
          if (choices[i] != '\0')
            if (!checkVisited(choices[i]))
              return i;

        return -1;
      }

  private:
    unsigned int feramoneOutput;
    unsigned short currentPheramone;
    int location_home;

    // take portal to get home, or for unknown portals?
    // if true, will only take a portal if it goes to the home base.
    bool portalHome;

    // the entrance you come out of is cached, for AI reference point.
//    Patch* savedEntrance;
};

#endif
