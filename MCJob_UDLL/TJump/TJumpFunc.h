// **************************************************************** //
//																	//
//	Class: TJumpFunc	(TJump Layer 1)								//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a jump of an atom in the unit cell 		//
//	  and its jump environment										//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TJumpFunc_H
#define TJumpFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TJump/TJumpBase.h"
#include "T3DVector.h"
#include "T4DLatticeVector.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;
class TSimJump;

// Klassendeklaration:
class TJumpFunc : public TJumpBase
{
	// Member functions
public:
	// NON-PUBLISHED
	TJumpFunc(TKMCJob* pJob);			// Constructor

	int GetStartPos(T4DLatticeVector& pos);					// absolute 4D-Position des Startatoms zurueckgeben (Vektor von (0,0,0,0) zu Startatom)
	int GetDestPos(T4DLatticeVector& pos);						// absolute 4D-Position des Zielatoms zurueckgeben (Vektor von (0,0,0,0) zu Zielatom)
	int GetEnvPos(vector<T4DLatticeVector>* envpos);			// absolute 4D-Positionen der Umgebungsatome zurueckgeben (Vektoren von (0,0,0,0) zu Umgebungsatomen)
	int GetUniqueJumpID(int& ID);								// UniqueJumpID ausgeben
	int GetEFieldProjection(T3DVector& efield, double& proj);	// Skalarprodukt [V] von E-Feld-Vektor [V/cm] und Sprungvektor [cm] ausgeben
	int CreateSimJump(TSimJump& o_simjump);						// Minimalbeschreibung fuer die Simulation erstellen

protected:
	~TJumpFunc();						// Destructor
};

#endif