#include "StaticDraw.h"

void StaticDraw::draw(Underground *ug, short slice, short width, short height)
{


	int x, y;
	Patch *topLeft = ug->getDisk()->getPatch(width, height);
	Patch *t;

//	Box(-3.5, 0, -7, 0.2, 1.5, 1);

	// Shift to the left a little so the screen will be centered.
	for(x = 0; x < UNDERGROUND_GRID_DRAW; x++)
		topLeft = topLeft->left;

	t = topLeft;

	// Draw the box 3 to the right and 3 down.
	y = UNDERGROUND_GRID_DRAW * -1;
	do {
		for (x = UNDERGROUND_GRID_DRAW * -1; t && x < UNDERGROUND_GRID_DRAW; x++)
		{
//			Box(x, y, -5,  0.4, 0.4, 1);
			Sphere(x, y, -5, 0.1);
			t = t->right;
		}
		if (topLeft)
		{
			topLeft = topLeft->bottom;
			t = topLeft;
		}
	} while (y++, t && y < UNDERGROUND_GRID_DRAW);


}


void StaticDraw::draw(Patch *p)
{

}

void StaticDraw::Sphere(float x, float y, float z, float radius)
{
	float lats = 6;
	float longs = lats;
//	v[(int)lats+1][(int)longs+1];

		int increment = 360/lats;
		int yinc = 180/longs;

		int xx=0;
		int yy=0;
    for (int latitude=-90; !cache && latitude<=90; latitude+=yinc) {
        double current_radius = cos(d2r(latitude)) * radius;
        double z = sin(d2r(latitude)) * radius;

        // Every 10 degrees of latitude, draw a longitude line.
        // Otherwise, draw a point every 10 degrees of longitude.
        for (int longitude=0; longitude<=360; longitude+=increment) {
                double x = cos(d2r(longitude))*current_radius;
                double y = sin(d2r(longitude))*current_radius;
                // (x,y,z) is a point in the wireframe
								v[xx][yy].x = x;
								v[xx][yy].y = y;
								v[xx][yy].z = z;

//								cout<<x<<" ";
						yy++;
        }
//				cout<<"\n";
				x++;
    }
	cache = true;

	int x1, y1;
	int upy, rightx;
	glBegin(GL_QUADS);
	for(x1=0; x1 < lats; x1++)
		for(y1=0; y1 < longs; y1++)
		{
//			upx = x1;
			//upy = (y1+1)%(int)lats;
			//rightx = (x1+1)%(int)longs;
			upy = y1+1;
			rightx = x1+1;
//			righty = y1;
//			uprightx = rightx;
//			uprighty = upy;
 
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

			glVertex3f( v[x1][y1].x+x, 			v[x1][y1].y+y, 			v[x1][y1].z+z				);
			glVertex3f( v[x1][upy].x+x, 		v[x1][upy].y+y, 		v[x1][upy].z+z			);
			glVertex3f( v[rightx][upy].x+x,	v[rightx][upy].y+y, v[rightx][upy].z+z	);
			glVertex3f( v[rightx][y1].x+x, 	v[rightx][y1].y+y, 	v[rightx][y1].z+z		);
		}
}

void StaticDraw::Box(float x, float y, float z, float width, float height, float depth)
{
	float w2 = width * 0.5;
	float h2 = height * 0.5;
	float d2 = depth * 0.5;

	glBegin(GL_QUADS);
	//z  face
	glColor3f(1,0,0);
	glVertex3f(x-w2 , y-h2 , z-d2 );
	glVertex3f(x + w2, y-h2 , z-d2 );
	glVertex3f(x + w2, y + h2, z-d2 );
	glVertex3f(x-w2 , y + h2, z-d2 );

	//z + depth face
	glColor3f(0,0,1);
	glVertex3f(x-w2 , y-h2 , z + d2);
	glVertex3f(x-w2 , y + h2, z + d2);
	glVertex3f(x + w2, y + h2, z + d2);
	glVertex3f(x + w2, y-h2 , z + d2);

	//x  face
	glColor3f(1,1,0);
	glVertex3f(x-w2 , y-h2 , z-d2 );
	glVertex3f(x-w2 , y + h2, z-d2 );
	glVertex3f(x-w2 , y + h2, z + d2);
	glVertex3f(x-w2 , y-h2 , z + d2);

	//x + width face
	glColor3f(1,1,0);
	glVertex3f(x + w2, y-h2 , z-d2 );
	glVertex3f(x + w2, y-h2 , z + d2);
	glVertex3f(x + w2, y + h2, z + d2);
	glVertex3f(x + w2, y + h2, z-d2 );

	//y  face
	glColor3f(1,1,0);
	glVertex3f(x - w2       , y-h2        , z-d2 );
	glVertex3f(x - w2       , y-h2 - 0.02 , z + d2);
	glVertex3f(x + w2 + 0.05, y-h2 - 0.02 , z + d2);
	glVertex3f(x + w2       , y-h2        , z-d2 );

	//y  + height face
	glColor3f(1,1,0);
	glVertex3f(x - w2       , y + h2       , z-d2 );
	glVertex3f(x + w2       , y + h2       , z-d2 );
	glVertex3f(x + w2 + 0.05, y + h2 + 0.03, z + d2);
	glVertex3f(x - w2       , y + h2 + 0.03, z + d2);

	glEnd();
}

