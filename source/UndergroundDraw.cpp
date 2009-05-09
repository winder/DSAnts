#include "UndergroundDraw.h"

void UndergroundDraw::draw()
{

	int x1, y1;
	int upy, rightx;

	glBegin(GL_QUADS);
	//for(x1=0; x1 < lats; x1++)
	for(x1=15; x1 < lats-15; x1++)  // this cuts off the top and bottom.
		//for(y1=15; y1 < longs-15; y1++)  // this makes it visor-like.
		for(y1=0; y1 < longs; y1++)
		{
			upy = (y1+1)%longs;
			rightx = x1+1;
 
			if (x1%3 == 0)
			glColor3f(1,0,0);
			else if (x1%3 == 1)
			glColor3f(1,1,0);
			else if (x1%3 == 2)
			glColor3f(0,1,1);
			else if (x1%3 == 3)
			glColor3f(1,0,1);
			else
			glColor3f(1,1,1);

			//glVertex3f( inner[rightx][y1].x, 	inner[rightx][y1].y, 	inner[rightx][y1].z		);
			//glVertex3f( inner[rightx][upy].x,	inner[rightx][upy].y, inner[rightx][upy].z	);
			//glVertex3f( inner[x1][upy].x, 		inner[x1][upy].y, 		inner[x1][upy].z			);
			//glVertex3f( inner[x1][y1].x, 			inner[x1][y1].y, 			inner[x1][y1].z				);
			glVertex3f( inner[x1][y1].x, 			inner[x1][y1].y, 			inner[x1][y1].z				);
			glVertex3f( inner[x1][upy].x, 		inner[x1][upy].y, 		inner[x1][upy].z			);
			glVertex3f( inner[rightx][upy].x,	inner[rightx][upy].y, inner[rightx][upy].z	);
			glVertex3f( inner[rightx][y1].x, 	inner[rightx][y1].y, 	inner[rightx][y1].z		);

			glVertex3f( outer[x1][y1].x, 			outer[x1][y1].y, 			outer[x1][y1].z				);
			glVertex3f( outer[x1][upy].x, 		outer[x1][upy].y, 		outer[x1][upy].z			);
			glVertex3f( outer[rightx][upy].x,	outer[rightx][upy].y, outer[rightx][upy].z	);
			glVertex3f( outer[rightx][y1].x, 	outer[rightx][y1].y, 	outer[rightx][y1].z		);
		}
		glEnd();
}

void UndergroundDraw::computeCircles()
{
//inner[x][y]
//outer[x][y]

		float innerR = 5;
		float outerR = 6;
		float increment = 360.0f/lats;
		float yinc = 180.0f/longs;

		int xx, yy;
		float latitude, longitude; 

    for (xx=0, latitude=-90; latitude<=90; xx++, latitude+=yinc) {
        double current_radius = cos(StaticDraw::d2r(latitude)); // * radius;
        double z = sin(StaticDraw::d2r(latitude)); // * radius;

        // Every 10 degrees of latitude, draw a longitude line.
        // Otherwise, draw a point every 10 degrees of longitude.
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

/*
		float innerR = 5;
		float outerR = 6;
		float lats = 50; // to increase this number must increase arrays "inner" and "outer"
		float longs = 14;

		// 90 degrees
		float increment = 360.0f/lats;
		// 90 degrees of calculations
		float yinc = 90.0f/longs;
		//int yinc = 180/longs;
		//int increment = 360/lats;

		int xx=0;
		int yy=0;
    for (float latitude=-45.0f; latitude<=45.0f; xx++, latitude+=yinc) {
        double current_radius = cos(StaticDraw::d2r(latitude));// * radius;
        double z = sin(StaticDraw::d2r(latitude));// * radius;

        // Every 10 degrees of latitude, draw a longitude line.
        // Otherwise, draw a point every 10 degrees of longitude.
        for (float longitude=0.0f; longitude<=360.0f; yy++, longitude+=increment) {
                double x = cos(StaticDraw::d2r(longitude));// *current_radius;
                double y = sin(StaticDraw::d2r(longitude));// *current_radius;
            // (x,y,z) is a point in the wireframe
						inner[xx][yy].x = x * current_radius * innerR;
						inner[xx][yy].y = y * current_radius * innerR;
						inner[xx][yy].z = z * innerR;

						outer[xx][yy].x = x * current_radius * outerR;
						outer[xx][yy].y = y * current_radius * outerR;
						outer[xx][yy].z = z * outerR;
        }
    }
*/
	cached = true;
}
