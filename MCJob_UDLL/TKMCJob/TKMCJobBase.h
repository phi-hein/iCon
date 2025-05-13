// **************************************************************** //
//																	//
//	Class: TKMCJobBase	(TKMCJob Layer 0)							//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing a KMC job									//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TKMCJobBase_H
#define TKMCJobBase_H

// Includes:
#include <string>
#include <exception>

// Eigene Includes:
#include "TKMCJob/IKMCJob.h"

using namespace std;

// Forward-Deklarationen:
class TElementsDesc;
class TElements;
class TStructure;
class TJumps;
class TUniqueJumps;
class TSettings;
class TSimulation;
class TResults;

// Klassendeklaration:
class TKMCJobBase : public IKMCJob
{
	// Member variables
protected:
	// Allgemeine Informationen
	string ProjectName;			// Name des KMC-Projekts
	string UserName;			// Name des Users, der das Projekt erstellt hat
	int ProjectDateYear;		// Datum der Projekterstellung -> Jahr
	int ProjectDateMonth;		// Datum der Projekterstellung -> Monat: 1-12
	int ProjectDateDay;			// Datum der Projekterstellung -> Tag: 1-31
	string ProjectDescription;	// Projektbeschreibung

	bool Ready;									// Flag, ob KMCJob erfolgreich initialisiert wurde

public:
	TElementsDesc* m_ElementsDesc;				// Beschreibung der Darstellungseigenschaften aller Elemente
	TElements* m_Elements;						// Beschreibung der verwendeten Elemente des KMC-Jobs
	TStructure* m_Structure;					// Beschreibung der untersuchten Struktur
	TJumps* m_Jumps;							// Beschreibung der Gitterspruenge
	TUniqueJumps* m_UniqueJumps;				// Beschreibung der einzigartigen Gitterspruenge
	TSettings* m_Settings;						// Beschreibung der Simulationseinstellungen
	TSimulation* m_Simulation;					// Klasse zu Durchfuehrung der Simulation
	TResults* m_Results;						// Beschreibung der Simulationsergebnisse

	// Member functions
public:
	// PUBLISHED
	bool IfReady();									// Rueckgabe von Ready

	int SetProjectName(string Name);					// Projektnamen setzen
	int SetUserName(string Name);						// Usernamen eingeben
	int SetProjectDate();								// Projektdatum auf aktuelles Datum setzen
	int SetProjectDescription(string Description);		// Projektbeschreibung eingeben
	int SaveToFile(string Filename);					// Job in Datei speichern
	int SaveToString(string& Output);					// Job in String speichern
	int LoadFromFile(string Filename);					// Job aus Datei laden
	int LoadFromString(string Input);					// Job aus String laden

	// -> Elemente
	int ClearElements();																		// Eingegebene Elemente loeschen
	int AddElement(string ElementSymbol, string ElementName, double ElementCharge);			// Element falls noch nicht vorhanden hinzufügen
	int SetMovingElement(string ElementSymbol, string ElementName, double ElementCharge);		// Element als bewegliche Spezies setzen
	int MakeElements();																		// Elemente sortieren und Elementliste schliessen

	// -> Struktur
	int ClearStructure();																																		// Eingegebene Struktur loeschen
	int SetStructureName(string StructureName);																												// Name der Struktur eingeben, max. 40 Zeichen
	int SetBasisByLatticeParam(double LengthA, double LengthB, double LengthC, double AngleA, double AngleB, double AngleC);									// Basis setzen aus 6 double (Längen, Winkel)
	int SetBasisByLatticeVectors(double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz);							// Basis setzen aus 9 double (Vektoren)
	int AddAtom(double Xrel, double Yrel, double Zrel, string AtomSymbol, string AtomName, double AtomCharge);													// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
	int AddAtom(double Xrel, double Yrel, double Zrel, int ElemID);																							// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
	int AddDoping(string DopedSymbol, string DopedName, double DopedCharge, string DopandSymbol, string DopandName, double DopandCharge, double VacDopRatio);	// Dotierung hinzufügen, noch keine Konzentrationen
	int AddDoping(int DopedID, int DopandID, double VacDopRatio);																								// Dotierung hinzufügen, noch keine Konzentrationen
	int MakeStructure();																																		// Struktur sortieren und überprüfen, dann Ready = true setzen

	// -> Gitterspruenge
	int ClearJumps();																								// Gitterspruenge loeschen
	int MakeJumps(int MovShellCount, int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount);		// alle Gittersprünge und deren Umgebung finden

