#include "CRender.h"

CRender::CRender() {

}
CRender::~CRender() {

}

void CRender::RDRCreateMandatoryForCube(CEngine& engine, CCube& cube_entity, int number) {
	glGenVertexArrays(1, &engine.puiENGVAOCubesEngine[number]);
	glGenBuffers(1, &engine.puiENGVBOCubesEngine[number]);
	glBindVertexArray(engine.puiENGVAOCubesEngine[number]);
	glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOCubesEngine[number]);
	glBufferData(GL_ARRAY_BUFFER, cube_entity.uiCUBVerticesSize * sizeof(GLfloat) * 4, cube_entity.pgfCUBVertices, GL_STATIC_DRAW);
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	//Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//Logique : 1 pour l'id d'attribution, 2 pour le nombre d'infos par point (cf vertices), 8* car maintenant 8 infos par vertices par sommet
	// et 3* car le 1er élément de texture coords commence au 3eme
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); //Unbind VAO
}

void CRender::RDRCreateMandatoryForLight(CEngine& engine, CLight& light_entity, int number) {
	switch (light_entity.enumLIGType) {
	case(directional):
		glGenVertexArrays(1, &engine.puiENGVAODirectionalLightsEngine[number]);
		glGenBuffers(1, &engine.puiENGVBODirectionalLightsEngine[number]);
		glBindVertexArray(engine.puiENGVAODirectionalLightsEngine[number]);
		glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBODirectionalLightsEngine[number]);
		break;
	case(point):
		glGenVertexArrays(1, &engine.puiENGVAOPointLightsEngine[number]);
		glGenBuffers(1, &engine.puiENGVBOPointLightsEngine[number]);
		glBindVertexArray(engine.puiENGVAOPointLightsEngine[number]);
		glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOPointLightsEngine[number]);
		break;
	case(spot):
		glGenVertexArrays(1, &engine.puiENGVAOSpotLightsEngine[number]);
		glGenBuffers(1, &engine.puiENGVBOSpotLightsEngine[number]);
		glBindVertexArray(engine.puiENGVAOSpotLightsEngine[number]);
		glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOSpotLightsEngine[number]);
		break;
	}
	glBufferData(GL_ARRAY_BUFFER, light_entity.uiLIGVerticesSize * sizeof(GLfloat) * 4, light_entity.pgfLIGVertices, GL_STATIC_DRAW);
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); //Unbind VAO
}

void CRender::RDRCubeVerticesAndTextureRendering(CEngine &engine, CCube &cube_entity, int number) {
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[cube_entity.uiENTTextureEngineNumber].guiTEXGetNumeroTexture());
    glBindVertexArray(engine.puiENGVAOCubesEngine[number]);
    glDrawArrays(GL_TRIANGLES, 0, cube_entity.uiCUBVerticesSize / 8);
    glBindVertexArray(0);
}

//Est-ce qu'il vaudrait mieux pas mettre le contenu actuel de rdrlightrendering dans le render classique ? car ca dépend du matériau et non de la lumiere
void CRender::RDRLightRenderingOnCube(CEngine& engine, CCube& cube_entity) {
	GLint materielAmbientUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.ambient");
	GLint materialDiffuseTextureUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.diffuseTexture");
	GLint materialSpecularTextureUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.specularTexture");
	GLint materialShininessUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.shininess");
	GLint materialTransparencyUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.transparency");
	engine.shaENGCoreShader.SHAUse(); //On utilise le shader avant de passer les uniform car uniform -> dernier shader actif
	glUniform3f(materielAmbientUniformLocation, cube_entity.vec3CUBAmbient.x, cube_entity.vec3CUBAmbient.y, cube_entity.vec3CUBAmbient.z);
	//glUniform3f(materialDiffuseUniformLocation, cube_entity.vec3CUBDiffuse.x, cube_entity.vec3CUBDiffuse.y, cube_entity.vec3CUBDiffuse.z);
	//glUniform3f(materialSpecularUniformLocation, cube_entity.vec3CUBSpecular.x, cube_entity.vec3CUBSpecular.y, cube_entity.vec3CUBSpecular.z);
	glUniform1f(materialShininessUniformLocation, cube_entity.fCUBShininess);
	glUniform1f(materialTransparencyUniformLocation, cube_entity.fCUBTransparency);
	
	glUniform1i(materialDiffuseTextureUniformLocation, 0);
	glUniform1i(materialSpecularTextureUniformLocation, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[cube_entity.uiENTTextureEngineNumber].guiTEXGetNumeroTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[cube_entity.uiCUBSpecularTextureEngineNumber].guiTEXGetNumeroTexture());
}

void CRender::RDRLightVerticesAndTextureRendering(CEngine& engine, CLight& light_entity, int number) {
	GLint lightColorUniformLocation = glGetUniformLocation(engine.shaENGLightShader.Program, "ownLightColor");
	engine.shaENGLightShader.SHAUse();
	glUniform3f(lightColorUniformLocation, light_entity.gfLIGColorLight[0], light_entity.gfLIGColorLight[1], light_entity.gfLIGColorLight[2]);
	//glUniform3f(lightColorUniformLocation, light_entity.vec3LIGColorLight.x, light_entity.vec3LIGColorLight.y, light_entity.vec3LIGColorLight.z);
	//On set les shaders avant de draw
	//std::cout << light_entity.uiLIGId << std::endl;
	glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[light_entity.uiENTTextureEngineNumber].guiTEXGetNumeroTexture());
	switch (light_entity.enumLIGType) {
	case(directional):
		glBindVertexArray(engine.puiENGVAODirectionalLightsEngine[number]);
		break;
	case(point):
		glBindVertexArray(engine.puiENGVAOPointLightsEngine[number]);
		break;
	case(spot):
		glBindVertexArray(engine.puiENGVAOSpotLightsEngine[number]);
		break;
	}
	glDrawArrays(GL_TRIANGLES, 0, light_entity.uiLIGVerticesSize / 5); //On divise la taille par le nombre d'infos par sommet
	glBindVertexArray(0);
}

