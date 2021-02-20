#include "vector.h"
#include "shapes.h"

extern const int sphereNum, cubeNum;

const int floatPerSph = 7;
const int floatPerCube = 9;

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

void spheres2floats(float* f, int num, Sphere* spheres) {
	for (int i = 0; i < num; i++) {
		f[i * floatPerSph + 0] = spheres[i].pos.x;
		f[i * floatPerSph + 1] = spheres[i].pos.y;
		f[i * floatPerSph + 2] = spheres[i].pos.z;

		f[i * floatPerSph + 3] = spheres[i].clr.x;
		f[i * floatPerSph + 4] = spheres[i].clr.y;
		f[i * floatPerSph + 5] = spheres[i].clr.z;

		f[i * floatPerSph + 6] = spheres[i].radius;
	}
}

void cubes2floats(float* f, int num, Cube* cubes) {
	for (int i = 0; i < num; i++) {
		f[i * floatPerCube + 0] = cubes[i].pos.x;
		f[i * floatPerCube + 1] = cubes[i].pos.y;
		f[i * floatPerCube + 2] = cubes[i].pos.z;

		f[i * floatPerCube + 3] = cubes[i].clr.x;
		f[i * floatPerCube + 4] = cubes[i].clr.y;
		f[i * floatPerCube + 5] = cubes[i].clr.z;

		f[i * floatPerCube + 6] = cubes[i].scale.x;
		f[i * floatPerCube + 7] = cubes[i].scale.y;
		f[i * floatPerCube + 8] = cubes[i].scale.z;
	}
}
