// **************************************************************** //
//																	//
//	Class: TJumpsBase	(TJumps Layer 0)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for creating and describing all possible jumps	 		//
//	  for moving species and vacancies in the unit cell				//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TJumpsBase_H
#define TJumpsBase_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "TJump/TJump.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TJumpsBase
{
	// Member variables
protected:
	TKMCJob* m_Job;				// Pointer zur uebergeordneten KMCJob-Instanz

	int MovShellCount;				// Anzahl an Sprungziel-Schalen
	int JumpStartShellCount;		// Anzahl an Sprungumgebungs-Schalen um die Sprungstartposition
	int JumpTSShellCount;			// Anzahl an Sprungumgebungs-Schalen um das Sprungzentrum (Uebergangszustand)
	int JumpDestShellCount;			// Anzahl an Sprungumgebungs-Schalen um die Sprungzielposition
	vector<vector<TJump>> Jumps;	// Gittersprünge für [Atom][Sprungrichtung]
	bool Ready;						// Flag, ob alle Gittersprünge gefunden wurden

	// Member functions
public:
	// PUBLISHED
	int MakeJumps(int i_MovShellCount, int i_JumpStartShellCount, int i_JumpTSShellCount, int i_JumpDestShellCount);		// mit Parametern MovShellCount und Jump[..]ShellCount alle Gittersprünge und deren Umgebung finden, und Ready = true setzen

	// NON-PUBLISHED
	int SetUniqueJumps(vector<vector<int>>* IDs, vector<vector<vector<int>>>* Mappings);	// in Jumps UniqueJumpIDs setzen und EnvPos entsprechend Mappings umordnen	

	TJumpsBase(TKMCJob* pJob);			// Constructor, Ready = false setzen
	bool IfReady();						// Rueckgabe von Ready, incl. aller Jumps
	int SaveToStream(ostream& output, int offset);		// Jumps in einen Stream schreiben, nur bei Ready == true moeglich
	int LoadFromStream(istream& input);				// Jumps aus einem Stream laden, nur bei Ready == false moeglich

protected:
	~TJumpsBase();							// Destructor
};

#endif