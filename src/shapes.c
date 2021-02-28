#include <stdlib.h>
#include "vector.h"
#include "shapes.h"

extern const int sphereNum, cubeNum;

const int sphereSize = 7;
const int cubeSize = 9;
const int cylinderSize = 10;
// group  = 2 shapes + 2 shapeTypes + operationType + k
const int groupSize = (cylinderSize * 2) + 2 + 1 + 1;

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

Cylinder* cyl(Vector3 start, Vector3 end, Vector3 clr, float r) {
	Cylinder* c = malloc(sizeof(Cylinder));
	c->start = start;
	c->end = end;
	c->clr = clr;
	c->radius = r;
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

void sphere2floats(float* f, Sphere* sphere) {
	f[0] = sphere->pos.x;
	f[1] = sphere->pos.y;
	f[2] = sphere->pos.z;

	f[3] = sphere->clr.x;
	f[4] = sphere->clr.y;
	f[5] = sphere->clr.z;

	f[6] = 0.0f;
	f[7] = 0.0f;
	f[8] = 0.0f;

	f[9] = sphere->radius;
}

void cube2floats(float* f, Cube* cube) {
	f[0] = cube->pos.x;
	f[1] = cube->pos.y;
	f[2] = cube->pos.z;

	f[3] = cube->clr.x;
	f[4] = cube->clr.y;
	f[5] = cube->clr.z;

	f[6] = cube->scale.x;
	f[7] = cube->scale.y;
	f[8] = cube->scale.z;

	f[9] = 0.0f;
}

void cyl2floats(float* f, Cylinder* cyl) {
	f[0] = cyl->start.x;
	f[1] = cyl->start.y;
	f[2] = cyl->start.z;

	f[3] = cyl->clr.x;
	f[4] = cyl->clr.y;
	f[5] = cyl->clr.z;

	f[6] = cyl->end.x;
	f[7] = cyl->end.y;
	f[8] = cyl->end.z;

	f[9] = cyl->radius;
}

void groups2floats(float* f, int num, ShapeGroup* grps) {
	for (int i = 0; i < num; i++) {
		if      (grps[i].a.type == CUBE)     {   cube2floats(&f[i * groupSize], (Cube*)    grps[i].a.shape); }
		else if (grps[i].a.type == SPHERE)   { sphere2floats(&f[i * groupSize], (Sphere*)  grps[i].a.shape); }
		else if (grps[i].a.type == CYLINDER) {    cyl2floats(&f[i * groupSize], (Cylinder*)grps[i].a.shape); }

		if      (grps[i].b.type == CUBE)     {   cube2floats(&f[i * groupSize + cylinderSize], (Cube*)    grps[i].b.shape); }
		else if (grps[i].b.type == SPHERE)   { sphere2floats(&f[i * groupSize + cylinderSize], (Sphere*)  grps[i].b.shape); }
		else if (grps[i].b.type == CYLINDER) {    cyl2floats(&f[i * groupSize + cylinderSize], (Cylinder*)grps[i].b.shape); }

		f[i * groupSize + cylinderSize * 2 + 0] = (float)grps[i].a.type;
		f[i * groupSize + cylinderSize * 2 + 1] = (float)grps[i].b.type;
		f[i * groupSize + cylinderSize * 2 + 2] = (float)grps[i].op;
		f[i * groupSize + cylinderSize * 2 + 3] = (float)grps[i].k;
	}
}
