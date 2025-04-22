// **************************************************************** //
//																	//
//	Klasse: TInteractionBase	(TInteraction Layer 0)				//
//	Autor: Philipp Hein												//
//	Datum: 30.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung einer Wechselwirkung		 			//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TInteractionBase_H
#define TInteractionBase_H

// Includes:
#include <vector>
#include <string>

// Eigene Includes:
#include "T3DVector.h"
#include "T4DLatticeVector.h"

using namespace std;

// Forward-Deklaration:
class TKMCJob;
class TInteraction;

// Klassendeklaration:
class TInteractionBase {
	// Member variables
	protected:
		TKMCJob * m_Job;

		int ElemID;							// ElemID des mit der springenden Leerstelle wechselwirkenden Atoms
		double StartDist;					// Abstand des wechselwirkenden Atoms zum Sprungstartatom
		double DestDist;					// Abstand des wechselwirkenden Atoms zum Sprungzielatom
		vector<T3DVector> StartWWCoord;		// Sprungkoordinaten der WW-Umgebung bzgl. Sprungstartatom
		vector<int> StartWWElemID;			// ElemIDs der WW-Umgebung bzgl. Sprungstartatom
		vector<T3DVector> DestWWCoord;		// Sprungkoordinaten der WW-Umgebung bzgl. Sprungzielatom
		vector<int> DestWWElemID;			// ElemIDs der WW-Umgebung bzgl. Sprungzielatom

		bool Ready;							// Flag, ob Interaction vollständig definiert wurde

	// Member functions
	public:
		// NON-PUBLISHED
		int MakeInteraction (T4DLatticeVector StartPos, T4DLatticeVector WWPos, T4DLatticeVector DestPos, int i_InteractionShellCount);		// Interaction erstellen aus drei absoluten 4D-Koordinaten
		int Compare (TInteraction &toInteraction);		// Interaction mit anderer Interaction vergleichen

		TInteractionBase (TKMCJob * pJob);				// Constructor, Ready = false setzen
		bool IfReady ();								// Rueckgabe von Ready
		int SaveToStream (ostream &output, int InteractionID, int offset);		// Interaction in einen Stream schreiben, nur bei Ready == true moeglich
		int LoadFromStream (istream &input);									// Interaction aus einem Stream laden, nur bei Ready == false moeglich

	protected:
		~TInteractionBase();							// Destructor
};

#endif