#ifndef CAMERA_H
#define CAMERA_H

#include <nds.h>
#include "Observer.h"
#include "global.h"

class Camera: public Observer
{
  public:
    Camera();

    // Setup wrappers.
    virtual void init();
    virtual void Perspective();
    virtual void Ortho();

    void move();
    void rotate();

    // future...
    // void set coord (x,y,z)
    // void set look at (x,y,z)

    // Move camera variables, inc.
    void translateXinc(float x){ translation.x += x; }
    void translateYinc(float y){ translation.y += y; }
    void translateZinc(float z){ translation.z += z; }

    void rotateXinc(float x){ rotation.x += x; }
    void rotateYinc(float y){ rotation.y += y; }
    void rotateZinc(float z){ rotation.z += z; }

    // Move camera variables, set.
    void translateXset(float x){ translation.x += x; }
    void translateYset(float y){ translation.y += y; }
    void translateZset(float z){ translation.z += z; }

    void rotateXset(float x){ rotation.x += x; }
    void rotateYset(float y){ rotation.y += y; }
    void rotateZset(float z){ rotation.z += z; }

    void render();
    void render(float x, float y);
    VectorF getCamLocation(){ return translation; }

    void update(int value);
  private:
    VectorF location;
    VectorF lookAt;

    VectorF translation;
    VectorF rotation;
};

#endif
