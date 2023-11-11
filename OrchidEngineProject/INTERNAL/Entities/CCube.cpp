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

CCube::CCube(unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float transparency) {
	enumENTType = cube;
	uiENTId = id;
	GLfloat* temp_vertices = nullptr;
	std::cout << "c'est un cube" << std::endl;
	strENTName = "Cube " + std::to_string(id);
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
