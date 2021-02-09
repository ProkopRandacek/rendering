#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "SDL2/SDL.h"

#include "camera.h"
#include "render.h"
#include "main.h"

#define RAY_MAX_LEN 50.0f
#define FRAMES 2
// how many threads to use to parallel animation rendering
#define IMG_THREADS 1 // for >1 its glitching idk FIXME scene is changing while rays are being cast
// how many threads to use to parallel image rendering
#define LINE_THREADS 2
extern Color shadow, sky, ground1, ground2, black, green, cyan, orange, yellow; // Colors from color.c

Vector3 ls1, ls2; // Animation stuff needs to be accessible for image threads to read

int lineBlockSize = (H / LINE_THREADS) + 1; // the amounts of work to do by a thread
int frameBlockSize = (FRAMES / IMG_THREADS) + 1;

char progress[FRAMES] = {'.'}; // used to visually track progress

int main() {
	cInit(); // fill colors with values

	ls1 = v3(10.0, 10.0f, 10.0f); // Define points to animate stuff between
	ls2 = v3(10.0, 10.0f, -10.0f);

	memset(progress, '.', FRAMES);
	printf("%s\n", progress);

	// Start threads
	pthread_t threads[IMG_THREADS];
	for (long f = 0; f < IMG_THREADS; f++) {
		pthread_create(&threads[f], NULL, renderImage, (void *)f);
	}

	// Wait for threads to finish
	for (int i = 0; i < IMG_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("\ndone\n");
	return 0;
}

void updateProgress(int i) { // updates the visual progress
	progress[i] = '#';
	printf("%s\n", progress);
}

void *renderImage(void *param) {
	long f = (long)param;

	int from = f * frameBlockSize;
	int to = (f + 1) * frameBlockSize - 1;
	to = MAX(to, FRAMES - 1);

	// Render frames
	for (int f = from; f <= to; f++) {
		Scene* s = createScene(f/(float)FRAMES);

		// Start threads
		pthread_t threads[LINE_THREADS];
		void* rs[LINE_THREADS]; // need to have these pointers to free them later
		for (int i = 0; i < LINE_THREADS; i++) {
			rld *r;
			r = malloc(sizeof(rld));
			r->s = s;
			r->i = i;
			rs[i] = r;
			pthread_create(&threads[i], NULL, renderLine, r);
		}

		// Wait for threads to finish
		for (int i = 0; i < LINE_THREADS; i++) {
			pthread_join(threads[i], NULL);
			free(rs[i]);
		}
		free(s);

		// Write image
		char fname[10];
		sprintf(fname, "%04d", f);
		strcat(fname, ".bmp");
		updateProgress(f);
		writeBMP(fname);
	}

	return NULL;
}

Scene* createScene(float time) {
	Scene *s = malloc(sizeof(Scene));
	Vector3 campos = vLerp(v3(2.0f, 4.0f, -10.0f), v3(-2.0f, 4.0f, -10.0), time);
	s->cam = cmr(campos, v3(0.0f, -2.0f, 10.0f), 0.0f, 0.2f, 0.2f);
	s->sky = sky;
	s->shadow = shadow;

	s->s[0] = sph(v3(0.0f, 2.0f, 0.0f), green, 1.0f, 0.0f);
	//s->s[1] = sph(v3(0.0f, 0.5f, 0.0f), orange, 0.2f);
	//s->s[2] = sph(v3(-1.0f, 2.0f, -1.0f), cyan, 0.7f);

	s->light = v3(10.0, 10.0f, -10.0f);
	// Update animated objects
	//s->light = vLerp(ls1, ls2, time);

	return s;
}

void *renderLine(void *param) {
	rld* d = param;
	int from = d->i * lineBlockSize;
	int to = (d->i + 1) * lineBlockSize - 1;
	to = MAX(to, H);

	Vector3 lpoint, rpoint, point;
	for (int y = from; y <= to; y++) {
		lpoint = vLerp(d->s->cam.tl, d->s->cam.bl, y / (float)H);
		rpoint = vLerp(d->s->cam.tr, d->s->cam.br, y / (float)H);

		for (int x = 0; x < W; x++) {
			point = vLerp(lpoint, rpoint, (float)x / (float)W);
			Color pix = renderPixel(point, d->s);
			putpixel(x, y, pix);
		}
	}
	return NULL;
}

Color renderPixel(Vector3 point, Scene* s) {
	Ray ray;
	ray.pos = s->cam.pos;
	ray.dir = vNorm(vDir(s->cam.pos, point));

	rayHit hit = castRay(ray, RAY_MAX_LEN, -1, s);

	if (!hit.hit) { //hit nothing
		return sky;
	} else { //hit something
		Ray light;
		light.pos = hit.pos;
		light.dir = vDir(hit.pos, s->light);
		float len = vMag(light.dir);
		light.dir = vNorm(light.dir);
		light.pos = moveRay(light, COLLISION_THRESHOLD * 1000); // move the ray outside the collision threshold
		rayHit shadow = castRay(light, len, -3, s); // cast ray towards the light to see if the point is in shadow

		Color surfaceColor;

		if (hit.index == -1) { // if I hit the ground;
			surfaceColor = checkerboard(hit.pos);
		} else { // hit a ball
			surfaceColor = s->s[hit.index].color;

			// Reflect the ray
			Vector3 reflectionPlane = vDir(s->s[hit.index].pos, hit.pos);
			ray.dir = vNorm(vReflect(reflectionPlane, ray.dir));
			struct castRayData ref = castRay(ray, RAY_MAX_LEN, hit.index, s);

			if (ref.hit) { // reflection hit something
				Color rClr;
				if (ref.index == -1) { // reflection hit ground
					rClr = checkerboard(ref.pos);
				} else { // reflection hit a sphere
					rClr = s->s[ref.index].color;
				}
				surfaceColor = cBlend(surfaceColor, rClr, s->s[hit.index].refw);
			} else { // reflection hit sky
				surfaceColor = cBlend(surfaceColor, sky, s->s[hit.index].refw);
			}
		}
		if (shadow.hit) { // the surface is in shadow
			surfaceColor = cDarker(surfaceColor, 0.7f);
		}
		return surfaceColor;
	}
}

Color checkerboard(Vector3 pos) {
	if ((((int)pos.x + (int)pos.z) % 2) == 0) {
		return ground1;
	} else {
		return ground2;
	}
}
