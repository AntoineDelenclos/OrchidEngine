#pragma once
#include "CEntity.h"

//enum light_type_enum { sun };

class CLight : public CEntity {
public:
    unsigned int uiLIGId;
    GLfloat* pgfLIGVertices;
    unsigned int uiLIGVerticesSize;
    GLfloat gfLIGScaleRatio;
	//light_type_enum enumLIGType;
	glm::vec3 vec3LIGColorLight;
    GLfloat gfLIGColorLight[3];
	GLfloat gfLIGAmbientIntensity;
	GLfloat gfLIGDiffuseStrength;
	GLfloat gfLIGSpecularStrength;

	static const GLfloat* lightVertices() {
        static const GLfloat lVer[180] = {
            -0.05f, -0.05f, -0.05f,  0.0f, 0.0f,
            0.05f, -0.05f, -0.05f,  1.0f, 0.0f,
            0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
            0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
            -0.05f,  0.05f, -0.05f,  0.0f, 1.0f,
            -0.05f, -0.05f, -0.05f,  0.0f, 0.0f,

            -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
            0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
            0.05f,  0.05f,  0.05f,  1.0f, 1.0f,
            0.05f,  0.05f,  0.05f,  1.0f, 1.0f,
            -0.05f,  0.05f,  0.05f,  0.0f, 1.0f,
            -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,

            -0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
            -0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
            -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
            -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
            -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
            -0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

            0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
            0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
            0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
            0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
            0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
            0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

            -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
            0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
            0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
            0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
            -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
            -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,

            -0.05f,  0.05f, -0.05f,  0.0f, 1.0f,
            0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
            0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
            0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
            -0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
            -0.05f,  0.05f, -0.05f,  0.0f, 1.0f
        };
        return lVer;
	}

	CLight();
    CLight(unsigned int id_global, unsigned int id_light, glm::vec3 position, GLfloat* light_color, GLfloat ambient, GLfloat diffuse, GLfloat specular, const char* vsFile, const char* fragFile, int texture_number);
    ~CLight();

    void LIGFirstTimeSetVerticesPosition();

    void LIGChangeWorldPosition(glm::vec3 new_position);
    void LIGScaleEntitySize(GLfloat ratio);
};
