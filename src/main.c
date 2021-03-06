#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "main.h"

#include "time.h"


extern GL* gl;

float deltaTime = 0.0f;

int main() {
	printf("\n\n====================================\n\n\n");
	clock_t initS = clock();
	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);
	glfwSetKeyCallback(gl->window, onKey);

	createScene();

	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	clock_t initE = clock();
	printf("init took %.2f ms\n", ((float)(initE - initS) / CLOCKS_PER_SEC * 1000.0f));

	printf("Initialization successful\nStarting main loop\n");

	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(gl->window)) {
		//clock_t start = clock();

		updateInput();
		updateScene(glfwGetTime());
		renderOGL();

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		//clock_t end = clock();
		//printf("\nmain loop took %.2f ms\n", ((float)(end - start) / CLOCKS_PER_SEC * 1000.0f));
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
