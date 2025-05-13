// **************************************************************** //
//																	//
//	Class: TSimulationOut	(TSimulation Layer 2)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for carrying out the simulation							//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSimulationOut_H
#define TSimulationOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TSimulation/TSimulationFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulationOut : public TSimulationFunc
{
	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	TSimulationOut(TKMCJob* pJob);	// Constructor

protected:
	~TSimulationOut();					// Destructor
};

#endif