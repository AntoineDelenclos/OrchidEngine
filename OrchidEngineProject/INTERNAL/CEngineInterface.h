#pragma once
#include "CRender.h"
#include "CCamera.h"
#define SIZE_TEXTURE_INTERFACE 100 //Taille de chaque texture dans les modules

//class CTexture;

class CEngineInterface {
public:
	bool bEGIFullscreen;
	bool bEGIWireframeChecked;
	bool bEGIFPSPlotChecked;

	int iEGIFpsLimiter;
	int iEGIWidth;
	int iEGIHeight;
	GLfloat gfEGIBrightness;
	GLfloat gfEGIContrast;
	GLfloat gfEGISaturation;
	GLfloat gfEGIGamma;
	bool bEGINormeRec_709;

	//New entity variables
	bool bEGICreateEntitySubModule;
	GLfloat* pgfEGINewEntityXYZPos;
	unsigned int uiEGINewEntityGlobalID;
	unsigned int uiEGINewEntityTypeID;
	GLfloat gfEGINewEntityScaleRatio;
	int iEGITextureNumber;
	//New entity material values
	glm::vec3 vec3EGINewEntityAmbient;
	glm::vec3 vec3EGINewEntityDiffuse;
	glm::vec3 vec3EGINewEntitySpecular;
	float fEGINewEntityShininess;
	float fEGINewEntityTransparency;

	CRender rdrEGIRender;
	//New light variables
	GLfloat pgfEGINewLightColor[3];
	GLfloat gfEGINewLightAmbientIntensity;

	//Entity modifications via interface
	float fEGINewX, fEGINewY, fEGINewZ;
	GLfloat gfEGINewRatio;
	float fEGINewDirectionX, fEGINewDirectionY, fEGINewDirectionZ;
	float fEGINewKC, fEGINewKL, fEGINewKQ;


	CEngineInterface(CEngine &engine);
	~CEngineInterface();
	bool bEGIIsDisplayed();
	void EGIChangeDisplayState();

	//IMGUI MODULES
	void EGIEngineModule(CEngine &engine);
	void EGIInputsModule(CEngine& engine);
	void EGITexturesModule(CEngine& engine);
	void EGIEntitiesListsModule(CEngine &engine);
	void EGINewEntityModule(CEngine& engine);
	void EGISelectedEntityModule(CEngine& engine);
	void EGICameraModule(CEngine& engine, CCamera& camera);

	void EGIWireframeUpdate();
	void EGIFullscreenUpdate(CEngine &engine);
	void EGIUpdate(CEngine &engine);

	void EGIInterfaceToEngine(CEngine &engine);
};