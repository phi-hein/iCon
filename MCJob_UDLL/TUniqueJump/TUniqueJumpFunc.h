// **************************************************************** //
//																	//
//	Class: TUniqueJumpFunc	(TUniqueJump Layer 1)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a unique jump 							//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TUniqueJumpFunc_H
#define TUniqueJumpFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJump/TUniqueJumpBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;
class TSimUniqueJump;

// Klassendeklaration:
class TUniqueJumpFunc : public TUniqueJumpBase
{
	// Member functions
public:
	// NON-PUBLISHED
	int GetCoords(vector<T3DVector>* o_Coord);						// Coord ausgeben
	int GetElemIDs(vector<int>* o_ElemID);							// ElemID ausgeben
	int CreateSimUniqueJump(TSimUniqueJump& o_simuniquejump);		// Minimalbeschreibung fuer die Simulation erstellen
	int GetAddEnvIDs(vector<int>* o_addenvids);					// Liste ausgeben, die jede CoordID angibt, fuer die das Atom nicht-ignoriert und additiv ist (d.h. Teil der additiven Umgebung)
	int GetCodeEnvIDs(vector<int>* o_codeenvids);					// Liste ausgeben, die jede CoordID angibt, fuer die das Atom nicht-ignoriert und nicht-additiv ist (d.h. Teil der kodierten Umgebung)

	TUniqueJumpFunc(TKMCJob* pJob);			// Constructor

protected:
	~TUniqueJumpFunc();						// Destructor
};

#endif