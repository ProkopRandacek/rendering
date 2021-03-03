// vim: filetype=c
typedef enum ShapeType {
	GROUP, CUBE, SPHERE, CYLINDER
} ShapeType;

typedef enum OperationType {
	NORMAL, BLEND, CUT, MASK, AVERAGE
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
	float roundEdge;
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
Cube* cube(Vector3 pos, Vector3 clr, Vector3 scale, float roundEdge);
Cylinder* cyl(Vector3 start, Vector3 stop, Vector3 clr, float r);
Primitive prmv(ShapeType type, void* shape);
ShapeGroup group(Primitive a, Primitive b, OperationType op, float k);
