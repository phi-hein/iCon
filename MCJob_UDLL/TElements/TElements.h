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

#ifndef TElements_H
#define TElements_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TElements/TElementsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TElements : public TElementsOut
{
	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	TElements(TKMCJob* pJob);			// Constructor
	~TElements();						// Destructor
};

#endif

