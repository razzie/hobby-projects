#include <math.h>

float Cosine_Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5;
	return  a*(1-f) + b*f;
};
