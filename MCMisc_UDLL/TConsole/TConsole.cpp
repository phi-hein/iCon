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

// Deklarierte Klasse:
#include "TConsole\TConsole.h"

// Includes:
#include <iostream>
#include <fcntl.h>
#include <io.h>

// Eigene Includes:
#include "..\MCJob_UDLL\ErrorCodes.h"
#include "..\MCJob_UDLL\GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TConsole::TConsole () {try {
	// Member Variablen initialisieren
    hStdOutHandle = INVALID_HANDLE_VALUE;
	hStdInHandle = INVALID_HANDLE_VALUE;
	hStdErrHandle = INVALID_HANDLE_VALUE;
	Console_Buffer_Rows = 500;
    Console_Buffer_Columns = 150;
	Console_Title = "Status Window";
	Ready = false;
	Visible = false;

	// Error Handler fuer CRT-Funktionen setzen
	_set_invalid_parameter_handler(CRTInvalidParameterHandler);

	// Standard File Descriptors speichern
	fflush(stdout);
	fflush(stdin);
	fflush(stderr);
	fd_saveout = _dup(_fileno(stdout));
	fd_savein = _dup(_fileno(stdin));
	fd_saveerr = _dup(_fileno(stderr));
        
	// Console oeffnen
    if (AllocConsole() == 0) return;

	// X-Button und "Schliessen" aus Popup-Menu loeschen
	HWND hWnd = GetConsoleWindow();
	if (hWnd == NULL) return;
	HMENU hMenu = GetSystemMenu(hWnd,false);
	EnableMenuItem(hMenu,SC_CLOSE,MF_BYCOMMAND | MF_GRAYED);
	RemoveMenu(hMenu,SC_CLOSE,MF_BYCOMMAND);

	// STRG+C verhindern
	SetConsoleCtrlHandler(NULL,true);

	// Groesse des Console Screen Buffers setzen
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&consoleinfo) == 0) return;
	consoleinfo.dwSize.X = Console_Buffer_Columns;
    consoleinfo.dwSize.Y = Console_Buffer_Rows;
    if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),consoleinfo.dwSize) == 0) return;
        
	// Titel setzen
    if (SetConsoleTitleA(Console_Title.c_str()) == 0) return;
		
    // Unbuffered STDOUT zur neuen Console umleiten
    hStdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);							// Liefert Win32-Handle zu Console Buffer zurueck
	if ((hStdOutHandle == INVALID_HANDLE_VALUE) || (hStdOutHandle == NULL)) return;
	int hConOutHandle = _open_osfhandle((intptr_t) hStdOutHandle,_O_TEXT);		// Oeffnet Console Buffer als C Run-Time "Text" File und liefert einen File Descriptor auf diesen File zurueck
	if (hConOutHandle == -1) return;
	fp_out = _fdopen(hConOutHandle,"w");										// Haengt einen low-level IO-Stream fuer "w" = Schreibzugriff bzw. "r" = Lesezugriff an den geoeffneten File, der durch den File Descriptor beschrieben wird
	if (fp_out == NULL) return;
	*stdout = *fp_out;															// Setzt den low-level Standard-Output/Input/Error-Stream auf den erstellten Stream, der zum entsprechenden Console Buffer fuehrt
	if (setvbuf(stdout,NULL,_IONBF,0) != 0) return;								// Setzt den low level Standard-IO-Stream auf "_IONBF" = unbuffered, d.h. IO wird so schnell wie moeglich weitergeleitet
		
    // Unbuffered STDIN zur neuen Console umleiten
    hStdInHandle = GetStdHandle(STD_INPUT_HANDLE);
	if ((hStdInHandle == INVALID_HANDLE_VALUE) || (hStdInHandle == NULL)) return;
    int hConInHandle = _open_osfhandle((intptr_t) hStdInHandle,_O_TEXT);
	if (hConInHandle == -1) return;
    fp_in = _fdopen(hConInHandle,"r");
	if (fp_in == NULL) return;
    *stdin = *fp_in;
    if (setvbuf(stdin,NULL,_IONBF,0) != 0) return;

    // Unbuffered STDERR zur neuen Console umleiten
    hStdErrHandle = GetStdHandle(STD_ERROR_HANDLE);
	if ((hStdErrHandle == INVALID_HANDLE_VALUE) || (hStdErrHandle == NULL)) return;
    int hConErrHandle = _open_osfhandle((intptr_t) hStdErrHandle,_O_TEXT);
	if (hConErrHandle == -1) return;
    fp_err = _fdopen(hConErrHandle,"w");
	if (fp_err == NULL) return;
    *stderr = *fp_err;
    if (setvbuf(stderr,NULL,_IONBF,0) != 0) return;
		
    // cout, wcout, cin, wcin, wcerr, cerr, wclog und clog mit den neu gesetzten STDOUT, STDIN und STDERR synchronisieren
    ios::sync_with_stdio();

	Ready = true;
	Visible = true;
} catch (exception &e) {Ready = false; StdExceptionHandler(e);} catch (...) {Ready = false; ExceptionHandler();}
}

