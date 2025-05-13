// **************************************************************** //
//																	//
//	Class: T3DMovVector												//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for a displacement vector (incl. jump counter)			//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "T3DMovVector.h"

// Includes:

// Eigene Includes:
#include "GlobalDefinitions.h"

using namespace std;


// ******************* STATIC MEMBER FUNCTIONS *************************

// Betrag eines Vektors a
double T3DMovVector::Length(const T3DMovVector& a)
{
	return a.Length();
}