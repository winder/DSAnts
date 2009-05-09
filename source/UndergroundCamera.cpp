#include "UndergroundCamera.h"

void UndergroundCamera::render()
{
	if (!cached) calcPosition();

	gluLookAt(camLoc.x, camLoc.y, camLoc.z, // Cam position.
						0.0, 0.0, 0.0,							 	// Look at origin
						0.0, 1.0, 0.0);							 	// Up
}


void UndergroundCamera::calcPosition()
{
	float latitude = lat;
	float longitude = track;

	// lock it to 0-360
	if (longitude < 0) longitude = 360 - fabs(longitude);
	else if (longitude > 360) longitude -= 360;

	float current_radius = cos(StaticDraw::d2r(latitude)) * radius;
	camLoc.y = sin(StaticDraw::d2r(latitude)) * radius;

	// for now, always along the equator.
	camLoc.x = cos(StaticDraw::d2r(longitude)) * current_radius;
	camLoc.z = sin(StaticDraw::d2r(longitude)) * current_radius;

	cached = true;
}
