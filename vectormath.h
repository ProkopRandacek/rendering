typedef struct Vector3s {
	float x, y, z;
} Vector3;


Vector3 v3(float x, float y, float z);
void vPrint(Vector3 a);
float vMag(Vector3 a);
Vector3 vNorm(Vector3 v);
Vector3 float2Vector3(float a);
float vDot(Vector3 a, Vector3 b);
Vector3 vCross(Vector3 a, Vector3 b);
Vector3 vAdd(Vector3 a, Vector3 b);
Vector3 vMult(Vector3 a, Vector3 b);
Vector3 vMultf(Vector3 a, float b);
Vector3 vDir(Vector3 a, Vector3 b);
Vector3 vLerp(Vector3 a, Vector3 b, float t);
float vDist(Vector3 a, Vector3 b);
