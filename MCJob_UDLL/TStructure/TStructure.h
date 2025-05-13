// **************************************************************** //
//																	//
//	Class: TStructure	(TStructure Layer 3)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the structure of the investigated material	//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TStructure_H
#define TStructure_H

// Includes:
#include <string>

// Eigene Includes:
#include "TStructure/TStructureOut.h"

using namespace std;

// Forward-Deklaration:
class TKMCJob;

// Klasse zur Verwaltung der Struktur der untersuchten Verbindung
class TStructure : public TStructureOut
{
	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	TStructure(TKMCJob* pJob);	// Constructor
	~TStructure();					// Destructor
};

#endif