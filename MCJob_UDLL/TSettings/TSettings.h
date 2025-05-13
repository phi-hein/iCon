// **************************************************************** //
//																	//
//	Class: TSettings	(TSettings Layer 3)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the job settings							//
//	  Layer 3: Top class											//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSettings_H
#define TSettings_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TSettings/TSettingsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSettings : public TSettingsOut
{
	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	TSettings(TKMCJob* pJob);			// Constructor
	~TSettings();						// Destructor
};

#endif

