#version 330 core
out vec4 outColor;

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

// linear vector interpolation
vec3 Lerp(in vec3 a, in vec3 b, float t) {
	return a + ((b - a) * t);
}

// distance to sphere
float d2Sphere(in vec3 pos, in vec3 sPos, float r) {
	return length(pos - sPos) - r;
}

// distance to nearest object
vec4 mapWorld(in vec3 pos) {
	vec4 nearest = vec2(99999.9, 0.0).yyyx;
	for (int i = 0; i < sphNum; i++) {
		float sphereDist = d2Sphere(pos, sphPos[i].xyz, sphPos[i].w);
		if (sphereDist < nearest.w) {
			nearest = vec4(sphCol[i].xyz, sphereDist);
		}
	}

	return nearest;
}

vec3 calculateNormal(in vec3 p) {
	const vec3 smol = vec3(0.001, 0.0, 0.0);

	float x = (mapWorld(p + smol.xyy) - mapWorld(p - smol.xyy)).w;
	float y = (mapWorld(p + smol.yxy) - mapWorld(p - smol.yxy)).w;
	float z = (mapWorld(p + smol.yyx) - mapWorld(p - smol.yyx)).w;

	return normalize(vec3(x, y, z));
}

// ray marching logic
vec3 rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;
	const int STEPSNUM = 32;
	const float COLLISION_THRESHOLD = 0.0001;
	const float MAX_TRACE_DIST = 1000.0;

	for (int i = 0; i < STEPSNUM; i++) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		vec4 nearest = mapWorld(currentPos);

		float safeDist = nearest.w;

		if (safeDist < COLLISION_THRESHOLD) { // collision
			return calculateNormal(currentPos) * 0.5 / 0.5;
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
