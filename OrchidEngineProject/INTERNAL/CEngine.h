#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread> //Utile depuis C++11 pour utiliser sleep_for() ou sleep_until()
#include <cmath>
#include <vector>

//IMGUI Docking branch V1.90
#include "../EXTERNAL/IMGUI/imgui.h"
#include "../EXTERNAL/IMGUI/imgui_impl_glfw.h"
#include "../EXTERNAL/IMGUI/imgui_impl_opengl3.h"

//IMPLOT
#include "../EXTERNAL/IMPLOT/implot.h"
#include "../EXTERNAL/IMPLOT/implot_internal.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#define GLEW_STATIC //car on lie via gl32s.lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GlobalTools.h"
#include "CException.h"
#include "Entities/CEntity.h"
#include "CShader.h"
#include "CCamera.h"
#include "CInputs.h"
#include "Entities/CCube.h"
#include "Entities/CLight.h"

#define TYPES_OF_ENTITIES 8 //2 : directional, 3 : point, 4 : spot

class CTexture;

class CEngine {
public:
	int iENGFpsLimiter;
	//Post-processing
	GLfloat gfENGBrightness;
	GLfloat gfENGContrast;
	GLfloat gfENGSaturation;
	GLfloat gfENGGamma;
	bool bENGNormeRec_709;
	bool bENGHasFocus;

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
	int piENGNumberOfEntities[TYPES_OF_ENTITIES]; //A incrémenter et décrémenter en temps réel
	unsigned int* puiENGNextFreeEntitiesIDs; //The next free id for each type of entity
	unsigned int uiENGNextFreeGlobalID;
	GLuint uiENGMaxNumberEntities;
	CEntity** ppentENGAllEntitiesList;
	CCube* pcubENGCubeEntitiesList; //A utiliser comme pour ptexENGAllTextures, un CEntity* peut etre mieux qu'un CEntity**
	CLight* pligENGLightEntitiesList;

	//Lights
	int iENGNumberDirectionalLights; int iENGNumberActiveDirectionalLights;
	int iENGNumberPointLights; /*Nombre de PointLight*/ int iENGNumberActivePointLights;//Nombre de PointLight prisent en compte dans le rendu
	int iENGNumberSpotLights; int iENGNumberActiveSpotLights;
	int* piENGDirectionalLightsID;
	int* piENGPointLightsID;
	int* piENGSpotLightsID;
	CLight* pligENGDirectionalLightsList;
	CLight* pligENGPointLightsList;
	CLight* pligENGSpotLightsList;

	//For interface
	std::map<std::string, int>mapStrIntENGNumberOfEachEntities;

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
	GLuint* puiENGVAOCubesEngine;
	GLuint* puiENGVBOCubesEngine;
	GLuint* puiENGVAODirectionalLightsEngine;
	GLuint* puiENGVBODirectionalLightsEngine;
	GLuint* puiENGVAOPointLightsEngine;
	GLuint* puiENGVBOPointLightsEngine;
	GLuint* puiENGVAOSpotLightsEngine;
	GLuint* puiENGVBOSpotLightsEngine;

	CEngine();
	~CEngine();

	//Getters & Setters
	void ENGSetFpsLimit(int limit);
	int iENGGetFpsLimit();
	void ENGSetNumberOfEntities(int type_of_entity, int nb_of_ent);
	void ENGIncreaseNumberOfEntities(int type_of_entity, int value_of_inc);
	int iENGGetNumberOfEntitiesTypeX(int type_of_entity);
	int iENGGetTotalNumberOfEntities();
	void ENGSetNextFreeEntityID(int type_of_entity, unsigned int next_id);
	void ENGIncrementNextFreeEntityID(int type_of_entity, int value_of_inc);
	unsigned int uiENGGetNextFreeEntityID(int type_of_entity);
	unsigned int uiENGGetNextFreeGlobalID();
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
	void ENGAddCubeEntity(CCube cube);
	void ENGAddLightEntity(CLight light);
	void ENGRemoveCubeEntity(CCube &cube);
	//unsigned int* puiENGEntitiesIdInTheLODArea();

	//Textures related
	//void ENGAddTextureToAllTexturesList(CTexture& texture);
	void ENGAddTextureToAllTexturesList(CTexture texture);

	//Inputs related
	//Usefull to pass difftime of the last frame to the engine's inputs
	void ENGPreUpdateInputsValues();
};

/////////////////////////// CTexture /////////////////////////

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