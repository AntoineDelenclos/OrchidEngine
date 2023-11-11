#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread> //Utile depuis C++11 pour utiliser sleep_for() ou sleep_until()
#include <cmath>
#include <vector>
//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLEW_STATIC //car on lie via gl32s.lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Il faudra rajouter au fur et � mesure (player, model, et peut etre penser � diff�rencier les types de lumiere)
enum entity_type_enum { cube, polyhedron, light, camera, entity, face };

class CEntity {
//private:
public:
	entity_type_enum enumENTType;
	unsigned int uiENTId;
    std::string strENTName;
	//On va translate de cette position apr�s l'avoir dessin� en (0,0,0) pour �viter des soucis avec les transformations matricielles
	glm::vec3 vec3ENTWorldPosition;




	const char* pcENTVertexShaderName; //Nom du fichier, par exemple : "core.vs"
	const char* pcENTFragmentShaderName;
    GLfloat* pgfENTVertices;
    unsigned int uiENTVerticesSize;
    
    //Material values
    glm::vec3 vec3ENTAmbient;
    glm::vec3 vec3ENTDiffuse;
    glm::vec3 vec3ENTSpecular;
    float fENTShininess;
    float fENTTransparency;
    

    //On peut stocker le num�ro de la texture dans l'ensemble des textures ? Et si on fait une liste s�lectionnable si des textures
    // ca devrait fonctionner
    unsigned int uiENTTextureEngineNumber;
    //Peut etre stocker l'adresse du VAO ou bien sa position dans le VAO/VBO (vbo plut�t) et ensuite modifier le vbo avec glBufferSubData par exemple
//public:
    //Les vertices d'un cube de base
    static const GLfloat* cubeVertices() {
        //Par d�faut dans OpenGL les valeurs de l'�cran vont de -1 � 1
        static const GLfloat cVer[288] = { //Perspective projection D�finition d'un cube
            //Positions           //Texture Coords    //Normal
            -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, //All normals goes to zero at initializing
            0.1f, -0.1f, -0.1f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
            0.1f,  0.1f, -0.1f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, //tester avec notre fonction normalizevector
            0.1f,  0.1f, -0.1f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            -0.1f,  0.1f, -0.1f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

            -0.1f, -0.1f,  0.1f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.1f, -0.1f,  0.1f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.1f,  0.1f,  0.1f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            0.1f,  0.1f,  0.1f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            -0.1f,  0.1f,  0.1f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            -0.1f, -0.1f,  0.1f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

            -0.1f,  0.1f,  0.1f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.1f,  0.1f, -0.1f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            -0.1f, -0.1f, -0.1f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            -0.1f, -0.1f, -0.1f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            -0.1f, -0.1f,  0.1f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.1f,  0.1f,  0.1f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

            0.1f,  0.1f,  0.1f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.1f,  0.1f, -0.1f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
            0.1f, -0.1f, -0.1f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
            0.1f, -0.1f, -0.1f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
            0.1f, -0.1f,  0.1f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.1f,  0.1f,  0.1f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

            -0.1f, -0.1f, -0.1f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
            0.1f, -0.1f, -0.1f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
            0.1f, -0.1f,  0.1f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
            0.1f, -0.1f,  0.1f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
            -0.1f, -0.1f,  0.1f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
            -0.1f, -0.1f, -0.1f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,

            -0.1f,  0.1f, -0.1f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
            0.1f,  0.1f, -0.1f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
            0.1f,  0.1f,  0.1f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
            0.1f,  0.1f,  0.1f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
            -0.1f,  0.1f,  0.1f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
            -0.1f,  0.1f, -0.1f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f
        };
        return cVer;
    }

	CEntity();
    CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile);
	CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, GLfloat* vertices);
    //Seul constructeur vraiment utilis� (notamment dans CEngineInterface)
    CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number);
    CEntity(entity_type_enum type, unsigned int id, glm::vec3 position, const char* vsFile, const char* fragFile, int texture_number, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float transparency);
    CEntity(CEntity* entity); //Constructeur de recopie
	//A compl�ter
	~CEntity();
    //Getters & Setters
    unsigned int uiENTGetVerticesSize();


    void ENTNormalVectorCalculation();

	void ENTChangeWorldPosition(glm::vec3 new_position);
    void ENTScaleEntitySize(GLfloat ratio);

	void ENTDisplayNormalVectors();
};