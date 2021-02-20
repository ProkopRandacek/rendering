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
		printf("ERROR: Camera cant face straight up or down\n"); // causes division by zero FIXME
		exit(1);
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
}

Camera updateCamDir(Vector3 pos, Vector3 dir) {
	return cmr(pos, dir, lastAngle, lastH, lastW);
}

void cam2floats(Camera cam, float* f) {
	f[ 0] = cam.pos.x; f[ 1] = cam.pos.y; f[ 2] = cam.pos.z;
	f[ 3] = cam.tr.x;  f[ 4] = cam.tr.y;  f[ 5] = cam.tr.z;
	f[ 6] = cam.tl.x;  f[ 7] = cam.tl.y;  f[ 8] = cam.tl.z;
	f[ 9] = cam.br.x;  f[10] = cam.br.y;  f[11] = cam.br.z;
	f[12] = cam.bl.x;  f[13] = cam.bl.y;  f[14] = cam.bl.z;
}
