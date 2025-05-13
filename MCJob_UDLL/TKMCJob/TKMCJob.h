// **************************************************************** //
//																	//
//	Class: TKMCJob	(TKMCJob Layer 3)								//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing a KMC job									//
//	  Layer 3: Top class											//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TKMCJob_H
#define TKMCJob_H

// Includes:
#include <string>

// Eigene Includes:
#include "TKMCJob/TKMCJobOut.h"

using namespace std;

// Klassendeklaration:
class TKMCJob : public TKMCJobOut
{
	// Member functions
public:
	// PUBLISHED
	void Release();									// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)

	// NON-PUBLISHED
	TKMCJob();					// Constructor
	virtual ~TKMCJob();		// Destructor
};

#endif