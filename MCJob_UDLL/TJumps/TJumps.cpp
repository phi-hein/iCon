// **************************************************************** //
//																	//
//	Class: TJumps	(TJumps Layer 3)								//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for creating and describing all possible jumps	 		//
//	  for moving species and vacancies in the unit cell				//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJumps/TJumps.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJumps::TJumps(TKMCJob* pJob) : TJumpsOut(pJob)
{

}

// Destructor
TJumps::~TJumps()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //