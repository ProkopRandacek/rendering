#include <stdlib.h>
#include <stdio.h> #include <string.h>
#include <pthread.h>
#include <math.h>

#include "render.h"
#include "vectormath.h"
#include "main.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (X) : (Y))

#define COLLISION_THRESHOLD 0.0001f
#define RAY_MAX_LEN 50.0f
#define GLOW_PER_MOVE 0
#define FRAMES 2
// how many threads to use to parallel animation rendering
#define IMG_THREADS 1 // for >1 its glitching idk FIXME scene is changing while rays are being cast
// how many threads to use to parallel image rendering
#define LINE_THREADS 12
// LINE_THREADS * IMG_THREADS should be equal to cpu core count

Camera cam;

Color shadow;
Color sky;
Color ground1;
Color ground2;

Color black;
Color green;
Color cyan;
Color orange;
Color yellow;

// Animation stuff needs to be accessible for image threads to read
Vector3 ls1;
Vector3 ls2;

Vector3 p1;
Vector3 p2;

Vector3 q1;
Vector3 q2;

Vector3 r1;
Vector3 r2;

int lineBlockSize = (H / LINE_THREADS) + 1;
int frameBlockSize = (FRAMES / IMG_THREADS) + 1;

char progress[FRAMES] = {'.'};

int main() {
	init();

	shadow = clr(128, 128, 128);
	sky = clr(0, 48, 78);
	ground1 = clr(40, 40, 40);
	ground2 = clr(80, 80, 80);

	green = clr(50, 200, 50);
	cyan = clr(0, 255, 200);
	orange = clr(255, 165, 0);
	yellow = clr(255, 255, 0);

	cam = cmr(v3(0.0f, 5.0f, -5.0f), vNorm(v3(0.0f, -1.0f, 5.0f)), v3(1.0f, 0.0f, 1.0f), 5.0f, 5.0f);

	ls1 = v3(10.0, 10.0f, 10.0f); // Define points to animate stuff between
	ls2 = v3(10.0, 10.0f, -10.0f);

	p1 = v3(1.5f, 5.0f, -2.0f);
	p2 = v3(-1.5f, 5.0f, -2.0f);

	q1 = v3(-1.0f, 2.0f, -1.0f);
	q2 = v3(1.5f, 1.5f, 2.0f);

	r1 = v3(1.5f, 1.5f, 2.0f);
	r2 = v3(4.5f, 1.5f, 2.0f);

	memset(progress, '.', FRAMES);
	printf("%s\r", progress);

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
	pthread_exit(NULL);

	return 0;
}

void updateProgress(int i) {
	progress[i] = '#';
	printf("%s\r", progress);
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
		Vector3 ls = vLerp(ls1, ls2, f/(float)FRAMES);
		//s->s[0].pos = vLerp(p1, p2, f/(float)FRAMES);
		//s->s[1].pos = vLerp(q1, q2, f/(float)FRAMES);
		//s->s[2].pos = vLerp(r1, r2, f/(float)FRAMES);

		//s->s[2].radius = f/(float)FRAMES;

		s->light = ls;

		// Start thread
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

			struct castRayData hit = castRay(ray, 50.0f, d->s);

			if (!hit.hit) { //hit nothing
				putpixel(x, y, sky);
			}
			else { //hit something
				ray.pos = hit.pos;
				ray.dir = vDir(hit.pos, d->s->light);
				float len = vMag(ray.dir);
				ray.dir = vNorm(ray.dir);

				ray.pos = moveRay(ray, COLLISION_THRESHOLD * 1000); // move the ray outside the collision threshold

				struct castRayData shad = castRay(ray, len, d->s); // cast ray towards the light to see if the point is in shadow;

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

struct castRayData castRay(Ray ray, float limit, Scene *s) {
	float traveled = 0.0f;
	char glow = 0;
	while (1) {
		float safeDist = 999999.0f;
		int sphIndex = 0;
		for (int i = 0; i < SPHERE_COUNT; i++) { // Nearest sphere
			float newDist = dist2Sphere(ray.pos, s->s[i]);
			if (safeDist > newDist) {
				safeDist = newDist;
				sphIndex = i;
			}
		}

		float dist2Floor = ray.pos.y; // floor is on y = 0 for now

		if (safeDist > dist2Floor) {
			safeDist = dist2Floor;
			sphIndex = -1;
		}


		if (safeDist > COLLISION_THRESHOLD) { // far enough -> move
			glow += GLOW_PER_MOVE;
			traveled += safeDist;
			ray.pos = moveRay(ray, safeDist);
		}
		else { // hit!
			struct castRayData crd;
			crd.hit = 1;
			crd.pos = ray.pos;
			crd.index = sphIndex;
			return crd;
		}
		if (traveled > limit) { // too far
			struct castRayData crd;
			crd.hit = 0;
			crd.pos = ray.pos;
			return crd;
		}
	}
}

Vector3 moveRay(Ray ray, float distance) {
	return vAdd(ray.pos, vMultf(ray.dir, distance));
}

float dist2Sphere(Vector3 v, Sphere s) {
	return vDist(v, s.pos) - s.radius;
}

Color clampColor(Color c) {
	Color o;
	o.r = MAX(MIN(c.r, 255), 0);
	o.g = MAX(MIN(c.g, 255), 0);
	o.b = MAX(MIN(c.b, 255), 0);
	return o;
}

Color cDarker(Color a, float b) { // Stop using Color and use Vector3 to describe color
	return clr(a.r * b, a.g * b, a.b * b);
}

Color clr(int r, int g, int b) {
	Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

Sphere sph(Vector3 pos, Color c, float r) {
	Sphere s;
	s.pos = pos;
	s.color = c;
	s.radius = r;
	return s;
}

Camera cmr(Vector3 pos, Vector3 dir, Vector3 left, float h, float w) {
	Camera c;
	c.pos = pos;

	Vector3 sc = vAdd(pos, dir); // Screen center
	// SIDE VIEW
	//
	// Camera        ...```t <- top points
	// |       ...```      |
	// V ...```            |
	// C- - - dir vec - ->SC <- Screen center point
	//   ```...            |
	//         ```...      |
	//               ```...b <- bottom points

	left = vMultf(vNorm(left), w);
	// CAMERA VIEW
	// tl---------tr // top left, top right
	// |           | // SC = Screen center point
	// |           |
	// <----SC     | // Left vector defines if the camera is tilted sideways TODO input angle and calculate vector from that
	// |           | // Set it to -1, 0, 0 for horizontall camera
	// |           | // modify x, y to modify rotation. Modifying z makes things weird
	// bl---------br // bottom left, bottom right

	Vector3 up = vMultf(vNorm(vCross(left, dir)), h);
	// tl----^----tr picture in camera space
	// |     |     | // SC = Screen center point
	// |     |     |
	// |    SC     |
	// |           |
	// |           |
	// bl---------br

	Vector3 right = vMultf(left, -1.0f);
	Vector3 down = vMultf(up, -1.0f);

	c.tl = vAdd(up, left);
	c.tr = vAdd(up, right);
	c.bl = vAdd(down, left);
	c.br = vAdd(down, right);

	return c;
}
