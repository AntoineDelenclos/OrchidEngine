#include "GlobalTools.h"

int LengthString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

char* FusionStrings(char* str1, char* str2){ //On pourrait penser à ajouter un mode qui permettrai soit de faire une fusion par recopie ou bien une fusion avec écrasement des anciennes str
    char* str_fusion = NULL;
    int taille1 = LengthString(str1);
    int taille2 = LengthString(str2);
    str_fusion = (char*)malloc(sizeof(char) * (taille1 + taille2 + 1));
    for (int i = 0; i < taille1; i++) {
        str_fusion[i] = str1[i];
    }
    for (int j = 0; j < taille2; j++) {
        str_fusion[taille1 + j] = str2[j];
    }
    str_fusion[taille1 + taille2] = '\0';
    return str_fusion;
}

char* AjoutString(char* str1, char* str2) {
    int taille1 = LengthString(str1);
    int taille2 = LengthString(str2);
    str1 = (char*)realloc(str1, sizeof(char) * (taille1 + taille2 + 1));
    //free(str2);
    for (int i = taille1; i < taille1 + taille2; i++) {
        str1[i] = str2[i - taille1];
    }
    str1[taille1 + taille2] = '\0';
    return str1;
}

int ConvertStringToInt(char* str) {
    int entier = 0;
    int taille = LengthString(str);
    for (int i = 0; i < taille; i++) {
        entier += (int)(pow(10, taille - i) * (str[i] - 48)); //On multiplie par une puissance de 10 en fonctione de la position et on fait -48 pour avoir la correspondance de la table ASCII
    }
    return entier;
}

char* ConvertIntToString(int entier) {
    char* str = NULL;
    int puissance = 0;
    int temp_entier = entier;
    int entier_convert = 0;
    while ((temp_entier / 10) > 1) {
        temp_entier = temp_entier / 10;
        puissance++;
    }//Ici on a donc la plus grande puissance de 10 de l'entier

    int diviseur = (int)(pow(10, puissance)); //1,10,100,1000,...
    str = (char*)malloc(sizeof(char) * (puissance + 2)); //Puissance + 1 pour avoir les unités et + 1 encore pour le '\0'
    for (int i = 0; i < puissance + 1; i++) {
        entier_convert = entier / diviseur; //On a donc l'entier qui multiplie le diviseur actuel. Pour 3318 on aura 3 puis 3 puis 1 et enfin 8
        str[i] = entier_convert + 48; //Conversion dans la table ASCII
        entier -= entier_convert * diviseur; //Enfin on calcule notre nouvel entier en soustrayant le nombre de la puissance la plus grande actuelle.
        diviseur = diviseur / 10; //Enfin on descend d'une puissance de 10
    }
    str[puissance + 1] = '\0';
    return str;
}

void ConvertToMinuscules(char* str) {
    if (str == NULL) {
        CException exception(CHAINE_CARACTERES_NULLE);
        throw(exception);
    }
    int boucle = 0;
    while (str[boucle]) {
        if (str[boucle] >= 'A' && str[boucle] <= 'Z') {
            str[boucle] -= 'A' - 'a';
        }
        boucle++;
    }
}

bool bFileIsEmpty(std::string PathFile) {
    std::fstream fFile;
    fFile.open(PathFile);
    if (!fFile.is_open()) {
        CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
        throw(exception);
    }
    bool empty = (fFile.get(), fFile.eof());
    fFile.close();
    return empty;
}

bool bTextAlreadyInFile(std::string Text, std::string PathFile) {
    std::fstream fFile;
    fFile.open(PathFile);
    if (!fFile.is_open()) {
        CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
        throw(exception);
    }
    bool contained = false;
    std::string line;
    while (std::getline(fFile, line) && contained == false) {
        if (line.find(Text) != std::string::npos) {
            contained = true;
        }
    }
    fFile.close();
    return contained;
}

void displayMat3(glm::mat3 matrice) {
    std::cout << matrice[0][0] << " " << matrice[0][1] << " " << matrice[0][2] << std::endl;
    std::cout << matrice[1][0] << " " << matrice[1][1] << " " << matrice[1][2] << std::endl;
    std::cout << matrice[2][0] << " " << matrice[2][1] << " " << matrice[2][2] << std::endl;
}

void displayMat4(glm::mat4 matrice) {
    std::cout << matrice[0][0] << " " << matrice[0][1] << " " << matrice[0][2] << " " << matrice[0][3] << std::endl;
    std::cout << matrice[1][0] << " " << matrice[1][1] << " " << matrice[1][2] << " " << matrice[1][3] << std::endl;
    std::cout << matrice[2][0] << " " << matrice[2][1] << " " << matrice[2][2] << " " << matrice[2][3] << std::endl;
    std::cout << matrice[3][0] << " " << matrice[3][1] << " " << matrice[3][2] << " " << matrice[3][3] << std::endl;
}

