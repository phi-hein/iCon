// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpsFunc	(TUniqueJumps Layer 1)				//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der einzigartigen Gitterspruenge 		//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TUniqueJumpsFunc_H
#define TUniqueJumpsFunc_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "TUniqueJumps/TUniqueJumpsBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;
class TSimUniqueJump;

// Klassendeklaration:
class TUniqueJumpsFunc: public TUniqueJumpsBase {
	// Member functions
	public:
		// NON-PUBLISHED
		int CreateSimUniqueJumps (TSimUniqueJump *&o_simuniquejumps, vector<int> *o_newuniqueidmap);	// Minimalbeschreibung aller einzigartigen Spruenge fuer die Simulation und Map: (alte UniqueID -> ID in Array) erstellen
		int GetAddEnvIDs (int JumpID, vector<int> *o_addenvids);										// Liste ausgeben, die jede CoordID eines UniqueJump angibt, fuer die das Atom nicht-ignoriert und additiv ist (d.h. Teil der additiven Umgebung)
		int GetCodeEnvIDs (int JumpID, vector<int> *o_codeenvids);										// Liste ausgeben, die jede CoordID eines UniqueJump angibt, fuer die das Atom nicht-ignoriert und nicht-additiv ist (d.h. Teil der kodierten Umgebung)

		TUniqueJumpsFunc (TKMCJob * pJob);			// Constructor

	protected:
		~TUniqueJumpsFunc ();						// Destructor

};

#endif