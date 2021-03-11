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

// Distance to a group
// If A or B is a subgroup, this function needs to be called for both subgroups before calling it for this group.
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
