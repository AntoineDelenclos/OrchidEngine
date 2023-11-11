#include "CInputs.h"

CInputs::CInputs() {
    mat4INPMovement = glm::mat4(1.0f);
    iINPInputsState = 0;
    strINPKeyBindsPathFile = "Keybinds/keybinds.txt";
    strINPKeyBindsTempPathFile = "Keybinds/keybindstemp.txt";
    strINPKeyFunctionNameToModify = "";
    iINPCountTrueInputFile = 0;
    iINPCameraState = 0;
    fINPSensitivity = 0.05f;
    bINPFirstMouse = true;
    camINPChosenCamera = CCamera();
    /*mapINPInputsList["RIGHT_DASH"] = GLFW_KEY_D;
    mapINPInputsList["LEFT_DASH"] = GLFW_KEY_A;
    mapINPInputsList["FORWARD_DASH"] = GLFW_KEY_W;
    mapINPInputsList["BACKWARD_DASH"] = GLFW_KEY_S;*/
}

CInputs::CInputs(CCamera& camera) {
    mat4INPMovement = glm::mat4(1.0f);
    iINPInputsState = 0;
    strINPKeyBindsPathFile = "Keybinds/keybinds.txt";
    strINPKeyBindsTempPathFile = "Keybinds/keybindstemp.txt";
    strINPKeyFunctionNameToModify = "";
    iINPCountTrueInputFile = 0;
    iINPCameraState = 0;
    fINPSensitivity = 0.1f;
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

void CInputs::INPSetKeyBindsTempPathFile(std::string path) {
    this->strINPKeyBindsTempPathFile = path;
}

void CInputs::INPSetKeyFunctionNameToModify(std::string FunctionName) {
    this->strINPKeyFunctionNameToModify = FunctionName;
}

void CInputs::INPSetStateInputs(int state) {
    this->iINPInputsState = state;
}

void CInputs::INPSetCountTrueInputFile(int count) {
    this->iINPCountTrueInputFile = count;
}

glm::mat4 CInputs::mat4INPGetMovement() {
    return this->mat4INPMovement;
}

std::string CInputs::strINPGetKeyBindsPathFile() {
    return this->strINPKeyBindsPathFile;
}

std::string CInputs::strINPGetKeyBindsTempPathFile() {
    return this->strINPKeyBindsTempPathFile;
}

std::string CInputs::strINPGetKeyFunctionNameToModify() {
    return this->strINPKeyFunctionNameToModify;
}

int CInputs::iINPGetInputsState() {
    return this->iINPInputsState;
}

int CInputs::iINPGetCountTrueInputFile() {
    return this->iINPCountTrueInputFile;
}

void CInputs::INPReplaceWithGoodFile() {
    if (iINPCountTrueInputFile % 2 == 1) {
        std::cout << "Il faut remplacer le fichier" << std::endl;
        const char* pcConstKeyBinds = strINPKeyBindsPathFile.c_str();
        const char* pcConstKeyBindsTemp = strINPKeyBindsTempPathFile.c_str();
        if (std::rename(pcConstKeyBinds, "Keybinds/temp.txt") == 0) {
            std::cout << "fonctionne etape 1" << std::endl;
        }
        if (std::rename(pcConstKeyBindsTemp, pcConstKeyBinds) == 0) {
            std::cout << "fonctionne etape 2" << std::endl;
        }
        if (std::rename("Keybinds/temp.txt", pcConstKeyBindsTemp) == 0) {
            std::cout << "fonctionne etape 3" << std::endl;
        }
    }
    /*if (iINPCountTrueInputFile % 2 == 0) {
        std::cout << "Il ne faut pas remplacer le fichier" << std::endl;
        if (std::rename("Keybinds/brouillon.txt", "Keybinds/testfile.txt") == 0) {
            std::cout << "La fonctionnalite est operationnelle" << std::endl;
            //On fait une boucle avec un nom de fichier temporaire entre keybinds.txt et keybindstemp.txt
        }
        else {
            std::cout << "Erreur remplacement fichier" << std::endl;
        }
    }*/
}

//Joue le même rôle que la fonction qui crée un inputs via le parser.
void CInputs::INPLoadNewMapKeyBinds(std::string FunctionName, int Key) {
    //Regarder si FunctionName est bien dans la liste des noms possibles et regarder si key est bien possible également
    mapINPInputsList[FunctionName] = Key;
    
    
    /*std::fstream fINPKeyBindsFile;
    fINPKeyBindsFile.open(strINPKeyBindsPathFile);
    if (!fINPKeyBindsFile.is_open()) {
        CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
        throw(exception);
    }
    else if (fINPKeyBindsFile.is_open()) {

    }
    fINPKeyBindsFile.close();*/
}

//On écrit dans le fichier texte
//On regarde que l'input n'est pas déjà renseigné
//Si c'est le cas alors on le remplace
//On fait une option pour debind une touche
//On fait en sorte qu'une fois debind une touche n'ait plus de valeur dans le fichier texte (e.g : RIGHT_DASH:;)

//Fonction qui va écrire dans le fichier, ensuite on charge le fichier qui donne modifie cinputs
void CInputs::INPNewBinding(std::string FunctionName, int Key) {
    //mapINPInputsList[FunctionName] = Key;
    std::fstream fINPKeyBindsFile;
    fINPKeyBindsFile.open(strINPKeyBindsPathFile);
    if (!fINPKeyBindsFile.is_open()) {
        CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
        throw(exception);
    }
    else if (fINPKeyBindsFile.is_open()) {
        if (bFileIsEmpty(strINPGetKeyBindsPathFile()) == 1) {
            fINPKeyBindsFile << FunctionName << ":" << Key << ";" << "\n";
        }
        else if (bFileIsEmpty(strINPGetKeyBindsPathFile()) == 0) {
            if (bTextAlreadyInFile(FunctionName, strINPKeyBindsPathFile)) {
                std::cout << "deja bind" << std::endl;
                std::fstream fINPKeyBindsTempFile;
                fINPKeyBindsTempFile.open(strINPKeyBindsTempPathFile);
                if (!fINPKeyBindsFile.is_open()) {
                    CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
                    throw(exception);
                }
                else if (fINPKeyBindsTempFile.is_open()) {
                    std::string strtemp;
                    while (fINPKeyBindsFile >> strtemp) {
                        if (strtemp.find(FunctionName) != std::string::npos) { //Dès qu'on est sur la ligne du fichier correspondant à l'input on va remplacer par new value
                            strtemp = FunctionName;
                            strtemp.append(":");
                            strtemp.append(ConvertIntToString(Key));
                            strtemp.append(";");
                        }
                        //Problème il faut sûrement faire un switch entre les 2 fichiers car seul le dernier input changé sera pris en compte
                        //Egalement revoir le processus d'initialisation pour la 1ère fois des inputs
                        strtemp += "\n";
                        fINPKeyBindsTempFile << strtemp; //Ici on a bien le fichier avec le nouvel input.
                        //On va maintenant réinverser les 2 fichiers
                    }
                }
                //On inverse les 2 fichiers (on a donc un fichier actualisé avec tous les dernières valeurs de chaque input renseignées)
                std::string PathTempFile = strINPGetKeyBindsTempPathFile();
                std::string PathTrueFile = strINPGetKeyBindsPathFile();
                INPSetKeyBindsPathFile(PathTempFile);
                INPSetKeyBindsTempPathFile(PathTrueFile);
                iINPCountTrueInputFile++; //Si l'action a été répétée 0[2] alors on dans le bon fichier (keybinds.txt)
                //Sinon (1[2]) le fichier avec les derniers inputs est keybindstemp.txt et il faut alors refaire un dernier changement
                //Comme ça, après réouverture de l'application et lecture dans le fichier keybinds.txt, tout sera bon
                fINPKeyBindsTempFile.close();
            }
            fINPKeyBindsFile.seekg(-1, std::ios_base::end); //On se rend à la fin actuelle du fichier
            fINPKeyBindsFile << FunctionName << ":" << Key << ";" << "\n";
        }
    }
    fINPKeyBindsFile.close();
}

void CInputs::INPInputsFromFile() {
    CParser parser = CParser(this->strINPGetKeyBindsPathFile(), ":;");
    parser.PARLireFichier();
    /*if (parser.strPARGetBuffer().empty()) { //Peut etre garder le pcpargetbuffer a la place
        CException exception(AFFICHAGE_BUFFER_VIDE);
        throw(exception);
    }
    else { //Ici on est censé avoir tout le fichier texte dans la string
        char* ptr = NULL;
        char* mots = strtok_s(parser.pcPARGetBuffer(), parser.strPARGetSeparators(), &ptr);
        char* dernier_mot = NULL;
        char** liste_mot = new char* [strlen(parser.pcPARGetBuffer())];
        int cpt_mot = 0;
        while (mots != NULL) {
            liste_mot[cpt_mot] = _strdup(mots);
            cpt_mot++;
            mots = strtok_s(NULL, parser.strPARGetSeparators(), &ptr); //Mot suivant
        }
        int check = 0;
        std::string FunctionName;
        int Key;
        for (int boucle = 0; boucle < cpt_mot; boucle++) {
            if (check == 0 && boucle % 2 == 0) { //Name of the function
                FunctionName = liste_mot[boucle];
                check = 1;
            }
            if (check == 1 && boucle % 2 == 1) { //Associated Key
                Key = ConvertStringToInt(liste_mot[boucle]);
                check = 2;
            }
            if (check == 2) {
                this->INPLoadNewMapKeyBinds(FunctionName, Key);
                check = 0;
            }
        }
    }*/
}

void CInputs::processInputs(GLFWwindow* window) {
    //On fait ça si input state vaut 1
    //Si input state vaut 2, on bloque ces actions là (évite de déplacer le personnage lorsqu'on change les inputs par exemple)
    //Faire un bouton d'annulation d'attente d'input
    if (glfwGetKey(window, mapINPInputsList["FORWARD_DASH"])) {
        std::cout << "AVANCE" << std::endl;
        //movement = glm::translate(movement, glm::vec3(0.0f, 0.0f, 0.01f));
    }
    if (glfwGetKey(window, mapINPInputsList["BACKWARD_DASH"])) {
        std::cout << "RECULE" << std::endl;
        //movement = glm::translate(movement, glm::vec3(0.0f, 0.0f, -0.01f));
    }
    if (glfwGetKey(window, mapINPInputsList["LEFT_DASH"])) {
        std::cout << "DECALE A GAUCHE" << std::endl;
        //movement = glm::translate(movement, glm::vec3(0.01f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, mapINPInputsList["RIGHT_DASH"])) {
        std::cout << "DECALE A DROITE" << std::endl;
        //movement = glm::translate(movement, glm::vec3(-0.01f, 0.0f, 0.0f));
    }
}

void CInputs::INPKeyCallback(GLFWwindow* window_, int key, int scancode, int action, int mods) {
    if (iINPGetInputsState() == 2) {
        std::cout << "right dash bien dans le fichier ? " << bTextAlreadyInFile(strINPGetKeyFunctionNameToModify(), strINPGetKeyBindsPathFile()) << std::endl;
        INPNewBinding(strINPGetKeyFunctionNameToModify(), key);
        INPSetStateInputs(1);
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS && iINPCameraState == 0) {
        iINPCameraState = 1;
    }
    if (key == GLFW_KEY_V && action == GLFW_RELEASE && iINPCameraState == 1) {
        iINPCameraState = 2;
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS && iINPCameraState == 2) {
        iINPCameraState = 0;
    }
}

void CInputs::INPMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (iINPCameraState == 2) {
        if (bINPFirstMouse) {
            fINPLastMouseX = xpos;
            fINPLastMouseY = ypos;
            bINPFirstMouse = false;
        }
        float xoffset = xpos - fINPLastMouseX;
        float yoffset = fINPLastMouseY - ypos; // reversed since y-coordinates range from bottom to top
        fINPLastMouseX = xpos;
        fINPLastMouseY = ypos;

        xoffset *= fINPSensitivity;
        yoffset *= fINPSensitivity;

        camINPChosenCamera.fCAMYaw += xoffset;
        camINPChosenCamera.fCAMPitch += yoffset;

        if (camINPChosenCamera.fCAMPitch > 89.f) {
            camINPChosenCamera.fCAMPitch = 89.f;
        }
        if (camINPChosenCamera.fCAMPitch < -89.f) {
            camINPChosenCamera.fCAMPitch = -89.f;
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