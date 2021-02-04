#include "vectormath.h"
#include "color.h"

typedef struct Spheres {
	Vector3 pos;
	Color color;
	float radius;
	float refw; // 0.00 .. 1.00; 0 -> The sphere color doesn't affect the reflection at all. 1 -> The reflection is not visible at all. Only the sphere color
} Sphere;

Sphere sph(Vector3 pos, Color c, float r, float w);
float d2Sphere(Vector3 v, Sphere s);
