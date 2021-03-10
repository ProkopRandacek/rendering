#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "debug.h"
#include "main.h"


extern GL* gl;

float deltaTime = 0.0f;

int main() {
	printf("\n\n====================================\n\n\n");
	startTime(); // debug

	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);
	glfwSetKeyCallback(gl->window, onKey);

	createScene();

	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	dprint("INIT DONE");

	float lastTime = 0.0f;

	dprint("starting main loop");

	while (!glfwWindowShouldClose(gl->window)) {
		updateInput();
		updateScene();
		renderOGL();

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
	}

	dprint("exiting GL");
	exitOGL();
	dprint("return 0;");
	return 0;
}

// Callback funcs
void onError(int error, const char* description) {
	printf("Error %d: %s\n", error, description);
}

void resize(GLFWwindow* window, int width, int height) {
	if (window != gl->window) return;

	glViewport(0, 0, width, height);
	shdSetIVec2(gl->s, "resolution", width, height);
	float k = 1000.0f;
	setWH(width / k, height / k);
}
