// **************************************************************** //
//																	//
//	Klasse: IKMCJob													//
//	Autor: Philipp Hein												//
//	Datum: 24.06.2013												//
//  Aufgabe:														//
//    Abstract base class zur Abstraktion der internen Klassen von	//
//	  den aufrufbaren Methoden (sodass kein Export aller internen	//
//	  Klassen noetig)												//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifdef MCJOB_UDLL_EXPORTS 
#define DECLSPEC __declspec(dllexport) 
#else 
#define DECLSPEC __declspec(dllimport) 
#endif

#ifndef IKMCJob_H
#define IKMCJob_H

// Includes:
#include <string>

using namespace std;

// Klassendeklaration:
struct IKMCJob {
	// Deklaration der published-Member-Funktionen als pure virtual (d.h. virtual ... = 0)
	virtual void Release () = 0;									// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)
	virtual bool IfReady () = 0;									// Rückgabe, ob KMCJob erfolgreich initialisiert wurde

	virtual int SetProjectName (string Name) = 0;					// Projektnamen setzen
	virtual int SetUserName (string Name) = 0;						// Usernamen eingeben
	virtual int SetProjectDate () = 0;								// Projektdatum auf aktuelles Datum setzen
	virtual int SetProjectDescription (string Description) = 0;		// Projektbeschreibung eingeben
	virtual int GetProjectName (string &Name) = 0;					// Projektnamen ausgeben
	virtual int GetUserName (string &Name) = 0;						// Usernamen ausgeben
	virtual int GetProjectDate (string &CreateDate) = 0;			// Projektdatum ausgeben
	virtual int GetProjectDescription (string &Description) = 0;	// Projektbeschreibung ausgeben
	virtual int GetProjectState (int &State) = 0;					// Ausgabe des Projectstatus
	virtual int WriteToConsole () = 0;								// Gesamtes Projekt in Konsole schreiben
	virtual int SaveToFile (string Filename) = 0;					// Job in Datei speichern
	virtual int SaveToString (string &Output) = 0;					// Job in String speichern
	virtual int LoadFromFile (string Filename) = 0;					// Job aus Datei laden
	virtual int LoadFromString (string Input) = 0;					// Job aus String laden

	// -> Elemente
	virtual int ClearElements () = 0;																		// Eingegebene Elemente loeschen
	virtual int AddElement (string ElementSymbol, string ElementName, double ElementCharge) = 0;			// Element falls noch nicht vorhanden hinzufügen
	virtual int SetMovingElement (string ElementSymbol, string ElementName, double ElementCharge) = 0;		// Element als bewegliche Spezies setzen
	virtual int MakeElements () = 0;																		// Elemente sortieren und Elementliste schliessen
	virtual int GetElementCount (int &ElementCount) = 0;																// Anzahl der Elemente ausgeben
	virtual int GetElement (int ElementID, string &ElementSymbol, string &ElementName, double &ElementCharge) = 0;		// Ausgabe eines Elements
	virtual int GetElement (int ElementID, string &ElementSymbol, string &ElementName, double &ElementCharge, 
		double &Radius, double &ColorR, double &ColorG, double &ColorB) = 0;											// Ausgabe eines Elements
	virtual int GetMovingElement (string &ElementSymbol, string &ElementName, double &ElementCharge) = 0;				// Ausgabe des MovingElement
	virtual int GetMovingElement (string &ElementSymbol, string &ElementName, double &ElementCharge, 
		double &Radius, double &ColorR, double &ColorG, double &ColorB) = 0;											// Ausgabe des MovingElement
	virtual int GetVacancyElement (string &ElementSymbol, string &ElementName, double &ElementCharge) = 0;				// Ausgabe des VacancyElement
	virtual int GetVacancyElement (string &ElementSymbol, string &ElementName, double &ElementCharge, 
		double &Radius, double &ColorR, double &ColorG, double &ColorB) = 0;											// Ausgabe des VacancyElement
	virtual int WriteElementsToConsole () = 0;																			// Elementbeschreibung in Konsole schreiben

