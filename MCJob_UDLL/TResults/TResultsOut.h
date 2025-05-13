// **************************************************************** //
//																	//
//	Class: TResultsOut		(TResults Layer 2)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the simulation results						//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TResultsOut : public TResultsFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetSummaryDesc(string i_ValDelimiter, string& o_SummaryDesc);	// Beschreibung der GetSummary-Ausgabe ausgeben
	int GetSummary(string i_ValDelimiter, string& o_Summary);			// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben

	// NON-PUBLISHED
	TResultsOut(TKMCJob* pJob);		// Constructor

protected:
	~TResultsOut();					// Destructor
};

#endif