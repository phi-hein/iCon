// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpBase	(TUniqueJump Layer 0)					//
//	Autor: Philipp Hein												//
//	Datum: 31.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines einzigartigen Gittersprungs 	//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TUniqueJumpBase_H
#define TUniqueJumpBase_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "T3DVector.h"

using namespace std;

// Forward-Deklaration:
class TKMCJob;
class TUniqueJump;
class TInteraction;

// Klassendeklaration:
class TUniqueJumpBase {
	// Member variables
	protected:
		TKMCJob * m_Job;

		double Length;							// Sprunglaenge
		vector<T3DVector> Coord;				// Koordinaten der Umgebungsatome im Sprungkoordinatensystem (sortiert ohne Berücksichtigung von active und additive)
		vector<int> ElemID;						// Element-IDs der Umgebungsatome (undotierte Umgebung)

		bool JumpActive;						// Sprung ist aktiv, d.h. kann im Gitter stattfinden
		vector<bool> AtomIgnore;				// true = Atom wird ignoriert, d.h. ist faktisch nicht vorhanden für Wechselwirkungen und Codierung
		vector<bool> AtomActive;				// true = Atom ist aktiv, false = inaktiv, d.h. Dotierung an dieser Stelle irrelevant bzw. Vacancy an dieser Stelle irrelevant fuer Aktivierungsenergie
		vector<bool> AtomAdditive;				// true = Atom ist additiv, d.h. additiver Beitrag zu Aktivierungsenergie, false = Atom ist nicht additiv, d.h. es wird bei Codierung verwendet

		vector<int> InteractionMap;						// Kategorisierung der WW, d.h. map: additive atom-CoordID -> InteractionID in übergeordnetem TUniqueJumps
		vector<vector<int>> InteractionElemIDs;			// enthält fuer jedes additive Atom in Coord die ElemIDs fuer [0] = Normalbesetzung, [1] = 1. Dopand, usw.
		vector<vector<double>> InteractionEnergies;		// enthält fuer jedes additive Atom in Coord den Energiebeitrag zur Aktivierungsenergie für [0] = Normalbesetzung, [1] = 1. Dopand, usw.

		vector<string> UniqueCodes;				// Unterscheidbare Besetzungen der Sprungumgebung (für nicht-additive Atome)
		vector<double> UniqueEnergies;			// Energien der UniqueCodes
		vector<string> FullCodes;				// alle Besetzungsmöglichkeiten der Sprungumgebung (für nicht-additive Atome)
		vector<int> FullToUniqueMap;			// map: FullCodesID -> UniqueCodesID

		bool EnvReady;							// Flag, ob Sprungumgebung vollständig definiert wurde
		bool JumpReady;							// Flag, ob Sprung vollständig definiert wurde
		bool CodesReady;						// Flag, ob mögliche Sprungumgebungsbesetzungen vollständig definiert wurden

	// Member functions
	public:
		// NON-PUBLISHED
		int SetEnv (int AtomID, int DirID);									// Setzt Length, Coord und ElemID aus bestimmtem TJump in TJumps
		int Sort ();														// Sortieren (ElemID (groß zuerst), Abstand von Sprungmitte, x-Koordinate, y-Koordinate, z-Koordinate (klein zuerst))
		int MapToJump (TUniqueJump &toJump, vector<int> *jMap);				// Sprung auf toJump mappen
		int SetJumpActive (bool State);										// Sprung aktivieren oder deaktivieren
		int SetAtomIgnore (int CoordID, bool State);						// Umgebungsatom ignorieren oder nicht
		int SetAtomActive (int CoordID, bool State);						// Umgebungsatom aktivieren oder deaktivieren
		int SetAtomAdditive (int CoordID, bool State);						// Umgebungsatom additiv oder nicht
		int CharacterizeInteractions (int UniqueJumpID, int InteractionShellCount, vector<TInteraction> *t_Interactions);		// Wechselwirkungen charakterisieren
		void ClearInteractions ();											// Wechselwirkungsinformationen löschen (incl. Codes)
		int UpdateInteractionMaps (vector<int> *old_id_map);				// InteractionMap nach Sortierung der Interactions in TUniqueJumps aktualisieren
		int MakeUniqueCodes (int UniqueJumpID);								// Mögliche Umgebungsbesetzungen bestimmen und vergleichen (UniqueJumpID ist nur für Fehlermeldungen)
		void ClearCodes ();													// Codes löschen
		int ApplyCode (string i_code);										// ElemIDs entsprechend Code ändern
		int SetInteractionEnergyByCoord (int CoordID, int ID, double Energy);			// WW-Energie für bestimmte additive, nicht-ignorierte CoordID und eine bestimmte (Elem)ID-Besetzung setzen
		int SetInteractionEnergyByInteraction (int WWID, int ID, double Energy);		// WW-Energie für bestimmte InteractionID und eine bestimmte (Elem)ID setzen
		int SetCodeEnergy (int UCodeID, double Energy);									// Code-Energie für bestimmte UniqueCodeID setzen

		TUniqueJumpBase (TKMCJob * pJob);		// Constructor, Ready = false setzen
		~TUniqueJumpBase ();					// Destructor
		bool IfEnvReady ();						// Rueckgabe von EnvReady
		bool IfJumpReady ();					// Rueckgabe von JumpReady
		bool IfCodesReady ();					// Rueckgabe von CodesReady
		int SaveToStream (ostream &output, int JumpID, int offset);		// UniqueJump in einen Stream schreiben
		int LoadJumpFromStream (istream &input, int JumpID);			// UniqueJump - Sprunginformationen aus einem Stream laden
		int LoadCodesFromStream (istream &input);						// UniqueJump - Codierung aus einem Stream laden

	protected:
		void RecursivePermutCodeGen (string recstr, int depth, vector<string> *outstr, vector<vector<int>> *dmap);		// Liefert rekursiv alle Code-Permutationen
};

#endif