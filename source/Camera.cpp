#include "Camera.h"

void Camera::init()
{
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);

	// initialize gl
	glInit();
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	// Set our view port to be the same size as the screen
	glViewport(0,0,255,191);
}

void Camera::Perspective()
{
	gluPerspective(70, 256.0 / 192.0, 0.1, 10);
}

void Camera::Ortho()
{
	glOrtho(-4,4,-3,3,0.1,10);	
}
