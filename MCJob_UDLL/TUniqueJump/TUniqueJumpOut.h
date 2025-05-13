// **************************************************************** //
//																	//
//	Class: TUniqueJumpOut	(TUniqueJump Layer 2)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a unique jump 							//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TUniqueJumpOut : public TUniqueJumpFunc
{
	// Member functions
public:
	// NON-PUBLISHED
	int GetLength(double& o_Length);		// Sprunglänge zurückgeben
	int GetJumpActive(bool& State);		// Aktivierungszustand des Sprungs ausgeben

	int GetCoordCount(int& Count);																	// Anzahl an Umgebungsatomen ausgeben
	int GetCoordElement(int CoordID, int& o_ElemID);												// Element eines Umgebungsatoms ausgeben
	int GetCoordZylPosition(int CoordID, double& X, double& Y, double& Z);							// Essentielle Informationen zu einem Umgebungsatom ausgeben
	int GetCoordStates(int CoordID, bool& IsIgnore, bool& IsAdditive, bool& IsActive);				// Zustände (ignore, additive, active) eines Umgebungsatoms ausgeben
	int GetCoordDistances(int CoordID, double& StartDist, double& TSDist, double& DestDist);		// Abstaende eines Umgebungsatoms ausgeben

	int GetWWAtomsCount(int& Count);															// Anzahl an WW-Atomen (nicht-ignoriert, additiv) ausgeben
	int GetWWAtomsInfo(int WWAtomsID, int& CoordID, int& WWID, int& EnergAnz);					// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
	int GetWWAtomsEnergy(int WWAtomsID, int ID, int& WWElemID, double& WWEnergy);				// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben

	int GetUCodesCount(int& Count);											// Anzahl an UniqueCodes ausgeben
	int GetUCode(int CodeID, string& Code, double& CodeEnergy);				// Energie und Code für einen UniqueCode ausgeben
	int GetFCodesCount(int& Count);											// Anzahl an FullCodes ausgeben
	int GetFCode(int CodeID, string& Code, int& UCodeID);						// UCodeID und Code für einen FullCode ausgeben

	TUniqueJumpOut(TKMCJob* pJob);		// Constructor

protected:
	~TUniqueJumpOut();						// Destructor
};

#endif