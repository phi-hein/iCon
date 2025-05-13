// **************************************************************** //
//																	//
//	Klasse: TKMCJobOut	(TKMCJob Layer 2)							//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung eines KMC-Jobs							//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TKMCJob/TKMCJobOut.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "TElements/TElements.h"
#include "TStructure/TStructure.h"
#include "TJumps/TJumps.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "TSettings/TSettings.h"
#include "TSimulation/TSimulation.h"
#include "TResults/TResults.h"
#include "GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TKMCJobOut::TKMCJobOut() : TKMCJobFunc()
{

}

// Destructor
TKMCJobOut::~TKMCJobOut()
{

}

// **************************** PUBLISHED ********************************* //

// Projektnamen ausgeben
int TKMCJobOut::GetProjectName(string& Name)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetProjectName)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		Name = ProjectName;

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

// Usernamen ausgeben
int TKMCJobOut::GetUserName(string& Name)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUserName)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		Name = UserName;

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

// Projektdatum ausgeben
int TKMCJobOut::GetProjectDate(string& CreateDate)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetProjectDate)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		CreateDate = "";
		if (IntToStr(ProjectDateDay).length() == 1) CreateDate += "0";
		CreateDate += IntToStr(ProjectDateDay) + ".";
		if (IntToStr(ProjectDateMonth).length() == 1) CreateDate += "0";
		CreateDate += IntToStr(ProjectDateMonth) + "." + IntToStr(ProjectDateYear);

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

// Projektbeschreibung ausgeben
int TKMCJobOut::GetProjectDescription(string& Description)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetProjectDescription)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		Description = ProjectDescription;

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

// Ausgabe des Projectstatus
int TKMCJobOut::GetProjectState(int& State)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		State = 0;

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid elements pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Elements->IfReady() == true) State = 1; else return KMCERR_OK;

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid structure pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Structure->IfReady() == true) State = 2; else return KMCERR_OK;

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid jumps pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Jumps->IfReady() == true) State = 3; else return KMCERR_OK;

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid unique jumps pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_UniqueJumps->IfJumpsReady() == true) State = 4; else return KMCERR_OK;
		if (m_UniqueJumps->IfCodesReady() == true) State = 5; else return KMCERR_OK;

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid settings pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Settings->IfReady() == true) State = 6; else return KMCERR_OK;

		if (m_Simulation == NULL)
		{
			cout << "Critical Error: Invalid simulation pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Simulation->IfReady() == true) State = 7;
		if (m_Simulation->IfCompleted() == true) State = 8;

		if (m_Results == NULL)
		{
			cout << "Critical Error: Invalid results pointer (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		long long required_MCSP;
		int rec_anz;
		if (m_Settings->GetMainKMCOptions(required_MCSP, rec_anz) != KMCERR_OK)
		{
			cout << "Critical Error: Cannot get required MCSP (in TKMCJobOut::GetProjectState)" << endl << endl;
			return KMCERR_INVALID_INPUT_CRIT;
		}
		if (m_Results->IfCompleted(required_MCSP) == true) State = 9; else return KMCERR_OK;

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

// Gesamtes Projekt in Konsole schreiben
int TKMCJobOut::WriteToConsole()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		// Funktion aufrufen
		cout.precision(KMCOUT_TKMCJOB_COUT_PRECISION);
		return SaveToStream(cout, 0);
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

// Anzahl der Elemente ausgeben
int TKMCJobOut::GetElementCount(int& ElementCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetElementCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetElementCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetElementCount(ElementCount);
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

// Ausgabe eines Elements
int TKMCJobOut::GetElement(int ElementID, string& ElementSymbol, string& ElementName, double& ElementCharge)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetElement(ElementID, ElementSymbol, ElementName, ElementCharge);
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

// Ausgabe eines Elements
int TKMCJobOut::GetElement(int ElementID, string& ElementSymbol, string& ElementName, double& ElementCharge,
	double& Radius, double& ColorR, double& ColorG, double& ColorB)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetElement(ElementID, ElementSymbol, ElementName, ElementCharge, Radius, ColorR, ColorG, ColorB);
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

// Ausgabe des MovingElement
int TKMCJobOut::GetMovingElement(string& ElementSymbol, string& ElementName, double& ElementCharge)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetMovingElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetMovingElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetMovingElement(ElementSymbol, ElementName, ElementCharge);
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

// Ausgabe des MovingElement
int TKMCJobOut::GetMovingElement(string& ElementSymbol, string& ElementName, double& ElementCharge,
	double& Radius, double& ColorR, double& ColorG, double& ColorB)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetMovingElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetMovingElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetMovingElement(ElementSymbol, ElementName, ElementCharge, Radius, ColorR, ColorG, ColorB);
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

// Ausgabe des VacancyElement
int TKMCJobOut::GetVacancyElement(string& ElementSymbol, string& ElementName, double& ElementCharge)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetVacancyElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetVacancyElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetVacancyElement(ElementSymbol, ElementName, ElementCharge);
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

// Ausgabe des VacancyElement
int TKMCJobOut::GetVacancyElement(string& ElementSymbol, string& ElementName, double& ElementCharge,
	double& Radius, double& ColorR, double& ColorG, double& ColorB)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetVacancyElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetVacancyElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Elements->GetVacancyElement(ElementSymbol, ElementName, ElementCharge, Radius, ColorR, ColorG, ColorB);
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

// Elementbeschreibung in Konsole schreiben
int TKMCJobOut::WriteElementsToConsole()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteElementsToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Elements == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteElementsToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		if (m_Elements->IfReady() == false)
		{
			cout << "Error: Cannot print elements to console." << endl;
			cout << "       Current elements are incomplete." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Funktion aufrufen
		cout.precision(KMCOUT_TELEMENTS_COUT_PRECISION);
		return m_Elements->SaveToStream(cout, 0);
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

// Name der Struktur ausgeben
int TKMCJobOut::GetStructureName(string& StructureName)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetStructureName)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetStructureName)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetStructureName(StructureName);
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

// Basis ausgeben als 6 double (Längen, Winkel)
int TKMCJobOut::GetBasisByLatticeParam(double& LengthA, double& LengthB, double& LengthC, double& AngleA, double& AngleB, double& AngleC)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetBasisByLatticeParam)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetBasisByLatticeParam)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetBasisByLatticeParam(LengthA, LengthB, LengthC, AngleA, AngleB, AngleC);
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

