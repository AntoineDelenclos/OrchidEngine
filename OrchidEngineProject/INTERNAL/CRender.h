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
	void RDREntityVerticesAndTextureRendering(CEngine &engine, CEntity &entity, int number);
	void RDRLightRenderingOnEntity(CEngine& engine, CEntity& entity);
	void RDRRenderingEntities(CEngine &engine);
	void RDRPostProcess(CEngine &engine);
};