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
	mat4CAMView = glm::mat4(1.f);
	mat4CAMView = glm::lookAt(vec3CAMCameraPosition, vec3CAMCameraPosition+vec3CAMCameraFront, vec3CAMCameraUp);
	mat4CAMProjection = glm::perspective(glm::radians(fCAMFovZoom), (float)1280 / (float)960, 0.1f, 100.f);
}

CCamera::~CCamera() {

}

void CCamera::CAMUpdate() {
	/*float radius = 3.f;
	float camX = static_cast<float>(sin(glfwGetTime()) * radius);
	float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
	vec3CAMCameraPosition = glm::vec3(camX, 0.0f, camZ);
	mat4CAMView = glm::lookAt(vec3CAMCameraPosition, vec3CAMCameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));*/
	mat4CAMView = glm::lookAt(vec3CAMCameraPosition, vec3CAMCameraPosition + vec3CAMCameraFront, vec3CAMCameraUp);
	mat4CAMProjection = glm::perspective(glm::radians(fCAMFovZoom), (float)1280 / (float)960, 0.1f, 100.f);
}