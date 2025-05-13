// **************************************************************** //
//																	//
//	Class: TStructure	(TStructure Layer 3)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the structure of the investigated material	//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TStructure/TStructure.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TStructure::TStructure(TKMCJob* pJob) : TStructureOut(pJob)
{

}

// Destructor
TStructure::~TStructure()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //
