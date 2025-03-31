// **************************************************************** //
//																	//
//	Klasse: TUniqueJump	(TUniqueJump Layer 3)						//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines einzigartigen Gittersprungs 	//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TUniqueJump_H
#define TUniqueJump_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJump/TUniqueJumpOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TUniqueJump: public TUniqueJumpOut {
	// Member functions
	public:
		// NON-PUBLISHED
		TUniqueJump (TKMCJob * pJob);		// Constructor
		~TUniqueJump ();					// Destructor

};

#endif