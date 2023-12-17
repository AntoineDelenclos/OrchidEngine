#include "CEngine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//Constructors & Destructors

//Default constructor
CEngine::CEngine() {
	glfwInit();
	if (!glfwInit()) {
		CException exception(GLFW_INIT_ERREUR);
		throw(exception);
	}
	uiENGWidth = 1280;
	uiENGHeight = 960;
	pgfENGBackgroundColor[0] = 0.2f;
	pgfENGBackgroundColor[1] = 0.4f;
	pgfENGBackgroundColor[2] = 0.8f;
	pgfENGBackgroundColor[3] = 1.0f;

	dENGPrevTime = 0.0; dENGCrntTime = 0.0; dENGDiffTime = 0.0;
	uiENGFpsTempCounter = 0;
	iENGFpsLimiter = 60;
	gfENGFpsCounter = 0.0; gfENGFrameDelayMS = 0.0;
	iENGFrameNumber = 0;

	for (int boucle_init = 0; boucle_init < 999; boucle_init++) {
		pgfENGFpsCounterBuffer[boucle_init] = 0.0;
		pgfENGFrameDelayMSBuffer[boucle_init] = (GLfloat)boucle_init / 1000;
	}

	pmonitorENGMonitor = glfwGetPrimaryMonitor();
	pwindowENGWindow = glfwCreateWindow(uiENGWidth, uiENGHeight, "OrchidEngine", nullptr, nullptr);
	uiENGState = 1;
	vec3ENGAbsoluteEnginePosition = { 0.0f,0.0f,0.0f };

	//CEngine* enginecopy = this;
	//prdrENGRenderPipeline = new CRender;
	strENGAssetsFolder = "../../Assets/"; //La racine est le .vcxproj

	iENGMaxNumberOfTextures = 100;
	pstrENGTexturesPath = new (std::string[iENGMaxNumberOfTextures]);
	//////////////////////////// AU DESSUS C EST DU VIEUX CODE AVEC LES TEXTURES

	//Entities
	for (int i = 0; i < TYPES_OF_ENTITIES; i++) {
		piENGNumberOfEntities[i] = 0;
	}
	uiENGMaxNumberEntities = 256;
	ppentENGAllEntitiesList = new (CEntity(*[uiENGMaxNumberEntities]));
	pcubENGCubeEntitiesList = new (CCube[uiENGMaxNumberEntities]);
	pligENGLightEntitiesList = new (CLight[uiENGMaxNumberEntities]);
	ppentENGAllEntitiesList[0] = pcubENGCubeEntitiesList;
	ppentENGAllEntitiesList[1] = pligENGLightEntitiesList;
	puiENGNextFreeEntitiesIDs = new(unsigned int[TYPES_OF_ENTITIES]); 
	for (int boucle = 0; boucle < TYPES_OF_ENTITIES; boucle++) {
		puiENGNextFreeEntitiesIDs[boucle] = 0;
	}
	uiENGNextFreeGlobalID = 0;
	
	//Textures
	uiENGMaxNumberOfTextures = 100;
	ptexENGAllTextures = new (CTexture[uiENGMaxNumberOfTextures]);
	uiENGNumberOfTexturesFile = 0;

	iENGMaxNumberVAO = 100;
	iENGMaxNumberVBO = 100;
	puiENGVAOCubesEngine = new (GLuint[iENGMaxNumberVAO]);
	puiENGVBOCubesEngine = new (GLuint[iENGMaxNumberVBO]);
	puiENGVAODirectionalLightsEngine = new (GLuint[iENGMaxNumberVAO]);
	puiENGVBODirectionalLightsEngine = new (GLuint[iENGMaxNumberVBO]);
	puiENGVAOPointLightsEngine = new (GLuint[iENGMaxNumberVAO]);
	puiENGVBOPointLightsEngine = new (GLuint[iENGMaxNumberVBO]);
	puiENGVAOSpotLightsEngine = new (GLuint[iENGMaxNumberVAO]);
	puiENGVBOSpotLightsEngine = new (GLuint[iENGMaxNumberVBO]);

	//Lights
	iENGNumberDirectionalLights = 0;
	iENGNumberPointLights =0;
	iENGNumberSpotLights = 0;
	piENGDirectionalLightsID = new(int[uiENGMaxNumberEntities]);
	piENGPointLightsID = new(int[uiENGMaxNumberEntities]);
	piENGSpotLightsID = new(int[uiENGMaxNumberEntities]);
	pligENGDirectionalLightsList = new(CLight[uiENGMaxNumberEntities]);
	pligENGPointLightsList = new(CLight[uiENGMaxNumberEntities]);
	pligENGSpotLightsList = new(CLight[uiENGMaxNumberEntities]);

	//Post-process
	gfENGBrightness = 0.0f;
	gfENGContrast = 1.0f;
	gfENGSaturation = 1.0f;
	gfENGGamma = 1.0f;
	bENGNormeRec_709 = false;

	inpENGInputs = CInputs();
}

