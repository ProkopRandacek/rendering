#version 330 core
out vec4 outColor;

uniform float time;
uniform vec3 lightPos;

uniform ivec2 resolution;
uniform vec3 cam[5];
// cam[0] = Camera position
// cam[1] = top    left
// cam[2] = top    right
// cam[3] = bottom right
// cam[4] = bottom left

// === SPHERES ===
const int sphNum = 1;
const int floatPerSph = 7;
uniform float rawSpheres[sphNum * floatPerSph];

vec3 sphPos(int i) {
	return vec3(
			rawSpheres[i * floatPerSph + 0],
			rawSpheres[i * floatPerSph + 1],
			rawSpheres[i * floatPerSph + 2]
			);
}

vec3 sphClr(int i) {
	return vec3(
			rawSpheres[i * floatPerSph + 3],
			rawSpheres[i * floatPerSph + 4],
			rawSpheres[i * floatPerSph + 5]
			);
}

float sphRad(int i) {
	return rawSpheres[i * floatPerSph + 6];
}

// === CUBES ===
const int cubeNum = 1;
const int floatPerCube = 9;
uniform float rawCubes[cubeNum * floatPerCube];

vec3 cubePos(int i) {
	return vec3(
			rawCubes[i * floatPerCube + 0],
			rawCubes[i * floatPerCube + 1],
			rawCubes[i * floatPerCube + 2]
			);
}

vec3 cubeClr(int i) {
	return vec3(
			rawCubes[i * floatPerCube + 3],
			rawCubes[i * floatPerCube + 4],
			rawCubes[i * floatPerCube + 5]
			);
}

vec3 cubeScale(int i) {
	return vec3(
			rawCubes[i * floatPerCube + 6],
			rawCubes[i * floatPerCube + 7],
			rawCubes[i * floatPerCube + 8]
			);
}

struct objData {
    float dist;
    int i;
	int shapeType; // 0 = floor; 1 = sphere; 2 = cube
	// maybe reserve ranges of i for different shapes and spare one int for optimalization TODO
};

struct rayHit {
	vec3 hitPos;
	vec3 surfaceClr;
	int steps;
};

float smoothMin(float a, float b, float k) {
	float h = max(k - abs(a - b), 0) / k;
	return min(a, b) - h * h * h * k * 1/6.0;
}

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

// distance to nearest object
objData mapWorld(in vec3 pos) {
	objData hit = objData(99999.9, -1, -1);
	float dist;

	// Check spheres
	for (int i = 0; i < sphNum; i++) {
		dist = d2Sphere(pos, sphPos(i), sphRad(i));
		if (dist < hit.dist) {
			hit.dist = dist;
			hit.i = i;
			hit.shapeType = 1;
		}
	}

	// Check cubes
	for (int i = 0; i < cubeNum; i++) {
		dist = d2Cube(pos, cubePos(i), cubeScale(i));
		if (dist < hit.dist) {
			hit.dist = dist;
			hit.i = i;
			hit.shapeType = 2;
		}
	}

	// Check floor
	dist = pos.y; // floor is on y = 0 for now
	if (dist < hit.dist) { // if floor is closest
		hit.dist = dist;
		hit.i = -1;
		hit.shapeType = 0;
	}

	return hit;
}

// calculate normal from given point on a surface
vec3 calculateNormal(in vec3 p) {
	const vec3 smol = vec3(0.01, 0.0, 0.0);

	float x = mapWorld(p + smol.xyy).dist - mapWorld(p - smol.xyy).dist;
    float y = mapWorld(p + smol.yxy).dist - mapWorld(p - smol.yxy).dist;
    float z = mapWorld(p + smol.yyx).dist - mapWorld(p - smol.yyx).dist;

	return normalize(vec3(x, y, z));
}

vec3 checkerboard(in vec3 pos) {
	if (((int(pos.x) + int(pos.z)) % 2) == 0) {
		return vec3(0.5);
	} else {
		return vec3(0.7);
	}
}

// ray marching logic
rayHit rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;
	const int STEPSNUM = 256;
	const float COLLISION_THRESHOLD = 0.01;
	const float MAX_TRACE_DIST = 10000.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		objData hit = mapWorld(currentPos);

		float safeDist = hit.dist;

		if (safeDist < COLLISION_THRESHOLD) { // collision
			vec3 surfaceClr;
			if (hit.shapeType == 0) {
				surfaceClr = checkerboard(currentPos);
			} else if (hit.shapeType == 1) {
				surfaceClr = sphClr(hit.i);
			} else if (hit.shapeType == 2) {
				surfaceClr = cubeClr(hit.i);
			}

			vec3 normal = calculateNormal(currentPos);
			vec3 dir2ls = normalize(lightPos - currentPos);

			float shadow = max(0.0, dot(normal, dir2ls));

			return rayHit(currentPos, shadow * surfaceClr, i);
		}
		distTraveled += safeDist;
		if (safeDist > MAX_TRACE_DIST) { // too far
			break;
		}
	}

	return rayHit(vec3(0.0), vec3(0.2), 0); // run out of trace_dist or stepsnum
}

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 lPoint = mix(cam[4], cam[2], uv.y);
	vec3 rPoint = mix(cam[3], cam[1], uv.y);
	vec3 point  = mix(rPoint, lPoint, uv.x);
	vec3 dir = normalize(point - cam[0]);

	rayHit hit = rayMarch(cam[0], dir);
	vec3 clr = hit.surfaceClr;

	for (int i = 0; i < 1; i++) {
		vec3 normal = calculateNormal(hit.hitPos);
		hit = rayMarch(hit.hitPos, reflect(hit.hitPos, normal));
		clr = mix(hit.surfaceClr, clr.xyz, 0.5);
	}
	outColor = vec4(clr, 1.0);
}
