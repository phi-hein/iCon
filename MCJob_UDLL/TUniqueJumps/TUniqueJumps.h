// **************************************************************** //
//																	//
//	Class: TUniqueJumps	(TUniqueJumps Layer 3)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the unique jumps in the material	 		//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TUniqueJumps_H
#define TUniqueJumps_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJumps/TUniqueJumpsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TUniqueJumps : public TUniqueJumpsOut
{
	// Member functions
public:
	// NON-PUBLISHED
	TUniqueJumps(TKMCJob* pJob);		// Constructor
	~TUniqueJumps();					// Destructor

};

#endif