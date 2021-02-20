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

extern GL* gl;
extern Camera cam;

float deltaTime = 0.0f;

char wDown = 0;
char aDown = 0;
char sDown = 0;
char dDown = 0;

int main() {
	const float moveSpeed = 5.0f;

	initOGL();

	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(gl->window, onKey);
	glfwSetFramebufferSizeCallback(gl->window, resize);

	createScene();

	printf("Initialization successful\nStarting main loop\n");

	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(gl->window)) {
		updateScene(glfwGetTime());
		renderOGL();
		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		Vector3 moveDir = v3(0.0f, 0.0f, 0.0f);
		if (wDown) moveDir.z += 1.0f;
		if (aDown) moveDir.x += 1.0f;
		if (sDown) moveDir.z -= 1.0f;
		if (dDown) moveDir.x -= 1.0f;
		if (vMag(moveDir) > 0.0f) {
			moveDir = vMultf(vNorm(moveDir), moveSpeed * deltaTime);
			updateCamPos(&cam, moveDir);
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
	if (scancode == 24 && action == 1) exit(0); // exit on `q` press

	if (scancode == 25 && action == 1) { wDown = 1; }
	if (scancode == 38 && action == 1) { aDown = 1; }
	if (scancode == 39 && action == 1) { sDown = 1; }
	if (scancode == 40 && action == 1) { dDown = 1; }

	if (scancode == 25 && action == 0) { wDown = 0; }
	if (scancode == 38 && action == 0) { aDown = 0; }
	if (scancode == 39 && action == 0) { sDown = 0; }
	if (scancode == 40 && action == 0) { dDown = 0; }
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	shdSetIVec2(gl->s, "resolution", width, height);
}
