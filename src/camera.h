#include "vector.h"

typedef struct Cameras {
	Vector3 pos;
	Vector3 tl; // The direction of most top left ray
	Vector3 tr; // top right
	Vector3 bl; // ...
	Vector3 br; // ...
} Camera;

Camera cmr(Vector3 pos, Vector3 dir, float angle, float h, float w);
void cam2floats(Camera cam, float* f);
void updateCamPos(Camera* cam, Vector3 offset);
