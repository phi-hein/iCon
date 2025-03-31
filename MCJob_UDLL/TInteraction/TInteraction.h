// **************************************************************** //
//																	//
//	Klasse: TInteraction	(TInteraction Layer 3)					//
//	Autor: Philipp Hein												//
//	Datum: 30.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung einer Wechselwirkung		 			//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TInteraction_H
#define TInteraction_H

// Includes:
#include <string>

// Eigene Includes:
#include "TInteraction/TInteractionOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TInteraction: public TInteractionOut {
	// Member functions
	public:
		// NON-PUBLISHED
		TInteraction (TKMCJob * pJob);		// Constructor
		~TInteraction ();					// Destructor

};

#endif