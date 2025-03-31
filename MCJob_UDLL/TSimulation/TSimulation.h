// **************************************************************** //
//																	//
//	Klasse: TSimulation	(TSimulation Layer 3)						//
//	Autor: Philipp Hein												//
//	Datum: 19.03.2014												//
//  Aufgabe:														//
//    Klasse zur Durchfuehrung der Simulation						//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSimulation_H
#define TSimulation_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TSimulation/TSimulationOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulation: public TSimulationOut {
	// Member functions
	public:
		// PUBLISHED

		// NON-PUBLISHED
		TSimulation (TKMCJob * pJob);			// Constructor
		~TSimulation ();						// Destructor
};

#endif

