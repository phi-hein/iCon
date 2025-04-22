// **************************************************************** //
//																	//
//	Klasse: TJumpFunc	(TJump Layer 1)								//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines Gittersprungs eines Atoms in  	//
//	  der Elementarzelle und Erstellung der Sprungumgebung			//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TJumpFunc_H
#define TJumpFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJump/TJumpBase.h"
#include "T3DVector.h"
#include "T4DLatticeVector.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;
class TSimJump;

// Klassendeklaration:
class TJumpFunc: public TJumpBase {
	// Member functions
	public:
		// NON-PUBLISHED
		TJumpFunc (TKMCJob * pJob);			// Constructor

		int GetStartPos (T4DLatticeVector &pos);					// absolute 4D-Position des Startatoms zurueckgeben (Vektor von (0,0,0,0) zu Startatom)
		int GetDestPos (T4DLatticeVector &pos);						// absolute 4D-Position des Zielatoms zurueckgeben (Vektor von (0,0,0,0) zu Zielatom)
		int GetEnvPos (vector<T4DLatticeVector> *envpos);			// absolute 4D-Positionen der Umgebungsatome zurueckgeben (Vektoren von (0,0,0,0) zu Umgebungsatomen)
		int GetUniqueJumpID (int &ID);								// UniqueJumpID ausgeben
		int GetEFieldProjection (T3DVector &efield, double &proj);	// Skalarprodukt [V] von E-Feld-Vektor [V/cm] und Sprungvektor [cm] ausgeben
		int CreateSimJump(TSimJump &o_simjump);						// Minimalbeschreibung fuer die Simulation erstellen

	protected:
		~TJumpFunc();						// Destructor
};

#endif