//Default destructor
CEngine::~CEngine() {
	delete[] ptexENGAllTextures;
	delete ppentENGAllEntitiesList;
	delete[] pcubENGCubeEntitiesList;
	delete[] pligENGLightEntitiesList;
	delete[] pligENGDirectionalLightsList;
	delete[] pligENGPointLightsList;
	delete[] pligENGSpotLightsList;
	delete[] pstrENGTexturesPath;
	delete[] puiENGVAOCubesEngine;
	delete[] puiENGVBOCubesEngine;
	delete[] puiENGVAODirectionalLightsEngine;
	delete[] puiENGVBODirectionalLightsEngine;
	delete[] puiENGVAOPointLightsEngine;
	delete[] puiENGVBOPointLightsEngine;
	delete[] puiENGVAOSpotLightsEngine;
	delete[] puiENGVBOSpotLightsEngine;
	delete[] puiENGNextFreeEntitiesIDs;
}

//////////// Getters & Setters ////////////////

void CEngine::ENGSetFpsLimit(int limit) {
	iENGFpsLimiter = limit;
}
int CEngine::iENGGetFpsLimit() {
	return iENGFpsLimiter;
}
void CEngine::ENGSetNumberOfEntities(int type_of_entity, int nb_of_ent) {
	piENGNumberOfEntities[type_of_entity] = nb_of_ent;
}
void CEngine::ENGIncreaseNumberOfEntities(int type_of_entity, int increase_value) {
	piENGNumberOfEntities[type_of_entity] += increase_value;
}
int CEngine::iENGGetNumberOfEntitiesTypeX(int type_of_entity) {
	return piENGNumberOfEntities[type_of_entity];
}
int CEngine::iENGGetTotalNumberOfEntities() {
	int total = 0;
	for (int type = 0; type < TYPES_OF_ENTITIES; type++) {
		total += piENGNumberOfEntities[type];
	}
	return total;
}
void CEngine::ENGSetNextFreeEntityID(int type_of_entity, unsigned int next_id) {
	puiENGNextFreeEntitiesIDs[type_of_entity] = next_id;
}
void CEngine::ENGIncrementNextFreeEntityID(int type_of_entity, int val_of_inc) {
	puiENGNextFreeEntitiesIDs[type_of_entity] += val_of_inc;
}
unsigned int CEngine::uiENGGetNextFreeEntityID(int type_of_entity) {
	return puiENGNextFreeEntitiesIDs[type_of_entity];
}
unsigned int CEngine::uiENGGetNextFreeGlobalID() {
	return uiENGNextFreeGlobalID;
}
void CEngine::ENGSetBrightness(GLfloat brightness) {
	gfENGBrightness = brightness;
}
GLfloat CEngine::gfENGGetBrightness() {
	return gfENGBrightness;
}
void CEngine::ENGSetContrast(GLfloat contrast) {
	gfENGContrast = contrast;
}
GLfloat CEngine::gfENGGetContrast() {
	return gfENGContrast;
}
void CEngine::ENGSetSaturation(GLfloat saturation) {
	gfENGSaturation = saturation;
}
GLfloat CEngine::gfENGGetSaturation() {
	return gfENGSaturation;
}
void CEngine::ENGSetGamma(GLfloat gamma) {
	gfENGGamma = gamma;
}
GLfloat CEngine::gfENGGetGamma() {
	return gfENGGamma;
}
void CEngine::ENGSetNormRec(bool norm) {
	bENGNormeRec_709 = norm;
}
bool CEngine::bENGGetNormRec() {
	return bENGNormeRec_709;
}
void CEngine::ENGSetAssetsFolder(std::string path) {
	strENGAssetsFolder = path;
}
std::string CEngine::strENGGetAssetsFolder() {
	return strENGAssetsFolder;
}