	// -> Struktur
	virtual int ClearStructure () = 0;																																		// Eingegebene Struktur loeschen
	virtual int SetStructureName (string StructureName) = 0;																												// Name der Struktur eingeben, max. 40 Zeichen
	virtual int SetBasisByLatticeParam (double LengthA, double LengthB, double LengthC, double AngleA, double AngleB, double AngleC) = 0;									// Basis setzen aus 6 double (Längen, Winkel)
	virtual int SetBasisByLatticeVectors (double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz) = 0;							// Basis setzen aus 9 double (Vektoren)
	virtual int AddAtom (double Xrel, double Yrel, double Zrel, string AtomSymbol, string AtomName, double AtomCharge) = 0;													// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
	virtual int AddAtom (double Xrel, double Yrel, double Zrel, int ElemID) = 0;																							// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
	virtual int AddDoping (string DopedSymbol, string DopedName, double DopedCharge, string DopandSymbol, string DopandName, double DopandCharge, double VacDopRatio) = 0;	// Dotierung hinzufügen, noch keine Konzentrationen
	virtual int AddDoping (int DopedID, int DopandID, double VacDopRatio) = 0;																								// Dotierung hinzufügen, noch keine Konzentrationen
	virtual int MakeStructure () = 0;																																		// Struktur sortieren und überprüfen, dann Ready = true setzen
	virtual int GetStructureName (string &StructureName) = 0;																												// Name der Struktur ausgeben
	virtual int GetBasisByLatticeParam (double &LengthA, double &LengthB, double &LengthC, double &AngleA, double &AngleB, double &AngleC) = 0;								// Basis ausgeben als 6 double (Längen, Winkel)
	virtual int GetBasisByLatticeVectors (double &Ax, double &Ay, double &Az, double &Bx, double &By, double &Bz, double &Cx, double &Cy, double &Cz) = 0;			// Basis ausgeben als 9 double (Vektoren)
	virtual int GetAtomCount (int &AtomCount) = 0;																													// Anzahl an Elementarzellatomen ausgeben
	virtual int GetMovCount (int &MovCount) = 0;																													// Anzahl der Mov-Atome in der Elementarzelle ausgeben
	virtual int GetAtom (int AtomID, double &Xrel, double &Yrel, double &Zrel, int &ElemID) = 0;																	// Atom ausgeben
	virtual int GetDopingCount (int &DopingCount) = 0;																												// Anzahl an Dotierungen ausgeben
	virtual int GetDoping (int DopingID, int &DopedID, int &DopandID, double &VacDopRatio) = 0;																		// Dotierung ausgeben
	virtual int WriteStructureToConsole () = 0;																														// Strukturbeschreibung in Konsole schreiben

	// -> Gitterspruenge
	virtual int ClearJumps () = 0;																										// Gitterspruenge loeschen
	virtual int MakeJumps (int MovShellCount, int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount) = 0;				// alle Gittersprünge und deren Umgebung finden
	virtual int GetShellCounts (int &MovShellCount, int &JumpStartShellCount, int &JumpTSShellCount, int &JumpDestShellCount) = 0;		// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
	virtual int GetJumpDirCount (int AtomID, int &DirectionCount) = 0;											// Anzahl an Sprungrichtungen fuer bestimmtes Mov- bzw. Vac-Atom in Elementarzelle
	virtual int GetJumpStartPos(int AtomID, int DirID, int &x, int &y, int &z, int &s) = 0;						// 4D-Vektor des Sprungstartatoms ausgeben
	virtual int GetJumpDestPos(int AtomID, int DirID, int &x, int &y, int &z, int &s) = 0;						// 4D-Vektor des Sprungzielatoms ausgeben
	virtual int GetJumpBackjumpDirID(int AtomID, int DirID, int &BackjumpDirID) = 0;							// Ruecksprung-DirID ausgeben
	virtual int GetJumpEnvCount(int AtomID, int DirID, int &EnvAtomCount) = 0;									// Anzahl an Umgebungsatomen ausgeben
	virtual int GetJumpEnvAtom(int AtomID, int DirID, int EnvAtomID, int &x, int &y, int &z, int &s) = 0;		// Umgebungsatom ausgeben
	virtual int GetJumpUniqueJumpID(int AtomID, int DirID, int &UniqueJumpID) = 0;								// ID des zugehörigen UniqueJump ausgeben
	virtual int WriteJumpsToConsole () = 0;																		// Sprungbeschreibungen in Konsole schreiben

