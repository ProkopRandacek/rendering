#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "main.h"
#include "opengl.h"
#include "scene.h"
#include "camera.h"
#include "input.h"

extern GL* gl;

float deltaTime = 0.0f;

int main() {
	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);
	glfwSetKeyCallback(gl->window, onKey);

	createScene();

	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	printf("Initialization successful\nStarting main loop\n");

	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(gl->window)) {
		updateInput();
		updateScene(glfwGetTime());
		renderOGL();

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
	}

	exitOGL();
	return 0;
}

// Callback funcs
void onError(int error, const char* description) {
	printf("Error %d: %s\n", error, description);
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	shdSetIVec2(gl->s, "resolution", width, height);
	float k = 1000.0f;
	setWH(width / k, height / k);
}
