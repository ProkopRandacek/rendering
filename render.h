#define W 500
#define H 500

typedef struct Colors {
	int r, g, b;
} Color;

void init();
void clearscreen();
void putpixel(int x, int y, Color color);
void drawline(int x0, int y0, int x1, int y, Color color);
void writeBMP(const char* filename);
