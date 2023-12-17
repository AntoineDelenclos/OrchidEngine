#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread> //Utile depuis C++11 pour utiliser sleep_for() ou sleep_until()
#include <cmath>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLEW_STATIC //car on lie via gl32s.lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class CCamera {
public:
	GLfloat fCAMYaw;
	GLfloat fCAMPitch;
	GLfloat fCAMFovZoom;
	float fCAMCameraSpeedMovement;
	glm::vec3 vec3CAMCameraPosition;
	glm::vec3 vec3CAMCameraTarget;
	glm::vec3 vec3CAMCameraDirection;
	glm::vec3 vec3CAMCameraUp;
	glm::vec3 vec3CAMCameraRight;

	glm::mat4 mat4CAMModel;
	glm::mat4 mat4CAMView;
	glm::mat4 mat4CAMProjection;

	glm::vec3 vec3CAMCameraFront;

	CCamera();
	~CCamera();
};