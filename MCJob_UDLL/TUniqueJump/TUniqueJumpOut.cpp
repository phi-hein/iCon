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

// Deklarierte Klasse:
#include "TUniqueJump/TUniqueJumpOut.h"

// Includes:
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TUniqueJumpOut::TUniqueJumpOut (TKMCJob * pJob): TUniqueJumpFunc (pJob) {
	
}

// Destructor
TUniqueJumpOut::~TUniqueJumpOut () {
	
}

// **************************** PUBLISHED ********************************* //


		
// ***************************** PUBLIC *********************************** //

// Sprunglänge zurückgeben
int TUniqueJumpOut::GetLength (double &o_Length) {

	o_Length = Length;

	return KMCERR_OK;
}

// Aktivierungszustand des Sprungs ausgeben
int TUniqueJumpOut::GetJumpActive (bool &State) {

	State = JumpActive;

	return KMCERR_OK;
}

// Anzahl an Umgebungsatomen ausgeben
int TUniqueJumpOut::GetCoordCount (int &Count) {

	Count = Coord.size();

	return KMCERR_OK;
}

// Element eines Umgebungsatoms ausgeben
int TUniqueJumpOut::GetCoordElement (int CoordID, int &o_ElemID) {

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int) ElemID.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetCoordElement)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	o_ElemID = ElemID[CoordID];

	return KMCERR_OK;
}

// Essentielle Informationen zu einem Umgebungsatom ausgeben
int TUniqueJumpOut::GetCoordZylPosition (int CoordID, double &X, double &Y, double &Z) {

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int) Coord.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetCoordZylPosition)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	X = Coord[CoordID].x;
	Y = Coord[CoordID].y;
	Z = Coord[CoordID].z;

	return KMCERR_OK;
}

// Zustände (ignore, additive, active) eines Umgebungsatoms ausgeben
int TUniqueJumpOut::GetCoordStates (int CoordID, bool &IsIgnore, bool &IsAdditive, bool &IsActive) {

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int) AtomIgnore.size()) || (CoordID >= (int) AtomAdditive.size()) || (CoordID >= (int) AtomActive.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetCoordStates)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	IsIgnore = AtomIgnore[CoordID];
	IsAdditive = AtomAdditive[CoordID];
	IsActive = AtomActive[CoordID];

	return KMCERR_OK;
}

// Abstaende eines Umgebungsatoms ausgeben
int TUniqueJumpOut::GetCoordDistances (int CoordID, double &StartDist, double &TSDist, double &DestDist) {

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int) Coord.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetCoordDistances)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	StartDist = sqrt((Coord[CoordID].x + 0.5*Length)*(Coord[CoordID].x + 0.5*Length) + Coord[CoordID].y*Coord[CoordID].y);
	TSDist = sqrt(Coord[CoordID].x*Coord[CoordID].x + Coord[CoordID].y*Coord[CoordID].y);
	DestDist = sqrt((Coord[CoordID].x - 0.5*Length)*(Coord[CoordID].x - 0.5*Length) + Coord[CoordID].y*Coord[CoordID].y);

	return KMCERR_OK;
}

// Anzahl an WW-Atomen (nicht-ignoriert, additiv) ausgeben
int TUniqueJumpOut::GetWWAtomsCount (int &Count) {

	Count = InteractionMap.size();

	return KMCERR_OK;
}

// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
int TUniqueJumpOut::GetWWAtomsInfo (int WWAtomsID, int &CoordID, int &WWID, int &EnergAnz) {

	// Input pruefen
	if ((WWAtomsID < 0) || (WWAtomsID >= (int) InteractionMap.size()) || 
		(WWAtomsID >= (int) InteractionElemIDs.size()) || 
		(WWAtomsID >= (int) InteractionEnergies.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetWWAtomsInfo)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// CoordID bestimmen
	int t_coordid = -1;
	int help_index = -1;
	if (Coord.size() != 0) {
		for (int i = 0; i < (int) Coord.size(); i++) {
			if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true)) {
				help_index++;
			}
			if (help_index == WWAtomsID) {
				t_coordid = i;
				break;
			}
		}
	}
	if (t_coordid == -1) {
		cout << "Critical Error: Cannot find atom (in TUniqueJumpOut::GetWWAtomsInfo)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	CoordID = t_coordid;
	WWID = InteractionMap[WWAtomsID];
	EnergAnz = InteractionElemIDs[WWAtomsID].size();

	return KMCERR_OK;
}

// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben
int TUniqueJumpOut::GetWWAtomsEnergy (int WWAtomsID, int ID, int &WWElemID, double &WWEnergy) {

	// Input pruefen
	if ((WWAtomsID < 0) || 
		(WWAtomsID >= (int) InteractionElemIDs.size()) || 
		(WWAtomsID >= (int) InteractionEnergies.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetWWAtomsEnergy)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((ID < 0) || (ID >= (int) InteractionElemIDs[WWAtomsID].size()) || (ID >= (int) InteractionEnergies[WWAtomsID].size())) {
		cout << "Critical Error: Invalid energy vector index (in TUniqueJumpOut::GetWWAtomsEnergy)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	WWElemID = InteractionElemIDs[WWAtomsID][ID];
	WWEnergy = InteractionEnergies[WWAtomsID][ID];

	return KMCERR_OK;
}

// Anzahl an UniqueCodes ausgeben
int TUniqueJumpOut::GetUCodesCount (int &Count) {

	Count = UniqueCodes.size();

	return KMCERR_OK;
}

// Energie und Code für einen UniqueCode ausgeben
int TUniqueJumpOut::GetUCode (int CodeID, string &Code, double &CodeEnergy) {

	// Input pruefen
	if ((CodeID < 0) || (CodeID >= (int) UniqueCodes.size()) || (CodeID >= (int) UniqueEnergies.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetUCode)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	Code = UniqueCodes[CodeID];
	CodeEnergy = UniqueEnergies[CodeID];

	return KMCERR_OK;
}

// Anzahl an FullCodes ausgeben
int TUniqueJumpOut::GetFCodesCount (int &Count) {

	Count = FullCodes.size();

	return KMCERR_OK;
}

// UCodeID und Code für einen UniqueCode ausgeben
int TUniqueJumpOut::GetFCode (int CodeID, string &Code, int &UCodeID) {

	// Input pruefen
	if ((CodeID < 0) || (CodeID >= (int) FullCodes.size()) || (CodeID >= (int) FullToUniqueMap.size())) {
		cout << "Critical Error: Invalid vector index (in TUniqueJumpOut::GetFCode)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	Code = FullCodes[CodeID];
	UCodeID = FullToUniqueMap[CodeID];

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //