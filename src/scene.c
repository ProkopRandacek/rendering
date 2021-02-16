#include <stdio.h>

#include "scene.h"
#include "opengl.h"

extern GL* gl;

void createScene() {
	// === Send scene info to GPU
	// == Camera
	float cam[] = {
		 0.0f, 5.0f, -10.0f, // camera pos
		-5.0f, 10.0f, 0.0f,  // top left
		 5.0f, 10.0f, 0.0f,  // top right
		-5.0f,  0.0f, 0.0f,  // bottom left
		 5.0f,  0.0f, 0.0f}; // bottom right

	printf("camera size: %ld bytes\n", sizeof(cam));
	shdSetVec3Array(gl->s, "cam", 5 * 3, cam);

	// == Spheres
	float sphPos[] = {
		 3.0f, 3.0f, 0.0f, 2.0f,  // x, y, z, radius
		-3.0f, 7.0f, 0.0f, 1.0f,
		 3.0f, 6.0f, 0.0f, 0.5f,};

	float sphCol[] = {
		1.0f, 0.0f, 0.0f, 1.0f, // r, g, b, reflectiveness
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f};

	printf("spheres size: %ld bytes\n", sizeof(sphPos) + sizeof(sphCol));
	shdSetVec4Array(gl->s, "sphPos", 3 * 4, sphPos);
	shdSetVec4Array(gl->s, "sphCol", 3 * 4, sphCol);

	/* == Cubes
	float cubes[] = {
		// pos + scale + rotation + color + reflectiveness = 3 + 3 + 3 + 3 + 1 = 13 floats per cube
	};// */
}

void updateScene(float time) {

}
