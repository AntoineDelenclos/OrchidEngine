#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "GlobalTools.h"
#include "CException.h"

class CLogs {
private:
	//std::fstream fLOGLastLogsFile;
	std::string strLOGPathEntireLogsFile; //Il faut trouver un moyen d'�tre s�r que le fichier existe.
	//Soit l'utilisateur d�signe un fichier pr�existant, soit il donne un nom � son fichier de logs, soit automatiquement et accessible si besoin
public:
	CLogs();
	CLogs(std::string PathFileEntireLogs);
	~CLogs();
	void LOGSetPathFileEntireLogs(std::string PathFile);
	std::string strLOGGetPathFileEntireLogs();
	void LOGWritingEntireLogs(std::string Text);
	void LOGResetEntireLogs();
};