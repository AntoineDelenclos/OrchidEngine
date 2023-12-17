#include "CShader.h"

CShader::CShader() {

}

CShader::CShader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	std::string strSHAVertexCode;
	std::string strSHAFragmentCode;
	std::ifstream ifsSHAVertexShaderFile;
	std::ifstream ifsSHAFragmentShaderFile;

	ifsSHAVertexShaderFile.exceptions(std::ifstream::badbit);
	ifsSHAFragmentShaderFile.exceptions(std::ifstream::badbit);
	try {
		//Open files
		ifsSHAVertexShaderFile.open(vertexPath);
		ifsSHAFragmentShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//Read file's buffer contents into streams
		vShaderStream << ifsSHAVertexShaderFile.rdbuf();
		fShaderStream << ifsSHAFragmentShaderFile.rdbuf();
		//Convert stream into string
		strSHAVertexCode = vShaderStream.str();
		strSHAFragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = strSHAVertexCode.c_str();
	const GLchar* fShaderCode = strSHAFragmentCode.c_str();
	//Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infolog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
	}
	//Link shaders
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->Program, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infolog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

CShader::~CShader() {

}

void CShader::SHAUse() {
	glUseProgram(this->Program);
}