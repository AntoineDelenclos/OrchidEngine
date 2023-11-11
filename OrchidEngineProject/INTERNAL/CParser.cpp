#include "CParser.h"


CParser::CParser() {
	pcPARPath = NULL;
	iPARTailleActuelleBuffer = 0;
	iPARTailleMaximaleBuffer = 0;
	pcPARBuffer = NULL;
}

CParser::CParser(const char* PATH) {
	pcPARPath = PATH;
	iPARTailleActuelleBuffer = 0;
	iPARTailleMaximaleBuffer = 10;
	pcPARBuffer = new char[iPARTailleMaximaleBuffer];
}
CParser::CParser(const char* PATH, int taille_limite) {
	pcPARPath = PATH;
	iPARTailleActuelleBuffer = 0;
	iPARTailleMaximaleBuffer = taille_limite;
	pcPARBuffer = new char[iPARTailleMaximaleBuffer];
}

CParser::CParser(std::string PATH, const char* Separators) {
	strPARPath = PATH;
	pcPARSeparators = Separators;
}

CParser::~CParser() {
	//delete pcPARBuffer;
}

int CParser::iPARGetTailleActuelleBuffer() {
	return iPARTailleActuelleBuffer;
}

int CParser::iPARGetTailleMaximaleBuffer() {
	return iPARTailleMaximaleBuffer;
}

const char* CParser::pcPARGetPath() {
	return pcPARPath;
}

char* CParser::pcPARGetBuffer() {
	return pcPARBuffer;
}

char CParser::cPARGetElementBuffer(int position) {
	if (position > this->iPARGetTailleMaximaleBuffer()) {
		CException exception(LECTURE_ELEMENT_BUFFER_IMPOSSIBLE);
		throw(exception);
	}
	return this->pcPARBuffer[position];
}

void CParser::PARModifierTailleActuelleBuffer(int new_taille) { //Va permettre d'écraser les anciennes valeurs si on repart sur une taille < à la taille actuelle
	if (new_taille > this->iPARGetTailleMaximaleBuffer()) {
		CException exception(TAILLE_MAXIMALE_DEPASSEE);
		throw(exception);
	}
	if (new_taille < 0) {
		CException exception(TAILLE_BUFFER_NEGATIVE);
		throw(exception);
	}
	iPARTailleActuelleBuffer = new_taille;
}

void CParser::PARModifierTailleMaximaleBuffer(int new_taille) {
	if (new_taille < this->iPARGetTailleActuelleBuffer()) {
		CException exception(DONNES_PERDUES_BUFFER);
		throw(exception);
	}
	char* new_buffer = new char[new_taille];
	for (int boucle = 0; boucle < this->iPARGetTailleActuelleBuffer(); boucle++) {
		new_buffer[boucle] = this->cPARGetElementBuffer(boucle);
	}
	pcPARBuffer = new_buffer;
	iPARTailleMaximaleBuffer = new_taille;
}

void CParser::PARAjouterElementBuffer(char element) {
	if (this->iPARGetTailleActuelleBuffer() == this->iPARGetTailleMaximaleBuffer()) {
		this->PARModifierTailleMaximaleBuffer(this->iPARGetTailleMaximaleBuffer() * 2);
	}
	pcPARBuffer[this->iPARGetTailleActuelleBuffer()] = element;
	int taille = this->iPARGetTailleActuelleBuffer();
	taille += 1;
	this->PARModifierTailleActuelleBuffer(taille);
}

