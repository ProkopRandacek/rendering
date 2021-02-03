#include "shapes.h"

#define COLLISION_THRESHOLD 0.0001f
#define GLOW_PER_MOVE 0
#define SPHERE_COUNT 3

typedef struct Cameras {
	Vector3 pos;
	Vector3 tl; // the direction of most top left ray
	Vector3 tr; // ...
	Vector3 br; // ...
	Vector3 bl; // ...
} Camera;

typedef struct Scenes {
	Camera cam;
	Color sky;
	Color shadow;
	Vector3 light;
	Sphere s[SPHERE_COUNT];
} Scene;

typedef struct Rays {
	Vector3 pos;
	Vector3 dir; // normalized!!!
} Ray;

typedef struct castRayData {
	char hit;
	Vector3 pos;
	int index;
} rayHit;

Camera cmr(Vector3 pos, Vector3 dir, float angle, float h, float w);
rayHit castRay(Ray ray, float limit, Scene *s);
Vector3 moveRay(Ray ray, float distance);
