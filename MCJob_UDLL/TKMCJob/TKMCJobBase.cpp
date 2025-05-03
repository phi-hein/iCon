// **************************************************************** //
//																	//
//	Klasse: TKMCJobBase	(TKMCJob Layer 0)							//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung eines KMC-Jobs							//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Textdatei, Rueckgabe von Ready				//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TKMCJob/TKMCJobBase.h"

// Includes:
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"
#include "TStructure/TStructure.h"
#include "TJumps/TJumps.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "TSettings/TSettings.h"
#include "TSimulation/TSimulation.h"
#include "TResults/TResults.h"
#include "TElementsDesc.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor -> Initialisierung
TKMCJobBase::TKMCJobBase()
{
	try
	{
		// Initialisierung der Member-Variablen
		ProjectName = KMCVAR_PROJECTNAME_INIT;
		UserName = KMCVAR_USERNAME_INIT;
		SetProjectDate();
		ProjectDescription = KMCVAR_PROJECTDESC_INIT;
		m_ElementsDesc = NULL;
		m_Elements = NULL;
		m_Structure = NULL;
		m_Jumps = NULL;
		m_UniqueJumps = NULL;
		m_Settings = NULL;
		m_Simulation = NULL;
		m_Results = NULL;
		Ready = false;
	}
	catch (exception& e)
	{
		Ready = false; 
		StdExceptionHandler(e);
		throw;
	}
	catch (...)
	{
		Ready = false; 
		ExceptionHandler();
		throw;
	}
}

// Destructor -> Destructor der Member-Variablen aufrufen
TKMCJobBase::~TKMCJobBase()
{
	try
	{
		delete m_ElementsDesc;
		delete m_Elements;
		delete m_Structure;
		delete m_Jumps;
		delete m_UniqueJumps;
		delete m_Settings;
		delete m_Simulation;
		delete m_Results;
	}
	catch (exception& e)
	{
		Ready = false; StdExceptionHandler(e);
	}
	catch (...)
	{
		Ready = false; ExceptionHandler();
	}
}

// **************************** PUBLISHED ********************************* //

