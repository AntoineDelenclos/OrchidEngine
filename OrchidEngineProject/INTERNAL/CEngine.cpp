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
		pgfENGFrameDelayMSBuffer[boucle_init] = boucle_init / 1000;
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
	iENGNumberOfEntities = 0;
	uiENGMaxNumberEntities = 1000;
	ppentENGAllEntitiesList = new (CEntity(*[uiENGMaxNumberEntities]));
	pentENGCubeEntitiesList = new (CEntity[uiENGMaxNumberEntities]);
	ppentENGAllEntitiesList[0] = pentENGCubeEntitiesList;
	puiENGAvailableEntitiesIDs = new(unsigned int[uiENGMaxNumberEntities]);
	for (int boucle = 0; boucle < uiENGMaxNumberEntities; boucle++) {
		puiENGAvailableEntitiesIDs[boucle] = boucle;
	}
	uiENGNextFreeEntityID = 0;

	//Textures
	uiENGMaxNumberOfTextures = 10;
	ptexENGAllTextures = new (CTexture[uiENGMaxNumberOfTextures]);
	uiENGNumberOfTexturesFile = 0;

	iENGMaxNumberVAO = 100;
	iENGMaxNumberVBO = 100;
	puiENGVAOEngine = new (GLuint[iENGMaxNumberVAO]);
	puiENGVBOEngine = new (GLuint[iENGMaxNumberVBO]);


	//TestLight
	vec3ENGTestLightColor = glm::vec3(0.89f, 0.66f, 0.4f); //coucher de soleil
	gfENGTestLightAmbientIntensity = 1.0f;
	gfENGTestLightDiffuseStrength = 1.0f;
	gfENGTestLightSpecularStrength = 1.0f;
	vec3ENGTestLightPosition = { 1.2f, 1.0f, 2.0f };

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
	delete[] pentENGCubeEntitiesList;
	delete[] pstrENGTexturesPath;
	delete[] puiENGVAOEngine;
	delete[] puiENGVBOEngine;
}

//////////// Getters & Setters ////////////////

void CEngine::ENGSetFpsLimit(int limit) {
	iENGFpsLimiter = limit;
}
int CEngine::iENGGetFpsLimit() {
	return iENGFpsLimiter;
}
void CEngine::ENGSetNumberOfEntities(int nb_of_ent) {
	iENGNumberOfEntities = nb_of_ent;
}
void CEngine::ENGIncreaseNumberOfEntities(int increase_value) {
	iENGNumberOfEntities += increase_value;
}
int CEngine::iENGGetNumberOfEntities() {
	return iENGNumberOfEntities;
}
void CEngine::ENGSetNextFreeEntityID(unsigned int next_id) {
	uiENGNextFreeEntityID = next_id;
}
void CEngine::ENGIncrementNextFreeEntityID(int val_of_inc) {
	uiENGNextFreeEntityID += val_of_inc;
}
unsigned int CEngine::uiENGGetNextFreeEntityID() {
	return uiENGNextFreeEntityID;
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
		gfENGFpsCounter = (1.0 / dENGDiffTime) * uiENGFpsTempCounter;
		gfENGFrameDelayMS = (dENGDiffTime / uiENGFpsTempCounter) * 1000;
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
	inpENGInputs.camINPChosenCamera.CAMUpdate();
}

void CEngine::ENGLightUpdate() {
	glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, "light.color"), vec3ENGTestLightColor.x, vec3ENGTestLightColor.y, vec3ENGTestLightColor.z);
	glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, "light.position"), vec3ENGTestLightPosition.x, vec3ENGTestLightPosition.y, vec3ENGTestLightPosition.z);
	glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, "light.ambientIntensity"), gfENGTestLightAmbientIntensity, gfENGTestLightAmbientIntensity, gfENGTestLightAmbientIntensity);
	glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, "light.diffuseStrength"), gfENGTestLightDiffuseStrength, gfENGTestLightDiffuseStrength, gfENGTestLightDiffuseStrength);
	glUniform3f(glGetUniformLocation(shaENGCoreShader.Program, "light.specularStrength"), gfENGTestLightSpecularStrength, gfENGTestLightSpecularStrength, gfENGTestLightSpecularStrength);
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
void CEngine::ENGAddCubeEntity(CEntity entity) {
	//ppentENGAllEntitiesList[uiENGNextFreeEntityID] = entity;
	pentENGCubeEntitiesList[uiENGNextFreeEntityID] = entity;
	ENGIncreaseNumberOfEntities(1);
	ENGIncrementNextFreeEntityID(1);
	//Mettre à jour la liste des id disponibles
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