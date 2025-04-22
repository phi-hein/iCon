// **************************************************************** //
//																	//
//	Klasse: TJumpsBase	(TJumps Layer 0)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Erstellung und Beschreibung aller möglichen 		//
//	  Gittersprünge fuer Mov- und Vac- in der Elementarzelle		//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
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
class TJumpsBase {
	// Member variables
	protected:
		TKMCJob * m_Job;				// Pointer zur uebergeordneten KMCJob-Instanz

		int MovShellCount;				// Anzahl an Sprungziel-Schalen
		int JumpStartShellCount;		// Anzahl an Sprungumgebungs-Schalen um die Sprungstartposition
		int JumpTSShellCount;			// Anzahl an Sprungumgebungs-Schalen um das Sprungzentrum (Uebergangszustand)
		int JumpDestShellCount;			// Anzahl an Sprungumgebungs-Schalen um die Sprungzielposition
		vector<vector<TJump>> Jumps;	// Gittersprünge für [Atom][Sprungrichtung]
		bool Ready;						// Flag, ob alle Gittersprünge gefunden wurden

	// Member functions
	public:
		// PUBLISHED
		int MakeJumps (int i_MovShellCount, int i_JumpStartShellCount, int i_JumpTSShellCount, int i_JumpDestShellCount);		// mit Parametern MovShellCount und Jump[..]ShellCount alle Gittersprünge und deren Umgebung finden, und Ready = true setzen
		
		// NON-PUBLISHED
		int SetUniqueJumps (vector<vector<int>> *IDs, vector<vector<vector<int>>> *Mappings);	// in Jumps UniqueJumpIDs setzen und EnvPos entsprechend Mappings umordnen	

		TJumpsBase (TKMCJob * pJob);			// Constructor, Ready = false setzen
		bool IfReady ();						// Rueckgabe von Ready, incl. aller Jumps
		int SaveToStream (ostream &output, int offset);		// Jumps in einen Stream schreiben, nur bei Ready == true moeglich
		int LoadFromStream (istream &input);				// Jumps aus einem Stream laden, nur bei Ready == false moeglich

	protected:
		~TJumpsBase();							// Destructor
};

#endif