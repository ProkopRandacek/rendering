#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opengl.h"
#include "scene.h"
#include "camera.h"

const int sphereNum = 2;
const int cubeNum = 1;

const int floatPerSph = 7;
const int floatPerCube = 9;

extern GL* gl;

void createScene() {
	// ==== Camera ====
	createCamera(0.0f);

	// ==== Light source ====
	createLight(0.0f);

	// ==== Spheres ====
	createSpheres(0.0f);

	// ==== Cubes ====
	createCubes(0.0f);

	/*shdSetInt(gl->s, "sphereNum", sphereNum);
	shdSetInt(gl->s, "cubeNum", cubeNum);
	shdSetInt(gl->s, "floatPerSph", floatPerSph);
	shdSetInt(gl->s, "floatPerCube", floatPerCube);*/ // array size must be constant. Switch to SSBO TODO
	// https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
}

void createCamera(float time) {
	Vector3 campos = v3(0.0f, 5.0f, -10.0f);
	Camera cam = cmr(campos, vNorm(vDir(campos, v3(0.0f, 5.0f, 0.0f))), 0.0f, 1.0f, 1.0f);
	float camFloats[15];
	cam2floats(cam, camFloats);

	//printf("camera size: %ld bytes\n", sizeof(camFloats));
	shdSetVec3Array(gl->s, "cam", 5 * 3, camFloats);
}

void createLight(float time) {
	float lsPos[] = { 0.0f, 5.0f, 0.0f };

	//printf("light size: %ld bytes\n", sizeof(lsPos));
	shdSetVec3Array(gl->s, "lightPos", 1, lsPos);
}

void createSpheres(float time) {
	float spheres[] = {
		 // position         //color             // radius
		 3.0f, 2.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f,
	//	-3.0f, 7.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f,
		 3.0f, 6.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f
	};

	shdSetFloatArray(gl->s, "rawSpheres", sphereNum * floatPerSph, spheres);
}

void createCubes(float time) {
	float cubes[] = {
		 // position         //color             // scale
	//	 3.0f, 2.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 0.0f, 2.0f,
		-3.0f, 7.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f,
	//	 3.0f, 6.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f, 0.5f
	};

	shdSetFloatArray(gl->s, "rawCubes", cubeNum * floatPerCube, cubes);
}

void updateScene(float time) {
	createCamera(time);
}
