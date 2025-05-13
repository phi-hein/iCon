// **************************************************************** //
//																	//
//	Class: TJumpsOut	(TJumps Layer 2)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for creating and describing all possible jumps	 		//
//	  for moving species and vacancies in the unit cell				//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TJumpsOut : public TJumpsFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetShellCounts(int& movcount, int& jumpstartcount, int& jumptscount, int& jumpdestcount);		// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
	int GetJumpDirCount(int atom, int& count);															// Anzahl an Sprungrichtungen fuer bestimmtes Mov-Atom in Elementarzelle ausgeben
	int GetJumpStartPos(int atom, int dir, int& x, int& y, int& z, int& s);				// 4D-Vektor des Sprungstartatoms ausgeben
	int GetJumpDestPos(int atom, int dir, int& x, int& y, int& z, int& s);				// 4D-Vektor des Sprungzielatoms ausgeben
	int GetJumpBackjumpDirID(int atom, int dir, int& dirID);							// Ruecksprung-DirID ausgeben
	int GetJumpEnvCount(int atom, int dir, int& count);									// Anzahl an Umgebungsatomen ausgeben
	int GetJumpEnvAtom(int atom, int dir, int id, int& x, int& y, int& z, int& s);		// Umgebungsatom ausgeben
	int GetJumpUniqueJumpID(int atom, int dir, int& uniqueid);							// ID des zugehörigen UniqueJump ausgeben

	// NON-PUBLISHED
	TJumpsOut(TKMCJob* pJob);		// Constructor

protected:
	~TJumpsOut();					// Destructor
};

#endif