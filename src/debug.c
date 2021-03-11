#include <stdio.h>
#include <time.h>

#include "debug.h"
#include "opengl.h"

extern GL* gl;

clock_t s;
int enabled = 1;

float t() { return ((float)(clock() - s) / CLOCKS_PER_SEC * 1000.0f); }
void ddisable() { enabled = 0; }
void startTime() { s = clock(); }
void dprint(const char* text) { if (enabled) printf("%8.2f -- %s\n", t(), text); }
void eprint(const char* text) {
	printf("%8.2f -- ERROR: %s\n", t(), text);
	glfwSetWindowShouldClose(gl->window, GL_TRUE);
}
