// **************************************************************** //
//																	//
//	Klasse: TResultsOut		(TResults Layer 2)						//
//	Autor: Philipp Hein												//
//	Datum: 16.10.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Simulationsergebnisse				//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TResultsOut_H
#define TResultsOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TResults/TResultsFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TResultsOut: public TResultsFunc {
	// Member functions
	public:
		// PUBLISHED
		int GetSummaryDesc(string i_ValDelimiter, string &o_SummaryDesc);	// Beschreibung der GetSummary-Ausgabe ausgeben
		int GetSummary(string i_ValDelimiter, string &o_Summary);			// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben
		
		// NON-PUBLISHED
		TResultsOut (TKMCJob * pJob);		// Constructor
		~TResultsOut ();					// Destructor
};

#endif