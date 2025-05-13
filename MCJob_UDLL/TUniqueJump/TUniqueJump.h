// **************************************************************** //
//																	//
//	Class: TUniqueJump	(TUniqueJump Layer 3)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a unique jump 							//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TUniqueJump_H
#define TUniqueJump_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJump/TUniqueJumpOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TUniqueJump : public TUniqueJumpOut
{
	// Member functions
public:
	// NON-PUBLISHED
	TUniqueJump(TKMCJob* pJob);		// Constructor
	~TUniqueJump();					// Destructor

};

#endif