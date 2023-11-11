#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread> //Utile depuis C++11 pour utiliser sleep_for() ou sleep_until()
#include <cmath>
#include <fstream>
#include <map>

#define GLEW_STATIC //car on lie via gl32s.lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <C:\Users\antoi\OneDrive\Bureau\Projet J\Code\OpenGLProject1\OpenGLProject1\Shader.h>

//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "GlobalTools.h"
#include "CException.h"
#include "CParser.h"
#include "CCamera.h"


class CInputs {
private:
	glm::mat4 mat4INPMovement;
	std::map<std::string, int> mapINPInputsList; //Map qui va permettre de bind des actions à différentes touches et de les gérer dans leur propre classe
	//Il faudrai charger les inputs via un fichier texte (sérialisation)
	int iINPInputsState;
	std::string strINPKeyBindsPathFile;
	std::string strINPKeyBindsTempPathFile;
	std::string strINPKeyFunctionNameToModify;
	int iINPCountTrueInputFile;
public:
	CCamera camINPChosenCamera;
	int iINPCameraState;
	float fINPLastMouseX;
	float fINPLastMouseY;
	float fINPSensitivity;
	bool bINPFirstMouse;
	CInputs();
	CInputs(CCamera &camera);
	~CInputs();
	void INPKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void INPMouseCallback(GLFWwindow* window, double xpos, double ypos);
	void INPScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void INPSetMovement(glm::mat4 movement);
	void INPSetKeyBindsPathFile(std::string path);
	void INPSetKeyBindsTempPathFile(std::string path);
	void INPSetKeyFunctionNameToModify(std::string name);
	void INPSetStateInputs(int state);
	void INPSetCountTrueInputFile(int count);
	glm::mat4 mat4INPGetMovement();
	std::string strINPGetKeyBindsPathFile();
	std::string strINPGetKeyBindsTempPathFile();
	std::string strINPGetKeyFunctionNameToModify();
	int iINPGetInputsState();
	int iINPGetCountTrueInputFile();

	void INPLoadNewMapKeyBinds(std::string FunctionName, int Key);
	void INPNewBinding(std::string FunctionName, int Key);
	void INPReplaceWithGoodFile();

	void INPInputsFromFile();

	void processInputs(GLFWwindow* window);
};