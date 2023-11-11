#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include "GlobalTools.h"
#include "CException.h"

class CParser {
private:
	std::string strPARPath;
	const char* pcPARSeparators;
	std::string strPARBuffer;
	//Inutiles
	const char* pcPARPath;
	char* pcPARBuffer;
	int iPARTailleActuelleBuffer;
	int iPARTailleMaximaleBuffer;
public:
	CParser();
	//Inutiles
	CParser(const char* PATH);
	CParser(const char* PATH, int taille_limite);
	//
	CParser(std::string PATH, const char* Separators);
	~CParser();

	void PARLireFichier();
	void PARLireBuffer(); //Lire buffer inutile
	void PARSetPath(std::string Path);
	void PARSetSeparators(const char* Separators);
	std::string strPARGetPath();
	const char* strPARGetSeparators();
	std::string strPARGetBuffer();
	
	//Inutiles
	char* pcPARGetBuffer();
	int iPARGetTailleActuelleBuffer();
	int iPARGetTailleMaximaleBuffer();
	const char* pcPARGetPath();
	char cPARGetElementBuffer(int position);
	void PARModifierTailleActuelleBuffer(int new_taille);
	void PARModifierTailleMaximaleBuffer(int new_taille);
	void PARAjouterElementBuffer(char element);

	//CGraphe graPARGrapheFromParser();
	//CInputs inpPARInputsFromParser(std::string Path);
};