	// -> Einzigartige Gitterspruenge
	int ClearUniqueJumps();												// Einzigartige Gittersprünge löschen
	int MakeUniqueJumps();													// Einzigartige Sprünge finden
	int SetJumpActive(int JumpID, bool State);								// Sprung aktivieren (= Sprung möglich) oder deaktivieren (= Sprung unmöglich), KMCERR_DEACTIVATION_IMPOSSIBLE zeigt, dass Sprung aktiviert bleiben muss
	int SetAtomIgnore(int JumpID, int CoordID, bool State);				// Umgebungsatom in einem Sprung ignorieren oder nicht-ignorieren
	int SetAtomAdditive(int JumpID, int CoordID, bool State);				// Umgebungsatom in einem Sprung additiv oder nicht-additiv setzen
	int SetAtomActive(int JumpID, int CoordID, bool State);				// Umgebungsatom in einem Sprung aktivieren oder deaktivieren
	int ClearUniqueCodes();												// Codierung loeschen
	int MakeUniqueCodes(int InteractionShellCount);										// Mögliche Umgebungsbesetzungen bestimmen und vergleichen (mehrfaches Update möglich), InteractionShellCount = Schalenanzahl zur Unterscheidung von Wechselwirkungen
	int SetInteractionEnergyByCoord(int JumpID, int CoordID, int ID, double Energy);		// WW-Energie [eV] für bestimmte CoordID (nicht-ignoriert, nicht-additiv) und bestimmte (Elem)ID-Besetzung setzen
	int SetInteractionEnergyByInteraction(int WWID, int ID, double Energy);				// WW-Energien [eV] für bestimmte InteractionID und bestimmte (Elem)ID-Besetzung setzen
	int SetCodeEnergy(int JumpID, int CodeID, double Energy);								// Code-Energie [eV] für bestimmte UniqueCodeID in bestimmtem UniqueJump setzen

	// -> Einstellungen
	int ClearSettings();																	// Einstellungen löschen
	int SetTemperature(double Temperature);												// Temperatur einstellen
	int SetAttemptFrequency(double Frequency);												// Sprungfrequenz einstellen
	int SetCheckpointFlags(bool WriteCheckpoint, bool LoadCheckpoint);						// Checkpoint-Flags setzen
	int SetEField(double EFieldX, double EFieldY, double EFieldZ, double MaxEField);		// E-Feld setzen aus MaxEField = maximaler E-Feld-Beitrag zu Aktivierungsenergie [in kBT] und EFieldxyz = E-Feld-Richtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
	int SetLatticeSize(int LatticeSize);													// Gittergröße einstellen
	int SetAdditionalVacAnz(long long AdditionalVacAnz);									// Zusätzliche Leerstellen eingeben
	int SetDopandConcentration(int DopingID, double Conc);									// Dotierkonzentration für eine bestimmte Dotierung eingeben
	int SetPrerunOptions(bool DoPrerun, long long PreMCSP, int PreRecordAnz);											// KMC-Vorlaufparameter einstellen
	int SetDynNormParameters(bool DoDynNorm, long long DynNormAttemptAnz, int DynNormRecordAnz, int DynNormNum);		// Parameter für dyn. Normierungssimulation setzen
	int SetMainKMCOptions(long long MCSP, int MainRecordAnz);															// KMC-Hauptsimulationsparameter eingeben
	int MakeSettings();																	// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen

	// -> Simulation
	int ClearSimulation();								// Simulationsdaten löschen
	int InitializeSimulation();						// Simulationsdaten initialisieren
	int SetCheckPointPath(string CheckPointPath);		// CheckPoint-Dateipfad setzen
	int LoadCheckPoint();								// Daten aus CheckPoint-Datei laden
	int MakeSimulation();								// Simulation durchfuehren

	// NON-PUBLISHED
	TKMCJobBase();		// Constructor -> Initialisierung, falls erfolgreich, Ready = true setzen

protected:
	~TKMCJobBase();	// Destructor -> Destructor der Member-Variablen aufrufen

	int SaveToStream(ostream& output, int offset);		// Projekt in einen Stream schreiben
	int LoadFromStream(istream& input);				// Projekt aus einem Stream laden

	int RCElements();							// Deletes and Recreates TElements Member Class
	int RCStructure();							// Deletes and Recreates TStructure Member Class
	int RCJumps();								// Deletes and Recreates TJumps Member Class
	int RCUniqueJumps();						// Deletes and Recreates TUniqueJumps Member Class
	int RCSettings();							// Deletes and Recreates TSettings Member Class
	int RCSimulation();						// Deletes and Recreates TSimulation Member Class
	int RCResults();							// Deletes and Recreates TResults Member Class
	int RCClasses(int FromClass);				// Deletes and Recreates Member Classes From FromClass (zero-based, in order of declaration)

	int StdExceptionHandler(exception& e);		// Global Standard Exception Handler
	int ExceptionHandler();					// Global Exception Handler for unknown Exceptions
};

#endif