// Basis ausgeben als 9 double (Vektoren)
int TKMCJobOut::GetBasisByLatticeVectors(double& Ax, double& Ay, double& Az, double& Bx, double& By, double& Bz, double& Cx, double& Cy, double& Cz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetBasisByLatticeVectors)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetBasisByLatticeVectors)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetBasisByLatticeVectors(Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz);
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

// Anzahl an Elementarzellatomen ausgeben
int TKMCJobOut::GetAtomCount(int& AtomCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetAtomCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetAtomCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetAtomCount(AtomCount);
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

// Anzahl der Mov--Atome in der Elementarzelle ausgeben
int TKMCJobOut::GetMovCount(int& MovCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetMovVacCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetMovVacCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetMovCount(MovCount);
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

// Atom ausgeben
int TKMCJobOut::GetAtom(int AtomID, double& Xrel, double& Yrel, double& Zrel, int& ElemID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetAtom)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetAtom)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetAtom(AtomID, Xrel, Yrel, Zrel, ElemID);
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

// Anzahl an Dotierungen ausgeben
int TKMCJobOut::GetDopingCount(int& DopingCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetDopingCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetDopingCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetDopingCount(DopingCount);
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

// Dotierung ausgeben
int TKMCJobOut::GetDoping(int DopingID, int& DopedID, int& DopandID, double& VacDopRatio)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetDoping)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetDoping)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Structure->GetDoping(DopingID, DopedID, DopandID, VacDopRatio);
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

// Strukturbeschreibung in Konsole schreiben
int TKMCJobOut::WriteStructureToConsole()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteStructureToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Structure == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteStructureToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		if (m_Structure->IfReady() == false)
		{
			cout << "Error: Cannot print structure to console." << endl;
			cout << "       Current structure is incomplete." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Funktion aufrufen
		cout.precision(KMCOUT_TSTRUCTURE_COUT_PRECISION);
		return m_Structure->SaveToStream(cout, 0);
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

// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
int TKMCJobOut::GetShellCounts(int& MovShellCount, int& JumpStartShellCount, int& JumpTSShellCount, int& JumpDestShellCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetShellCounts)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetShellCounts)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetShellCounts(MovShellCount, JumpStartShellCount, JumpTSShellCount, JumpDestShellCount);
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

// Anzahl an Sprungrichtungen fuer bestimmtes Mov- bzw. Vac-Atom in Elementarzelle
int TKMCJobOut::GetJumpDirCount(int AtomID, int& DirectionCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpDirCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpDirCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpDirCount(AtomID, DirectionCount);
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

// 4D-Vektor des Sprungstartatoms ausgeben
int TKMCJobOut::GetJumpStartPos(int AtomID, int DirID, int& x, int& y, int& z, int& s)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpStartPos)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpStartPos)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpStartPos(AtomID, DirID, x, y, z, s);
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

