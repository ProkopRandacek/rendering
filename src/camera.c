#include "camera.h"

Camera cmr(Vector3 pos, Vector3 dir, float angle, float h, float w) {
	Camera c;
	c.pos = pos;
	dir = vNorm(dir);

	Vector3 sc = vAdd(pos, dir); // Screen center
	// SIDE VIEW
	//
	// Camera        ...```t <- top points
	// |       ...```      |
	// V ...```            |
	// C- - - dir vec - ->SC <- Screen center point
	//   ```...            |
	//         ```...      |
	//               ```...b <- bottom points

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

	c.tl = vAdd(vAdd(up, left), sc);
	c.tr = vAdd(vAdd(up, right), sc);
	c.bl = vAdd(vAdd(down, left), sc);
	c.br = vAdd(vAdd(down, right), sc);

	return c;
}