//////////////////////////////////////////////

//Launch the mandatory code line to run OpenGL and create the main window
void CEngine::ENGStart() {
	//Utilisation d'OpenGL 3.3 pour avoir les nouveautés:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Permet d'utiliser le GPU et les vertex (nouveautés OpenGL)
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Obligatoire pour macOS
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); //Fenêtre redimensionnable ou non

	glfwGetFramebufferSize(pwindowENGWindow, &iENGScreenWidth, &iENGScreenHeight);

	//Create a windowed mode window and its OpenGL context
	if (pwindowENGWindow == nullptr) {
		CException exception(GLFW_WINDOW_CREATION);
		throw(exception);
	}

	//Make the window's context current
	glfwMakeContextCurrent(pwindowENGWindow);

	glewExperimental = GL_TRUE; //utile pour avoir la gestion moderne des pointeurs
	if (GLEW_OK != glewInit()) {
		CException exception(GLEW_NOT_INITIALIZE);
		throw(exception);
	}

	//Creating shaders use in the engine
	shaENGCoreShader = CShader("INTERNAL/Shaders/core.vs", "INTERNAL/Shaders/core.frag"); //La racine est le .vcxproj
	shaENGLightShader = CShader("INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag");
	
	//Adding and loading all our texture files
	CTexture tex_1 = CTexture("wall_0_4.png", true);
	tex_1.TEXBinding(*this);
	ENGAddTextureToAllTexturesList(tex_1);
	CTexture tex_2 = CTexture("minecraft.png", true);
	tex_2.TEXBinding(*this);
	ENGAddTextureToAllTexturesList(tex_2);
	CTexture tex_3 = CTexture("test.png", false);
	tex_3.TEXBinding(*this);
	ENGAddTextureToAllTexturesList(tex_3);
	CTexture tex_4 = CTexture("white_light_texture.png", false);
	tex_4.TEXBinding(*this);
	ENGAddTextureToAllTexturesList(tex_4);
	CTexture tex_5 = CTexture("container2.png", true);
	tex_5.TEXBinding(*this);
	ENGAddTextureToAllTexturesList(tex_5);
	std::cout << tex_5.guiTEXGetNumeroTexture();
	CTexture tex_5_specular = CTexture("container2_specular.png", true);
	tex_5_specular.TEXBinding(*this);
	ENGAddTextureToAllTexturesList(tex_5_specular);
	std::cout << tex_5_specular.guiTEXGetNumeroTexture();

	glViewport(0, 0, iENGScreenWidth, iENGScreenHeight); //Redimensionne relativement à la taille d'écran
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //Permet d'activer le canal de transparence (alpha)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void CEngine::ENGChangeResolution(GLuint width, GLuint height) {
	glfwSetWindowSize(pwindowENGWindow, width, height);
}

void CEngine::ENGFpsCounterAndLimiter() {
	dENGCrntTime = glfwGetTime();
	dENGDiffTime = dENGCrntTime - dENGPrevTime;
	uiENGFpsTempCounter++;
	if (dENGDiffTime >= 1.0 / iENGGetFpsLimit()) {
		gfENGFpsCounter = ((GLfloat)1.0 / (GLfloat)dENGDiffTime) * uiENGFpsTempCounter;
		gfENGFrameDelayMS = ((GLfloat)dENGDiffTime / uiENGFpsTempCounter) * 1000;
		dENGPrevTime = dENGCrntTime;
		uiENGFpsTempCounter = 0;
	}

	//Décaler de 1 vers la gauche
	for (int boucle = 0; boucle < 999; boucle++) {
		pgfENGFpsCounterBuffer[boucle] = pgfENGFpsCounterBuffer[boucle + 1];
		//pgfENGFrameDelayMSBuffer[boucle] = pgfENGFrameDelayMSBuffer[boucle + 1];
	}
	pgfENGFpsCounterBuffer[999] = gfENGFpsCounter;
	//pgfENGFrameDelayMSBuffer[999] = gfENGFrameDelayMS;
	iENGFrameNumber++;
}

