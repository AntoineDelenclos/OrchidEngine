#include "CCube.h"

/// Constructors & Destructors ///
CCube::CCube() {
	enumENTType = entity;
	uiENTId = 0;
	strENTName = "##";
	vec3ENTWorldPosition = glm::vec3(0.0f);
	pcENTVertexShaderName = NULL;
	pcENTFragmentShaderName = NULL;
	pgfENTVertices = NULL;
	uiENTVerticesSize = 0;
}

CCube::CCube(unsigned int id_global, unsigned int id_cube, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number) {
	enumENTType = cube;
	uiENTId = id_global;
	uiCUBId = id_cube;
	strENTName = "Cube " + std::to_string(id_cube);
	GLfloat* temp_vertices = nullptr;
	temp_vertices = new (GLfloat[288]);
	for (int i = 0; i < 288; i++) {
		temp_vertices[i] = cubeVertices()[i];
	}
	pgfENTVertices = temp_vertices;
	uiENTVerticesSize = 288;
	vec3ENTWorldPosition = position;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTTextureEngineNumber = texture_number;
	//Material values
	vec3ENTAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
	vec3ENTDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	vec3ENTSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	fENTShininess = 0.25f;
	fENTTransparency = 1.0f;
}

CCube::CCube(unsigned int id_global, unsigned int id_cube, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float transparency) {
	enumENTType = cube;
	uiENTId = id_global;
	uiCUBId = id_cube;
	GLfloat* temp_vertices = nullptr;
	strENTName = "Cube " + std::to_string(id_cube);
	temp_vertices = new (GLfloat[288]);
	for (int i = 0; i < 288; i++) {
		temp_vertices[i] = cubeVertices()[i];
	}
	pgfENTVertices = temp_vertices;
	uiENTVerticesSize = 288;
	vec3ENTWorldPosition = position;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTTextureEngineNumber = texture_number;
	//Material values
	vec3CUBAmbient = ambient;
	vec3CUBDiffuse = diffuse;
	vec3CUBSpecular = specular;
	fCUBShininess = shininess;
	fCUBTransparency = transparency;
}

CCube::~CCube() {

}

/// Getters & Setters ///
unsigned int CCube::uiCUBGetVerticesSize() {
	return uiCUBVerticesSize;
}

void CCube::CUBNormalVectorCalculation() { //Utile pour le calcul de la lumière
	//for (int)
}

void CCube::CUBDisplayNormalVectors() {

}
