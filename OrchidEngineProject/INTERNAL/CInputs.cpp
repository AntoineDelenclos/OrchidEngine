#include "CInputs.h"

CInputs::CInputs() {
    mat4INPMovement = glm::mat4(1.0f);
    strINPKeyBindsPathFile = "Keybinds/keybinds.txt";
    iINPCameraState = 0;
    dINPSensitivity = 0.05f;
    bINPFirstMouse = true;
    camINPChosenCamera = CCamera();
}

CInputs::CInputs(CCamera& camera) {
    mat4INPMovement = glm::mat4(1.0f);
    strINPKeyBindsPathFile = "Keybinds/keybinds.txt";
    iINPCameraState = 0;
    dINPSensitivity = 0.1f;
    camINPChosenCamera = camera;
    bINPFirstMouse = true;
}

CInputs::~CInputs() {
    //Il faut faire en sorte que lorsqu'on close le programme il ne reste qu'un fichier d'inputs nommé keybinds.txt
    //On supprime l'autre 
    //https://cplusplus.com/reference/fstream/fstream/swap-free/
}

void CInputs::INPSetMovement(glm::mat4 movement) {
    this->mat4INPMovement = movement;
}

void CInputs::INPSetKeyBindsPathFile(std::string path) {
    this->strINPKeyBindsPathFile = path;
}

glm::mat4 CInputs::mat4INPGetMovement() {
    return this->mat4INPMovement;
}

std::string CInputs::strINPGetKeyBindsPathFile() {
    return this->strINPKeyBindsPathFile;
}

///////////////////////////// CALLBACK FUNCTIONS ////////////////////////////////////////////////////////////

void CInputs::processInputs(GLFWwindow* window) {
    //Movements
    if (glfwGetKey(window, mapStrIntINPKeybinds["FORWARD"])) {
        camINPChosenCamera.vec3CAMCameraPosition += (float)(camINPChosenCamera.fCAMCameraSpeedMovement * dINPDiffTime) * camINPChosenCamera.vec3CAMCameraFront;
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["BACKWARD"])) {
        camINPChosenCamera.vec3CAMCameraPosition -= (float)(camINPChosenCamera.fCAMCameraSpeedMovement * dINPDiffTime) * camINPChosenCamera.vec3CAMCameraFront;
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["LEFT_DASH"])) {
        camINPChosenCamera.vec3CAMCameraPosition -= glm::normalize(glm::cross(camINPChosenCamera.vec3CAMCameraFront, camINPChosenCamera.vec3CAMCameraUp)) * (float)(camINPChosenCamera.fCAMCameraSpeedMovement * dINPDiffTime);
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["RIGHT_DASH"])) {
        camINPChosenCamera.vec3CAMCameraPosition += glm::normalize(glm::cross(camINPChosenCamera.vec3CAMCameraFront, camINPChosenCamera.vec3CAMCameraUp)) * (float)(camINPChosenCamera.fCAMCameraSpeedMovement * dINPDiffTime);
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["FLY_DOWN"])) {
        camINPChosenCamera.vec3CAMCameraPosition -= (float)(camINPChosenCamera.fCAMCameraSpeedMovement * dINPDiffTime) * camINPChosenCamera.vec3CAMCameraUp;
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["FLY_UP"])) {
        camINPChosenCamera.vec3CAMCameraPosition += (float)(camINPChosenCamera.fCAMCameraSpeedMovement * dINPDiffTime) * camINPChosenCamera.vec3CAMCameraUp;
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["ROTATE_VIEW_LEFT"])) { //Essayer avec camINPChosenCamera.mat4CAMView
        mat4INPMovement = glm::rotate(mat4INPMovement, glm::radians((GLfloat)(-6.f * dINPSensitivity)), glm::vec3(0.0f,1.0f,0.0f));
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["ROTATE_VIEW_RIGHT"])) {
        mat4INPMovement = glm::rotate(mat4INPMovement, glm::radians((GLfloat)(6.f * dINPSensitivity)), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["ROTATE_VIEW_UP"])) {
        mat4INPMovement = glm::rotate(mat4INPMovement, glm::radians((GLfloat)(6.f * dINPSensitivity)), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, mapStrIntINPKeybinds["ROTATE_VIEW_DOWN"])) {
        mat4INPMovement = glm::rotate(mat4INPMovement, glm::radians((GLfloat)(-6.f * dINPSensitivity)), glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

void CInputs::INPKeyCallback(GLFWwindow* window_, int key, int scancode, int action, int mods) {
    //View camera
    if (key == GLFW_KEY_V && action == GLFW_PRESS && iINPCameraState == 0) {
        iINPCameraState = 1;
    }
    if (key == GLFW_KEY_V && action == GLFW_RELEASE && iINPCameraState == 1) {
        iINPCameraState = 2;
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS && iINPCameraState == 2) {
        iINPCameraState = 0;
    }
    //Engine
    if (key == mapStrIntINPKeybinds["QUIT"] && action == GLFW_PRESS) {
        glfwTerminate();
    }
}

void CInputs::INPMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (iINPCameraState == 2) {
        if (bINPFirstMouse) {
            dINPLastMouseX = xpos;
            dINPLastMouseY = ypos;
            bINPFirstMouse = false;
        }
        double xoffset = xpos - dINPLastMouseX;
        double yoffset = dINPLastMouseY - ypos; // reversed since y-coordinates range from bottom to top
        dINPLastMouseX = xpos;
        dINPLastMouseY = ypos;

        xoffset *= dINPSensitivity;
        yoffset *= dINPSensitivity;

        camINPChosenCamera.fCAMYaw += (GLfloat)xoffset;
        camINPChosenCamera.fCAMPitch += (GLfloat)yoffset;

        if (camINPChosenCamera.fCAMPitch > (GLfloat)89.f) {
            camINPChosenCamera.fCAMPitch = (GLfloat)89.f;
        }
        if (camINPChosenCamera.fCAMPitch < (GLfloat)(- 89.f)) {
            camINPChosenCamera.fCAMPitch = (GLfloat)(- 89.f);
        }

        glm::vec3 front;
        front.x = cos(glm::radians(camINPChosenCamera.fCAMYaw)) * cos(glm::radians(camINPChosenCamera.fCAMPitch));
        front.y = sin(glm::radians(camINPChosenCamera.fCAMPitch));
        front.z = sin(glm::radians(camINPChosenCamera.fCAMYaw)) * cos(glm::radians(camINPChosenCamera.fCAMPitch));
        camINPChosenCamera.vec3CAMCameraFront = glm::normalize(front);
        //L'update camINPChosenCamera.CAMUpdate() se fait dans une méthode de CEngine() donc pas besoin de réupdate dans le callback (sinon on ferai une mise à jour 2 fois par
        // frame lorsqu'on désactive le curseur)
    }
}

void CInputs::INPScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camINPChosenCamera.fCAMFovZoom -= (float)yoffset;
    if (camINPChosenCamera.fCAMFovZoom < 1.f) {
        camINPChosenCamera.fCAMFovZoom = 1.f;
    }
    if (camINPChosenCamera.fCAMFovZoom > 45.f) {
        camINPChosenCamera.fCAMFovZoom = 45.f;
    }
}

