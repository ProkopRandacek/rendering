const int STEPSNUM = 1024;
const float COLLISION_THRESHOLD = 0.00001;
const float MAX_TRACE_DIST = 100.0;

const float SUN_SIZE = 0.2;
const int PIXEL_GAP = 0; // 0 = no gap; 10 = render only every 10th pixel

// 3D checkerboard pattern for coloring object. RN accesible only if hardcoded (like floor in mapWorld())
vec3 checkerboard(in vec3 pos) {
	pos += vec3(4.0); // there is a off by one error around y=0 || x=0, so i just move these outside the shape
	if (((int(pos.x) + int(pos.y) + int(pos.z)) % 2) == 0.0) { return vec3(0.5);
	} else { return vec3(0.7); }
}

// distance to nearest object
// returns xyz as a color of the surface and w as the distance to it
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
	const vec3 smol = vec3(0.00001, 0.0, 0.0);
	float x = mapWorld(p + smol.xyy).w - mapWorld(p - smol.xyy).w;
	float y = mapWorld(p + smol.yxy).w - mapWorld(p - smol.yxy).w;
	float z = mapWorld(p + smol.yyx).w - mapWorld(p - smol.yyx).w;
	return normalize(vec3(x, y, z));
}

// casts a ray
rayHit rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
	float distTraveled = 0.0;
	float shadow = 1.0;

	for (int i = 0; i < STEPSNUM; ++i) {
		vec3 currentPos = rayOrigin + (distTraveled * rayDir);
		vec4 hit = mapWorld(currentPos);

		float safeDist = hit.w;
		shadow = min(shadow, safeDist/(distTraveled * SUN_SIZE));

		if (safeDist < COLLISION_THRESHOLD) { // collision
			return rayHit(currentPos, hit.rgb, i, true, shadow);
		}
		distTraveled += safeDist;
		if (distTraveled > MAX_TRACE_DIST) { // too far
			return rayHit(vec3(0.0), vec3(0.1), i, false, shadow); // run out of trace_dist or stepsnum
		}
	}

	return rayHit(vec3(0.0), vec3(0.1), STEPSNUM, false, shadow); // run out of trace_dist or stepsnum
}

void main() {
	// skiping pixels (if enabled)
	if ((int(gl_FragCoord.x) + int(gl_FragCoord.y)) % (PIXEL_GAP + 1) != 0) {
		outColor = vec4(0.0);
		return;
	}

	// calculate ray direction
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	vec3 startPos, dir;
	vec3 lPoint = mix(cam[4], cam[2], uv.y);
	vec3 rPoint = mix(cam[3], cam[1], uv.y);
	vec3 point  = mix(rPoint, lPoint, uv.x);
	if (false) { // isometric camera TODO
		vec3 lMid = mix(cam[4], cam[2], 0.5);
		vec3 rMid = mix(cam[3], cam[1], 0.5);
		vec3 mid  = mix(rPoint, lPoint, 0.5);
		dir = normalize(mid);
		startPos = point - mid;
	} else {
		dir = normalize(point - cam[0]);
		startPos = cam[0];
	}

	// cast main ray
	vec3 finalClr;
	rayHit hit = rayMarch(startPos, dir);
	finalClr = hit.surfaceClr;

	if (hit.hit) { // only calculate shadows if hit a surface
		// cast shadow ray
		vec3 smolNormal = calculateNormal(hit.hitPos) * COLLISION_THRESHOLD * 100; // use 0.0001 for cool glitch
		// move outside collision threshold
		vec3 moved = hit.hitPos + smolNormal;
		vec3 dir2ls = normalize(lightPos - moved);

		rayHit shadow = rayMarch(moved, dir2ls);

		finalClr *= vec3(shadow.shadow);
	}

	// output color
	outColor = vec4(finalClr, 1.0);
}