void CEngine::ENGCameraUpdate() {
	CCamera camera = inpENGInputs.camINPChosenCamera;
	camera.mat4CAMView = glm::lookAt(camera.vec3CAMCameraPosition, camera.vec3CAMCameraPosition + camera.vec3CAMCameraFront, camera.vec3CAMCameraUp);
	camera.mat4CAMProjection = glm::perspective(glm::radians(camera.fCAMFovZoom), (float)uiENGWidth / (float)uiENGHeight, 0.1f, 100.f);
	GLint modelLoc = glGetUniformLocation(shaENGCoreShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(shaENGCoreShader.Program, "view");
	GLint projLoc = glGetUniformLocation(shaENGCoreShader.Program, "projection");
	GLint movLoc = glGetUniformLocation(shaENGCoreShader.Program, "movement");
	shaENGCoreShader.SHAUse();
	//On les passe aux shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(camera.mat4CAMModel));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.mat4CAMView));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.mat4CAMProjection));
	glUniformMatrix4fv(movLoc, 1, GL_FALSE, glm::value_ptr(inpENGInputs.mat4INPMovement));

	//LIGHT SHADERS
	GLint modelLightLoc = glGetUniformLocation(shaENGLightShader.Program, "model");
	GLint viewLightLoc = glGetUniformLocation(shaENGLightShader.Program, "view");
	GLint projLightLoc = glGetUniformLocation(shaENGLightShader.Program, "projection");
	GLint movLightLoc = glGetUniformLocation(shaENGLightShader.Program, "movement");
	shaENGLightShader.SHAUse();
	glUniformMatrix4fv(modelLightLoc, 1, GL_FALSE, glm::value_ptr(camera.mat4CAMModel));
	glUniformMatrix4fv(viewLightLoc, 1, GL_FALSE, glm::value_ptr(camera.mat4CAMView));
	glUniformMatrix4fv(projLightLoc, 1, GL_FALSE, glm::value_ptr(camera.mat4CAMProjection));
	glUniformMatrix4fv(movLightLoc, 1, GL_FALSE, glm::value_ptr(inpENGInputs.mat4INPMovement));
	//inpENGInputs.camINPChosenCamera.CAMUpdate();
}

void CEngine::ENGLightUpdate() {
	shaENGCoreShader.SHAUse();
	//Directional lights
	glUniform1i(glGetUniformLocation(shaENGCoreShader.Program, "number_dir_light"), iENGNumberDirectionalLights);
	for (int boucle_dir = 0; boucle_dir < iENGNumberDirectionalLights; boucle_dir++) {
		std::string str = "directionalLights["; str += std::to_string(boucle_dir); str += "]";
		std::string locColor = str + ".color";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locColor.c_str()), pligENGDirectionalLightsList[boucle_dir].gfLIGColorLight[0], pligENGDirectionalLightsList[boucle_dir].gfLIGColorLight[1], pligENGDirectionalLightsList[boucle_dir].gfLIGColorLight[2]);
		std::string logDirection = str + ".direction";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, logDirection.c_str()), pligENGDirectionalLightsList[boucle_dir].vec3LIGDirection.x, pligENGDirectionalLightsList[boucle_dir].vec3LIGDirection.y, pligENGDirectionalLightsList[boucle_dir].vec3LIGDirection.z);
		std::string locAmbient = str + ".ambientIntensity";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locAmbient.c_str()), pligENGDirectionalLightsList[boucle_dir].gfLIGAmbientIntensity, pligENGDirectionalLightsList[boucle_dir].gfLIGAmbientIntensity, pligENGDirectionalLightsList[boucle_dir].gfLIGAmbientIntensity);
		std::string locDiffuse = str + ".diffuseStrength";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locDiffuse.c_str()), pligENGDirectionalLightsList[boucle_dir].gfLIGDiffuseStrength, pligENGDirectionalLightsList[boucle_dir].gfLIGDiffuseStrength, pligENGDirectionalLightsList[boucle_dir].gfLIGDiffuseStrength);
		std::string locSpecular = str + ".specularStrength";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locSpecular.c_str()), pligENGDirectionalLightsList[boucle_dir].gfLIGSpecularStrength, pligENGDirectionalLightsList[boucle_dir].gfLIGSpecularStrength, pligENGDirectionalLightsList[boucle_dir].gfLIGSpecularStrength);
	}
	//Points lights
	glUniform1i(glGetUniformLocation(shaENGCoreShader.Program, "number_point_light"), iENGNumberPointLights);
	for (int boucle_point = 0; boucle_point < iENGNumberPointLights; boucle_point++) {
		std::string str = "pointLights["; str += std::to_string(boucle_point); str += "]";
		std::string locColor = str + ".color";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locColor.c_str()), pligENGPointLightsList[boucle_point].gfLIGColorLight[0], pligENGPointLightsList[boucle_point].gfLIGColorLight[1], pligENGPointLightsList[boucle_point].gfLIGColorLight[2]);
		std::string locPosition = str + ".position";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locPosition.c_str()), pligENGPointLightsList[boucle_point].vec3ENTWorldPosition.x, pligENGPointLightsList[boucle_point].vec3ENTWorldPosition.y, pligENGPointLightsList[boucle_point].vec3ENTWorldPosition.z);
		std::string locConstant = str + ".constant";
		glUniform1f(glGetUniformLocation(shaENGCoreShader.Program, locConstant.c_str()), pligENGPointLightsList[boucle_point].fLIGPointKC);
		std::string locLinear = str + ".linear";
		glUniform1f(glGetUniformLocation(shaENGCoreShader.Program, locLinear.c_str()), pligENGPointLightsList[boucle_point].fLIGPointKL);
		std::string locQuadratic = str + ".quadratic";
		glUniform1f(glGetUniformLocation(shaENGCoreShader.Program, locQuadratic.c_str()), pligENGPointLightsList[boucle_point].fLIGPointKQ);
		std::string locAmbient = str + ".ambientIntensity";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locAmbient.c_str()), pligENGPointLightsList[boucle_point].gfLIGAmbientIntensity, pligENGPointLightsList[boucle_point].gfLIGAmbientIntensity, pligENGPointLightsList[boucle_point].gfLIGAmbientIntensity);
		std::string locDiffuse = str + ".diffuseStrength";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locDiffuse.c_str()), pligENGPointLightsList[boucle_point].gfLIGDiffuseStrength, pligENGPointLightsList[boucle_point].gfLIGDiffuseStrength, pligENGPointLightsList[boucle_point].gfLIGDiffuseStrength);
		std::string locSpecular = str + ".specularStrength";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locSpecular.c_str()), pligENGPointLightsList[boucle_point].gfLIGSpecularStrength, pligENGPointLightsList[boucle_point].gfLIGSpecularStrength, pligENGPointLightsList[boucle_point].gfLIGSpecularStrength);
	}
	//Spotlights
	glUniform1i(glGetUniformLocation(shaENGCoreShader.Program, "number_spot_light"), iENGNumberSpotLights);
	for (int boucle_spot = 0; boucle_spot < iENGNumberSpotLights; boucle_spot++) {
		std::string str = "spotLights["; str += std::to_string(boucle_spot); str += "]";
		std::string locColor = str + ".color";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locColor.c_str()), pligENGSpotLightsList[boucle_spot].gfLIGColorLight[0], pligENGSpotLightsList[boucle_spot].gfLIGColorLight[1], pligENGSpotLightsList[boucle_spot].gfLIGColorLight[2]);
		std::string locPosition = str + ".position";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locPosition.c_str()), pligENGSpotLightsList[boucle_spot].vec3ENTWorldPosition.x, pligENGSpotLightsList[boucle_spot].vec3ENTWorldPosition.y, pligENGSpotLightsList[boucle_spot].vec3ENTWorldPosition.z);
		std::string logDirection = str + ".direction";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, logDirection.c_str()), pligENGSpotLightsList[boucle_spot].vec3LIGDirection.x, pligENGSpotLightsList[boucle_spot].vec3LIGDirection.y, pligENGSpotLightsList[boucle_spot].vec3LIGDirection.z);
		std::string logInner = str + ".innerCutOff";
		glUniform1f(glGetUniformLocation(shaENGCoreShader.Program, logInner.c_str()), pligENGSpotLightsList[boucle_spot].fLIGInnerCutOff);
		std::string logOuter = str + ".outerCutOff";
		glUniform1f(glGetUniformLocation(shaENGCoreShader.Program, logOuter.c_str()), pligENGSpotLightsList[boucle_spot].fLIGOuterCutOff);
		std::string locAmbient = str + ".ambientIntensity";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locAmbient.c_str()), pligENGSpotLightsList[boucle_spot].gfLIGAmbientIntensity, pligENGSpotLightsList[boucle_spot].gfLIGAmbientIntensity, pligENGSpotLightsList[boucle_spot].gfLIGAmbientIntensity);
		std::string locDiffuse = str + ".diffuseStrength";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locDiffuse.c_str()), pligENGSpotLightsList[boucle_spot].gfLIGDiffuseStrength, pligENGSpotLightsList[boucle_spot].gfLIGDiffuseStrength, pligENGSpotLightsList[boucle_spot].gfLIGDiffuseStrength);
		std::string locSpecular = str + ".specularStrength";
		glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, locSpecular.c_str()), pligENGSpotLightsList[boucle_spot].gfLIGSpecularStrength, pligENGSpotLightsList[boucle_spot].gfLIGSpecularStrength, pligENGSpotLightsList[boucle_spot].gfLIGSpecularStrength);
	}
}