// Projektnamen setzen
int TKMCJobBase::SetProjectName(string Name)
{
	try
	{
		// Input prüfen
		if (StringCheck(Name, KMCVAL_TKMCJOB_PROJECTNAME_MINLENGTH, KMCVAL_TKMCJOB_PROJECTNAME_MAXLENGTH, KMCVAL_TKMCJOB_PROJECTNAME_ALLOWED_CHARS) == false)
		{
			cout << "Error: Invalid project name: " << Name << endl;
			cout << "       Project name must have " << KMCVAL_TKMCJOB_PROJECTNAME_MINLENGTH << " to " << KMCVAL_TKMCJOB_PROJECTNAME_MAXLENGTH << " characters." << endl;
			if (KMCVAL_TKMCJOB_PROJECTNAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TKMCJOB_PROJECTNAME_ALLOWED_CHARS << "]" << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Input übertragen
		ProjectName = Trim(Name);

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Usernamen eingeben
int TKMCJobBase::SetUserName(string Name)
{
	try
	{
		// Input prüfen
		if (StringCheck(Name, KMCVAL_TKMCJOB_USERNAME_MINLENGTH, KMCVAL_TKMCJOB_USERNAME_MAXLENGTH, KMCVAL_TKMCJOB_USERNAME_ALLOWED_CHARS) == false)
		{
			cout << "Error: Invalid user name: " << Name << endl;
			cout << "       User name must have " << KMCVAL_TKMCJOB_USERNAME_MINLENGTH << " to " << KMCVAL_TKMCJOB_USERNAME_MAXLENGTH << " characters." << endl;
			if (KMCVAL_TKMCJOB_USERNAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TKMCJOB_USERNAME_ALLOWED_CHARS << "]" << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Input übertragen
		UserName = Trim(Name);

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Projektdatum auf aktuelles Datum setzen
int TKMCJobBase::SetProjectDate()
{
	try
	{

		time_t raw_time = time(NULL);
		if (raw_time == -1)
		{
			ProjectDateYear = 1900;
			ProjectDateMonth = 1;
			ProjectDateDay = 1;
		}
		else
		{
			tm current_time;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
			localtime_s(&current_time, &raw_time);
#else
			localtime_r(&raw_time, &current_time);
#endif
			ProjectDateYear = current_time.tm_year + 1900;
			ProjectDateMonth = current_time.tm_mon + 1;
			ProjectDateDay = current_time.tm_mday;
		}

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Projektbeschreibung eingeben
int TKMCJobBase::SetProjectDescription(string Description)
{
	try
	{
		// Input prüfen
		if (StringCheck(Description, KMCVAL_TKMCJOB_PROJECTDESC_MINLENGTH, KMCVAL_TKMCJOB_PROJECTDESC_MAXLENGTH, KMCVAL_TKMCJOB_PROJECTDESC_ALLOWED_CHARS) == false)
		{
			cout << "Error: Invalid project description" << endl;
			cout << "       Project description must have " << KMCVAL_TKMCJOB_PROJECTDESC_MINLENGTH << " to " << KMCVAL_TKMCJOB_PROJECTDESC_MAXLENGTH << " characters." << endl;
			if (KMCVAL_TKMCJOB_PROJECTDESC_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TKMCJOB_PROJECTDESC_ALLOWED_CHARS << "]" << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Input übertragen (\r loeschen weil das verbeliebende \n beim Laden in Streams (also bei <<) als \r\n umgewandelt wird)
		Description.erase(remove(Description.begin(), Description.end(), '\r'), Description.end());
		ProjectDescription = Trim(Description);

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Job in Datei speichern
int TKMCJobBase::SaveToFile(string Filename)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SaveToFile)" << endl;
			cout << "                Input: " << Trim(Filename) << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Datei oeffnen
		ofstream outfile(std::string(Trim(Filename)));
		if (outfile.is_open() == false)
		{
			cout << "Error: Invalid file name: " << Trim(Filename) << endl;
			cout << "       Cannot save to this file." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Double-Precision setzen
		outfile.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);

		// Daten in Datei schreiben
		int ErrorCode = SaveToStream(outfile, 0);
		outfile.close();

		if (ErrorCode != KMCERR_OK) return ErrorCode;
		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Job in String speichern
int TKMCJobBase::SaveToString(string& Output)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SaveToString)" << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Daten in Stringstream schreiben
		stringstream outstr;
		outstr.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
		int ErrorCode = SaveToStream(outstr, 0);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// Stringstream in String kopieren
		Output = outstr.str();

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Job aus Datei laden
int TKMCJobBase::LoadFromFile(string Filename)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::LoadFromFile)" << endl;
			cout << "                Input: " << Trim(Filename) << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Datei oeffnen
		ifstream infile(std::string(Trim(Filename)));
		if (infile.is_open() == false)
		{
			cout << "Error: Invalid file name: " << Trim(Filename) << endl;
			cout << "       Cannot read from this file." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Zeilenenden korrigieren
		string infilestr((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
		infile.close();
		infilestr.erase(remove(infilestr.begin(), infilestr.end(), '\r'), infilestr.end());
		stringstream infilestream(infilestr);

		// Daten aus Datei auslesen
		int ErrorCode = LoadFromStream(infilestream);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Job aus String laden
int TKMCJobBase::LoadFromString(string Input)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::LoadFromString)" << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Zeilenenden korrigieren
		Input.erase(remove(Input.begin(), Input.end(), '\r'), Input.end());
		stringstream instr(Input);

		// Daten aus Stringstream auslesen
		int ErrorCode = LoadFromStream(instr);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ******************************************** //
// -> Elemente
// ******************************************** //

// Eingegebene Elemente loeschen
int TKMCJobBase::ClearElements()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearElements)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Klasseninhalte loeschen
		return RCClasses(0);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Element falls noch nicht vorhanden hinzufügen
int TKMCJobBase::AddElement(string ElementSymbol, string ElementName, double ElementCharge)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::AddElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Elements pruefen
		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid m_Elements pointer (in TKMCJobBase::AddElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Elements->AddElement(ElementSymbol, ElementName, ElementCharge);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(1);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Element als bewegliche Spezies setzen
int TKMCJobBase::SetMovingElement(string ElementSymbol, string ElementName, double ElementCharge)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetMovingElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Elements pruefen
		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid m_Elements pointer (in TKMCJobBase::SetMovingElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Elements->SetMovingElement(ElementSymbol, ElementName, ElementCharge);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(1);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Elemente sortieren und Elementliste schliessen
int TKMCJobBase::MakeElements()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeElements)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Elements pruefen
		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid m_Elements pointer (in TKMCJobBase::MakeElements)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Elements->MakeElements();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(1);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ******************************************** //
// -> Struktur
// ******************************************** //

// Eingegebene Struktur loeschen
int TKMCJobBase::ClearStructure()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearStructure)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Klasseninhalte loeschen
		return RCClasses(1);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Name der Struktur eingeben, max. 40 Zeichen
int TKMCJobBase::SetStructureName(string StructureName)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetStructureName)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::SetStructureName)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->SetStructureName(StructureName);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Basis setzen aus 6 double (Längen, Winkel)
int TKMCJobBase::SetBasisByLatticeParam(double LengthA, double LengthB, double LengthC, double AngleA, double AngleB, double AngleC)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetBasisByLatticeParam)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::SetBasisByLatticeParam)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->SetBasisByLatticeParam(LengthA, LengthB, LengthC, AngleA, AngleB, AngleC);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Basis setzen aus 9 double (Vektoren)
int TKMCJobBase::SetBasisByLatticeVectors(double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetBasisByLatticeVectors)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::SetBasisByLatticeVectors)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->SetBasisByLatticeVectors(Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
int TKMCJobBase::AddAtom(double Xrel, double Yrel, double Zrel, string AtomSymbol, string AtomName, double AtomCharge)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::AddAtom)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::AddAtom)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->AddAtom(Xrel, Yrel, Zrel, AtomSymbol, AtomName, AtomCharge);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
int TKMCJobBase::AddAtom(double Xrel, double Yrel, double Zrel, int ElemID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::AddAtom)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::AddAtom)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->AddAtom(Xrel, Yrel, Zrel, ElemID);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Dotierung hinzufügen, noch keine Konzentrationen
int TKMCJobBase::AddDoping(string DopedSymbol, string DopedName, double DopedCharge, string DopandSymbol, string DopandName, double DopandCharge, double VacDopRatio)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::AddDoping)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::AddDoping)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->AddDoping(DopedSymbol, DopedName, DopedCharge, DopandSymbol, DopandName, DopandCharge, VacDopRatio);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Dotierung hinzufügen, noch keine Konzentrationen
int TKMCJobBase::AddDoping(int DopedID, int DopandID, double VacDopRatio)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::AddDoping)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::AddDoping)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->AddDoping(DopedID, DopandID, VacDopRatio);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Struktur sortieren und überprüfen, dann Ready = true setzen
int TKMCJobBase::MakeStructure()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeStructure)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Structure pruefen
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::MakeStructure)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Structure->MakeStructure();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ******************************************** //
// -> Gitterspruenge
// ******************************************** //

