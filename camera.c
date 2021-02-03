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

	up.y *= -1.0f; // There is a bug that the screen is for some reason upside down
	// I could find the place where I flip it or just flip it once more here... TODO

	Vector3 right = vMultf(left, -1.0f);
	Vector3 down = vMultf(up, -1.0f);

	c.tl = vAdd(vAdd(up, left), sc);
	c.tr = vAdd(vAdd(up, right), sc);
	c.bl = vAdd(vAdd(down, left), sc);
	c.br = vAdd(vAdd(down, right), sc);

	return c;
}

struct castRayData castRay(Ray ray, float limit, int ignore, Scene *s) {
	float traveled = 0.0f;
	char glow = 0;
	while (1) {
		float safeDist = 999.0f;
		int sphIndex = 0;
		for (int i = 0; i < SPHERE_COUNT; i++) { // Nearest sphere
			if (i == ignore) continue;
			float newDist = d2Sphere(ray.pos, s->s[i]);
			if (safeDist > newDist) {
				safeDist = newDist;
				sphIndex = i;
			}
		}

		float d2Floor = ray.pos.y; // floor is on y = 0 for now
		if (safeDist > d2Floor) {
			safeDist = d2Floor;
			sphIndex = -1;
		}

		if (safeDist > COLLISION_THRESHOLD) { // far enough -> move
			glow += GLOW_PER_MOVE;
			traveled += safeDist;
			ray.pos = moveRay(ray, safeDist);
		} else { // hit!
			struct castRayData crd;
			crd.hit = 1;
			crd.pos = ray.pos;
			crd.index = sphIndex;
			return crd;
		}
		if (traveled > limit) { // too far
			struct castRayData crd;
			crd.hit = 0;
			crd.pos = ray.pos;
			crd.index = -2;
			return crd;
		}
	}
}

Vector3 moveRay(Ray ray, float distance) {
	return vAdd(ray.pos, vMultf(ray.dir, distance));
}
