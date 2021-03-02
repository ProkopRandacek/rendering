#include <stdint.h>

#include "camera.h"
#include "shapes.h"
#include "shapeSerialization.h"


extern int groupSize, shapeSize;

void sphere2floats(float* f, Sphere* sphere) {
	f[0] = sphere->pos.x; f[1] = sphere->pos.y; f[2] = sphere->pos.z;
	f[3] = sphere->clr.x; f[4] = sphere->clr.y; f[5] = sphere->clr.z;
	f[6] = 0.0f;          f[7] = 0.0f;          f[8] = 0.0f;
	f[9] = sphere->radius;
}

void cube2floats(float* f, Cube* cube) {
	f[0] = cube->pos.x;   f[1] = cube->pos.y;   f[2] = cube->pos.z;
	f[3] = cube->clr.x;   f[4] = cube->clr.y;   f[5] = cube->clr.z;
	f[6] = cube->scale.x; f[7] = cube->scale.y; f[8] = cube->scale.z;
	f[9] = cube->roundEdge;
}

void cyl2floats(float* f, Cylinder* cyl) {
	f[0] = cyl->start.x; f[1] = cyl->start.y; f[2] = cyl->start.z;
	f[3] = cyl->clr.x;   f[4] = cyl->clr.y;   f[5] = cyl->clr.z;
	f[6] = cyl->end.x;   f[7] = cyl->end.y;   f[8] = cyl->end.z;
	f[9] = cyl->radius;
}

void groups2floats(float* f, int num, ShapeGroup* groups) {
	for (int i = 0; i < num; i++) {
		if      (groups[i].a.type == CUBE)     {   cube2floats(&f[i * groupSize],     (Cube*) groups[i].a.shape); }
		else if (groups[i].a.type == SPHERE)   { sphere2floats(&f[i * groupSize],   (Sphere*) groups[i].a.shape); }
		else if (groups[i].a.type == CYLINDER) {    cyl2floats(&f[i * groupSize], (Cylinder*) groups[i].a.shape); }
		else if (groups[i].a.type == GROUP) {
			f[i * groupSize + shapeSize * 0 + 1] = (float) (intptr_t) groups[i].a.shape; // a "pointer" to where this node is
		}

		if      (groups[i].b.type == CUBE)     {   cube2floats(&f[i * groupSize + shapeSize],     (Cube*) groups[i].b.shape); }
		else if (groups[i].b.type == SPHERE)   { sphere2floats(&f[i * groupSize + shapeSize],   (Sphere*) groups[i].b.shape); }
		else if (groups[i].b.type == CYLINDER) {    cyl2floats(&f[i * groupSize + shapeSize], (Cylinder*) groups[i].b.shape); }
		else if (groups[i].b.type == GROUP) {
			f[i * groupSize + shapeSize * 1 + 1] = (float) (intptr_t) groups[i].b.shape; // a "pointer" to where this node is
		}

		f[i * groupSize + shapeSize * 2 + 0] = (float) groups[i].a.type;
		f[i * groupSize + shapeSize * 2 + 1] = (float) groups[i].b.type;
		f[i * groupSize + shapeSize * 2 + 2] = (float) groups[i].op;
		f[i * groupSize + shapeSize * 2 + 3] = (float) groups[i].k;
	}
}

void cam2floats(Camera cam, float* f) {
	f[0]  = cam.pos.x; f[1]  = cam.pos.y; f[2] = cam.pos.z;
	f[3]  = cam.tr.x;  f[4]  = cam.tr.y;  f[5] = cam.tr.z;
	f[6]  = cam.tl.x;  f[7]  = cam.tl.y;  f[8] = cam.tl.z;
	f[9]  = cam.br.x;  f[10] = cam.br.y; f[11] = cam.br.z;
	f[12] = cam.bl.x;  f[13] = cam.bl.y; f[14] = cam.bl.z;
}
