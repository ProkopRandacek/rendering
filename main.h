#define SPHERE_COUNT 3

typedef struct Spheres {
	Vector3 pos;
	Color color;
	float radius;
} Sphere;

typedef struct Rays {
	Vector3 pos;
	Vector3 dir; // normalized!!!
} Ray;

struct castRayData {
	char hit;
	Vector3 pos;
	int index;
};

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

typedef struct RenderLineData { // is given to line rendering threads
	Scene *s;
	int i; // defines what lines to render
} rld;

void *renderImage(void *param);
void *renderLine(void *param);
Vector3 moveRay(Ray ray, float distance);
struct castRayData castRay(Ray ray, float limit, Scene* s);
float dist2Sphere(Vector3 v, Sphere s);
Color cDarker(Color a, float b);
Color cCombine(Color a, Color b);
Color clr(int r, int g, int b);
Sphere sph(Vector3 pos, Color c, float r);
Camera cmr(Vector3 pos, Vector3 dir, float angle, float h, float w);
