// **************************************************************** //
//																	//
//	Class: IKMCJob													//
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

// Deklarierte Klasse:
#include "TKMCJob/IKMCJob.h"

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

#ifndef NON_DLL_BUILD

#pragma comment (linker, "/export:CreateKMCJob=_CreateKMCJob@0")

// Exportierte Funktion zur Erstellung einer Instanz der Klasse
DECLSPEC IKMCJob* __stdcall CreateKMCJob()
{
	IKMCJob* o_Job = nullptr;
	try
	{
		o_Job = new TKMCJob;
	}
	catch (...)
	{
		delete o_Job;
		return nullptr;
	}
	return o_Job;
}

#endif