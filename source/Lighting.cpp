#include "Lighting.h"

Lighting::Lighting()
{
	// TODO: figure out how static member variables work.
//	light = lights;
//	Lighting::lights++;
	x=0.97;
	y=0.97;
	z=0;
	r=31;
	g=31;
	b=31;
	// need to set the light number.
	enabled = false;
}


void Lighting::move(float x, float y, float z)
{
	move( (v10)floattov10(x), (v10)floattov10(y), (v10)floattov10(z));
}

// bounds issue? -/+ 0.97 min/max??
void Lighting::move(v10 inx, v10 iny, v10 inz)
{
	x = inx;
	y = iny;
	z = inz;
}

void Lighting::set()
{
	if (enabled)
	  glLight(light, RGB15(r,g,b) , x, y, z); // setup the light
}

void Lighting::color(short inr, short ing, short inb)
{
	r = inr;
	g = ing;
	b = inb;
}

