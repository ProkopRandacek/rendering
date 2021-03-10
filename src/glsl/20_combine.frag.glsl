vec4 Blend(in float a, in float b, in vec3 colA, in vec3 colB, in float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	float blendDst = mix(b, a, h) - k * h * (1.0 - h);
	vec3 blendCol = mix(colB, colA, h);
	return vec4(blendCol, blendDst);
}

vec4 Average(in float a, in float b, in vec3 colA, in vec3 colB, in float k) {
	float i = 1.0 - k;
	float c = (a * i) + (b * k);
	vec3 colC = colA * i + colB * k;
	return vec4(colC, c);
}

vec4 Combine(in float dstA, in float dstB, in vec3 colorA, in vec3 colorB, in int operation, in float k) {
	float dst = dstA;
	vec3 color = colorA;

	if (operation == 1) { // Blend
		vec4 blend = Blend(dstA, dstB, colorA, colorB, k);
		dst = blend.w;
		color = blend.xyz;
	} else if (operation == 4) { // Average
		vec4 avg = Average(dstA, dstB, colorA, colorB, k);
		dst = avg.w;
		color = avg.xyz;
	}
	else if (operation == 0) { if ( dstB < dstA) { dst =  dstB; color = colorB; } } // Normal (min(a,  b))
	else if (operation == 2) { if (-dstB > dst)  { dst = -dstB; color = colorB; } } // Cut    (max(a, -b))
	else if (operation == 3) { if ( dstB > dst)  { dst =  dstB; color = colorB; } } // Mask   (max(a,  b))
	// planed:
	// abstraction - A is a complicated subgroup and B is a simple abstraction to use when far away from the object for optimalization.

	return vec4(color, dst);
}
