#include "CLight.h"

CLight::CLight() {
	enumENTType = dir_light;
	uiENTId = 0;
	bENTActive = true;
	strENTName = "##";
	vec3ENTWorldPosition = glm::vec3(0.0f);
	pcENTVertexShaderName = NULL;
	pcENTFragmentShaderName = NULL;
	pgfENTVertices = NULL;
	uiENTVerticesSize = 0;
}

//Directional lights
CLight::CLight(light_type_enum type, unsigned int id_global, unsigned int id_light, glm::vec3 position, glm::vec3 direction, GLfloat* light_color, GLfloat ambient, GLfloat diffuse, GLfloat specular, const char* vsFile, const char* fragFile, int texture_number) {
	enumENTType = dir_light;
	enumLIGType = type; //Doit être égal à directional
	uiENTId = id_global;
	uiLIGId = id_light;
	bENTActive = true;
	strENTName = "Directional Light " + std::to_string(id_light);
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
	vec3LIGDirection = direction;
}

//Point lights
CLight::CLight(light_type_enum type, unsigned int id_global, unsigned int id_light, glm::vec3 position, GLfloat* light_color, float constant, float linear, float quadratic, GLfloat ambient, GLfloat diffuse, GLfloat specular, const char* vsFile, const char* fragFile, int texture_number) {
	enumENTType = point_light;
	enumLIGType = type; //Doit être égal à point
	uiENTId = id_global;
	uiLIGId = id_light;
	bENTActive = true;
	strENTName = "Point Light " + std::to_string(id_light);
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
	fLIGPointKC = constant;
	fLIGPointKL = linear;
	fLIGPointKQ = quadratic;
}

//Spotlights
CLight::CLight(light_type_enum type, unsigned int id_global, unsigned int id_light, glm::vec3 position, glm::vec3 direction, float innercutoff, float outercutoff, GLfloat* light_color, GLfloat ambient, GLfloat diffuse, GLfloat specular, const char* vsFile, const char* fragFile, int texture_number) {
	enumENTType = spot_light;
	enumLIGType = type; //Doit être égal à spot
	uiENTId = id_global;
	uiLIGId = id_light;
	bENTActive = true;
	strENTName = "Spot Light " + std::to_string(id_light);
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
	vec3LIGDirection = direction;
	fLIGInnerCutOff = innercutoff;
	fLIGOuterCutOff = outercutoff;
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

void CLight::LIGChangeLightDirection(glm::vec3 new_direction) {
	vec3LIGDirection = new_direction;
}

void CLight::LIGChangeKConstants(float new_kc, float new_kl, float new_kq) {
	fLIGPointKC = new_kc;
	fLIGPointKL = new_kl;
	fLIGPointKQ = new_kq;
}