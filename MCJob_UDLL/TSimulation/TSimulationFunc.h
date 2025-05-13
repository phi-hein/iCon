// **************************************************************** //
//																	//
//	Class: TSimulationFunc	(TSimulation Layer 1)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for carrying out the simulation							//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSimulationFunc_H
#define TSimulationFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TSimulation/TSimulationBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulationFunc : public TSimulationBase
{
	// Member functions
public:
	// NON-PUBLISHED
	TSimulationFunc(TKMCJob* pJob);			// Constructor

	int GetMovementResults(vector<double>* o_results) const;	// Ergebnisse der Hauptsimulation ausgeben
	int GetMainSimStatus(TSimPhaseInfo& o_phase) const;			// Status der Hauptsimulation ausgeben

protected:
	~TSimulationFunc();						// Destructor
};

#endif