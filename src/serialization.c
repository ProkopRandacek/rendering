#include <stdint.h>
#include <stdio.h>

#include "camera.h"
#include "shapes.h"
#include "serialization.h"
#include "debug.h"
#include "opengl.h"

extern GL gl;
extern int groupSize, shapeSize;

void sphere2floats(float* f, Sphere* s) {
	f[0 ] = s->pos.x;  f[1 ] = s->pos.y; f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;  f[4 ] = s->clr.y; f[5 ] = s->clr.z;
	f[6 ] = 0.0f;      f[7 ] = 0.0f;     f[8 ] = 0.0f;
	f[9 ] = s->radius; f[10] = 0.0f;     f[11] = 0.0f;
	f[12] = 0.0f;      f[13] = 0.0f;     f[14] = 0.0f;
	f[15] = 0.0f;
}

void cube2floats(float* f, Cube* s) {
	f[0 ] = s->pos.x;     f[1 ] = s->pos.y;   f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;     f[4 ] = s->clr.y;   f[5 ] = s->clr.z;
	f[6 ] = s->scale.x;   f[7 ] = s->scale.y; f[8 ] = s->scale.z;
	f[9 ] = s->roundEdge; f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;         f[13] = 0.0f;       f[14] = 0.0f;
	f[15] = 0.0f;
}

void boxFrame2floats(float* f, BoxFrame* s) {
	f[0 ] = s->pos.x;   f[1 ] = s->pos.y;   f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;   f[4 ] = s->clr.y;   f[5 ] = s->clr.z;
	f[6 ] = s->scale.x; f[7 ] = s->scale.y; f[8 ] = s->scale.z;
	f[9 ] = s->width;   f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
	f[15] = 0.0f;
}

void torus2floats(float* f, Torus* s) {
	f[0 ] = s->pos.x;  f[1 ] = s->pos.y;   f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;  f[4 ] = s->clr.y;   f[5 ] = s->clr.z;
	f[6 ] = 0.0f;      f[7 ] = 0.0f;       f[8 ] = 0.0f;
	f[9 ] = s->radius; f[10] = s->fatness; f[11] = 0.0f;
	f[12] = 0.0f;      f[13] = 0.0f;       f[14] = 0.0f;
	f[15] = 0.0f;
}

void ctorus2floats(float* f, CTorus* s) {
	f[0 ] = s->pos.x; f[1 ] = s->pos.y; f[2 ] = s->pos.z;
	f[3 ] = s->clr.x; f[4 ] = s->clr.y; f[5 ] = s->clr.z;
	f[6 ] = 0.0f;     f[7 ] = 0.0f;     f[8 ] = 0.0f;
	f[9 ] = s->scX;   f[10] = s->scY;   f[11] = s->ra;
	f[12] = s->rb;    f[13] = 0.0f;     f[14] = 0.0f;
	f[15] = 0.0f;
}

void link2floats(float* f, Link* s) {
	f[0 ] = s->pos.x;  f[1 ] = s->pos.y;  f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;  f[4 ] = s->clr.y;  f[5 ] = s->clr.z;
	f[6 ] = 0.0f;      f[7 ] = 0.0f;      f[8 ] = 0.0f;
	f[9 ] = s->innerR; f[10] = s->outerR; f[11] = s->length;
	f[12] = 0.0f;      f[13] = 0.0f;      f[14] = 0.0f;
	f[15] = 0.0f;
}

void plane2floats(float* f, Plane* s) {
	f[0 ] = s->pos.x; f[1 ] = s->pos.y; f[2 ] = s->pos.z;
	f[3 ] = s->clr.x; f[4 ] = s->clr.y; f[5 ] = s->clr.z;
	f[6 ] = s->n.x;   f[7 ] = s->n.y;   f[8 ] = s->n.z;
	f[9 ] = s->h;     f[10] = 0.0f;     f[11] = 0.0f;
	f[12] = 0.0f;     f[13] = 0.0f;     f[14] = 0.0f;
	f[15] = 0.0f;
}

void hexprism2floats(float* f, HexPrism* s) {
	f[0 ] = s->pos.x;  f[1 ] = s->pos.y;  f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;  f[4 ] = s->clr.y;  f[5 ] = s->clr.z;
	f[6 ] = 0.0f;      f[7 ] = 0.0f;      f[8 ] = 0.0f;
	f[9 ] = s->height; f[10] = s->radius; f[11] = 0.0f;
	f[12] = 0.0f;      f[13] = 0.0f;      f[14] = 0.0f;
	f[15] = 0.0f;
}

