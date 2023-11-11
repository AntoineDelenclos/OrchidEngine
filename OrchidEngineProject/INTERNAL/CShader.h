#pragma once
#ifndef CSHADER_H
#define CSHADER_H
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class CShader {
public:
	GLuint Program;

	CShader();
	CShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~CShader();

	void SHAUse();
};


#endif // !CSHADER_H