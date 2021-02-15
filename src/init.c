#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "init.h"

GLFWwindow* initGL() {
	// glfw init
	if (!glfwInit()) {
		printf("glfw init failed\n");
		exit(1);
	}

	// create window
	GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL, NULL);
	if (!window) {
		glfwTerminate();
		printf("window creation failed\n");
		exit(1);
	}
	glfwMakeContextCurrent(window);

	// glew init
	glewExperimental = GL_TRUE;
	glewInit();

	return window;
}
