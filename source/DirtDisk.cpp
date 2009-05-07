#include "DirtDisk.h"

DirtDisk::DirtDisk()
{
	int x,y,z;
	for (x=0; x < SLICES; x++)
		for (y=0; y < WIDTH; y++)
			for (z=0; z < DEPTH; z++)
			{
//			if (z == DEPTH-1)
//				dd[x][y][z] = new Barrier();
//			else
					dd[x][y][z] = new Dirt(x,y,z);
			}
}
