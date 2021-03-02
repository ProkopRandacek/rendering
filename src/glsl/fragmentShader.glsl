#version 330 core
// vim: noai:ts=4:
out vec4 outColor;

uniform float time;
uniform vec3 lightPos;

uniform ivec2 resolution;
uniform vec3 cam[5];

const int STEPSNUM = 1024;
const float COLLISION_THRESHOLD = 0.001;
const float MAX_TRACE_DIST = 30.0;

const int sphereSize = 7;
const int cubeSize = 9; const int cylinderSize = 10;

// 2 shapes + 2 shapeTypes + operationType + k
const int groupSize = (cylinderSize * 2) + 4;
const int groupNum = 4;

// this uniform contains all shape data
uniform float rawGroups[groupNum * groupSize];
vec4 d2Groups[groupNum * groupSize];

// The following functions are to find information in the rawGroups array
// i is the requested group's index
int aShapeType(in int i) { return int(rawGroups[groupSize * i + cylinderSize * 2 + 0]); }// 0 = cube, 1 = sphere
int bShapeType(in int i) { return int(rawGroups[groupSize * i + cylinderSize * 2 + 1]); }// -//-
int aNodeIndex(in int i) { return int(rawGroups[groupSize * i + cylinderSize * 0 + 1]); }// where the node is pointing
int bNodeIndex(in int i) { return int(rawGroups[groupSize * i + cylinderSize * 1 + 1]); }// -//-
int grpOpType (in int i) { return int(rawGroups[groupSize * i + cylinderSize * 2 + 2]); }// 0 = normal, 1 = blending
int grpK      (in int i) { return int(rawGroups[groupSize * i + cylinderSize * 2 + 3]); }

float aShapeRadius(in int i) { return rawGroups[groupSize * i + cylinderSize * 0 + 9]; }
float bShapeRadius(in int i) { return rawGroups[groupSize * i + cylinderSize * 1 + 9]; }

vec3 aShapePos(in int i) {
	return vec3(
			rawGroups[groupSize * i + 0],
			rawGroups[groupSize * i + 1],
			rawGroups[groupSize * i + 2]);
}
vec3 aShapeClr(in int i) {
	return vec3(
			rawGroups[groupSize * i + 3],
			rawGroups[groupSize * i + 4],
			rawGroups[groupSize * i + 5]);
}
vec3 aShapeScale(in int i) {
	return vec3(
			rawGroups[groupSize * i + 6],
			rawGroups[groupSize * i + 7],
			rawGroups[groupSize * i + 8]);
}
vec3 bShapePos(in int i) {
	return vec3(
			rawGroups[groupSize * i + 0 + cylinderSize],
			rawGroups[groupSize * i + 1 + cylinderSize],
			rawGroups[groupSize * i + 2 + cylinderSize]);
}
vec3 bShapeClr(in int i) {
	return vec3(
			rawGroups[groupSize * i + 3 + cylinderSize],
			rawGroups[groupSize * i + 4 + cylinderSize],
			rawGroups[groupSize * i + 5 + cylinderSize]);
}
vec3 bShapeScale(in int i) {
	return vec3(
			rawGroups[groupSize * i + 6 + cylinderSize],
			rawGroups[groupSize * i + 7 + cylinderSize],
			rawGroups[groupSize * i + 8 + cylinderSize]);
}
// end of rawGroups functions

struct rayHit {
	vec3 hitPos;
	vec3 surfaceClr;
	int steps;
};

// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
float d2Sphere(in vec3 pos, in vec3 sphereCenter, in float radius) {
	//float displacement = sin(5.0 * pos.x) * sin(5.0 * pos.y) * sin(5.0 * pos.z) * 0.25;
	return length(pos - sphereCenter) - radius/* + displacement*/;
}

float d2Cube(in vec3 pos, in vec3 cubeCenter, in vec3 scale) {
	vec3 o = abs(pos - cubeCenter) - scale;
	float ud = length(max(o, 0));
	float n = max(max(min(o.x, 0), min(o.y, 0)), min(o.z, 0));
	return ud + n;
}

