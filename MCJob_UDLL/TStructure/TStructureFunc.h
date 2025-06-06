// **************************************************************** //
//																	//
//	Class: TStructureFunc	(TStructure Layer 1)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the structure of the investigated material	//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TStructureFunc_H
#define TStructureFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TStructure/TStructureBase.h"
#include "T4DLatticeVector.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TStructureFunc : public TStructureBase
{
	// Member functions
public:
	// NON-PUBLISHED
	int GetMovAtomAnz();									// Anzahl der beweglichen Atome in der Elementarzelle ausgeben (d.h. Mov)
	int GetElemID(int s);									// ElemID[s] zurueckgeben
	int GetElemIDs(vector<int>* o_elemids);				// ElemIDs ausgeben
	int GetDopingIDs(vector<int>* o_dopedids, vector<int>* o_dopandids);	// DopedIDs und DopandIDs ausgeben
	T3DVector Get3DVector(int x, int y, int z, int s);		// 3D-Vektor zu einem 4D-LatticeVector als ints ausgeben
	T3DVector Get3DVector(const T4DLatticeVector& vec);	// 3D-Vektor zu einem 4D-LatticeVector ausgeben
	int NNAnalysis(const T3DVector& i_center, int shellanz, vector<int>* elem_ids, vector<T4DLatticeVector>* no_atoms,
		vector<T4DLatticeVector>* out_atoms);				// Nearest-Neighbor-Schalen-Analyse um einen Raumpunkt, wobei no_atoms nicht beruecksichtigt werden, elem_ids = erlaubte ElemIDs (keine = alle), out_atoms = absolute Umgebungsvektoren
	int GetDopands4ElemID(int ElemID, bool IfActive, vector<int>* ElemOut);	// Dotierungsmoeglichkeiten fuer eine bestimmte elem_id ermitteln
	int GetUCVolume(double& volume);											// Volumen [Angstrom^3] der Elementarzelle ausgeben
	int GetVacDopingDesc(string& o_Desc);					// Beschreibung der Leerstellen-"Dotierung" im Format: Vac <- Mov ausgeben
	int GetDopingDesc(int i_DopID, string& o_Desc);			// Beschreibung der Dotierung im Format: Dopand <- Doped ausgeben

	TStructureFunc(TKMCJob* pJob);		// Constructor

protected:
	~TStructureFunc();						// Destructor
};

#endif