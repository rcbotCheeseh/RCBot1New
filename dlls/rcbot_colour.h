#ifndef __RCBOT_COLOUR_H__
#define __RCBOT_COLOUR_H__

#include <stdint.h>

#define BEAM_ALPHA 200

class Colour
{
public:
	Colour()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	Colour(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = BEAM_ALPHA)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	void mix(const Colour &other)
	{
		float fr = (r - other.r) * 0.5;
		float fg = (g - other.g) * 0.5;
		float fb = (b - other.b) * 0.5;

		if (fr < 0)
			fr = 0;
		if (fg < 0)
			fg = 0;
		if (fb < 0)
			fb = 0;

		r = (uint8_t)((int)((float)other.r * 0.5)) + (uint8_t)((int)fr);
		g = (uint8_t)((int)((float)other.g * 0.5)) + (uint8_t)((int)fg);
		b = (uint8_t)((int)((float)other.b * 0.5)) + (uint8_t)((int)fb);

	}

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

#endif 
