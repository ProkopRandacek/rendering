#include "shapes.h"

Sphere sph(Vector3 pos, Color c, float r) {
	Sphere s;
	s.pos = pos;
	s.color = c;
	s.radius = r;
	return s;
}

float d2Sphere(Vector3 v, Sphere s) {
	return vDist(v, s.pos) - s.radius;
}