void CParser::PARLireFichier() {
	/*const char* PATH = this->pcPARGetPath();
	FILE* graphe_fichier;
	int caracter;
	fopen_s(&graphe_fichier, PATH, "r"); //On utilise cette fonction plutôt que fopen car cette dernière est désaprouvée par le compilateur
	if (graphe_fichier == NULL) {
		CException exception(PATH_FICHIER_INCORRECT);
		throw(exception);
	}
	while ((caracter = fgetc(graphe_fichier)) != EOF) {
		this->PARAjouterElementBuffer(caracter);
	}
	this->PARAjouterElementBuffer('\0');
	fclose(graphe_fichier);*/

	std::fstream fPARFile;
	fPARFile.open(strPARPath,std::ios::in); //On ouvre le fichier en mode lecture seule
	if (!fPARFile.is_open()) {
		CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
		throw(exception);
	}
	else if (fPARFile.is_open()) { 

		/*std::ifstream in(strPARPath);
		char c;

		if (in.is_open()) {
			while (in.good()) {
				in.get(c);
				// Play with the data
				std::cout << c;
			}
		}

		if (!in.eof() && in.fail())
			std::cout << "error reading " << strPARPath << std::endl;

		in.close();*/
		//Essayer lecture caractere par caractere
		
		/*std::string test;
		while (getline(fPARFile, test)) {
			std::cout << test << "\n";
		}*/

		std::stringstream buffer;
		buffer << fPARFile.rdbuf();
		std::string file_contents = buffer.str();
		std::cout << file_contents;
		//strPARBuffer.assign((std::istreambuf_iterator<char>(fPARFile)), (std::istreambuf_iterator<char>()));//Réécrit toute la string.
		//std::cout << strPARBuffer;
		fPARFile.close();
		//pcPARBuffer = new char[strPARBuffer.size() + 1]; //https://stackoverflow.com/questions/17910320/c-copy-string-into-char-string-array
		//std::cout << "test lire fichier" << std::endl;
		//std::cout << "test reussi" << std::endl;
		//strcpy(pcPARBuffer, strPARBuffer.c_str()); //Erreur lors de la recopie du buffer
		//strcpy_s(pcPARBuffer, sizeof (pcPARBuffer), strPARBuffer.c_str()); //Erreur lors de la recopie du buffer
		//Copy strPARBuffer -> pcPARBuffer (pour strtok)
	}
}

void CParser::PARLireBuffer() {
	/*if (this->iPARGetTailleActuelleBuffer() == 0) {
		CException exception(AFFICHAGE_BUFFER_VIDE);
		throw(exception);
	}
	for (int boucle = 0; boucle < this->iPARGetTailleActuelleBuffer(); boucle++) {
		std::cout << this->cPARGetElementBuffer(boucle);
	}*/
	if (this->strPARBuffer.empty()) {
		CException exception(AFFICHAGE_BUFFER_VIDE);
		throw(exception);
	}
	else {
		std::cout << strPARBuffer << std::endl;
	}
}

/*CInputs CParser::inpPARInputsFromParser(std::string Path) {
	CInputs inputs = CInputs();
	this->strPARPath = Path;
	PARLireFichier();
	if (this->strPARBuffer.empty()) {
		CException exception(AFFICHAGE_BUFFER_VIDE);
		throw(exception);
	}
	else { //Ici on est censé avoir tout le fichier texte dans la string
		char* ptr = NULL;
		char* mots = strtok_s(pcPARBuffer, pcPARSeparators, &ptr);
		char* dernier_mot = NULL;
		char** liste_mot = new char* [strlen(pcPARBuffer)];
		int cpt_mot = 0;
		while (mots != NULL) {
			liste_mot[cpt_mot] = _strdup(mots);
			cpt_mot++;
			mots = strtok_s(NULL, pcPARSeparators, &ptr); //Mot suivant
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
				inputs.INPLoadNewMapKeyBinds(FunctionName, Key);
				check = 0;
			}
		}
	}
	return inputs;
}*/

