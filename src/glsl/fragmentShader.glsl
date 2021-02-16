#version 330 core
out vec4 FragColor;

uniform ivec2 resolution;
uniform float time;
uniform vec3 camVectors[5];

struct Camera {
	vec3 pos; // position of the camera
	vec3 tl; // The direction of most top left ray
	vec3 tr; // top right
	vec3 bl; // ...
	vec3 br;
};

// linera vector interpolation
vec3 Lerp(in vec3 a, in vec3 b, float t) {
	return a + ((b - a) * t);
}

// disitance to sphere
float d2Sphere(in vec3 p, in vec3 c, float r) {
	return length(p - c) - r;
}

vec3 rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;
	const int STEPSNUM = 32;
	const float COLLISION_THRESHOLD = 0.001;
	const float MAX_TRACE_DIST = 1000.0;

	for (int i = 0; i < STEPSNUM; i++) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);

		float safeDist = d2Sphere(currentPos, vec3(0.0, 5.0, 0.0), 1.0);

		if (safeDist < COLLISION_THRESHOLD) { // collision
			return vec3(1.0, 0.0, 0.0);
		}
		distTraveled += safeDist;
		if (safeDist > MAX_TRACE_DIST) { // too far
			break;
		}
	}
	return vec3(0.0); // run out of trace_dist or stepsnum
}

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	Camera cam = Camera(
			vec3(0.0, 5.0, -10.0),  // cam pos
			vec3(-5.0, 10.0, 0.0),  // top left
			vec3( 5.0, 10.0, 0.0),  // top right
			vec3(-5.0,  0.0, 0.0),  // bottom left
			vec3( 5.0,  0.0, 0.0)); // botttom right

	vec3 lPoint = Lerp(cam.tl, cam.bl, uv.y);
	vec3 rPoint = Lerp(cam.tr, cam.br, uv.y);
	vec3 point  = Lerp(rPoint, lPoint, uv.x);

	vec3 dir = normalize(point - cam.pos);

	FragColor = vec4(rayMarch(cam.pos, dir), 1.0);
}
