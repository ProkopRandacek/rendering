#include "color.h"

Color shadow, sky, ground1, ground2;
Color black, green, cyan, orange, yellow;

void cInit() {
	shadow = clr(128, 128, 128);
	sky = clr(0, 48, 78);
	ground1 = clr(40, 40, 40);
	ground2 = clr(80, 80, 80);

	green = clr(50, 200, 50);
	cyan = clr(0, 255, 200);
	orange = clr(255, 165, 0);
	yellow = clr(255, 255, 0);
}

Color clr(int r, int g, int b) {
	Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

Color cDarker(Color a, float b) {
	return clr(a.r * b, a.g * b, a.b * b);
}

Color cBlend(Color a, Color b, float aw) {
	Color c;
	float bw = 1.0 - aw;
	c.r = a.r * aw + b.r * bw;
	c.g = a.g * aw + b.g * bw;
	c.b = a.b * aw + b.b * bw;
	return c;
}
