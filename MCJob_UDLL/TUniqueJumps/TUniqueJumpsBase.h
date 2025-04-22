// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpsBase	(TUniqueJumps Layer 0)				//
//	Autor: Philipp Hein												//
//	Datum: 31.03.2013												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der einzigartigen Gitterspruenge 		//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TUniqueJumpsBase_H
#define TUniqueJumpsBase_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "TUniqueJump/TUniqueJump.h"
#include "TInteraction/TInteraction.h"

using namespace std;

// Forward-Deklaration:
class TKMCJob;

// Klassendeklaration:
class TUniqueJumpsBase {
	// Member variables
	protected:
		TKMCJob * m_Job;

		int InteractionShellCount;
		vector<TUniqueJump> UJumps;
		vector<TInteraction> Interactions;
		
		bool JumpsReady;								// Flag, ob einzigartige Gitterspruenge vollst�ndig definiert wurde
		bool CodesReady;								// Flag, ob einzigartige Gitterspruenge vollst�ndig codiert wurden

	// Member functions
	public:
		// PUBLISHED
		int MakeUniqueJumps ();													// Aus Gittersprungumgebungen von TJumps die einzigartigen Spr�nge finden
		int SetJumpActive (int JumpID, bool State);								// Sprung aktivieren (= Sprung m�glich) oder deaktivieren (= Sprung unm�glich), KMCERR_DEACTIVATION_IMPOSSIBLE zeigt, dass Sprung aktiviert bleiben muss
		int SetAtomActive (int JumpID, int CoordID, bool State);				// Umgebungsatom in einem Sprung aktivieren oder deaktivieren
		int SetAtomAdditive (int JumpID, int CoordID, bool State);				// Umgebungsatom in einem Sprung als additiv (true) oder nicht additiv (false) markieren
		int SetAtomIgnore (int JumpID, int CoordID, bool State);				// Umgebungsatom ignorieren oder nicht-ignorieren
		int ClearUniqueCodes ();															// Codierung loeschen
		int MakeUniqueCodes (int i_InteractionShellCount);									// M�gliche Umgebungsbesetzungen bestimmen und vergleichen, i_InteractionShellCount = Schalenanzahl zur Unterscheidung von Wechselwirkungen
		int SetInteractionEnergyByCoord (int JumpID, int CoordID, int ID, double Energy);	// WW-Energie [eV] f�r bestimmte CoordID (muss additiv und nicht-ignoriert sein) f�r eine bestimmte (Elem)ID setzen
		int SetInteractionEnergyByInteraction (int WWID, int ID, double Energy);			// WW-Energie [eV] f�r bestimmte InteractionID in allen UJumps f�r eine bestimmte (Elem)ID setzen
		int SetCodeEnergy (int JumpID, int UCodeID, double Energy);							// Code-Energie [eV] f�r bestimmte UniqueCodeID in bestimmtem UJump setzen

		// NON-PUBLISHED
		TUniqueJumpsBase (TKMCJob * pJob);		// Constructor, Ready = false setzen
		bool IfJumpsReady ();					// Rueckgabe von JumpsReady
		bool IfCodesReady ();					// Rueckgabe von CodesReady
		int SaveToStream (ostream &output, int offset);		// Elemente in einen Stream schreiben
		int LoadFromStream (istream &input);				// Elemente aus einem Stream laden

	protected:
		~TUniqueJumpsBase();					// Destructor
};

#endif