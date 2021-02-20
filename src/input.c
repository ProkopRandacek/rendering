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

const float moveSpeed = 5.0f;
const float mouseSens = 0.12f;

void updateInput() {
	double xpos, ypos;

	// wasd
	Vector3 moveDir = v3(0.0f, 0.0f, 0.0f);
	if (wDown) moveDir.z =  1.0f;
	if (sDown) moveDir.z = -1.0f;
	if (aDown) moveDir.x =  1.0f;
	if (dDown) moveDir.x = -1.0f;

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
	//printf("%d, %d\n", scancode, action);
	if (scancode == 24 && action == 1) exit(0); // exit on `q` press

	if (scancode == 25 && action == 1) { wDown = 1; /* printf("wdown\n"); */ }
	if (scancode == 38 && action == 1) { aDown = 1; /* printf("adown\n"); */ }
	if (scancode == 39 && action == 1) { sDown = 1; /* printf("sdown\n"); */ }
	if (scancode == 40 && action == 1) { dDown = 1; /* printf("ddown\n"); */ }

	if (scancode == 25 && action == 0) { wDown = 0; /* printf("wup\n"); */ }
	if (scancode == 38 && action == 0) { aDown = 0; /* printf("aup\n"); */ }
	if (scancode == 39 && action == 0) { sDown = 0; /* printf("sup\n"); */ }
	if (scancode == 40 && action == 0) { dDown = 0; /* printf("dup\n"); */ }
}
