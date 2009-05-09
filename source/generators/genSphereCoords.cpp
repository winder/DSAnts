
#include <iostream>
#include <math.h>
using namespace std;

#define UNDERGROUND_GRID_DRAW 16

typedef struct
{ float x,y,z;
} Verticie;

#define PIE 3.14

float degreeToRadian(float degree)
{
//	cout<<"PIE/180 "<< PIE/180.0<<"\n";
//	cout<<" * degree = "<< (PIE/180.0)*degree;
	return (PIE / 180.0) * (float)degree;
}

void genPoints(double r, int lats, int longs)
{
    int i, j;
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);
  
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
    				cout<<x * zr0<<"\t";
						// x= x*zr0
						// y= y*zr0
						// z= zr0
        }
				cout<<"\n";
    }
}

// This generates the entire sphere, I want to get a subset of this though.

// x = p cos(theta) sin(phi)
// y = p sin(theta) sin(phi)
// z = p cos(phi)
// theta = longitude
// phi = colatitude (90 degrees - latitude)
// p = radius

double phi, theta;

double d2r(double degrees) {
        const double conversion = 3.1416f/180.0f;
        return degrees * conversion;

}

void Sphere(double radius, int longs, int lats) {
		int increment = 360/lats;
		int yinc = 180/longs;

//    for (int latitude=-90; latitude<90; latitude++) {
    for (int latitude=-90; latitude<90; latitude+=yinc) {
        double current_radius = cos(d2r(latitude)) * radius;
        double z = sin(d2r(latitude)) * radius;

        // Every 10 degrees of latitude, draw a longitude line.
        // Otherwise, draw a point every 10 degrees of longitude.
//        int increment = latitude % 10 ? 10 : 1;

        for (int longitude=0; longitude<360; longitude+=increment) {
                double x = cos(d2r(longitude))*current_radius;
                double y = sin(d2r(longitude))*current_radius;
                // (x,y,z) is a point in the wireframe
								cout<<x<<" ";
        }
				cout<<"\n";
    }

}

void genPointsAngle(float r, int lats, int longs, float angle)
{
//		float angleR = degreeToRadian(angle);
//cout<<"angleR, longs: "<<angleR<<" "<<longs;

//		float deltaLong = angleR / (float)longs;
//		float deltaLat = angleR / (float)lats;
		float deltaLong = angle / (float)longs;
		float deltaLat = angle / (float)lats;

		int x,y;
		float l, lat;
		// Longitude
    for(y=0, l = 0; y <= longs; y++, l+= deltaLong) {

				theta = l;
				// Latitude
        for(x=0, lat = 0; x <= lats; x++, lat+= deltaLat) {
					phi = fabs(90 - lat);

					cout << r * cos(theta) * sin(phi)<< "\t";
//					cout << r * sin(theta) * sin(phi)<<"\t";
//					cout << r * cos(degreeToRadian(phi)) <<"\t";
//					cout<<"("<<l<<","<<lat<<")\t";
        }
				cout<<"\n";
    }
}

int main()
{
	int numBoxes = UNDERGROUND_GRID_DRAW * UNDERGROUND_GRID_DRAW;
	int numVerticies = (UNDERGROUND_GRID_DRAW + 1) * (UNDERGROUND_GRID_DRAW + 1);

	Verticie v[UNDERGROUND_GRID_DRAW+1][UNDERGROUND_GRID_DRAW+1];

	// In degrees I want of the sphere
	int viewAngle = 40;
	float deltaAngle = viewAngle / UNDERGROUND_GRID_DRAW;

	// Flat-Coordinates of grid.
	int leftX = -1 * (UNDERGROUND_GRID_DRAW/2);
	int topY = leftX * -1;


	int rightX = -1 * leftX;
	int bottomY = -1 * topY;

	int x,y;

	genPoints(1, 4, 4);
cout<<"\n---------------\n";
	Sphere(1, 4, 4);
//	genPointsAngle(1, 4, 4, 360);
/*
	for (x= 0; x < UNDERGROUND_GRID_DRAW; x++)
	{
		double lat = PIE * ( x * deltaAngle );
		double z = sin(x);
		double zr = cos(x);

		for (y= 0; y <= UNDERGROUND_GRID_DRAW; y++)
		{
			double lng = 2 * PIE * (y * deltaAngle);
			double zl = sin(lng);
			double zrl = cos(lng);

			v[x][y].x = x * z;
			v[x][y].y = y * z;
			v[x][y].z = z;
			cout << "("<<v[x][y].x<<", "<<v[x][y].y<<", "<<v[x][y].z<<") ";
		}
	cout<<"\n";
	}
*/
/*
	int xx, yy;
	for (xx=0, x= leftX; x < rightX; x++, xx++)
	{
		double lat = PIE * (x * deltaAngle );
		double z = sin(x);
		double zr = cos(x);

		for (yy=0, y= topY; y > bottomY; yy++, y++)
		{
			double lng = 2 * PIE * (y * deltaAngle);
			double zl = sin(lng);
			double zrl = cos(lng);

			v[x][y].x = x * z;
			v[x][y].y = y * z;
			v[x][y].z = z;
//			cout << "("<<v[xx][yy].x<<", "<<v[xx][yy].y<<", "<<v[xx][yy].z<<") ";
			cout<<v[xx][yy].x<<" ";
		}
	cout<<"\n";
	}
*/
	
	return 0;
}

