#include "CLight.h"

CLight::CLight() {
	enumENTType = light;
	uiENTId = 0;
	strENTName = "##";
	vec3ENTWorldPosition = glm::vec3(0.0f);
	pcENTVertexShaderName = NULL;
	pcENTFragmentShaderName = NULL;
	pgfENTVertices = NULL;
	uiENTVerticesSize = 0;
}

CLight::CLight(unsigned int id_global, unsigned int id_light, glm::vec3 position, GLfloat* light_color, GLfloat ambient, GLfloat diffuse, GLfloat specular, const char* vsFile, const char* fragFile, int texture_number) {
	enumENTType = light;
	uiENTId = id_global;
	uiLIGId = id_light;
	strENTName = "Light " + std::to_string(id_light);
	GLfloat* temp_vertices = nullptr;
	temp_vertices = new (GLfloat[180]);
	for (int i = 0; i < 180; i++) {
		temp_vertices[i] = lightVertices()[i];
	}
	pgfENTVertices = temp_vertices;
	uiENTVerticesSize = 180;
	vec3ENTWorldPosition = position;
	vec3LIGColorLight.x = light_color[0];
	vec3LIGColorLight.y = light_color[1];
	vec3LIGColorLight.z = light_color[2];
	gfLIGAmbientIntensity = ambient;
	gfLIGDiffuseStrength = diffuse;
	gfLIGSpecularStrength = specular;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTTextureEngineNumber = texture_number;
}

CLight::~CLight() {

}