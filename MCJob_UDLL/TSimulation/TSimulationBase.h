// **************************************************************** //
//																	//
//	Class: TSimulationBase	(TSimulation Layer 0)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for carrying out the simulation							//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSimulationBase_H
#define TSimulationBase_H

// Includes:
#include <string>
#include <vector>
#include <random>

// Eigene Includes:
#include "TSimulation/TSimProb.h"
#include "TSimulation/TSimUniqueJump.h"
#include "TSimulation/TSimJump.h"
#include "TSimulation/TSimPhaseInfo.h"
#include "GlobalDefinitions.h"
#include "T4DLatticeVector.h"
#include "T3DVector.h"
#include "T3DMovVector.h"
#include "TCustomTime.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSimulationBase
{
	// Member variables
protected:
	TKMCJob* m_Job;				// Pointer zur uebergeordneten KMCJob-Instanz

	// Checkpoint-Parameter
	string CheckPointPath;												// Pfad der CheckPoint-Datei

	// Startparameter (letzter CheckPoint-Stand)
	vector<vector<vector<vector<int>>>> StartLattice;					// Gitterzustand beim Start

	// Vorlauf-Parameter (letzter CheckPoint-Stand)
	TSimPhaseInfo PrerunPhaseData;										// aktueller Stand aller Vorlauf-Parameter

	// Parameter der dynamischen Normierung (letzter CheckPoint-Stand)
	TSimPhaseInfo DynNormPhaseData;										// aktueller Stand aller Parameter der dynamischen Normierung

	// Hauptsimulation-Parameter (letzter CheckPoint-Stand)
	TSimPhaseInfo MainPhaseData;										// aktueller Stand aller Hauptsimulationsparameter

	// Konstante Simulationsparameter (sp = direkter Zugriff waehrend Simulation, konstant wenn Ready = true)
	unsigned long long spTotalMovAnz;				// Anzahl an Atomen der beweglichen Spezies im Gitter
	unsigned long long spTotalVacAnz;				// Anzahl an Leerstellen im Gitter
	int spLatticeSize;								// Gittergroesse
	int spStackSize;								// Anzahl an Atomen in der Elementarzelle
	int spMovStackSize;								// Anzahl an Atomen der beweglichen Spezies in der Elementarzelle
	mt19937_64 spRNG;								// Zufallszahlengenerator-Instanz
	uniform_real_distribution<double> spProbDist;	// Random-Distribution zur Sprungannahme oder Ablehnung (Range: [0.0, 1.0) )
	int spDirSelCount;								// Anzahl an unterschiedlichen Sprungrichtungsanzahlen
	int* spDirCount;								// Liste der Sprungrichtungsanzahlen
	int* spDirLink;									// Zuordnung von s-Koordinate zu Sprungrichtungsanzahl
	TSimUniqueJump* spUniqueJumps;					// Liste der UniqueJumps
	TSimJump** spJumps;								// Liste der Spruenge (incl. Umgebungen) fuer jedes Atom der beweglichen Spezies in der Elementarzelle
	double spHighestProb;							// Hoechstmoegliche Wahrscheinlichkeit (ohne Normierung)
	bool spIfBackjumpRelevant;						// Flag, die anzeigt, ob unnormierte Ruecksprungwahrscheinlichkeiten >= 1 moeglich sind (d.h. ob Ruecksprungwahrscheinlichkeiten bestimmt werden muessen)
	double spFrequency;								// Sprungversuchsfrequenz (in 1/s)
	bool spCanCalcCond;								// Flag: true = Leitfaehigkeit kann berechnet werden (= E-Feld ist vorhanden)					
	double spCondFactor;							// Faktor zur Leitfaehigkeitsberechnung (= MovCharge * MovVolConc / E-Feld-Betrag in C/(Vcm^2))

	// Variable Simulationsparameter (sp, veraendern sich waehrend der Simulation oder zwischen den verschiedenen Simulationsphasen)
	unsigned long long spMCSP;								// MCSP-Zaehler
	unsigned long long spTargetMCSP;						// Ziel-MCSP
	unsigned long long spJumpAttempts;						// Zaehler fuer Sprungversuche
	unsigned long long spTargetJumpAttempts;				// Ziel-Sprungversuche
	unsigned long long spNonsenseAttempts;					// Zaehler fuer Sprungversuche mit Ruecksprungwahrscheinlichkeit >= 1 (sind in spJumpAttempts enthalten)
	unsigned long long spOverkillAttempts;					// Zaehler fuer Sprungversuche mit Hinsprungwahrscheinlichkeit >= 1 (sind nicht in spJumpAttempts enthalten)
	unsigned long long spSiteBlockingCounter;				// Zaehler fuer Site-Blocking, d.h. Zielplatz schon von Leerstelle besetzt (sind in spJumpAttempts enthalten)
	double spNormalization;									// Skalierungsfaktor fuer die Wahrscheinlichkeiten
	int* spLattice;											// Gitter
	T3DMovVector* spMovLattice;								// Gitter zur Bewegungsspeicherung
	unsigned long long* spVacListSizes;						// Anzahlen an Leerstellen je Sprungrichtungsanzahl (schon multipliziert mit Richtungsanzahl)
	T4DLatticeVector** spVacLists;							// Liste der Leerstellenpositionen je Sprungrichtungsanzahl
	unsigned long long spVacMaxIndex;						// Maximaler Index zur Leerstellen- und Richtungsauswahl
	uniform_int_distribution<unsigned long long> spDirDist;	// Random-Distribution zur Leerstellen- und Richtungsauswahl
	double spAttemptPathRatioSum;							// Summe des Quotienten aus Sprungversuchen und Sprungmoeglichkeiten (ueber alle MCS)
	int spProbListSize;										// Groesse der Wahrscheinlichkeitslisten
	TSimProb* spAttemptProbList;							// Liste der Sprungversuchswahrscheinlichkeiten
	TSimProb* spAcceptedProbList;							// Liste der Wahrscheinlichkeiten der durchgefuehrten Spruenge
	TCustomTime spPreviousRunTime;							// Laufzeit der aktuellen Simulationsphase vor der aktuellen Simulation (fuer fortgesetzte Simulationen)
	TCustomTime spStartTime;								// Startzeit der aktuellen Simulation
	TCustomTime spRunTime;									// Laufzeit der aktuellen Simulation

	bool Ready;			// Flag, die anzeigt, ob alle Daten in TSimulation korrekt initialisiert wurden und die Klasse bereit zur Durchfuehrung der Simulation ist
	bool Completed;		// Flag, die anzeigt, ob die Simulation erfolgreich abgeschlossen wurde

	// Member functions
public:
	// PUBLISHED
	int InitializeSimulation();							// Simulationsdaten initialisieren, Ready = true setzen
	int SetCheckPointPath(string i_CheckPointPath);		// CheckPoint-Dateipfad setzen
	int LoadCheckPoint();								// Daten aus CheckPoint-Datei laden
	int MakeSimulation();								// Simulation durchfuehren

	// NON-PUBLISHED
	TSimulationBase(TKMCJob* pJob);		// Constructor, Ready = false setzen
	bool IfReady();						// Rueckgabe von Ready
	bool IfCompleted();					// Rueckgabe von Completed

	// Member functions
private:
	// Simulation phases
	int SPStartPreparations();					// Startvorbereitungen
	int SPPrerunPreparations();					// Vorbereitungen fuer die Vorlauf-Simulation
	int SPPrerunPostProcessing();				// Nachbereitungen der Vorlauf-Simulation
	int SPDynNormPreparations();				// Vorbereitungen fuer die DynNorm-Simulation
	int SPDynNormPostProcessing();				// Nachbereitungen der DynNorm-Simulation
	int SPMainPreparations();					// Vorbereitungen fuer die Hauptsimulation 
	int SPMainPostProcessing();					// Nachbereitungen der Hauptsimulation
	int SPSimulate(TSimPhaseInfo& ispPhase);	// Simulationsroutine: KMC-Simulation mit den aktuellen Parametern durchfuehren

	// Helper functions
	void Clear();								// Alle dynamisch erzeugten Arrays loeschen, uebrige Daten auf Standardwerte setzen, Ready = false setzen
	void ClearCurrentSim();						// Alle variablen Simulationsparameter zuruecksetzen
	int IfAllClassesReady() const;				// Alle Einstellungsklassen auf Existenz und Ready = true pruefen
	int GetHighestProb(double& o_prob) const;													// Hoechstmoegliche Wahrscheinlichkeit (ohne Normierung) berechnen
	int GetCondFactor(bool& o_cancalccond, double& o_freq, double& o_condfactor) const;			// Pruefen ob Leitfaehigkeitsberechnung moeglich und Auswertungsparameter berechnen
	int CreateRandomLattice(vector<vector<vector<vector<int>>>>* o_lattice);					// Gitter mit zufaelliger Verteilung der Dopanden und Leerstellen erzeugen
	int CreateMovLattice(vector<vector<vector<vector<T3DMovVector>>>>* o_movlattice) const;				// Leeres Gitter zur Bewegungsspeicherung erstellen
	int LatticeAnalysis(string i_space, const vector<vector<vector<vector<int>>>>* i_lattice) const;	// Gitterstatistik ausgeben
	int LatticeProbabilitiesAnalysis(string i_space, const vector<vector<vector<vector<int>>>>* i_lattice) const;	// Statistik aller aktuellen Sprungwahrscheinlichkeiten ausgeben (ohne Normierung)
	int PhaseAnalysis(string i_space, bool is_short, const TSimPhaseInfo& i_phase, bool show_probs) const;			// Simulationsphase auswerten
	int UniqueSampling(size_t id_count, size_t pos_count, vector<size_t>* o_selected);								// id_count Zahlen aus dem Interval [0, pos_count-1] zufaellig auswaehlen (keine Zahl doppelt, in aufsteigender Ordnung)
	int SetSimulationData(const TSimPhaseInfo& i_phase);						// Synchronisation: i_phase -> variable Simulationsparameter
	int GetSimulationData(TSimPhaseInfo& o_phase) const;						// Synchronisation: variable Simulationsparameter -> o_phase
	int ValidatePrerunData(TSimPhaseInfo& i_phase);								// PrerunPhaseData validieren
	int ValidateDynNormData(TSimPhaseInfo& i_phase);							// DynNormPhaseData validieren
	int ValidateMainData(TSimPhaseInfo& i_phase);								// MainPhaseData validieren
	int ValidateLattice(vector<vector<vector<vector<int>>>>* i_lattice);		// Gitter validieren
	int ValidateProbLists(TSimPhaseInfo& i_phase, int i_listsize);		// Wahrscheinlichkeitslisten einer Phase validieren
	void SaveCheckPoint(string i_space, string i_chkpath);				// Daten in CheckPoint-Datei speichern
	int LoadCheckPoint(string i_chkpath);								// Daten aus CheckPoint-Datei laden
	int StdExceptionHandler(exception& e);				// Standard Exception Handler
	int ExceptionHandler();							// Exception Handler for unknown Exceptions

	// Member functions
protected:
	~TSimulationBase();					// Destructor

	// Helper functions
	int MovLatticeAnalysis(const vector<vector<vector<vector<int>>>>* i_lattice,
		const vector<vector<vector<vector<T3DMovVector>>>>* i_movlattice, vector<double>* o_results) const;			// Bewegungsstatistik ausgeben
	int GetProjOnEField(const T3DVector& i_disp_vec, double& o_comp_parallel, double& o_comp_perpendicular) const;	// Verschiebungsvektor in eine Komponente parallel zum E-Feld und eine Komponente senkrecht zum E-Feld zerlegen (in der Ebene, die durch Verschiebungsvektor und E-Feld-Vektor aufgespannt wird)

};

#endif