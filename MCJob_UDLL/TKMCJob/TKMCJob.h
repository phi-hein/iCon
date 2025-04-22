// **************************************************************** //
//																	//
//	Klasse: TKMCJob	(TKMCJob Layer 3)								//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung eines KMC-Jobs							//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TKMCJob_H
#define TKMCJob_H

// Includes:
#include <string>

// Eigene Includes:
#include "TKMCJob/TKMCJobOut.h"

using namespace std;

// Klassendeklaration:
class TKMCJob: public TKMCJobOut {
	// Member functions
	public:
		// PUBLISHED
		void Release();									// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)
		
		// NON-PUBLISHED
		TKMCJob ();					// Constructor
		virtual ~TKMCJob ();		// Destructor
};

#endif