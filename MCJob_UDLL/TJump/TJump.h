// **************************************************************** //
//																	//
//	Klasse: TJump	(TJump Layer 3)									//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines Gittersprungs eines Atoms in  	//
//	  der Elementarzelle und Erstellung der Sprungumgebung			//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TJump_H
#define TJump_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJump/TJumpOut.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJump: public TJumpOut {
	// Member functions
	public:
		// NON-PUBLISHED
		TJump (TKMCJob * pJob);		// Constructor
		~TJump ();					// Destructor

};

#endif