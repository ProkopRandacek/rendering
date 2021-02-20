#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opengl.h"
#include "camera.h"
#include "shapes.h"
#include "scene.h"

extern GL* gl;
extern const int floatPerSph, floatPerCube;

const int sphereNum = 0;
const int cubeNum = 1;

// these are global so other object can be created relative to them
Camera cam;

void createScene() {
	createCamera(0.0f);
	sendCamera();
	createLight(0.0f);
	createSpheres(0.0f);
	createCubes(0.0f);
}

void createCamera(float time) {
	Vector3 campos = v3(0.0f, 5.0f, -10.0f);
	cam = cmr(campos, vNorm(vDir(campos, v3(0.0f, 5.0f, 0.0f))), 0.0f, 1.0f, 1.0f);
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

void createSpheres(float time) {
	Sphere spheres[sphereNum];
	//spheres[0] = sph(v3(3.0f, 2.5f, 0.0f), v3(1.0f, 0.0f, 0.0f), 2.0f);
	//spheres[1] = sph(v3(3.0f, 6.0f, 0.0f), v3(0.0f, 0.0f, 1.0f), 0.5f);

	float f[sphereNum * floatPerSph];
	spheres2floats(f, sphereNum, spheres);

	shdSetFloatArray(gl->s, "rawSpheres", sphereNum * floatPerSph, f);
}

void createCubes(float time) {
	Cube cubes[cubeNum];
	cubes[0] = cube(v3(-3.0f, 7.0f, 0.0f), v3(0.0f, 1.0f, 0.0f), v3(1.0f, 3.0f, 1.0f));

	float f[cubeNum * floatPerCube];
	cubes2floats(f, cubeNum, cubes);

	shdSetFloatArray(gl->s, "rawCubes", cubeNum * floatPerCube, f);
}

void updateScene(float time) {
	// recreate all object that are supposed to be moving
	sendCamera();
	createLight(time);
}
