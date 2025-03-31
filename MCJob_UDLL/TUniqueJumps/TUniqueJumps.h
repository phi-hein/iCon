// **************************************************************** //
//																	//
//	Klasse: TUniqueJumps	(TUniqueJumps Layer 3)					//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der einzigartigen Gitterspruenge 		//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TUniqueJumps_H
#define TUniqueJumps_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJumps/TUniqueJumpsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TUniqueJumps: public TUniqueJumpsOut {
	// Member functions
	public:
		// NON-PUBLISHED
		TUniqueJumps (TKMCJob * pJob);		// Constructor
		~TUniqueJumps ();					// Destructor

};

#endif