void CEngine::ENGFrameUpdate() {
	if (uiENGState != 1) { //Penser à rajouter une méthode pour le changement d'état du moteur
		CException exception(ENGINE_NOT_RUNNING);
		throw(exception);
	}
	//Window state update
	glfwPollEvents();//Poll for and process events
	glClearColor(pgfENGBackgroundColor[0], pgfENGBackgroundColor[1], pgfENGBackgroundColor[2], pgfENGBackgroundColor[3]); //Permet de vider le cache de ce qu'il y avait avant
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Et de mettre la couleur que l'on souhaite
	//Rendering update
	//ENGWireframeUpdate();
	ENGCameraUpdate();
	ENGFpsCounterAndLimiter();
	//ENGPostProcess();
	
	//ENGRender();

	//Interface update
}

////// ENTITY RELATED //////
void CEngine::ENGAddCubeEntity(CCube cube) {
	pcubENGCubeEntitiesList[puiENGNextFreeEntitiesIDs[0]] = cube;
	ENGIncreaseNumberOfEntities(0, 1);
	ENGIncrementNextFreeEntityID(0, 1);
}

void CEngine::ENGAddLightEntity(CLight light) {
	pligENGLightEntitiesList[puiENGNextFreeEntitiesIDs[2]] = light;
	switch (light.enumLIGType) {
	case (directional):
		pligENGDirectionalLightsList[iENGGetNumberOfEntitiesTypeX(dir_light)] = light;
		//piENGDirectionalLightsID[iENGNumberDirectionalLights] = light.uiLIGId;
		ENGIncreaseNumberOfEntities(2, 1);
		ENGIncrementNextFreeEntityID(2, 1);
		iENGNumberDirectionalLights += 1;
		mapStrIntENGNumberOfEachEntities["dir_light"] += 1;
		break;
	case (point):
		pligENGPointLightsList[iENGGetNumberOfEntitiesTypeX(point_light)] = light;
		//piENGPointLightsID[iENGNumberPointLights] = light.uiLIGId;
		ENGIncreaseNumberOfEntities(3, 1);
		ENGIncrementNextFreeEntityID(3, 1);
		iENGNumberPointLights += 1;
		mapStrIntENGNumberOfEachEntities["point_light"] += 1;
		break;
	case (spot):
		pligENGSpotLightsList[iENGGetNumberOfEntitiesTypeX(spot_light)] = light;
		//piENGSpotLightsID[iENGNumberSpotLights] = light.uiLIGId;
		ENGIncreaseNumberOfEntities(4, 1);
		ENGIncrementNextFreeEntityID(4, 1);
		iENGNumberSpotLights += 1;
		mapStrIntENGNumberOfEachEntities["spot_light"] += 1;
		break;
	}
}

