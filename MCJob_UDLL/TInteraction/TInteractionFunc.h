// **************************************************************** //
//																	//
//	Klasse: TInteractionFunc	(TInteraction Layer 1)				//
//	Autor: Philipp Hein												//
//	Datum: 30.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung einer Wechselwirkung		 			//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TInteractionFunc_H
#define TInteractionFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TInteraction/TInteractionBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TInteractionFunc: public TInteractionBase {
	// Member functions
	public:
		// NON-PUBLISHED
		int GetWWDesc (int &o_ElemID, double &o_StartDist, double &o_DestDist);																								// Basis-Charakteristika der WW zurückgeben
		int GetWWShells (vector<T3DVector> *o_StartWWCoord, vector<int> *o_StartWWElemID, vector<T3DVector> *o_DestWWCoord, vector<int> *o_DestWWElemID);		// WW-Schalen zurückgeben

		TInteractionFunc (TKMCJob * pJob);			// Constructor
		~TInteractionFunc ();						// Destructor

};

#endif