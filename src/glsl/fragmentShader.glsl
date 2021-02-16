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
uniform vec3 lightPos;

struct flnt { // fl(oat and i)nt
    float d;
    int i;
};

// linear vector interpolation
vec3 Lerp(in vec3 a, in vec3 b, float t) {
	return a + ((b - a) * t);
}

// distance to sphere
float d2Sphere(in vec3 pos, in vec3 sPos, float r) {
	float displacement = sin(5.0 * pos.x) * sin(5.0 * pos.y) * sin(5.0 * pos.z) * 0.25;
	return length(pos - sPos) - r + displacement;
}

// distance to nearest object
flnt mapWorld(in vec3 pos) {
	flnt nearest = flnt(99999.9, -1);
	for (int i = 0; i < sphNum; i++) {
		float sphereDist = d2Sphere(pos, sphPos[i].xyz, sphPos[i].w);
		if (sphereDist < nearest.d) {
			nearest.d = sphereDist;
			nearest.i = i;
		}
	}

	return nearest;
}

// calculate normal from given point on a surface
vec3 calculateNormal(in vec3 p) {
	const vec3 smol = vec3(0.01, 0.0, 0.0);

	float x = mapWorld(p + smol.xyy).d - mapWorld(p - smol.xyy).d;
    float y = mapWorld(p + smol.yxy).d - mapWorld(p - smol.yxy).d;
    float z = mapWorld(p + smol.yyx).d - mapWorld(p - smol.yyx).d;

	return normalize(vec3(x, y, z));
}

// ray marching logic
vec3 rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;
	const int STEPSNUM = 256;
	const float COLLISION_THRESHOLD = 0.001;
	const float MAX_TRACE_DIST = 1000.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		flnt nearest = mapWorld(currentPos);

		float safeDist = nearest.d;

		if (safeDist < COLLISION_THRESHOLD) { // collision
			vec3 surfaceClr = sphCol[nearest.i].rgb;

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
