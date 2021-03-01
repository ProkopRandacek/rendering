#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "camera.h"
#include "opengl.h"


extern GL* gl;
extern Camera cam;
extern int w, h;
extern float deltaTime;

char wDown = 0;
char aDown = 0;
char sDown = 0;
char dDown = 0;
char shiftDown = 0;
char spaceDown = 0;
const float moveSpeed = 5.0f;
const float mouseSens = 0.12f;

void updateInput() {
	double xpos, ypos;

	// wasd
	Vector3 moveDir = v3(0.0f, 0.0f, 0.0f);
	if (wDown) moveDir.z = 1.0f;
	if (sDown) moveDir.z = -1.0f;
	if (aDown) moveDir.x = 1.0f;
	if (dDown) moveDir.x = -1.0f;
	if (spaceDown) moveDir.y = 1.0f;
	if (shiftDown) moveDir.y = -1.0f;
	if (vMag(moveDir) > 0.0f) {
		moveDir = vMultf(vNorm(moveDir), moveSpeed * deltaTime);
		updateCamPos(&cam, moveDir);
	}

	// mouse
	glfwGetCursorPos(gl->window, &xpos, &ypos);
	float x = ((2.0f * xpos) / w - 1.0f) * -mouseSens;
	float y = (1.0f - (2.0f * ypos) / h) * mouseSens;

	Vector3 dir = vNorm(v3(
		cos(y) * sin(x),
		sin(y),
		cos(y) * cos(x)
	));
	cam = updateCamDir(cam.pos, dir);
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (scancode == 24 && action == 1) exit(0); // exit on `q` press

	if (scancode == 25 && action == 1) { wDown = 1; }
	if (scancode == 38 && action == 1) { aDown = 1; }
	if (scancode == 39 && action == 1) { sDown = 1; }
	if (scancode == 40 && action == 1) { dDown = 1; }
	if (scancode == 50 && action == 1) { shiftDown = 1; }
	if (scancode == 65 && action == 1) { spaceDown = 1; }

	if (scancode == 25 && action == 0) { wDown = 0; }
	if (scancode == 38 && action == 0) { aDown = 0; }
	if (scancode == 39 && action == 0) { sDown = 0; }
	if (scancode == 40 && action == 0) { dDown = 0; }
	if (scancode == 50 && action == 0) { shiftDown = 0; }
	if (scancode == 65 && action == 0) { spaceDown = 0; }
}
