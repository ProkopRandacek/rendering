#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opengl.h"
#include "scene.h"

extern GL* gl;
extern const int floatPerSph, floatPerCube;

const int sphereNum = 2;
const int cubeNum = 4;

extern const int sphereSize, cubeSize, groupSize;

// these are global so other object can be created relative to them
Camera cam;

void createScene() {
	createCamera(0.0f);
	sendCamera();
	createLight(0.0f);
	createObjects();
}

void createCamera(float time) {
	Vector3 campos = v3(0.0f, 5.0f, -10.0f);
	cam = cmr(campos, vNorm(vDir(campos, v3(0.0f, 5.0f, 0.0f))), 0.0f, 0.01f, 0.01f);
}

void sendCamera() {
	float camFloats[15];
	cam2floats(cam, camFloats);

	shdSetVec3Array(gl->s, "cam", 5 * 3, camFloats);
}

void createLight(float time) {
	float lsPos[] = { cam.pos.x, cam.pos.y, cam.pos.z };

	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void createObjects() {
	Primitive greenSph = prmv(SPHERE, (void*)sph(v3(3.0f, 2.5f, 0.0f), v3(0.0f, 1.0f, 0.0f), 1.0f));
	Primitive blueSph  = prmv(SPHERE, (void*)sph(v3(0.0f, 2.5f, 0.0f), v3(0.0f, 0.0f, 1.0f), 1.5f));

	ShapeGroup sphsGroup = group(greenSph, blueSph, BLENDING, 2.0f);

	Primitive greenCube = prmv(CUBE, (void*)cube(v3( 0.0f, 2.5f, 2.0f), v3(0.0f, 1.0f, 0.0f), v3(1.0f, 1.0f, 1.0f)));
	Primitive blueCube  = prmv(CUBE, (void*)cube(v3(-3.0f, 2.5f, 2.0f), v3(0.0f, 0.0f, 1.0f), v3(1.0f, 1.0f, 1.0f)));

	ShapeGroup cubeGroup = group(greenCube, blueCube, BLENDING, 2.0f);

	ShapeGroup groups[2];
	groups[0] = sphsGroup;
	groups[1] = cubeGroup;

	float f[groupSize * 2];
	groups2floats(f, 2, groups);

	shdSetFloatArray(gl->s, "rawGroups", groupSize * 2, f);
}

void updateScene(float time) {
	// recreate all object that are supposed to be moving
	sendCamera();
	createLight(time);
}
