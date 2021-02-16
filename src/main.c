#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "shader.h"
#include "init.h"

shader s;

int main() {
	// init
	GLFWwindow* window = initGL();

	// callbacks
	glfwSetErrorCallback(onError);
	glfwSetKeyCallback(window, onKey);
	glfwSetFramebufferSizeCallback(window, resize);

	// init gl objects
	unsigned int VBO, VAO, EBO;

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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// create shader
	s = shd("./vertexShader.glsl", "./fragmentShader.glsl");
	shdUse(&s);

	// FPS counting vars
	double previousTime = glfwGetTime();
	int frameCount = 0;

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// update time uniform
		shdSetFloat(s, "time", glfwGetTime());

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// count FPS
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0) {
			printf("FPS: %d\n", frameCount);
			frameCount = 0;
			previousTime = currentTime;
		}
	}

	// exit
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

// glfw callback
void onError(int error, const char* description) {
	printf("Error %d: %s\n", error, description);
}

// glfw callback
void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//printf("%d, %d, %d, %d\n", key, scancode, action, mods);
	if (scancode == 24 && action == 1) glfwDestroyWindow(window); // exit on `q` press
}

// glfw callback
void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	shdSetIVec2(s, "resolution", width, height);
}
