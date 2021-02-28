#include <stdio.h>
#include <stdlib.h>

#include "camera.h"

float lastAngle, lastH, lastW;

Camera cmr(Vector3 pos, Vector3 dir, float angle, float h, float w) {
	lastAngle = angle;
	lastH = h;
	lastW = w;

	Camera cam;
	cam.pos = pos;
	if (dir.x == 0.0f && (dir.y == 1.0f || dir.y == -1.0f) && dir.z == 0.0f) {
		dir.y += 0.0001f;
		// ugly fix for division by zero. probably works
	}

	Vector3 sc = vAdd(pos, dir); // Screen center
	// SIDE VIEW
	// Camera        ..--``t <- top points
	// |       ..--``      |
	// V ..--``            |
	// C- - - dir vec - ->SC <- Screen center point
	//   ``--..            |
	//         ``--..      |
	//               ``--..b <- bottom points

	//float y = 1.0f / tan(angle); calculate angle TODO

	Vector3 left = vMultf(vNorm(v3(-dir.z, angle, dir.x)), w);
	// CAMERA VIEW
	// tl---------tr // top left, top right
	// |           | // SC = Screen center point
	// |           |
	// <----SC     |
	// |           |
	// |           |
	// bl---------br // bottom left, bottom right

	Vector3 up = vMultf(vNorm(vCross(left, dir)), h);
	// tl----^----tr picture in camera space
	// |     |     | // SC = Screen center point
	// |     |     |
	// |    SC     |
	// |           |
	// |           |
	// bl---------br

	Vector3 right = vMultf(left, -1.0f);
	Vector3 down = vMultf(up, -1.0f);

	cam.left = vNorm(left);
	cam.foward = vNorm(dir);

	cam.tl = vAdd(vAdd(up, left), sc);
	cam.tr = vAdd(vAdd(up, right), sc);
	cam.bl = vAdd(vAdd(down, left), sc);
	cam.br = vAdd(vAdd(down, right), sc);
	return cam;
}

void updateCamPos(Camera* cam, Vector3 offset) {
	Vector3 foward = v3(cam->foward.x, 0, cam->foward.z);
	Vector3 left = v3(cam->left.x, 0, cam->left.z);
	cam->pos = vAdd(cam->pos, vMultf(foward, offset.z));
	cam->pos = vAdd(cam->pos, vMultf(left, -offset.x));
	cam->pos.y += offset.y;
}

void setWH(float w, float h) { lastH = h; lastW = w; }
Camera updateCamDir(Vector3 pos, Vector3 dir) { return cmr(pos, dir, lastAngle, lastH, lastW); }
