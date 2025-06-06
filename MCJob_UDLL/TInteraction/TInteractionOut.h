// **************************************************************** //
//																	//
//	Class: TInteractionOut	(TInteraction Layer 2)					//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for describing a pair interaction						//
//	  Layer 2: Output class = get generated data					//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TInteractionOut_H
#define TInteractionOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TInteraction/TInteractionFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TInteractionOut : public TInteractionFunc
{
	// Member functions
public:
	// NON-PUBLISHED
	int GetInteractionDesc(int& o_ElemID, double& SDist, double& DDist);								// Grundelementbesetzung und Abst�nde ausgeben
	int GetInteractionStartEnvCount(int& Count);														// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvCount(int& Count);														// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionStartEnvAtom(int ID, int& o_ElemID, double& o_X, double& o_Y, double& o_Z);		// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvAtom(int ID, int& o_ElemID, double& o_X, double& o_Y, double& o_Z);		// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben

	TInteractionOut(TKMCJob* pJob);		// Constructor

protected:
	~TInteractionOut();					// Destructor
};

#endif