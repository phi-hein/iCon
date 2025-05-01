// **************************************************************** //
//																	//
//	Class: TConsoleWrapper											//
//	Author: Philipp Hein											//
//  Description:													//
//    Managed C++/CLI assembly (managed DLL) as wrapper for the		//
//	  TConsole class of the unmanaged DLL							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TConsoleWrapper_H
#define TConsoleWrapper_H

// Includes:

// Own includes:
#include "..\MCMisc_UDLL\TConsole\IConsole.h"

// Class declaration:
public ref class TConsoleWrapper 
{
protected:
	IConsole * m_Console;

	void String_M2U(System::String ^ in_str, std::string & out_str);
	void String_U2M(std::string in_str, System::String ^ % out_str);

public:
	TConsoleWrapper();		// Constructor
	~TConsoleWrapper();		// Destructor
	!TConsoleWrapper();		// Finalizer

	int Show();										// Show console window
	int Hide();										// Hide console window
	int Status();									// Get console status
	int SetBufferSize(int rows, int columns);		// Change size of console screen buffer
	int SetTitle(System::String ^title);			// Set console window title
};

#endif