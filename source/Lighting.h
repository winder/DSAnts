#ifndef LIGHTING_H
#define LIGHTING_H

#include <nds.h>

class Lighting
{
	public:
		Lighting();
		void enable(){ enabled = true; }
		void disable(){ enabled = false; }
		void set();
		void setLight(short l){ light = l; }
		void move(float x, float y, float z);
		void move(v10 x, v10 y, v10 z);
		void color(short r, short g, short b);
		
		
		uint32 getPolyFmtFlag()
		{
			if (!enabled) return 0;
			else if (light == 0) return POLY_FORMAT_LIGHT0;
			else if (light == 1) return POLY_FORMAT_LIGHT1;
			else if (light == 2) return POLY_FORMAT_LIGHT2;
			else if (light == 3) return POLY_FORMAT_LIGHT3;
			else return 0;
		}
	private:
		bool enabled;
		short light;
		// light direction
		v10 x, y, z;
		// light color
		short r,g,b;
		static short lights; // relying on this to be initialized to 0...

};

#endif
