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

#ifndef TConsoleWrapper_H
#define TConsoleWrapper_H

// Includes:

// Eigene Includes:
#include "..\MCMisc_UDLL\TConsole\IConsole.h"

// Klassendeklaration:
public ref class TConsoleWrapper {
protected:
	IConsole * m_Console;

	void String_M2U(System::String ^ in_str, std::string & out_str);
	void String_U2M(std::string in_str, System::String ^ % out_str);

public:
	TConsoleWrapper();			// Constructor
	~TConsoleWrapper();		// Destructor
	!TConsoleWrapper();		// Finalizer

	int Show();										// Konsolenfenster anzeigen
	int Hide();										// Konsolenfenster nicht anzeigen
	int Status();									// Status ausgeben
	int SetBufferSize(int Rows, int Columns);		// Groesse des Konsolenpuffers veraendern
	int SetTitle(System::String ^Title);			// Konsolentitel setzen
};

#endif