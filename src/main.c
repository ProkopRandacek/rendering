#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "opengl.h"
#include "scene.h"

extern GL* gl;

int main() {
	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);

	createScene();

	printf("Initialization successful\nStarting main loop\n");

	double previousTime = glfwGetTime();
	int frameCount = 0;

	while (!glfwWindowShouldClose(gl->window)) {
		renderOGL();

		// count FPS
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0) {
			//printf("FPS: %d\n", frameCount);
			frameCount = 0;
			previousTime = currentTime;
		}
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
