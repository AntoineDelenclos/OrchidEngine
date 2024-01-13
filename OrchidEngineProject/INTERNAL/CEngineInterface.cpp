#include "CEngineInterface.h"


CEngineInterface::CEngineInterface(CEngine &engine) {
    bEGIFullscreen = true;
    bEGIWireframeChecked = false;
    bEGIFPSPlotChecked = false;
    bEGIScriptEditorON = false;
    iEGIFpsLimiter = 85;
    iEGIWidth = engine.uiENGWidth;
    iEGIHeight = engine.uiENGHeight;
    gfEGIBrightness = engine.gfENGGetBrightness();
    gfEGIContrast = engine.gfENGGetContrast();
    gfEGISaturation = engine.gfENGGetSaturation();
    gfEGIGamma = engine.gfENGGetGamma();
    bEGINormeRec_709 = engine.bENGGetNormRec();
    piEGITexturePanelSize[0] = 32; //Taille minimum d'une fenêtre ImGui.
    piEGITexturePanelSize[1] = 32;
    iEGINombreTexturesParLigne = 1;
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
    //New entity light
    pgfEGINewLightColor[0] = 1.f; pgfEGINewLightColor[1] = 1.f; pgfEGINewLightColor[2] = 1.f;
    gfEGINewLightAmbientIntensity = 0.5f;


    //Entity modifications via interface
    fEGINewX = 0.f; fEGINewY = 0.f; fEGINewZ = 0.f;
    gfEGINewRatio = 1.f;
    fEGINewDirectionX = 1.0f; fEGINewDirectionY = 1.0f; fEGINewDirectionZ = 1.0f;
    fEGINewKC = 1.0f; fEGINewKL = 0.09f; fEGINewKQ = 0.032f;
    //Selected entity in the lists
    siEGISelectedEntity_cube = -1; siEGISelectedEntity_dir_light = -1; siEGISelectedEntity_point_light = -1;
    siEGISelectedEntity_spot_light = -1; siEGISelectedType = -1;

    iEGITextureNumber = 0;
    rdrEGIRender = CRender();

    //Launch commands for ImGui and ImPlot
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Va permettre de mettre le docking en place 
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
    ImGui::Checkbox("Fullscreen", &bEGIFullscreen);
    ImGui::Text("Width");
    ImGui::SameLine();
    ImGui::SliderInt("w", &iEGIWidth, 640, 1920);
    ImGui::Text("Height");
    ImGui::SameLine();
    ImGui::SliderInt("h", &iEGIHeight, 480, 1080);
    if (ImGui::SmallButton("Apply new resolution")) {
        engine.ENGChangeResolution((GLuint)iEGIWidth, (GLuint)iEGIHeight);
    }
    std::string FPS = std::to_string(engine.gfENGFpsCounter);
    std::string ms = std::to_string(engine.gfENGFrameDelayMS);
    std::string FPS_MS_TEXT = "Frame " + std::to_string(engine.iENGFrameNumber) + "\nFPS : " + std::to_string(engine.gfENGFpsCounter) + " / FrameTime : " + std::to_string(engine.gfENGFrameDelayMS) + " ms";
    ImGui::Text(FPS_MS_TEXT.c_str());
    ImGui::Text("Frame Limite");
    ImGui::SameLine();
    ImGui::SliderInt("##", &iEGIFpsLimiter, 1, 400); //"##" <=> label vide
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
    ImGui::ColorEdit4("Background color", engine.pgfENGBackgroundColor);
    ImGui::Checkbox("Wireframe display", &bEGIWireframeChecked);
    ImGui::End();
}

//Interface for post processing
void CEngineInterface::EGIPostProcessingModule(CEngine& engine) {
    ImGui::Begin("Post-processing tools");
    ImGui::SliderFloat("Brightness", &gfEGIBrightness, 0.0f, 1.0f);
    ImGui::SliderFloat("Contrast", &gfEGIContrast, 0.0f, 4.0f);
    ImGui::Checkbox("Norme Rec. 709", &bEGINormeRec_709);
    ImGui::SliderFloat("Saturation", &gfEGISaturation, -20.0f, 20.0f);
    ImGui::SliderFloat("Gamma", &gfEGIGamma, 0.0f, 4.0f);
    if (ImGui::SmallButton("Reset to default values")) {
        gfEGIBrightness = 1.0f;
        gfEGIContrast = 1.0f;
        gfEGISaturation = 1.0f;
        gfEGIGamma = 1.0f;
    }
    ImGui::ShowAboutWindow(); //Window that will show informations about the current build of ImGui.
    ImGui::End();
}

//Interface for inputs
void CEngineInterface::EGIInputsModule(CEngine &engine) {

}

