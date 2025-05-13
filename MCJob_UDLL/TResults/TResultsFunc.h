// **************************************************************** //
//																	//
//	Class: TResultsFunc	(TResults Layer 1)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the simulation results						//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TResultsFunc_H
#define TResultsFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TResults/TResultsBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TResultsFunc : public TResultsBase
{
	// Member functions
public:
	// NON-PUBLISHED
	TResultsFunc(TKMCJob* pJob);			// Constructor

protected:
	~TResultsFunc();						// Destructor
};

#endif