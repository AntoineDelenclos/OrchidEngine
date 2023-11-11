#include "CRender.h"

CRender::CRender() {

}
CRender::~CRender() {

}

void CRender::RDRCreateMandatoryForEntity(CEngine& engine, CEntity& entity, int number) {
    glGenVertexArrays(1, &engine.puiENGVAOEngine[number]);
    glGenBuffers(1, &engine.puiENGVBOEngine[number]);
    glBindVertexArray(engine.puiENGVAOEngine[number]);
    glBindBuffer(GL_ARRAY_BUFFER, engine.puiENGVBOEngine[number]);
    glBufferData(GL_ARRAY_BUFFER, entity.uiENTGetVerticesSize() * sizeof(GLfloat) * 4, entity.pgfENTVertices, GL_STATIC_DRAW);
	//Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
	//Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//Logique : 2 pour l'id d'attribution, 2 pour le nombre d'infos par point (cf vertices), 8* car maintenant 8 infos par vertices par sommet
	// et 3* car le 1er élément de texture coords commence au 3eme
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
    glBindVertexArray(0); //Unbind VAO
}

void CRender::RDREntityVerticesAndTextureRendering(CEngine &engine, CEntity &entity, int number) {
	//Il faut peut-être rajouter un glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, engine.ptexENGAllTextures[entity.uiENTTextureEngineNumber].guiTEXGetNumeroTexture());
    glBindVertexArray(engine.puiENGVAOEngine[number]);
    glDrawArrays(GL_TRIANGLES, 0, entity.uiENTGetVerticesSize() / 8);
    glBindVertexArray(0);
}

//Est-ce qu'il vaudrait mieux pas mettre le contenu actuel de rdrlightrendering dans le render classique ? car ca dépend du matériau et non de la lumiere
void CRender::RDRLightRenderingOnEntity(CEngine& engine, CEntity& entity) {
	GLint materielAmbientUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.ambient");
	GLint materialDiffuseUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.diffuse");
	GLint materialSpecularUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.specular");
	GLint materialShininessUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.shininess");
	GLint materialTransparencyUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "material.transparency");

	glUniform3f(materielAmbientUniformLocation, entity.vec3ENTAmbient.x, entity.vec3ENTAmbient.y, entity.vec3ENTAmbient.z);
	glUniform3f(materialDiffuseUniformLocation, entity.vec3ENTDiffuse.x, entity.vec3ENTDiffuse.y, entity.vec3ENTDiffuse.z);
	glUniform3f(materialSpecularUniformLocation, entity.vec3ENTSpecular.x, entity.vec3ENTSpecular.y, entity.vec3ENTSpecular.z);
	glUniform1f(materialShininessUniformLocation, entity.fENTShininess);
	glUniform1f(materialTransparencyUniformLocation, entity.fENTTransparency);

	engine.shaENGCoreShader.SHAUse();
}

void CRender::RDRRenderingEntities(CEngine& engine) {
	for (int boucle_entity = 0; boucle_entity < engine.iENGGetNumberOfEntities(); boucle_entity++) {
        RDREntityVerticesAndTextureRendering(engine, engine.pentENGCubeEntitiesList[boucle_entity], engine.pentENGCubeEntitiesList[boucle_entity].uiENTId);
		RDRLightRenderingOnEntity(engine, engine.pentENGCubeEntitiesList[boucle_entity]);
	}
}

void CRender::RDRPostProcess(CEngine &engine) {
	GLint brightnessUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.brightness");
	GLint contrastUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.contrast");
	GLint saturationUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.saturation");
	GLint gammaUniformLocation = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.gamma");
	GLint grayscale_normLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "post_process.grayscale_norm");

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

	engine.shaENGCoreShader.SHAUse();
}
//Check la liste des entités et prévoir une valeur LOD qui dit le rayon à partir de notre position à laquelle on affiche les objets et qu'on les charge dans la pipeline