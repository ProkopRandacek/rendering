#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "camera.h"
#include "render.h"
#include "main.h"

#define RAY_MAX_LEN 50.0f
#define FRAMES 2
// how many threads to use to parallel animation rendering
#define IMG_THREADS 1 // for >1 its glitching idk FIXME scene is changing while rays are being cast
// how many threads to use to parallel image rendering
#define LINE_THREADS 12

Camera cam;

extern Color shadow, sky, ground1, ground2, black, green, cyan, orange, yellow; // Colors from color.c

Vector3 ls1, ls2; // Animation stuff needs to be accessible for image threads to read

int lineBlockSize = (H / LINE_THREADS) + 1;
int frameBlockSize = (FRAMES / IMG_THREADS) + 1;

char progress[FRAMES] = {'.'};

int main() {
	cInit(); // fill colors with values

	cam = cmr(v3(0.0f, 5.0f, -10.0f), v3(0.0f, -2.0f, 5.0f), 0.0f, 0.2f, 0.2f);

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

void updateProgress(int i) {
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
		// Calculate scene
		Scene *s = malloc(sizeof(Scene));
		s->cam = cam;
		s->sky = sky;
		s->shadow = shadow;

		// The spheres in the scene
		s->s[0] = sph(v3(2.5f, 2.0f, 0.5f), green, 1.0f);
		s->s[1] = sph(v3(0.0f, 0.5f, 0.0f), orange, 0.2f);
		s->s[2] = sph(v3(-1.0f, 2.0f, -1.0f), cyan, 0.7f);

		// Update animated objects
		s->light = vLerp(ls1, ls2, f/(float)FRAMES);

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
}

void *renderLine(void *param) {
	rld* d = param;
	int from = d->i * lineBlockSize;
	int to = (d->i + 1) * lineBlockSize - 1;
	to = MAX(to, H);

	Vector3 lpoint, rpoint, point;
	for (int y = from; y <= to; y++) {
		lpoint = vLerp(cam.tl, cam.bl, y / (float)H);
		rpoint = vLerp(cam.tr, cam.br, y / (float)H);

		for (int x = 0; x < W; x++) {
			point = vLerp(lpoint, rpoint, (float)x / (float)W);
			Ray ray;
			ray.pos = cam.pos;
			ray.dir = vNorm(vDir(cam.pos, point));

			rayHit hit = castRay(ray, RAY_MAX_LEN, d->s);

			if (!hit.hit) { //hit nothing
				putpixel(x, y, sky);
			}
			else { //hit something
				ray.pos = hit.pos;
				ray.dir = vDir(hit.pos, d->s->light);
				float len = vMag(ray.dir);
				ray.dir = vNorm(ray.dir);

				ray.pos = moveRay(ray, COLLISION_THRESHOLD * 1000); // move the ray outside the collision threshold

				rayHit shad = castRay(ray, len, d->s); // cast ray towards the light to see if the point is in shadow;

				Color surfaceColor;

				if (hit.index == -1) { // if I hit the ground;
					int sx = (int)hit.pos.x;
					int sz = (int)hit.pos.z;

					if (((sz + sx) % 2) == 0) {
						surfaceColor = ground1;
					}
					else {
						surfaceColor = ground2;
					}
				}
				else { // hit a ball
					// Reflect the ray
					ray.pos = hit.pos;
					ray.pos = moveRay(ray, COLLISION_THRESHOLD * 1000); // move the ray outside the collision threshold
					Vector3 reflectionPlane = vDir(d->s->s[hit.index].pos, hit.pos);
					ray.dir = vReflect(ray.dir, reflectionPlane);
					struct castRayData ref = castRay(ray, RAY_MAX_LEN, d->s);

					if (!ref.hit) {
						//surfaceColor = TODO reflection
					}

					surfaceColor = d->s->s[hit.index].color;
				}

				if (shad.hit) { // the surface is in shadow
					surfaceColor = cDarker(surfaceColor, 0.2f);
				}
				putpixel(x, y, surfaceColor);
			}
		}
	}
	return NULL;
}
