#include "CLogs.h"
#include <filesystem>

CLogs::CLogs() {
	
}

CLogs::CLogs(std::string PathFileEntireLogs) {
	//FILE* file = 
	strLOGPathEntireLogsFile = PathFileEntireLogs;
	//fLOGLastLogsFile.open(PathFileLastLogs, std::fstream::in | std::fstream::out);
}

CLogs::~CLogs() {
	//appel de la méthode close ?
}

void CLogs::LOGSetPathFileEntireLogs(std::string PathFile) {
	this->strLOGPathEntireLogsFile = PathFile;
}

std::string CLogs::strLOGGetPathFileEntireLogs() {
	return this->strLOGPathEntireLogsFile;
}

void CLogs::LOGWritingEntireLogs(std::string Text) {
	std::fstream fLOGEntireLogsFile;
	fLOGEntireLogsFile.open(strLOGPathEntireLogsFile);
	if (!fLOGEntireLogsFile.is_open()) {
		CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
		throw(exception);
	}
	else if (fLOGEntireLogsFile.is_open()) {
		if (bFileIsEmpty(strLOGGetPathFileEntireLogs()) == 1) {
			fLOGEntireLogsFile << "Entire_Logs_File\n\n";
		}
		fLOGEntireLogsFile.seekg(-1, std::ios_base::end); //On se rend à la fin actuelle du fichier
		fLOGEntireLogsFile << Text;
		fLOGEntireLogsFile.close();
	}
}

void CLogs::LOGResetEntireLogs() {
	std::fstream fLOGEntireLogsFile;
	fLOGEntireLogsFile.open(strLOGPathEntireLogsFile, std::fstream::out | std::fstream::trunc);
	if (!fLOGEntireLogsFile.is_open()) {
		CException exception(OUVERTURE_FICHIER_IMPOSSIBLE);
		throw(exception);
	}
	fLOGEntireLogsFile.close();
}