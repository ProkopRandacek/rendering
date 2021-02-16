#include <GL/glew.h>
#include "shader.h"

typedef struct GLs {
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	GLFWwindow* window;
	shader s;
} GL;

void initOGL();
void exitOGL();
