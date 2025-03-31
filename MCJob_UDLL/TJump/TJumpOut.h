// **************************************************************** //
//																	//
//	Klasse: TJumpOut	(TJump Layer 2)								//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines Gittersprungs eines Atoms in  	//
//	  der Elementarzelle und Erstellung der Sprungumgebung			//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TJumpOut_H
#define TJumpOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJump/TJumpFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJumpOut: public TJumpFunc {
	// Member functions
	public:
		// PUBLISHED
		int GetJumpStartPos(int &x, int &y, int &z, int &s);		// 4D-Vektor des Sprungstartatoms ausgeben
		int GetJumpDestPos(int &x, int &y, int &z, int &s);			// 4D-Vektor des Sprungzielatoms ausgeben
		int GetJumpBackjumpDirID(int &dirID);						// Ruecksprung-DirID ausgeben
		int GetJumpEnvCount(int &count);							// Anzahl an Umgebungsatomen ausgeben
		int GetJumpEnvAtom(int id, int &x, int &y, int &z, int &s);	// Umgebungsatom ausgeben
		int GetJumpUniqueJumpID(int &uniqueid);						// ID des zugehörigen UniqueJump ausgeben

		// NON-PUBLISHED
		TJumpOut (TKMCJob * pJob);		// Constructor
		~TJumpOut ();					// Destructor
};

#endif