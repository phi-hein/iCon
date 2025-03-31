// **************************************************************** //
//																	//
//	Klasse: TElements	(TElements Layer 3)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der verwendeten Elemente/Spezies		//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TElements_H
#define TElements_H

// Includes:
#include <string>

// Eigene Klassen:
#include "TElements/TElementsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TElements: public TElementsOut {
	// Member functions
	public:
		// PUBLISHED

		// NON-PUBLISHED
		TElements (TKMCJob * pJob);			// Constructor
		~TElements ();						// Destructor
};

#endif

