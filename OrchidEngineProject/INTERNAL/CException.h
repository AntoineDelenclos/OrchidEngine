#pragma once

#define FAUX 0
#define MODIF_SUR_OBJET_NON_INIT 1
#define INCOHERENCE_LIMITES 2
#define OUVERTURE_FICHIER_IMPOSSIBLE 3
#define NUMERO_TEXTURE_IMPOSSIBLE 4
#define ERREUR_LOAD_IMAGE 5
#define CLOSE_APPLICATION 6
//PARSER
#define LECTURE_ELEMENT_BUFFER_IMPOSSIBLE 7
#define TAILLE_MAXIMALE_DEPASSEE 8
#define TAILLE_BUFFER_NEGATIVE 9
#define DONNES_PERDUES_BUFFER 10
#define PATH_FICHIER_INCORRECT 11
#define AFFICHAGE_BUFFER_VIDE 12
#define CHAINE_CARACTERES_NULLE 13
#define ERREUR_DEPLACEMENT_FICHIER 14

//ENGINE
#define GLFW_INIT_ERREUR 15
#define GLFW_WINDOW_CREATION 16
#define GLEW_NOT_INITIALIZE 17
#define ENGINE_NOT_RUNNING 18
#define ENGINE_TEXTURE_LIMIT 19

class CException {
private:
	unsigned int uiEXCValeur;
public:
	CException(); //Constructeur par défaut
	CException(unsigned int valeur);
	~CException(); //Destructeur par défaut

	unsigned int uiEXCLireValeur();
	void EXCModfierValeur(unsigned int new_valeur);
	void EXCAfficherMessage(unsigned int uiEXCValeur);
};