// **************************************************************** //
//																	//
//	Class: TJumps	(TJumps Layer 3)								//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for creating and describing all possible jumps	 		//
//	  for moving species and vacancies in the unit cell				//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TJumps_H
#define TJumps_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJumps/TJumpsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJumps : public TJumpsOut
{
	// Member functions
public:
	// NON-PUBLISHED
	TJumps(TKMCJob* pJob);		// Constructor
	~TJumps();						// Destructor

};

#endif