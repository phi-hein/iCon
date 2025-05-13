// **************************************************************** //
//																	//
//	Class: TInteraction	(TInteraction Layer 3)						//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for describing a pair interaction						//
//	  Layer 3: Top class											//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TInteraction_H
#define TInteraction_H

// Includes:
#include <string>

// Eigene Includes:
#include "TInteraction/TInteractionOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TInteraction : public TInteractionOut
{
	// Member functions
public:
	// NON-PUBLISHED
	TInteraction(TKMCJob* pJob);		// Constructor
	~TInteraction();					// Destructor

};

#endif