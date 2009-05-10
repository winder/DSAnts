#include "UndergroundDraw.h"

void UndergroundDraw::draw()
{

	int left = centerX - GRID_SIZE;
	int right = centerX + GRID_SIZE;
	int top = centerY - GRID_SIZE;
	int bottom = centerY + GRID_SIZE;
	int temp=0;


	//if (left < 0) left+=lats;
	//if (top < 0) top+=longs;
	left = 90;
	top = 20;

	Patch* topLeft;
	Patch* center;
	Patch* tp;

	// Eventually this call will be made to whatever generic character the
	// map should be centered on.  i.e. the player, or an enemy ant.
	topLeft = center = underground->getPatch(slice, centerX, centerY);
	
	// Shift from the center to the topLeft.
	// shift left a bunch of times.
	for (temp=0; temp > left; temp--)
		topLeft = topLeft->getLeft();

	// shift up a bunch of times or until we hit the surface.
	for (temp=0; (topLeft->getTop()) && (temp < top); temp++)
		topLeft = topLeft->getTop();

	tp=topLeft;


	int xindex;
	int yindex;
	int x, y, upy, rightx;
	glBegin(GL_QUADS);
	// This needs to be made to work with the wrapped coordinates.
	// i.e. x=40+++++ x=10
	//for(x=15; x < lats-15; x+=2)  // this cuts off the top and bottom.
	for(x=0, xindex=left; x < ((GRID_SIZE*2)+1); x++, xindex++)
	{
		for(y=0, yindex=top; y < ((GRID_SIZE*2)+1); y++, yindex++)
		{
			if (xindex==lats) xindex=0;
			if (yindex==longs) yindex=0;

			upy = (yindex+1)%longs;
			rightx = (xindex+1)%lats;
						
		
			drawSegment(xindex, yindex, rightx, upy, tp);
			tp=tp->getRight();				
		}
		if (topLeft->getBottom())
			topLeft=topLeft->getBottom();
		tp=topLeft;
	}

	glEnd();
}

void UndergroundDraw::drawSegment(int y, int x, int upy, int rightx, Patch* p)
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

			glColor3f(p->getR(), p->getG(), p->getB());

			 // front face.
			glVertex3f( inner[x][y].x, 			inner[x][y].y, 			inner[x][y].z							);
			glVertex3f( inner[x][upy].x, 		inner[x][upy].y, 		inner[x][upy].z						);
			glVertex3f( inner[rightx][upy].x,	inner[rightx][upy].y, inner[rightx][upy].z	);
			glVertex3f( inner[rightx][y].x, 	inner[rightx][y].y, 	inner[rightx][y].z		);

			// rear face.
			glVertex3f( outer[x][y].x, 			outer[x][y].y, 			outer[x][y].z							);
			glVertex3f( outer[x][upy].x, 		outer[x][upy].y, 		outer[x][upy].z						);
			glVertex3f( outer[rightx][upy].x,	outer[rightx][upy].y, outer[rightx][upy].z	);
			glVertex3f( outer[rightx][y].x, 	outer[rightx][y].y, 	outer[rightx][y].z		);

			// x face.
			glVertex3f( inner[x][y].x, 		inner[x][y].y, 		inner[x][y].z		);
			glVertex3f( inner[x][upy].x, 	inner[x][upy].y, 	inner[x][upy].z	);
			glVertex3f( outer[x][upy].x,	outer[x][upy].y,	outer[x][upy].z	);
			glVertex3f( outer[x][y].x, 		outer[x][y].y, 		outer[x][y].z		);

			// x+ face.
			glVertex3f( inner[rightx][y].x, 	inner[rightx][y].y, 	inner[rightx][y].z		);
			glVertex3f( outer[rightx][y].x,		outer[rightx][y].y, 	outer[rightx][y].z		);
			glVertex3f( outer[rightx][upy].x,	outer[rightx][upy].y,	outer[rightx][upy].z	);
			glVertex3f( inner[rightx][upy].x, inner[rightx][upy].y, inner[rightx][upy].z	);

			// y face.
			glVertex3f( inner[x][y].x, 			inner[x][y].y, 			inner[x][y].z				);
			glVertex3f( outer[x][y].x,			outer[x][y].y, 			outer[x][y].z				);
			glVertex3f( outer[rightx][y].x,	outer[rightx][y].y,	outer[rightx][y].z	);
			glVertex3f( inner[rightx][y].x, inner[rightx][y].y, inner[rightx][y].z	);

			// y+ face.
			glVertex3f( inner[x][upy].x, 			inner[x][upy].y, 			inner[x][upy].z				);
			glVertex3f( inner[rightx][upy].x,	inner[rightx][upy].y, inner[rightx][upy].z	);
			glVertex3f( outer[rightx][upy].x,	outer[rightx][upy].y,	outer[rightx][upy].z	);
			glVertex3f( outer[x][upy].x, 			outer[x][upy].y, 			outer[x][upy].z				);
}


// This function could probably utilize the 8-way symmetry that circles are said to have.
void UndergroundDraw::computeCircles()
{
		float increment = 360.0f/lats;
		float yinc = 180.0f/longs;

		int xx, yy;
		float latitude, longitude; 

		// swapping the x,y,z = x,y,z to z,x,y = x,y,z turned the sphere 90 degrees
		// which is how I wanted it.
    for (xx=0, latitude=-90; latitude<=90; xx++, latitude+=yinc) {
        double current_radius = cos(StaticDraw::d2r(latitude));
        double z = sin(StaticDraw::d2r(latitude));

        for (yy=0, longitude=0; longitude<=360; yy++, longitude+=increment) {
                double x = cos(StaticDraw::d2r(longitude));
                double y = sin(StaticDraw::d2r(longitude));
                // (x,y,z) is a point in the wireframe
								inner[xx][yy].z = x * current_radius * innerR;
								inner[xx][yy].x = y * current_radius * innerR;
								inner[xx][yy].y = z * innerR;

								outer[xx][yy].z = x * current_radius * outerR;
								outer[xx][yy].x = y * current_radius * outerR;
								outer[xx][yy].y = z * outerR;
        }
    }
/* // This works perfectly, but the north and south poles run across the x axis.
    for (xx=0, latitude=-90; latitude<=90; xx++, latitude+=yinc) {
        double current_radius = cos(StaticDraw::d2r(latitude));
        double z = sin(StaticDraw::d2r(latitude));

        for (yy=0, longitude=0; longitude<=360; yy++, longitude+=increment) {
                double x = cos(StaticDraw::d2r(longitude));
                double y = sin(StaticDraw::d2r(longitude));
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
