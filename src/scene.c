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
	const int groupNum = 4;

	// primitive shapes
	Primitive cylA = prmv(CYLINDER, (void*) cyl(v3(5.0f, 1.0f, 5.0f), v3(5.0f, 3.0f, 5.0f), v3(0.0f, 1.0f, 0.0f), 0.5f));
	Primitive cylB = prmv(CYLINDER, (void*) cyl(v3(4.0f, 2.0f, 5.0f), v3(6.0f, 2.0f, 5.0f), v3(0.0f, 1.0f, 0.0f), 0.5f));
	Primitive cylC = prmv(CYLINDER, (void*) cyl(v3(5.0f, 2.0f, 4.0f), v3(5.0f, 2.0f, 6.0f), v3(0.0f, 1.0f, 0.0f), 0.5f));

	Primitive cubeA = prmv(CUBE,   (void*) cube(v3(5.0f, 2.0f, 5.0f), v3(1.0f, 0.0f, 1.0f), v3f(0.8f)));
	Primitive sphA  = prmv(SPHERE, (void*)  sph(v3(5.0f, 2.0f, 5.0f), v3(0.0f, 0.0f, 1.0f), 1.1f));


	// group cylA and cylB together
	ShapeGroup  sgAB = group(cylA, cylB, NORMAL, 1.0f);
	// wrap the AB group into a primitive
	Primitive     AB = prmv(GROUP, (void*) (intptr_t) 1); // 0 is the position of sgAB in groups array. Its a "pointer" for glsl.
	// group AB group and cylC together
	ShapeGroup sgABC = group(cylC, AB, NORMAL, 1.0f);
	// wrap the ABC group into a primitive
	Primitive    ABC = prmv(GROUP, (void*) (intptr_t) 2);



	// group the cube and sphere together
	ShapeGroup sgCS = group(sphA, cubeA, MASK, 1.0f);
	// wrap the CS group into a primitive
	Primitive    CS = prmv(GROUP, (void*) (intptr_t) 0);


	ShapeGroup sgROOT = group(CS, ABC, CUT, 1.0f);

	ShapeGroup groups[groupNum];
	groups[0] = sgCS;
	groups[1] = sgAB;
	groups[2] = sgABC;
	groups[3] = sgROOT;

	float f[groupSize * groupNum];
	groups2floats(f, groupNum, groups);

	shdSetFloatArray(gl->s, "rawGroups", groupSize * groupNum, f);

	free( cylA.shape);
	free( cylB.shape);
	free( cylC.shape);
	free(cubeA.shape);
	free( sphA.shape);
}

void updateScene(float time) {
	// recreate all object that are supposed to be moving
	sendCamera();
	createLight(time);
}
