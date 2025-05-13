// **************************************************************** //
//																	//
//	Class: TSimulation	(TSimulation Layer 3)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for carrying out the simulation							//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSimulation_H
#define TSimulation_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TSimulation/TSimulationOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulation : public TSimulationOut
{
	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	TSimulation(TKMCJob* pJob);			// Constructor
	~TSimulation();						// Destructor
};

#endif

