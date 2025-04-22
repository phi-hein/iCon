// **************************************************************** //
//																	//
//	Klasse: TSimulationOut	(TSimulation Layer 2)					//
//	Autor: Philipp Hein												//
//	Datum: 19.03.2014												//
//  Aufgabe:														//
//    Klasse zur Durchfuehrung der Simulation						//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSimulationOut_H
#define TSimulationOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TSimulation/TSimulationFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulationOut: public TSimulationFunc {
	// Member functions
	public:
		// PUBLISHED

		// NON-PUBLISHED
		TSimulationOut (TKMCJob * pJob);	// Constructor

	protected:
		~TSimulationOut ();					// Destructor
};

#endif