void triprism2floats(float* f, TriPrism* s) {
	f[0 ] = s->pos.x;  f[1 ] = s->pos.y;  f[2 ] = s->pos.z;
	f[3 ] = s->clr.x;  f[4 ] = s->clr.y;  f[5 ] = s->clr.z;
	f[6 ] = 0.0f;      f[7 ] = 0.0f;      f[8 ] = 0.0f;
	f[9 ] = s->height; f[10] = s->radius; f[11] = 0.0f;
	f[12] = 0.0f;      f[13] = 0.0f;      f[14] = 0.0f;
	f[15] = 0.0f;
}

void caps2floats(float* f, Capsule* s) {
	f[0 ] = s->start.x; f[1 ] = s->start.y; f[2 ] = s->start.z;
	f[3 ] = s->clr.x;   f[4 ] = s->clr.y;   f[5 ] = s->clr.z;
	f[6 ] = s->end.x;   f[7 ] = s->end.y;   f[8 ] = s->end.z;
	f[9 ] = s->radius;  f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
	f[15] = 0.0f;
}

void cyl2floats(float* f, Cylinder* s) {
	f[0 ] = s->start.x; f[1 ] = s->start.y; f[2 ] = s->start.z;
	f[3 ] = s->clr.x;   f[4 ] = s->clr.y;   f[5 ] = s->clr.z;
	f[6 ] = s->end.x;   f[7 ] = s->end.y;   f[8 ] = s->end.z;
	f[9 ] = s->radius;  f[10] = 0.0f;       f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
	f[15] = 0.0f;
}

void ccone2floats(float* f, CCone*s) {
	f[0 ] = s->start.x; f[1 ] = s->start.y; f[2 ] = s->start.z;
	f[3 ] = s->clr.x;   f[4 ] = s->clr.y;   f[5 ] = s->clr.z;
	f[6 ] = s->end.x;   f[7 ] = s->end.y;   f[8 ] = s->end.z;
	f[9 ] = s->startR;  f[10] = s->endR;    f[11] = 0.0f;
	f[12] = 0.0f;       f[13] = 0.0f;       f[14] = 0.0f;
	f[15] = 0.0f;
}


void groups2floats(float* f, int num, ShapeGroup* groups) {
	for (int i = 0; i < num; i++) {
		f[i * groupSize + 0] = (float) groups[i].ta;
		f[i * groupSize + 0] = (float) groups[i].a;
		f[i * groupSize + 1] = (float) groups[i].tb;
		f[i * groupSize + 0] = (float) groups[i].a;
		f[i * groupSize + 2] = (float) groups[i].op;
		f[i * groupSize + 3] = (float) groups[i].k;
	}
}

void shapes2floats(float *f, int num, Primitive* prmv) {
	for (int i = 0; i < num; i++) {
		if      (prmv[i].type == CUBE)     {   cube2floats(&f[i * shapeSize],     (Cube*) prmv[i].shape); }
		else if (prmv[i].type == SPHERE)   { sphere2floats(&f[i * shapeSize],   (Sphere*) prmv[i].shape); }
		else if (prmv[i].type == CYLINDER) {    cyl2floats(&f[i * shapeSize], (Cylinder*) prmv[i].shape); }
		else if (prmv[i].type == TORUS)    {  torus2floats(&f[i * shapeSize],    (Torus*) prmv[i].shape); }
		else if (prmv[i].type == CCONE)    {  ccone2floats(&f[i * shapeSize],    (CCone*) prmv[i].shape); }
		else {
			char errMsg[55];
			sprintf(errMsg, "Unknown shape %d on index %d\nexiting\n", prmv[i].type, num);
			eprint(errMsg);
		}
	}
}

void cam2floats(Camera cam, float* f) {
	f[0]  = cam.pos.x; f[1]  = cam.pos.y; f[2] = cam.pos.z;
	f[3]  = cam.tr.x;  f[4]  = cam.tr.y;  f[5] = cam.tr.z;
	f[6]  = cam.tl.x;  f[7]  = cam.tl.y;  f[8] = cam.tl.z;
	f[9]  = cam.br.x;  f[10] = cam.br.y; f[11] = cam.br.z;
	f[12] = cam.bl.x;  f[13] = cam.bl.y; f[14] = cam.bl.z;
}
