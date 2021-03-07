#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opengl.h"
#include "debug.h"

#include "time.h"


int w = 1000;
int h = 1000;

GL* gl;
char* pixels;
unsigned int frameCount = 0;

void initOGL() {
	dprint("GL START");
	gl = malloc(sizeof(GL));
	pixels = malloc(w * h * 3);

	// glfw init
	if (!glfwInit()) {
		printf("glfw init failed\n");
		exit(1);
	}
	dprint("GL - glfw init");

	// create window
	gl->window = glfwCreateWindow(w, h, "Title", NULL, NULL);
	if (!gl->window) {
		glfwTerminate();
		printf("window creation failed\n");
		exit(1);
	}

	dprint("GL - window init");

	glViewport(10, 10, w, h);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glfwMakeContextCurrent(gl->window);

	dprint("GL - window setup");

	glewExperimental = GL_TRUE;
	glewInit();

	// triangles
	float vertices[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &gl->VAO);
	glGenBuffers(1, &gl->VBO);
	glGenBuffers(1, &gl->EBO);

	glBindVertexArray(gl->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, gl->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	dprint("GL - buffers done");

	// create shader
	gl->s = shd("./vertexShader.glsl", "./fragmentShader.glsl");
	shdUse(&gl->s);

	dprint("GL - shaders done");

	glBindVertexArray(gl->VAO);

	dprint("GL DONE");
}

void renderOGL() {
	shdSetFloat(gl->s, "time", glfwGetTime());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// swap buffers
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
}

void screenshot() {
	frameCount++;

	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	char fname[15];
	sprintf(fname, "%04d", frameCount);
	strcat(fname, ".bmp");

	printf("taking a screenshot \"%s\", %dx%d\n", fname, w, h);

	writeBMP(fname, pixels, w, h);
}

void exitOGL() {
	free(gl);
	glDeleteVertexArrays(1, &gl->VAO);
	glDeleteBuffers(1, &gl->VBO);
	glDeleteBuffers(1, &gl->EBO);
	glfwTerminate();
}
