#include "stb_image.h"
//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "CPostProcessing.h"
#include "CLogs.h"
#include "GlobalTools.h"
#include "CInputs.h"
#include "CParser.h"
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
    //CParser parser = CParser(inputs.strINPGetKeyBindsPathFile(), ":;");
    //parser.inpPARInputsFromParser(inputs.strINPGetKeyBindsPathFile());
    
//////////////////////////////////// OLD ENGINE |^ ///////////////////////////////////
    engine.ENGStart();
    CEngineInterface engineInterface = CEngineInterface(engine);
    CRender render = CRender();

    float lastX = engine.iENGScreenWidth / 2;
    float lastY = engine.iENGScreenHeight / 2;


    //Cube light
    GLfloat cube_light[]{
        -0.05f, -0.05f, -0.05f,  0.0f, 0.0f,
        0.05f, -0.05f, -0.05f,  1.0f, 0.0f,
        0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
        0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
        -0.05f,  0.05f, -0.05f,  0.0f, 1.0f,
        -0.05f, -0.05f, -0.05f,  0.0f, 0.0f,

        -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
        0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
        0.05f,  0.05f,  0.05f,  1.0f, 1.0f,
        0.05f,  0.05f,  0.05f,  1.0f, 1.0f,
        -0.05f,  0.05f,  0.05f,  0.0f, 1.0f,
        -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,

        -0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
        -0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
        -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
        -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
        -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
        -0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

        0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
        0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
        0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
        0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
        0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
        0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

        -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
        0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
        0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
        0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
        -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
        -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,

        -0.05f,  0.05f, -0.05f,  0.0f, 1.0f,
        0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
        0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
        0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
        -0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
        -0.05f,  0.05f, -0.05f,  0.0f, 1.0f
    };
    glm::vec3 lightColor = glm::vec3(0.89f, 0.66f, 0.4f);
    GLfloat ambientIntensity = 0.3f;
    GLfloat transparency = 1.0f;
    GLfloat specularStrength = 0.5f;
    glm::vec3 pos_cube_light = { 1.2f, 1.0f, 2.0f };
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, pos_cube_light);
    //move_cube_coordinates(cube_light, pos_cube_light);
    GLuint VAO_cube_light, VBO_cube_light;
    glGenVertexArrays(1, &VAO_cube_light);
    glGenBuffers(1, &VBO_cube_light);
    glBindVertexArray(VAO_cube_light);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube_light);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(cube_light) / 4, cube_light, GL_STATIC_DRAW); //Le static draw fait que nous ne pouvons pas allouer plus d'espaces à ce VAO quand ca run
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    GLuint VAO_light;
    glGenVertexArrays(1, &VAO_light);
    glBindVertexArray(VAO_light);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //TEXTURE 2 (for light)
    GLuint texture2;
    unsigned char* image2;
    int width2, height2;
    int nrChannels2;
    /*glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image2 = stbi_load("../../../Assets/white_light_texture.png", &width2, &height2, &nrChannels2, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image2);
    */
    engine.shaENGCoreShader.SHAUse();
    glUniform1i(glGetUniformLocation(engine.shaENGCoreShader.Program, "ourTexture"), 0);
    glUniform3f(glGetUniformLocation(engine.shaENGCoreShader.Program, "viewPos"), engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.x, engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.y, engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition.z);
    /*glUniform3f(glGetUniformLocation(engine.shaENGCoreShader.Program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(glGetUniformLocation(engine.shaENGCoreShader.Program, "lightPos"), pos_cube_light.x, pos_cube_light.y, pos_cube_light.z);
    glUniform1f(glGetUniformLocation(engine.shaENGCoreShader.Program, "lightAmbientIntensity"), ambientIntensity);
    glUniform1f(glGetUniformLocation(engine.shaENGCoreShader.Program, "lightSpecularStrength"), specularStrength);
    glUniform1f(glGetUniformLocation(engine.shaENGCoreShader.Program, "transparency"), transparency);*/

    //lightCubeShader.Use();
    //glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));

    //lightShader.Use();
    //glUniform4f(glGetUniformLocation(lightShader.Program, "lightcolor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

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
    //glfwSetCursorPosCallback(engine.pwindowENGWindow, mouse_callback);
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);    

    glm::vec3 Position_test_1(1.3f, -1.3f, 1.f);
    glm::vec3 Position_test_2(-0.3f, 1.f, 0.8f);
    glm::vec3 Position_nulle(0.f, 0.f, 0.f);


    CEntity testEntity_1 = CEntity(cube, engine.uiENGGetNextFreeEntityID(), Position_nulle, "core.vs", "core.frag", 0);
    CEntity testEntity_2 = CEntity(cube, engine.uiENGGetNextFreeEntityID() + 1, Position_test_2, "core.vs", "core.frag", 1); //Le dernier numéro correspond aux textures bind dans le moteur

    testEntity_1.ENTChangeWorldPosition(testEntity_1.vec3ENTWorldPosition);
    testEntity_2.ENTChangeWorldPosition(testEntity_2.vec3ENTWorldPosition);

    testEntity_1.ENTScaleEntitySize(1.5f);
    testEntity_2.ENTScaleEntitySize(3.f);

    render.RDRCreateMandatoryForEntity(engine, testEntity_1, testEntity_1.uiENTId); //Ici le numero correspond au numero de l'entité et donc de la paire (VAO,VBO)
    render.RDRCreateMandatoryForEntity(engine, testEntity_2, testEntity_2.uiENTId);

    engine.ENGAddCubeEntity(testEntity_1);
    engine.ENGAddCubeEntity(testEntity_2);

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



        //On active maintenant les textures avant d'utiliser les shaders
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        */

        //Permet de fusionner les textures
        /*if (glfwGetKey(window, GLFW_KEY_Y)) {
            glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 0);
        }
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        */
        //glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

