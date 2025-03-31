// **************************************************************** //
//																	//
//	Klasse: TJumpsOut	(TJumps Layer 2)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Erstellung und Beschreibung aller möglichen 		//
//	  Gittersprünge fuer Mov- und Vac- in der Elementarzelle		//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TJumpsOut_H
#define TJumpsOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJumps/TJumpsFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJumpsOut: public TJumpsFunc {
	// Member functions
	public:
		// PUBLISHED
		int GetShellCounts (int &movcount, int &jumpstartcount, int &jumptscount, int &jumpdestcount);		// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
		int GetJumpDirCount (int atom, int &count);															// Anzahl an Sprungrichtungen fuer bestimmtes Mov-Atom in Elementarzelle ausgeben
		int GetJumpStartPos(int atom, int dir, int &x, int &y, int &z, int &s);				// 4D-Vektor des Sprungstartatoms ausgeben
		int GetJumpDestPos(int atom, int dir, int &x, int &y, int &z, int &s);				// 4D-Vektor des Sprungzielatoms ausgeben
		int GetJumpBackjumpDirID(int atom, int dir, int &dirID);							// Ruecksprung-DirID ausgeben
		int GetJumpEnvCount(int atom, int dir, int &count);									// Anzahl an Umgebungsatomen ausgeben
		int GetJumpEnvAtom(int atom, int dir, int id, int &x, int &y, int &z, int &s);		// Umgebungsatom ausgeben
		int GetJumpUniqueJumpID(int atom, int dir, int &uniqueid);							// ID des zugehörigen UniqueJump ausgeben

		// NON-PUBLISHED
		TJumpsOut (TKMCJob * pJob);		// Constructor
		~TJumpsOut ();					// Destructor
};

#endif