void displayVec3(glm::vec3 vector) {
    std::cout << vector[0] << ", " << vector[1] << ", " << vector[2] << std::endl;
}

//Produit vectoriel de 2 vecteurs a 3 dimensions
glm::vec3 vec3CrossProduct(glm::vec3 vec_1, glm::vec3 vec_2) {
    glm::vec3 result = glm::vec3(1.0);
    for (int boucle = 0; boucle < 3; boucle++) {
        result[boucle] = vec_1[(boucle + 1) % 3] * vec_2[(boucle + 2) % 3] - vec_1[(boucle + 2) % 3] * vec_2[(boucle + 1) % 3];
    }
    return result;
}

//Calcule la norme d'un vecteur de dimension 3
float VectorNorm(glm::vec3 vector) {
    float norm = 0.0;
    for (int boucle = 0; boucle < 3; boucle++) {
        norm += vector[boucle] * vector[boucle];
    }
    norm = sqrt(norm);
    return norm;
}

//Normalise un vecteur de dimension 3
glm::vec3 NormalizeVector(glm::vec3 vector) {
    glm::vec3 normalized;
    float norm = VectorNorm(vector);
    for (int boucle = 0; boucle < 3; boucle++) {
        normalized[boucle] = vector[boucle] / norm;
    }
    return normalized;
}

//La matrice représente les coordonnées des 3 points du triangle dans un espace en 3D
glm::vec3 vec3TriangleNormalVector(glm::mat3 matrice) {
    glm::vec3 directorVec_1;
    glm::vec3 directorVec_2;
    //Logiquement les vecteurs directeurs du triangle ne devraient jamais être colinéaires entre eux car c'est un triangle CQFD
    for (int i = 0; i < 3; i++) {
        directorVec_1[i] = matrice[1][i] - matrice[0][i];
        directorVec_2[i] = matrice[2][i] - matrice[0][i];
    }
    glm::vec3 preNormalVector = vec3CrossProduct(directorVec_1, directorVec_2); //Fonctionne
    std::cout << "norm : " << VectorNorm(preNormalVector) << std::endl;
    glm::vec3 normalVector = NormalizeVector(preNormalVector);
    //displayVec3(normalVector);
    return normalVector;
}

//////////////////////////// OLD MAIN FUNCTIONS /////////////////////////////////

//Va permettre d'avoir le nombre à ajouter dans les textures coords
unsigned int iEntierParcourtTexture(unsigned int nombre_sous_textures) {
    unsigned int entier_parcourt = 0;
    entier_parcourt = (unsigned int)sqrt(nombre_sous_textures);
    if (entier_parcourt < sqrt(nombre_sous_textures)) {
        entier_parcourt = (unsigned int)sqrt(nombre_sous_textures) + 1;
    }
    return entier_parcourt;
}

float* texture_cube(unsigned int numero_texture, unsigned int entier_parcourt) { //On parcourt les textures de gauche à droite puis de haut en bas
    if ((numero_texture < 0) || (numero_texture > (entier_parcourt * entier_parcourt))) {
        CException exception(NUMERO_TEXTURE_IMPOSSIBLE);
        throw(exception);
    }

    float* vertex = NULL;
    vertex = (float*)malloc(sizeof(float) * 12);

    unsigned int ligne = 0;
    int colonne = 0;
    ligne = numero_texture / entier_parcourt;

    colonne = numero_texture % entier_parcourt;
    if (colonne == 0) {
        colonne = entier_parcourt;
    }
    colonne--;
    float parc = 1.f / entier_parcourt;
    vertex[0] = colonne * parc; // BAS GAUCHE X
    vertex[1] = ligne * parc; // BAS GAUCHE Y
    vertex[2] = (colonne + 1) * parc; // BAS DROITE X
    vertex[3] = ligne * parc; // BAS DROITE Y
    vertex[4] = (colonne + 1) * parc; // HAUT DROITE X
    vertex[5] = (ligne + 1) * parc; // HAUT DROITE Y
    vertex[6] = vertex[4]; // HAUT DROITE X
    vertex[7] = vertex[5]; // HAUT DROITE Y
    vertex[8] = colonne * parc; // HAUT GAUCHE X
    vertex[9] = (ligne + 1) * parc; // HAUT GAUCHE Y
    vertex[10] = vertex[0]; // BAS GAUCHE X
    vertex[11] = vertex[1]; // BAS GAUCHE Y

    for (int i = 0; i < 12; i++) {
        std::cout << "\n vertex:" << vertex[i];
    }
    return vertex;
}

float* modif_cube_texture(float* vertex_cube, float* texture) {
    int k = 0;
    for (int i = 0; i < 36; i++) {
        for (int j = 3; j < 5; j++) {
            vertex_cube[5 * i + j] = texture[k % 12];
            k++;
        }
    }
    return vertex_cube;
}