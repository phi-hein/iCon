// **************************************************************** //
//																	//
//	Class: TJump	(TJump Layer 3)									//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a jump of an atom in the unit cell 		//
//	  and its jump environment										//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJump/TJump.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJump::TJump(TKMCJob* pJob) : TJumpOut(pJob)
{

}

// Destructor
TJump::~TJump()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //