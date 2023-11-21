#include "CEngineInterface.h"

CEngineInterface::CEngineInterface(CEngine &engine) {
    bEGIFullscreen = false;
    bEGIWireframeChecked = false;
    bEGIFPSPlotChecked = false;
    iEGIFpsLimiter = 85;
    iEGIWidth = engine.uiENGWidth;
    iEGIHeight = engine.uiENGHeight;
    gfEGIBrightness = engine.gfENGGetBrightness();
    gfEGIContrast = engine.gfENGGetContrast();
    gfEGISaturation = engine.gfENGGetSaturation();
    gfEGIGamma = engine.gfENGGetGamma();
    bEGINormeRec_709 = engine.bENGGetNormRec();
    bEGICreateEntitySubModule = true; //A remettre à false
    //New entity values
    pgfEGINewEntityXYZPos = new GLfloat[3];
    pgfEGINewEntityXYZPos[0] = 0.f;
    pgfEGINewEntityXYZPos[1] = 0.f;
    pgfEGINewEntityXYZPos[2] = 0.f;
    uiEGINewEntityGlobalID = 0;
    uiEGINewEntityTypeID = 0;
    gfEGINewEntityScaleRatio = 1.f;
    //New entity material values
    vec3EGINewEntityAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
    vec3EGINewEntityDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    vec3EGINewEntitySpecular = glm::vec3(1.0f, 1.0f, 1.0f);
    fEGINewEntityShininess = 1.0f;
    fEGINewEntityTransparency = 1.0f;

    
    vec3EGITestLightColor = engine.testLight.vec3LIGColorLight;
    vec3EGITestLightPosition = engine.testLight.vec3ENTWorldPosition;
    gfEGITestLightAmbientIntensity = engine.testLight.gfLIGAmbientIntensity;
    gfEGITestLightDiffuseStrength = engine.testLight.gfLIGDiffuseStrength;
    gfEGITestLightSpecularStrength = engine.testLight.gfLIGSpecularStrength;

    //Entity modifications via interface
    fEGINewX = 0.f; fEGINewY = 0.f; fEGINewZ = 0.f;
    gfEGINewRatio = 1.f;

    iEGITextureNumber = 0;
    rdrEGIRender = CRender();

    //Launch commands for ImGui and ImPlot
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(engine.pwindowENGWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

CEngineInterface::~CEngineInterface() {
    //EXIT IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

///////////////////////////////// IMGUI MODULES /////////////////////////////////

//Interface module related to engine settings
void CEngineInterface::EGIEngineModule(CEngine &engine) {
    ImGui::Begin("Engine Settings");
    ImGui::Columns(2);
    ImGui::Text("Engine settings");
    ImGui::Checkbox("Fullscreen", &bEGIFullscreen);
    ImGui::Text("Width");
    ImGui::SameLine();
    ImGui::SliderInt("w", &iEGIWidth, 640, 1920);
    ImGui::Text("Height");
    ImGui::SameLine();
    ImGui::SliderInt("h", &iEGIHeight, 480, 1080);
    ImGui::SameLine();
    if (ImGui::SmallButton("Apply new resolution")) {
        engine.ENGChangeResolution((GLuint)iEGIWidth, (GLuint)iEGIHeight);
        std::cout << "Nouvelle résolution";
    }
    std::string FPS = std::to_string(engine.gfENGFpsCounter);
    std::string ms = std::to_string(engine.gfENGFrameDelayMS);
    std::string FPS_MS_TEXT = "Frame " + std::to_string(engine.iENGFrameNumber) + "\nFPS : " + std::to_string(engine.gfENGFpsCounter) + " / FrameTime : " + std::to_string(engine.gfENGFrameDelayMS) + " ms";
    ImGui::Text(FPS_MS_TEXT.c_str());
    //ImGui::Columns(2); //Permet de créer 2 colonnes dans la même interface et potentiellement de faire des sous-menus
    ImGui::Text("Frame Limite");
    ImGui::SameLine();
    ImGui::SliderInt("fpslim", &iEGIFpsLimiter, 1, 400); //"##" <=> label vide
    ImGui::Checkbox("FPS Plot display", &bEGIFPSPlotChecked);

    //Frame Stats Plot
    if (bEGIFPSPlotChecked == true) {
        if (ImPlot::BeginPlot("FPS Plot")) {
            ImPlot::SetupAxes("t(s)", "FPS");
            ImPlot::PlotLine("FPS", engine.pgfENGFrameDelayMSBuffer, engine.pgfENGFpsCounterBuffer, 1001);
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::EndPlot();
        }
    }
    ImGui::ColorPicker3("Light color vec", (float*)&vec3EGITestLightColor);
    ImGui::SliderFloat("Light ambient intensity", &gfEGITestLightAmbientIntensity, 0.f, 1.f);
    ImGui::SliderFloat("Light diffuse strength", &gfEGITestLightDiffuseStrength, 0.f, 1.f);
    ImGui::SliderFloat("Light specular strength", &gfEGITestLightSpecularStrength, 0.f, 256.f);
    ImGui::SliderFloat3("Light position", (float*)&vec3EGITestLightPosition, -10.f, 10.f);

    ImGui::ColorEdit4("Background color", engine.pgfENGBackgroundColor);
    ImGui::Checkbox("Wireframe display", &bEGIWireframeChecked);

    ImGui::NextColumn();
    ImGui::Text("Post-processing tools");
    ImGui::SliderFloat("Brightness", &gfEGIBrightness, 0.0f, 1.0f);
    ImGui::SliderFloat("Contrast", &gfEGIContrast, 0.0f, 4.0f);
    ImGui::Checkbox("Norme Rec. 709", &bEGINormeRec_709);
    ImGui::SliderFloat("Saturation", &gfEGISaturation, -20.0f, 20.0f);
    ImGui::SliderFloat("Gamma", &gfEGIGamma, 0.0f, 4.0f);
    if (ImGui::SmallButton("Reset to default values")) {
        gfEGIBrightness = 0.0f;
        gfEGIContrast = 1.0f;
        gfEGISaturation = 1.0f;
        gfEGIGamma = 1.0f;
    }
    ImGui::End();
}

//Interface for inputs
void CEngineInterface::EGIInputsModule(CEngine &engine) {

}

//Interface for textures
void CEngineInterface::EGITexturesModule(CEngine &engine) {
    ImGui::SetNextWindowSizeConstraints(ImVec2(SIZE_TEXTURE_INTERFACE, 0), ImVec2(engine.iENGScreenWidth, engine.iENGScreenHeight));
    ImGui::Begin("Textures");
    int nombre_texture_par_ligne = ImGui::GetWindowSize()[0]/SIZE_TEXTURE_INTERFACE;
    ImGui::BeginChild("Textures panel", ImVec2(0, 0), true);
    for (int boucle_tex = 0; boucle_tex < engine.uiENGNumberOfTexturesFile; boucle_tex++) {
        //On doit cast le numero de texture GLuint vers (void*)(intptr_t) car ImGui demande un ImTextureId et vu que c'est propre à chaque API
        //Il faut bien cast notre valeur (cela aurait été différent avec DX9 ou Vulkan)
        // Fonction pour charger l'affichage d'une image dans un module ImGui ci-dessous
        //ImGui::Image((void*)(intptr_t)textureImage.guiTEXGetNumeroTexture(), ImVec2(textureImage.iTEXGetTextureWidth() / 5, textureImage.iTEXGetTextureHeight() / 5));
        CTexture textureImage = engine.ptexENGAllTextures[boucle_tex];
        if (boucle_tex % nombre_texture_par_ligne != 0) {
            ImGui::SameLine();
        }
        if (ImGui::ImageButton((void*)(intptr_t)textureImage.guiTEXGetNumeroTexture(), ImVec2(SIZE_TEXTURE_INTERFACE, SIZE_TEXTURE_INTERFACE))) {
            iEGITextureNumber = boucle_tex;
            std::cout << "Valeur texture = " << iEGITextureNumber << std::endl;
        }
    }
    ImGui::EndChild();
    ImGui::End();
}

//Interface module related to entities 
void CEngineInterface::EGIEntitiesModule(CEngine &engine) {
    ImGui::Begin("Entities");
    static int selectedEntity = 0;

    ImGui::Columns(3);
    ImGui::Checkbox("New entity", &bEGICreateEntitySubModule);

    ImGui::BeginChild("Entity panel", ImVec2(180, 0), true);
    for (int nb_ent = 0; nb_ent < engine.iENGGetTotalNumberOfEntities(); nb_ent++) {
        // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
        char label[128];
        if (nb_ent < engine.iENGGetNumberOfEntitiesTypeX(cube)) {
            sprintf_s(label, engine.pcubENGCubeEntitiesList[nb_ent].strENTName.c_str(),  nb_ent);
        }
        if (nb_ent >= engine.iENGGetNumberOfEntitiesTypeX(cube)) {
            sprintf_s(label, engine.pligENGLightEntitiesList[nb_ent-engine.iENGGetNumberOfEntitiesTypeX(cube)].strENTName.c_str(), nb_ent);
        }
        if (ImGui::Selectable(label, selectedEntity == nb_ent)) {
            selectedEntity = nb_ent;
            if (selectedEntity < engine.iENGGetNumberOfEntitiesTypeX(cube)) {
                unsigned int pos_cub = selectedEntity;
                fEGINewX = engine.pcubENGCubeEntitiesList[pos_cub].vec3ENTWorldPosition.x;
                fEGINewY = engine.pcubENGCubeEntitiesList[pos_cub].vec3ENTWorldPosition.y;
                fEGINewZ = engine.pcubENGCubeEntitiesList[pos_cub].vec3ENTWorldPosition.z;
                gfEGINewRatio = engine.pcubENGCubeEntitiesList[pos_cub].gfCUBScaleRatio;
            }
            if (selectedEntity >= engine.iENGGetNumberOfEntitiesTypeX(cube)) {
                unsigned int pos_lig = selectedEntity - engine.iENGGetNumberOfEntitiesTypeX(cube);
                fEGINewX = engine.pligENGLightEntitiesList[pos_lig].vec3ENTWorldPosition.x;
                fEGINewY = engine.pligENGLightEntitiesList[pos_lig].vec3ENTWorldPosition.y;
                fEGINewZ = engine.pligENGLightEntitiesList[pos_lig].vec3ENTWorldPosition.z;
                gfEGINewRatio = engine.pligENGLightEntitiesList[pos_lig].gfLIGScaleRatio;
            }
        }
    }
    ImGui::EndChild();

    if (bEGICreateEntitySubModule == true) {
        ImGui::NextColumn();
        static int entityTypeCombo;
        const char* entityItems[] = { "Cube", "Light" };
        ImGui::Combo("Entity type", &entityTypeCombo, "Cube\0" "Light\0");
        //Sliders for XYZ Axis position of the new entity
        const char* axisSliders[] = { "X", "Y", "Z" };
        for (int boucle_axe = 0; boucle_axe < 3; boucle_axe++) {
            std::string axisText = axisSliders[boucle_axe];
            axisText += " Axis";
            ImGui::SliderFloat(axisText.c_str(), &pgfEGINewEntityXYZPos[boucle_axe], -10.0f, 10.0f); //On peut mettre -1000 1000
            ImGui::SameLine();
            std::string axisRound = axisSliders[boucle_axe];
            axisRound += " Round value";
            if (ImGui::SmallButton(axisRound.c_str())) {
                pgfEGINewEntityXYZPos[boucle_axe] = (int)round(pgfEGINewEntityXYZPos[boucle_axe]);
            }
            ImGui::NewLine();
            int button_values[6] = { -100,-10,-1,1,10,100 };
            for (int boucle_button = 0; boucle_button < 6; boucle_button++) {
                ImGui::SameLine();
                std::string buttonTextValue = std::to_string(button_values[boucle_button]) + " ";
                buttonTextValue += axisSliders[boucle_axe];
                if (ImGui::SmallButton(buttonTextValue.c_str())) {
                    pgfEGINewEntityXYZPos[boucle_axe] += button_values[boucle_button];
                }
            }
        }
        ImGui::SliderFloat("Scale ratio", &gfEGINewEntityScaleRatio, 0.0f, 100.0f);
        //Entity's material values
        ImGui::SliderFloat3("Entity's ambient", (float*)&vec3EGINewEntityAmbient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Entity's diffuse", (float*)&vec3EGINewEntityDiffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Entity's specular", (float*)&vec3EGINewEntitySpecular, 0.0f, 1.0f);
        ImGui::SliderFloat("Entity's shininess", &fEGINewEntityShininess, 0.0f, 1.0f);
        ImGui::SliderFloat("Entity's transparency", &fEGINewEntityTransparency, 0.0f, 1.0f);

        //If the user want to create a new light
        if (entityTypeCombo == 1) {
            ImGui::ColorPicker3("Color's light", pgfEGINewLightColor);
            ImGui::SliderFloat("Ambient Intensity", &gfEGINewLightAmbientIntensity, 0.f, 1.f);
        }
        
        //ImStrncpy(newEntityTexturePath, "../../../Assets/", sizeof("../../../Assets/"));
        char newEntityTexturePath[512] = "../../../Assets/";
        ImGui::InputText("Texture PATH", newEntityTexturePath, sizeof(newEntityTexturePath));
        if (ImGui::SmallButton("Create entity")) {
            //Create a new entity with the set parameters and reset values to default ones
            entity_type_enum newEntityType;
            glm::vec3 newEntityWorldPosition(pgfEGINewEntityXYZPos[0], pgfEGINewEntityXYZPos[1], pgfEGINewEntityXYZPos[2]);
            unsigned int newEntityGlobalId = engine.uiENGGetNextFreeGlobalID();
            if (entityTypeCombo == 0) {
                newEntityType = cube;
                unsigned int newEntityTypeId = engine.uiENGGetNextFreeEntityID(newEntityType);
                CCube newCube = CCube(newEntityGlobalId, newEntityTypeId, newEntityWorldPosition, "INTERNAL/Shaders/core.vs", "INTERNAL/Shaders/core.frag", iEGITextureNumber, vec3EGINewEntityAmbient, vec3EGINewEntityDiffuse, vec3EGINewEntitySpecular, fEGINewEntityShininess, fEGINewEntityTransparency);
                newCube.CUBChangeWorldPosition(newCube.vec3ENTWorldPosition);
                newCube.CUBScaleEntitySize(gfEGINewEntityScaleRatio);
                std::cout << newCube.uiCUBId << std::endl;
                rdrEGIRender.RDRCreateMandatoryForCube(engine, newCube, newCube.uiCUBId);
                engine.ENGAddCubeEntity(newCube); //Modifier pour ajouter dans les listes correspondantes (séparer light et cube par ex.)
            }
            if (entityTypeCombo == 1) {
                newEntityType = light;
                unsigned int newEntityTypeId = engine.uiENGGetNextFreeEntityID(newEntityType);
            }
        }
    }
    ImGui::NextColumn();
    //Display informations about selected entity : ID, Position X,Y,Z et texture pour l'instant
    //Si on a une entité cube
    if (selectedEntity < engine.iENGGetNumberOfEntitiesTypeX(cube)) {
        float Xc, Yc, Zc;
        unsigned int pos_cub = selectedEntity;
        Xc = engine.pcubENGCubeEntitiesList[pos_cub].vec3ENTWorldPosition.x;
        Yc = engine.pcubENGCubeEntitiesList[pos_cub].vec3ENTWorldPosition.y;
        Zc = engine.pcubENGCubeEntitiesList[pos_cub].vec3ENTWorldPosition.z;
        ImGui::SliderFloat("X", &fEGINewX, -10.f, 10.f);
        ImGui::SliderFloat("Y", &fEGINewY, -10.f, 10.f);
        ImGui::SliderFloat("Z", &fEGINewZ, -10.f, 10.f);
        float difXc = fEGINewX - Xc; float difYc = fEGINewY - Yc; float difZc = fEGINewZ - Zc;
        if (difXc != 0 || difYc != 0 || difZc != 0) {
            engine.pcubENGCubeEntitiesList[pos_cub].CUBChangeWorldPosition(glm::vec3(fEGINewX, fEGINewY, fEGINewZ));
            rdrEGIRender.RDRCreateMandatoryForCube(engine, engine.pcubENGCubeEntitiesList[pos_cub], engine.pcubENGCubeEntitiesList[pos_cub].uiCUBId);
        }
        ImGui::Text("Position : X = %.3f, Y = %.3f, Z = %.3f", Xc, Yc, Zc);
        ImGui::SliderFloat("Scale Ratio", &gfEGINewRatio, 0.01f, 10.f);
        GLfloat ScaleRatio = engine.pcubENGCubeEntitiesList[pos_cub].gfCUBScaleRatio;
        GLfloat difScaleRatio = gfEGINewRatio - ScaleRatio;
        if (difScaleRatio != 0) {
            engine.pcubENGCubeEntitiesList[pos_cub].CUBScaleEntitySize(gfEGINewRatio);
            rdrEGIRender.RDRCreateMandatoryForCube(engine, engine.pcubENGCubeEntitiesList[pos_cub], engine.pcubENGCubeEntitiesList[pos_cub].uiCUBId);
        }
    }
    //Si on a une light (actuellement avec les compteurs d'entités mais il faudra modifier les conditions lorsqu'il y aura d'autres entités)
    if (selectedEntity >= engine.iENGGetNumberOfEntitiesTypeX(cube)) {
        float Xl, Yl, Zl; //Update the cube light position (only when it's needed)
        unsigned int pos_lig = selectedEntity - engine.iENGGetNumberOfEntitiesTypeX(cube);
        Xl = engine.pligENGLightEntitiesList[pos_lig].vec3ENTWorldPosition.x;
        Yl = engine.pligENGLightEntitiesList[pos_lig].vec3ENTWorldPosition.y;
        Zl = engine.pligENGLightEntitiesList[pos_lig].vec3ENTWorldPosition.z;
        ImGui::SliderFloat("X", &fEGINewX, -10.f, 10.f);
        ImGui::SliderFloat("Y", &fEGINewY, -10.f, 10.f);
        ImGui::SliderFloat("Z", &fEGINewZ, -10.f, 10.f);
        float difXl = fEGINewX - Xl; float difYl = fEGINewY - Yl; float difZl = fEGINewZ - Zl;
        if (difXl != 0 || difYl != 0 || difZl != 0) {
            engine.pligENGLightEntitiesList[pos_lig].LIGChangeWorldPosition(glm::vec3(fEGINewX, fEGINewY, fEGINewZ));
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGLightEntitiesList[pos_lig], engine.pligENGLightEntitiesList[pos_lig].uiLIGId);
        }
        ImGui::Text("Position : X = %.3f, Y = %.3f, Z = %.3f", Xl, Yl, Zl);
        ImGui::SliderFloat("Scale Ratio", &gfEGINewRatio, 0.01f, 10.f); //Update the scale ratio of the light cube (when update is needed)
        GLfloat ScaleRatio = engine.pligENGLightEntitiesList[pos_lig].gfLIGScaleRatio;
        GLfloat difScaleRatio = gfEGINewRatio - ScaleRatio;
        if (difScaleRatio != 0) {
            engine.pligENGLightEntitiesList[pos_lig].LIGScaleEntitySize(gfEGINewRatio);
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGLightEntitiesList[pos_lig], engine.pligENGLightEntitiesList[pos_lig].uiLIGId);
        }
        ImGui::ColorEdit3("Light Color", engine.pligENGLightEntitiesList[pos_lig].gfLIGColorLight); //Update the light's color and settings
        ImGui::SliderFloat("Ambient Intensity", &engine.pligENGLightEntitiesList[pos_lig].gfLIGAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &engine.pligENGLightEntitiesList[pos_lig].gfLIGDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &engine.pligENGLightEntitiesList[pos_lig].gfLIGSpecularStrength, 0.f, 1.f);
    }
    ImGui::SameLine();
    ImGui::End();
}

void CEngineInterface::EGICameraModule(CEngine& engine, CCamera& camera) {

}

////////////////////////////////////////////////////////////////////////////////

void CEngineInterface::EGIWireframeUpdate() {
    if (bEGIWireframeChecked == false) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (bEGIWireframeChecked == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

//Check if the render mode should be windowed or fullscreen
void CEngineInterface::EGIFullscreenUpdate(CEngine &engine) {
    if (bEGIFullscreen == false) {
        glfwSetWindowMonitor(engine.pwindowENGWindow, NULL, 60, 60, engine.uiENGWidth, engine.uiENGHeight, 0);
    }
    else if (bEGIFullscreen == true) {
        glfwSetWindowMonitor(engine.pwindowENGWindow, engine.pmonitorENGMonitor, 0, 0, engine.uiENGWidth, engine.uiENGHeight, 0);
    }
}

//Fonction à compléter notamment avec les wireframe et le fullscreen car les laisser dans l'interface est moins logique
void CEngineInterface::EGIInterfaceToEngine(CEngine &engine) {
    engine.ENGSetFpsLimit(iEGIFpsLimiter);
    engine.ENGSetBrightness(gfEGIBrightness);
    engine.ENGSetContrast(gfEGIContrast);
    engine.ENGSetSaturation(gfEGISaturation);
    engine.ENGSetGamma(gfEGIGamma);
    engine.ENGSetNormRec(bEGINormeRec_709);
    engine.testLight.vec3LIGColorLight = vec3EGITestLightColor;
    engine.testLight.gfLIGAmbientIntensity = gfEGITestLightAmbientIntensity;
    engine.testLight.gfLIGDiffuseStrength = gfEGITestLightDiffuseStrength;
    engine.testLight.gfLIGSpecularStrength = gfEGITestLightSpecularStrength;
    engine.testLight.vec3ENTWorldPosition = vec3EGITestLightPosition;
}

void CEngineInterface::EGIUpdate(CEngine &engine) { //Update the display of the interface at each frame time
    //Pre-Update
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Interface Modules rendering methods
    EGIEngineModule(engine);
    EGIInputsModule(engine);
    EGITexturesModule(engine);
    EGIEntitiesModule(engine);
    EGICameraModule(engine, engine.inpENGInputs.camINPChosenCamera);

    EGIWireframeUpdate();
    EGIFullscreenUpdate(engine);

    EGIInterfaceToEngine(engine);

    //Post Update
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}