#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "shader.h"
#include "debug.h"

#include "vert.h" // generated on build. contain the shader source code string.
#include "frag.h" // -//-


shader shd(char* vertPath, char* fragPath) {
	shader s;
	int success;
	char infoLog[2048];
	const char* vertShdSource = vertFull_glsl;
	unsigned int vertShd = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShd, 1, &vertShdSource, NULL);
	glCompileShader(vertShd);
	glGetShaderiv(vertShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShd, 512, NULL, infoLog);
		printf("Error while compiling vertex shader\n%s\n", infoLog);
		exit(1);
	}
	dprint("GL - SHADER - fragment shader compiled");

	const char* fragShdSource = fragFull_glsl;
	unsigned int fragShd = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShd, 1, &fragShdSource, NULL);
	glCompileShader(fragShd);
	glGetShaderiv(fragShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShd, 512, NULL, infoLog);
		printf("Error while compiling fragment shader\n%s\n", infoLog);
		exit(1);
	}
	dprint("GL - SHADER - vertex shader compiled");

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
	dprint("GL - SHADER - shaders linked");

	glDeleteShader(vertShd);
	glDeleteShader(fragShd);
	return s;
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