//////////////////////////////////// NEW KEYBINDS MAPPING SYSTEM //////////////////////////////////

void CInputs::INPAddingKeybind(std::string function_name, int key) {
    mapStrIntINPKeybinds[function_name] = key;
}

void CInputs::INPWriteMapBindingsOnTxtFile() {
    int number_of_binds = (int)mapStrIntINPKeybinds.size();
    if (number_of_binds == 0) {
        CException exception(MAP_KEYBINDS_EMPTY);
        throw(exception);
    }
    else {
        std::fstream fINPKeyBindsFile;
        fINPKeyBindsFile.open(strINPKeyBindsPathFile); //On va écraser l'ancien texte du fichier
        if (!fINPKeyBindsFile.is_open()) {
            CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
            throw(exception);
        }
        else if (fINPKeyBindsFile.is_open()) {
            std::map<std::string, int>::iterator map_iterator = mapStrIntINPKeybinds.begin();
            while (map_iterator != mapStrIntINPKeybinds.end()) {
                fINPKeyBindsFile << map_iterator->first << ";" << map_iterator->second << ";";
                ++map_iterator;
            }
        }
        fINPKeyBindsFile.close();
    }
}

std::string CInputs::strINPMapBindingsFromFileToString() {
    std::ifstream iFile;
    iFile.open(strINPKeyBindsPathFile);
    std::string strFromTextFile;
    if (!iFile.is_open()) {
        CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
        throw(exception);
    }
    else if (iFile.is_open()) {
        iFile >> strFromTextFile; //On a bien une string qui contient la map entière
    }
    return strFromTextFile;
}

void CInputs::INPMapBindings(std::string bindings_from_file) {
    int parseState = 0;
    std::string function_name;
    std::string key;
    //On va mettre un iterator de string et on va lire caractère par caractère.
    for (std::string::iterator strIterator = bindings_from_file.begin(); strIterator != bindings_from_file.end(); strIterator++) {
        if (parseState == 0) {
            if (*strIterator == ';') {
                parseState = 1;
            }
            else {
                function_name += *strIterator;
            }
        }
        if (parseState == 1) {
            if (*strIterator != ';') {
                parseState = 2;
            }
        }
        if (parseState == 2) {
            if (*strIterator == ';') {
                int key_value = stoi(key);
                INPAddingKeybind(function_name, key_value);
                function_name = "";
                key = "";
                parseState = 0;
            }
            else {
                key += *strIterator;
            }
        }
    }
}