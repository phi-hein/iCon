// **************************************************************** //
//																	//
//	Hauptquelldatei von: iConSearcher								//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Laden und Zusammenfassen fertiger Simulationen				//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Standard-Includes
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// Windows-Includes
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))

#include <Windows.h>

#endif

// Linux-Includes
#if (defined(__linux__) || defined(__linux))

#include <ftw.h>

#endif

// Eigene Includes
#include "TKMCJob/TKMCJob.h"
#include "GlobalDefinitions.h"
#include "ErrorCodes.h"

using namespace std;

// ************************************************************************* //
//					Global variables										 //
// ************************************************************************* //

vector<string> os_FileList;		// Liste aller .kmc-Dateien im Ordner in dem diese Anwendung gespeichert ist (und in allen Unterordnern)


// ************************************************************************* //
//					Get KMC files in Windows								 //
// ************************************************************************* //

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))

// Rekursive Dateisuche (WINDOWS)
void RecursiveFileSearch(const string &dir) 
{
	string s_dir = dir + "\\*";

	WIN32_FIND_DATAA findfiledata;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFileA((LPCSTR)s_dir.c_str(), &findfiledata);
	
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
			if (findfiledata.cFileName[0] != '.') {
				if ((findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
					RecursiveFileSearch(dir + "\\" + string(findfiledata.cFileName));
				} else {
					string s_file(findfiledata.cFileName);
					if (s_file.length() > 4) {
						if (s_file.substr(s_file.length() - 4) == ".kmc") {
							os_FileList.push_back(dir + "\\" + s_file);
						}
					}
				}
			}
		} while (FindNextFileA(hFind, &findfiledata) != 0);
		FindClose(hFind);
    }
}

// KMC-Dateien finden (WINDOWS)
void GetKMCFiles(const string& path) 
{
	os_FileList.clear();
	RecursiveFileSearch(path);
}

#endif

// ************************************************************************* //
//					Get KMC files in Linux									 //
// ************************************************************************* //

#if (defined(__linux__) || defined(__linux))

// ftw-callback (LINUX)
int ftwfunc (const char *fpath, const struct stat *sb, int typeflag) 
{
	if (typeflag == FTW_F) {
		string s_fpath(fpath);
		if (s_fpath.length() > 4) {
			if (s_fpath.substr(s_fpath.length() - 4) == ".kmc") {
				os_FileList.push_back(s_fpath);
			}
		}
	}
	return 0;
}

// KMC-Dateien finden (LINUX)
void GetKMCFiles(const string& path) 
{
    os_FileList.clear();
	ftw(path.c_str(),ftwfunc,10);
}

#endif

