// **************************************************************** //
//																	//
//	Class: TKMCJobFunc	(TKMCJob Layer 1)							//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing a KMC job									//
//	  Layer 1: Functionality class = helper methods 				//
//    -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TKMCJobFunc_H
#define TKMCJobFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TKMCJob/TKMCJobBase.h"

using namespace std;

// Klassendeklaration:
class TKMCJobFunc : public TKMCJobBase
{
	// Member functions
public:
	// NON-PUBLISHED
	TKMCJobFunc();			// Constructor

protected:
	~TKMCJobFunc();		// Destructor
};

#endif