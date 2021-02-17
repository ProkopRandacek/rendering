#version 330 core
out vec4 outColor;

uniform float time;

uniform ivec2 resolution;
uniform vec3 cam[5];
// cam[0] = Camera position
// cam[1] = top    left 
// cam[2] = top    right
// cam[3] = bottom right
// cam[4] = bottom left

const int sphNum = 3;
uniform vec4 sphPos[sphNum];
uniform vec4 sphCol[sphNum];
uniform vec3 lightPos;

struct hitData {
    float dist;
    int i;
};

// linear vector interpolation
vec3 Lerp(in vec3 a, in vec3 b, float t) {
	return a + ((b - a) * t);
}

// distance to sphere
float d2Sphere(in vec3 pos, in vec3 sPos, float r) {
	//float displacement = sin(5.0 * pos.x) * sin(5.0 * pos.y) * sin(5.0 * pos.z) * 0.25;
	return length(pos - sPos) - r/* + displacement*/;
}

// distance to nearest object
hitData mapWorld(in vec3 pos) {
	hitData hit = hitData(99999.9, -1);
	for (int i = 0; i < sphNum; i++) {
		float sphereDist = d2Sphere(pos, sphPos[i].xyz, sphPos[i].w);
		if (sphereDist < hit.dist) {
			hit.dist = sphereDist;
			hit.i = i;
		}
	}
	float d2Floor = pos.y; // floor is on y = 0 for now
	if (hit.dist > d2Floor) { // if floor is closest
		hit.dist = d2Floor;
		hit.i = -1;
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
vec3 rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;
	const int STEPSNUM = 512;
	const float COLLISION_THRESHOLD = 0.01;
	const float MAX_TRACE_DIST = 10000.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		hitData hit = mapWorld(currentPos);

		float safeDist = hit.dist;

		if (safeDist < COLLISION_THRESHOLD) { // collision
			vec3 surfaceClr;
			if (hit.i != -1) {
				surfaceClr = sphCol[hit.i].rgb; // sphere color
			} else {
				surfaceClr = checkerboard(currentPos);
			}

			vec3 normal = calculateNormal(currentPos);
			vec3 dir2ls = normalize(lightPos - currentPos);

			float lightIntensity = max(0.0, dot(normal, dir2ls));

			return lightIntensity * surfaceClr;
		}
		distTraveled += safeDist;
		if (safeDist > MAX_TRACE_DIST) { // too far
			break;
		}
	}

	return vec3(0.2); // run out of trace_dist or stepsnum
}

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 lPoint = Lerp(cam[4], cam[2], uv.y);
	vec3 rPoint = Lerp(cam[3], cam[1], uv.y);
	vec3 point  = Lerp(rPoint, lPoint, uv.x);
	vec3 dir = normalize(point - cam[0]);

	outColor = vec4(rayMarch(cam[0], dir), 1.0);
}

