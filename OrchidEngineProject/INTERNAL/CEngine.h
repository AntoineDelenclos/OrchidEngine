#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread> //Utile depuis C++11 pour utiliser sleep_for() ou sleep_until()
#include <cmath>
#include <vector>

//IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//IMPLOT
#include <implot.h>
#include <implot_internal.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#define GLEW_STATIC //car on lie via gl32s.lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <C:\Users\antoi\OneDrive\Bureau\Projet J\Code\OpenGLProject1\OpenGLProject1\Shader.h>

#include "GlobalTools.h"
#include "CException.h"
#include "Entities/CEntity.h"
#include "CShader.h"
#include "CCamera.h"
#include "CInputs.h"
#include "Entities/CLight.h"


class CTexture;

class CEngine {
public:
	int iENGFpsLimiter;
	int iENGNumberOfEntities; //A incrémenter et décrémenter en temps réel
	unsigned int* puiENGAvailableEntitiesIDs; //List of all the available entities id
	unsigned int uiENGNextFreeEntityID;
	//Post-processing
	GLfloat gfENGBrightness;
	GLfloat gfENGContrast;
	GLfloat gfENGSaturation;
	GLfloat gfENGGamma;
	bool bENGNormeRec_709;
	
	//Render pipeline
	//Il faudra charger "../../../Assets/nom_texture.extension" Dans pstrENGTexturesPath seul nom_texture.extension sera stocké
	std::string strENGAssetsFolder;
	int iENGMaxNumberOfTextures;//Valeur déprécié dans la dernière version
	std::string* pstrENGTexturesPath; //Est-ce qu'il faudrait pas mieux stocker les GLuint qui sont bind avec GlBindTexture(GL_TEXTURE_2D, texture) où texture est un GLuint
	//CRender* prdrENGRenderPipeline;

//public: //Modifier et mettre en privée ce qui est nécessaire
	GLFWwindow* pwindowENGWindow;
	GLFWmonitor* pmonitorENGMonitor;
	GLuint uiENGWidth, uiENGHeight;
	int iENGScreenWidth, iENGScreenHeight;
	GLfloat pgfENGBackgroundColor[4];
	//Un état en mode moteur, un autre en mode jeu par exemple
	GLuint uiENGState;
	glm::vec3 vec3ENGAbsoluteEnginePosition;

	CInputs inpENGInputs;

	
	//Frames 
	double dENGPrevTime, dENGCrntTime, dENGDiffTime;
	unsigned int uiENGFpsTempCounter;
	GLfloat gfENGFpsCounter, gfENGFrameDelayMS;
	GLfloat pgfENGFpsCounterBuffer[1000]; //On aura donc les infos sur les dernières 1000 frames
	GLfloat pgfENGFrameDelayMSBuffer[1000];
	GLuint iENGFrameNumber; //Numéro de la frame actuelle
	
	//Entities
	GLuint uiENGMaxNumberEntities;
	CEntity** ppentENGAllEntitiesList;
	CEntity* pentENGCubeEntitiesList; //A utiliser comme pour ptexENGAllTextures, un CEntity* peut etre mieux qu'un CEntity**
	CLight* pligENGLightEntitiesList;

	//Test Light
	glm::vec3 vec3ENGTestLightColor;// = glm::vec3(0.89f, 0.66f, 0.53f);
	GLfloat gfENGTestLightAmbientIntensity;// = 0.4f;
	GLfloat gfENGTestLightDiffuseStrength;
	GLfloat gfENGTestLightSpecularStrength;
	glm::vec3 vec3ENGTestLightPosition;// = { 1.2f, 1.0f, 2.0f };

	//Textures
	//List of all textures of the engine project
	GLuint uiENGNumberOfTexturesFile;
	GLuint uiENGMaxNumberOfTextures;
	CTexture* ptexENGAllTextures;

	//Render
	CShader shaENGCoreShader;
	CShader shaENGLightShader;

	int iENGMaxNumberVAO;
	int iENGMaxNumberVBO;
	GLuint* puiENGVAOEngine;
	GLuint* puiENGVBOEngine;

	CEngine();
	~CEngine();

	//Getters & Setters
	void ENGSetFpsLimit(int limit);
	int iENGGetFpsLimit();
	void ENGSetNumberOfEntities(int nb_of_ent);
	void ENGIncreaseNumberOfEntities(int value_of_inc);
	int iENGGetNumberOfEntities();
	void ENGSetNextFreeEntityID(unsigned int next_id);
	void ENGIncrementNextFreeEntityID(int value_of_inc);
	unsigned int uiENGGetNextFreeEntityID();
	void ENGSetBrightness(GLfloat brightness);
	GLfloat gfENGGetBrightness();
	void ENGSetContrast(GLfloat contrast);
	GLfloat gfENGGetContrast();
	void ENGSetSaturation(GLfloat saturation);
	GLfloat gfENGGetSaturation();
	void ENGSetGamma(GLfloat gamma);
	GLfloat gfENGGetGamma();
	void ENGSetNormRec(bool checked);
	bool bENGGetNormRec();
	void ENGSetAssetsFolder(std::string path);
	std::string strENGGetAssetsFolder();
	
	//CRender* rdrENGGetRenderPipeline();

	//Engine pipeline
	void ENGStart();
	void ENGCameraUpdate();
	void ENGFrameUpdate();
	void ENGLightUpdate();
	//void ENGRender();

	void ENGChangeResolution(GLuint width, GLuint height);
	void ENGPostProcess();
	void ENGFpsCounterAndLimiter();

	//Entity related
	void ENGAddCubeEntity(CEntity entity);
	void ENGAddLightEntity(CLight light);
	//unsigned int* puiENGEntitiesIdInTheLODArea();

	//Textures related
	//void ENGAddTextureToAllTexturesList(CTexture& texture);
	void ENGAddTextureToAllTexturesList(CTexture texture);
};

class CTexture {
public:
	GLuint guiTEXNumeroTexture;
	//unsigned char* pucTEXImage;
	int iTEXTextureWidth;
	int iTEXTextureHeight;
	int iTEXNumeroChannels;
	bool bTEXTransparent;
//public:
	std::string strTEXImagePath;
	CTexture();
	CTexture(std::string path, bool transparent);
	~CTexture();

	void TEXSetNumeroTexture(GLuint nr_tex);
	GLuint guiTEXGetNumeroTexture();
	//void TEXSetImage(unsigned char* image);
	//unsigned char* pucTEXGetImage();
	void TEXSetTextureWidth(int width);
	int iTEXGetTextureWidth();
	void TEXSetTextureHeight(int height);
	int iTEXGetTextureHeight();
	void TEXSetNumeroChannels(int nr_chan);
	int iTEXGetNumeroChannels();
	void TEXSetTransparent(bool transparent);
	bool bTEXGetTransparent();

	void TEXBinding(CEngine& engine);
};