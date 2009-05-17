#include "Camera.h"

Camera::Camera()
{
	//rotation = {0,0,0};
	//translation = {0,0,0};
	translation.x = 0;
	translation.y = 0;
	translation.z = 0;
	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
}

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
	gluPerspective(70, 256.0 / 192.0, 0.1, 20);
}

void Camera::Ortho()
{
	glOrtho(-4,4,-3,3,0.1,10);	
}

void Camera::move()
{
	glTranslatef( translation.x, translation.y, translation.z );
}

void Camera::rotate()
{
	glRotateX(rotation.x);
	glRotateY(rotation.y);
	glRotateZ(rotation.z);
}

void Camera::render()
{
	gluLookAt(translation.x, translation.y, translation.z, // Cam position.
						0.0, 0.0, 0.0,															 // Look at origin
						0.0, 1.0, 0.0);															 // Up
}

void Camera::render(float x, float y)
{
	gluLookAt(translation.x, translation.y, translation.z, // Cam position.
						x  , y  , 0.0,															 // Look at origin
						0.0, 1.0, 0.0);															 // Up
}

void Camera::update(int value)
{
	if (value == PLAYER_HELD_SHOULDER_R)
		translateZinc(0.5);
	else if (value == PLAYER_HELD_SHOULDER_L)
		translateZinc(-0.5);

}
