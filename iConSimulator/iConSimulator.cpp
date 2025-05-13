// **************************************************************** //
//																	//
//	Hauptquelldatei von: iConSimulator								//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Durchfuehrung einer MC-Simulation								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Standard-Includes
#include <iostream>
#include <string>
#include <string_view>

// Eigene Includes
#include "TKMCJob/TKMCJob.h"
#include "TCustomTime.h"
#include "GlobalDefinitions.h"
#include "ErrorCodes.h"

using namespace std;

int main (int argc, char *argv[]) {

	// Kommandozeilenargumente verarbeiten (argv[0] = Programmpfad, argv[1] = Argument)
	if (argc != 2) {
		cout << "Error: Invalid number of command line arguments. Use -help or -h for further information." << endl;
		return 1;
	}
	string_view CmdArg = Trim(argv[1]);
	if ((CmdArg == "-help") || (CmdArg == "-h")) {
		cout << "iCon - Kinetic Monte-Carlo Simulation of Ionic Conductivity" << endl;
		cout << "Copyright 2016-2025, P. Hein, Workgroup Martin, IPC, RWTH Aachen" << endl;
		cout << "Distributed under GPL v3 license. Publication of results requires proper attribution." << endl;
		cout << KMCOUT_VERSION << " " << KMC_VERSION << endl;
		cout << endl;
		cout << "Available command line arguments:" << endl;
		cout << "-help / -h: Show this help text." << endl;
		cout << "-version: Show plain version number." << endl;
		cout << "<file path>: Path of the job file (*.kmc) that should be processed." << endl;
		return 0;
	}
	if (CmdArg == "-version") {
		cout << KMC_VERSION << endl;
		return 0;
	}
	string JobPath = std::string(CmdArg);
	int ErrorCode = KMCERR_OK;

	// Log-File-Header ausgeben
	cout << "SIMULATION LOG" << endl;
	cout << "iCon - Kinetic Monte-Carlo Simulation of Ionic Conductivity" << endl;
	cout << "Copyright 2016-2025, P. Hein, Workgroup Martin, IPC, RWTH Aachen" << endl;
	cout << "Distributed under GPL v3 license. Publication of results requires proper attribution." << endl;
	cout << KMCOUT_VERSION << " " << KMC_VERSION << endl;
	cout << "Start-Time: ";
	TCustomTime StartTime = TCustomTime::GetCurrentTime(true);
	cout << endl << endl;

	// KMC-Job Objekt erstellen
	TKMCJob KMCJob;
	if (KMCJob.IfReady() == false) {
		cout << "Critical Error: Job object creation failed." << endl;
		cout << "Contact the developers!" << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}

	// Datei laden
	cout << "Loading job file ..." << endl;
	ErrorCode = KMCJob.LoadFromFile(JobPath);
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "Done." << endl << endl;

	// Dateistatus pruefen
	cout << "Testing job status ..." << endl;
	int JobStatus = 0;
	ErrorCode = KMCJob.GetProjectState(JobStatus);
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}
	if (JobStatus < 6) {
		cout << "Error: The job configuration in the input file is incomplete." << endl;
		cout << "Cannot start simulation due to missing settings." << endl;
		cout << endl;
		cout << "Program terminated." << endl;
		return 1;
	}
	if (JobStatus >= 9)
	{
		cout << "The simulation is already complete." << endl;
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "Done." << endl;

	// Simulation initialisieren
	ErrorCode = KMCJob.ClearSimulation();
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}
	ErrorCode = KMCJob.InitializeSimulation();
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << endl;

	// Checkpoint-Dateipfad erstellen
	string ChkPath = JobPath;
	string::size_type ext_pos = ChkPath.rfind('.', ChkPath.length());
	if (ext_pos != string::npos) {
		ChkPath.erase(ext_pos);
	}
	ChkPath += KMCPATH_CHK_EXT;

	// Checkpoint-Pfad setzen und Checkpoint laden
	ErrorCode = KMCJob.SetCheckPointPath(ChkPath);
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}
	ErrorCode = KMCJob.LoadCheckPoint();
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}

	// Simulation durchfuehren
	ErrorCode = KMCJob.MakeSimulation();
	if (ErrorCode != KMCERR_OK) {
		if (ErrorCode == KMCERR_HAS_TO_SET_DYNNORM_ENTRY) {

		} else {
			cout << "Program terminated." << endl;
			return 1;
		}
	}
	cout << endl;

	// Datei speichern
	cout << "Saving job file ..." << endl;
	ErrorCode = KMCJob.SaveToFile(JobPath);
	if (ErrorCode != KMCERR_OK) {
		cout << "Program terminated." << endl;
		return 1;
	}
	cout << "Done." << endl << endl;

	cout << "End-Time: ";
	TCustomTime EndTime = TCustomTime::GetCurrentTime(true);
	cout << endl;
	TCustomTime TotalRuntime = EndTime - StartTime;
	cout << "Total runtime: " << TotalRuntime.ToString() << endl;
	cout << "Job completed successfully." << endl;	

	return 0;
}
