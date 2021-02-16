#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl.h"

GL* gl;

void initOGL() {
	gl = malloc(sizeof(GL));

	// glfw init
	if (!glfwInit()) {
		printf("glfw init failed\n");
		exit(1);
	}

	// create window
	gl->window = glfwCreateWindow(800, 800, "Title", NULL, NULL);
	if (!gl->window) {
		glfwTerminate();
		printf("window creation failed\n");
		exit(1);
	}
	glfwMakeContextCurrent(gl->window);

	glewExperimental = GL_TRUE;
	glewInit();

	// triangles
	float vertices[] = {
		1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(gl->VAO);

	// create shader
	gl->s = shd("./vertexShader.glsl", "./fragmentShader.glsl");
	shdUse(&gl->s);

}

void renderGL() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void exitOGL() {
	free(gl);
	glDeleteVertexArrays(1, &gl->VAO);
	glDeleteBuffers(1, &gl->VBO);
	glDeleteBuffers(1, &gl->EBO);
	glfwTerminate();
}
