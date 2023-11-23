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

int main() {
    engine.ENGStart();
    CEngineInterface engineInterface = CEngineInterface(engine);
    CRender render = CRender();

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
    float  sensitivity = 1.0f;
    int nb_frames = 0; //Permet de fixer des events en fonction du temps (par exemple au bout de 3 sec changer le sens de translation)

    glfwSetKeyCallback(engine.pwindowENGWindow, key_callback);

    glm::vec3 Position_test_1(1.f, 0.f, 0.f);
    glm::vec3 Position_test_2(-0.3f, 1.f, 0.8f);
    glm::vec3 Position_nulle(0.f, 0.f, 0.f);

    CCube testCube_1 = CCube(engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(cube), Position_nulle, "INTERNAL/Shaders/core.vs", "INTERNAL/Shaders/core.frag", 0);
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

    CLight testLight_1 = CLight(engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(light), pos_cube_light, ENTITYlightColorFloat_1, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_1.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_1, testLight_1.uiLIGId);
    engine.ENGAddLightEntity(testLight_1);

    GLfloat ENTITYlightColorFloat_2[3] = { 0.f,0.5f,0.f };

    CLight testLight_2 = CLight(engine.uiENGGetNextFreeGlobalID(), engine.uiENGGetNextFreeEntityID(light), Position_test_2, ENTITYlightColorFloat_2, ambientIntensity, diffuseStrength, specularStrength, "INTERNAL/Shaders/light.vs", "INTERNAL/Shaders/light.frag", 3);
    testLight_2.LIGFirstTimeSetVerticesPosition();
    render.RDRCreateMandatoryForLight(engine, testLight_2, testLight_2.uiLIGId);
    engine.ENGAddLightEntity(testLight_2);


    engine.inpENGInputs.INPAddingKeybind("FORWARD", GLFW_KEY_W);
    engine.inpENGInputs.INPAddingKeybind("BACKWARD", GLFW_KEY_S);
    engine.inpENGInputs.INPAddingKeybind("LEFT_DASH", GLFW_KEY_A);
    engine.inpENGInputs.INPAddingKeybind("RIGHT_DASH", GLFW_KEY_D);
    engine.inpENGInputs.INPAddingKeybind("FLY_DOWN", GLFW_KEY_LEFT_ALT);
    engine.inpENGInputs.INPAddingKeybind("FLY_UP", GLFW_KEY_SPACE);
    engine.inpENGInputs.INPAddingKeybind("QUIT", GLFW_KEY_ESCAPE);
    engine.inpENGInputs.INPWriteMapBindingsOnTxtFile();
    std::string strMap = engine.inpENGInputs.strINPMapBindingsFromFileToString();
    engine.inpENGInputs.INPMapBindings(strMap);
    
    while (!glfwWindowShouldClose(engine.pwindowENGWindow)) { //Loop until the user closes the window

        engine.ENGFrameUpdate();
        render.RDRPostProcess(engine);

        engine.ENGLightUpdate();

        //IMGUI ELEMENTS
        /////// OLD CODE (BEFORE ENGINE UPDATE) ///////
        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Nom de la fenetre");
        ImGui::Text("Emplacement du texte");
        ImGui::ColorEdit4("Background color", background_color);
        ImGui::End();


        ImGui::Begin("Debug window");
        if (ImGui::SmallButton("Reset the Entire Logs file")) {
            CLogs logs = CLogs("Logs/logs.txt");
            logs.LOGResetEntireLogs(); 
            logs.~CLogs();
        }
        ImGui::Checkbox("Wireframe display", &wireframe_display_on);
        ImGui::End();

        ImGui::Begin("KeyBinds");
        if (ImGui::SmallButton("RIGHT_DASH")) {
            inputsTest.INPSetKeyFunctionNameToModify("RIGHT_DASH");
            inputsTest.INPSetStateInputs(2);
        }
        if (ImGui::SmallButton("LEFT_DASH")) {
            inputsTest.INPSetKeyFunctionNameToModify("LEFT_DASH");
            inputsTest.INPSetStateInputs(2);
        }
        if (ImGui::SmallButton("FORWARD_DASH")) {
            inputsTest.INPSetKeyFunctionNameToModify("FORWARD_DASH");
            inputsTest.INPSetStateInputs(2);
        }
        if (ImGui::SmallButton("BACKWARD_DASH")) {
            inputsTest.INPSetKeyFunctionNameToModify("BACKWARD_DASH");
            inputsTest.INPSetStateInputs(2);
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        */

        //Permet de fusionner les textures
        /*if (glfwGetKey(window, GLFW_KEY_Y)) {
            glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 0);
        }
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        */
        //glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

///////////////////////////////////////////////////////////////////////////////////////////////               
        //Maintenant on peut passer les informations
        //int texture_value_location = glGetUniformLocation(ourShader.Program, "textureValue");
        //glUniform1i(texture_value_location, texture_value);

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
        
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_LEFT)) {
            movement = glm::rotate(movement, glm::radians(-0.5f*sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_RIGHT)) {
            movement = glm::rotate(movement, glm::radians(0.5f*sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_UP)) {
            movement = glm::rotate(movement, glm::radians(0.5f*sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_DOWN)) {
            movement = glm::rotate(movement, glm::radians(-0.5f*sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_ESCAPE)) {
            glfwTerminate();
            return 0;
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


        //model = glm::rotate(model, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)); //Orthographic projection
        //view = glm::translate(view, glm::vec3(screenWidth / 2, screenHeight / 2, -700.0f)); //Orthographic projection
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.5f, 1.0f, 0.0f)); //Perspective projection
        //view = glm::translate(view, glm::vec3(0.0f,0.0f,translation)); //Perspective projection
        //movement = glm::vec3(0.0f, 0.0f, 0.0f);
        //std::cout << glfwGetTime();
        /*Pour tester avec une projection ortographique on peut utiliser
        model = glm::rotate(model, 0.5f, glm::vec3(1.0f,0.0f,0.0f));
        view = glm::translate(view, glm::vec3(screenWidth/2, screenHeight/2, -700.0f));*/

        GLint modelLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "view");
        GLint projLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "projection");
        GLint movLoc = glGetUniformLocation(engine.shaENGCoreShader.Program, "movement");
        engine.shaENGCoreShader.SHAUse();
        //On les passe aux shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(engine.inpENGInputs.camINPChosenCamera.mat4CAMView));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(engine.inpENGInputs.camINPChosenCamera.mat4CAMProjection));
        glUniformMatrix4fv(movLoc, 1, GL_FALSE, glm::value_ptr(movement));

        //LIGHT SHADERS
        GLint modelLightLoc = glGetUniformLocation(engine.shaENGLightShader.Program, "model");
        GLint viewLightLoc = glGetUniformLocation(engine.shaENGLightShader.Program, "view");
        GLint projLightLoc = glGetUniformLocation(engine.shaENGLightShader.Program, "projection");
        GLint movLightLoc = glGetUniformLocation(engine.shaENGLightShader.Program, "movement");
        engine.shaENGLightShader.SHAUse();
        glUniformMatrix4fv(modelLightLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLightLoc, 1, GL_FALSE, glm::value_ptr(engine.inpENGInputs.camINPChosenCamera.mat4CAMView));
        glUniformMatrix4fv(projLightLoc, 1, GL_FALSE, glm::value_ptr(engine.inpENGInputs.camINPChosenCamera.mat4CAMProjection));
        glUniformMatrix4fv(movLightLoc, 1, GL_FALSE, glm::value_ptr(movement));
        
        engineInterface.EGIUpdate(engine);
        render.RDRRenderingCubes(engine);
        render.RDRRenderingLightCubes(engine);
        engine.ENGPreUpdateInputsValues();

        glfwSwapBuffers(engine.pwindowENGWindow); //Swap front and back buffers

        //Penser à bien mettre cette fonction dans CEngine car sinon le moteur ne va pas se limiter à un certain nombre de FPS
        if ( (1000.0 / engine.iENGGetFpsLimit()) > (1000*engine.dENGDiffTime) ){
            std::this_thread::sleep_for(std::chrono::milliseconds((1000 / engine.iENGGetFpsLimit()) - (unsigned int)(1000 * engine.dENGDiffTime)));
        }
        nb_frames++;
    }

    render.~CRender();
    engineInterface.~CEngineInterface();
    engine.~CEngine();

    glfwTerminate(); 
    return 0;
}