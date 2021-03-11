#version 330 core
uniform float time;
uniform vec3 lightPos;
uniform ivec2 resolution;
uniform vec3 cam[5];

const int shapeNum = 4;
const int groupNum = 3;

const int shapeSize = 15;
const int groupSize = 4;

uniform float rawShapes[shapeSize * shapeNum];
uniform float rawGroups[groupNum * groupSize];
vec4 d2Groups[groupNum * groupSize];

struct rayHit {
	vec3 hitPos;
	vec3 surfaceClr;
	int steps;
	bool hit;
	float shadow;
};
