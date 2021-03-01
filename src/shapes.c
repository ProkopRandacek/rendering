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
