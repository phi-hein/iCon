// **************************************************************** //
//																	//
//	Klasse: TSimulationFunc	(TSimulation Layer 1)					//
//	Autor: Philipp Hein												//
//	Datum: 19.03.2014												//
//  Aufgabe:														//
//    Klasse zur Durchfuehrung der Simulation						//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSimulationFunc_H
#define TSimulationFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TSimulation/TSimulationBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulationFunc: public TSimulationBase {
	// Member functions
	public:
		// NON-PUBLISHED
		TSimulationFunc (TKMCJob * pJob);			// Constructor

		int GetMovementResults(vector<double> *o_results) const;	// Ergebnisse der Hauptsimulation ausgeben
		int GetMainSimStatus(TSimPhaseInfo &o_phase) const;			// Status der Hauptsimulation ausgeben

	protected:
		~TSimulationFunc();						// Destructor
};

#endif