#include "shapes.h"

Sphere sph(Vector3 pos, Vector3 clr, float radius) {
	Sphere s;
	s.pos = pos;
	s.clr = clr;
	s.radius = radius;
	return s;
}

Cube cube(Vector3 pos, Vector3 clr, Vector3 scale) {
	Cube c;
	c.pos = pos;
	c.clr = clr;
	c.scale = scale;
	return c;
}

/*float* spheres2floats(int num, Sphere* spheres) {

}*/