	// -> Einzigartige Gitterspruenge
	virtual int ClearUniqueJumps () = 0;												// Einzigartige Gittersprünge löschen
	virtual int MakeUniqueJumps () = 0;													// Einzigartige Sprünge finden
	virtual int SetJumpActive (int JumpID, bool State) = 0;								// Sprung aktivieren (= Sprung möglich) oder deaktivieren (= Sprung unmöglich), KMCERR_DEACTIVATION_IMPOSSIBLE zeigt, dass Sprung aktiviert bleiben muss
	virtual int SetAtomIgnore (int JumpID, int CoordID, bool State) = 0;				// Umgebungsatom in einem Sprung ignorieren oder nicht-ignorieren
	virtual int SetAtomAdditive (int JumpID, int CoordID, bool State) = 0;				// Umgebungsatom in einem Sprung additiv oder nicht-additiv setzen
	virtual int SetAtomActive (int JumpID, int CoordID, bool State) = 0;				// Umgebungsatom in einem Sprung aktivieren oder deaktivieren
	virtual int ClearUniqueCodes () = 0;												// Codierung loeschen
	virtual int MakeUniqueCodes (int InteractionShellCount) = 0;										// Mögliche Umgebungsbesetzungen bestimmen und vergleichen (mehrfaches Update möglich), InteractionShellCount = Schalenanzahl zur Unterscheidung von Wechselwirkungen
	virtual int SetInteractionEnergyByCoord (int JumpID, int CoordID, int ID, double Energy) = 0;		// WW-Energie [eV] für bestimmte CoordID (nicht-ignoriert, nicht-additiv) und bestimmte (Elem)ID-Besetzung setzen
	virtual int SetInteractionEnergyByInteraction (int WWID, int ID, double Energy) = 0;				// WW-Energien [eV] für bestimmte InteractionID und bestimmte (Elem)ID-Besetzung setzen
	virtual int SetCodeEnergy (int JumpID, int CodeID, double Energy) = 0;								// Code-Energie [eV] für bestimmte UniqueCodeID in bestimmtem UniqueJump setzen
	virtual int GetUJumpCount (int &Count) = 0;								// Anzahl an UJumps ausgeben
	virtual int GetUJumpLength (int JumpID, double &JumpLength) = 0;		// Laenge eines Sprungs ausgeben
	virtual int GetUJumpActive (int JumpID, bool &JumpState) = 0;			// Aktivierungszustand eines Sprungs ausgeben
	virtual int GetUJumpAtomCount (int JumpID, int &Count) = 0;																	// Anzahl an Umgebungsatomen in bestimmtem UJump ausgeben
	virtual int GetUJumpAtomElement (int JumpID, int CoordID, int &ElemID) = 0;													// Element eines Umgebungsatoms eines bestimmten UJump ausgeben
	virtual int GetUJumpAtomZylPosition (int JumpID, int CoordID, double &X, double &Y, double &Z) = 0;							// Essentielle Informationen zu einem Umgebungsatom eines bestimmten UJump ausgeben
	virtual int GetUJumpAtomStates (int JumpID, int CoordID, bool &IsIgnore, bool &IsAdditive, bool &IsActive) = 0;				// Zustände (ignore, additive, active) eines Umgebungsatoms eines UJumps ausgeben
	virtual int GetUJumpAtomDistances (int JumpID, int CoordID, double &StartDist, double &TSDist, double &DestDist) = 0;		// Abstaende eines Umgebungsatoms eines bestimmten UJump ausgeben
	virtual int GetUJumpWWAtomsCount (int JumpID, int &Count) = 0;														// Anzahl an WW-Atomen (nicht-ignoriert, additiv) in bestimmtem UJump ausgeben
	virtual int GetUJumpWWAtomsInfo (int JumpID, int WWAtomsID, int &CoordID, int &WWID, int &EnergAnz) = 0;			// CoordID, InteractionID und Anzahl an InteractionElemIDs/Energies eines WW-Atoms ausgeben
	virtual int GetUJumpWWAtomsEnergy (int JumpID, int WWAtomsID, int ID, int &WWElemID, double &WWEnergy) = 0;			// Energie und ElemID der ID-ten Energieangabe eines WW-Atoms ausgeben
	virtual int GetUJumpUCodesCount (int JumpID, int &Count) = 0;														// Anzahl an UniqueCodes in bestimmtem UJump ausgeben
	virtual int GetUJumpUCode (int JumpID, int CodeID, string &Code, double &CodeEnergy) = 0;							// Energie und Code für einen UniqueCode in einem UJump ausgeben
	virtual int GetUJumpFCodesCount (int JumpID, int &Count) = 0;														// Anzahl an FullCodes in bestimmtem UJump ausgeben
	virtual int GetUJumpFCode (int JumpID, int CodeID, string &Code, int &UCodeID) = 0;									// UCodeID und Code für einen FullCode in einem UJump ausgeben
	virtual int GetInteractionShellCount (int &ShellCount) = 0;														// InteractionShellCount ausgeben
	virtual int GetInteractionCount (int &Count) = 0;																// Anzahl an Interactions ausgeben
	virtual int GetInteractionDesc (int WWID, int &ElemID, double &StartDist, double &DestDist) = 0;				// Grundelementbesetzung und Abstände für eine InteractionID ausgeben
	virtual int GetInteractionStartEnvCount (int WWID, int &Count) = 0;												// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	virtual int GetInteractionDestEnvCount (int WWID, int &Count) = 0;												// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	virtual int GetInteractionStartEnvAtom (int WWID, int ID, int &ElemID, double &X, double &Y, double &Z) = 0;	// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	virtual int GetInteractionDestEnvAtom (int WWID, int ID, int &ElemID, double &X, double &Y, double &Z) = 0;		// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
	virtual int WriteUJumpToConsole (int JumpID) = 0;			// Vollständige Ausgabe eines UJump
	virtual int WriteInteractionToConsole (int WWID) = 0;		// Vollständige Ausgabe einer Wechselwirkung
	virtual int WriteUniqueJumpsToConsole () = 0;				// Beschreibung der einzigartigen Sprünge in Konsole schreiben

