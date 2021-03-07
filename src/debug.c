#include <stdio.h>

#include "debug.h"
#include "time.h"

clock_t s;
int enabled = 1;

float t()                { return ((float)(clock() - s) / CLOCKS_PER_SEC * 1000.0f); }
void  ddisable()         { enabled = 0; }
void  startTime()        { s = clock(); }
void  dprint(char* text) { if (enabled) printf("%8.2f -- %s\n", t(), text); }