// Gitterspruenge loeschen
int TKMCJobBase::ClearJumps()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearJumps)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Klasseninhalte loeschen
		return RCClasses(2);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// mit Parametern MovShellCount und JumpShellCount alle Gittersprünge und deren Umgebung finden
int TKMCJobBase::MakeJumps(int MovShellCount, int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeJumps)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Jumps pruefen
		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid m_Jumps pointer (in TKMCJobBase::MakeJumps)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Jumps->MakeJumps(MovShellCount, JumpStartShellCount, JumpTSShellCount, JumpDestShellCount);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(3);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ******************************************** //
// -> Einzigartige Gitterspruenge
// ******************************************** //

// Einzigartige Gittersprünge löschen
int TKMCJobBase::ClearUniqueJumps()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearUniqueJumps)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Klasseninhalte loeschen
		return RCClasses(3);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Einzigartige Sprünge finden, VacShellCount = Schalenanzahl zur Unterscheidung von VV-Wechselwirkungen
int TKMCJobBase::MakeUniqueJumps()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeUniqueJumps)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::MakeUniqueJumps)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->MakeUniqueJumps();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Sprung aktivieren (= Sprung möglich) oder deaktivieren (= Sprung unmöglich), KMCERR_DEACTIVATION_IMPOSSIBLE zeigt, dass Sprung aktiviert bleiben muss
int TKMCJobBase::SetJumpActive(int JumpID, bool State)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetJumpActive)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetJumpActive)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetJumpActive(JumpID, State);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Umgebungsatom in einem Sprung ignorieren oder nicht-ignorieren
int TKMCJobBase::SetAtomIgnore(int JumpID, int CoordID, bool State)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetAtomIgnore)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetAtomIgnore)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetAtomIgnore(JumpID, CoordID, State);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Umgebungsatom in einem Sprung additiv oder nicht-additiv setzen
int TKMCJobBase::SetAtomAdditive(int JumpID, int CoordID, bool State)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetAtomAdditive)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetAtomAdditive)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetAtomAdditive(JumpID, CoordID, State);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Umgebungsatom in einem Sprung aktivieren oder deaktivieren
int TKMCJobBase::SetAtomActive(int JumpID, int CoordID, bool State)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetAtomActive)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetAtomActive)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetAtomActive(JumpID, CoordID, State);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Codierung loeschen
int TKMCJobBase::ClearUniqueCodes()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearUniqueCodes)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::ClearUniqueCodes)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->ClearUniqueCodes();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Mögliche Umgebungsbesetzungen bestimmen und vergleichen (mehrfaches Update möglich)
int TKMCJobBase::MakeUniqueCodes(int InteractionShellCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeUniqueCodes)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::MakeUniqueCodes)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->MakeUniqueCodes(InteractionShellCount);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// WW-Energie [eV] für bestimmte CoordID (nicht-ignoriert, nicht-additiv) und bestimmte (Elem)ID-Besetzung setzen
int TKMCJobBase::SetInteractionEnergyByCoord(int JumpID, int CoordID, int ID, double Energy)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetInteractionEnergyByCoord)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetInteractionEnergyByCoord)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetInteractionEnergyByCoord(JumpID, CoordID, ID, Energy);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// WW-Energien [eV] für bestimmte InteractionID und bestimmte (Elem)ID-Besetzung setzen
int TKMCJobBase::SetInteractionEnergyByInteraction(int WWID, int ID, double Energy)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetInteractionEnergyByInteraction)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetInteractionEnergyByInteraction)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetInteractionEnergyByInteraction(WWID, ID, Energy);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Code-Energie [eV] für bestimmte UniqueCodeID in bestimmtem UniqueJump setzen
int TKMCJobBase::SetCodeEnergy(int JumpID, int CodeID, double Energy)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetCodeEnergy)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_UniqueJumps pruefen
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SetCodeEnergy)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_UniqueJumps->SetCodeEnergy(JumpID, CodeID, Energy);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ******************************************** //
// -> Einstellungen
// ******************************************** //

