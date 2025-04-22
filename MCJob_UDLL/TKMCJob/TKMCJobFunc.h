// **************************************************************** //
//																	//
//	Klasse: TKMCJobFunc	(TKMCJob Layer 1)							//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung eines KMC-Jobs							//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
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
class TKMCJobFunc: public TKMCJobBase {
	// Member functions
	public:
		// NON-PUBLISHED
		TKMCJobFunc ();			// Constructor

	protected:
		~TKMCJobFunc ();		// Destructor
};

#endif