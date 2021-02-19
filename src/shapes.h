typedef struct Spheres {
	Vector3 pos;
	Vector3 clr;
	float radius;
} Sphere;

typedef struct Cube {
	Vector3 pos;
	Vector3 clr;
	Vector3 scale;
} Cube;

Sphere sph(Vector3 pos, Vector3 clr, float radius);
Cube cube(Vector3 pos, Vector3 clr, Vector3 scale);

void spheres2floats(float* f, int num, Sphere* spheres);
void cubes2floats(float* f, int num, Cube* cubes);
