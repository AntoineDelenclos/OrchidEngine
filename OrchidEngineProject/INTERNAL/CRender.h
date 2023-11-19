#pragma once
#include "CEngine.h"

class CRender {
private:
	//CEngine* pengRDRRenderedEngine;
public:
	//void RDRSetRenderedEngine(CEngine* engine);
	//CEngine* pengRDRGetRenderedEngine();

	CRender();
	//CRender(CEngine* engine);
	~CRender();

	void RDRCreateMandatoryForEntity(CEngine& engine, CEntity& entity, int number);
	void RDRCreateMandatoryForCube(CEngine& engine, CCube& cube_entity, int number);
	void RDRCreateMandatoryForLight(CEngine& engine, CLight& light_entity, int number);

	void RDRCubeVerticesAndTextureRendering(CEngine &engine, CCube &cube_entity, int number);
	void RDRLightVerticesAndTextureRendering(CEngine& engine, CLight& light_entity, int number);
	void RDRLightRenderingOnCube(CEngine& engine, CCube& cube_entity);
	void RDRRenderingEntities(CEngine &engine);
	void RDRPostProcess(CEngine &engine);

	void RDRRenderingCubes(CEngine& engine);
	void RDRRenderingLightCubes(CEngine& engine);
};