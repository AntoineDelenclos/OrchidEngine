#include "CEntity.h"

/// Constructors & Destructors ///
CEntity::CEntity() {
	enumENTType = entity;
	uiENTId = 0;
	strENTName = "##";
	vec3ENTWorldPosition = glm::vec3(0.0f);
	pcENTVertexShaderName = NULL;
	pcENTFragmentShaderName = NULL;
	pgfENTVertices = NULL;
	uiENTVerticesSize = 0;
}

CEntity::CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile) {
	enumENTType = type;
	uiENTId = id;
	switch (type) {
	case entity:
		strENTName = "Entity " + std::to_string(id);
	case face:
		strENTName = "Face " + std::to_string(id);
	case cube:
		strENTName = "Cube " + std::to_string(id);
		pgfENTVertices = (GLfloat*)cubeVertices();
	case polyhedron:
		strENTName = "Polyhedron " + std::to_string(id);
	case light:
		strENTName = "Light " + std::to_string(id);
	case camera:
		strENTName = "Camera " + std::to_string(id);
	}
	vec3ENTWorldPosition = position;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTVerticesSize = sizeof(pgfENTVertices);
}

CEntity::CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, GLfloat* vertices) {
	enumENTType = type;
	uiENTId = id;
	vec3ENTWorldPosition = position;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	pgfENTVertices = vertices;
	uiENTVerticesSize = sizeof(vertices);
}

CEntity::CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number) {
	enumENTType = type;
	uiENTId = id;
	GLfloat* temp_vertices = nullptr;
	if (type == cube) {
		std::cout << "c'est un cube" << std::endl;
		strENTName = "Cube " + std::to_string(id);
		temp_vertices = new (GLfloat[288]);
		for (int i = 0; i < 288; i++) {
			temp_vertices[i] = cubeVertices()[i];
		}
		pgfENTVertices = temp_vertices;
		uiENTVerticesSize = 288;
		//pgfENTVertices = (GLfloat*)cubeVertices();
	}
	if (type == camera) {
		strENTName = "Camera " + std::to_string(id);
	}
	/*switch (type) {
	case entity:
		strENTName = "Entity " + std::to_string(id);
	case face:
		strENTName = "Face " + std::to_string(id);
	case cube:
		std::cout << "c'est un cube" << std::endl;
		strENTName = "Cube " + std::to_string(id);
		temp_vertices = new (GLfloat[288]);
		for (int i = 0; i < 288; i++) {
			temp_vertices[i] = cubeVertices()[i];
		}
		pgfENTVertices = temp_vertices;
		uiENTVerticesSize = 288;
		//pgfENTVertices = (GLfloat*)cubeVertices();
	case polyhedron:
		strENTName = "Polyhedron " + std::to_string(id);
	case light:
		strENTName = "Light " + std::to_string(id);
	case camera:
		strENTName = "Camera " + std::to_string(id);
	}*/
	vec3ENTWorldPosition = position;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTTextureEngineNumber = texture_number;
	/*vec3ENTAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
	vec3ENTDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	vec3ENTSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	fENTShininess = 0.25f;
	fENTTransparency = 1.0f;*/
}

CEntity::CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float transparency) {
	enumENTType = type;
	uiENTId = id;
	GLfloat* temp_vertices = nullptr;
	if (type == cube) {
		std::cout << "c'est un cube" << std::endl;
		strENTName = "Cube " + std::to_string(id);
		temp_vertices = new (GLfloat[288]);
		for (int i = 0; i < 288; i++) {
			temp_vertices[i] = cubeVertices()[i];
		}
		pgfENTVertices = temp_vertices;
		uiENTVerticesSize = 288;
		//pgfENTVertices = (GLfloat*)cubeVertices();
	}
	if (type == camera) {
		strENTName = "Camera " + std::to_string(id);
	}
	/*switch (type) {
	case entity:
		strENTName = "Entity " + std::to_string(id);
	case face:
		strENTName = "Face " + std::to_string(id);
	case cube:
		strENTName = "Cube " + std::to_string(id);
		temp_vertices = new (GLfloat[288]);
		for (int i = 0; i < 288; i++) {
			temp_vertices[i] = cubeVertices()[i];
		}
		pgfENTVertices = temp_vertices;
		uiENTVerticesSize = 288;
		//pgfENTVertices = (GLfloat*)cubeVertices();
	case polyhedron:
		strENTName = "Polyhedron " + std::to_string(id);
	case light:
		strENTName = "Light " + std::to_string(id);
	case camera:
		strENTName = "Camera " + std::to_string(id);
	}*/
	vec3ENTWorldPosition = position;
	pcENTVertexShaderName = vsFile;
	pcENTFragmentShaderName = fragFile;
	uiENTTextureEngineNumber = texture_number;
	//Material values
	/*vec3ENTAmbient = ambient;
	vec3ENTDiffuse = diffuse;
	vec3ENTSpecular = specular;
	fENTShininess = shininess;
	fENTTransparency = transparency;*/
}

CEntity::~CEntity() {

}

/// Getters & Setters ///
unsigned int CEntity::uiENTGetVerticesSize() {
	return uiENTVerticesSize;
}

//Déplace le cube en temps réel
void CEntity::ENTChangeWorldPosition(glm::vec3 new_position) {
	vec3ENTWorldPosition = new_position;
	switch (enumENTType) {
	case cube:
		for (int sommet = 0; sommet < 36; sommet++) {
			for (int axe = 0; axe < 3; axe++) {
				pgfENTVertices[8 * sommet + axe] += new_position[axe];
			}
		}
	//default: ?
	}
}

//Scale la taille de l'entité en temps réel
void CEntity::ENTScaleEntitySize(GLfloat ratio) {
	switch (enumENTType) {
	case cube:
		for (int sommet = 0; sommet < 36; sommet++) {
			for (int axe = 0; axe < 3; axe++) {
				pgfENTVertices[8 * sommet + axe] *= ratio;
			}
		}
	}
}

void CEntity::ENTNormalVectorCalculation() { //Utile pour le calcul de la lumière
	//for (int)
}