// Cylinder from a to b with radius r
float d2Cylinder(in vec3 p, in vec3 a, in vec3 b, in float r) {
	vec3  ba = b - a;
	vec3  pa = p - a;
	float baba = dot(ba, ba);
	float paba = dot(pa, ba);
	float x = length(pa*baba-ba*paba) - r*baba;
	float y = abs(paba-baba*0.5)-baba*0.5;
	float x2 = x*x;
	float y2 = y*y*baba;
	float d = (max(x, y)<0.0)?-min(x2, y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0));
	return sign(d)*sqrt(abs(d))/baba;
}

vec4 Blend(in float a, in float b, in vec3 colA, in vec3 colB, in float k) {
	float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
	float blendDst = mix(b, a, h) - k*h*(1.0-h);
	vec3 blendCol = mix(colB, colA, h);

	return vec4(blendCol, blendDst);
}

vec4 Combine(in float dstA, in float dstB, in vec3 colourA, in vec3 colourB, in int operation, in float blendStrength) {
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

	return vec4(colour, dst);
}

vec3 checkerboard(in vec3 pos) {
	if (((int(pos.x) + int(pos.z)) % 2) == 0) {
		return vec3(0.5);
	} else {
		return vec3(0.7);
	}
}

vec4 d2Group(in vec3 pos, in int i) {
	float d2a, d2b;
	vec3 aClr, bClr;

	// Shape A
	if (aShapeType(i) == 0) { // A is a subgroup
		vec4 s = d2Groups[aNodeIndex(i)];
		d2a = s.w;
		aClr = s.rgb;
	} // else its a regular shape
	else if (aShapeType(i) == 1) { aClr = aShapeClr(i); d2a = d2Cube    (pos, aShapePos(i), aShapeScale(i)); }
	else if (aShapeType(i) == 2) { aClr = aShapeClr(i); d2a = d2Sphere  (pos, aShapePos(i), aShapeRadius(i)); }
	else if (aShapeType(i) == 3) { aClr = aShapeClr(i); d2a = d2Cylinder(pos, aShapePos(i), aShapeScale(i), aShapeRadius(i)); }
	// Shape B
	if (bShapeType(i) == 0) { // B is a subgroup
		vec4 s = d2Groups[bNodeIndex(i)];
		d2b = s.w;
		bClr = s.rgb;
	} // else its a regular shape
	else if (bShapeType(i) == 1) { bClr = bShapeClr(i); d2b = d2Cube    (pos, bShapePos(i), bShapeScale(i)); }
	else if (bShapeType(i) == 2) { bClr = bShapeClr(i); d2b = d2Sphere  (pos, bShapePos(i), bShapeRadius(i)); }
	else if (bShapeType(i) == 3) { bClr = bShapeClr(i); d2b = d2Cylinder(pos, bShapePos(i), bShapeScale(i), bShapeRadius(i)); }

	vec4 g = Combine(d2a, d2b, aClr, bClr, grpOpType(i), grpK(i));

	// save the dist for later groups that contain this group.
	d2Groups[i] = g;

	return g;
}

// distance to nearest object
vec4 mapWorld(in vec3 pos) {
	float localDist = 200.0;
	vec3  localClr = vec3(0.0);

	for (int i = 0; i < groupNum; i++) {
		vec4 grp = d2Group(pos, i);
		//vec4 combined = Combine(localDist, grp.w, localClr, grp.xyz, 0, 0.0);
		//vec4 combined = grp;
		localClr = grp.xyz;
		localDist = grp.w;
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
			return rayHit(vec3(0.0), vec3(1.0), i);// run out of trace_dist or stepsnum
		}
	}

	return rayHit(vec3(0.0), vec3(1.0), STEPSNUM);// run out of trace_dist or stepsnum
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

