// **************************************************************** //
//																	//
//	Klasse: TJumps	(TJumps Layer 3)								//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Erstellung und Beschreibung aller möglichen 		//
//	  Gittersprünge fuer Mov- und Vac- in der Elementarzelle		//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TJumps_H
#define TJumps_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJumps/TJumpsOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJumps: public TJumpsOut {
	// Member functions
	public:
		// NON-PUBLISHED
		TJumps (TKMCJob * pJob);		// Constructor
		~TJumps ();						// Destructor

};

#endif