#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "opengl.h"

extern GL* gl;

int main() {
	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);

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


	double previousTime = glfwGetTime();
	int frameCount = 0;

	printf("Initialization successful\nStarting main loop\n");

	while (!glfwWindowShouldClose(gl->window)) {
		// render
		renderOGL();

		// count FPS
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0) {
			printf("FPS: %d\r\n", frameCount);
			frameCount = 0;
			previousTime = currentTime;
		} // */
	}

	exitOGL();
	return 0;
}

// Callback funcs

void onError(int error, const char* description) {
	printf("Error %d: %s\n", error, description);
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//printf("%d, %d, %d, %d\n", key, scancode, action, mods);
	if (scancode == 24 && action == 1) exit(0); // exit on `q` press
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	shdSetIVec2(gl->s, "resolution", width, height);
}
