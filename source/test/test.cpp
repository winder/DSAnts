#include <iostream>

#include "Underground.h"
#include "DirtDisk.h"
//#include "Patch.h"
//#include "Dirt.h"
//#include "Empty.h"
//#include "Barrier.h"

void printP(Patch *p)
{
	if (p == '\0')
		std::cout<<"NULL";
	else
//		std::cout<<"("<<p->getSlice()<<','<<p->getWidth()<<','<<p->getDepth()<<')';
		std::cout<<"good.";
}

int main()
{
	Underground *ud = new Underground();
	DirtDisk *dd = ud->getDisk();

	Patch *temp;
	Patch *temp2;
	int x,y,z;
	for (x=0; x < SLICES; x++)
		for (y=0; y < WIDTH; y++)
			for (z=0; z < DEPTH; z++)
			{
				temp = dd->getPatch(x,y,z);
				std::cout<<"\nPatch @ ("<<x<<','<<y<<','<<z<<")";
				temp2 = temp->right;
				std::cout<<"\n\tright: "; printP(temp2);
				temp2 = temp->right;
				std::cout<<"\n\tleft: "; printP(temp2);
				temp2 = temp->top;
				std::cout<<"\n\ttop: "; printP(temp2);
				temp2 = temp->bottom;
				std::cout<<"\n\tbottom: "; printP(temp2);
			}


	std::cout<<"\n";
	return 0;
}
