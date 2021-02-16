#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "shader.h"

shader shd(char* vertPath, char* fragPath) {
	shader s;
	int success;
	char infoLog[512];


	// vertex shader
	const char* vertShdSource = readFile("./vertexShader.glsl");
	unsigned int vertShd = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShd, 1, &vertShdSource, NULL);
	glCompileShader(vertShd);
	// Check error
	glGetShaderiv(vertShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShd, 512, NULL, infoLog);
		printf("Error while compiling vertex shader\n%s\n", infoLog);
	}


	// fragment shader
	const char* fragShdSource = readFile("./fragmentShader.glsl");
	unsigned int fragShd = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShd, 1, &fragShdSource, NULL);
	glCompileShader(fragShd);
	// Check error
	glGetShaderiv(fragShd, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShd, 512, NULL, infoLog);
		printf("Error while compiling fragmen shader\n%s\n", infoLog);
	}


	// shader program
	s.ID = glCreateProgram();
	glAttachShader(s.ID, vertShd);
	glAttachShader(s.ID, fragShd);
	glLinkProgram(s.ID);
	// Check error
	glGetProgramiv(s.ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(s.ID, 512, NULL, infoLog);
		printf("Error while linking shaders\n%s\n", infoLog);
	}


	glDeleteShader(vertShd);
	glDeleteShader(fragShd);


	return s;
}

void shdUse(shader* s) {
	glUseProgram(s->ID);
}

void shdSetInt(shader s, const char* name, int value) {
	glUniform1i(glGetUniformLocation(s.ID, name), value);
}

void shdSetFloat(shader s, const char* name, float value) {
	glUniform1f(glGetUniformLocation(s.ID, name), value);
}

void shdSetIVec2(shader s, const char* name, int x, int y) {
	glUniform2i(glGetUniformLocation(s.ID, name), x, y);
}

char* readFile(char* filename) {
        FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

        char c, buffer[4096];
        unsigned int len = 0;

        while((c = fgetc(fp)) != EOF) { // read file byte by byte until EOF
                buffer[len] = c;
                len++;
        }
        fclose(fp);

        char* data = malloc(sizeof(char) * len); // allocate right ammount of memory and copy the content there
        memcpy(data, buffer, len);
	data[len] = 0;

        return data;
}
