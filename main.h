#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef struct RenderLineData { // is given to line rendering threads
	Scene *s;
	int i; // defines what lines to render
} rld;

void *renderImage(void *param);
void *renderLine(void *param);
