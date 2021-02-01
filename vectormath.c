#include <stdio.h>
#include <math.h>
#include "vectormath.h"

// Create vector
Vector3 v3(float x, float y, float z) {
	Vector3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

void vPrint(Vector3 v) {
	printf("%.2f %.2f %.2f", v.x, v.y, v.z);
}

// Vector magnitude
float vMag(Vector3 a) {
	return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

// Returns new normalized vector
Vector3 vNorm(Vector3 v) {
	return vMultf(v, 1/vMag(v));
}

Vector3 float2Vector3(float a) {
	return v3(a, a, a);
}

Vector3 vAdd(Vector3 a, Vector3 b) {
	return v3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Dot product
float vDot(Vector3 a, Vector3 b) {
	return a.x * b.x + a.y * b.y;
}

Vector3 vMult(Vector3 a, Vector3 b) {
	return v3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 vMultf(Vector3 a, float b) {
	return v3(a.x * b, a.y * b, a.z * b);
}

// Returns a new vector pointing from a to b
Vector3 vDir(Vector3 a, Vector3 b) {
	// a->b = b - a
	// a->b = b + (a * -1)
	return vAdd(b, vMultf(a, -1.0f));
}

// Return a new vector pointing from a to b; t/1 the way there
Vector3 vLerp(Vector3 a, Vector3 b, float t) {
	return vAdd(a, vMultf(vDir(a, b), t));
}

float vDist(Vector3 a, Vector3 b) {
	return vMag(vDir(a, b));
}
