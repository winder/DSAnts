#ifndef QUEENANT_H
#define QUEENANT_H

#include "Ant.h"

class QueenAnt: public Ant
{
	public:
		QueenAnt():Ant(){};
		QueenAnt(Patch *pat, int location):Ant(pat, location){};
		virtual int getType(){ return ANT_QUEEN; }
		



};

#endif