//Interface for textures
void CEngineInterface::EGITexturesModule(CEngine &engine) {
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2((float)engine.iENGScreenWidth, (float)engine.iENGScreenHeight));
    ImGui::SetNextWindowSize(ImVec2((float)piEGITexturePanelSize[0], (float)piEGITexturePanelSize[1]));
    ImGui::Begin("Textures");
    int nombre_texture_par_ligne = (int)(ImGui::GetWindowSize()[0]/SIZE_TEXTURE_INTERFACE);
    if (nombre_texture_par_ligne != 0 && engine.bENGHasFocus) { //Cas où on est sur la fenêtre (on va alors freeze lors des ALT+TAB)
        iEGINombreTexturesParLigne = nombre_texture_par_ligne; //Permet de resize l'interface de sélection de texture en fonction de ce que souhaite l'utilisateur
        piEGITexturePanelSize[0] = (int)ImGui::GetWindowSize()[0]; //On veut stocker les valeurs de la fenêtre ImGui pour pouvoir avoir les bonnes dimensions lorsqu'on revient sur la fenêtre
        piEGITexturePanelSize[1] = (int)ImGui::GetWindowSize()[1];
    }
    ImGui::BeginChild("Textures panel", ImVec2(0, 0), true);
    for (int boucle_tex = 0; boucle_tex < engine.uiENGNumberOfTexturesFile; boucle_tex++) {
        //On doit cast le numero de texture GLuint vers (void*)(intptr_t) car ImGui demande un ImTextureId et vu que c'est propre à chaque API
        //Il faut bien cast notre valeur (cela aurait été différent avec DX9 ou Vulkan)
        // Fonction pour charger l'affichage d'une image dans un module ImGui ci-dessous
        //ImGui::Image((void*)(intptr_t)textureImage.guiTEXGetNumeroTexture(), ImVec2(textureImage.iTEXGetTextureWidth() / 5, textureImage.iTEXGetTextureHeight() / 5));
        CTexture textureImage = engine.ptexENGAllTextures[boucle_tex];
        if (boucle_tex % iEGINombreTexturesParLigne != 0) {
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

void CEngineInterface::EGINewEntityModule(CEngine& engine) {
    ImGui::Begin("New Entity");
    ImGui::NextColumn();
    static int entityTypeCombo;
    const char* entityItems[] = { "Cube", "Directional Light", "Point Light", "SpotLight" };
    ImGui::Combo("Entity type", &entityTypeCombo, "Cube\0" "Directional Light\0" "Point Light\0" "SpotLight\0");
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
            pgfEGINewEntityXYZPos[boucle_axe] = round(pgfEGINewEntityXYZPos[boucle_axe]);
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
    if (ImGui::SmallButton("Current view position")) {
        pgfEGINewEntityXYZPos[0] = engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.x;
        pgfEGINewEntityXYZPos[1] = engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.y;
        pgfEGINewEntityXYZPos[2] = engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.z;
    }
    ImGui::SliderFloat("Scale ratio", &gfEGINewEntityScaleRatio, 0.0f, 100.0f);
    //Entity's material values
    if (entityTypeCombo == 0) {
        ImGui::SliderFloat3("Cube's ambient", (float*)&vec3EGINewEntityAmbient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Cube's diffuse", (float*)&vec3EGINewEntityDiffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Cube's specular", (float*)&vec3EGINewEntitySpecular, 0.0f, 1.0f);
        ImGui::SliderFloat("Cube's shininess", &fEGINewEntityShininess, 0.0f, 1.0f);
        ImGui::SliderFloat("Cube's transparency", &fEGINewEntityTransparency, 0.0f, 1.0f);
    }
    //If the user wants to create a new directional light
    if (entityTypeCombo == 1) { 
        ImGui::ColorEdit3("Light's color", pgfEGINewLightColor);
        ImGui::SliderFloat("Direction X", &gfEGINewLightDirectionX, -10.f, 10.f);
        ImGui::SliderFloat("Direction Y", &gfEGINewLightDirectionY, -10.f, 10.f);
        ImGui::SliderFloat("Direction Z", &gfEGINewLightDirectionZ, -10.f, 10.f);
        ImGui::SliderFloat("Ambient Intensity", &gfEGINewLightAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &gfEGINewLightDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &gfEGINewLightSpecularStrength, 0.f, 256.f);
    }
    //If the user wants to create a new point light
    if (entityTypeCombo == 2) {
        ImGui::ColorEdit3("Light's color", pgfEGINewLightColor);
        ImGui::SliderFloat("KC", &fEGINewLightKC, 0.f, 1.f);
        ImGui::SliderFloat("KL", &fEGINewLightKL, 0.f, 1.f);
        ImGui::SliderFloat("KQ", &fEGINewLightKQ, 0.f, 1.f);
        ImGui::SliderFloat("Ambient Intensity", &gfEGINewLightAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &gfEGINewLightDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &gfEGINewLightSpecularStrength, 0.f, 256.f);
    }
    if (entityTypeCombo == 3) {
        ImGui::ColorEdit3("Light's color", pgfEGINewLightColor);
        ImGui::SliderFloat("Direction X", &gfEGINewLightDirectionX, -10.f, 10.f);
        ImGui::SliderFloat("Direction Y", &gfEGINewLightDirectionY, -10.f, 10.f);
        ImGui::SliderFloat("Direction Z", &gfEGINewLightDirectionZ, -10.f, 10.f);
        ImGui::SliderFloat("Inner CutOff", &fEGINewLightInnerCutOff, 0.f, 1.f);
        ImGui::SliderFloat("Outer CutOff", &fEGINewLightOuterCutOff, 0.f, 1.f);
        ImGui::SliderFloat("Ambient Intensity", &gfEGINewLightAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &gfEGINewLightDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &gfEGINewLightSpecularStrength, 0.f, 256.f);
    }
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
            newEntityType = dir_light;
            unsigned int newEntityTypeId = engine.uiENGGetNextFreeEntityID(newEntityType);
            CLight newDirectionalLight = CLight(directional, newEntityTypeId, engine.uiENGGetNextFreeEntityID(dir_light), newEntityWorldPosition, glm::vec3(fEGINewDirectionX, fEGINewDirectionY, fEGINewDirectionZ), pgfEGINewLightColor, gfEGINewLightAmbientIntensity, gfEGINewLightDiffuseStrength, gfEGINewLightSpecularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", iEGITextureNumber);
            newDirectionalLight.LIGFirstTimeSetVerticesPosition();
            rdrEGIRender.RDRCreateMandatoryForLight(engine, newDirectionalLight, newDirectionalLight.uiLIGId);
            engine.ENGAddLightEntity(newDirectionalLight);
        }
        if (entityTypeCombo == 2) {
            newEntityType = point_light;
            unsigned int newEntityTypeId = engine.uiENGGetNextFreeEntityID(newEntityType);
            CLight newPointLight = CLight(point, newEntityTypeId, engine.uiENGGetNextFreeEntityID(point_light), newEntityWorldPosition, pgfEGINewLightColor, fEGINewKC, fEGINewKL, fEGINewKQ, gfEGINewLightAmbientIntensity, gfEGINewLightDiffuseStrength, gfEGINewLightSpecularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", iEGITextureNumber);
            rdrEGIRender.RDRCreateMandatoryForLight(engine, newPointLight, newPointLight.uiLIGId);
            engine.ENGAddLightEntity(newPointLight);
        }
        if (entityTypeCombo == 3) {
            newEntityType = spot_light;
            unsigned int newEntityTypeId = engine.uiENGGetNextFreeEntityID(newEntityType);
            CLight newSpotLight = CLight(spot, newEntityTypeId, engine.uiENGGetNextFreeEntityID(spot_light), newEntityWorldPosition, glm::vec3(fEGINewDirectionX, fEGINewDirectionY, fEGINewDirectionZ), fEGINewLightInnerCutOff, fEGINewLightOuterCutOff, pgfEGINewLightColor, gfEGINewLightAmbientIntensity, gfEGINewLightDiffuseStrength, gfEGINewLightSpecularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", iEGITextureNumber);
            rdrEGIRender.RDRCreateMandatoryForLight(engine, newSpotLight, newSpotLight.uiLIGId);
            engine.ENGAddLightEntity(newSpotLight);
        }
    }
    ImGui::End();
}

void CEngineInterface::EGIEntitiesListsModule(CEngine &engine) {
    ImGui::Begin("Entities Lists");  
    std::string strHeaderCube = "Cube entities ("; strHeaderCube += std::to_string(engine.iENGGetNumberOfEntitiesTypeX(cube)).c_str(); strHeaderCube += ')';
    if (ImGui::CollapsingHeader(strHeaderCube.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) { //Mettre en DefaultOpen permet de réouvrir si on ajoute une entité du type correspondant
        int draw_lines_cube = engine.iENGGetNumberOfEntitiesTypeX(cube);
        static int max_height_in_lines_cube = 5;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::DragInt("Max Height (in Lines) Cubes", &max_height_in_lines_cube, 0.2f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * max_height_in_lines_cube));
        if (ImGui::BeginChild("ConstrainedChildCube", ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (int nb_ent = 0; nb_ent < draw_lines_cube; nb_ent++) {
                char label[128];
                sprintf_s(label, engine.pcubENGCubeEntitiesList[nb_ent].strENTName.c_str(), nb_ent);
                ImU32 text_color; glm::vec4 rgbc;
                if (engine.pcubENGCubeEntitiesList[nb_ent].bENTActive) {
                    rgbc = vec4HexToRGBAColor(ACTIVE_COLOR);
                }
                else {
                    rgbc = vec4HexToRGBAColor(UNACTIVE_COLOR);
                }
                text_color = IM_COL32(rgbc.x, rgbc.y, rgbc.z, rgbc.w);
                ImGui::PushStyleColor(ImGuiCol_Text, text_color);
                bool selected = ImGui::Selectable(label, siEGISelectedEntity_cube == nb_ent);
                ImGui::PopStyleColor();
                if (selected) {
                    siEGISelectedEntity_cube = nb_ent;
                    siEGISelectedType = 0;
                    fEGINewX = engine.pcubENGCubeEntitiesList[siEGISelectedEntity_cube].vec3ENTWorldPosition.x;
                    fEGINewY = engine.pcubENGCubeEntitiesList[siEGISelectedEntity_cube].vec3ENTWorldPosition.y;
                    fEGINewZ = engine.pcubENGCubeEntitiesList[siEGISelectedEntity_cube].vec3ENTWorldPosition.z;
                    gfEGINewRatio = engine.pcubENGCubeEntitiesList[siEGISelectedEntity_cube].gfCUBScaleRatio;
                }
            }
        }
        ImGui::EndChild();
    }
    std::string strHeaderDirLight = "Directional light entities ("; strHeaderDirLight += std::to_string(engine.iENGGetNumberOfEntitiesTypeX(dir_light)).c_str(); strHeaderDirLight += ')';
    if (ImGui::CollapsingHeader(strHeaderDirLight.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        int draw_lines_directional = engine.iENGGetNumberOfEntitiesTypeX(dir_light);
        static int max_height_in_lines_directional = 5;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::DragInt("Max Height (in Lines) Directional Lights", &max_height_in_lines_directional, 0.2f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * max_height_in_lines_directional));
        if (ImGui::BeginChild("ConstrainedChildDirectional", ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (int nb_ent = 0; nb_ent < draw_lines_directional; nb_ent++) {
                char label[128];
                sprintf_s(label, engine.pligENGDirectionalLightsList[nb_ent].strENTName.c_str(), nb_ent);
                ImU32 text_color; glm::vec4 rgbc;
                if (engine.pligENGDirectionalLightsList[nb_ent].bENTActive) {
                    rgbc = vec4HexToRGBAColor(ACTIVE_COLOR);
                }
                else {
                    rgbc = vec4HexToRGBAColor(UNACTIVE_COLOR);
                }
                text_color = IM_COL32(rgbc.x, rgbc.y, rgbc.z, rgbc.w);
                ImGui::PushStyleColor(ImGuiCol_Text, text_color);
                bool selected = ImGui::Selectable(label, siEGISelectedEntity_dir_light == nb_ent);
                ImGui::PopStyleColor();
                if (selected) {
                    siEGISelectedEntity_dir_light = nb_ent;
                    siEGISelectedType = 1;
                    fEGINewX = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].vec3ENTWorldPosition.x;
                    fEGINewY = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].vec3ENTWorldPosition.y;
                    fEGINewZ = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].vec3ENTWorldPosition.z;
                    gfEGINewRatio = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].gfLIGScaleRatio;
                    fEGINewDirectionX = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].vec3LIGDirection.x;
                    fEGINewDirectionY = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].vec3LIGDirection.y;
                    fEGINewDirectionZ = engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].vec3LIGDirection.z;
                }
            }
        }
        ImGui::EndChild();
    }
    
    std::string strHeaderPointLight = "Point light entities ("; strHeaderPointLight += std::to_string(engine.iENGGetNumberOfEntitiesTypeX(point_light)).c_str(); strHeaderPointLight += ')';
    if (ImGui::CollapsingHeader(strHeaderPointLight.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        int draw_lines_point = engine.iENGGetNumberOfEntitiesTypeX(point_light);
        static int max_height_in_lines_point = 5;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::DragInt("Max Height (in Lines) Point Lights", &max_height_in_lines_point, 0.2f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * max_height_in_lines_point));
        if (ImGui::BeginChild("ConstrainedChildPoint", ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (int nb_ent = 0; nb_ent < draw_lines_point; nb_ent++) {
                // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                char label[128];
                sprintf_s(label, engine.pligENGPointLightsList[nb_ent].strENTName.c_str(), nb_ent);
                ImU32 text_color; glm::vec4 rgbc;
                if (engine.pligENGPointLightsList[nb_ent].bENTActive) {
                    rgbc = vec4HexToRGBAColor(ACTIVE_COLOR);
                }
                else {
                    rgbc = vec4HexToRGBAColor(UNACTIVE_COLOR);
                }
                text_color = IM_COL32(rgbc.x, rgbc.y, rgbc.z, rgbc.w);
                ImGui::PushStyleColor(ImGuiCol_Text, text_color);
                bool selected = ImGui::Selectable(label, siEGISelectedEntity_point_light == nb_ent);
                ImGui::PopStyleColor();
                if (selected) {
                    siEGISelectedEntity_point_light = nb_ent;
                    siEGISelectedType = 2;
                    fEGINewX = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].vec3ENTWorldPosition.x;
                    fEGINewY = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].vec3ENTWorldPosition.y;
                    fEGINewZ = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].vec3ENTWorldPosition.z;
                    gfEGINewRatio = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].gfLIGScaleRatio;
                    fEGINewKC = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].fLIGPointKC;
                    fEGINewKL = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].fLIGPointKL;
                    fEGINewKQ = engine.pligENGPointLightsList[siEGISelectedEntity_point_light].fLIGPointKQ;
                }
            }
        }
        ImGui::EndChild();
    }

    std::string strHeaderSpotLight = "Spotlight entities ("; strHeaderSpotLight += std::to_string(engine.iENGGetNumberOfEntitiesTypeX(spot_light)).c_str(); strHeaderSpotLight += ')';
    if (ImGui::CollapsingHeader(strHeaderSpotLight.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        int draw_lines_spot = engine.iENGGetNumberOfEntitiesTypeX(spot_light);
        static int max_height_in_lines_spot = 5;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::DragInt("Max Height (in Lines) SpotLights", &max_height_in_lines_spot, 0.2f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * max_height_in_lines_spot));
        if (ImGui::BeginChild("ConstrainedChildSpot", ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            for (int nb_ent = 0; nb_ent < draw_lines_spot; nb_ent++) {
                // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                char label[128];
                sprintf_s(label, engine.pligENGSpotLightsList[nb_ent].strENTName.c_str(), nb_ent);
                ImU32 text_color; glm::vec4 rgbc;
                if (engine.pligENGSpotLightsList[nb_ent].bENTActive) {
                    rgbc = vec4HexToRGBAColor(ACTIVE_COLOR);
                }
                else {
                    rgbc = vec4HexToRGBAColor(UNACTIVE_COLOR);
                }
                text_color = IM_COL32(rgbc.x, rgbc.y, rgbc.z, rgbc.w);
                ImGui::PushStyleColor(ImGuiCol_Text, text_color);
                bool selected = ImGui::Selectable(label, siEGISelectedEntity_spot_light == nb_ent);
                ImGui::PopStyleColor();
                if (selected) {
                    siEGISelectedEntity_spot_light = nb_ent;
                    siEGISelectedType = 3;
                    fEGINewX = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].vec3ENTWorldPosition.x;
                    fEGINewY = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].vec3ENTWorldPosition.y;
                    fEGINewZ = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].vec3ENTWorldPosition.z;
                    gfEGINewRatio = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].gfLIGScaleRatio;
                    fEGINewDirectionX = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].vec3LIGDirection.x;
                    fEGINewDirectionY = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].vec3LIGDirection.y;
                    fEGINewDirectionZ = engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].vec3LIGDirection.z;
                }
            }
        }
        ImGui::EndChild();
    }    
    ImGui::End();
}