// 4D-Vektor des Sprungzielatoms ausgeben
int TKMCJobOut::GetJumpDestPos(int AtomID, int DirID, int& x, int& y, int& z, int& s)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpDestPos)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpDestPos)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpDestPos(AtomID, DirID, x, y, z, s);
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

// Ruecksprung-DirID ausgeben
int TKMCJobOut::GetJumpBackjumpDirID(int AtomID, int DirID, int& BackjumpDirID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpBackjumpDirID)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpBackjumpDirID)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpBackjumpDirID(AtomID, DirID, BackjumpDirID);
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

// Anzahl an Umgebungsatomen ausgeben
int TKMCJobOut::GetJumpEnvCount(int AtomID, int DirID, int& EnvAtomCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpEnvCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpEnvCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpEnvCount(AtomID, DirID, EnvAtomCount);
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

// Umgebungsatom ausgeben
int TKMCJobOut::GetJumpEnvAtom(int AtomID, int DirID, int EnvAtomID, int& x, int& y, int& z, int& s)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpEnvAtom)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpEnvAtom)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpEnvAtom(AtomID, DirID, EnvAtomID, x, y, z, s);
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

// ID des zugehörigen UniqueJump ausgeben
int TKMCJobOut::GetJumpUniqueJumpID(int AtomID, int DirID, int& UniqueJumpID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetJumpUniqueJumpID)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetJumpUniqueJumpID)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Jumps->GetJumpUniqueJumpID(AtomID, DirID, UniqueJumpID);
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

// Sprungbeschreibungen in Konsole schreiben
int TKMCJobOut::WriteJumpsToConsole()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteJumpsToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Jumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteJumpsToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		if (m_Jumps->IfReady() == false)
		{
			cout << "Error: Cannot print jumps to console." << endl;
			cout << "       Current jumps are incomplete." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Funktion aufrufen
		cout.precision(KMCOUT_TJUMPS_COUT_PRECISION);
		return m_Jumps->SaveToStream(cout, 0);
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

// Anzahl an UJumps ausgeben
int TKMCJobOut::GetUJumpCount(int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpCount(Count);
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

// Laenge eines Sprungs ausgeben
int TKMCJobOut::GetUJumpLength(int JumpID, double& JumpLength)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpLength)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpLength)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpLength(JumpID, JumpLength);
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

// Aktivierungszustand eines Sprungs ausgeben
int TKMCJobOut::GetUJumpActive(int JumpID, bool& JumpState)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpActive)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpActive)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpActive(JumpID, JumpState);
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

// Anzahl an Umgebungsatomen in bestimmtem UJump ausgeben
int TKMCJobOut::GetUJumpAtomCount(int JumpID, int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpAtomCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpAtomCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpAtomCount(JumpID, Count);
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

// Element eines Umgebungsatoms eines bestimmten UJump ausgeben
int TKMCJobOut::GetUJumpAtomElement(int JumpID, int CoordID, int& ElemID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpAtomElement)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpAtomElement)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpAtomElement(JumpID, CoordID, ElemID);
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

// Essentielle Informationen zu einem Umgebungsatom eines bestimmten UJump ausgeben
int TKMCJobOut::GetUJumpAtomZylPosition(int JumpID, int CoordID, double& X, double& Y, double& Z)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpAtomZylPosition)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpAtomZylPosition)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpAtomZylPosition(JumpID, CoordID, X, Y, Z);
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

// Zustände (ignore, additive, active) eines Umgebungsatoms eines UJumps ausgeben
int TKMCJobOut::GetUJumpAtomStates(int JumpID, int CoordID, bool& IsIgnore, bool& IsAdditive, bool& IsActive)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpAtomStates)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpAtomStates)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpAtomStates(JumpID, CoordID, IsIgnore, IsAdditive, IsActive);
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

// Abstaende eines Umgebungsatoms eines bestimmten UJump ausgeben
int TKMCJobOut::GetUJumpAtomDistances(int JumpID, int CoordID, double& StartDist, double& TSDist, double& DestDist)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpAtomDistances)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpAtomDistances)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpAtomDistances(JumpID, CoordID, StartDist, TSDist, DestDist);
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

// Anzahl an WW-Atomen (nicht-ignoriert, additiv) in bestimmtem UJump ausgeben
int TKMCJobOut::GetUJumpWWAtomsCount(int JumpID, int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpWWAtomsCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpWWAtomsCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpWWAtomsCount(JumpID, Count);
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

// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
int TKMCJobOut::GetUJumpWWAtomsInfo(int JumpID, int WWAtomsID, int& CoordID, int& WWID, int& EnergAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpWWAtomsInfo)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpWWAtomsInfo)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpWWAtomsInfo(JumpID, WWAtomsID, CoordID, WWID, EnergAnz);
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

// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben
int TKMCJobOut::GetUJumpWWAtomsEnergy(int JumpID, int WWAtomsID, int ID, int& WWElemID, double& WWEnergy)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpWWAtomsEnergy)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpWWAtomsEnergy)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpWWAtomsEnergy(JumpID, WWAtomsID, ID, WWElemID, WWEnergy);
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

// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
int TKMCJobOut::GetUJumpUCodesCount(int JumpID, int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpUCodesCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpUCodesCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpUCodesCount(JumpID, Count);
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

// Energie und Code für einen UniqueCode in einem UJump ausgeben
int TKMCJobOut::GetUJumpUCode(int JumpID, int CodeID, string& Code, double& CodeEnergy)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpUCode)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpUCode)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpUCode(JumpID, CodeID, Code, CodeEnergy);
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

// Anzahl an FullCodes in bestimmtem UJump ausgeben
int TKMCJobOut::GetUJumpFCodesCount(int JumpID, int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpFCodesCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpFCodesCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpFCodesCount(JumpID, Count);
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

// UCodeID und Code für einen FullCode in einem UJump ausgeben
int TKMCJobOut::GetUJumpFCode(int JumpID, int CodeID, string& Code, int& UCodeID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetUJumpFCode)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetUJumpFCode)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetUJumpFCode(JumpID, CodeID, Code, UCodeID);
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

// InteractionShellCount ausgeben
int TKMCJobOut::GetInteractionShellCount(int& ShellCount)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionShellCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionShellCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionShellCount(ShellCount);
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

// Anzahl an Interactions ausgeben
int TKMCJobOut::GetInteractionCount(int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionCount(Count);
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

// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
int TKMCJobOut::GetInteractionDesc(int WWID, int& ElemID, double& StartDist, double& DestDist)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionDesc)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionDesc)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionDesc(WWID, ElemID, StartDist, DestDist);
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

// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TKMCJobOut::GetInteractionStartEnvCount(int WWID, int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionStartEnvCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionStartEnvCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionStartEnvCount(WWID, Count);
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

// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TKMCJobOut::GetInteractionDestEnvCount(int WWID, int& Count)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionDestEnvCount)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionDestEnvCount)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionDestEnvCount(WWID, Count);
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

// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TKMCJobOut::GetInteractionStartEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionStartEnvAtom)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionStartEnvAtom)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionStartEnvAtom(WWID, ID, ElemID, X, Y, Z);
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

// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TKMCJobOut::GetInteractionDestEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetInteractionDestEnvAtom)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetInteractionDestEnvAtom)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->GetInteractionDestEnvAtom(WWID, ID, ElemID, X, Y, Z);
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

// Vollständige Ausgabe eines UJump
int TKMCJobOut::WriteUJumpToConsole(int JumpID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteUJumpToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteUJumpToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->WriteUJumpToConsole(JumpID);
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

// Vollständige Ausgabe einer Wechselwirkung
int TKMCJobOut::WriteInteractionToConsole(int WWID)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteInteractionToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteInteractionToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_UniqueJumps->WriteInteractionToConsole(WWID);
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

// Beschreibung der einzigartigen Sprünge in Konsole schreiben
int TKMCJobOut::WriteUniqueJumpsToConsole()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteUniqueJumpsToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_UniqueJumps == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteUniqueJumpsToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		if (m_UniqueJumps->IfJumpsReady() == false)
		{
			cout << "Error: Cannot print unique jumps to console." << endl;
			cout << "       Current unique jumps are incomplete." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Funktion aufrufen
		cout.precision(KMCOUT_TUNIQUEJUMPS_COUT_PRECISION);
		return m_UniqueJumps->SaveToStream(cout, 0);
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

// Temperatur ausgeben
int TKMCJobOut::GetTemperature(double& Temperature)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetTemperature)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetTemperature)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetTemperature(Temperature);
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

// Sprungfrequenz ausgeben
int TKMCJobOut::GetAttemptFrequency(double& Frequency)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetAttemptFrequency)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetAttemptFrequency)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetAttemptFrequency(Frequency);
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

// Checkpoint-Flags ausgeben
int TKMCJobOut::GetCheckpointFlags(bool& WriteCheckpoint, bool& LoadCheckpoint)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetCheckpointFlags)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetCheckpointFlags)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetCheckpointFlags(WriteCheckpoint, LoadCheckpoint);
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

// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
int TKMCJobOut::GetEFieldSettings(double& EFieldDirX, double& EFieldDirY, double& EFieldDirZ, double& MaxEField)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetEFieldSettings)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetEFieldSettings)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetEFieldSettings(EFieldDirX, EFieldDirY, EFieldDirZ, MaxEField);
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

// Berechnetes E-Feld ausgeben
int TKMCJobOut::GetEField(double& EFieldX, double& EFieldY, double& EFieldZ)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetEField)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetEField)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetEField(EFieldX, EFieldY, EFieldZ);
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

// Gittergröße ausgeben
int TKMCJobOut::GetLatticeSize(int& LatticeSize)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetLatticeSize)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetLatticeSize)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetLatticeSize(LatticeSize);
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

// Zusätzliche Leerstellen ausgeben
int TKMCJobOut::GetAdditionalVacAnz(long long& AdditionalVacAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetAdditionalVacAnz)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetAdditionalVacAnz)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetAdditionalVacAnz(AdditionalVacAnz);
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

// Leerstellenkonzentration ausgeben
int TKMCJobOut::GetTotalVacancyConc(double& Conc)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetTotalVacancyConc)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetTotalVacancyConc)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetTotalVacancyConc(Conc);
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

// Anzahl an Leerstellen ausgeben
int TKMCJobOut::GetTotalVacancyAnz(long long& Anz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetTotalVacancyAnz)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetTotalVacancyAnz)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetTotalVacancyAnz(Anz);
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

// Volumenkonzentration der beweglichen Spezies ausgeben
int TKMCJobOut::GetMovVolConc(double& MovVolConc)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetMovVolConc)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetMovVolConc)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetMovVolConc(MovVolConc);
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

// Dotierkonzentration für eine bestimmte Dotierung ausgeben
int TKMCJobOut::GetDopandConc(int ID, double& Conc)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetDopandConc)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetDopandConc)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetDopandConc(ID, Conc);
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

// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben
int TKMCJobOut::GetDopandAnz(int ID, long long& Anz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetDopandAnz)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetDopandAnz)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetDopandAnz(ID, Anz);
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

// KMC-Vorlaufparameter ausgeben
int TKMCJobOut::GetPrerunOptions(bool& DoPrerun, long long& PreMCSP, int& PreRecordAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetPrerunOptions)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetPrerunOptions)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetPrerunOptions(DoPrerun, PreMCSP, PreRecordAnz);
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

// Parameter für dyn. Normierungssimulation ausgeben
int TKMCJobOut::GetDynNormParameters(bool& DoDynNorm, long long& DynNormAttemptAnz, int& DynNormRecordAnz, int& DynNormNum)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetDynNormParameters)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetDynNormParameters)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetDynNormParameters(DoDynNorm, DynNormAttemptAnz, DynNormRecordAnz, DynNormNum);
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

// KMC-Hauptsimulationsparameter ausgeben
int TKMCJobOut::GetMainKMCOptions(long long& MCSP, int& MainRecordAnz)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetMainKMCOptions)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetMainKMCOptions)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Settings->GetMainKMCOptions(MCSP, MainRecordAnz);
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

// Beschreibung der Einstellungen in Konsole schreiben
int TKMCJobOut::WriteSettingsToConsole()
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::WriteSettingsToConsole)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Settings == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::WriteSettingsToConsole)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		if (m_Settings->IfReady() == false)
		{
			cout << "Error: Cannot print settings to console." << endl;
			cout << "       Current settings are incomplete." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}

		// Funktion aufrufen
		cout.precision(KMCOUT_TSETTINGS_COUT_PRECISION);
		return m_Settings->SaveToStream(cout, 0);
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
// -> Ergebnisse
// ******************************************** //

// Beschreibung der GetResultSummary-Ausgabe ausgeben
int TKMCJobOut::GetResultSummaryDesc(string ValDelimiter, string& SummaryDesc)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetResultSummaryDesc)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Results == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetResultSummaryDesc)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Results->GetSummaryDesc(ValDelimiter, SummaryDesc);
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

// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben
int TKMCJobOut::GetResultSummary(string ValDelimiter, string& Summary)
{
	try
	{
		if (Ready != true)
		{
			cout << "Critical Error: TKMCJob not ready (in TKMCJobOut::GetResultSummary)" << endl << endl;
			return KMCERR_READY_NOT_TRUE;
		}

		if (m_Results == NULL)
		{
			cout << "Critical Error: Invalid member pointer (in TKMCJobOut::GetResultSummary)" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}

		// Funktion aufrufen
		return m_Results->GetSummary(ValDelimiter, Summary);
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

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //