// **************************************************************** //
//																	//
//	Klasse: TSettings	(TSettings Layer 3)							//
//	Autor: Philipp Hein												//
//	Datum: 16.10.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Job-Spezifikationen					//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
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
class TSettings: public TSettingsOut {
	// Member functions
	public:
		// PUBLISHED

		// NON-PUBLISHED
		TSettings (TKMCJob * pJob);			// Constructor
		~TSettings ();						// Destructor
};

#endif