// Einstellungen löschen
int TKMCJobBase::ClearSettings()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearSettings)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Klasseninhalte loeschen
		return RCClasses(4);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Temperatur einstellen
int TKMCJobBase::SetTemperature(double Temperature)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetTemperature)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetTemperature)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetTemperature(Temperature);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Sprungfrequenz einstellen
int TKMCJobBase::SetAttemptFrequency(double Frequency)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetAttemptFrequency)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetAttemptFrequency)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetAttemptFrequency(Frequency);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Checkpoint-Flags setzen
int TKMCJobBase::SetCheckpointFlags(bool WriteCheckpoint, bool LoadCheckpoint)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetCheckpointFlags)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetCheckpointFlags)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetCheckpointFlags(WriteCheckpoint, LoadCheckpoint);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// E-Feld setzen aus MaxEField = maximaler E-Feld-Beitrag zu Aktivierungsenergie [in kBT] und EFieldxyz = E-Feld-Richtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
int TKMCJobBase::SetEField(double EFieldX, double EFieldY, double EFieldZ, double MaxEField)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetEField)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetEField)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetEField(EFieldX, EFieldY, EFieldZ, MaxEField);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Gittergröße einstellen
int TKMCJobBase::SetLatticeSize(int LatticeSize)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetLatticeSize)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetLatticeSize)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetLatticeSize(LatticeSize);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Zusätzliche Leerstellen eingeben
int TKMCJobBase::SetAdditionalVacAnz(long long AdditionalVacAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetAdditionalVacAnz)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetAdditionalVacAnz)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetAdditionalVacAnz(AdditionalVacAnz);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Dotierkonzentration für eine bestimmte Dotierung eingeben
int TKMCJobBase::SetDopandConcentration(int DopingID, double Conc)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetDopandConcentration)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetDopandConcentration)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetDopandConcentration(DopingID, Conc);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// KMC-Vorlaufparameter einstellen
int TKMCJobBase::SetPrerunOptions(bool DoPrerun, long long PreMCSP, int PreRecordAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetPrerunOptions)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetPrerunOptions)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetPrerunOptions(DoPrerun, PreMCSP, PreRecordAnz);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Parameter für dyn. Normierungssimulation setzen
int TKMCJobBase::SetDynNormParameters(bool DoDynNorm, long long DynNormAttemptAnz, int DynNormRecordAnz, int DynNormNum)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetDynNormParameters)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetDynNormParameters)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetDynNormParameters(DoDynNorm, DynNormAttemptAnz, DynNormRecordAnz, DynNormNum);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// KMC-Hauptsimulationsparameter eingeben
int TKMCJobBase::SetMainKMCOptions(long long MCSP, int MainRecordAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetMainKMCOptions)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SetMainKMCOptions)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->SetMainKMCOptions(MCSP, MainRecordAnz);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen
int TKMCJobBase::MakeSettings()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeSettings)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Settings pruefen
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::MakeSettings)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Settings->MakeSettings();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ******************************************** //
// -> Simulation
// ******************************************** //

// Simulationsdaten löschen
int TKMCJobBase::ClearSimulation()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::ClearSimulation)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Klasseninhalte loeschen
		return RCClasses(5);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Simulationsdaten initialisieren
