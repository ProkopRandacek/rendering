#version 330 core
out vec4 outColor;

uniform float time;
uniform vec3 lightPos;

uniform ivec2 resolution;
uniform vec3 cam[5];

const int STEPSNUM = 1024;
const float COLLISION_THRESHOLD = 0.001;
const float MAX_TRACE_DIST = 30.0;

const int sphereSize = 7;
const int cubeSize = 9;

// 2 shapes + 2 shapeTypes + operationType + k
const int groupSize = (cubeSize * 2) + 4;
const int groupNum = 2;

// this uniform contains all shape data
uniform float rawGroups[groupNum * groupSize];

// The following functions are to find information in the rawGroups array
// i is the requested group index
int aShapeType(int i) { return int(rawGroups[groupSize * i + cubeSize * 2 + 0]); } // 0 = cube, 1 = sphere
int bShapeType(int i) { return int(rawGroups[groupSize * i + cubeSize * 2 + 1]); } // -//-
int grpOpType (int i) { return int(rawGroups[groupSize * i + cubeSize * 2 + 2]); } // 0 = normal, 1 = blending
int grpK      (int i) { return int(rawGroups[groupSize * i + cubeSize * 2 + 3]); }

vec3 aShapePos(int i) {
	return vec3(
			rawGroups[groupSize * i + 0],
			rawGroups[groupSize * i + 1],
			rawGroups[groupSize * i + 2]);
}

vec3 aShapeClr(int i) {
	return vec3(
			rawGroups[groupSize * i + 3],
			rawGroups[groupSize * i + 4],
			rawGroups[groupSize * i + 5]);
}

vec3 aShapeScale(int i) {
	return vec3(
			rawGroups[groupSize * i + 6],
			rawGroups[groupSize * i + 7],
			rawGroups[groupSize * i + 8]);
}
vec3 bShapePos(int i) {
	return vec3(
			rawGroups[groupSize * i + 0 + cubeSize],
			rawGroups[groupSize * i + 1 + cubeSize],
			rawGroups[groupSize * i + 2 + cubeSize]);
}
vec3 bShapeClr(int i) {
	return vec3(
			rawGroups[groupSize * i + 3 + cubeSize],
			rawGroups[groupSize * i + 4 + cubeSize],
			rawGroups[groupSize * i + 5 + cubeSize]);
}
vec3 bShapeScale(int i) {
	return vec3(
			rawGroups[groupSize * i + 6 + cubeSize],
			rawGroups[groupSize * i + 7 + cubeSize],
			rawGroups[groupSize * i + 8 + cubeSize]);
}
// end of rawGroups functions

struct rayHit {
	vec3 hitPos;
	vec3 surfaceClr;
	int steps;
};

// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
float d2Sphere(in vec3 pos, in vec3 sphereCenter, float radius) {
	//float displacement = sin(5.0 * pos.x) * sin(5.0 * pos.y) * sin(5.0 * pos.z) * 0.25;
	return length(pos - sphereCenter) - radius/* + displacement*/;
}

float d2Cube(in vec3 pos, in vec3 cubeCenter, in vec3 scale) {
	vec3 o = abs(pos - cubeCenter) - scale;
	float ud = length(max(o, 0));
	float n = max(max(min(o.x, 0), min(o.y, 0)), min(o.z, 0));
	return ud + n;
}

vec4 Blend(float a, float b, vec3 colA, vec3 colB, float k )
{
	float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
	float blendDst = mix( b, a, h ) - k*h*(1.0-h);
	vec3 blendCol = mix(colB,colA,h);

	return vec4(blendCol, blendDst);
}

