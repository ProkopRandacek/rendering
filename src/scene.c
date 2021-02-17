#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "scene.h"
#include "opengl.h"
#include "camera.h"

extern GL* gl;

void createScene() {
	// ==== Camera ====
	createCamera(0.0f);

	// ==== Light source ====
	createLight(0.0f);

	// ==== Spheres ====
	createSpheres(0.0f);
}

void createCamera(float time) {
	Vector3 campos = vLerp(
			v3(-2.0f, 5.0f, -10.0f),
			v3(2.0f, 5.0f, -10.0f),
			sin(time) * 0.5f + 0.5f);
	Camera cam = cmr(campos, v3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f, 1.0f);
	float camFloats[15];
	cam2floats(cam, camFloats);

	//printf("camera size: %ld bytes\n", sizeof(camFloats));
	shdSetVec3Array(gl->s, "cam", 5 * 3, camFloats);
}

void createLight(float time) {
	float lsPos[] = { 2.0, 10.0, -2.0 };

	//printf("light size: %ld bytes\n", sizeof(lsPos));
	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void createSpheres(float time) {
	float sphPos[] = {
		 3.0f, 3.0f, 0.0f, 2.0f,  // x, y, z, radius
		-3.0f, 7.0f, 0.0f, 1.0f,
		 3.0f, 6.0f, 0.0f, 0.5f
	};
	float sphCol[] = {
		1.0f, 0.0f, 0.0f, 1.0f, // r, g, b, reflectiveness
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	//printf("spheres size: %ld bytes\n", sizeof(sphPos) + sizeof(sphCol));
	shdSetVec4Array(gl->s, "sphPos", 3 * 4, sphPos);
	shdSetVec4Array(gl->s, "sphCol", 3 * 4, sphCol);
}

void updateScene(float time) {
	createCamera(time);
}