int TKMCJobBase::InitializeSimulation()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::InitializeSimulation)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Simulation pruefen
		if (m_Simulation == NULL)
		{
			cout << "Critical Error: Invalid m_Simulation pointer (in TKMCJobBase::InitializeSimulation)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Simulation->InitializeSimulation();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(6);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// CheckPoint-Dateipfad setzen
int TKMCJobBase::SetCheckPointPath(string CheckPointPath)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SetCheckPointPath)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Simulation pruefen
		if (m_Simulation == NULL)
		{
			cout << "Critical Error: Invalid m_Simulation pointer (in TKMCJobBase::SetCheckPointPath)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Simulation->SetCheckPointPath(CheckPointPath);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(6);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Daten aus CheckPoint-Datei laden
int TKMCJobBase::LoadCheckPoint()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::LoadCheckPoint)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// m_Simulation pruefen
		if (m_Simulation == NULL)
		{
			cout << "Critical Error: Invalid m_Simulation pointer (in TKMCJobBase::LoadCheckPoint)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		int ErrorCode = m_Simulation->LoadCheckPoint();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// uebrige Resultate falls vorhanden loeschen
		return RCClasses(6);
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Simulation durchfuehren
int TKMCJobBase::MakeSimulation()
{
	if (Ready != true)
	{
		cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::MakeSimulation)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// m_Simulation pruefen
	if (m_Simulation == NULL)
	{
		cout << "Critical Error: Invalid m_Simulation pointer (in TKMCJobBase::MakeSimulation)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}

	// Funktion aufrufen
	int ErrorCode = m_Simulation->MakeSimulation();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// uebrige Resultate falls vorhanden loeschen
	ErrorCode = RCClasses(6);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// m_Results pruefen
	if (m_Results == NULL)
	{
		cout << "Critical Error: Invalid m_Results pointer (in TKMCJobBase::MakeSimulation)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}

	// Funktion aufrufen
	ErrorCode = m_Results->MakeResults();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// uebrige Resultate falls vorhanden loeschen
	return RCClasses(7);

	// Achtung: Kein Exception-Handling auf dieser Funktion da dies die Simulation verlangsamen würde
}

// ***************************** PUBLIC *********************************** //

// Rueckgabe von Ready
bool TKMCJobBase::IfReady()
{
	return Ready;
}

// ***************************** PROTECTED ********************************** //

// Projekt in einen Stream schreiben
int TKMCJobBase::SaveToStream(ostream& output, int offset)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::SaveToStream)" << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Input pruefen
		if (output.fail() == true)
		{
			cout << "Critical Error: Output stream not ready (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}
		if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET))
		{
			cout << "Critical Error: Offset out of bounds (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_INPUT_CRIT;
		}
		string s_offset = "";
		if (offset > 0) s_offset = string(offset, ' ');

		// Offset des untergeordneten Outputs definieren
		string sub_offset = "";
		if ((int)KMCOUT_TKMCJOB_OFFSET > 0) sub_offset = string((int)KMCOUT_TKMCJOB_OFFSET, ' ');

		// Ausgabe der untergeordneten Objekte ermitteln
		vector<string> t_outstr;
		int ErrorCode = KMCERR_OK;

		// Elemente schreiben
		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid m_Elements pointer (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		else
		{
			if (m_Elements->IfReady() == true)
			{
				stringstream t_stream;
				t_stream.precision(output.precision());
				ErrorCode = m_Elements->SaveToStream(t_stream, offset + KMCOUT_TKMCJOB_OFFSET);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				t_outstr.push_back(t_stream.str());
			}
		}

		// Struktur schreiben
		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid m_Structure pointer (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		else
		{
			if (m_Structure->IfReady() == true)
			{
				stringstream t_stream;
				t_stream.precision(output.precision());
				ErrorCode = m_Structure->SaveToStream(t_stream, offset + KMCOUT_TKMCJOB_OFFSET);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				t_outstr.push_back(t_stream.str());
			}
		}

		// Spruenge schreiben
		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid m_Jumps pointer (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		else
		{
			if (m_Jumps->IfReady() == true)
			{
				stringstream t_stream;
				t_stream.precision(output.precision());
				ErrorCode = m_Jumps->SaveToStream(t_stream, offset + KMCOUT_TKMCJOB_OFFSET);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				t_outstr.push_back(t_stream.str());
			}
		}

		// Einzigartige Spruenge schreiben
		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid m_UniqueJumps pointer (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		else
		{
			if (m_UniqueJumps->IfJumpsReady() == true)
			{
				stringstream t_stream;
				t_stream.precision(output.precision());
				ErrorCode = m_UniqueJumps->SaveToStream(t_stream, offset + KMCOUT_TKMCJOB_OFFSET);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				t_outstr.push_back(t_stream.str());
			}
		}

		// Einstellungen schreiben
		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid m_Settings pointer (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		else
		{
			if (m_Settings->IfReady() == true)
			{
				stringstream t_stream;
				t_stream.precision(output.precision());
				ErrorCode = m_Settings->SaveToStream(t_stream, offset + KMCOUT_TKMCJOB_OFFSET);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				t_outstr.push_back(t_stream.str());
			}
		}

		// Ergebnisse schreiben
		if (m_Results == NULL)
		{
			cout << "Critical Error: Invalid m_Results pointer (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		else
		{
			if (m_Results->IfReady() == true)
			{
				stringstream t_stream;
				t_stream.precision(output.precision());
				ErrorCode = m_Results->SaveToStream(t_stream, offset + KMCOUT_TKMCJOB_OFFSET);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				t_outstr.push_back(t_stream.str());
			}
		}

		// Ausgabeparameter setzen
		output.unsetf(ios::floatfield);
		output.setf(ios::right, ios::adjustfield);

		// Dateiheader schreiben
		output << s_offset << "JOB FILE" << endl;
		output << s_offset << "iCon - Kinetic Monte-Carlo Simulation of Ionic Conductivity" << endl;
		output << s_offset << "Copyright 2014" << endl;
		output << s_offset << "RWTH Aachen, IPC, Workgroup Martin" << endl;
		output << s_offset << KMCOUT_VERSION << " " << KMC_VERSION << endl;
		output << s_offset << "File creation time: " << TCustomTime::GetCurrentTimeStr() << endl;
		output << endl;

		// Job-Bezeichner setzen
		output << s_offset << KMCOUT_TKMCJOB_START << endl;

		// Job-Name + aktuelles Datum + Benutzername + Beschreibung setzen
		output << s_offset << sub_offset << KMCOUT_TKMCJOB_NAME << " " << ProjectName << endl;
		if (UserName.length() != 0)
		{
			output << s_offset << sub_offset << KMCOUT_TKMCJOB_USERNAME << " " << UserName << endl;
		}
		output << s_offset << sub_offset << KMCOUT_TKMCJOB_DATE << " " << ProjectDateDay << " . " << ProjectDateMonth << " . " << ProjectDateYear << endl;
		if (ProjectDescription.length() != 0)
		{
			output << endl;
			output << s_offset << sub_offset << KMCOUT_TKMCJOB_DESCSTART << endl;
			output << ProjectDescription << endl;
			output << s_offset << sub_offset << KMCOUT_TKMCJOB_DESCEND << endl;
		}

		// Ausgabe der Streams der untergeordneten Objekte
		if (t_outstr.size() != 0)
		{
			for (int i = 0; i < (int)t_outstr.size(); i++)
			{
				output << endl;
				output << t_outstr[i];
				output.flush();
			}
		}

		output << s_offset << KMCOUT_TKMCJOB_END << endl;

		if (output.fail() == true)
		{
			cout << "Critical Error: Output stream failure (in TKMCJobBase::SaveToStream)" << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}
		return KMCERR_OK;
	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Projekt aus einem Stream laden
int TKMCJobBase::LoadFromStream(istream& input)
{
	TElements* t_Elements = NULL;
	TStructure* t_Structure = NULL;
	TJumps* t_Jumps = NULL;
	TUniqueJumps* t_UniqueJumps = NULL;
	TSettings* t_Settings = NULL;
	TResults* t_Results = NULL;
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobBase::LoadFromStream)" << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Input pruefen
		if (input.good() == false)
		{
			cout << "Critical Error: Input stream not ready (in TKMCJobBase::LoadFromStream)" << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}

		// Inputparameter setzen
		input >> skipws;

		// Bisherigen Input zwischenspeichern und loeschen (nur NULL setzen, noch kein delete)
		t_Elements = m_Elements;
		m_Elements = NULL;
		t_Structure = m_Structure;
		m_Structure = NULL;
		t_Jumps = m_Jumps;
		m_Jumps = NULL;
		t_UniqueJumps = m_UniqueJumps;
		m_UniqueJumps = NULL;
		t_Settings = m_Settings;
		m_Settings = NULL;
		t_Results = m_Results;
		m_Results = NULL;

		// Datei oeffnen
		int ErrorCode = KMCERR_OK;
		string line = "";
		string s_temp = "";
		bool job_found = false;
		bool if_failed = false;
		while (input.good() == true)
		{
			// Zeile laden
			if (getline(input, line).fail() == true)
			{
				if_failed = true;
				break;
			}
			// Zeile interpretieren
			stringstream linestream(line);
			if ((linestream >> s_temp).fail() == true) s_temp = "";

			// Version pruefen (nur wenn Version angegeben ist)
			if (s_temp == KMCOUT_VERSION)
			{
				if ((linestream >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
				if (string(KMC_COMP_FILE_VERSIONS).find(s_temp) == string::npos)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Job laden
			if (s_temp == KMCOUT_TKMCJOB_START)
			{
				// Alle Resultate falls vorhanden loeschen
				RCClasses(0);

				ProjectName = KMCVAR_PROJECTNAME_INIT;
				UserName = KMCVAR_USERNAME_INIT;
				SetProjectDate();
				ProjectDescription = KMCVAR_PROJECTDESC_INIT;
				job_found = true;
				continue;
			}

			// Projektnamen laden
			if ((s_temp == KMCOUT_TKMCJOB_NAME) && (job_found == true))
			{
				if (getline(linestream, s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
				if (SetProjectName(s_temp) != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Usernamen laden
			if ((s_temp == KMCOUT_TKMCJOB_USERNAME) && (job_found == true))
			{
				if (getline(linestream, s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
				if (SetUserName(s_temp) != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Datum laden
			if ((s_temp == KMCOUT_TKMCJOB_DATE) && (job_found == true))
			{
				int i_Day = 0;
				int i_Month = 0;
				int i_Year = 0;
				if ((linestream >> i_Day).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((i_Day < 1) || (i_Day > 31))
				{
					if_failed = true;
					break;
				}
				if ((linestream >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((linestream >> i_Month).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((i_Month < 1) || (i_Month > 12))
				{
					if_failed = true;
					break;
				}
				if ((linestream >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((linestream >> i_Year).fail() == true)
				{
					if_failed = true;
					break;
				}
				if (i_Year < 1900)
				{
					if_failed = true;
					break;
				}
				ProjectDateDay = i_Day;
				ProjectDateMonth = i_Month;
				ProjectDateYear = i_Year;
				continue;
			}

			// Projektbeschreibung laden
			if ((s_temp == KMCOUT_TKMCJOB_DESCSTART) && (job_found == true))
			{
				vector<string> i_Desc;
				do
				{
					if (getline(input, line).fail() == true)
					{
						if_failed = true;
						break;
					}
					stringstream desc_linestream(line);
					if ((desc_linestream >> s_temp).fail() == true) s_temp = "";
					if (s_temp != KMCOUT_TKMCJOB_DESCEND) i_Desc.emplace_back(RightTrim(line));
				} while ((s_temp != KMCOUT_TKMCJOB_DESCEND) && (input.good() == true));
				if ((if_failed == true) || (input.good() != true))
				{
					if_failed = true;
					break;
				}
				if (i_Desc.size() == 0)
				{
					if (SetProjectDescription("") != KMCERR_OK)
					{
						if_failed = true;
						break;
					}
				}
				else
				{
					stringstream ssDesc("");
					ssDesc << i_Desc[0];
					if ((int)i_Desc.size() > 1)
					{
						for (int i = 1; i < (int)i_Desc.size(); i++)
						{
							ssDesc << endl << i_Desc[i];
						}
					}
					ssDesc.flush();
					if (SetProjectDescription(ssDesc.str()) != KMCERR_OK)
					{
						if_failed = true;
						break;
					}
				}
				continue;
			}

			// Elemente laden
			if ((s_temp == KMCOUT_TELEMENTS_START) && (job_found == true))
			{
				// m_Elements loeschen
				ErrorCode = ClearElements();
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}

				// m_Elements laden
				ErrorCode = m_Elements->LoadFromStream(input);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Struktur laden
			if ((s_temp == KMCOUT_TSTRUCTURE_START) && (job_found == true))
			{
				// m_Structure loeschen
				ErrorCode = ClearStructure();
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}

				// m_Structure laden
				ErrorCode = m_Structure->LoadFromStream(input);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Spruenge laden
			if ((s_temp == KMCOUT_TJUMPS_START) && (job_found == true))
			{
				// m_Jumps loeschen
				ErrorCode = ClearJumps();
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}

				// m_Jumps laden
				ErrorCode = m_Jumps->LoadFromStream(input);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Einzigartige Spruenge laden
			if ((s_temp == KMCOUT_TUNIQUEJUMPS_START) && (job_found == true))
			{
				// m_UniqueJumps loeschen
				ErrorCode = ClearUniqueJumps();
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}

				ErrorCode = m_UniqueJumps->LoadFromStream(input);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Einstellungen laden
			if ((s_temp == KMCOUT_TSETTINGS_START) && (job_found == true))
			{
				// m_Settings loeschen
				ErrorCode = ClearSettings();
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}

				ErrorCode = m_Settings->LoadFromStream(input);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Ergebnisse laden
			if ((s_temp == KMCOUT_TRESULTS_START) && (job_found == true))
			{
				// m_Results loeschen
				RCClasses(6);

				ErrorCode = m_Results->LoadFromStream(input);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
				continue;
			}

			// Job vollstaendig
			if (s_temp == KMCOUT_TKMCJOB_END)
			{
				job_found = false;
				break;
			}
		}
		if ((if_failed == true) && (ErrorCode == KMCERR_OK))
		{
			cout << "Error: Invalid file format" << endl << endl;
			ErrorCode = KMCERR_INVALID_FILE_FORMAT;
		}
		if (if_failed == true)
		{
			delete m_Results;
			delete m_Settings;
			delete m_UniqueJumps;
			delete m_Jumps;
			delete m_Structure;
			delete m_Elements;
			m_Elements = t_Elements;
			m_Structure = t_Structure;
			m_Jumps = t_Jumps;
			m_UniqueJumps = t_UniqueJumps;
			m_Settings = t_Settings;
			m_Results = t_Results;
			return ErrorCode;
		}

		delete t_Results;
		delete t_Settings;
		delete t_UniqueJumps;
		delete t_Jumps;
		delete t_Structure;
		delete t_Elements;
		return KMCERR_OK;
	}
	catch (exception& e)
	{
		delete t_Results;
		delete t_Settings;
		delete t_UniqueJumps;
		delete t_Jumps;
		delete t_Structure;
		delete t_Elements;
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		delete t_Results;
		delete t_Settings;
		delete t_UniqueJumps;
		delete t_Jumps;
		delete t_Structure;
		delete t_Elements;
		return ExceptionHandler();
	}
}

// Deletes and Recreates TElements Member Class
int TKMCJobBase::RCElements()
{
	delete m_Elements;
	m_Elements = new TElements(dynamic_cast<TKMCJob*>(this));
	if (m_Elements == NULL)
	{
		cout << "Critical Error: TElements allocation failed (in TKMCJobBase::RCElements)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates TStructure Member Class
int TKMCJobBase::RCStructure()
{
	delete m_Structure;
	m_Structure = new TStructure(dynamic_cast<TKMCJob*>(this));
	if (m_Structure == NULL)
	{
		cout << "Critical Error: TStructure allocation failed (in TKMCJobBase::RCStructure)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates TJumps Member Class
int TKMCJobBase::RCJumps()
{
	delete m_Jumps;
	m_Jumps = new TJumps(dynamic_cast<TKMCJob*>(this));
	if (m_Jumps == NULL)
	{
		cout << "Critical Error: TJumps allocation failed (in TKMCJobBase::RCJumps)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates TUniqueJumps Member Class
int TKMCJobBase::RCUniqueJumps()
{
	delete m_UniqueJumps;
	m_UniqueJumps = new TUniqueJumps(dynamic_cast<TKMCJob*>(this));
	if (m_UniqueJumps == NULL)
	{
		cout << "Critical Error: TUniqueJumps allocation failed (in TKMCJobBase::RCUniqueJumps)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates TSettings Member Class
int TKMCJobBase::RCSettings()
{
	delete m_Settings;
	m_Settings = new TSettings(dynamic_cast<TKMCJob*>(this));
	if (m_Settings == NULL)
	{
		cout << "Critical Error: TSettings allocation failed (in TKMCJobBase::RCSettings)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates TSimulation Member Class
int TKMCJobBase::RCSimulation()
{
	delete m_Simulation;
	m_Simulation = new TSimulation(dynamic_cast<TKMCJob*>(this));
	if (m_Simulation == NULL)
	{
		cout << "Critical Error: TSimulation allocation failed (in TKMCJobBase::RCSimulation)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates TResults Member Class
int TKMCJobBase::RCResults()
{
	delete m_Results;
	m_Results = new TResults(dynamic_cast<TKMCJob*>(this));
	if (m_Results == NULL)
	{
		cout << "Critical Error: TResults allocation failed (in TKMCJobBase::RCResults)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	return KMCERR_OK;
}

// Deletes and Recreates Member Classes From FromClass (zero-based, in order of declaration)
int TKMCJobBase::RCClasses(int FromClass)
{
	int ErrorCode = KMCERR_OK;

	if ((FromClass < 0) || (FromClass > 10))
	{
		cout << "Critical Error: Invalid Class Index (in TKMCJobBase::RCClasses)" << endl;
		cout << "                Input: " << FromClass << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	if ((FromClass <= 6) && (ErrorCode == KMCERR_OK)) ErrorCode = RCResults();
	if ((FromClass <= 5) && (ErrorCode == KMCERR_OK)) ErrorCode = RCSimulation();
	if ((FromClass <= 4) && (ErrorCode == KMCERR_OK)) ErrorCode = RCSettings();
	if ((FromClass <= 3) && (ErrorCode == KMCERR_OK)) ErrorCode = RCUniqueJumps();
	if ((FromClass <= 2) && (ErrorCode == KMCERR_OK)) ErrorCode = RCJumps();
	if ((FromClass <= 1) && (ErrorCode == KMCERR_OK)) ErrorCode = RCStructure();
	if ((FromClass <= 0) && (ErrorCode == KMCERR_OK)) ErrorCode = RCElements();

	return ErrorCode;
}

// Global Standard Exception Handler
int TKMCJobBase::StdExceptionHandler(exception& e)
{
	cout << KMCERRSTR_STDERR << " " << e.what() << endl;
	return KMCERR_EXCEPTION_OCCURED;
}

// Global Exception Handler for unknown Exceptions
int TKMCJobBase::ExceptionHandler()
{
	cout << KMCERRSTR_UNKNOWNERR << endl;
	return KMCERR_EXCEPTION_OCCURED;
}