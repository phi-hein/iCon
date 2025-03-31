// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpOut	(TUniqueJump Layer 2)					//
//	Autor: Philipp Hein												//
//	Datum: 02.04.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines einzigartigen Gittersprungs 	//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TUniqueJumpOut_H
#define TUniqueJumpOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJump/TUniqueJumpFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TUniqueJumpOut: public TUniqueJumpFunc {
	// Member functions
	public:
		// NON-PUBLISHED
		int GetLength (double &o_Length);		// Sprunglänge zurückgeben
		int GetJumpActive (bool &State);		// Aktivierungszustand des Sprungs ausgeben
		
		int GetCoordCount (int &Count);																	// Anzahl an Umgebungsatomen ausgeben
		int GetCoordElement (int CoordID, int &o_ElemID);												// Element eines Umgebungsatoms ausgeben
		int GetCoordZylPosition (int CoordID, double &X, double &Y, double &Z);							// Essentielle Informationen zu einem Umgebungsatom ausgeben
		int GetCoordStates (int CoordID, bool &IsIgnore, bool &IsAdditive, bool &IsActive);				// Zustände (ignore, additive, active) eines Umgebungsatoms ausgeben
		int GetCoordDistances (int CoordID, double &StartDist, double &TSDist, double &DestDist);		// Abstaende eines Umgebungsatoms ausgeben
		
		int GetWWAtomsCount (int &Count);															// Anzahl an WW-Atomen (nicht-ignoriert, additiv) ausgeben
		int GetWWAtomsInfo (int WWAtomsID, int &CoordID, int &WWID, int &EnergAnz);					// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
		int GetWWAtomsEnergy (int WWAtomsID, int ID, int &WWElemID, double &WWEnergy);				// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben

		int GetUCodesCount (int &Count);											// Anzahl an UniqueCodes ausgeben
		int GetUCode (int CodeID, string &Code, double &CodeEnergy);				// Energie und Code für einen UniqueCode ausgeben
		int GetFCodesCount (int &Count);											// Anzahl an FullCodes ausgeben
		int GetFCode (int CodeID, string &Code, int &UCodeID);						// UCodeID und Code für einen FullCode ausgeben

		TUniqueJumpOut (TKMCJob * pJob);		// Constructor
		~TUniqueJumpOut ();						// Destructor
};

#endif