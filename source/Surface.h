#ifndef SURFACE_H
#define SURFACE_H

#include "MapDraw.h"

class Surface: public MapDraw
{
	public:
		// The surface is pretty much empty.
		// TODO: remove X wrapping or add Y wrapping for the surface.
		Surface(){ initGrid(); }
		virtual ~Surface(){ /* superclass deletes structure */ }

		void initGrid();


};

#endif