	// -> Einstellungen
	virtual int ClearSettings () = 0;																	// Einstellungen löschen
	virtual int SetTemperature (double Temperature) = 0;												// Temperatur einstellen
	virtual int SetAttemptFrequency (double Frequency) = 0;												// Sprungfrequenz einstellen
	virtual int SetCheckpointFlags (bool WriteCheckpoint, bool LoadCheckpoint) = 0;						// Checkpoint-Flags setzen
	virtual int SetEField (double EFieldX, double EFieldY, double EFieldZ, double MaxEField) = 0;		// E-Feld setzen aus MaxEField = maximaler E-Feld-Beitrag zu Aktivierungsenergie [in kBT] und EFieldxyz = E-Feld-Richtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
	virtual int SetLatticeSize (int LatticeSize) = 0;													// Gittergröße einstellen
	virtual int SetAdditionalVacAnz (long long AdditionalVacAnz) = 0;									// Zusätzliche Leerstellen eingeben
	virtual int SetDopandConcentration (int DopingID, double Conc) = 0;									// Dotierkonzentration für eine bestimmte Dotierung eingeben
	virtual int SetPrerunOptions (bool DoPrerun, long long PreMCSP, int PreRecordAnz) = 0;											// KMC-Vorlaufparameter einstellen
	virtual int SetDynNormParameters (bool DoDynNorm, long long DynNormAttemptAnz, int DynNormRecordAnz, int DynNormNum) = 0;		// Parameter für dyn. Normierungssimulation setzen
	virtual int SetMainKMCOptions (long long MCSP, int MainRecordAnz) = 0;															// KMC-Hauptsimulationsparameter eingeben
	virtual int MakeSettings () = 0;																	// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen
	virtual int GetTemperature (double &Temperature) = 0;																	// Temperatur ausgeben
	virtual int GetAttemptFrequency (double &Frequency) = 0;																// Sprungfrequenz ausgeben
	virtual int GetCheckpointFlags (bool &WriteCheckpoint, bool &LoadCheckpoint) = 0;										// Checkpoint-Flags ausgeben
	virtual int GetEFieldSettings (double &EFieldDirX, double &EFieldDirY, double &EFieldDirZ, double &MaxEField) = 0;		// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
	virtual int GetEField (double &EFieldX, double &EFieldY, double &EFieldZ) = 0;											// Berechnetes E-Feld ausgeben
	virtual int GetLatticeSize (int &LatticeSize) = 0;									// Gittergröße ausgeben
	virtual int GetAdditionalVacAnz (long long &AdditionalVacAnz) = 0;					// Zusätzliche Leerstellen ausgeben
	virtual int GetTotalVacancyConc (double &Conc) = 0;									// Leerstellenkonzentration ausgeben
	virtual int GetTotalVacancyAnz (long long &Anz) = 0;								// Anzahl an Leerstellen ausgeben
	virtual int GetMovVolConc (double &MovVolConc) = 0;									// Volumenkonzentration der beweglichen Spezies ausgeben
	virtual int GetDopandConc (int ID, double &Conc) = 0;								// Dotierkonzentration für eine bestimmte Dotierung ausgeben
	virtual int GetDopandAnz (int ID, long long &Anz) = 0;								// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben
	virtual int GetPrerunOptions (bool &DoPrerun, long long &PreMCSP, int &PreRecordAnz) = 0;											// KMC-Vorlaufparameter ausgeben
	virtual int GetDynNormParameters (bool &DoDynNorm, long long &DynNormAttemptAnz, int &DynNormRecordAnz, int &DynNormNum) = 0;		// Parameter für dyn. Normierungssimulation ausgeben
	virtual int GetMainKMCOptions (long long &MCSP, int &MainRecordAnz) = 0;															// KMC-Hauptsimulationsparameter ausgeben
	virtual int WriteSettingsToConsole () = 0;											// Beschreibung der Einstellungen in Konsole schreiben

	// -> Simulation
	virtual int ClearSimulation () = 0;								// Simulationsdaten löschen
	virtual int InitializeSimulation () = 0;						// Simulationsdaten initialisieren
	virtual int SetCheckPointPath (string CheckPointPath) = 0;		// CheckPoint-Dateipfad setzen
	virtual int LoadCheckPoint () = 0;								// Daten aus CheckPoint-Datei laden
	virtual int MakeSimulation () = 0;								// Simulation durchfuehren

	// -> Ergebnisse
	virtual int GetResultSummaryDesc(string ValDelimiter, string &SummaryDesc) = 0;		// Beschreibung der GetResultSummary-Ausgabe ausgeben
	virtual int GetResultSummary(string ValDelimiter, string &Summary) = 0;				// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben 

};

// Exportierte Funktion zur Erstellung einer Instanz der Klasse
#ifndef NON_DLL_BUILD
extern "C" DECLSPEC IKMCJob * __stdcall CreateKMCJob ();
#endif

#endif