// Destructor
TConsole::~TConsole () {try {
	// Console schliessen
    FreeConsole();
		
	// Standard File Descriptors wiederherstellen
	fflush(stdout);
	fflush(stdin);
	fflush(stderr);

	if (fd_saveout != -1) _dup2(fd_saveout,_fileno(stdout));
	if (fd_savein != -1) _dup2(fd_savein,_fileno(stdin));
	if (fd_saveerr != -1) _dup2(fd_saveerr,_fileno(stderr));
		
	if (fd_saveout != -1) _close(fd_saveout);
	if (fd_savein != -1) _close(fd_savein);
	if (fd_saveerr != -1) _close(fd_saveerr);
		
	// Console Buffer Files schliessen
    fclose(fp_out);
	fclose(fp_in);
	fclose(fp_err);
		
	hStdOutHandle = INVALID_HANDLE_VALUE;
	hStdInHandle = INVALID_HANDLE_VALUE;
	hStdErrHandle = INVALID_HANDLE_VALUE;
} catch (exception &e) {Ready = false; StdExceptionHandler(e);} catch (...) {Ready = false; ExceptionHandler();}
}

// **************************** PUBLISHED ********************************* //

// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)
void TConsole::Release() {try {
	delete this;
} catch (exception &e) {StdExceptionHandler(e);} catch (...) {ExceptionHandler();}
}

// Konsolenfenster anzeigen
int TConsole::Show() {try {
    if (Ready != true) return KMCERR_READY_NOT_TRUE;
	
	HWND hWnd = GetConsoleWindow();
	if (hWnd == NULL) {
		Ready = false;
		return KMCERR_NO_CONSOLE;
	}
	ShowWindow(hWnd,SW_SHOW);

	Visible = true;
	return KMCERR_OK;
} catch (exception &e) {return StdExceptionHandler(e);} catch (...) {return ExceptionHandler();}
}

// Konsolenfenster nicht anzeigen
int TConsole::Hide() {try {
    if (Ready != true) return KMCERR_READY_NOT_TRUE;

	HWND hWnd = GetConsoleWindow();
	if (hWnd == NULL) {
		Ready = false;
		return KMCERR_NO_CONSOLE;
	}
	ShowWindow(hWnd,SW_HIDE);

	Visible = false;
	return KMCERR_OK;
} catch (exception &e) {return StdExceptionHandler(e);} catch (...) {return ExceptionHandler();}
}

// Status ausgeben
int TConsole::Status() {try {		
	if (Ready != true) return KMCERR_READY_NOT_TRUE;		// d.h. Ready == false
	if (Visible == false) return KMCERR_NO_CONSOLE;			// d.h. Ready == true && Visible == false
	return KMCERR_OK;										// d.h. Ready == true && Visible == true
} catch (exception &e) {return StdExceptionHandler(e);} catch (...) {return ExceptionHandler();}
}

// Groesse des Konsolenpuffers veraendern
int TConsole::SetBufferSize (int Rows, int Columns) {try {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	// Groesse des Console Screen Buffers setzen
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&consoleinfo) == 0) {
		Ready = false;
		return KMCERR_NO_CONSOLE;
	}
	consoleinfo.dwSize.X = Columns;
    consoleinfo.dwSize.Y = Rows;
    if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),consoleinfo.dwSize) == 0) {
		consoleinfo.dwSize.X = Console_Buffer_Columns;
		consoleinfo.dwSize.Y = Console_Buffer_Rows;
		if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),consoleinfo.dwSize) == 0) {
			Ready = false;
			return KMCERR_NO_CONSOLE;
		}
		return KMCERR_INVALID_INPUT;
	}
    Console_Buffer_Columns = Columns;
	Console_Buffer_Rows = Rows;

	return KMCERR_OK;
} catch (exception &e) {return StdExceptionHandler(e);} catch (...) {return ExceptionHandler();}
}

// Konsolentitel setzen
int TConsole::SetTitle (string Title) {try {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	// Titel setzen
    if (SetConsoleTitleA(Title.c_str()) == 0) {
		if (SetConsoleTitleA(Console_Title.c_str()) == 0) return KMCERR_NO_CONSOLE;
		return KMCERR_INVALID_INPUT;
	}
	Console_Title = Title;

	return KMCERR_OK;
} catch (exception &e) {return StdExceptionHandler(e);} catch (...) {return ExceptionHandler();}
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //

// CRT Error Handler
void TConsole::CRTInvalidParameterHandler (const wchar_t * expression,const wchar_t * function,const wchar_t * file,unsigned int line,uintptr_t pReserved) {
	// nix
}

// Global Standard Exception Handler
int TConsole::StdExceptionHandler (exception &e) {
	cerr << KMCERRSTR_STDERR << " " << e.what() << endl;
	return KMCERR_EXCEPTION_OCCURED;
}

// Global Exception Handler for unknown Exceptions
int TConsole::ExceptionHandler () {
	cerr << KMCERRSTR_UNKNOWNERR << endl;
	return KMCERR_EXCEPTION_OCCURED;
}