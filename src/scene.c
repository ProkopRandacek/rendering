#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opengl.h"
#include "scene.h"
#include "shapeSerialization.h"
#include "time.h"


extern GL* gl;

extern const int groupSize;

// these are global so other object can be created relative to them
Camera cam;

void createScene() {
	createCamera();
	sendCamera();
	createLight();
	createObjects();
}

void createCamera() {
	Vector3 campos = v3(0.0f, 5.0f, -10.0f);
	cam = cmr(campos, vNorm(vDir(campos, v3(0.0f, 5.0f, 0.0f))), 0.0f, 0.01f, 0.01f);
}

void sendCamera() {
	float camFloats[15];
	cam2floats(cam, camFloats);

	shdSetVec3Array(gl->s, "cam", 5 * 3, camFloats);
}

void createLight() {
	float lsPos[] = {5.0f, 5.0f, 5.0f};

	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void createObjects() {
	const int groupNum = 3;
	ShapeGroup groups[groupNum];

	// primitive shapes
	Primitive head = prmv(SPHERE, (void*) sph  (v3(0.5f, 2.1f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.3f));
	Primitive ring = prmv(TORUS,  (void*) tor  (v3(0.5f, 1.9f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.25f, 0.09f));
	Primitive body = prmv(CCONE,  (void*) ccone(v3(0.5f, 2.2f, 0.5f), v3(0.5f, 1.3f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.17f, 0.3f));
	Primitive base = prmv(CCONE,  (void*) ccone(v3(0.5f, 1.3f, 0.5f), v3(0.5f, 1.0f, 0.5f), v3(0.9f, 0.0f, 0.9f), 0.4f, 0.4f));

	ShapeGroup sgTop = group(head, ring, NORMAL, 0.5f);
	Primitive    Top = prmv(GROUP, (void*) (intptr_t) 0);

	ShapeGroup sgBottom = group(body, base, BLEND, 0.2f);
	Primitive    Bottom = prmv(GROUP, (void*) (intptr_t) 1);

	ShapeGroup sgROOT = group(Top, Bottom, BLEND, 0.02f);

	groups[0] = sgTop;
	groups[1] = sgBottom;
	groups[2] = sgROOT;

	float f[groupSize * groupNum];
	groups2floats(f, groupNum, groups);

	shdSetFloatArray(gl->s, "rawGroups", groupSize * groupNum, f);

	free(head.shape);
	free(ring.shape);
	free(body.shape);
}

void updateScene() {
	// recreate all object that are supposed to be moving
	sendCamera();
	createLight();
}
