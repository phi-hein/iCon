// **************************************************************** //
//																	//
//	Class: TInteractionFunc	(TInteraction Layer 1)					//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for describing a pair interaction						//
//	  Layer 1: Functionality class = helper methods					//
//    -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TInteractionFunc_H
#define TInteractionFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TInteraction/TInteractionBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TInteractionFunc : public TInteractionBase
{
	// Member functions
public:
	// NON-PUBLISHED
	int GetWWDesc(int& o_ElemID, double& o_StartDist, double& o_DestDist);																								// Basis-Charakteristika der WW zurückgeben
	int GetWWShells(vector<T3DVector>* o_StartWWCoord, vector<int>* o_StartWWElemID, vector<T3DVector>* o_DestWWCoord, vector<int>* o_DestWWElemID);		// WW-Schalen zurückgeben

	TInteractionFunc(TKMCJob* pJob);			// Constructor

protected:
	~TInteractionFunc();						// Destructor

};

#endif