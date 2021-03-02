#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "shader.h"
#include "fileio.h"


shader shd(char* vertPath, char* fragPath) {
	shader s;
	int success;
	char infoLog[512];
	const char* vertShdSource = readFile("./vertexShader.glsl");
	unsigned int vertShd = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShd, 1, &vertShdSource, NULL);
	glCompileShader(vertShd);
	glGetShaderiv(vertShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShd, 512, NULL, infoLog);
		printf("Error while compiling vertex shader\n%s\n", infoLog);
		exit(1);
	}

	// fake fragment shader to check for errors and print error messages with correct line numbers
/*	const char* fakeShdSource = readFile("./fragmentShader.glsl");
	unsigned int fakeShd = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fakeShd, 1, &fakeShdSource, NULL);
	glCompileShader(fakeShd);
	glGetShaderiv(fakeShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fakeShd, 512, NULL, infoLog);
		printf("Error while compiling fake shader\n%s\n", infoLog);
		exit(1);
	}*/

	// real fragment shader. Errors can be only in SDFs.glsl
	const char* fragShdSource = collectFragShd();
	unsigned int fragShd = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShd, 1, &fragShdSource, NULL);
	glCompileShader(fragShd);
	glGetShaderiv(fragShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShd, 512, NULL, infoLog);
		printf("Error while compiling fragment shader\n%s\n", infoLog);
		exit(1);
	}

	s.ID = glCreateProgram();
	glAttachShader(s.ID, vertShd);
	glAttachShader(s.ID, fragShd);
	glLinkProgram(s.ID);
	glGetProgramiv(s.ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(s.ID, 512, NULL, infoLog);
		printf("Error while linking shaders\n%s\n", infoLog);
		exit(1);
	}
	glDeleteShader(vertShd);
	glDeleteShader(fragShd);
	return s;
}

char* collectFragShd() {
	char* SDFs = readFile("./SDFs.glsl");
	char* main  = readFile("./fragmentShader.glsl");
	char* full = malloc(sizeof(char) * 16384);

	strcpy(full, SDFs);
	strcat(full, "\n");
	strcat(full, main);

	return full;
}

void shdUse(shader* s) {
	glUseProgram(s->ID);
}

// single values
void shdSetInt(shader s, const char* name, int value) { glUniform1i(glGetUniformLocation(s.ID, name), value); }
void shdSetFloat(shader s, const char* name, float value) { glUniform1f(glGetUniformLocation(s.ID, name), value); }
void shdSetIVec2(shader s, const char* name, int x, int y) { glUniform2i(glGetUniformLocation(s.ID, name), x, y); }

// arrays
void shdSetFloatArray(shader s, const char* name, unsigned int count, float* values) { glUniform1fv(glGetUniformLocation(s.ID, name), count, values); }
void shdSetVec3Array (shader s, const char* name, unsigned int count, float* values) { glUniform3fv(glGetUniformLocation(s.ID, name), count, values); }
void shdSetVec4Array (shader s, const char* name, unsigned int count, float* values) { glUniform4fv(glGetUniformLocation(s.ID, name), count, values); }
