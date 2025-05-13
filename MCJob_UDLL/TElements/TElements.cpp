// **************************************************************** //
//																	//
//	Class: TElements	(TElements Layer 3)							//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing the used elements							//
//	  Layer 3: Top class											//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TElements/TElements.h"

// Includes:

// Eigene Klassen:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TElements::TElements(TKMCJob* pJob) : TElementsOut(pJob)
{

}

// Destructor
TElements::~TElements()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //