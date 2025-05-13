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
#include <string_view>
#include <fstream>
#include <vector>
#include <filesystem>

// Eigene Includes
#include "TKMCJob/TKMCJob.h"
#include "GlobalDefinitions.h"
#include "ErrorCodes.h"

using namespace std;

void GetKMCFiles(const filesystem::path& dir, vector<string>& file_list)
{
	for (const auto& file : filesystem::recursive_directory_iterator(dir))
	{
		if ((filesystem::is_regular_file(file)) && (file.path().extension() == ".kmc"))
		{
			file_list.push_back(file.path().string());
		}
	}
}

int main(int argc, char* argv[])
{
	// Default ist das aktuelle Arbeitsverzeichnis
	filesystem::path RootDir = filesystem::current_path();

	// Kommandozeilenargumente verarbeiten (argv[0] = Programmpfad, argv[1] = optionales Argument)
	if (argc > 2)
	{
		cout << "Error: Invalid number of command line arguments. Use -help or -h for further information." << endl;
		return 1;
	}
	if (argc == 2)
	{
		string_view CmdArg = Trim(argv[1]);
		if ((CmdArg == "-help") || (CmdArg == "-h"))
		{
			cout << "iConSearcher" << endl;
			cout << "Retrieval tool for iConSimulator results." << endl;
			cout << "Copyright 2016-2025, P. Hein, Workgroup Martin, IPC, RWTH Aachen" << endl;
			cout << "Distributed under GPL v3 license. Publication of results requires proper attribution." << endl;
			cout << KMCOUT_VERSION << " " << KMC_VERSION << endl;
			cout << "Collects the KMC results from a certain directory and from all sub-directories into a summary file." << endl;
			cout << endl;
			cout << "Available command line arguments:" << endl;
			cout << "-help / -h: Show this help text." << endl;
			cout << "-version: Show plain version number." << endl;
			cout << "<directory path>: Path of the directory whose *.kmc files should be collected." << endl;
			cout << "If no directory is specified, then it defaults to the current working directory." << endl;
			return 0;
		}
		if (CmdArg == "-version")
		{
			cout << KMC_VERSION << endl;
			return 0;
		}
		try
		{
			RootDir = CmdArg;
		}
		catch (const exception& e)
		{
			cout << "Error: Invalid directory path as command line argument (" << CmdArg << ")." << endl;
			cout << e.what() << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}
	}

	cout << "iConSearcher" << endl;
	cout << "Retrieval tool for iConSimulator results." << endl;
	cout << "Copyright 2016-2025, P. Hein, Workgroup Martin, IPC, RWTH Aachen" << endl;
	cout << "Distributed under GPL v3 license. Publication of results requires proper attribution." << endl;
	cout << KMCOUT_VERSION << " " << KMC_VERSION << endl;
	cout << endl;

	// Pruefen ob Ordner existiert
	try
	{
		if (filesystem::exists(RootDir) == false)
		{
			cout << "Error: Specified directory does not exist (" << RootDir << ")." << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}
		if (filesystem::is_directory(RootDir) == false)
		{
			cout << "Error: Specified path is not a directory (" << RootDir << ")." << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}
	}
	catch (const exception& e)
	{
		cout << endl;
		cout << "Critical Error: Exception during directory validation:" << endl;
		cout << e.what() << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "Search folder: " << RootDir << endl;

	// Dateiliste (*.kmc) erstellen
	vector<string> FileList;
	cout << "Retrieving *.kmc files in search folder and all subdirectories ... ";
	try
	{
		GetKMCFiles(RootDir, FileList);
		if (FileList.size() == 0)
		{
			cout << endl;
			cout << "No *.kmc files found." << endl;
			cout << endl;
			cout << "Program completed." << endl;
			return 0;
		}
	}
	catch (const exception& e)
	{
		cout << endl;
		cout << "Critical Error: Exception during file search:" << endl;
		cout << e.what() << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "OK" << endl;
	cout << "Number of *.kmc files: " << FileList.size() << endl;

	int ErrorCode = KMCERR_OK;

	// KMC-Job Objekt erstellen
	TKMCJob KMCJob;
	if (KMCJob.IfReady() == false)
	{
		cout << "Critical Error: Job object creation failed." << endl;
		cout << "Contact the developers!" << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}

	// Zusammenfassungsdatei oeffnen
	ofstream summaryfile;
	try
	{
		filesystem::path summarypath = RootDir / KMCPATH_DEFAULT_SUMMARY;
		summaryfile.open(summarypath);
		if (summaryfile.is_open() == false)
		{
			cout << "Critical Error: Failed to open summary file." << endl;
			cout << "Contact the developers!" << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}
	}
	catch (const exception& e)
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
	for (const string& filepath : FileList)
	{
		// Job-Datei laden
		ErrorCode = KMCJob.LoadFromFile(filepath);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "Invalid job file (" << ErrorCode << "): " << filepath << endl;
			continue;
		}

		// Status pruefen
		int job_status = 0;
		ErrorCode = KMCJob.GetProjectState(job_status);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "Invalid job status (" << ErrorCode << "): " << filepath << endl;
			continue;
		}
		if (job_status < 9)
		{
			cout << "Simulation incomplete (" << job_status << "): " << filepath << endl;
			continue;
		}

		// Anzahl an Dotierungen pruefen
		int t_dop_count = -1;
		ErrorCode = KMCJob.GetDopingCount(t_dop_count);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "Invalid doping (" << ErrorCode << "): " << filepath << endl;
			continue;
		}
		if (doping_count == -1)		// d.h. noch kein Job geschrieben
		{     
			// Kopfzeile ermitteln
			string t_summarydesc = "";
			ErrorCode = KMCJob.GetResultSummaryDesc(" ; ", t_summarydesc);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "Invalid summary description (" << ErrorCode << "): " << filepath << endl;
				continue;
			}

			// Kopfzeile in Datei schreiben
			try
			{
				summaryfile << t_summarydesc << endl;
			}
			catch (const exception& e)
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
		}
		else
		{
			// Dotierungsanzahl pruefen
			if (t_dop_count != doping_count)
			{
				cout << "Inconsistent: " << filepath << endl;
				continue;
			}
		}

		// Zusammenfassung ermitteln
		string t_summary = "";
		ErrorCode = KMCJob.GetResultSummary(" ; ", t_summary);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "Invalid summary (" << ErrorCode << "): " << filepath << endl;
			continue;
		}

		// Zusammenfassung in Datei schreiben
		try
		{
			summaryfile << t_summary << endl;
		}
		catch (const exception& e)
		{
			summaryfile.close();
			cout << "Error: Exception when writing to file:" << endl;
			cout << e.what() << endl;
			cout << endl;
			cout << "Program terminated." << endl;
			return 1;
		}

		cout << "Added job: " << filepath << endl;
	}

	// Datei schließen
	summaryfile.close();
	cout << endl;
	cout << "All files processed." << endl;
	cout << "Program completed." << endl;

	return 0;
}