// ShapeGroup
//  |
//  +->Primitive a & b
//  |   |
//  |   +-> a general shape. sphere or cube
//  |
//  +-> Operation
//       |
//       +-> The type of min function that is used between these shapes.



typedef enum ShapeType {
	CUBE, SPHERE, CYLINDER
} ShapeType;

typedef enum OperationType {
	NORMAL, BLENDING, CUT, MASK
} OperationType;


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

typedef struct Cylinder {
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float radius;
} Cylinder;

typedef struct Primitive {
	ShapeType type;
	void* shape;
} Primitive;

typedef struct ShapeGroup {
	Primitive a;
	Primitive b;
	OperationType op;
	float k; // modifier for blending operation.
} ShapeGroup;


Sphere* sph(Vector3 pos, Vector3 clr, float radius);
Cube* cube(Vector3 pos, Vector3 clr, Vector3 scale);
Cylinder* cyl(Vector3 start, Vector3 stop, Vector3 clr, float r);

Primitive prmv(ShapeType type, void* shape);
ShapeGroup group(Primitive a, Primitive b, OperationType op, float k);

void spheres2floats(float* f, int num, Sphere* spheres);
void cubes2floats(float* f, int num, Cube* cubes);
void groups2floats(float* f, int num, ShapeGroup* groups);
void cyl2floats(float* f, Cylinder* cyl);
