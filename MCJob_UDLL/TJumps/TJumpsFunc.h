// **************************************************************** //
//																	//
//	Klasse: TJumpsFunc	(TJumps Layer 1)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Erstellung und Beschreibung aller möglichen 		//
//	  Gittersprünge fuer Mov- und Vac- in der Elementarzelle		//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TJumpsFunc_H
#define TJumpsFunc_H

// Includes:
#include <string>
#include <random>

// Eigene Includes:
#include "TJumps/TJumpsBase.h"
#include "T3DVector.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;
class TSimJump;
class TSimUniqueJump;

// Klassendeklaration:
class TJumpsFunc: public TJumpsBase {
	// Member functions
	public:
		// NON-PUBLISHED
		int GetStartPos (int AtomID, int DirID, T4DLatticeVector &pos);				// absolute 4D-Position des Startatoms zurueckgeben (Vektor von (0,0,0,0) zu Startatom)
		int GetDestPos (int AtomID, int DirID, T4DLatticeVector &pos);				// absolute 4D-Position des Zielatoms zurueckgeben (Vektor von (0,0,0,0) zu Zielatom)
		int GetEnvPos (int AtomID, int DirID, vector<T4DLatticeVector> *envpos);	// absolute 4D-Positionen der Umgebungsatome zurueckgeben (Vektoren von (0,0,0,0) zu Umgebungsatomen)
		int GetUniqueJumpID (int AtomID, int DirID, int &ID);						// UniqueJumpID ausgeben
		int FindUniqueJumpID (int ID, int &AtomID, int &DirID);						// Beispielumgebung mit einer bestimmten UniqueJumpID finden
		int GetEFieldProjection (int AtomID, int DirID, T3DVector &efield, double &proj);   // Skalarprodukt [V] von E-Feld-Vektor [V/cm] und Sprungvektor [cm] ausgeben
		int GetMaxEFieldProjection (T3DVector &efield, double &max_proj);					// Betragsmaessig maximales Skalarprodukt [V] von E-Feld-Vektor [V/cm] und allen Sprungvektoren [cm] ausgeben
		int CreateJumpDirList (int &o_count, int *&o_dircount, int *&o_dirlink);			// Listen mit Richtungsanzahlen entsprechend der moeglichen Sprungrichtungen fuer jedes bewegliche Elementarzellatom erstellen
		int CreateSimJumps (int last_movstacksize, TSimJump **&o_simjumps, TSimUniqueJump *&o_simuniquejumps);		// Minimalbeschreibung aller aktiven Spruenge und einzigartigen Spruenge fuer die Simulation erstellen

		TJumpsFunc (TKMCJob * pJob);		// Constructor
		~TJumpsFunc ();						// Destructor
};

#endif