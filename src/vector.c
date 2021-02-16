#include <stdio.h>
#include <math.h>
#include "vector.h"

Vector3 v3(float x, float y, float z) {
	Vector3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

void vPrint(Vector3 v) { printf("%.2f %.2f %.2f", v.x, v.y, v.z); }

float	vMag	(Vector3 a)			{ return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));}
float	vDot	(Vector3 a, Vector3 b)		{ return a.x * b.x + a.y * b.y;				}
float	vDist	(Vector3 a, Vector3 b)		{ return vMag(vDir(a, b));				}
Vector3	vNorm	(Vector3 a)			{ return vMultf(a, 1 / vMag(a));			}
Vector3	vAdd	(Vector3 a, Vector3 b)		{ return v3(a.x + b.x, a.y + b.y, a.z + b.z);		}
Vector3	vMult	(Vector3 a, Vector3 b)		{ return v3(a.x * b.x, a.y * b.y, a.z * b.z);		}
Vector3	vMultf	(Vector3 a,   float b)		{ return v3(a.x * b, a.y * b, a.z * b);			}
Vector3	vDir	(Vector3 a, Vector3 b)		{ return vAdd(b, vMultf(a, -1.0f));			}
Vector3	vLerp	(Vector3 a, Vector3 b, float c)	{ return vAdd(a, vMultf(vDir(a, b), c));		}

