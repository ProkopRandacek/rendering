typedef struct Vector3s {
	float x, y, z;
} Vector3;


// Vector constructor
Vector3 v3(float x, float y, float z);

// Create vector from a single float
Vector3 v3f(float a);

// Print vector to console
void vPrint(Vector3 a);

// Vector magnitude
float vMag(Vector3 a);

// Distance between 2 vectors
float vDist(Vector3 a, Vector3 b);

// Dot product of 2 vectors
float vDot(Vector3 a, Vector3 b);

// Cross product of 2 vectors
Vector3 vCross(Vector3 a, Vector3 b);

// Normalize vector
Vector3 vNorm(Vector3 v);

// Add 2 vectors
Vector3 vAdd(Vector3 a, Vector3 b);

// Multiply 2 vectors
Vector3 vMult(Vector3 a, Vector3 b);

// Multiply vector and a float
Vector3 vMultf(Vector3 a, float b);

// Return vector pointing from a to b
Vector3 vDir(Vector3 a, Vector3 b);

// Interpolate between a and b.
Vector3 vLerp(Vector3 a, Vector3 b, float t);

// Rotate on a circle, c is center, t is input for cos/sin, k controls whether to use sins or cosines to specific axis.
Vector3 vRotate(Vector3 c, Vector3 sinK, Vector3 cosK, float t);
