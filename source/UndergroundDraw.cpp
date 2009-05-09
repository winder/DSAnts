#include "UndergroundDraw.h"

void UndergroundDraw::draw()
{

	int x, y, upy, rightx;
//	Patch *topLeft = ug->getDisk()->getPatch(slice, width, height);
//	Patch *t;

	glBegin(GL_QUADS);
	//for(x1=0; x1 < lats; x1++)
	for(x=15; x < lats-15; x++)  // this cuts off the top and bottom.
		//for(y1=15; y1 < longs-15; y1++)  // this makes it visor-like.
		for(y=0; y < longs; y++)
		{

			upy = (y+1)%longs;
			rightx = x+1;
			
			drawSegment(x, y, rightx, upy);
		}
		glEnd();
//	Box(-3.5, 0, -7, 0.2, 1.5, 1);
/*
	// Shift to the left a little so the screen will be centered.
	for(x = 0; x < UNDERGROUND_GRID_DRAW; x++)
		topLeft = topLeft->getLeft();

	t = topLeft;

	// Draw the box 3 to the right and 3 down.
	y = UNDERGROUND_GRID_DRAW * -1;
	do {
		for (x = UNDERGROUND_GRID_DRAW * -1; t && x < UNDERGROUND_GRID_DRAW; x++)
		{
//			Box(x, y, -5,  0.4, 0.4, 1);
			Sphere(x, y, -5, 0.1);
			t = t->getRight();
		}
		if (topLeft)
		{
			topLeft = topLeft->getBottom();
			t = topLeft;
		}
	} while (y++, t && y < UNDERGROUND_GRID_DRAW);
*/
}

void UndergroundDraw::drawSegment(int x, int y, int rightx, int upy)
{
			if (x%3 == 0)
			glColor3f(1,0,0);
			else if (x%3 == 1)
			glColor3f(1,1,0);
			else if (x%3 == 2)
			glColor3f(0,1,1);
			else if (x%3 == 3)
			glColor3f(1,0,1);
			else
			glColor3f(1,1,1);

			//glVertex3f( inner[rightx][y1].x, 	inner[rightx][y1].y, 	inner[rightx][y1].z		);
			//glVertex3f( inner[rightx][upy].x,	inner[rightx][upy].y, inner[rightx][upy].z	);
			//glVertex3f( inner[x1][upy].x, 		inner[x1][upy].y, 		inner[x1][upy].z			);
			//glVertex3f( inner[x1][y1].x, 			inner[x1][y1].y, 			inner[x1][y1].z				);
	
			// front face.
			glVertex3f( inner[x][y].x, 			inner[x][y].y, 			inner[x][y].z				);
			glVertex3f( inner[x][upy].x, 		inner[x][upy].y, 		inner[x][upy].z			);
			glVertex3f( inner[rightx][upy].x,	inner[rightx][upy].y, inner[rightx][upy].z	);
			glVertex3f( inner[rightx][y].x, 	inner[rightx][y].y, 	inner[rightx][y].z		);

			// rear face.
			glVertex3f( outer[x][y].x, 			outer[x][y].y, 			outer[x][y].z				);
			glVertex3f( outer[x][upy].x, 		outer[x][upy].y, 		outer[x][upy].z			);
			glVertex3f( outer[rightx][upy].x,	outer[rightx][upy].y, outer[rightx][upy].z	);
			glVertex3f( outer[rightx][y].x, 	outer[rightx][y].y, 	outer[rightx][y].z		);
}
void UndergroundDraw::computeCircles()
{
		float innerR = 5;
		float outerR = 6;
		float increment = 360.0f/lats;
		float yinc = 180.0f/longs;

		int xx, yy;
		float latitude, longitude; 

    for (xx=0, latitude=-90; latitude<=90; xx++, latitude+=yinc) {
        double current_radius = cos(StaticDraw::d2r(latitude)); // * radius;
        double z = sin(StaticDraw::d2r(latitude)); // * radius;

        for (yy=0, longitude=0; longitude<=360; yy++, longitude+=increment) {
                double x = cos(StaticDraw::d2r(longitude)); // *current_radius;
                double y = sin(StaticDraw::d2r(longitude)); // *current_radius;
                // (x,y,z) is a point in the wireframe
								inner[xx][yy].x = x * current_radius * innerR;
								inner[xx][yy].y = y * current_radius * innerR;
								inner[xx][yy].z = z * innerR;

								outer[xx][yy].x = x * current_radius * outerR;
								outer[xx][yy].y = y * current_radius * outerR;
								outer[xx][yy].z = z * outerR;
        }
    }
		cached = true;
}