int main (int argc, char *argv[]) 
{
	cout << "iConSearcher" << endl;
	cout << "Retrieval tool for iConSimulator results." << endl;
	cout << KMCOUT_VERSION << " " << KMC_VERSION << endl;
	cout << endl;

	// Exe-Pfad ermitteln
	cout << "Find application path ... ";
	string app_path;
	try {
		if (GetApplicationPath(app_path) == false) {
			cout << endl;
			cout << "Critical Error: Failed to retrieve exe path." << endl;
			cout << "Contact the developers!" << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}
	} 
	catch (exception e)
	{
		cout << endl;
		cout << "Critical Error: Exception during exe path retrieval:" << endl;
		cout << e.what() << endl;
		cout << "Contact the developers!" << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "OK" << endl;
	cout << "Application path: " << app_path << endl;

	// Dateiliste (*.kmc) erstellen
	cout << "Retrieving *.kmc files in application folder and all subdirectories ... ";
	try {
		GetKMCFiles(app_path);
		if (os_FileList.size() == 0) {
			cout << endl;
			cout << "No *.kmc files found." << endl;
			cout << endl;
			cout << "Program completed." << endl;
			return 0;
		}
	} 
	catch (exception e)
	{
		cout << endl;
		cout << "Critical Error: Exception during file search:" << endl;
		cout << e.what() << endl;
		cout << "Contact the developers!" << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "OK" << endl;
	cout << "Number of *.kmc files: " << os_FileList.size() << endl;

	int ErrorCode = KMCERR_OK;

	// KMC-Job Objekt erstellen
	TKMCJob KMCJob;
	if (KMCJob.IfReady() == false) {
		cout << "Critical Error: Job object creation failed." << endl;
		cout << "Contact the developers!" << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}

	// Zusammenfassungsdatei oeffnen
	ofstream summaryfile;
	try {
		summaryfile.open(KMCPATH_DEFAULT_SUMMARY);
		if (summaryfile.is_open() == false) {
			cout << "Critical Error: Failed to open summary file." << endl;
			cout << "Contact the developers!" << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}
	} 
	catch (exception e)
	{
		summaryfile.close();
		cout << "Critical Error: Exception when opening summary file:" << endl;
		cout << e.what() << endl;
		cout << "Contact the developers!" << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}

	// Dateien laden und Ergebnisse speichern
	int doping_count = -1;
	cout << "Loading files ... " << endl;
	for (size_t i = 0; i < os_FileList.size(); i++) {

		// Job-Datei laden
		ErrorCode = KMCJob.LoadFromFile(os_FileList[i]);
		if (ErrorCode != KMCERR_OK) {
			cout << "Invalid job file (" << ErrorCode << "): " << os_FileList[i] << endl;
			continue;
		}

		// Status pruefen
		int job_status = 0;
		ErrorCode = KMCJob.GetProjectState(job_status);
		if (ErrorCode != KMCERR_OK) {
			cout << "Invalid job status (" << ErrorCode << "): " << os_FileList[i] << endl;
			continue;
		}
		if (job_status < 9) {
			cout << "Simulation incomplete (" << job_status << "): " << os_FileList[i] << endl;
			continue;
		}

		// Anzahl an Dotierungen pruefen
		int t_dop_count = -1;
		ErrorCode = KMCJob.GetDopingCount(t_dop_count);
		if (ErrorCode != KMCERR_OK) {
			cout << "Invalid doping (" << ErrorCode << "): " << os_FileList[i] << endl;
			continue;
		}
		if (doping_count == -1) {     // d.h. noch kein Job geschrieben

			// Kopfzeile ermitteln
			string t_summarydesc = "";
			ErrorCode = KMCJob.GetResultSummaryDesc(" ; ", t_summarydesc);
			if (ErrorCode != KMCERR_OK) {
				cout << "Invalid summary description (" << ErrorCode << "): " << os_FileList[i] << endl;
				continue;
			}

			// Kopfzeile in Datei schreiben
			try {
				summaryfile << t_summarydesc << endl;
			}
			catch (exception e)
			{
				summaryfile.close();
				cout << "Error: Exception when writing to file:" << endl;
				cout << e.what() << endl;
				cout << endl;
				cout << "Program terminated." << endl;
				return 1;
			}

			// Dotierungsanzahl setzen
			doping_count = t_dop_count;

		} else {

			// Dotierungsanzahl pruefen
			if (t_dop_count != doping_count) {
				cout << "Inconsistent: " << os_FileList[i] << endl;
				continue;
			}
		}

		// Zusammenfassung ermitteln
		string t_summary = "";
		ErrorCode = KMCJob.GetResultSummary(" ; ", t_summary);
		if (ErrorCode != KMCERR_OK) {
			cout << "Invalid summary (" << ErrorCode << "): " << os_FileList[i] << endl;
			continue;
		}

		// Zusammenfassung in Datei schreiben
		try {
			summaryfile << t_summary << endl;
		}
		catch (exception e)
		{
			summaryfile.close();
			cout << "Error: Exception when writing to file:" << endl;
			cout << e.what() << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}

		cout << "Added job: " << os_FileList[i] << endl;
	}

	// Datei schließen
	summaryfile.close();
	cout << endl;
	cout << "All files processed." << endl;
	cout << "Program completed." << endl;
	
	return 0;
}