//Display informations about selected entity : ID, Position X,Y,Z et texture pour l'instant
void CEngineInterface::EGISelectedEntityModule(CEngine& engine) {
    ImGui::Begin("Selected entity"); //Essayer de passer le nom de l'entité
    //Pour nos différentes entités cubes
    if (siEGISelectedEntity_cube >= 0 && siEGISelectedType == 0) {
        ImGui::Checkbox("Active", &engine.pcubENGCubeEntitiesList[siEGISelectedEntity_cube].bENTActive);
        ImGui::InputText("Name", &engine.pcubENGCubeEntitiesList[siEGISelectedEntity_cube].strENTName);
        float Xc, Yc, Zc;
        int pos_cub = siEGISelectedEntity_cube;
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
        ImGui::Text("Texture :"); ImGui::SameLine(); //It's also the diffuse map
        if (ImGui::ImageButton((void*)(intptr_t)(engine.pcubENGCubeEntitiesList[pos_cub].uiENTTextureEngineNumber + 1), ImVec2(SIZE_TEXTURE_INTERFACE, SIZE_TEXTURE_INTERFACE))) {
            EGITexturesModule(engine);
            engine.pcubENGCubeEntitiesList[pos_cub].uiENTTextureEngineNumber = iEGITextureNumber;
        }
        ImGui::Text("Specular map :"); ImGui::SameLine();
        if (ImGui::ImageButton((void*)(intptr_t)(engine.pcubENGCubeEntitiesList[pos_cub].uiCUBSpecularTextureEngineNumber + 1), ImVec2(SIZE_TEXTURE_INTERFACE, SIZE_TEXTURE_INTERFACE))) {
            EGITexturesModule(engine);
            std::cout << "actuel = " << engine.pcubENGCubeEntitiesList[pos_cub].uiCUBSpecularTextureEngineNumber << std::endl;
            engine.pcubENGCubeEntitiesList[pos_cub].uiCUBSpecularTextureEngineNumber = iEGITextureNumber;
            std::cout << "nouveau = " << iEGITextureNumber << std::endl;
        }
    }
    //Cas d'une Directional Light
    if (siEGISelectedEntity_dir_light >= 0 && siEGISelectedType == 1) {
        ImGui::Checkbox("Active", &engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].bENTActive);
        ImGui::InputText("Name", &engine.pligENGDirectionalLightsList[siEGISelectedEntity_dir_light].strENTName);
        unsigned int pos_lig = siEGISelectedEntity_dir_light;
        float Xl, Yl, Zl; //Update the cube light position (only when it's needed)
        Xl = engine.pligENGDirectionalLightsList[pos_lig].vec3ENTWorldPosition.x;
        Yl = engine.pligENGDirectionalLightsList[pos_lig].vec3ENTWorldPosition.y;
        Zl = engine.pligENGDirectionalLightsList[pos_lig].vec3ENTWorldPosition.z;
        ImGui::SliderFloat("X", &fEGINewX, -10.f, 10.f);
        ImGui::SliderFloat("Y", &fEGINewY, -10.f, 10.f);
        ImGui::SliderFloat("Z", &fEGINewZ, -10.f, 10.f);
        float difXl = fEGINewX - Xl; float difYl = fEGINewY - Yl; float difZl = fEGINewZ - Zl;
        if (difXl != 0 || difYl != 0 || difZl != 0) {
            engine.pligENGDirectionalLightsList[pos_lig].LIGChangeWorldPosition(glm::vec3(fEGINewX, fEGINewY, fEGINewZ));
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGDirectionalLightsList[pos_lig], engine.pligENGDirectionalLightsList[pos_lig].uiLIGId);
        }
        ImGui::Text("Position : X = %.3f, Y = %.3f, Z = %.3f", Xl, Yl, Zl);
        ImGui::SliderFloat("Scale Ratio", &gfEGINewRatio, 0.01f, 10.f); //Update the scale ratio of the light cube (when update is needed)
        GLfloat ScaleRatio = engine.pligENGDirectionalLightsList[pos_lig].gfLIGScaleRatio;
        GLfloat difScaleRatio = gfEGINewRatio - ScaleRatio;
        if (difScaleRatio != 0) {
            engine.pligENGDirectionalLightsList[pos_lig].LIGScaleEntitySize(gfEGINewRatio);
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGDirectionalLightsList[pos_lig], engine.pligENGDirectionalLightsList[pos_lig].uiLIGId);
        }
        ImGui::ColorEdit3("Light Color", engine.pligENGDirectionalLightsList[pos_lig].gfLIGColorLight); //Update the light's color and settings
        float Xd, Yd, Zd; //Update the light's direction
        Xd = engine.pligENGDirectionalLightsList[pos_lig].vec3LIGDirection.x;
        Yd = engine.pligENGDirectionalLightsList[pos_lig].vec3LIGDirection.y;
        Zd = engine.pligENGDirectionalLightsList[pos_lig].vec3LIGDirection.z;
        ImGui::SliderFloat("X Direction", &fEGINewDirectionX, -10.f, 10.f);
        ImGui::SliderFloat("Y Direction", &fEGINewDirectionY, -10.f, 10.f);
        ImGui::SliderFloat("Z Direction", &fEGINewDirectionZ, -10.f, 10.f);
        float difXd = fEGINewDirectionX - Xd; float difYd = fEGINewDirectionY - Yd; float difZd = fEGINewDirectionZ - Zd;
        if (difXd != 0 || difYd != 0 || difZd != 0) {
            engine.pligENGDirectionalLightsList[pos_lig].LIGChangeLightDirection(glm::vec3(fEGINewDirectionX, fEGINewDirectionY, fEGINewDirectionZ));
        }
        ImGui::SliderFloat("Ambient Intensity", &engine.pligENGDirectionalLightsList[pos_lig].gfLIGAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &engine.pligENGDirectionalLightsList[pos_lig].gfLIGDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &engine.pligENGDirectionalLightsList[pos_lig].gfLIGSpecularStrength, 0.f, 256.f);
    }
    //Cas d'une Point Light
    if (siEGISelectedEntity_point_light >= 0 && siEGISelectedType == 2) {
        ImGui::Checkbox("Active", &engine.pligENGPointLightsList[siEGISelectedEntity_point_light].bENTActive);
        ImGui::InputText("Name", &engine.pligENGPointLightsList[siEGISelectedEntity_point_light].strENTName);
        unsigned int pos_lig = siEGISelectedEntity_point_light;
        float Xl, Yl, Zl; //Update the cube light position (only when it's needed)
        Xl = engine.pligENGPointLightsList[pos_lig].vec3ENTWorldPosition.x;
        Yl = engine.pligENGPointLightsList[pos_lig].vec3ENTWorldPosition.y;
        Zl = engine.pligENGPointLightsList[pos_lig].vec3ENTWorldPosition.z;
        ImGui::SliderFloat("X", &fEGINewX, -10.f, 10.f);
        ImGui::SliderFloat("Y", &fEGINewY, -10.f, 10.f);
        ImGui::SliderFloat("Z", &fEGINewZ, -10.f, 10.f);
        float difXl = fEGINewX - Xl; float difYl = fEGINewY - Yl; float difZl = fEGINewZ - Zl;
        if (difXl != 0 || difYl != 0 || difZl != 0) {
            engine.pligENGPointLightsList[pos_lig].LIGChangeWorldPosition(glm::vec3(fEGINewX, fEGINewY, fEGINewZ));
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGPointLightsList[pos_lig], engine.pligENGPointLightsList[pos_lig].uiLIGId);
        }
        ImGui::Text("Position : X = %.3f, Y = %.3f, Z = %.3f", Xl, Yl, Zl);
        ImGui::SliderFloat("Scale Ratio", &gfEGINewRatio, 0.01f, 10.f); //Update the scale ratio of the light cube (when update is needed)
        GLfloat ScaleRatio = engine.pligENGPointLightsList[pos_lig].gfLIGScaleRatio;
        GLfloat difScaleRatio = gfEGINewRatio - ScaleRatio;
        if (difScaleRatio != 0) {
            engine.pligENGPointLightsList[pos_lig].LIGScaleEntitySize(gfEGINewRatio);
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGPointLightsList[pos_lig], engine.pligENGPointLightsList[pos_lig].uiLIGId);
        }
        ImGui::ColorEdit3("Light Color", engine.pligENGPointLightsList[pos_lig].gfLIGColorLight); //Update the light's color and settings
        float KC = engine.pligENGPointLightsList[pos_lig].fLIGPointKC;
        float KL = engine.pligENGPointLightsList[pos_lig].fLIGPointKL;
        float KQ = engine.pligENGPointLightsList[pos_lig].fLIGPointKQ;
        ImGui::SliderFloat("KC Constant", &fEGINewKC, 0.f, 1.f);
        ImGui::SliderFloat("KL Constant", &fEGINewKL, 0.f, 1.f);
        ImGui::SliderFloat("KQ Constant", &fEGINewKQ, 0.f, 1.f);
        float difKC = fEGINewKC - KC; float difKL = fEGINewKL - KL; float difKQ = fEGINewKQ - KQ;
        if (difKC != 0 || difKL != 0 || difKQ != 0) {
            engine.pligENGPointLightsList[pos_lig].LIGChangeKConstants(fEGINewKC, fEGINewKL, fEGINewKQ);
        }
        ImGui::SliderFloat("Ambient Intensity", &engine.pligENGPointLightsList[pos_lig].gfLIGAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &engine.pligENGPointLightsList[pos_lig].gfLIGDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &engine.pligENGPointLightsList[pos_lig].gfLIGSpecularStrength, 0.f, 256.f);
    }
    //Cas d'une Spotlight
    if (siEGISelectedEntity_spot_light >= 0 && siEGISelectedType == 3) {
        ImGui::Checkbox("Active", &engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].bENTActive);
        ImGui::InputText("Name", &engine.pligENGSpotLightsList[siEGISelectedEntity_spot_light].strENTName);
        unsigned int pos_lig = siEGISelectedEntity_spot_light;
        float Xl, Yl, Zl; //Update the cube light position (only when it's needed)
        Xl = engine.pligENGSpotLightsList[pos_lig].vec3ENTWorldPosition.x;
        Yl = engine.pligENGSpotLightsList[pos_lig].vec3ENTWorldPosition.y;
        Zl = engine.pligENGSpotLightsList[pos_lig].vec3ENTWorldPosition.z;
        ImGui::SliderFloat("X", &fEGINewX, -10.f, 10.f);
        ImGui::SliderFloat("Y", &fEGINewY, -10.f, 10.f);
        ImGui::SliderFloat("Z", &fEGINewZ, -10.f, 10.f);
        float difXl = fEGINewX - Xl; float difYl = fEGINewY - Yl; float difZl = fEGINewZ - Zl;
        if (difXl != 0 || difYl != 0 || difZl != 0) {
            engine.pligENGSpotLightsList[pos_lig].LIGChangeWorldPosition(glm::vec3(fEGINewX, fEGINewY, fEGINewZ));
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGSpotLightsList[pos_lig], engine.pligENGSpotLightsList[pos_lig].uiLIGId);
        }
        ImGui::Text("Position : X = %.3f, Y = %.3f, Z = %.3f", Xl, Yl, Zl);
        ImGui::SliderFloat("Scale Ratio", &gfEGINewRatio, 0.01f, 10.f); //Update the scale ratio of the light cube (when update is needed)
        GLfloat ScaleRatio = engine.pligENGSpotLightsList[pos_lig].gfLIGScaleRatio;
        GLfloat difScaleRatio = gfEGINewRatio - ScaleRatio;
        if (difScaleRatio != 0) {
            engine.pligENGSpotLightsList[pos_lig].LIGScaleEntitySize(gfEGINewRatio);
            rdrEGIRender.RDRCreateMandatoryForLight(engine, engine.pligENGSpotLightsList[pos_lig], engine.pligENGSpotLightsList[pos_lig].uiLIGId);
        }
        ImGui::ColorEdit3("Light Color", engine.pligENGSpotLightsList[pos_lig].gfLIGColorLight); //Update the light's color and settings
        float Xd, Yd, Zd; //Update the light's direction
        Xd = engine.pligENGSpotLightsList[pos_lig].vec3LIGDirection.x;
        Yd = engine.pligENGSpotLightsList[pos_lig].vec3LIGDirection.y;
        Zd = engine.pligENGSpotLightsList[pos_lig].vec3LIGDirection.z;
        ImGui::SliderFloat("X Direction", &fEGINewDirectionX, -10.f, 10.f);
        ImGui::SliderFloat("Y Direction", &fEGINewDirectionY, -10.f, 10.f);
        ImGui::SliderFloat("Z Direction", &fEGINewDirectionZ, -10.f, 10.f);
        float difXd = fEGINewDirectionX - Xd; float difYd = fEGINewDirectionY - Yd; float difZd = fEGINewDirectionZ - Zd;
        if (difXd != 0 || difYd != 0 || difZd != 0) {
            engine.pligENGSpotLightsList[pos_lig].LIGChangeLightDirection(glm::vec3(fEGINewDirectionX, fEGINewDirectionY, fEGINewDirectionZ));
        }
        ImGui::SliderFloat("Inner CutOff Angle", &engine.pligENGSpotLightsList[pos_lig].fLIGInnerCutOff, 0.f, 1.f);
        ImGui::SliderFloat("Outer CutOff Angle", &engine.pligENGSpotLightsList[pos_lig].fLIGOuterCutOff, 0.f, 1.f);
        ImGui::SliderFloat("Ambient Intensity", &engine.pligENGSpotLightsList[pos_lig].gfLIGAmbientIntensity, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse Strength", &engine.pligENGSpotLightsList[pos_lig].gfLIGDiffuseStrength, 0.f, 1.f);
        ImGui::SliderFloat("Specular Strength", &engine.pligENGSpotLightsList[pos_lig].gfLIGSpecularStrength, 0.f, 256.f);
    }
    ImGui::End();
}

