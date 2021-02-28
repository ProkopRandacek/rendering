#include <stdlib.h>
#include "vector.h"
#include "shapes.h"

extern const int sphereNum, cubeNum;

const int sphereSize = 7;
const int cubeSize = 9;
// group  = 2 shapes + 2 shapeTypes + operationType + k
const int groupSize = (cubeSize * 2) + 2 + 1 + 1;

Sphere* sph(Vector3 pos, Vector3 clr, float radius) {
	Sphere* s = malloc(sizeof(Sphere));
	s->pos = pos;
	s->clr = clr;
	s->radius = radius;
	return s;
}

Cube* cube(Vector3 pos, Vector3 clr, Vector3 scale) {
	Cube* c = malloc(sizeof(Cube));
	c->pos = pos;
	c->clr = clr;
	c->scale = scale;
	return c;
}

Primitive prmv(ShapeType type, void* shape) {
	Primitive p;
	p.type = type;
	p.shape = shape;
	return p;
}

ShapeGroup group(Primitive a, Primitive b, OperationType op, float k) {
	ShapeGroup sg;
	sg.a = a;
	sg.b = b;
	sg.op = op;
	sg.k = k;
	return sg;
}

void spheres2floats(float* f, int num, Sphere* spheres) {
	for (int i = 0; i < num; i++) {
		f[i * sphereSize + 0] = spheres[i].pos.x;
		f[i * sphereSize + 1] = spheres[i].pos.y;
		f[i * sphereSize + 2] = spheres[i].pos.z;

		f[i * sphereSize + 3] = spheres[i].clr.x;
		f[i * sphereSize + 4] = spheres[i].clr.y;
		f[i * sphereSize + 5] = spheres[i].clr.z;

		f[i * sphereSize + 6] = spheres[i].radius;
		f[i * sphereSize + 6] = spheres[i].radius;
		f[i * sphereSize + 6] = spheres[i].radius;
	}
}

void cubes2floats(float* f, int num, Cube* cubes) {
	for (int i = 0; i < num; i++) {
		f[i * cubeSize + 0] = cubes[i].pos.x;
		f[i * cubeSize + 1] = cubes[i].pos.y;
		f[i * cubeSize + 2] = cubes[i].pos.z;

		f[i * cubeSize + 3] = cubes[i].clr.x;
		f[i * cubeSize + 4] = cubes[i].clr.y;
		f[i * cubeSize + 5] = cubes[i].clr.z;

		f[i * cubeSize + 6] = cubes[i].scale.x;
		f[i * cubeSize + 7] = cubes[i].scale.y;
		f[i * cubeSize + 8] = cubes[i].scale.z;
	}
}

void groups2floats(float* f, int num, ShapeGroup* grps) {
	for (int i = 0; i < num; i++) {
		if      (grps[i].a.type == CUBE)   {   cubes2floats(&f[i * groupSize + 0], 1, (Cube*)  grps[i].a.shape); }
		else if (grps[i].a.type == SPHERE) { spheres2floats(&f[i * groupSize + 0], 1, (Sphere*)grps[i].a.shape); }

		if      (grps[i].b.type == CUBE)   {   cubes2floats(&f[i * groupSize + cubeSize], 1, (Cube*)  grps[i].b.shape); }
		else if (grps[i].b.type == SPHERE) { spheres2floats(&f[i * groupSize + cubeSize], 1, (Sphere*)grps[i].b.shape); }

		f[i * groupSize + cubeSize * 2 + 0] = (float)grps[i].a.type;
		f[i * groupSize + cubeSize * 2 + 1] = (float)grps[i].b.type;
		f[i * groupSize + cubeSize * 2 + 2] = (float)grps[i].op;
		f[i * groupSize + cubeSize * 2 + 3] = (float)grps[i].k;
	}
}
