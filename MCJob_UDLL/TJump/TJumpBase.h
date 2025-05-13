// **************************************************************** //
//																	//
//	Class: TJumpBase	(TJump Layer 0)								//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a jump of an atom in the unit cell 		//
//	  and its jump environment										//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TJumpBase_H
#define TJumpBase_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "T4DLatticeVector.h"

using namespace std;

// Forward-Deklaration:
class TKMCJob;

// Klassendeklaration:
class TJumpBase
{
	// Member variables
protected:
	TKMCJob* m_Job;

	T4DLatticeVector StartPos;				// 4D-Position des Startatoms (in Ursprungselementarzelle -> (0,0,0,s)
	T4DLatticeVector DestPos;				// 4D-Position des Zielatoms (relativ, Vektor von Startatom zu Zielatom)
	int BackjumpDirID;						// DirID (2. Index in vector<vector<TJump>>) des zugehoerigen Ruecksprungs (1. Index wird aus StartPos.s + DestPos.s erhalten) 

	vector<T4DLatticeVector> EnvPos;		// 4D-Positionen der Sprungumgebungsatome (relativ, Vektor von Startatom zu Umgebungsatom)
	int UniqueJumpID;						// Index des zugehörigen UniqueJumps
	bool Ready;								// Flag, ob Gittersprung vollständig definiert wurde

	// Member functions
public:
	// NON-PUBLISHED
	int SetStartPos(T4DLatticeVector pos);				// absolute 4D-Position des Startatoms setzen (Vektor von (0,0,0,0) zu Startatom)
	int SetDestPos(T4DLatticeVector pos);				// absolute 4D-Position des Zielatoms setzen (Vektor von (0,0,0,0) zu Zielatom)
	int SetBackjumpDirID(int dirID);					// DirID des Ruecksprungs setzen
	int MakeJump(int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount);		// Schalenanalyse zur Bestimmung der Sprungumgebungs-4D-Vektoren, Ready = true setzen
	int SetUniqueJump(int ID, vector<int>* mapping);	// UniqueJumpID setzen und EnvPos entsprechend mapping umordnen

	TJumpBase(TKMCJob* pJob);				// Constructor, Ready = false setzen
	bool IfReady();						// Rueckgabe von Ready
	int SaveToStream(ostream& output, int AtomID, int DirID, int offset);		// Elemente in einen Stream schreiben, nur bei Ready == true moeglich
	int LoadFromStream(istream& input);										// Elemente aus einem Stream laden, nur bei Ready == true moeglich

protected:
	~TJumpBase();							// Destructor
};

#endif