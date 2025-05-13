// **************************************************************** //
//																	//
//	Class: TKMCJobOut	(TKMCJob Layer 2)							//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing a KMC job									//
//	  Layer 2: Output class = get generated data					//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TKMCJobOut_H
#define TKMCJobOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TKMCJob/TKMCJobFunc.h"

using namespace std;

// Klassendeklaration:
class TKMCJobOut : public TKMCJobFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetProjectName(string& Name);					// Projektnamen ausgeben
	int GetUserName(string& Name);						// Usernamen ausgeben
	int GetProjectDate(string& CreateDate);			// Projektdatum ausgeben
	int GetProjectDescription(string& Description);	// Projektbeschreibung ausgeben
	int GetProjectState(int& State);					// Ausgabe des Projektstatus
	int WriteToConsole();								// Gesamtes Projekt in Konsole schreiben

	// -> Elemente
	int GetElementCount(int& ElementCount);															// Anzahl der Elemente ausgeben
	int GetElement(int ElementID, string& ElementSymbol, string& ElementName, double& ElementCharge);	// Ausgabe eines Elements
	int GetElement(int ElementID, string& ElementSymbol, string& ElementName, double& ElementCharge,
		double& Radius, double& ColorR, double& ColorG, double& ColorB);					// Ausgabe eines Elements
	int GetMovingElement(string& ElementSymbol, string& ElementName, double& ElementCharge);			// Ausgabe des MovingElement
	int GetMovingElement(string& ElementSymbol, string& ElementName, double& ElementCharge,
		double& Radius, double& ColorR, double& ColorG, double& ColorB);				// Ausgabe des MovingElement
	int GetVacancyElement(string& ElementSymbol, string& ElementName, double& ElementCharge);			// Ausgabe des VacancyElement
	int GetVacancyElement(string& ElementSymbol, string& ElementName, double& ElementCharge,
		double& Radius, double& ColorR, double& ColorG, double& ColorB);				// Ausgabe des VacancyElement
	int WriteElementsToConsole();																		// Elementbeschreibung in Konsole schreiben

	// -> Struktur
	int GetStructureName(string& StructureName);																									// Name der Struktur ausgeben
	int GetBasisByLatticeParam(double& LengthA, double& LengthB, double& LengthC, double& AngleA, double& AngleB, double& AngleC);					// Basis ausgeben als 6 double (Längen, Winkel)
	int GetBasisByLatticeVectors(double& Ax, double& Ay, double& Az, double& Bx, double& By, double& Bz, double& Cx, double& Cy, double& Cz);		// Basis ausgeben als 9 double (Vektoren)
	int GetAtomCount(int& AtomCount);																												// Anzahl an Elementarzellatomen ausgeben
	int GetMovCount(int& MovCount);														// Anzahl der Mov-Atome in der Elementarzelle ausgeben
	int GetAtom(int AtomID, double& Xrel, double& Yrel, double& Zrel, int& ElemID);		// Atom ausgeben
	int GetDopingCount(int& DopingCount);													// Anzahl an Dotierungen ausgeben
	int GetDoping(int DopingID, int& DopedID, int& DopandID, double& VacDopRatio);			// Dotierung ausgeben
	int WriteStructureToConsole();															// Strukturbeschreibung in Konsole schreiben

	// -> Gitterspruenge
	int GetShellCounts(int& MovShellCount, int& JumpStartShellCount, int& JumpTSShellCount, int& JumpDestShellCount);		// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
	int GetJumpDirCount(int AtomID, int& DirectionCount);											// Anzahl an Sprungrichtungen fuer bestimmtes Mov- bzw. Vac-Atom in Elementarzelle
	int GetJumpStartPos(int AtomID, int DirID, int& x, int& y, int& z, int& s);						// 4D-Vektor des Sprungstartatoms ausgeben
	int GetJumpDestPos(int AtomID, int DirID, int& x, int& y, int& z, int& s);						// 4D-Vektor des Sprungzielatoms ausgeben
	int GetJumpBackjumpDirID(int AtomID, int DirID, int& BackjumpDirID);							// Ruecksprung-DirID ausgeben
	int GetJumpEnvCount(int AtomID, int DirID, int& EnvAtomCount);									// Anzahl an Umgebungsatomen ausgeben
	int GetJumpEnvAtom(int AtomID, int DirID, int EnvAtomID, int& x, int& y, int& z, int& s);		// Umgebungsatom ausgeben
	int GetJumpUniqueJumpID(int AtomID, int DirID, int& UniqueJumpID);								// ID des zugehörigen UniqueJump ausgeben
	int WriteJumpsToConsole();																		// Sprungbeschreibungen in Konsole schreiben

	// -> Einzigartige Gitterspruenge
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
	int GetUJumpUCodesCount(int JumpID, int& Count);														// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
	int GetUJumpUCode(int JumpID, int CodeID, string& Code, double& CodeEnergy);							// Energie und Code für einen UniqueCode in einem UJump ausgeben
	int GetUJumpFCodesCount(int JumpID, int& Count);														// Anzahl an FullCodes in bestimmtem UJump ausgeben
	int GetUJumpFCode(int JumpID, int CodeID, string& Code, int& UCodeID);									// UCodeID und Code für einen FullCode in einem UJump ausgeben
	int GetInteractionShellCount(int& ShellCount);														// InteractionShellCount ausgeben
	int GetInteractionCount(int& Count);																// Anzahl an Interactions ausgeben
	int GetInteractionDesc(int WWID, int& ElemID, double& StartDist, double& DestDist);				// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
	int GetInteractionStartEnvCount(int WWID, int& Count);												// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvCount(int WWID, int& Count);												// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionStartEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z);	// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvAtom(int WWID, int ID, int& ElemID, double& X, double& Y, double& Z);		// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int WriteUJumpToConsole(int JumpID);			// Vollständige Ausgabe eines UJump
	int WriteInteractionToConsole(int WWID);		// Vollständige Ausgabe einer Wechselwirkung
	int WriteUniqueJumpsToConsole();				// Beschreibung der einzigartigen Sprünge in Konsole schreiben

	// -> Einstellungen
	int GetTemperature(double& Temperature);																	// Temperatur ausgeben
	int GetAttemptFrequency(double& Frequency);																// Sprungfrequenz ausgeben
	int GetCheckpointFlags(bool& WriteCheckpoint, bool& LoadCheckpoint);										// Checkpoint-Flags ausgeben
	int GetEFieldSettings(double& EFieldDirX, double& EFieldDirY, double& EFieldDirZ, double& MaxEField);		// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
	int GetEField(double& EFieldX, double& EFieldY, double& EFieldZ);											// Berechnetes E-Feld ausgeben
	int GetLatticeSize(int& LatticeSize);									// Gittergröße ausgeben
	int GetAdditionalVacAnz(long long& AdditionalVacAnz);					// Zusätzliche Leerstellen ausgeben
	int GetTotalVacancyConc(double& Conc);									// Leerstellenkonzentration ausgeben
	int GetTotalVacancyAnz(long long& Anz);								// Anzahl an Leerstellen ausgeben
	int GetMovVolConc(double& MovVolConc);									// Volumenkonzentration der beweglichen Spezies ausgeben
	int GetDopandConc(int ID, double& Conc);								// Dotierkonzentration für eine bestimmte Dotierung ausgeben
	int GetDopandAnz(int ID, long long& Anz);								// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben
	int GetPrerunOptions(bool& DoPrerun, long long& PreMCSP, int& PreRecordAnz);											// KMC-Vorlaufparameter ausgeben
	int GetDynNormParameters(bool& DoDynNorm, long long& DynNormAttemptAnz, int& DynNormRecordAnz, int& DynNormNum);		// Parameter für dyn. Normierungssimulation ausgeben
	int GetMainKMCOptions(long long& MCSP, int& MainRecordAnz);															// KMC-Hauptsimulationsparameter ausgeben
	int WriteSettingsToConsole();											// Beschreibung der Einstellungen in Konsole schreiben

	// -> Ergebnisse
	int GetResultSummaryDesc(string ValDelimiter, string& SummaryDesc);		// Beschreibung der GetResultSummary-Ausgabe ausgeben
	int GetResultSummary(string ValDelimiter, string& Summary);				// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben

	// NON-PUBLISHED
	TKMCJobOut();		// Constructor

protected:
	~TKMCJobOut();		// Destructor
};

#endif