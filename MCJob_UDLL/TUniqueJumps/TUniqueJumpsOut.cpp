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

// Deklarierte Klasse:
#include "TUniqueJumps/TUniqueJumpsOut.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TUniqueJumpsOut::TUniqueJumpsOut(TKMCJob* pJob) : TUniqueJumpsFunc(pJob)
{

}

// Destructor
TUniqueJumpsOut::~TUniqueJumpsOut()
{

}

// **************************** PUBLISHED ********************************* //

// Anzahl an UJumps ausgeben
int TUniqueJumpsOut::GetUJumpCount(int& Count)
{

	Count = UJumps.size();

	return KMCERR_OK;
}

// Laenge eines Sprungs ausgeben
int TUniqueJumpsOut::GetUJumpLength(int JumpID, double& JumpLength)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpLength)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetLength(JumpLength);
}

// Aktivierungszustand eines Sprungs ausgeben
int TUniqueJumpsOut::GetUJumpActive(int JumpID, bool& State)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpActive)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetJumpActive(State);
}

// Anzahl an Umgebungsatomen in bestimmtem UJump ausgeben
int TUniqueJumpsOut::GetUJumpAtomCount(int JumpID, int& Count)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpAtomCount)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetCoordCount(Count);
}

// Element eines Umgebungsatoms eines bestimmten UJump ausgeben
int TUniqueJumpsOut::GetUJumpAtomElement(int JumpID, int CoordID, int& ElemID)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpAtomElement)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetCoordElement(CoordID, ElemID);
}

// Essentielle Informationen zu einem Umgebungsatom eines bestimmten UJump ausgeben
int TUniqueJumpsOut::GetUJumpAtomZylPosition(int JumpID, int CoordID, double& X, double& Y, double& Z)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpAtomZylPosition)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetCoordZylPosition(CoordID, X, Y, Z);
}

// Zustände (ignore, additive, active) eines Umgebungsatoms eines UJumps ausgeben
int TUniqueJumpsOut::GetUJumpAtomStates(int JumpID, int CoordID, bool& IsIgnore, bool& IsAdditive, bool& IsActive)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpAtomStates)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetCoordStates(CoordID, IsIgnore, IsAdditive, IsActive);
}

// Abstaende eines Umgebungsatoms eines bestimmten UJump ausgeben
int TUniqueJumpsOut::GetUJumpAtomDistances(int JumpID, int CoordID, double& StartDist, double& TSDist, double& DestDist)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpAtomDistances)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetCoordDistances(CoordID, StartDist, TSDist, DestDist);
}

// Anzahl an WW-Atomen (nicht-ignoriert, additiv) in bestimmtem UJump ausgeben
int TUniqueJumpsOut::GetUJumpWWAtomsCount(int JumpID, int& Count)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpWWAtomsCount)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetWWAtomsCount(Count);
}

// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
int TUniqueJumpsOut::GetUJumpWWAtomsInfo(int JumpID, int WWAtomsID, int& CoordID, int& WWID, int& EnergAnz)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpWWAtomsInfo)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetWWAtomsInfo(WWAtomsID, CoordID, WWID, EnergAnz);
}

// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben
int TUniqueJumpsOut::GetUJumpWWAtomsEnergy(int JumpID, int WWAtomsID, int ID, int& WWElemID, double& WWEnergy)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpWWAtomsEnergy)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetWWAtomsEnergy(WWAtomsID, ID, WWElemID, WWEnergy);
}

// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
int TUniqueJumpsOut::GetUJumpUCodesCount(int JumpID, int& Count)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpUCodesCount)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetUCodesCount(Count);
}

// Energie und Code für einen UniqueCode in einem UJump ausgeben
int TUniqueJumpsOut::GetUJumpUCode(int JumpID, int CodeID, string& Code, double& CodeEnergy)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpUCode)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetUCode(CodeID, Code, CodeEnergy);
}

// Anzahl an FullCodes in bestimmtem UJump ausgeben
int TUniqueJumpsOut::GetUJumpFCodesCount(int JumpID, int& Count)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpFCodesCount)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetFCodesCount(Count);
}

// UCodeID und Code für einen FullCode in einem UJump ausgeben
int TUniqueJumpsOut::GetUJumpFCode(int JumpID, int CodeID, string& Code, int& UCodeID)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetUJumpFCode)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].GetFCode(CodeID, Code, UCodeID);
}

// InteractionShellCount ausgeben
int TUniqueJumpsOut::GetInteractionShellCount(int& ShellCount)
{

	ShellCount = InteractionShellCount;

	return KMCERR_OK;
}

// Anzahl an Interactions ausgeben
int TUniqueJumpsOut::GetInteractionCount(int& Count)
{

	Count = Interactions.size();

	return KMCERR_OK;
}

// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
int TUniqueJumpsOut::GetInteractionDesc(int WWID, int& ElemID, double& StartDist, double& DestDist)
{

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int)Interactions.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetInteractionDesc)" << endl;
		cout << "                Input: " << WWID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Interactions[WWID].GetInteractionDesc(ElemID, StartDist, DestDist);
}

// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TUniqueJumpsOut::GetInteractionStartEnvCount(int WWID, int& Count)
{

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int)Interactions.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetInteractionStartEnvCount)" << endl;
		cout << "                Input: " << WWID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Interactions[WWID].GetInteractionStartEnvCount(Count);
}

// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TUniqueJumpsOut::GetInteractionDestEnvCount(int WWID, int& Count)
{

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int)Interactions.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetInteractionDestEnvCount)" << endl;
		cout << "                Input: " << WWID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Interactions[WWID].GetInteractionDestEnvCount(Count);
}

// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TUniqueJumpsOut::GetInteractionStartEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z)
{

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int)Interactions.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetInteractionStartEnvAtom)" << endl;
		cout << "                Input: " << WWID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Interactions[WWID].GetInteractionStartEnvAtom(ID, ElemID, X, Y, Z);
}

// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TUniqueJumpsOut::GetInteractionDestEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z)
{

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int)Interactions.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::GetInteractionDestEnvAtom)" << endl;
		cout << "                Input: " << WWID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Interactions[WWID].GetInteractionDestEnvAtom(ID, ElemID, X, Y, Z);
}

// Vollständige Ausgabe eines UJump
int TUniqueJumpsOut::WriteUJumpToConsole(int JumpID)
{

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int)UJumps.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::WriteUJumpToConsole)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	cout.precision(KMCOUT_TUNIQUEJUMP_COUT_PRECISION);
	return UJumps[JumpID].SaveToStream(cout, JumpID, 0);
}

// Vollständige Ausgabe einer Wechselwirkung
int TUniqueJumpsOut::WriteInteractionToConsole(int WWID)
{

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int)Interactions.size()))
	{
		cout << "Critical Error: Index exceeds range (in TUniqueJumpsOut::WriteInteractionToConsole)" << endl;
		cout << "                Input: " << WWID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	cout.precision(KMCOUT_TINTERACTION_COUT_PRECISION);
	return Interactions[WWID].SaveToStream(cout, WWID, 0);
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //