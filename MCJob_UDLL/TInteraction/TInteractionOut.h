// **************************************************************** //
//																	//
//	Klasse: TInteractionOut		(TInteraction Layer 2)				//
//	Autor: Philipp Hein												//
//	Datum: 30.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung einer Wechselwirkung		 			//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TInteractionOut_H
#define TInteractionOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TInteraction/TInteractionFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TInteractionOut: public TInteractionFunc {
	// Member functions
	public:
		// NON-PUBLISHED
		int GetInteractionDesc (int &o_ElemID, double &SDist, double &DDist);								// Grundelementbesetzung und Abstände ausgeben
		int GetInteractionStartEnvCount (int &Count);														// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
		int GetInteractionDestEnvCount (int &Count);														// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
		int GetInteractionStartEnvAtom (int ID, int &o_ElemID, double &o_X, double &o_Y, double &o_Z);		// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
		int GetInteractionDestEnvAtom (int ID, int &o_ElemID, double &o_X, double &o_Y, double &o_Z);		// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben

		TInteractionOut (TKMCJob * pJob);		// Constructor
		~TInteractionOut ();					// Destructor
};

#endif