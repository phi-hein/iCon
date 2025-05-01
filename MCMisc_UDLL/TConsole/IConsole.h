// **************************************************************** //
//																	//
//	Class: IConsole													//
//	Author: Philipp Hein											//
//  Description:													//
//    Abstract base class for separating internal classes from		//
//	  callable methods (to avoid exposing of internal classes)		//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
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

// Class declaration:
struct IConsole {
	// Declaration of published member functions as pure virtual (i.e. virtual ... = 0)
	virtual void Release () = 0;								// Destructor method against memory leaks (instance suicide)

	virtual int Show() = 0;										// Show console window
    virtual int Hide() = 0;										// Hide console window
    virtual int Status() const = 0;								// Get console status
	virtual int SetBufferSize(int rows, int columns) = 0;		// Change size of console screen buffer
	virtual int SetTitle(std::string title) = 0;				// Set console window title
};

// Exported factory function for creating a TConsole object
extern "C" DECLSPEC IConsole * __stdcall CreateConsole ();

#endif