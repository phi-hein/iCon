// **************************************************************** //
//																	//
//	Klasse: TResultsFunc	(TResults Layer 1)						//
//	Autor: Philipp Hein												//
//	Datum: 16.10.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Simulationsergebnisse				//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TResultsFunc_H
#define TResultsFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TResults/TResultsBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TResultsFunc: public TResultsBase {
	// Member functions
	public:
		// NON-PUBLISHED
		TResultsFunc (TKMCJob * pJob);			// Constructor
		~TResultsFunc ();						// Destructor
};

#endif