// **************************************************************** //
//																	//
//	Klasse: TResults	(TResults Layer 3)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Simulationsergebnisse				//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TResults_H
#define TResults_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TResults/TResultsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TResults: public TResultsOut {
	// Member functions
	public:
		// PUBLISHED

		// NON-PUBLISHED
		TResults (TKMCJob * pJob);			// Constructor
		~TResults ();						// Destructor
};

#endif

