// **************************************************************** //
//																	//
//	Klasse: IConsole												//
//	Autor: Philipp Hein												//
//	Datum: 15.09.2012												//
//  Aufgabe:														//
//    Abstract base class zur Abstraktion der internen Klassen von	//
//	  den aufrufbaren Methoden (sodass kein Export aller internen	//
//	  Klassen noetig)												//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifdef MCMISC_UDLL_EXPORTS 
#define DECLSPEC __declspec(dllexport) 
#else 
#define DECLSPEC __declspec(dllimport) 
#endif

#ifndef IConsole_H
#define IConsole_H

// Includes:
#include <string>

using namespace std;

// Klassendeklaration:
struct IConsole {
	// Deklaration der published-Member-Funktionen als pure virtual (d.h. virtual ... = 0)
	virtual void Release () = 0;								// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)

	virtual int Show() = 0;										// Konsolenfenster anzeigen
    virtual int Hide() = 0;										// Konsolenfenster nicht anzeigen
    virtual int Status() = 0;									// Status ausgeben
	virtual int SetBufferSize (int Rows, int Columns) = 0;		// Groesse des Konsolenpuffers veraendern
	virtual int SetTitle (std::string Title) = 0;				// Konsolentitel setzen
};

// Exportierte Funktion zur Erstellung einer Instanz der Klasse
extern "C" DECLSPEC IConsole * __stdcall CreateConsole ();

#endif