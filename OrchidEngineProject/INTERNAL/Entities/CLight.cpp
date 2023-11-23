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
	pgfLIGVertices = temp_vertices;
	uiLIGVerticesSize = 180;
	gfLIGScaleRatio = 1.f;
	vec3ENTWorldPosition = position;
	vec3LIGColorLight.x = light_color[0];
	vec3LIGColorLight.y = light_color[1];
	vec3LIGColorLight.z = light_color[2];
	gfLIGColorLight[0] = light_color[0]; gfLIGColorLight[1] = light_color[1]; gfLIGColorLight[2] = light_color[2];
	gfLIGAmbientIntensity = ambient;
	gfLIGDiffuseStrength = diffuse;
	gfLIGSpecularStrength = specular;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTTextureEngineNumber = texture_number;
}


CLight::~CLight() {

}

void CLight::LIGFirstTimeSetVerticesPosition() {
	for (int sommet = 0; sommet < 36; sommet++) {
		for (int axe = 0; axe < 3; axe++) {
			pgfLIGVertices[5 * sommet + axe] += vec3ENTWorldPosition[axe] * gfLIGScaleRatio;
		}
	}
}

void CLight::LIGChangeWorldPosition(glm::vec3 new_position) {
	for (int sommet = 0; sommet < 36; sommet++) {
		for (int axe = 0; axe < 3; axe++) {
			pgfLIGVertices[5 * sommet + axe] += (new_position[axe] - vec3ENTWorldPosition[axe]);
		}
	}
	vec3ENTWorldPosition = new_position;
}

void CLight::LIGScaleEntitySize(GLfloat ratio) {
	for (int sommet = 0; sommet < 36; sommet++) {
		for (int axe = 0; axe < 3; axe++) {
			pgfLIGVertices[5 * sommet + axe] = lightVertices()[5 * sommet + axe];
			pgfLIGVertices[5 * sommet + axe] *= ratio;
			pgfLIGVertices[5 * sommet + axe] += vec3ENTWorldPosition[axe];
		}
	}
	gfLIGScaleRatio = ratio;
}