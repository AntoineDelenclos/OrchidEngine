#include <fstream>
#include <iostream>
#include "CException.h"

CException::CException() {
	uiEXCValeur = FAUX;
}

CException::CException(unsigned int valeur) {
	uiEXCValeur = valeur;
}

CException::~CException() { //On va quand même créer un destructeur par défaut et il ne va "rien" faire mais au moins on n'utilisera pas celui par défaut du compilateur

}

unsigned int CException::uiEXCLireValeur() {
	return uiEXCValeur;
}

void CException::EXCModfierValeur(unsigned int valeur) {
	uiEXCValeur = valeur;
}

void CException::EXCAfficherMessage(unsigned int valeur) {
	switch (uiEXCLireValeur()) {
	case MODIF_SUR_OBJET_NON_INIT:
		std::cout << "Modification de variable sur un objet non initialise/instancie." << std::endl;
		break;
	case INCOHERENCE_LIMITES:
		std::cout << "Les valeurs limites sont incoherentes." << std::endl;
		break;
	case OUVERTURE_FICHIER_IMPOSSIBLE:
		std::cout << "Ouverture du fichier impossible." << std::endl;
		break;
	case NUMERO_TEXTURE_IMPOSSIBLE:
		std::cout << "Le numero de texure n'est pas coherent, est impossible." << std::endl;
		break;
	case ERREUR_LOAD_IMAGE:
		std::cout << "L'image n'a pas reussi a etre chargee." << std::endl;
		break;
	case CLOSE_APPLICATION:
		std::cout << "L'application a ete correctement fermee." << std::endl;
		break;
	case LECTURE_ELEMENT_BUFFER_IMPOSSIBLE:
		std::cout << "Vous ne pouvez acceder a cet element du buffer car l'espace memoire associe n'existe pas, n'est pas accessible, ou autre." << std::endl;
		break;
	case TAILLE_MAXIMALE_DEPASSEE:
		std::cout << "Vous essayez d'acceder ou de modifier le buffer en depassant l'espace memoire qui lui a ete attribue." << std::endl;
		break;
	case TAILLE_BUFFER_NEGATIVE:
		std::cout << "Vous ne pouvez pas avoir un buffer de taille negative." << std::endl;
		break;
	case DONNES_PERDUES_BUFFER:
		std::cout << "Vous avez ecrase des donnees du buffer. Vous devriez recreer un nouveau buffer puis detruire l'ancien." << std::endl;
		break;
	case PATH_FICHIER_INCORRECT:
		std::cout << "Impossible de lire le fichier au chemin d'acces specifie." << std::endl;
		break;
	case AFFICHAGE_BUFFER_VIDE:
		std::cout << "Vous essayez d'afficher un buffer qui ne contient aucun element." << std::endl;
		break;
	case CHAINE_CARACTERES_NULLE:
		std::cout << "Vous ne pouvez pas lire une chaine de caracteres nulle." << std::endl;
		break;
	case ERREUR_DEPLACEMENT_FICHIER:
		std::cout << "Le fichier n'a pas pu etre deplace." << std::endl;
		break;
	case GLFW_INIT_ERREUR:
		std::cout << "glfwInit() n'a pas pu correctement s'effectuer." << std::endl;
		break;
	case GLFW_WINDOW_CREATION:
		std::cout << "Erreur : Creation GLFW fenetre" << std::endl;
		break;
	case GLEW_NOT_INITIALIZE:
		std::cout << "GLEW n'a pas ete initialise" << std::endl;
		break;
	case ENGINE_NOT_RUNNING:
		std::cout << "Engine currently inactive" << std::endl;
		break;
	case ENGINE_TEXTURE_LIMIT:
		std::cout << "Max number of textures for the engine is reached" << std::endl;
		break;
	case MAP_KEYBINDS_EMPTY:
		std::cout << "Keybinds' map is empty" << std::endl;
		break;
	default:
		std::cout << "Autre exception." << std::endl;
		break;
	}
}