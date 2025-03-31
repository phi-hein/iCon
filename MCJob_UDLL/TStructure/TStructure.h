// **************************************************************** //
//																	//
//	Klasse: TStructure	(TStructure Layer 3)						//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der eingegebenen Struktur der			//
//	  untersuchten Verbindung										//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TStructure_H
#define TStructure_H

// Includes:
#include <string>

// Eigene Includes:
#include "TStructure/TStructureOut.h"

using namespace std;

// Forward-Deklaration:
class TKMCJob;

// Klasse zur Verwaltung der Struktur der untersuchten Verbindung
class TStructure: public TStructureOut {
	// Member functions
	public:
		// PUBLISHED

		// NON-PUBLISHED
		TStructure (TKMCJob * pJob);	// Constructor
		~TStructure ();					// Destructor
};

#endif