///////////////////////////////////////////////////////////////////////////////////////////////

        //Draw light cube
        /*glUniform1i(texture_value_location, 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO_light);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_light)/20);
        glBindVertexArray(0);*/

        engine.shaENGCoreShader.SHAUse();
        //Maintenant on peut passer les informations
        //int texture_value_location = glGetUniformLocation(ourShader.Program, "textureValue");
        //glUniform1i(texture_value_location, texture_value);

        if (nb_frames > 180) {
            translation = -1 * translation;
            nb_frames = 0;
        }

        engine.inpENGInputs.processInputs(engine.pwindowENGWindow);

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
        engineInterface.EGIUpdate(engine);
        
        //Mouvement
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_W)) {
            engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition += (float)(engine.inpENGInputs.camINPChosenCamera.fCAMCameraSpeedMovement * engine.dENGDiffTime ) * engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraFront;
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_S)) {
            engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition -= (float)(engine.inpENGInputs.camINPChosenCamera.fCAMCameraSpeedMovement * engine.dENGDiffTime) * engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraFront;
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_A)) {
            engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition -= glm::normalize(glm::cross(engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraFront, engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraUp)) * (float)(engine.inpENGInputs.camINPChosenCamera.fCAMCameraSpeedMovement * engine.dENGDiffTime);
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_D)) {
            engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition += glm::normalize(glm::cross(engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraFront, engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraUp)) * (float)(engine.inpENGInputs.camINPChosenCamera.fCAMCameraSpeedMovement * engine.dENGDiffTime);
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_SPACE)) {
            engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition += (float)(engine.inpENGInputs.camINPChosenCamera.fCAMCameraSpeedMovement * engine.dENGDiffTime) * engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraUp;
        }
        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_LEFT_ALT)) {
            engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraPosition -= (float)(engine.inpENGInputs.camINPChosenCamera.fCAMCameraSpeedMovement * engine.dENGDiffTime) * engine.inpENGInputs.camINPChosenCamera.vec3CAMCameraUp;
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
            engine.inpENGInputs.INPReplaceWithGoodFile();
            glfwTerminate();
            return 0;
        }

        if (glfwGetKey(engine.pwindowENGWindow, GLFW_KEY_R)) {
            engine.inpENGInputs.INPInputsFromFile();
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
        
        //On les passe aux shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(engine.inpENGInputs.camINPChosenCamera.mat4CAMView));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(engine.inpENGInputs.camINPChosenCamera.mat4CAMProjection));
        glUniformMatrix4fv(movLoc, 1, GL_FALSE, glm::value_ptr(movement));

        render.RDRRenderingEntities(engine);

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

    //Desalloue de la mémoire:
    //glDeleteVertexArrays(1, &VAO_1);
    //glDeleteBuffers(1, &VBO_1);
    glfwTerminate(); 
    return 0;
}