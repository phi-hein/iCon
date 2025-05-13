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

// Declared class:
#include "TConsole\IConsole.h"

// Own includes:
#include "TConsole\TConsole.h"

#pragma comment (linker, "/export:CreateConsole=_CreateConsole@0")

// Exported factory function for creating a TConsole object
DECLSPEC IConsole* __stdcall CreateConsole()
{
	IConsole* o_Console = nullptr;
	try
	{
		o_Console = new TConsole;
	}
	catch (...)
	{
		delete o_Console;
		return nullptr;
	}
	return o_Console;
}