// vim: tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab
const int STEPSNUM = 1024;
const float COLLISION_THRESHOLD = 0.001;
const float MAX_TRACE_DIST = 30.0;

const int shapeSize = 15;
// 2 shapes + 2 shapeTypes + operationType + k
const int groupSize = (shapeSize * 2) + 4;
const int groupNum = 3;

uniform float time;
uniform vec3 lightPos;
uniform ivec2 resolution;
uniform vec3 cam[5];

// this uniform contains all shape data
uniform float rawGroups[groupNum * groupSize];
vec4 d2Groups[groupNum * groupSize];

// The following functions are to find information in the rawGroups array ("the color of shape B in group number 2" = `bShapeClr(2)`)
// i is the requested group's index
int aShapeType(int i) { return int(rawGroups[groupSize * i + shapeSize * 2 + 0]); } // 0 = cube, 1 = sphere, 2 = donut
int bShapeType(int i) { return int(rawGroups[groupSize * i + shapeSize * 2 + 1]); } // -//-
int aNodeIndex(int i) { return int(rawGroups[groupSize * i + shapeSize * 0 + 1]); } // where the node is pointing
int bNodeIndex(int i) { return int(rawGroups[groupSize * i + shapeSize * 1 + 1]); } // -//-
int grpOpType (int i) { return int(rawGroups[groupSize * i + shapeSize * 2 + 2]); } // 0 = normal, 1 = blending
float grpK    (int i) { return     rawGroups[groupSize * i + shapeSize * 2 + 3] ; }

float aShapeRadius(int i) { return rawGroups[groupSize * i + shapeSize * 0 + 9]; }
float bShapeRadius(int i) { return rawGroups[groupSize * i + shapeSize * 1 + 9]; }

vec3 aShapePos  (int i) { return vec3(rawGroups[groupSize * i + 0            ], rawGroups[groupSize * i + 1            ], rawGroups[groupSize * i + 2            ]); }
vec3 aShapeClr  (int i) { return vec3(rawGroups[groupSize * i + 3            ], rawGroups[groupSize * i + 4            ], rawGroups[groupSize * i + 5            ]); }
vec3 aShapeScale(int i) { return vec3(rawGroups[groupSize * i + 6            ], rawGroups[groupSize * i + 7            ], rawGroups[groupSize * i + 8            ]); }
vec2 aShapeRVec (int i) { return vec2(rawGroups[groupSize * i + 9            ], rawGroups[groupSize * i + 10           ]); }
float aShapeFloat(int i, int o) { return rawGroups[groupSize * i + o]; }

vec3 bShapePos  (int i) { return vec3(rawGroups[groupSize * i + 0 + shapeSize], rawGroups[groupSize * i + 1 + shapeSize], rawGroups[groupSize * i + 2 + shapeSize]); }
vec3 bShapeClr  (int i) { return vec3(rawGroups[groupSize * i + 3 + shapeSize], rawGroups[groupSize * i + 4 + shapeSize], rawGroups[groupSize * i + 5 + shapeSize]); }
vec3 bShapeScale(int i) { return vec3(rawGroups[groupSize * i + 6 + shapeSize], rawGroups[groupSize * i + 7 + shapeSize], rawGroups[groupSize * i + 8 + shapeSize]); }
vec2 bShapeRVec (int i) { return vec2(rawGroups[groupSize * i + 9 + shapeSize], rawGroups[groupSize * i + 10+ shapeSize]); }
float bShapeFloat(int i, int o) { return rawGroups[groupSize * i + o + shapeSize]; }
// end of rawGroups functions

struct rayHit {
	vec3 hitPos;
	vec3 surfaceClr;
	int steps;
};

