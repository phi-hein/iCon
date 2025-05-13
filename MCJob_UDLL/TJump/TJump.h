// **************************************************************** //
//																	//
//	Class: TJump	(TJump Layer 3)									//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a jump of an atom in the unit cell 		//
//	  and its jump environment										//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TJump_H
#define TJump_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJump/TJumpOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJump : public TJumpOut
{
	// Member functions
public:
	// NON-PUBLISHED
	TJump(TKMCJob* pJob);		// Constructor
	~TJump();					// Destructor

};

#endif