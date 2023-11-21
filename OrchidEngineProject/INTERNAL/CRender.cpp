#include "CRender.h"

CRender::CRender() {

}
CRender::~CRender() {

}
/*
void CRender::RDRCreateMandatoryForEntity(CEngine& engine, CEntity& entity, int number) {
    glGenVertexArrays(1, &engine.puiENGVAOEngine[number]);
    glGenBuffers(1, &engine.puiENGVBOEngine[number]);
    glBindVertexArray(engine.puiENGVAOEngine[number]);
    glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOEngine[number]);
    glBufferData(GL_ARRAY_BUFFER, entity.uiENTGetVerticesSize() * sizeof(GLfloat) * 4, entity.pgfENTVertices, GL_STATIC_DRAW);
	//Position
	if (entity.enumENTType == cube) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		//Texture coordinates
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//Logique : 2 pour l'id d'attribution, 2 pour le nombre d'infos par point (cf vertices), 8* car maintenant 8 infos par vertices par sommet
		// et 3* car le 1er élément de texture coords commence au 3eme
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}
	if (entity.enumENTType == light) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
    glBindVertexArray(0); //Unbind VAO
}
*/

void CRender::RDRCreateMandatoryForCube(CEngine& engine, CCube& cube_entity, int number) {
	glGenVertexArrays(1, &engine.puiENGVAOCubesEngine[number]);
	glGenBuffers(1, &engine.puiENGVBOCubesEngine[number]);
	glBindVertexArray(engine.puiENGVAOCubesEngine[number]);
	glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOCubesEngine[number]);
	//glBufferData(GL_ARRAY_BUFFER, cube_entity.uiENTGetVerticesSize() * sizeof(GLfloat) * 4, cube_entity.pgfCUBVertices, GL_STATIC_DRAW);
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
	glGenVertexArrays(1, &engine.puiENGVAOLightsEngine[number]);
	glGenBuffers(1, &engine.puiENGVBOLightsEngine[number]);
	glBindVertexArray(engine.puiENGVAOLightsEngine[number]);
	glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOLightsEngine[number]);
	glBufferData(GL_ARRAY_BUFFER, light_entity.uiLIGVerticesSize * sizeof(GLfloat) * 4, light_entity.pgfLIGVertices, GL_STATIC_DRAW);
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); //Unbind VAO
}

void CRender::RDRCubeVerticesAndTextureRendering(CEngine &engine, CCube &cube_entity, int number) {
	//glActiveTexture(GL_TEXTURE0 + entity.uiENTTextureEngineNumber);
    glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[cube_entity.uiENTTextureEngineNumber].guiTEXGetNumeroTexture());
    glBindVertexArray(engine.puiENGVAOCubesEngine[number]);
    glDrawArrays(GL_TRIANGLES, 0, cube_entity.uiCUBVerticesSize / 8);
    glBindVertexArray(0);
}

void CRender::RDRLightVerticesAndTextureRendering(CEngine& engine, CLight& light_entity, int number) {
	GLint lightColorUniformLocation = glGetUniformLocation(engine.shaENGLightShader.Program, "ownLightColor");
	engine.shaENGLightShader.SHAUse();
	glUniform3f(lightColorUniformLocation, light_entity.gfLIGColorLight[0], light_entity.gfLIGColorLight[1], light_entity.gfLIGColorLight[2]);
	//glUniform3f(lightColorUniformLocation, light_entity.vec3LIGColorLight.x, light_entity.vec3LIGColorLight.y, light_entity.vec3LIGColorLight.z);
	//On set les shaders avant de draw

	glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[light_entity.uiENTTextureEngineNumber].guiTEXGetNumeroTexture());
	glBindVertexArray(engine.puiENGVAOLightsEngine[number]);
	glDrawArrays(GL_TRIANGLES, 0, light_entity.uiLIGVerticesSize / 5); //On divise la taille par le nombre d'infos par sommet
	glBindVertexArray(0);
}

