#include <stdlib.h>

#include "vector.h"
#include "shapes.h"


const int shapeSize = 15; // max number of floats per shape
// group  = 2 shapes + 2 shapeTypes + operationType + k
const int groupSize = (shapeSize * 2) + 2 + 1 + 1;

Sphere* sph(Vector3 pos, Vector3 clr, float radius) {
	Sphere* s = malloc(sizeof(Sphere));
	s->pos = pos;
	s->clr = clr;
	s->radius = radius;
	return s;
}

Cube* cube(Vector3 pos, Vector3 clr, Vector3 scale, float roundEdge) {
	Cube* c = malloc(sizeof(Cube));
	c->pos = pos;
	c->clr = clr;
	c->scale = scale;
	c->roundEdge = roundEdge;
	return c;
}

BoxFrame* frame(Vector3 pos, Vector3 clr, Vector3 scale, float width) {
	BoxFrame* f = malloc(sizeof(BoxFrame));
	f->pos = pos;
	f->clr = clr;
	f->scale = scale;
	f->width = width;
	return f;
}

Torus* tor(Vector3 pos, Vector3 clr, float innerR, float outerR) {
	Torus* t = malloc(sizeof(Torus));
	t->pos = pos;
	t->clr = clr;
	t->innerR = innerR;
	t->outerR = outerR;
	return t;
}

CTorus* ctor(Vector3 pos, Vector3 clr, float scX, float scY, float ra, float rb) {
	CTorus* t = malloc(sizeof(CTorus));
	t->pos = pos;
	t->clr = clr;
	t->scX = scX;
	t->scY = scY;
	t->ra = ra;
	t->rb = rb;
	return t;
}

Link* link(Vector3 pos, Vector3 clr, float innerR, float outerR, float length) {
	Link* l = malloc(sizeof(Link));
	l->pos = pos;
	l->clr = clr;
	l->innerR = innerR;
	l->outerR = outerR;
	l->length = length;
	return l;
}

Plane* pln(Vector3 pos, Vector3 clr, Vector3 n, float h) {
	Plane* p = malloc(sizeof(Plane));
	p->pos = pos;
	p->clr = clr;
	p->n = n;
	p->h = h;
	return p;
}

HexPrism* xprism(Vector3 pos, Vector3 clr, float h) {
	HexPrism* x = malloc(sizeof(HexPrism));
	x->pos = pos;
	x->clr = clr;
	x->height = h;
	return x;
}

TriPrism* tprism(Vector3 pos, Vector3 clr, float h) {
	TriPrism* t = malloc(sizeof(TriPrism));
	t->pos = pos;
	t->clr = clr;
	t->height = h;
	return t;
}

Capsule* caps(Vector3 start, Vector3 end, Vector3 clr, float r) {
	Capsule* c = malloc(sizeof(Capsule));
	c->start = start;
	c->end = end;
	c->clr = clr;
	c->radius = r;
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

RoundCone* rcone(Vector3 start, Vector3 end, Vector3 clr, float startR, float endR) {
	RoundCone* r = malloc(sizeof(RoundCone));
	r->start = start;
	r->end = end;
	r->clr = clr;
	r->startR = startR;
	r->endR = endR;
	return r;
}

Pyramid* pyr(Vector3 pos, Vector3 clr, float h) {
	Pyramid* r = malloc(sizeof(Pyramid));
	r->pos = pos;
	r->clr = clr;
	r->h = h;
	return r;
}

Triangle* tri(Vector3 a, Vector3 b, Vector3 c, Vector3 clr) {
	Triangle* t = malloc(sizeof(Triangle));
	t->a = a;
	t->b = b;
	t->c = c;
	t->clr = clr;
	return t;
}

Quad* quad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 clr) {
	Quad* q = malloc(sizeof(Quad));
	q->a = a;
	q->b = b;
	q->c = c;
	q->d = d;
	q->clr = clr;
	return q;
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
