// **************************************************************** //
//																	//
//	Class: TResults	(TResults Layer 3)								//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the simulation results						//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TResults/TResults.h"

// Includes:

// Eigene Klassen:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TResults::TResults(TKMCJob* pJob) : TResultsOut(pJob)
{

}

// Destructor
TResults::~TResults()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //