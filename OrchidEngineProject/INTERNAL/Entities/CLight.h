#pragma once
#include "CEntity.h"

enum light_type_enum { sun };

class CLight : public CEntity {
public:
	light_type_enum enumLIGType;
	glm::vec3 vec3LIGColorLight;
	CLight();
	CLight(entity_type_enum type, unsigned int id, glm::vec3 position, GLfloat* light_color, const char* vsFile, const char* fragFile, int texture_number) {
		enumENTType = type;
		uiENTId = id;
		GLfloat* temp_vertices = nullptr;
		strENTName = "Light " + std::to_string(id);
		temp_vertices = new (GLfloat[288]);
		for (int i = 0; i < 288; i++) {
			temp_vertices[i] = cubeVertices()[i];
		}
		pgfENTVertices = temp_vertices;
		uiENTVerticesSize = 288;
		vec3ENTWorldPosition = position;
		vec3LIGColorLight.x = light_color[0];
		vec3LIGColorLight.y = light_color[1];
		vec3LIGColorLight.z  = light_color[2];
		pcENTVertexShaderName = vsFile;
		pcENTFragmentShaderName = fragFile;
		uiENTTextureEngineNumber = texture_number;
	}
	CLight(CEntity entity);
	~CLight();
};
