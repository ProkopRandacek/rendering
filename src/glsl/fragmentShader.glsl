#version 330 core
out vec4 outColor;

uniform float time;
uniform vec3 lightPos;

uniform ivec2 resolution;
uniform vec3 cam[5]; // cam[0] = Camera position
// cam[1] = top    left
// cam[2] = top    right
// cam[3] = bottom right
// cam[4] = bottom left

const int STEPSNUM = 512;
const float COLLISION_THRESHOLD = 0.001;
const float MAX_TRACE_DIST = 30.0;

// === SPHERES ===
const int sphNum = 2;
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
const int cubeNum = 4;
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
	vec3 clr;
	//int shapeType; // 0 = floor; 1 = sphere; 2 = cube
	// maybe reserve ranges of i for different shapes and spare one int for optimalization TODO
};

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

vec3 checkerboard(in vec3 pos) {
	if (((int(pos.x) + int(pos.z)) % 2) == 0) {
		return vec3(0.5);
	} else {
		return vec3(0.7);
	}
}

// distance to nearest object
objData mapWorld(in vec3 pos) {
	float k = sin(time) + 1.0;
	const int operation = 1;

	float localDist = 200.0;
	vec3  localClr = vec3(0.0);
	float dist;

	// Check spheres
	for (int i = 0; i < sphNum; i++) {
		dist = d2Sphere(pos, sphPos(i), sphRad(i));
		vec4 combined = Combine(localDist, dist, localClr, sphClr(i), operation, k);
		localClr = combined.xyz;
		localDist = combined.w;
	}

	// Check cubes
	for (int i = 0; i < cubeNum; i++) {
		dist = d2Cube(pos, cubePos(i), cubeScale(i));
		vec4 combined = Combine(localDist, dist, localClr, cubeClr(i), operation, k);
		localClr = combined.xyz;
		localDist = combined.w;
	}

	// Check floor
	dist = d2Cube(pos, vec3(0.0), vec3(10.0, 0.1, 10.0));
	vec4 combined = Combine(localDist, dist, localClr, checkerboard(pos), operation, k);
	localClr = combined.xyz;
	localDist = combined.w;

	objData hit = objData(localDist, localClr);
	return hit;
}

// calculate normal from given point on a surface
vec3 calculateNormal(in vec3 p) {
	const vec3 smol = vec3(0.0001, 0.0, 0.0);

	float x = mapWorld(p + smol.xyy).dist - mapWorld(p - smol.xyy).dist;
	float y = mapWorld(p + smol.yxy).dist - mapWorld(p - smol.yxy).dist;
	float z = mapWorld(p + smol.yyx).dist - mapWorld(p - smol.yyx).dist;

	return normalize(vec3(x, y, z));
}

// ray marching logic
rayHit rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		objData hit = mapWorld(currentPos);

		float safeDist = hit.dist;

		if (safeDist < COLLISION_THRESHOLD) { // collision
			vec3 surfaceClr = hit.clr;

			vec3 normal = calculateNormal(currentPos);
			vec3 dir2ls = normalize(lightPos - currentPos);

			float shadow = max(0.0, dot(normal, dir2ls));

			return rayHit(currentPos, shadow * surfaceClr, i);
		}
		distTraveled += safeDist;
		if (safeDist > MAX_TRACE_DIST) { // too far
			return rayHit(vec3(0.0), vec3(0.2), i); // run out of trace_dist or stepsnum
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

	outColor = vec4(clr, 1.0);
	//outColor = vec4(vec3(hit.steps / float(STEPSNUM)), 1.0);
}

