#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "fileio.h"


typedef struct GLs {
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	GLFWwindow* window;
	shader s;
} GL;

// init GL stuff
void initOGL();

// render fragment shader basically
void renderOGL();

// exit GL stuff
void exitOGL();

// take a screenshot and write it to bmp file
void screenshot();