vec4 Combine(float dstA, float dstB, vec3 colourA, vec3 colourB, int operation, float blendStrength) {
	float dst = dstA;
	vec3 colour = colourA;

	if (operation == 0) {
		if (dstB < dstA) {
			dst = dstB;
			colour = colourB;
		}
	}
	// Blend
	else if (operation == 1) {
		vec4 blend = Blend(dstA, dstB, colourA, colourB, blendStrength);
		dst = blend.w;
		colour = blend.xyz;
	}
	// Cut
	else if (operation == 2) {
		// max(a,-b)
		if (-dstB > dst) {
			dst = -dstB;
			colour = colourB;
		}
	}
	// Mask
	else if (operation == 3) {
		// max(a,b)
		if (dstB > dst) {
			dst = dstB;
			colour = colourB;
		}
	}

	return vec4(colour,dst);
}

vec4 d2Group(in vec3 pos, int i) {
	float d2a, d2b;

	// Shape A
	if (aShapeType(i) == 0) {
		d2a = d2Cube(pos, aShapePos(i), aShapeScale(i));
	} else if (aShapeType(i) == 1) {
		d2a = d2Sphere(pos, aShapePos(i), aShapeScale(i).x);
	}
	// Shape B
	if (bShapeType(i) == 0) {
		d2b = d2Cube(pos, bShapePos(i), bShapeScale(i));
	} else if (bShapeType(i) == 1) {
		d2b = d2Sphere(pos, bShapePos(i), bShapeScale(i).x);
	}

	return Combine(d2a, d2b, aShapeClr(i), bShapeClr(i), grpOpType(i), grpK(i));
}

vec3 checkerboard(in vec3 pos) {
	if (((int(pos.x) + int(pos.z)) % 2) == 0) {
		return vec3(0.5);
	} else {
		return vec3(0.7);
	}
}

// distance to nearest object
vec4 mapWorld(in vec3 pos) {
	float localDist = 200.0;
	vec3  localClr = vec3(0.0);

	for (int i = 0; i < groupNum; i++) {
		vec4 grp = d2Group(pos, i);
		vec4 combined = Combine(localDist, grp.w, localClr, grp.xyz, 0, 0.0);
		localClr = combined.xyz;
		localDist = combined.w;
	}

	// Check floor
	float dist = d2Cube(pos, vec3(0.0), vec3(10.0, 0.1, 10.0));
	vec4 combined = Combine(localDist, dist, localClr, checkerboard(pos), 0, 0.0);
	localClr = combined.xyz;
	localDist = combined.w;

	return vec4(localClr, localDist);
}

// calculate normal from given point on a surface
vec3 calculateNormal(in vec3 p) {
	const vec3 smol = vec3(0.0001, 0.0, 0.0);

	float x = mapWorld(p + smol.xyy).w - mapWorld(p - smol.xyy).w;
	float y = mapWorld(p + smol.yxy).w - mapWorld(p - smol.yxy).w;
	float z = mapWorld(p + smol.yyx).w - mapWorld(p - smol.yyx).w;

	return normalize(vec3(x, y, z));
}

// ray marching logic
rayHit rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		vec4 hit = mapWorld(currentPos);

		float safeDist = hit.w;

		if (safeDist < COLLISION_THRESHOLD) { // collision
			vec3 surfaceClr = hit.rgb;

			vec3 normal = calculateNormal(currentPos);
			vec3 dir2ls = normalize(lightPos - currentPos);

			float shadow = max(0.0, dot(normal, dir2ls));

			return rayHit(currentPos, shadow * surfaceClr, i);
		}
		distTraveled += safeDist;
		if (safeDist > MAX_TRACE_DIST) { // too far
			return rayHit(vec3(0.0), vec3(1.0), i); // run out of trace_dist or stepsnum
		}
	}

	return rayHit(vec3(0.0), vec3(1.0), STEPSNUM); // run out of trace_dist or stepsnum
}

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 lPoint = mix(cam[4], cam[2], uv.y);
	vec3 rPoint = mix(cam[3], cam[1], uv.y);
	vec3 point  = mix(rPoint, lPoint, uv.x);
	vec3 dir = normalize(point - cam[0]);

	rayHit hit = rayMarch(cam[0], dir);
	vec3 clr = hit.surfaceClr;

	outColor = vec4(clr, 1.0);
	//outColor = vec4(vec3(hit.steps / float(STEPSNUM)), 1.0);
}