///// TEXTURES RELATED /////

void CEngine::ENGAddTextureToAllTexturesList(CTexture texture) {
	if (uiENGMaxNumberOfTextures > uiENGNumberOfTexturesFile + 1) {
		ptexENGAllTextures[uiENGNumberOfTexturesFile] = texture;
		uiENGNumberOfTexturesFile++;
	}
	else {
		CException exception(ENGINE_TEXTURE_LIMIT);
		throw(exception);
	}
}

//Function to pass to the callback functions in inputs the values needed
void CEngine::ENGPreUpdateInputsValues() {
	inpENGInputs.dINPDiffTime = dENGDiffTime;
}



///////////////////////////////////////////////////////////////////////// CTEXTURE //////////////////////////////////////////////////////////////
CTexture::CTexture() {

}

CTexture::CTexture(std::string path, bool transparent) {
	guiTEXNumeroTexture;
	//pucTEXImage;
	iTEXTextureWidth;
	iTEXTextureHeight;
	iTEXNumeroChannels;
	strTEXImagePath = path;
	bTEXTransparent = transparent;
}

CTexture::~CTexture() {

}

void CTexture::TEXSetNumeroTexture(GLuint nr_tex) {
	guiTEXNumeroTexture = nr_tex;
}
GLuint CTexture::guiTEXGetNumeroTexture() {
	return guiTEXNumeroTexture;
}