void CEngineInterface::EGICameraModule(CEngine& engine, CCamera& camera) {

}

void CEngineInterface::EGIScriptEditorModule(CEngine& engine) {

}

void CEngineInterface::EGIDockingEngine(CEngine& engine) { //https://github.com/ocornut/imgui/issues/7067 documentation Novembre 2023
    //Dockspace
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("Orchid Engine", NULL, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::End();
}

void CEngineInterface::EGIDockingScriptEditor(CEngine& engine) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("Script Editor", NULL, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::End();
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
        int monitorXPos; int monitorYPos; int monitorWidth; int monitorHeight;
        glfwGetMonitorWorkarea(engine.pmonitorENGMonitor, &monitorXPos, &monitorYPos, &monitorWidth, &monitorHeight);
        glfwSetWindowMonitor(engine.pwindowENGWindow, NULL, 50, 50, monitorWidth, monitorHeight, 0);
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
}

//Do the pre-update process
void CEngineInterface::EGIPreUpdate(CEngine& engine) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void CEngineInterface::EGIPostUpdate(CEngine& engine) {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//Update the display of the interface at each frame time
void CEngineInterface::EGIUpdate(CEngine &engine) {
    //Interface Modules rendering methods
    EGIDockingEngine(engine);
    EGIEngineModule(engine);
    EGIScriptEditorModule(engine);
    EGIPostProcessingModule(engine);
    EGIInputsModule(engine);
    EGITexturesModule(engine);
    EGIEntitiesListsModule(engine);
    EGISelectedEntityModule(engine);
    EGINewEntityModule(engine);
    EGICameraModule(engine, engine.inpENGInputs.camINPChosenCamera);
    
    if (bEGIScriptEditorON) {
        EGIDockingScriptEditor(engine);
    }

    EGIMenuBar(engine);

    EGIWireframeUpdate();
    EGIFullscreenUpdate(engine);

    EGIInterfaceToEngine(engine);
}

//Framebuffer window module
void CEngineInterface::EGIFramebufferModule(CEngine& engine, GLuint texture) {
    ImGui::Begin("Framebuffer window"); {
        //ImGui::BeginChild("Framebuffer child"); //Using a child will auto fill the window
        ImVec2 wSize = ImGui::GetWindowSize();
        //ImGui::Text("test");
        ImGui::Image((ImTextureID)texture, ImVec2(engine.iENGScreenWidth, engine.iENGScreenHeight), ImVec2(0, 1), ImVec2(1, 0));
        //ImGui::EndChild();
    }
    ImGui::End();
}

std::string CEngineInterface::openfiledialog(char* filter, HWND owner) {
    std::wstring src;
    const std::wstring title = L"Select a File";
    std::wstring filename(MAX_PATH, L'\0');

    OPENFILENAMEW ofn = { };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner; //Put NULL
    ofn.lpstrFilter = L"Textures\0*.png*\0All\0*.*\0";
    ofn.lpstrFile = &filename[0];  // use the std::wstring buffer directly
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        src = filename;    //<----------Save filepath in global variable 
    }
    
    std::string converted_string_src(src.begin(), src.end());
    return converted_string_src;
}

//Test menubar + file explorer
void CEngineInterface::EGIMenuBar(CEngine& engine) {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Menu")) {
        if (ImGui::MenuItem("Open file", "Ctrl+O")) {
            std::string pathFile = openfiledialog((char*)"All Files (*.*)\0*.*\0", NULL);
            std::cout << "path file : " << pathFile << std::endl;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Project")) {
        if (ImGui::MenuItem("Add")) {
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window")) {
        if (ImGui::MenuItem("Script Editor")) {
            bEGIScriptEditorON = !bEGIScriptEditorON;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}