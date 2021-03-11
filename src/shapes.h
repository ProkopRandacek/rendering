// vim: filetype=c
typedef enum ShapeType {
	GROUP, CUBE, SPHERE, CYLINDER, BOXFRAME, TORUS, CTORUS, LINK, PLANE, HEXPRISM, TRIPRISM, CAPSULE, CCONE, PYRAMID, TRIANGLE, QUAD
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
	float radius;
	float fatness;
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
	float radius;
} HexPrism;

typedef struct TriPrism {
	Vector3 pos;
	Vector3 clr;
	float height;
	float radius;
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

typedef struct CCone {
	Vector3 start;
	Vector3 end;
	Vector3 clr;
	float startR;
	float endR;
} CCone;

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
	ShapeType ta;
	ShapeType tb;
	OperationType op;
	float k; // modifier for operation.
	int a;
	int b;
} ShapeGroup;

Primitive prmv(ShapeType type, void* shape);
ShapeGroup group(ShapeType ta, int a, ShapeType tb, int b, OperationType op, float k);

Sphere* sph(Vector3 pos, Vector3 clr, float radius);
Cube* cube(Vector3 pos, Vector3 clr, Vector3 scale, float roundEdge);
BoxFrame* frame(Vector3 pos, Vector3 clr, Vector3 scale, float width);
Torus* tor(Vector3 pos, Vector3 clr, float innerR, float outerR);
CTorus* ctor(Vector3 pos, Vector3 clr, float scX, float scY, float ra, float rb);
Link* lik(Vector3 pos, Vector3 clr, float innerR, float outerR, float length);
Plane* pln(Vector3 pos, Vector3 clr, Vector3 n, float h);
HexPrism* xprism(Vector3 pos, Vector3 clr, float h, float r);
TriPrism* tprism(Vector3 pos, Vector3 clr, float h, float r);
Capsule* caps(Vector3 start, Vector3 end, Vector3 clr, float r);
Cylinder* cyl(Vector3 start, Vector3 stop, Vector3 clr, float r);
CCone* ccone(Vector3 start, Vector3 end, Vector3 clr, float startR, float endR);
Pyramid* pyr(Vector3 pos, Vector3 clr, float h);
Triangle* tri(Vector3 a, Vector3 b, Vector3 c, Vector3 clr);
Quad* quad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 clr);