/*void CTexture::TEXSetImage(unsigned char* image) {
	pucTEXImage = image;
}
unsigned char* CTexture::pucTEXGetImage() {
	return pucTEXImage;
}*/

void CTexture::TEXSetTextureWidth(int width) {
	iTEXTextureWidth = width;
}
int CTexture::iTEXGetTextureWidth() {
	return iTEXTextureWidth;
}
void CTexture::TEXSetTextureHeight(int height) {
	iTEXTextureHeight = height;
}
int CTexture::iTEXGetTextureHeight() {
	return iTEXTextureHeight;
}
void CTexture::TEXSetNumeroChannels(int nr_chan) {
	iTEXNumeroChannels = nr_chan;
}
int CTexture::iTEXGetNumeroChannels() {
	return iTEXNumeroChannels;
}
void CTexture::TEXSetTransparent(bool transparent) {
	bTEXTransparent = transparent;
}
bool CTexture::bTEXGetTransparent() {
	return bTEXTransparent;
}

void CTexture::TEXBinding(CEngine& engine) {
	glGenTextures(1, &guiTEXNumeroTexture);
	glBindTexture(GL_TEXTURE_2D, guiTEXNumeroTexture);
	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string filepath = engine.strENGGetAssetsFolder().c_str() + strTEXImagePath;
	unsigned char* pucTEXImage = stbi_load(filepath.c_str(), &iTEXTextureWidth, &iTEXTextureHeight, &iTEXNumeroChannels, 0);
	if (!pucTEXImage) {
		CException exception = CException(ERREUR_LOAD_IMAGE);
		throw(exception);
	}
	else if (pucTEXImage) {
		if (bTEXTransparent) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTEXTextureWidth, iTEXTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pucTEXImage);
		}
		else if (!bTEXTransparent) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTEXTextureWidth, iTEXTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pucTEXImage);
		}
		//Si texture transparente : utiliser GL_RGBA sinon ne pas le faire car ca fait un conflit
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(pucTEXImage);
}