// **************************************************************** //
//																	//
//	Class: TJumpOut	(TJump Layer 2)									//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a jump of an atom in the unit cell 		//
//	  and its jump environment										//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TJumpOut : public TJumpFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetJumpStartPos(int& x, int& y, int& z, int& s);		// 4D-Vektor des Sprungstartatoms ausgeben
	int GetJumpDestPos(int& x, int& y, int& z, int& s);			// 4D-Vektor des Sprungzielatoms ausgeben
	int GetJumpBackjumpDirID(int& dirID);						// Ruecksprung-DirID ausgeben
	int GetJumpEnvCount(int& count);							// Anzahl an Umgebungsatomen ausgeben
	int GetJumpEnvAtom(int id, int& x, int& y, int& z, int& s);	// Umgebungsatom ausgeben
	int GetJumpUniqueJumpID(int& uniqueid);						// ID des zugehörigen UniqueJump ausgeben

	// NON-PUBLISHED
	TJumpOut(TKMCJob* pJob);		// Constructor

protected:
	~TJumpOut();					// Destructor
};

#endif