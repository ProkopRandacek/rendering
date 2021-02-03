#include "vectormath.h"
#include "color.h"

typedef struct Spheres {
	Vector3 pos;
	Color color;
	float radius;
} Sphere;

Sphere sph(Vector3 pos, Color c, float r);
float d2Sphere(Vector3 v, Sphere s);
