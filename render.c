#include <stdio.h>
#include <string.h>

#include "color.h"
#include "render.h"

Color screen[W][H];

void putpixel(int x, int y, Color color) {
	screen[x][y] = color;
}

/*void drawline(int x0, int y0, int x1, int y1, Color color) {
	int dx, dy, p, x, y;
	dx = x1 - x0;
	dy = y1 - y0;
	p = 2 * dy - dx;
	while (x0 <= x1) {
		if (p >= 0) {
			putpixel(x0, y0, color);
			y0 = y0 + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else {
			putpixel(x0, y0, color);
			p = p + 2 * dy;
		}
		x0 = x0 + 1;
	}
}*/

void writeBMP(const char* filename) {
	unsigned int header[14];
	int i, j;
	FILE* fp = fopen(filename, "wb");
	unsigned char pad[3] = {0, 0, 0};
	header[0] = 0x4d420000;
	header[1] = 54 + 3 * H * W;
	header[2] = 0;
	header[3] = 54;
	header[4] = 40;
	header[5] = W;
	header[6] = H;
	header[7] = 0x00180001;
	header[8] = 0;
	header[9] = 3 * W * H;
	header[10] = header[11] = header[12] = header[13] = 0;
	fwrite((char*)header + 2, 1, 54, fp);
	fflush(fp);
	for(i = 0; i < H; i++) {
		for(j = 0; j < W; j++) {
			unsigned char R = screen[j][i].r;
			unsigned char G = screen[j][i].g;
			unsigned char B = screen[j][i].b;
			fwrite(&B, 1, 1, fp);
			fwrite(&G, 1, 1, fp);
			fwrite(&R, 1, 1, fp);
		}
		fwrite(pad, W % 4, 1, fp);
	}
	fclose(fp);
}
