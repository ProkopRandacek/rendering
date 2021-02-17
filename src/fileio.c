#include <stdio.h>

void writeBMP(const char* filename, char* pixels, int w, int h) {
	unsigned int header[14];
	int i, j;
	FILE* fp = fopen(filename, "wb");
	unsigned char pad[3] = {0, 0, 0};
	header[0] = 0x4d420000;
	header[1] = 54 + 3 * h * w;
	header[2] = 0;
	header[3] = 54;
	header[4] = 40;
	header[5] = w;
	header[6] = h;
	header[7] = 0x00180001;
	header[8] = 0;
	header[9] = 3 * w * h;
	header[10] = header[11] = header[12] = header[13] = 0;
	fwrite((char*)header + 2, 1, 54, fp);
	//fflush(fp);
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			int index = w * i + j;
			unsigned char R = pixels[index * 3];
			unsigned char G = pixels[index * 3 + 1];
			unsigned char B = pixels[index * 3 + 2];
			fwrite(&B, 1, 1, fp);
			fwrite(&G, 1, 1, fp);
			fwrite(&R, 1, 1, fp);
		}
		fwrite(pad, w % 4, 1, fp);
	}
	fclose(fp);
}