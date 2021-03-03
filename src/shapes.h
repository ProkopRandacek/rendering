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

typedef struct BoxFrame {
	Vector3 pos;
	Vector3 clr;
	Vector3 scale;
	float width;
} BoxFrame;

typedef struct Torus {
	Vector3 pos;
	Vector3 clr;
	float innerR;
	float outerR;
} Torus;

typedef struct CTorus {
	Vector3 pos;
	Vector3 clr;
	float scX; // TODO investigate what the values do
	float scY;
	float ra;
	float rb;
} CTorus;

typedef struct Link {
	Vector3 pos;
	Vector3 clr;
	float length;
	float innerR;
	float outerR;
} Link;

typedef struct Plane {
	Vector3 pos;
	Vector3 clr;
	Vector3 n;
	float h; // TODO whats h
} Plane;

typedef struct HexPrism {
	Vector3 pos;
	Vector3 clr;
	float height;
} HexPrism;

typedef struct TriPrism {
	Vector3 pos;
	Vector3 clr;
	float height;
} TriPrism;

typedef struct Capsule { // Like cylinder but round
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float radius;
} Capsule;

typedef struct Cylinder {
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float radius;
} Cylinder;

typedef struct RoundCone {
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float startR;
	float endR;
} RoundCone;

typedef struct Pyramid {
	Vector3 pos;
	Vector3 clr;
	float h;
} Pyramid;

typedef struct Triangle {
	Vector3 a, b, c;
	Vector3 clr;
} Triangle;

typedef struct Quad {
	Vector3 a, b, c, d;
	Vector3 clr;
} Quad;


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
