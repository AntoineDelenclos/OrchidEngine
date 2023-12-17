#include "CCamera.h"

CCamera::CCamera() {
	fCAMYaw = -90.f;
	fCAMPitch = 0.f;
	fCAMFovZoom = 45.f;
	fCAMCameraSpeedMovement = 2.f; //Pour les déplacements de personnage et non pas pour la sensi de caméra
	vec3CAMCameraPosition = glm::vec3{ 0.f,0.f,0.f };
	vec3CAMCameraFront = glm::vec3{ 0.f,0.f,-1.f };
	vec3CAMCameraUp = glm::vec3{ 0.f,1.f,0.f };
	vec3CAMCameraDirection.x = cos(glm::radians(fCAMYaw)) * cos(glm::radians(fCAMPitch));
	vec3CAMCameraDirection.y = sin(glm::radians(fCAMPitch));
	vec3CAMCameraDirection.z = sin(glm::radians(fCAMYaw)) * cos(glm::radians(fCAMPitch));
	//vec3CAMCameraTarget = glm::vec3{ 0.f,0.f,0.f }; Devient pos+front
	//vec3CAMCameraDirection = glm::normalize(vec3CAMCameraPosition - vec3CAMCameraTarget);
	//vec3CAMCameraRight = glm::normalize(glm::cross(glm::vec3{ 0.f,1.f,0.f }, vec3CAMCameraDirection));
	//vec3CAMCameraUp = glm::cross(vec3CAMCameraDirection, vec3CAMCameraRight);
	mat4CAMModel = glm::mat4(1.f);
	//model = glm::rotate(model, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)); //Orthographic projection
		//view = glm::translate(view, glm::vec3(screenWidth / 2, screenHeight / 2, -700.0f)); //Orthographic projection
	mat4CAMModel = glm::rotate(mat4CAMModel, glm::radians(0.0f), glm::vec3(0.5f, 1.0f, 0.0f)); //Perspective projection ROTATION = 0.0f ICI
	//view = glm::translate(view, glm::vec3(0.0f,0.0f,translation)); //Perspective projection
	mat4CAMView = glm::mat4(1.f);
	mat4CAMView = glm::lookAt(vec3CAMCameraPosition, vec3CAMCameraPosition+vec3CAMCameraFront, vec3CAMCameraUp);
	mat4CAMProjection = glm::perspective(glm::radians(fCAMFovZoom), (float)1280 / (float)960, 0.1f, 100.f);
}

CCamera::~CCamera() {

}