/*CGraphe CParser::graPARGrapheFromParser() {
	this->pcPARConvertMinuscules(this->pcPARGetBuffer());
	const char* separateurs = " ,.;:=\n";
	char* ptr = NULL;
	char* mots = strtok_s(this->pcPARGetBuffer(), separateurs, &ptr);
	char* dernier_mot = NULL;
	int iPARNombreSommets = 0;
	int iPARNombreArcs = 0;
	int cpt_mot = 0;
	//On stocke tous les mots dans une liste de char* 
	char** liste_mot = new char* [strlen(this->pcPARGetBuffer())];
	while (mots != NULL) {

		liste_mot[cpt_mot] = _strdup(mots);
		cpt_mot++;

		mots = strtok_s(NULL, separateurs, &ptr); //On prend le mot suivant
	}

	if (strcmp(liste_mot[0], "nbsommets") != 0 || strcmp(liste_mot[2], "nbarcs") != 0) {
		CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
		throw(exception);
	}
	for (int b = 0; b < strlen(liste_mot[1]); b++) {
		if (liste_mot[1][b] < '0' || liste_mot[1][b]>'9') {
			CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
			throw(exception);
		}
	}
	for (int b = 0; b < strlen(liste_mot[3]); b++) {
		if (liste_mot[3][b] < '0' || liste_mot[3][b]>'9') {
			CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
			throw(exception);
		}
	}
	iPARNombreSommets = atoi(liste_mot[1]);
	iPARNombreArcs = atoi(liste_mot[3]);


	//On va utiliser strstr sur nbsommets= ce qui va nous renvoyer un pointeur sur la 1ere occurence dans le fichier.
	//Ensuite on va dire que tant qu'on n'a pas un saut de ligne on lit les valeurs de caractère
	//S'il y a un espace on passe à la valeur suivante, sinon s'il y a un autre caractère qui n'est pas un nombre on renvoie une exception
	//Si après la 1ère valeur il y a une valeur différente de '\n' on renvoie une exception


	CGraphe graPARGraphe = CGraphe(iPARNombreSommets, iPARNombreArcs);

	if (strcmp(liste_mot[4], "sommets") != 0 || strcmp(liste_mot[5], "[") != 0) {
		CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
		throw(exception);
	}
	for (int boucle_sommet = 0; boucle_sommet < 2 * iPARNombreSommets; boucle_sommet++) {
		if (boucle_sommet % 2 == 0) {
			if (strcmp(liste_mot[6 + boucle_sommet], "numero") != 0) {
				CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
				throw(exception);
			}
		}
		if (boucle_sommet % 2 == 1) {
			for (int b_int = 0; b_int < strlen(liste_mot[6 + boucle_sommet]); b_int++) {
				if (liste_mot[6 + boucle_sommet][b_int] < '0' || liste_mot[6 + boucle_sommet][b_int] > '9') {
					CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
					throw(exception);
				}
			}
			graPARGraphe.GRAAjouterSommet(atoi(liste_mot[6 + boucle_sommet]));
		}
	}
	if (strcmp(liste_mot[6 + 2 * iPARNombreSommets], "]") != 0) {
		CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
		throw(exception);
	}


	if (strcmp(liste_mot[7 + 2 * iPARNombreSommets], "arcs") != 0 || strcmp(liste_mot[8 + 2 * iPARNombreSommets], "[") != 0) {
		CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
		throw(exception);
	}

	int arc_deb = 0;
	int arc_fin = 0;

	for (int boucle_arc = 0; boucle_arc < 4 * iPARNombreArcs; boucle_arc++) {
		if (boucle_arc % 4 == 0) {
			if (strcmp(liste_mot[9 + 2 * iPARNombreSommets + boucle_arc], "debut") != 0) {
				CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
				throw(exception);
			}
		}
		if (boucle_arc % 4 == 1) {
			for (int b_int = 0; b_int < strlen(liste_mot[9 + 2 * iPARNombreSommets + boucle_arc]); b_int++) {
				if (liste_mot[9 + 2 * iPARNombreSommets + boucle_arc][b_int] < '0' || liste_mot[9 + 2 * iPARNombreSommets + boucle_arc][b_int] > '9') {
					CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
					throw(exception);
				}
			}
			arc_deb = atoi(liste_mot[9 + 2 * iPARNombreSommets + boucle_arc]);
		}
		if (boucle_arc % 4 == 2) {
			if (strcmp(liste_mot[9 + 2 * iPARNombreSommets + boucle_arc], "fin") != 0) {
				CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
				throw(exception);
			}
		}
		if (boucle_arc % 4 == 3) {
			for (int b_int = 0; b_int < strlen(liste_mot[9 + 2 * iPARNombreSommets + boucle_arc]); b_int++) {
				if (liste_mot[9 + 2 * iPARNombreSommets + boucle_arc][b_int] < '0' || liste_mot[9 + 2 * iPARNombreSommets + boucle_arc][b_int] > '9') {
					CException exception(FORMAT_FICHIER_TEXTE_INCORRECT);
					throw(exception);
				}
			}
			arc_fin = atoi(liste_mot[9 + 2 * iPARNombreSommets + boucle_arc]);
			graPARGraphe.GRAAjouterArc(arc_deb, arc_fin); //On ajoute ici car on sait arc_deb et arc_fin ont été mis à jour
		}
	}
	return graPARGraphe;
}*/

void CParser::PARSetPath(std::string Path) {
	this->strPARPath = Path;
}

void CParser::PARSetSeparators(const char* Separators) {
	this->pcPARSeparators = Separators;
}

std::string CParser::strPARGetPath() {
	return this->strPARPath;
}

const char* CParser::strPARGetSeparators() {
	return this->pcPARSeparators;
}

std::string CParser::strPARGetBuffer() {
	return this->strPARBuffer;
}