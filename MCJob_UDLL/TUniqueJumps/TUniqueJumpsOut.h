// **************************************************************** //
//																	//
//	Class: TUniqueJumpsOut	(TUniqueJumps Layer 2)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the unique jumps in the material	 		//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TUniqueJumpsOut_H
#define TUniqueJumpsOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TUniqueJumps/TUniqueJumpsFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TUniqueJumpsOut : public TUniqueJumpsFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetUJumpCount(int& Count);								// Anzahl an UJumps ausgeben
	int GetUJumpLength(int JumpID, double& JumpLength);		// Laenge eines Sprungs ausgeben
	int GetUJumpActive(int JumpID, bool& JumpState);			// Aktivierungszustand eines Sprungs ausgeben

	int GetUJumpAtomCount(int JumpID, int& Count);																	// Anzahl an Umgebungsatomen in bestimmtem UJump ausgeben
	int GetUJumpAtomElement(int JumpID, int CoordID, int& ElemID);													// Element eines Umgebungsatoms eines bestimmten UJump ausgeben
	int GetUJumpAtomZylPosition(int JumpID, int CoordID, double& X, double& Y, double& Z);							// Essentielle Informationen zu einem Umgebungsatom eines bestimmten UJump ausgeben
	int GetUJumpAtomStates(int JumpID, int CoordID, bool& IsIgnore, bool& IsAdditive, bool& IsActive);				// Zustände (ignore, additive, active) eines Umgebungsatoms eines UJumps ausgeben
	int GetUJumpAtomDistances(int JumpID, int CoordID, double& StartDist, double& TSDist, double& DestDist);		// Abstaende eines Umgebungsatoms eines bestimmten UJump ausgeben

	int GetUJumpWWAtomsCount(int JumpID, int& Count);														// Anzahl an WW-Atomen (nicht-ignoriert, additiv) in bestimmtem UJump ausgeben
	int GetUJumpWWAtomsInfo(int JumpID, int WWAtomsID, int& CoordID, int& WWID, int& EnergAnz);			// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
	int GetUJumpWWAtomsEnergy(int JumpID, int WWAtomsID, int ID, int& WWElemID, double& WWEnergy);			// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben

	int GetUJumpUCodesCount(int JumpID, int& Count);									// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
	int GetUJumpUCode(int JumpID, int CodeID, string& Code, double& CodeEnergy);		// Energie und Code für einen UniqueCode in einem UJump ausgeben
	int GetUJumpFCodesCount(int JumpID, int& Count);									// Anzahl an FullCodes in bestimmtem UJump ausgeben
	int GetUJumpFCode(int JumpID, int CodeID, string& Code, int& UCodeID);				// UCodeID und Code für einen FullCode in einem UJump ausgeben

	int GetInteractionShellCount(int& ShellCount);														// InteractionShellCount ausgeben
	int GetInteractionCount(int& Count);																// Anzahl an Interactions ausgeben
	int GetInteractionDesc(int WWID, int& ElemID, double& StartDist, double& DestDist);				// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
	int GetInteractionStartEnvCount(int WWID, int& Count);												// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvCount(int WWID, int& Count);												// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionStartEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z);	// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z);		// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben

	int WriteUJumpToConsole(int JumpID);			// Vollständige Ausgabe eines UJump
	int WriteInteractionToConsole(int WWID);		// Vollständige Ausgabe einer Wechselwirkung

	// NON-PUBLISHED
	TUniqueJumpsOut(TKMCJob* pJob);			// Constructor

protected:
	~TUniqueJumpsOut();						// Destructor
};

#endif