//Est-ce qu'il vaudrait mieux pas mettre le contenu actuel de rdrlightrendering dans le render classique ? car ca dépend du matériau et non de la lumiere
void CRender::RDRLightRenderingOnCube(CEngine& engine, CCube& cube_entity) {
	GLint materielAmbientUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.ambient");
	GLint materialDiffuseUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.diffuse");
	GLint materialSpecularUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.specular");
	GLint materialShininessUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.shininess");
	GLint materialTransparencyUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.transparency");
	engine.shaENGCoreShader.SHAUse(); //On utilise le shader avant de passer les uniform car uniform -> dernier shader actif
	glUniform3f(materielAmbientUniformLocation, cube_entity.vec3CUBAmbient.x, cube_entity.vec3CUBAmbient.y, cube_entity.vec3CUBAmbient.z);
	glUniform3f(materialDiffuseUniformLocation, cube_entity.vec3CUBDiffuse.x, cube_entity.vec3CUBDiffuse.y, cube_entity.vec3CUBDiffuse.z);
	glUniform3f(materialSpecularUniformLocation, cube_entity.vec3CUBSpecular.x, cube_entity.vec3CUBSpecular.y, cube_entity.vec3CUBSpecular.z);
	glUniform1f(materialShininessUniformLocation, cube_entity.fCUBShininess);
	glUniform1f(materialTransparencyUniformLocation, cube_entity.fCUBTransparency);
}

void CRender::RDRRenderingCubes(CEngine &engine) {
	for (int boucle_cube = 0; boucle_cube < engine.iENGGetNumberOfEntitiesTypeX(cube); boucle_cube++) {
		RDRLightRenderingOnCube(engine, engine.pcubENGCubeEntitiesList[boucle_cube]);
		RDRCubeVerticesAndTextureRendering(engine, engine.pcubENGCubeEntitiesList[boucle_cube], boucle_cube);
	}
}

void CRender::RDRRenderingLightCubes(CEngine& engine) {
	for (int boucle_light = 0; boucle_light < engine.iENGGetNumberOfEntitiesTypeX(light); boucle_light++) {
		RDRLightVerticesAndTextureRendering(engine, engine.pligENGLightEntitiesList[boucle_light], boucle_light);
	}
}

/*void CRender::RDRRenderingEntities(CEngine& engine) {
	for (int boucle_type = 0; boucle_type < TYPES_OF_ENTITIES; boucle_type++) {
		if (boucle_type == cube) {
			for (int boucle_cube = 0; boucle_cube < engine.iENGGetNumberOfEntitiesTypeX(cube); boucle_cube++) {
				//std::cout << "cube" << std::endl;
				//engine.ENGLightUpdate(); A ne pas mettre
				//RDREntityVerticesAndTextureRendering(engine, engine.pcubENGCubeEntitiesList[boucle_cube], engine.pcubENGCubeEntitiesList[boucle_cube].uiENTId);
				RDRLightRenderingOnCube(engine, engine.pcubENGCubeEntitiesList[boucle_cube]);
			}
		}
		if (boucle_type == light) {
			for (int boucle_light = 0; boucle_light < engine.iENGGetNumberOfEntitiesTypeX(light); boucle_light++) {
			}
		}
	}
	//std::cout << "next frame" << std::endl;
}*/

void CRender::RDRPostProcess(CEngine &engine) {
	GLint brightnessUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.brightness");
	GLint contrastUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.contrast");
	GLint saturationUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.saturation");
	GLint gammaUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.gamma");
	GLint grayscale_normLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.grayscale_norm");
	engine.shaENGCoreShader.SHAUse();
	if (engine.bENGGetNormRec() == false) {
		glUniform3f(grayscale_normLoc, 0.299, 0.587, 0.114);
	}
	else if (engine.bENGGetNormRec() == true) {
		glUniform3f(grayscale_normLoc, 0.213, 0.715, 0.0722);
	}

	glUniform1f(brightnessUniformLocation, engine.gfENGGetBrightness());
	glUniform1f(contrastUniformLocation, engine.gfENGGetContrast());
	glUniform1f(saturationUniformLocation, engine.gfENGGetSaturation());
	glUniform1f(gammaUniformLocation, engine.gfENGGetGamma());
}
//Check la liste des entités et prévoir une valeur LOD qui dit le rayon à partir de notre position à laquelle on affiche les objets et qu'on les charge dans la pipeline