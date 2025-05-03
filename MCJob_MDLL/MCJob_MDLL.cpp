// **************************************************************** //
//																	//
//	Klasse: TMCJobWrapper											//
//	Autor: Philipp Hein												//
//	Datum: 02.09.2012												//
//  Aufgabe:														//
//    Managed C++/CLI-Assembly (Managed DLL) als Wrapper fuer die	//
//	  TKMCJob-Klasse der unmanaged DLL								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#define KMCERR_OK 0
#define KMCERR_INVALID_POINTER -6
#define KMCERR_EXCEPTION_OCCURED -15
#define KMCERRSTR_STDERR "Exception occured:"

// Includes:
#include "stdafx.h"

// Eigene Includes:

// Deklarierte Klasse:
#include "MCJob_MDLL.h"

// *********************** CONSTRUCTOR/DESTRUCTOR ************************* //

// Constructor
TMCJobWrapper::TMCJobWrapper()
{
	try
	{
		m_Job = NULL;
		m_Job = CreateKMCJob();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
	}
	if (m_Job == NULL)
	{
		throw gcnew System::ApplicationException("KMC-job creation failed (TMCJobWrapper constructor).");
	}
}

// Destructor
TMCJobWrapper::~TMCJobWrapper()
{
	try
	{
		if (m_Job != NULL) m_Job->Release();
		m_Job = NULL;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
	}
}

// Finalizer
TMCJobWrapper::!TMCJobWrapper()
{
	try
	{
		if (m_Job != NULL) m_Job->Release();
		m_Job = NULL;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
	}
}

// **************************** PUBLISHED ********************************** //

// Rückgabe, ob KMCJob erfolgreich initialisiert wurde
bool TMCJobWrapper::IfReady()
{
	try
	{
		if (m_Job == NULL) return false;

		return m_Job->IfReady();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return false;
	}
}

// Projektnamen setzen
int TMCJobWrapper::SetProjectName(System::String^ Name)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Name = "";
		String_M2U(Name, U_Name);

		return m_Job->SetProjectName(U_Name);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Usernamen eingeben
int TMCJobWrapper::SetUserName(System::String^ Name)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Name = "";
		String_M2U(Name, U_Name);

		return m_Job->SetUserName(U_Name);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Projektdatum auf aktuelles Datum setzen
int TMCJobWrapper::SetProjectDate()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetProjectDate();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Projektbeschreibung eingeben
int TMCJobWrapper::SetProjectDescription(System::String^ Description)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Description = "";
		String_M2U(Description, U_Description);

		return m_Job->SetProjectDescription(U_Description);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Projektnamen ausgeben
int TMCJobWrapper::GetProjectName(System::String^% Name)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Name = "";
		String_M2U(Name, U_Name);

		int ErrorCode = m_Job->GetProjectName(U_Name);

		String_U2M(U_Name, Name);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Usernamen ausgeben
int TMCJobWrapper::GetUserName(System::String^% Name)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Name = "";
		String_M2U(Name, U_Name);

		int ErrorCode = m_Job->GetUserName(U_Name);

		String_U2M(U_Name, Name);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Projektdatum ausgeben
int TMCJobWrapper::GetProjectDate(System::String^% CreateDate)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_CreateDate = "";
		String_M2U(CreateDate, U_CreateDate);

		int ErrorCode = m_Job->GetProjectDate(U_CreateDate);

		String_U2M(U_CreateDate, CreateDate);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Projektbeschreibung ausgeben
int TMCJobWrapper::GetProjectDescription(System::String^% Description)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Description = "";
		String_M2U(Description, U_Description);

		int ErrorCode = m_Job->GetProjectDescription(U_Description);

		String_U2M(U_Description, Description);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe des Projectstatus
