// **************************************************************** //
//																	//
//	Class: TElementsFunc	(TElements Layer 1)						//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing the used elements							//
//	  Layer 1: Functionality class = helper methods					//
//    -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TElementsFunc_H
#define TElementsFunc_H

// Includes:
#include <string>
#include <string_view>

// Eigene Includes:
#include "TElements/TElementsBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TElementsFunc : public TElementsBase
{
	// Member functions
public:
	// NON-PUBLISHED
	TElementsFunc(TKMCJob* pJob);			// Constructor

	int GetElemID(string_view i_symbol, string_view i_name, double i_charge);	// ElemID zu Symbol, Name, Charge finden
	int GetMovCharge(double& o_charge);											// Ladung der beweglichen Spezies ausgeben
	int GetElemDesc(int i_ElemID, string& o_Desc);								// Elementbeschreibung ausgeben

protected:
	~TElementsFunc();						// Destructor
};

#endif