void CRender::RDRRenderingCubes(CEngine &engine) {
	for (int boucle_cube = 0; boucle_cube < engine.iENGGetNumberOfEntitiesTypeX(cube); boucle_cube++) {
		if (engine.pcubENGCubeEntitiesList[boucle_cube].bENTActive) { //If the cube isn't activated, now needs to calculate light & to render it
			RDRLightRenderingOnCube(engine, engine.pcubENGCubeEntitiesList[boucle_cube]);
			RDRCubeVerticesAndTextureRendering(engine, engine.pcubENGCubeEntitiesList[boucle_cube], boucle_cube);
		}
	}
}

void CRender::RDRRenderingLightCubes(CEngine& engine) {
	for (int boucle_dir_light = 0; boucle_dir_light < engine.iENGGetNumberOfEntitiesTypeX(dir_light); boucle_dir_light++) {
		if (engine.pligENGDirectionalLightsList[boucle_dir_light].bENTActive) {
			RDRLightVerticesAndTextureRendering(engine, engine.pligENGDirectionalLightsList[boucle_dir_light], boucle_dir_light);
		}
	}
	for (int boucle_point_light = 0; boucle_point_light < engine.iENGGetNumberOfEntitiesTypeX(point_light); boucle_point_light++) {
		if (engine.pligENGPointLightsList[boucle_point_light].bENTActive) {
			RDRLightVerticesAndTextureRendering(engine, engine.pligENGPointLightsList[boucle_point_light], boucle_point_light);
		}
	}
	for (int boucle_spot_light = 0; boucle_spot_light < engine.iENGGetNumberOfEntitiesTypeX(spot_light); boucle_spot_light++) {
		if (engine.pligENGSpotLightsList[boucle_spot_light].bENTActive) {
			RDRLightVerticesAndTextureRendering(engine, engine.pligENGSpotLightsList[boucle_spot_light], boucle_spot_light);
		}
	}
}

void CRender::RDRPostProcess(CEngine &engine) {
	//GLint brightnessUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.brightness");
	//GLint contrastUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.contrast");
	//GLint saturationUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.saturation");
	//GLint gammaUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.gamma");
	//GLint grayscale_normLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.grayscale_norm");
	//engine.shaENGCoreShader.SHAUse();
	//if (engine.bENGGetNormRec() == false) {
	//	glUniform3f(grayscale_normLoc, (GLfloat)0.299, (GLfloat)0.587, (GLfloat)0.114);
	//}
	//else if (engine.bENGGetNormRec() == true) {
	//	glUniform3f(grayscale_normLoc, (GLfloat)0.213, (GLfloat)0.715, (GLfloat)0.0722);
	//}

	//glUniform1f(brightnessUniformLocation, engine.gfENGGetBrightness());
	//glUniform1f(contrastUniformLocation, engine.gfENGGetContrast());
	//glUniform1f(saturationUniformLocation, engine.gfENGGetSaturation());
	//glUniform1f(gammaUniformLocation, engine.gfENGGetGamma());
	// 
	// 
	// 
	////New shaders
	/*GLint brightnessUniformLocation_2 = glGetUniformLocation(engine.shaENGPostProcessShader.Program, "post_process.brightness");
	GLint contrastUniformLocation_2 = glGetUniformLocation(engine.shaENGPostProcessShader.Program, "post_process.contrast");
	GLint saturationUniformLocation_2 = glGetUniformLocation(engine.shaENGPostProcessShader.Program, "post_process.saturation");
	GLint gammaUniformLocation_2 = glGetUniformLocation(engine.shaENGPostProcessShader.Program, "post_process.gamma");
	GLint grayscale_normLoc_2 = glGetUniformLocation(engine.shaENGPostProcessShader.Program, "post_process.grayscale_norm");
	engine.shaENGPostProcessShader.SHAUse();
	if (engine.bENGGetNormRec() == false) {
		glUniform3f(grayscale_normLoc_2, (GLfloat)0.299, (GLfloat)0.587, (GLfloat)0.114);
	}
	else if (engine.bENGGetNormRec() == true) {
		glUniform3f(grayscale_normLoc_2, (GLfloat)0.213, (GLfloat)0.715, (GLfloat)0.0722);
	}
	std::cout << "brightness = " << engine.gfENGGetBrightness() << std::endl;

	glUniform1f(brightnessUniformLocation_2, engine.gfENGGetBrightness());
	glUniform1f(contrastUniformLocation_2, engine.gfENGGetContrast());
	glUniform1f(saturationUniformLocation_2, engine.gfENGGetSaturation());
	glUniform1f(gammaUniformLocation_2, engine.gfENGGetGamma());*/


	//Test

	engine.shaENGPostProcessShader.SHAUse();
	glUniform1f(glGetUniformLocation(engine.shaENGPostProcessShader.Program, "ourTexture"), 8);
}
//Check la liste des entités et prévoir une valeur LOD qui dit le rayon à partir de notre position à laquelle on affiche les objets et qu'on les charge dans la pipeline