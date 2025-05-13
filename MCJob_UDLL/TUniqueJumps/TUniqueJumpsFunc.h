// **************************************************************** //
//																	//
//	Class: TUniqueJumpsFunc	(TUniqueJumps Layer 1)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the unique jumps in the material	 		//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TUniqueJumpsFunc : public TUniqueJumpsBase
{
	// Member functions
public:
	// NON-PUBLISHED
	int CreateSimUniqueJumps(TSimUniqueJump*& o_simuniquejumps, vector<int>* o_newuniqueidmap);	// Minimalbeschreibung aller einzigartigen Spruenge fuer die Simulation und Map: (alte UniqueID -> ID in Array) erstellen
	int GetAddEnvIDs(int JumpID, vector<int>* o_addenvids);										// Liste ausgeben, die jede CoordID eines UniqueJump angibt, fuer die das Atom nicht-ignoriert und additiv ist (d.h. Teil der additiven Umgebung)
	int GetCodeEnvIDs(int JumpID, vector<int>* o_codeenvids);										// Liste ausgeben, die jede CoordID eines UniqueJump angibt, fuer die das Atom nicht-ignoriert und nicht-additiv ist (d.h. Teil der kodierten Umgebung)

	TUniqueJumpsFunc(TKMCJob* pJob);			// Constructor

protected:
	~TUniqueJumpsFunc();						// Destructor

};

#endif