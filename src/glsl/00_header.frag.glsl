#version 330 core
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

struct rayHit {
	vec3 hitPos;
	vec3 surfaceClr;
	int steps;
	bool hit;
	float shadow;
};
