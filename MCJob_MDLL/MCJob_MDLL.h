// **************************************************************** //
//																	//
//	Class: TMCJobWrapper											//
//	Author: Philipp Hein											//
//  Description:													//
//    Managed C++/CLI assembly (managed DLL) as wrapper for the		//
//	  TKMCJob class of the unmanaged DLL							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TMCJobWrapper_H
#define TMCJobWrapper_H

// Includes:

// Eigene Includes:
#include "..\MCJob_UDLL\TKMCJob\IKMCJob.h"

// Klassendeklaration:
public ref class TMCJobWrapper
{
protected:
	IKMCJob* m_Job;

	void String_M2U(System::String^ in_str, std::string& out_str);
	void String_U2M(std::string in_str, System::String^% out_str);

public:
	TMCJobWrapper();		// Constructor
	~TMCJobWrapper();		// Destructor
	!TMCJobWrapper();		// Finalizer

	bool IfReady();		// Rückgabe, ob KMCJob erfolgreich initialisiert wurde

	int SetProjectName(System::String^ Name);						// Projektnamen setzen
	int SetUserName(System::String^ Name);							// Usernamen eingeben
	int SetProjectDate();											// Projektdatum auf aktuelles Datum setzen
	int SetProjectDescription(System::String^ Description);		// Projektbeschreibung eingeben
	int GetProjectName(System::String^% Name);						// Projektnamen ausgeben
	int GetUserName(System::String^% Name);						// Usernamen ausgeben
	int GetProjectDate(System::String^% CreateDate);				// Projektdatum ausgeben
	int GetProjectDescription(System::String^% Description);		// Projektbeschreibung ausgeben
	int GetProjectState(int% State);								// Ausgabe des Projectstatus
	int WriteToConsole();											// Gesamtes Projekt in Konsole schreiben
	int SaveToFile(System::String^ Filename);						// Job in Datei speichern
	int SaveToString(System::String^% Output);						// Job in String speichern
	int LoadFromFile(System::String^ Filename);					// Job aus Datei laden
	int LoadFromString(System::String^ Input);						// Job aus String laden

	// -> Elemente
	int ClearElements();																						// Eingegebene Elemente loeschen
	int AddElement(System::String^ ElementSymbol, System::String^ ElementName, double ElementCharge);			// Element falls noch nicht vorhanden hinzufügen
	int SetMovingElement(System::String^ ElementSymbol, System::String^ ElementName, double ElementCharge);	// Element als bewegliche Spezies setzen
	int MakeElements();																						// Elemente sortieren und Elementliste schliessen
	int GetElementCount(int% ElementCount);																				// Anzahl der Elemente ausgeben
	int GetElement(int ElementID, System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge);	// Ausgabe eines Elements
	int GetElement(int ElementID, System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge,
		double% Radius, double% ColorR, double% ColorG, double% ColorB);										// Ausgabe eines Elements
	int GetMovingElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge);				// Ausgabe des MovingElement
	int GetMovingElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge,
		double% Radius, double% ColorR, double% ColorG, double% ColorB);									// Ausgabe des MovingElement
	int GetVacancyElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge);			// Ausgabe des VacancyElement
	int GetVacancyElement(System::String^% ElementSymbol, System::String^% ElementName, double% ElementCharge,
		double% Radius, double% ColorR, double% ColorG, double% ColorB);									// Ausgabe des VacancyElement
	int WriteElementsToConsole();																							// Elementbeschreibung in Konsole schreiben

	// -> Struktur
	int ClearStructure();																																											// Eingegebene Struktur loeschen
	int SetStructureName(System::String^ StructureName);																																			// Name der Struktur eingeben, max. 40 Zeichen
	int SetBasisByLatticeParam(double LengthA, double LengthB, double LengthC, double AngleA, double AngleB, double AngleC);																		// Basis setzen aus 6 double (Längen, Winkel)
	int SetBasisByLatticeVectors(double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz);																// Basis setzen aus 9 double (Vektoren)
	int AddAtom(double Xrel, double Yrel, double Zrel, System::String^ AtomSymbol, System::String^ AtomName, double AtomCharge);																	// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
	int AddAtom(double Xrel, double Yrel, double Zrel, int ElemID);																																// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
	int AddDoping(System::String^ DopedSymbol, System::String^ DopedName, double DopedCharge, System::String^ DopandSymbol, System::String^ DopandName, double DopandCharge, double VacDopRatio);	// Dotierung hinzufügen, noch keine Konzentrationen
	int AddDoping(int DopedID, int DopandID, double VacDopRatio);																																	// Dotierung hinzufügen, noch keine Konzentrationen
	int MakeStructure();																																											// Struktur sortieren und überprüfen, dann Ready = true setzen
	int GetStructureName(System::String^% StructureName);																											// Name der Struktur ausgeben
	int GetBasisByLatticeParam(double% LengthA, double% LengthB, double% LengthC, double% AngleA, double% AngleB, double% AngleC);									// Basis ausgeben als 6 double (Längen, Winkel)
	int GetBasisByLatticeVectors(double% Ax, double% Ay, double% Az, double% Bx, double% By, double% Bz, double% Cx, double% Cy, double% Cz);						// Basis ausgeben als 9 double (Vektoren)
	int GetAtomCount(int% AtomCount);																																// Anzahl an Elementarzellatomen ausgeben
	int GetMovCount(int% MovCount);																																// Anzahl der Mov-Atome in der Elementarzelle ausgeben
	int GetAtom(int AtomID, double% Xrel, double% Yrel, double% Zrel, int% ElemID);																				// Atom ausgeben
	int GetDopingCount(int% DopingCount);																															// Anzahl an Dotierungen ausgeben
	int GetDoping(int DopingID, int% DopedID, int% DopandID, double% VacDopRatio);																					// Dotierung ausgeben
	int WriteStructureToConsole();																																	// Strukturbeschreibung in Konsole schreiben

	// -> Gitterspruenge
	int ClearJumps();													// Gitterspruenge loeschen
	int MakeJumps(int MovShellCount, int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount);				// alle Gittersprünge und deren Umgebung finden
	int GetShellCounts(int% MovShellCount, int% JumpStartShellCount, int% JumpTSShellCount, int% JumpDestShellCount);		// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
	int GetJumpDirCount(int AtomID, int% DirectionCount);											// Anzahl an Sprungrichtungen fuer bestimmtes Mov- bzw. Vac-Atom in Elementarzelle
	int GetJumpStartPos(int AtomID, int DirID, int% x, int% y, int% z, int% s);						// 4D-Vektor des Sprungstartatoms ausgeben
	int GetJumpDestPos(int AtomID, int DirID, int% x, int% y, int% z, int% s);						// 4D-Vektor des Sprungzielatoms ausgeben
	int GetJumpBackjumpDirID(int AtomID, int DirID, int% BackjumpDirID);							// Ruecksprung-DirID ausgeben
	int GetJumpEnvCount(int AtomID, int DirID, int% EnvAtomCount);									// Anzahl an Umgebungsatomen ausgeben
	int GetJumpEnvAtom(int AtomID, int DirID, int EnvAtomID, int% x, int% y, int% z, int% s);		// Umgebungsatom ausgeben
	int GetJumpUniqueJumpID(int AtomID, int DirID, int% UniqueJumpID);								// ID des zugehörigen UniqueJump ausgeben
	int WriteJumpsToConsole();																		// Sprungbeschreibungen in Konsole schreiben

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
	int GetUJumpCount(int% Count);										// Anzahl an UJumps ausgeben
	int GetUJumpLength(int JumpID, double% JumpLength);				// Laenge eines Sprungs ausgeben
	int GetUJumpActive(int JumpID, bool% JumpState);					// Aktivierungszustand eines Sprungs ausgeben
	int GetUJumpAtomCount(int JumpID, int% Count);																			// Anzahl an Umgebungsatomen in bestimmtem UJump ausgeben
	int GetUJumpAtomElement(int JumpID, int CoordID, int% ElemID);															// Element eines Umgebungsatoms eines bestimmten UJump ausgeben
	int GetUJumpAtomZylPosition(int JumpID, int CoordID, double% X, double% Y, double% Z);									// Essentielle Informationen zu einem Umgebungsatom eines bestimmten UJump ausgeben
	int GetUJumpAtomStates(int JumpID, int CoordID, bool% IsIgnore, bool% IsAdditive, bool% IsActive);						// Zustände (ignore, additive, active) eines Umgebungsatoms eines UJumps ausgeben
	int GetUJumpAtomDistances(int JumpID, int CoordID, double% StartDist, double% TSDist, double% DestDist);				// Abstaende eines Umgebungsatoms eines bestimmten UJump ausgeben
	int GetUJumpWWAtomsCount(int JumpID, int% Count);																	// Anzahl an WW-Atomen (nicht-ignoriert, additiv) in bestimmtem UJump ausgeben
	int GetUJumpWWAtomsInfo(int JumpID, int WWAtomsID, int% CoordID, int% WWID, int% EnergAnz);						// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
	int GetUJumpWWAtomsEnergy(int JumpID, int WWAtomsID, int ID, int% WWElemID, double% WWEnergy);						// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben
	int GetUJumpUCodesCount(int JumpID, int% Count);														// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
	int GetUJumpUCode(int JumpID, int CodeID, System::String^% Code, double% CodeEnergy);					// Energie und Code für einen UniqueCode in einem UJump ausgeben
	int GetUJumpFCodesCount(int JumpID, int% Count);														// Anzahl an FullCodes in bestimmtem UJump ausgeben
	int GetUJumpFCode(int JumpID, int CodeID, System::String^% Code, int% UCodeID);						// UCodeID und Code für einen FullCode in einem UJump ausgeben
	int GetInteractionShellCount(int% ShellCount);																		// InteractionShellCount ausgeben
	int GetInteractionCount(int% Count);																				// Anzahl an Interactions ausgeben
	int GetInteractionDesc(int WWID, int% ElemID, double% StartDist, double% DestDist);								// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
	int GetInteractionStartEnvCount(int WWID, int% Count);												// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvCount(int WWID, int% Count);												// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionStartEnvAtom(int WWID, int ID, int% ElemID, double% X, double% Y, double% Z);	// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int GetInteractionDestEnvAtom(int WWID, int ID, int% ElemID, double% X, double% Y, double% Z);		// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	int WriteUJumpToConsole(int JumpID);				// Vollständige Ausgabe eines UJump
	int WriteInteractionToConsole(int WWID);			// Vollständige Ausgabe einer Wechselwirkung
	int WriteUniqueJumpsToConsole();					// Beschreibung der einzigartigen Sprünge in Konsole schreiben

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
	int MakeSettings();																					// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen
	int GetTemperature(double% Temperature);																// Temperatur ausgeben
	int GetAttemptFrequency(double% Frequency);															// Sprungfrequenz ausgeben
	int GetCheckpointFlags(bool% WriteCheckpoint, bool% LoadCheckpoint);									// Checkpoint-Flags ausgeben
	int GetEFieldSettings(double% EFieldDirX, double% EFieldDirY, double% EFieldDirZ, double% MaxEField);	// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
	int GetEField(double% EFieldX, double% EFieldY, double% EFieldZ);										// Berechnetes E-Feld ausgeben
	int GetLatticeSize(int% LatticeSize);									// Gittergröße ausgeben
	int GetAdditionalVacAnz(long long% AdditionalVacAnz);					// Zusätzliche Leerstellen ausgeben
	int GetTotalVacancyConc(double% Conc);									// Leerstellenkonzentration ausgeben
	int GetTotalVacancyAnz(long long% Anz);								// Anzahl an Leerstellen ausgeben
	int GetMovVolConc(double% MovVolConc);									// Volumenkonzentration der beweglichen Spezies ausgeben
	int GetDopandConc(int ID, double% Conc);								// Dotierkonzentration für eine bestimmte Dotierung ausgeben
	int GetDopandAnz(int ID, long long% Anz);								// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben
	int GetPrerunOptions(bool% DoPrerun, long long% PreMCSP, int% PreRecordAnz);											// KMC-Vorlaufparameter ausgeben
	int GetDynNormParameters(bool% DoDynNorm, long long% DynNormAttemptAnz, int% DynNormRecordAnz, int% DynNormNum);		// Parameter für dyn. Normierungssimulation ausgeben
	int GetMainKMCOptions(long long% MCSP, int% MainRecordAnz);															// KMC-Hauptsimulationsparameter ausgeben
	int WriteSettingsToConsole();											// Beschreibung der Einstellungen in Konsole schreiben

	// -> Simulation
	int ClearSimulation();										// Simulationsdaten löschen
	int InitializeSimulation();								// Simulationsdaten initialisieren
	int SetCheckPointPath(System::String^ CheckPointPath);		// CheckPoint-Dateipfad setzen
	int LoadCheckPoint();										// Daten aus CheckPoint-Datei laden
	int MakeSimulation();										// Simulation durchfuehren

	// -> Ergebnisse
	int GetResultSummaryDesc(System::String^ ValDelimiter, System::String^% SummaryDesc);		// Beschreibung der GetResultSummary-Ausgabe ausgeben
	int GetResultSummary(System::String^ ValDelimiter, System::String^% Summary);				// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben

};

#endif
