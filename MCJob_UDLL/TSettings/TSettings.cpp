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

// Deklarierte Klasse:
#include "TSettings/TSettings.h"

// Includes:

// Eigene Klassen:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSettings::TSettings(TKMCJob* pJob) : TSettingsOut(pJob)
{

}

// Destructor
TSettings::~TSettings()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //