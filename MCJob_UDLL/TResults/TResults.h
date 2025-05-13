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

#ifndef TResults_H
#define TResults_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TResults/TResultsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TResults : public TResultsOut
{
	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	TResults(TKMCJob* pJob);			// Constructor
	~TResults();						// Destructor
};

#endif

