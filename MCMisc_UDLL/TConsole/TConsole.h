// **************************************************************** //
//																	//
//	Klasse: TConsole												//
//	Autor: Philipp Hein												//
//	Datum: 15.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Console								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TConsole_H
#define TConsole_H

// Includes:
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <string>

// Eigene Includes:
#include "TConsole\IConsole.h"

using namespace std;

// Klassendeklaration:
class TConsole: public IConsole {
	// Member variables
	protected:
		bool Ready;						// Flag, ob Console erfolgreich geoeffnet wurde
		bool Visible;					// Flag, ob Console sichtbar oder versteckt ist

        int Console_Buffer_Rows;		// Anzahl der Zeilen der Console
        int Console_Buffer_Columns;		// Anzahl der Spalten (d.h. Zeichen pro Zeile) der Console
		string Console_Title;			// Titel des Konsolenfensters

        FILE *fp_out;					// CRT File Stream zu Console Out Buffer
		FILE *fp_in;					// CRT File Stream zu Console In Buffer
		FILE *fp_err;					// CRT File Stream zu Console Err Buffer
		int fd_saveout;					// CRT File Descriptor auf Original stdout
		int fd_savein;					// CRT File Descriptor auf Original stdin
		int fd_saveerr;					// CRT File Descriptor auf Original stderr
        HANDLE hStdOutHandle;			// Win32 Handle zu Console Out Buffer
		HANDLE hStdInHandle;			// Win32 Handle zu Console In Buffer
		HANDLE hStdErrHandle;			// Win32 Handle zu Console Err Buffer

	// Member functions
    public:
        TConsole();				// Constructor
        ~TConsole();			// Destructor
		void Release ();		// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)
        
        int Show();										// Konsolenfenster anzeigen
        int Hide();										// Konsolenfenster nicht anzeigen
        int Status();									// Status ausgeben
		int SetBufferSize (int Rows, int Columns);		// Groesse des Konsolenpuffers veraendern
		int SetTitle (string Title);					// Konsolentitel setzen

	protected:
		int StdExceptionHandler(exception &e);																															// Global Standard Exception Handler
		int ExceptionHandler ();																																		// Global Exception Handler for unknown Exceptions
		static void CRTInvalidParameterHandler (const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,uintptr_t pReserved);		// CRT Error Handler
};

#endif