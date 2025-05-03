// **************************************************************** //
//																	//
//	Klasse: IKMCJob													//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Abstract base class zur Abstraktion der internen Klassen von	//
//	  den aufrufbaren Methoden (sodass kein Export aller internen	//
//	  Klassen noetig)												//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
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
DECLSPEC IKMCJob * __stdcall CreateKMCJob () 
{
	IKMCJob* o_Job = NULL;
	try
	{
		o_Job = new TKMCJob;
	}
	catch (...)
	{
		return NULL;
	}
	return o_Job;
}

#endif