vec4 Blend(in float a, in float b, in vec3 colA, in vec3 colB, in float k) {
	float h = clamp( 0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	float blendDst = mix(b, a, h) - k * h * (1.0 - h);
	vec3 blendCol = mix(colB, colA, h);
	return vec4(blendCol, blendDst);
}

vec4 Average(in float a, in float b, in vec3 colA, in vec3 colB, in float k) {
	float i = 1.0 - k;
	float c = (a * i) + (b * k);
	vec3 colC = colA * i + colB * k;
	return vec4(colC, c);
}

vec4 Combine(in float dstA, in float dstB, in vec3 colorA, in vec3 colorB, in int operation, in float k) {
	float dst = dstA;
	vec3 color = colorA;

	if (operation == 1) { // Blend
		vec4 blend = Blend(dstA, dstB, colorA, colorB, k);
		dst = blend.w;
		color = blend.xyz;
	}
	else if (operation == 4) { // Average
		vec4 avg = Average(dstA, dstB, colorA, colorB, k);
		dst = avg.w;
		color = avg.xyz;
	}
	else if (operation == 0) { if ( dstB < dstA) { dst =  dstB; color = colorB; } } // Normal (min(a,  b))
	else if (operation == 2) { if (-dstB > dst)  { dst = -dstB; color = colorB; } } // Cut    (max(a, -b))
	else if (operation == 3) { if ( dstB > dst)  { dst =  dstB; color = colorB; } } // Mask   (max(a,  b))

	return vec4(color, dst);
}

vec3 checkerboard(in vec3 pos) {
	pos += vec3(4.0); // there is a off by one error around y=0 || x=0, so i just move these outside the shape
	if (((int(pos.x) + int(pos.y) + int(pos.z)) % 2) == 0.0) { return vec3(0.5);
	} else { return vec3(0.7); }
}

void d2Group(in vec3 pos, in int i) {
	float d2a, d2b;
	vec3 aClr, bClr;

	// Shape A
	if (aShapeType(i) == 0) { // A is a subgroup
		vec4 s = d2Groups[aNodeIndex(i)];
		d2a = s.w; aClr = s.rgb;
	} // else its a regular shape
	else if (aShapeType(i) == 1 ) { aClr = aShapeClr(i); d2a = d2Cube     (pos, aShapePos(i), aShapeScale(i), aShapeRadius(i)); } // cube
	else if (aShapeType(i) == 2 ) { aClr = aShapeClr(i); d2a = d2Sphere   (pos, aShapePos(i), aShapeRadius(i)); }                 // sphere
	else if (aShapeType(i) == 3 ) { aClr = aShapeClr(i); d2a = d2Cylinder (pos, aShapePos(i), aShapeScale(i), aShapeRadius(i)); } // cylinder
	else if (aShapeType(i) == 4 ) { aClr = aShapeClr(i); d2a = d2BoxFrame (pos, aShapePos(i), aShapeScale(i), aShapeRadius(i)); } // box frame
	else if (aShapeType(i) == 5 ) { aClr = aShapeClr(i); d2a = d2Torus    (pos, aShapePos(i), aShapeRVec(i)); } // torus
	else if (aShapeType(i) == 6 ) { aClr = aShapeClr(i); d2a = d2CTorus   (pos, aShapePos(i), aShapeRVec(i), aShapeFloat(i, 11), aShapeFloat(i, 12)); } // ctorus
	else if (aShapeType(i) == 7 ) { aClr = aShapeClr(i); d2a = d2Link     (pos, aShapePos(i), aShapeFloat(i, 11), aShapeFloat(i, 9), aShapeFloat(i, 10)); } // link
	else if (aShapeType(i) == 8 ) { aClr = aShapeClr(i); d2a = d2Plane    (pos, aShapePos(i), aShapeScale(i), aShapeRadius(i)); } // plane
	else if (aShapeType(i) == 9 ) { aClr = aShapeClr(i); d2a = d2HexPrism (pos, aShapePos(i), aShapeRVec(i)); } // hexprism
	else if (aShapeType(i) == 10) { aClr = aShapeClr(i); d2a = d2TriPrism (pos, aShapePos(i), aShapeRVec(i)); } // triprism
	else if (aShapeType(i) == 11) { aClr = aShapeClr(i); d2a = d2Capsule  (pos, aShapePos(i), aShapeScale(i), aShapeRadius(i)); } // capsule
	else if (aShapeType(i) == 12) { aClr = aShapeClr(i); d2a = d2CCone    (pos, aShapePos(i), aShapeScale(i), aShapeFloat(i, 9), aShapeFloat(i, 10)); } // round cone
	else if (aShapeType(i) == 13) { aClr = aShapeClr(i); d2a = d2Pyramid  (pos, aShapePos(i), aShapeRadius(i)); } // pyramid
	// Shape B
	if (bShapeType(i) == 0) { // B is a subgroup
		vec4 s = d2Groups[bNodeIndex(i)];
		d2b = s.w; bClr = s.rgb;
	} // else its a regular shape
	else if (bShapeType(i) == 1 ) { bClr = bShapeClr(i); d2b = d2Cube     (pos, bShapePos(i), bShapeScale(i), bShapeRadius(i)); }
	else if (bShapeType(i) == 2 ) { bClr = bShapeClr(i); d2b = d2Sphere   (pos, bShapePos(i), bShapeRadius(i)); }
	else if (bShapeType(i) == 3 ) { bClr = bShapeClr(i); d2b = d2Cylinder (pos, bShapePos(i), bShapeScale(i), bShapeRadius(i)); }
	else if (bShapeType(i) == 4 ) { bClr = bShapeClr(i); d2b = d2BoxFrame (pos, bShapePos(i), bShapeScale(i), bShapeRadius(i)); } // box frame
	else if (bShapeType(i) == 5 ) { bClr = bShapeClr(i); d2b = d2Torus    (pos, bShapePos(i), bShapeRVec(i)); } // torus
	else if (bShapeType(i) == 6 ) { bClr = bShapeClr(i); d2b = d2CTorus   (pos, bShapePos(i), bShapeRVec(i), bShapeFloat(i, 11), bShapeFloat(i, 12)); } // ctorus
	else if (bShapeType(i) == 7 ) { bClr = bShapeClr(i); d2b = d2Link     (pos, bShapePos(i), bShapeFloat(i, 11), bShapeFloat(i, 9), bShapeFloat(i, 10)); } // link
	else if (bShapeType(i) == 8 ) { bClr = bShapeClr(i); d2b = d2Plane    (pos, bShapePos(i), bShapeScale(i), bShapeRadius(i)); } // plane
	else if (bShapeType(i) == 9 ) { bClr = bShapeClr(i); d2b = d2HexPrism (pos, bShapePos(i), bShapeRVec(i)); } // hexprism
	else if (bShapeType(i) == 10) { bClr = bShapeClr(i); d2b = d2TriPrism (pos, bShapePos(i), bShapeRVec(i)); } // triprism
	else if (bShapeType(i) == 11) { bClr = bShapeClr(i); d2b = d2Capsule  (pos, bShapePos(i), bShapeScale(i), bShapeRadius(i)); } // capsule
	else if (bShapeType(i) == 12) { bClr = bShapeClr(i); d2b = d2CCone    (pos, bShapePos(i), bShapeScale(i), bShapeFloat(i, 9), bShapeFloat(i, 10)); } // capped cone
	else if (bShapeType(i) == 13) { bClr = bShapeClr(i); d2b = d2Pyramid  (pos, bShapePos(i), bShapeRadius(i)); } // pyramid

	// save the dist for later groups that contain this group.
	d2Groups[i] = Combine(d2a, d2b, aClr, bClr, grpOpType(i), grpK(i));
}

// distance to nearest object
vec4 mapWorld(in vec3 pos) {
	float localDist = 200.0;
	vec3  localClr = vec3(0.0);

	for (int i = 0; i < groupNum; i++) { d2Group(pos, i); }

	localClr = d2Groups[groupNum - 1].xyz;
	localDist = d2Groups[groupNum - 1].w;

	// Check floor
	float dist = d2Cube(pos, vec3(0.0, -1.0, 0.0), vec3(4.0, 2.0, 4.0), 0.0);
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
}


