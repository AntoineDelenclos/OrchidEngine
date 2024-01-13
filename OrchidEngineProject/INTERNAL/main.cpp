//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "CPostProcessing.h"
#include "CLogs.h"
#include "GlobalTools.h"
#include "CInputs.h"
#include "Entities/CEntity.h"
#include "CEngineInterface.h"
#include "CRender.h"
#include "Entities/CModel.h"

CEngine engine = CEngine();

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    engine.inpENGInputs.INPKeyCallback(window, key, scancode, action, mods);
}
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    engine.inpENGInputs.INPMouseCallback(window, xpos, ypos);
}
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    engine.inpENGInputs.INPScrollCallback(window, xoffset, yoffset);
}
static void handleWindowFocus(GLFWwindow* window, int focused) {
    if (focused == GL_TRUE) {
        engine.bENGHasFocus = true;
    }
    else {
        engine.bENGHasFocus = false;
    }
}

int main() {
    engine.ENGStart();
    CEngineInterface engineInterface = CEngineInterface(engine);
    CRender render = CRender();

    glfwSetWindowFocusCallback(engine.pwindowENGWindow, handleWindowFocus);

    glm::vec3 lightColor = glm::vec3(0.f, 0.66f, 0.4f);
    GLfloat lightColorFloat[3] = { 0.89f,0.66f,0.4f };
    GLfloat ambientIntensity = 0.3f;
    GLfloat transparency = 1.0f;
    GLfloat diffuseStrength = 0.5f;
    GLfloat specularStrength = 0.5f;
    glm::vec3 pos_cube_light = { 1.3f, 1.3f, 1.3f };
    //glm::mat4 lightModel = glm::mat4(1.0f);
    //lightModel = glm::translate(lightModel, pos_cube_light);
    //move_cube_coordinates(cube_light, pos_cube_light);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    engine.shaENGCoreShader.SHAUse();
    glUniform1i(glGetUniformLocation(engine.shaENGCoreShader.Program, "ourTexture"), 0);
    glUniform3f(glGetUniformLocation(engine.shaENGCoreShader.Program, "viewPos"), engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.x, engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.y, engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.z);
    
    //glBindTexture(GL_TEXTURE_2D, 0);
    //gluLookAt(0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    //glm::mat4 model_test;
    glm::mat4 projection;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    
    glm::mat4 movement = glm::mat4(1.0f); //Permet de définir les movements de caméra (mais donc bizarre car on pourrait travailler directement sur view ?)
    
    //fov : 45°, aspect ratio, near clipping plane, far clipping plane
    projection = glm::perspective(90.0f, (GLfloat)engine.iENGScreenWidth/(GLfloat)engine.iENGScreenHeight, 0.1f, 1000.0f);
    //projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);
    float rotation = 0.0f;
    float translation = -0.00f;
    float acceleration_rotation = 1.0f;
    float acceleration_translation = 1.0f;
    int nb_frames = 0; //Permet de fixer des events en fonction du temps (par exemple au bout de 3 sec changer le sens de translation)

    glfwSetKeyCallback(engine.pwindowENGWindow, key_callback);

    glm::vec3 Position_test_1(1.f, 0.f, 0.f);
    glm::vec3 Position_test_2(-0.3f, 1.f, 0.8f);
    glm::vec3 Position_nulle(0.f, 0.f, 0.f);

    CCube testCube_1 = CCube(engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(cube), Position_nulle, "INTERNAL/Shaders/core.vs", "INTERNAL/Shaders/core.frag", 2);
    testCube_1.CUBFirstTimeSetVerticesPosition();
    testCube_1.CUBScaleEntitySize(1.f);
    render.RDRCreateMandatoryForCube(engine, testCube_1, testCube_1.uiCUBId); //Ici le numero correspond au numero de l'entité et donc de la paire (VAO,VBO)
    engine.ENGAddCubeEntity(testCube_1);

    CCube testCube_2 = CCube(engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(cube) , Position_test_1, "INTERNAL/Shaders/core.vs", "INTERNAL/Shaders/core.frag", 1); //Le dernier numéro correspond aux textures bind dans le moteur
    testCube_2.CUBFirstTimeSetVerticesPosition();
    testCube_2.CUBScaleEntitySize(2.f);
    render.RDRCreateMandatoryForCube(engine, testCube_2, testCube_2.uiCUBId);
    engine.ENGAddCubeEntity(testCube_2);

    GLfloat ENTITYlightColorFloat_1[3] = { 1.f,0.f,1.f };

    CLight testLight_1 = CLight(directional, engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(dir_light), pos_cube_light, glm::vec3(1.f, 1.f, 1.f), ENTITYlightColorFloat_1, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_1.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_1, testLight_1.uiLIGId);
    engine.ENGAddLightEntity(testLight_1);
    
    GLfloat colorlight2[3] = {0.f, 0.5f, 0.f};

    CLight testLight_2 = CLight(directional, engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(dir_light), Position_test_2, glm::vec3(1.f, 1.f, 1.f), colorlight2, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_2.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_2, testLight_2.uiLIGId);
    engine.ENGAddLightEntity(testLight_2);

    glm::vec3 Position_test_3 = glm::vec3(2.f, 2.f, 2.f);
    GLfloat colorlight3[3] = { 1.f, 1.f, 1.f };

    CLight testLight_3 = CLight(point, engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(point_light), Position_test_3, colorlight3, 1.0f, 0.09f, 0.032f, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_3.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_3, testLight_3.uiLIGId);
    engine.ENGAddLightEntity(testLight_3);

    glm::vec3 Position_test_4 = glm::vec3(1.f, 3.f, 2.f);
    GLfloat colorlight4[3] = { 0.4f, 1.f, 0.2f };

    CLight testLight_4 = CLight(spot, engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(spot_light), Position_test_4, glm::vec3(1.f, 1.f, 1.f), 0.91f, 0.82f, colorlight4, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_4.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_4, testLight_4.uiLIGId);
    engine.ENGAddLightEntity(testLight_4);

    glm::vec3 Position_test_5 = glm::vec3(2.f, 1.f, 0.f);

    CLight testLight_5 = CLight(directional, engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(dir_light), Position_test_5, glm::vec3(1.f, 1.f, 1.f), colorlight2, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_5.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_5, testLight_5.uiLIGId);
    engine.ENGAddLightEntity(testLight_5);

    //for (int test_performance = 0; test_performance<100; test_performance++) {
    //    CCube testCube = CCube(engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(cube), glm::vec3(test_performance * 0.2f,test_performance*0.2f, test_performance * 0.2f), "INTERNAL/Shaders/core.vs", "INTERNAL/Shaders/core.frag", 0);
    //    testCube.CUBFirstTimeSetVerticesPosition();
    //    testCube.CUBScaleEntitySize(1.f);
    //    render.RDRCreateMandatoryForCube(engine, testCube, testCube.uiCUBId); //Ici le numero correspond au numero de l'entité et donc de la paire (VAO,VBO)
    //    engine.ENGAddCubeEntity(testCube);
    //}

    //Key bindings
    engine.inpENGInputs.INPAddingKeybind("FORWARD", GLFW_KEY_W);
    engine.inpENGInputs.INPAddingKeybind("BACKWARD", GLFW_KEY_S);
    engine.inpENGInputs.INPAddingKeybind("LEFT_DASH", GLFW_KEY_A);
    engine.inpENGInputs.INPAddingKeybind("RIGHT_DASH", GLFW_KEY_D);
    engine.inpENGInputs.INPAddingKeybind("FLY_DOWN", GLFW_KEY_LEFT_ALT);
    engine.inpENGInputs.INPAddingKeybind("FLY_UP", GLFW_KEY_SPACE);
    engine.inpENGInputs.INPAddingKeybind("ROTATE_VIEW_UP", GLFW_KEY_UP);
    engine.inpENGInputs.INPAddingKeybind("ROTATE_VIEW_DOWN", GLFW_KEY_DOWN);
    engine.inpENGInputs.INPAddingKeybind("ROTATE_VIEW_LEFT", GLFW_KEY_LEFT);
    engine.inpENGInputs.INPAddingKeybind("ROTATE_VIEW_RIGHT", GLFW_KEY_RIGHT);
    engine.inpENGInputs.INPAddingKeybind("QUIT", GLFW_KEY_ESCAPE);
    engine.inpENGInputs.INPWriteMapBindingsOnTxtFile();
    std::string strMap = engine.inpENGInputs.strINPMapBindingsFromFileToString();
    engine.inpENGInputs.INPMapBindings(strMap);


    //////////////////////// TEST CUSTOM FRAMEBUFFER ///////////////////////////////
    
    //Framebuffer config
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    //Color attachment
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, engine.uiENGWidth, engine.uiENGHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, engine.uiENGWidth, engine.uiENGHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        CException exception(FRAMEBUFFER_INCOMPLETE);
        throw(exception);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind the framebuffer

    
    engine.shaENGPostProcessShader.SHAUse();
    glUniform1i(glGetUniformLocation(engine.shaENGPostProcessShader.Program, "ourTexture"), 0);

    CShader modelShaders = CShader("INTERNAL/Shaders/model_loading.vs", "INTERNAL/Shaders/model_loading.frag"); //La racine est le .vcxproj

    CModel modelBackpack = CModel("../../Assets/Models/backpack/backpack.obj");
    //CModel modelBackpack2 = CModel("../../Assets/Models/test2/untitled.obj");

    while (!glfwWindowShouldClose(engine.pwindowENGWindow)) { //Loop until the user closes the window
        //glViewport(0, 0, engine.iENGScreenWidth, engine.iENGScreenHeight); //Redimensionne relativement à la taille d'écran
        
        //////////////////////TEST FRAMEBUFFER///////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //render.RDRPostProcess(engine);

        engine.ENGLightUpdate();

        //Permet de fusionner les textures
        /*if (glfwGetKey(window, GLFW_KEY_Y)) {
            glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 0);
        }
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        */
        //glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

        if (nb_frames > 180) {
            translation = -1 * translation;
            nb_frames = 0;
        }

        engine.inpENGInputs.processInputs(engine.pwindowENGWindow);
        glfwSetKeyCallback(engine.pwindowENGWindow, key_callback);
        //Pour activer/désactiver la souris
        if (engine.inpENGInputs.iINPCameraState == 0) {
            glfwSetInputMode(engine.pwindowENGWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            engine.inpENGInputs.bINPFirstMouse = true; //Empêche le calcul du décalage entre la position de la souris à la fin du mode déplacement
            //de vue et entre le début de la prochaine fois où on entre dans ce mode.
            glfwSetCursorPosCallback(engine.pwindowENGWindow, ImGui_ImplGlfw_CursorPosCallback); //MDR Il suffisait de call le callback d'imgui
            glfwSetScrollCallback(engine.pwindowENGWindow, ImGui_ImplGlfw_ScrollCallback);
            //ImGui_ImplGlfw_RestoreCallbacks(engine.pwindowENGWindow); // Crée un crash abort()
            // Lien erreur similaire avec d'autres callback de glfw et imgui https://github.com/ocornut/imgui/issues/5708 
            // https://github.com/ocornut/imgui/issues/1759#issue-316130528
            // https://github.com/ocornut/imgui/issues/4981
        }
        if (engine.inpENGInputs.iINPCameraState == 2) {
            glfwSetInputMode(engine.pwindowENGWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(engine.pwindowENGWindow, mouse_callback);
            glfwSetScrollCallback(engine.pwindowENGWindow, scroll_callback);
        }

        //KEY_TEST        
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_B)) {
            CLogs logs = CLogs("Logs/logs.txt");
            std::string Text = "test des logs\nca marche\n";
            logs.LOGWritingEntireLogs(Text);
            logs.~CLogs();
        }

        rotation = acceleration_rotation * rotation;
        translation = acceleration_translation * translation;
        //std::cout << rotation;
        //Create transformations
        //Vector movement
        
        //movement = glm::vec3(0.0f, 0.0f, 0.0f);
        //std::cout << glfwGetTime();
        /*Pour tester avec une projection ortographique on peut utiliser
        model = glm::rotate(model, 0.5f, glm::vec3(1.0f,0.0f,0.0f));
        view = glm::translate(view, glm::vec3(screenWidth/2, screenHeight/2, -700.0f));*/

        engine.ENGCameraUpdate();
        engineInterface.EGIPreUpdate(engine);
        engineInterface.EGIUpdate(engine);
        render.RDRRenderingCubes(engine);
        render.RDRRenderingLightCubes(engine);
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        modelBackpack.Draw(modelShaders);
        //modelBackpack2.Draw(modelShaders);
        engine.ENGPreUpdateInputsValues();
       

        /*engine.shaENGPostProcessShader.SHAUse();
        glUniform1i(glGetUniformLocation(engine.shaENGPostProcessShader.Program, "ourTexture"), 0);*/

        render.RDRPostProcess(engine);

        //On reset le framebuffer après chaque frame.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat constantColor[] = { 0.3f,0.7f,1.0f,1.0f };
        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constantColor);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //render.RDRPostProcess(engine);

        engineInterface.EGIFramebufferModule(engine, 8); //Il faut penser à mettre le framebuffer en numéro 0
        engineInterface.EGIPostUpdate(engine);

        /*glViewport(100, 50, engine.iENGScreenWidth, engine.iENGScreenHeight);
        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        //GLuint TOF;
        glGenTextures(1, &engine.framebuffer_texture);
        glBindTexture(GL_TEXTURE_2D, engine.framebuffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 300, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        render.RDRPostProcess(engine);

        engine.ENGLightUpdate();
        engine.ENGCameraUpdate();
        engineInterface.EGIPreUpdate(engine);
        engineInterface.EGIUpdate(engine);
        engineInterface.EGIPostUpdate(engine);
        render.RDRRenderingCubes(engine);
        render.RDRRenderingLightCubes(engine);
        engine.ENGPreUpdateInputsValues();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); //On va debind le framebuffer puis le rebind au début de chaque frame*/
        //glDeleteFramebuffers(1, &fbo);
        //glDeleteTextures(1, &TOF);
        
        
        //Test viewport imgui rendering
        //GLuint fbo;
        //glGenFramebuffers(1, &fbo);
        //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        
        //GLuint colorTexture;
        //glGenTextures(1, &colorTexture);
        //glBindTexture(GL_TEXTURE_2D, colorTexture);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, engine.iENGScreenWidth, engine.iENGScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
        //glViewport(200, 200, 400, 400);
        //render.RDRPostProcess(engine);

        //engine.ENGLightUpdate();
        //engine.ENGCameraUpdate();
        //engineInterface.EGIPreUpdate(engine);
        //engineInterface.EGIUpdate(engine);
        //engineInterface.EGIViewportTest(engine, colorTexture);
        //engineInterface.EGIPostUpdate(engine);
        //render.RDRRenderingCubes(engine);
        //render.RDRRenderingLightCubes(engine);
        //engine.ENGPreUpdateInputsValues();
        ////Fin du test viewport

        glfwSwapBuffers(engine.pwindowENGWindow); //Swap front and back buffers
        engine.ENGFrameUpdate(); //Contains glfwPollEvents()

        //Penser à bien mettre cette fonction dans CEngine car sinon le moteur ne va pas se limiter à un certain nombre de FPS
        if ( (1000.0 / engine.iENGGetFpsLimit()) > (1000*engine.dENGDiffTime) ){
            std::this_thread::sleep_for(std::chrono::milliseconds((1000 / engine.iENGGetFpsLimit()) - (unsigned int)(1000 * engine.dENGDiffTime)));
        }
        nb_frames++;
    }

    //render.~CRender();
    //engineInterface.~CEngineInterface(); //Enlève le abort mais bon c'est pas très propre sans l'appel au destructeur
    //engine.~CEngine();

    //glfwTerminate(); 
    return 0;
}