// **************************************************************** //
//																	//
//	Klasse: TConsoleWrapper											//
//	Autor: Philipp Hein												//
//	Datum: 15.09.2012												//
//  Aufgabe:														//
//    Managed C++/CLI-Assembly (Managed DLL) als Wrapper fuer die	//
//	  TConsole-Klasse der unmanaged DLL								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#define KMCERR_INVALID_POINTER -6
#define KMCERR_EXCEPTION_OCCURED -15
#define KMCERRSTR_STDERR "Exception occured:"

// Includes:
#include "stdafx.h"

// Eigene Includes:

// Deklarierte Klasse:
#include "TConsoleWrapper.h"

// *********************** CONSTRUCTOR/DESTRUCTOR ************************* //

// Constructor
TConsoleWrapper::TConsoleWrapper() {
	try {
		m_Console = NULL;
		m_Console = CreateConsole();
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); }
}

// Destructor
TConsoleWrapper::~TConsoleWrapper() {
	try {
		if (m_Console != NULL) m_Console->Release();
		m_Console = NULL;
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); }
}

// Finalizer
TConsoleWrapper::!TConsoleWrapper() {
	try {
		if (m_Console != NULL) m_Console->Release();
		m_Console = NULL;
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); }
}

// **************************** PUBLISHED ********************************** //

// Konsolenfenster anzeigen
int TConsoleWrapper::Show() {
	try {
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->Show();
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED; }
}

// Konsolenfenster nicht anzeigen
int TConsoleWrapper::Hide() {
	try {
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->Hide();
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED; }
}

// Status ausgeben
int TConsoleWrapper::Status() {
	try {
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->Status();
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED; }
}

// Groesse des Konsolenpuffers veraendern
int TConsoleWrapper::SetBufferSize(int Rows, int Columns) {
	try {
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->SetBufferSize(Rows, Columns);
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED; }
}

// Konsolentitel setzen
int TConsoleWrapper::SetTitle(System::String ^Title) {
	try {
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		std::string U_Title = "";
		String_M2U(Title, U_Title);

		return m_Console->SetTitle(U_Title);
	}
	catch (System::Exception ^e) { System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e); return KMCERR_EXCEPTION_OCCURED; }
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
void TConsoleWrapper::String_M2U(System::String ^ in_str, std::string & out_str) {
	const char * cstr = (const char *)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(in_str)).ToPointer();
	out_str = cstr;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
}

// Transfer von unmanaged-String zu managed-String
void TConsoleWrapper::String_U2M(std::string in_str, System::String ^ % out_str) {
	out_str = gcnew System::String(in_str.c_str());
}