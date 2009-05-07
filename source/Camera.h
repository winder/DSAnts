#ifndef CAMERA_H
#define CAMERA_H

#include <nds.h>
#include "global.h"

class Camera
{
	public:
		Camera();

		// Setup wrappers.
		void init();
		void Perspective();
		void Ortho();

		void move();
		void rotate();

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

	private:
		VectorF translation;
		VectorF rotation;
};

#endif