int TMCJobWrapper::GetProjectState(int% State)
{
	try
	{

		int U_State = State;

		int ErrorCode = m_Job->GetProjectState(U_State);

		State = U_State;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Gesamtes Projekt in Konsole schreiben
int TMCJobWrapper::WriteToConsole()
{
	try
	{

		return m_Job->WriteToConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Job in Datei speichern
int TMCJobWrapper::SaveToFile(System::String^ Filename)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Filename = "";
		String_M2U(Filename, U_Filename);

		int ErrorCode = m_Job->SaveToFile(U_Filename);

		// Bei Fehler Datei loeschen falls vorhanden
		if (ErrorCode != KMCERR_OK) System::IO::File::Delete(Filename);

		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Job in String speichern
int TMCJobWrapper::SaveToString(System::String^% Output)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Output = "";
		String_M2U(Output, U_Output);

		int ErrorCode = m_Job->SaveToString(U_Output);

		String_U2M(U_Output, Output);

		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Job aus Datei laden
int TMCJobWrapper::LoadFromFile(System::String^ Filename)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Filename = "";
		String_M2U(Filename, U_Filename);

		return m_Job->LoadFromFile(U_Filename);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Job aus String laden
int TMCJobWrapper::LoadFromString(System::String^ Input)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Input = "";
		String_M2U(Input, U_Input);

		return m_Job->LoadFromString(U_Input);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// *********************************** //
// -> Elemente
// *********************************** //

// Eingegebene Elemente loeschen
int TMCJobWrapper::ClearElements()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearElements();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Element falls noch nicht vorhanden hinzufügen
int TMCJobWrapper::AddElement(System::String^ ElementSymbol, System::String^ ElementName, double ElementCharge)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);

		return m_Job->AddElement(U_ElementSymbol, U_ElementName, ElementCharge);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Element als bewegliche Spezies setzen
int TMCJobWrapper::SetMovingElement(System::String^ ElementSymbol, System::String^ ElementName, double ElementCharge)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);

		return m_Job->SetMovingElement(U_ElementSymbol, U_ElementName, ElementCharge);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Elemente sortieren und Elementliste schliessen
int TMCJobWrapper::MakeElements()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->MakeElements();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl der Elemente ausgeben
int TMCJobWrapper::GetElementCount(int% ElementCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_ElementCount = ElementCount;

		int ErrorCode = m_Job->GetElementCount(U_ElementCount);

		ElementCount = U_ElementCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe eines Elements
int TMCJobWrapper::GetElement(int ElementID, System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);
		double U_ElementCharge = ElementCharge;

		int ErrorCode = m_Job->GetElement(ElementID, U_ElementSymbol, U_ElementName, U_ElementCharge);

		String_U2M(U_ElementSymbol, ElementSymbol);
		String_U2M(U_ElementName, ElementName);
		ElementCharge = U_ElementCharge;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe eines Elements
int TMCJobWrapper::GetElement(int ElementID, System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge,
	double% Radius, double% ColorR, double% ColorG, double% ColorB)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);
		double U_ElementCharge = ElementCharge;
		double U_Radius = Radius;
		double U_ColorR = ColorR;
		double U_ColorG = ColorG;
		double U_ColorB = ColorB;

		int ErrorCode = m_Job->GetElement(ElementID, U_ElementSymbol, U_ElementName, U_ElementCharge, U_Radius, U_ColorR, U_ColorG, U_ColorB);

		String_U2M(U_ElementSymbol, ElementSymbol);
		String_U2M(U_ElementName, ElementName);
		ElementCharge = U_ElementCharge;
		Radius = U_Radius;
		ColorR = U_ColorR;
		ColorG = U_ColorG;
		ColorB = U_ColorB;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe des MovingElement
int TMCJobWrapper::GetMovingElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);
		double U_ElementCharge = ElementCharge;

		int ErrorCode = m_Job->GetMovingElement(U_ElementSymbol, U_ElementName, U_ElementCharge);

		String_U2M(U_ElementSymbol, ElementSymbol);
		String_U2M(U_ElementName, ElementName);
		ElementCharge = U_ElementCharge;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe eines MovingElements
int TMCJobWrapper::GetMovingElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge,
	double% Radius, double% ColorR, double% ColorG, double% ColorB)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);
		double U_ElementCharge = ElementCharge;
		double U_Radius = Radius;
		double U_ColorR = ColorR;
		double U_ColorG = ColorG;
		double U_ColorB = ColorB;

		int ErrorCode = m_Job->GetMovingElement(U_ElementSymbol, U_ElementName, U_ElementCharge, U_Radius, U_ColorR, U_ColorG, U_ColorB);

		String_U2M(U_ElementSymbol, ElementSymbol);
		String_U2M(U_ElementName, ElementName);
		ElementCharge = U_ElementCharge;
		Radius = U_Radius;
		ColorR = U_ColorR;
		ColorG = U_ColorG;
		ColorB = U_ColorB;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe des VacancyElement
int TMCJobWrapper::GetVacancyElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);
		double U_ElementCharge = ElementCharge;

		int ErrorCode = m_Job->GetVacancyElement(U_ElementSymbol, U_ElementName, U_ElementCharge);

		String_U2M(U_ElementSymbol, ElementSymbol);
		String_U2M(U_ElementName, ElementName);
		ElementCharge = U_ElementCharge;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ausgabe eines VacancyElements
int TMCJobWrapper::GetVacancyElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge,
	double% Radius, double% ColorR, double% ColorG, double% ColorB)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ElementSymbol = "";
		std::string U_ElementName = "";
		String_M2U(ElementSymbol, U_ElementSymbol);
		String_M2U(ElementName, U_ElementName);
		double U_ElementCharge = ElementCharge;
		double U_Radius = Radius;
		double U_ColorR = ColorR;
		double U_ColorG = ColorG;
		double U_ColorB = ColorB;

		int ErrorCode = m_Job->GetVacancyElement(U_ElementSymbol, U_ElementName, U_ElementCharge, U_Radius, U_ColorR, U_ColorG, U_ColorB);

		String_U2M(U_ElementSymbol, ElementSymbol);
		String_U2M(U_ElementName, ElementName);
		ElementCharge = U_ElementCharge;
		Radius = U_Radius;
		ColorR = U_ColorR;
		ColorG = U_ColorG;
		ColorB = U_ColorB;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Elementbeschreibung in Konsole schreiben
int TMCJobWrapper::WriteElementsToConsole()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteElementsToConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// *********************************** //
// -> Struktur
// *********************************** //

// Eingegebene Struktur loeschen
int TMCJobWrapper::ClearStructure()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearStructure();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Name der Struktur eingeben, max. 40 Zeichen
int TMCJobWrapper::SetStructureName(System::String^ StructureName)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_StructureName = "";
		String_M2U(StructureName, U_StructureName);

		return m_Job->SetStructureName(U_StructureName);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Basis setzen aus 6 double (Längen, Winkel)
