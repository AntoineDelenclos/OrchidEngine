#pragma once
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <algorithm>
#include "CException.h"


int LengthString(char* str);
char* FusionStrings(char* str1, char* str2);
char* AjoutString(char* str1, char* str2);
int ConvertStringToInt(char* str);
char* ConvertIntToString(int entier);
char* ConvertToLowerCase(char* texte); //(char*)texte qui est un const char*
char* ConvertToUpperCase(char* texte);
bool bFileIsEmpty(std::string PathFile);
bool bTextAlreadyInFile(std::string Text, std::string PathFile);
glm::vec4 vec4HexToRGBAColor(std::string hex_code);

glm::vec3 vec3TriangleNormalVector(glm::mat3 matrice);
glm::vec3 vec3CrossProduct(glm::vec3 vec_1, glm::vec3 vec_2);
float VectorNorm(glm::vec3 vector);
void displayMat3(glm::mat3 matrice);
void displayMat4(glm::mat4 matrice);
void displayVec3(glm::vec3 vector);

//OLD MAIN FUNCTIONS
unsigned int iEntierParcourtTexture(unsigned int nombre_sous_textures);
float* texture_cube(unsigned int numero_texture, unsigned int entier_parcourt);
float* modif_cube_texture(float* vertex_cube, float* texture);