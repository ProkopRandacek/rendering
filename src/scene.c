#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opengl.h"
#include "scene.h"
#include "shapeSerialization.h"


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
	float lsPos[] = {cam.pos.x, cam.pos.y, cam.pos.z};

	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void createObjects() {
	const int groupNum = 2;

	Primitive cylA = prmv(CYLINDER, (void*) cyl(v3(5.0f, 1.0f, 5.0f), v3(5.0f, 3.0f, 5.0f), v3(1.0f, 0.0f, 1.0f), 0.5f));
	Primitive cylB = prmv(CYLINDER, (void*) cyl(v3(4.0f, 2.0f, 5.0f), v3(6.0f, 2.0f, 5.0f), v3(1.0f, 0.0f, 1.0f), 0.5f));
	Primitive cylC = prmv(CYLINDER, (void*) cyl(v3(5.0f, 2.0f, 4.0f), v3(5.0f, 2.0f, 6.0f), v3(1.0f, 0.0f, 1.0f), 0.5f));

	ShapeGroup sgAB = group(cylA, cylB, NORMAL, 1.0f);

	Primitive AB = prmv(GROUP, (void*) (intptr_t) 0); // position of the shape group in the groups array

	ShapeGroup sgABC = group(AB, cylC, NORMAL, 1.0f);

	ShapeGroup groups[groupNum];
	groups[0] = sgAB;
	groups[1] = sgABC;

	float f[groupSize * groupNum];
	groups2floats(f, groupNum, groups);

	shdSetFloatArray(gl->s, "rawGroups", groupSize * groupNum, f);

	free(cylA.shape);
	free(cylB.shape);
	free(cylC.shape);
}

void updateScene(float time) {
	// recreate all object that are supposed to be moving
	sendCamera();
	createLight(time);
}