int TMCJobWrapper::SetBasisByLatticeParam(double LengthA, double LengthB, double LengthC, double AngleA, double AngleB, double AngleC)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetBasisByLatticeParam(LengthA, LengthB, LengthC, AngleA, AngleB, AngleC);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Basis setzen aus 9 double (Vektoren)
int TMCJobWrapper::SetBasisByLatticeVectors(double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetBasisByLatticeVectors(Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
int TMCJobWrapper::AddAtom(double Xrel, double Yrel, double Zrel, System::String^ AtomSymbol, System::String^ AtomName, double AtomCharge)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_AtomSymbol = "";
		std::string U_AtomName = "";
		String_M2U(AtomSymbol, U_AtomSymbol);
		String_M2U(AtomName, U_AtomName);

		return m_Job->AddAtom(Xrel, Yrel, Zrel, U_AtomSymbol, U_AtomName, AtomCharge);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
int TMCJobWrapper::AddAtom(double Xrel, double Yrel, double Zrel, int ElemID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->AddAtom(Xrel, Yrel, Zrel, ElemID);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Dotierung hinzufügen, noch keine Konzentrationen
int TMCJobWrapper::AddDoping(System::String^ DopedSymbol, System::String^ DopedName, double DopedCharge, System::String^ DopandSymbol, System::String^ DopandName, double DopandCharge, double VacDopRatio)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_DopedSymbol = "";
		std::string U_DopedName = "";
		std::string U_DopandSymbol = "";
		std::string U_DopandName = "";
		String_M2U(DopedSymbol, U_DopedSymbol);
		String_M2U(DopedName, U_DopedName);
		String_M2U(DopandSymbol, U_DopandSymbol);
		String_M2U(DopandName, U_DopandName);

		return m_Job->AddDoping(U_DopedSymbol, U_DopedName, DopedCharge, U_DopandSymbol, U_DopandName, DopandCharge, VacDopRatio);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Dotierung hinzufügen, noch keine Konzentrationen
int TMCJobWrapper::AddDoping(int DopedID, int DopandID, double VacDopRatio)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->AddDoping(DopedID, DopandID, VacDopRatio);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Struktur sortieren und überprüfen, dann Ready = true setzen
int TMCJobWrapper::MakeStructure()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->MakeStructure();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Name der Struktur ausgeben
int TMCJobWrapper::GetStructureName(System::String^% StructureName)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_StructureName = "";
		String_M2U(StructureName, U_StructureName);

		int ErrorCode = m_Job->GetStructureName(U_StructureName);

		String_U2M(U_StructureName, StructureName);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Basis ausgeben als 6 double (Längen, Winkel)
int TMCJobWrapper::GetBasisByLatticeParam(double% LengthA, double% LengthB, double% LengthC, double% AngleA, double% AngleB, double% AngleC)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_LengthA = LengthA;
		double U_LengthB = LengthB;
		double U_LengthC = LengthC;
		double U_AngleA = AngleA;
		double U_AngleB = AngleB;
		double U_AngleC = AngleC;

		int ErrorCode = m_Job->GetBasisByLatticeParam(U_LengthA, U_LengthB, U_LengthC, U_AngleA, U_AngleB, U_AngleC);

		LengthA = U_LengthA;
		LengthB = U_LengthB;
		LengthC = U_LengthC;
		AngleA = U_AngleA;
		AngleB = U_AngleB;
		AngleC = U_AngleC;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Basis ausgeben als 9 double (Vektoren)
int TMCJobWrapper::GetBasisByLatticeVectors(double% Ax, double% Ay, double% Az, double% Bx, double% By, double% Bz, double% Cx, double% Cy, double% Cz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_Ax = Ax;
		double U_Ay = Ay;
		double U_Az = Az;
		double U_Bx = Bx;
		double U_By = By;
		double U_Bz = Bz;
		double U_Cx = Cx;
		double U_Cy = Cy;
		double U_Cz = Cz;

		int ErrorCode = m_Job->GetBasisByLatticeVectors(U_Ax, U_Ay, U_Az, U_Bx, U_By, U_Bz, U_Cx, U_Cy, U_Cz);

		Ax = U_Ax;
		Ay = U_Ay;
		Az = U_Az;
		Bx = U_Bx;
		By = U_By;
		Bz = U_Bz;
		Cx = U_Cx;
		Cy = U_Cy;
		Cz = U_Cz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Elementarzellatomen ausgeben
int TMCJobWrapper::GetAtomCount(int% AtomCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_AtomCount = AtomCount;

		int ErrorCode = m_Job->GetAtomCount(U_AtomCount);

		AtomCount = U_AtomCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl der Mov- und Vac-Atome in der Elementarzelle ausgeben
int TMCJobWrapper::GetMovCount(int% MovCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_MovCount = MovCount;

		int ErrorCode = m_Job->GetMovCount(U_MovCount);

		MovCount = U_MovCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Atom ausgeben
int TMCJobWrapper::GetAtom(int AtomID, double% Xrel, double% Yrel, double% Zrel, int% ElemID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_Xrel = Xrel;
		double U_Yrel = Yrel;
		double U_Zrel = Zrel;
		int U_ElemID = ElemID;

		int ErrorCode = m_Job->GetAtom(AtomID, U_Xrel, U_Yrel, U_Zrel, U_ElemID);

		Xrel = U_Xrel;
		Yrel = U_Yrel;
		Zrel = U_Zrel;
		ElemID = U_ElemID;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Dotierungen ausgeben
int TMCJobWrapper::GetDopingCount(int% DopingCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_DopingCount = DopingCount;

		int ErrorCode = m_Job->GetDopingCount(U_DopingCount);

		DopingCount = U_DopingCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Dotierung ausgeben
int TMCJobWrapper::GetDoping(int DopingID, int% DopedID, int% DopandID, double% VacDopRatio)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_DopedID = DopedID;
		int U_DopandID = DopandID;
		double U_VacDopRatio = VacDopRatio;

		int ErrorCode = m_Job->GetDoping(DopingID, U_DopedID, U_DopandID, U_VacDopRatio);

		DopedID = U_DopedID;
		DopandID = U_DopandID;
		VacDopRatio = U_VacDopRatio;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Strukturbeschreibung in Konsole schreiben
int TMCJobWrapper::WriteStructureToConsole()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteStructureToConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// *********************************** //
// -> Gitterspruenge
// *********************************** //

// Gitterspruenge loeschen
int TMCJobWrapper::ClearJumps()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearJumps();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// alle Gittersprünge und deren Umgebung finden
int TMCJobWrapper::MakeJumps(int MovShellCount, int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->MakeJumps(MovShellCount, JumpStartShellCount, JumpTSShellCount, JumpDestShellCount);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
int TMCJobWrapper::GetShellCounts(int% MovShellCount, int% JumpStartShellCount, int% JumpTSShellCount, int% JumpDestShellCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_MovShellCount = MovShellCount;
		int U_JumpStartShellCount = JumpStartShellCount;
		int U_JumpTSShellCount = JumpTSShellCount;
		int U_JumpDestShellCount = JumpDestShellCount;

		int ErrorCode = m_Job->GetShellCounts(U_MovShellCount, U_JumpStartShellCount, U_JumpTSShellCount, U_JumpDestShellCount);

		MovShellCount = U_MovShellCount;
		JumpStartShellCount = U_JumpStartShellCount;
		JumpTSShellCount = U_JumpTSShellCount;
		JumpDestShellCount = U_JumpDestShellCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Sprungrichtungen fuer bestimmtes Mov- bzw. Vac-Atom in Elementarzelle
int TMCJobWrapper::GetJumpDirCount(int AtomID, int% DirectionCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_DirectionCount = DirectionCount;

		int ErrorCode = m_Job->GetJumpDirCount(AtomID, U_DirectionCount);

		DirectionCount = U_DirectionCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// 4D-Vektor des Sprungstartatoms ausgeben
int TMCJobWrapper::GetJumpStartPos(int AtomID, int DirID, int% x, int% y, int% z, int% s)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_x = x;
		int U_y = y;
		int U_z = z;
		int U_s = s;

		int ErrorCode = m_Job->GetJumpStartPos(AtomID, DirID, U_x, U_y, U_z, U_s);

		x = U_x;
		y = U_y;
		z = U_z;
		s = U_s;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// 4D-Vektor des Sprungzielatoms ausgeben
int TMCJobWrapper::GetJumpDestPos(int AtomID, int DirID, int% x, int% y, int% z, int% s)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_x = x;
		int U_y = y;
		int U_z = z;
		int U_s = s;

		int ErrorCode = m_Job->GetJumpDestPos(AtomID, DirID, U_x, U_y, U_z, U_s);

		x = U_x;
		y = U_y;
		z = U_z;
		s = U_s;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Ruecksprung-DirID ausgeben
int TMCJobWrapper::GetJumpBackjumpDirID(int AtomID, int DirID, int% BackjumpDirID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_BackjumpDirID = BackjumpDirID;

		int ErrorCode = m_Job->GetJumpBackjumpDirID(AtomID, DirID, U_BackjumpDirID);

		BackjumpDirID = U_BackjumpDirID;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Umgebungsatomen ausgeben
int TMCJobWrapper::GetJumpEnvCount(int AtomID, int DirID, int% EnvAtomCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_EnvAtomCount = EnvAtomCount;

		int ErrorCode = m_Job->GetJumpEnvCount(AtomID, DirID, U_EnvAtomCount);

		EnvAtomCount = U_EnvAtomCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Umgebungsatom ausgeben
int TMCJobWrapper::GetJumpEnvAtom(int AtomID, int DirID, int EnvAtomID, int% x, int% y, int% z, int% s)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_x = x;
		int U_y = y;
		int U_z = z;
		int U_s = s;

		int ErrorCode = m_Job->GetJumpEnvAtom(AtomID, DirID, EnvAtomID, U_x, U_y, U_z, U_s);

		x = U_x;
		y = U_y;
		z = U_z;
		s = U_s;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// ID des zugehörigen UniqueJump ausgeben
int TMCJobWrapper::GetJumpUniqueJumpID(int AtomID, int DirID, int% UniqueJumpID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_UniqueJumpID = UniqueJumpID;

		int ErrorCode = m_Job->GetJumpUniqueJumpID(AtomID, DirID, U_UniqueJumpID);

		UniqueJumpID = U_UniqueJumpID;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Sprungbeschreibungen in Konsole schreiben
int TMCJobWrapper::WriteJumpsToConsole()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteJumpsToConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// *********************************** //
// -> Einzigartige Gitterspruenge
// *********************************** //

// Einzigartige Gittersprünge löschen
int TMCJobWrapper::ClearUniqueJumps()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearUniqueJumps();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Einzigartige Sprünge finden, VacShellCount = Schalenanzahl zur Unterscheidung von VV-Wechselwirkungen
int TMCJobWrapper::MakeUniqueJumps()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->MakeUniqueJumps();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Sprung aktivieren (= Sprung möglich) oder deaktivieren (= Sprung unmöglich), KMCERR_DEACTIVATION_IMPOSSIBLE zeigt, dass Sprung aktiviert bleiben muss
int TMCJobWrapper::SetJumpActive(int JumpID, bool State)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetJumpActive(JumpID, State);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Umgebungsatom in einem Sprung ignorieren oder nicht-ignorieren
int TMCJobWrapper::SetAtomIgnore(int JumpID, int CoordID, bool State)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetAtomIgnore(JumpID, CoordID, State);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Umgebungsatom in einem Sprung additiv oder nicht-additiv setzen
int TMCJobWrapper::SetAtomAdditive(int JumpID, int CoordID, bool State)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetAtomAdditive(JumpID, CoordID, State);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Umgebungsatom in einem Sprung aktivieren oder deaktivieren
int TMCJobWrapper::SetAtomActive(int JumpID, int CoordID, bool State)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetAtomActive(JumpID, CoordID, State);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Codierung loeschen
int TMCJobWrapper::ClearUniqueCodes()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearUniqueCodes();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Mögliche Umgebungsbesetzungen bestimmen und vergleichen (mehrfaches Update möglich), InteractionShellCount = Schalenanzahl zur Unterscheidung von Wechselwirkungen
int TMCJobWrapper::MakeUniqueCodes(int InteractionShellCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->MakeUniqueCodes(InteractionShellCount);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// WW-Energie [eV] für bestimmte CoordID (nicht-ignoriert, nicht-additiv) und bestimmte (Elem)ID-Besetzung setzen
int TMCJobWrapper::SetInteractionEnergyByCoord(int JumpID, int CoordID, int ID, double Energy)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetInteractionEnergyByCoord(JumpID, CoordID, ID, Energy);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// WW-Energien [eV] für bestimmte InteractionID und bestimmte (Elem)ID-Besetzung setzen
int TMCJobWrapper::SetInteractionEnergyByInteraction(int WWID, int ID, double Energy)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetInteractionEnergyByInteraction(WWID, ID, Energy);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Code-Energie [eV] für bestimmte UniqueCodeID in bestimmtem UniqueJump setzen
int TMCJobWrapper::SetCodeEnergy(int JumpID, int CodeID, double Energy)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetCodeEnergy(JumpID, CodeID, Energy);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an UJumps ausgeben
int TMCJobWrapper::GetUJumpCount(int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetUJumpCount(U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Laenge eines Sprungs ausgeben
int TMCJobWrapper::GetUJumpLength(int JumpID, double% JumpLength)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_JumpLength = JumpLength;

		int ErrorCode = m_Job->GetUJumpLength(JumpID, U_JumpLength);

		JumpLength = U_JumpLength;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Aktivierungszustand eines Sprungs ausgeben
int TMCJobWrapper::GetUJumpActive(int JumpID, bool% JumpState)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		bool U_JumpState = JumpState;

		int ErrorCode = m_Job->GetUJumpActive(JumpID, U_JumpState);

		JumpState = U_JumpState;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Umgebungsatomen in bestimmtem UJump ausgeben
int TMCJobWrapper::GetUJumpAtomCount(int JumpID, int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetUJumpAtomCount(JumpID, U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Element eines Umgebungsatoms eines bestimmten UJump ausgeben
int TMCJobWrapper::GetUJumpAtomElement(int JumpID, int CoordID, int% ElemID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_ElemID = ElemID;

		int ErrorCode = m_Job->GetUJumpAtomElement(JumpID, CoordID, U_ElemID);

		ElemID = U_ElemID;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Essentielle Informationen zu einem Umgebungsatom eines bestimmten UJump ausgeben
int TMCJobWrapper::GetUJumpAtomZylPosition(int JumpID, int CoordID, double% X, double% Y, double% Z)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_X = X;
		double U_Y = Y;
		double U_Z = Z;

		int ErrorCode = m_Job->GetUJumpAtomZylPosition(JumpID, CoordID, U_X, U_Y, U_Z);

		X = U_X;
		Y = U_Y;
		Z = U_Z;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Zustände (ignore, additive, active) eines Umgebungsatoms eines UJumps ausgeben
int TMCJobWrapper::GetUJumpAtomStates(int JumpID, int CoordID, bool% IsIgnore, bool% IsAdditive, bool% IsActive)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		bool U_IsIgnore = IsIgnore;
		bool U_IsAdditive = IsAdditive;
		bool U_IsActive = IsActive;

		int ErrorCode = m_Job->GetUJumpAtomStates(JumpID, CoordID, U_IsIgnore, U_IsAdditive, U_IsActive);

		IsIgnore = U_IsIgnore;
		IsAdditive = U_IsAdditive;
		IsActive = U_IsActive;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Abstaende eines Umgebungsatoms eines bestimmten UJump ausgeben
int TMCJobWrapper::GetUJumpAtomDistances(int JumpID, int CoordID, double% StartDist, double% TSDist, double% DestDist)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_StartDist = StartDist;
		double U_TSDist = TSDist;
		double U_DestDist = DestDist;

		int ErrorCode = m_Job->GetUJumpAtomDistances(JumpID, CoordID, U_StartDist, U_TSDist, U_DestDist);

		StartDist = U_StartDist;
		TSDist = U_TSDist;
		DestDist = U_DestDist;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an WW-Atomen (nicht-ignoriert, additiv) in bestimmtem UJump ausgeben
int TMCJobWrapper::GetUJumpWWAtomsCount(int JumpID, int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetUJumpWWAtomsCount(JumpID, U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
int TMCJobWrapper::GetUJumpWWAtomsInfo(int JumpID, int WWAtomsID, int% CoordID, int% WWID, int% EnergAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_CoordID = CoordID;
		int U_WWID = WWID;
		int U_EnergAnz = EnergAnz;

		int ErrorCode = m_Job->GetUJumpWWAtomsInfo(JumpID, WWAtomsID, U_CoordID, U_WWID, U_EnergAnz);

		CoordID = U_CoordID;
		WWID = U_WWID;
		EnergAnz = U_EnergAnz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben
int TMCJobWrapper::GetUJumpWWAtomsEnergy(int JumpID, int WWAtomsID, int ID, int% WWElemID, double% WWEnergy)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_WWElemID = WWElemID;
		double U_WWEnergy = WWEnergy;

		int ErrorCode = m_Job->GetUJumpWWAtomsEnergy(JumpID, WWAtomsID, ID, U_WWElemID, U_WWEnergy);

		WWElemID = U_WWElemID;
		WWEnergy = U_WWEnergy;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
int TMCJobWrapper::GetUJumpUCodesCount(int JumpID, int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetUJumpUCodesCount(JumpID, U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Energie und Code für einen UniqueCode in einem UJump ausgeben
int TMCJobWrapper::GetUJumpUCode(int JumpID, int CodeID, System::String^% Code, double% CodeEnergy)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Code = "";
		String_M2U(Code, U_Code);
		double U_CodeEnergy = CodeEnergy;

		int ErrorCode = m_Job->GetUJumpUCode(JumpID, CodeID, U_Code, U_CodeEnergy);

		String_U2M(U_Code, Code);
		CodeEnergy = U_CodeEnergy;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an FullCodes in bestimmtem UJump ausgeben
int TMCJobWrapper::GetUJumpFCodesCount(int JumpID, int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetUJumpFCodesCount(JumpID, U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// UCodeID und Code für einen FullCode in einem UJump ausgeben
int TMCJobWrapper::GetUJumpFCode(int JumpID, int CodeID, System::String^% Code, int% UCodeID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Code = "";
		String_M2U(Code, U_Code);
		int U_UCodeID = UCodeID;

		int ErrorCode = m_Job->GetUJumpFCode(JumpID, CodeID, U_Code, U_UCodeID);

		String_U2M(U_Code, Code);
		UCodeID = U_UCodeID;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// InteractionShellCount ausgeben
int TMCJobWrapper::GetInteractionShellCount(int% ShellCount)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_ShellCount = ShellCount;

		int ErrorCode = m_Job->GetInteractionShellCount(U_ShellCount);

		ShellCount = U_ShellCount;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Interactions ausgeben
int TMCJobWrapper::GetInteractionCount(int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetInteractionCount(U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
int TMCJobWrapper::GetInteractionDesc(int WWID, int% ElemID, double% StartDist, double% DestDist)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_ElemID = ElemID;
		double U_StartDist = StartDist;
		double U_DestDist = DestDist;

		int ErrorCode = m_Job->GetInteractionDesc(WWID, U_ElemID, U_StartDist, U_DestDist);

		ElemID = U_ElemID;
		StartDist = U_StartDist;
		DestDist = U_DestDist;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TMCJobWrapper::GetInteractionStartEnvCount(int WWID, int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetInteractionStartEnvCount(WWID, U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TMCJobWrapper::GetInteractionDestEnvCount(int WWID, int% Count)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_Count = Count;

		int ErrorCode = m_Job->GetInteractionDestEnvCount(WWID, U_Count);

		Count = U_Count;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TMCJobWrapper::GetInteractionStartEnvAtom(int WWID, int ID, int% ElemID, double% X, double% Y, double% Z)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_ElemID = ElemID;
		double U_x = X;
		double U_y = Y;
		double U_z = Z;

		int ErrorCode = m_Job->GetInteractionStartEnvAtom(WWID, ID, U_ElemID, U_x, U_y, U_z);

		ElemID = U_ElemID;
		X = U_x;
		Y = U_y;
		Z = U_z;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TMCJobWrapper::GetInteractionDestEnvAtom(int WWID, int ID, int% ElemID, double% X, double% Y, double% Z)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_ElemID = ElemID;
		double U_x = X;
		double U_y = Y;
		double U_z = Z;

		int ErrorCode = m_Job->GetInteractionDestEnvAtom(WWID, ID, U_ElemID, U_x, U_y, U_z);

		ElemID = U_ElemID;
		X = U_x;
		Y = U_y;
		Z = U_z;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Vollständige Ausgabe eines UJump
int TMCJobWrapper::WriteUJumpToConsole(int JumpID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteUJumpToConsole(JumpID);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Vollständige Ausgabe einer Wechselwirkung
int TMCJobWrapper::WriteInteractionToConsole(int WWID)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteInteractionToConsole(WWID);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Beschreibung der einzigartigen Sprünge in Konsole schreiben
int TMCJobWrapper::WriteUniqueJumpsToConsole()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteUniqueJumpsToConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// *********************************** //
// -> Einstellungen
// *********************************** //

// Einstellungen löschen
int TMCJobWrapper::ClearSettings()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearSettings();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Temperatur einstellen
int TMCJobWrapper::SetTemperature(double Temperature)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetTemperature(Temperature);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Sprungfrequenz einstellen
int TMCJobWrapper::SetAttemptFrequency(double Frequency)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetAttemptFrequency(Frequency);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Checkpoint-Flags setzen
int TMCJobWrapper::SetCheckpointFlags(bool WriteCheckpoint, bool LoadCheckpoint)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetCheckpointFlags(WriteCheckpoint, LoadCheckpoint);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// E-Feld setzen aus MaxEField = maximaler E-Feld-Beitrag zu Aktivierungsenergie [in kBT] und EFieldxyz = E-Feld-Richtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
int TMCJobWrapper::SetEField(double EFieldX, double EFieldY, double EFieldZ, double MaxEField)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetEField(EFieldX, EFieldY, EFieldZ, MaxEField);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Gittergröße einstellen
int TMCJobWrapper::SetLatticeSize(int LatticeSize)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetLatticeSize(LatticeSize);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Zusätzliche Leerstellen eingeben
int TMCJobWrapper::SetAdditionalVacAnz(long long AdditionalVacAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetAdditionalVacAnz(AdditionalVacAnz);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Dotierkonzentration für eine bestimmte Dotierung eingeben
int TMCJobWrapper::SetDopandConcentration(int DopingID, double Conc)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetDopandConcentration(DopingID, Conc);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// KMC-Vorlaufparameter einstellen
int TMCJobWrapper::SetPrerunOptions(bool DoPrerun, long long PreMCSP, int PreRecordAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetPrerunOptions(DoPrerun, PreMCSP, PreRecordAnz);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Parameter für dyn. Normierungssimulation setzen
int TMCJobWrapper::SetDynNormParameters(bool DoDynNorm, long long DynNormAttemptAnz, int DynNormRecordAnz, int DynNormNum)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetDynNormParameters(DoDynNorm, DynNormAttemptAnz, DynNormRecordAnz, DynNormNum);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// KMC-Hauptsimulationsparameter eingeben
int TMCJobWrapper::SetMainKMCOptions(long long MCSP, int MainRecordAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->SetMainKMCOptions(MCSP, MainRecordAnz);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen
int TMCJobWrapper::MakeSettings()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->MakeSettings();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Temperatur ausgeben
int TMCJobWrapper::GetTemperature(double% Temperature)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_Temperature = Temperature;

		int ErrorCode = m_Job->GetTemperature(U_Temperature);

		Temperature = U_Temperature;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Sprungfrequenz ausgeben
int TMCJobWrapper::GetAttemptFrequency(double% Frequency)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_Frequency = Frequency;

		int ErrorCode = m_Job->GetAttemptFrequency(U_Frequency);

		Frequency = U_Frequency;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Checkpoint-Flags ausgeben
int TMCJobWrapper::GetCheckpointFlags(bool% WriteCheckpoint, bool% LoadCheckpoint)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		bool U_WriteCheckpoint = WriteCheckpoint;
		bool U_LoadCheckpoint = LoadCheckpoint;

		int ErrorCode = m_Job->GetCheckpointFlags(U_WriteCheckpoint, U_LoadCheckpoint);

		WriteCheckpoint = U_WriteCheckpoint;
		LoadCheckpoint = U_LoadCheckpoint;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
int TMCJobWrapper::GetEFieldSettings(double% EFieldDirX, double% EFieldDirY, double% EFieldDirZ, double% MaxEField)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_EFieldDirX = EFieldDirX;
		double U_EFieldDirY = EFieldDirY;
		double U_EFieldDirZ = EFieldDirZ;
		double U_MaxEField = MaxEField;

		int ErrorCode = m_Job->GetEFieldSettings(U_EFieldDirX, U_EFieldDirY, U_EFieldDirZ, U_MaxEField);

		EFieldDirX = U_EFieldDirX;
		EFieldDirY = U_EFieldDirY;
		EFieldDirZ = U_EFieldDirZ;
		MaxEField = U_MaxEField;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Berechnetes E-Feld ausgeben
int TMCJobWrapper::GetEField(double% EFieldX, double% EFieldY, double% EFieldZ)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_EFieldX = EFieldX;
		double U_EFieldY = EFieldY;
		double U_EFieldZ = EFieldZ;

		int ErrorCode = m_Job->GetEField(U_EFieldX, U_EFieldY, U_EFieldZ);

		EFieldX = U_EFieldX;
		EFieldY = U_EFieldY;
		EFieldZ = U_EFieldZ;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Gittergröße ausgeben
int TMCJobWrapper::GetLatticeSize(int% LatticeSize)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		int U_LatticeSize = LatticeSize;

		int ErrorCode = m_Job->GetLatticeSize(U_LatticeSize);

		LatticeSize = U_LatticeSize;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Zusätzliche Leerstellen ausgeben
int TMCJobWrapper::GetAdditionalVacAnz(long long% AdditionalVacAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		long long U_AdditionalVacAnz = AdditionalVacAnz;

		int ErrorCode = m_Job->GetAdditionalVacAnz(U_AdditionalVacAnz);

		AdditionalVacAnz = U_AdditionalVacAnz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Leerstellenkonzentration ausgeben
int TMCJobWrapper::GetTotalVacancyConc(double% Conc)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_Conc = Conc;

		int ErrorCode = m_Job->GetTotalVacancyConc(U_Conc);

		Conc = U_Conc;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl an Leerstellen ausgeben
int TMCJobWrapper::GetTotalVacancyAnz(long long% Anz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		long long U_Anz = Anz;

		int ErrorCode = m_Job->GetTotalVacancyAnz(U_Anz);

		Anz = U_Anz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Volumenkonzentration der beweglichen Spezies ausgeben
int TMCJobWrapper::GetMovVolConc(double% MovVolConc)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_MovVolConc = MovVolConc;

		int ErrorCode = m_Job->GetMovVolConc(U_MovVolConc);

		MovVolConc = U_MovVolConc;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Dotierkonzentration für eine bestimmte Dotierung ausgeben
int TMCJobWrapper::GetDopandConc(int ID, double% Conc)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		double U_Conc = Conc;

		int ErrorCode = m_Job->GetDopandConc(ID, U_Conc);

		Conc = U_Conc;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben
int TMCJobWrapper::GetDopandAnz(int ID, long long% Anz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		long long U_Anz = Anz;

		int ErrorCode = m_Job->GetDopandAnz(ID, U_Anz);

		Anz = U_Anz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// KMC-Vorlaufparameter ausgeben
int TMCJobWrapper::GetPrerunOptions(bool% DoPrerun, long long% PreMCSP, int% PreRecordAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		bool U_DoPrerun = DoPrerun;
		long long U_PreMCSP = PreMCSP;
		int U_PreRecordAnz = PreRecordAnz;

		int ErrorCode = m_Job->GetPrerunOptions(U_DoPrerun, U_PreMCSP, U_PreRecordAnz);

		DoPrerun = U_DoPrerun;
		PreMCSP = U_PreMCSP;
		PreRecordAnz = U_PreRecordAnz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Parameter für dyn. Normierungssimulation ausgeben
int TMCJobWrapper::GetDynNormParameters(bool% DoDynNorm, long long% DynNormAttemptAnz, int% DynNormRecordAnz, int% DynNormNum)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		bool U_DoDynNorm = DoDynNorm;
		long long U_DynNormAttemptAnz = DynNormAttemptAnz;
		int U_DynNormRecordAnz = DynNormRecordAnz;
		int U_DynNormNum = DynNormNum;

		int ErrorCode = m_Job->GetDynNormParameters(U_DoDynNorm, U_DynNormAttemptAnz, U_DynNormRecordAnz, U_DynNormNum);

		DoDynNorm = U_DoDynNorm;
		DynNormAttemptAnz = U_DynNormAttemptAnz;
		DynNormRecordAnz = U_DynNormRecordAnz;
		DynNormNum = U_DynNormNum;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// KMC-Hauptsimulationsparameter ausgeben
int TMCJobWrapper::GetMainKMCOptions(long long% MCSP, int% MainRecordAnz)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		long long U_MCSP = MCSP;
		int U_MainRecordAnz = MainRecordAnz;

		int ErrorCode = m_Job->GetMainKMCOptions(U_MCSP, U_MainRecordAnz);

		MCSP = U_MCSP;
		MainRecordAnz = U_MainRecordAnz;
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Beschreibung der Einstellungen in Konsole schreiben
int TMCJobWrapper::WriteSettingsToConsole()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->WriteSettingsToConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// *********************************** //
// -> Simulation
// *********************************** //

// Simulationsdaten löschen
int TMCJobWrapper::ClearSimulation()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->ClearSimulation();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Simulationsdaten initialisieren
int TMCJobWrapper::InitializeSimulation()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->InitializeSimulation();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// CheckPoint-Dateipfad setzen
int TMCJobWrapper::SetCheckPointPath(System::String^ CheckPointPath)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_CheckPointPath = "";
		String_M2U(CheckPointPath, U_CheckPointPath);

		return m_Job->SetCheckPointPath(U_CheckPointPath);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Daten aus CheckPoint-Datei laden
int TMCJobWrapper::LoadCheckPoint()
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		return m_Job->LoadCheckPoint();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Simulation durchfuehren
int TMCJobWrapper::MakeSimulation()
{
	if (m_Job == NULL) return KMCERR_INVALID_POINTER;

	return m_Job->MakeSimulation();

	// Achtung: Kein Exception-Handling auf dieser Funktion, da dies die Simulation verlangsamen wuerde
}

// *********************************** //
// -> Ergebnisse
// *********************************** //

// Beschreibung der GetResultSummary-Ausgabe ausgeben
int TMCJobWrapper::GetResultSummaryDesc(System::String^ ValDelimiter, System::String^% SummaryDesc)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ValDelimiter = "";
		std::string U_SummaryDesc = "";
		String_M2U(ValDelimiter, U_ValDelimiter);
		String_M2U(SummaryDesc, U_SummaryDesc);

		int ErrorCode = m_Job->GetResultSummaryDesc(U_ValDelimiter, U_SummaryDesc);

		String_U2M(U_SummaryDesc, SummaryDesc);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben
int TMCJobWrapper::GetResultSummary(System::String^ ValDelimiter, System::String^% Summary)
{
	try
	{
		if (m_Job == NULL) return KMCERR_INVALID_POINTER;

		std::string U_ValDelimiter = "";
		std::string U_Summary = "";
		String_M2U(ValDelimiter, U_ValDelimiter);
		String_M2U(Summary, U_Summary);

		int ErrorCode = m_Job->GetResultSummary(U_ValDelimiter, U_Summary);

		String_U2M(U_Summary, Summary);
		return ErrorCode;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED;
	}
}

// **************************** PROTECTED ********************************** //

// Allgemein zum Transfer zwischen unmanaged und managed Variablen:
// - blittable-Typen (int, double, ...) koennen ganz normal by-Value weitergereicht werden, 
//	 nur by-Reference erfordert eine Zwischenspeicherung als unmanaged-Variable
// - non-blittable-Typen (string, eigene Klassen, ...):
//		- bei by-Value-Input kopieren der managed Variablen in den unmanaged-Heap per Marshal-Funktionen noetig
//		- bei by-Value-Output (return-Value) muss eine neue managed-Variable erstellt werden
//		- bei by-Reference muss zuerst die by-Value-Input-Technik angewandt, dann die unmanaged-Variable per unmanaged-DLL
//		  veraendert und schliesslich die by-Value-Output-Technik angewandt werden

// Transfer von managed-String zu unmanaged-String
void TMCJobWrapper::String_M2U(System::String^ in_str, std::string& out_str)
{
	const char* cstr = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(in_str)).ToPointer();
	out_str = cstr;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
}

// Transfer von unmanaged-String zu managed-String
void TMCJobWrapper::String_U2M(std::string in_str, System::String^% out_str)
{
	out_str = gcnew System::String(in_str.c_str());
}

