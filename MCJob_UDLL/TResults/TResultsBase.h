// **************************************************************** //
//																	//
//	Class: TResultsBase	(TResults Layer 0)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the simulation results						//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TResultsBase_H
#define TResultsBase_H

// Includes:
#include <string>
#include <vector>
#include <iostream>

// Eigene Includes:
#include "T3DVector.h"
#include "TCustomTime.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TResultsBase
{
	// Member variables
protected:
	TKMCJob* m_Job;				// Pointer zur uebergeordneten KMCJob-Instanz

	// Main-Results
	unsigned long long MCSP;						// Monte-Carlo-Steps per Particle
	unsigned long long JumpAttempts;				// Sprungversuchsanzahl
	unsigned long long NonsenseJumpAttempts;		// Anzahl an Sprungversuchen mit Ruecksprungwahrscheinlichkeit >= 1 (sind in JumpAttempts enthalten)
	unsigned long long OverkillJumpAttempts;		// Anzahl an Sprungversuchen mit Hinsprungwahrscheinlichkeit >= 1 (sind nicht in JumpAttempts enthalten)
	unsigned long long SiteBlockingCounter;			// Anzahl an SiteBlocking, d.h. Zielort schon von Leerstelle besetzt (sind in JumpAttempts enthalten)
	double Normalization;							// Normierungsfaktor der Wahrscheinlichkeiten
	TCustomTime Runtime;							// Laufzeit der Simulation
	double SimulatedTime;							// Simulierte Zeitspanne in s
	double Conductivity;							// Ionische Leitfaehigkeit in E-Feldvektorrichtung in S/cm

	// Main-Results: Mov-Bewegungsparameter
	double MovMeanJumpCount;		// <jcount> = mittlere Sprunganzahl
	T3DVector MovMeanDispVec;		// <x>, <y>, <z> = mittlerer Verschiebungsvektor in cm
	double MovDispParallel;			// Komponente des mittleren Verschiebungsvektors parallel zum E-Feld in cm
	double MovDispPerpendicular;	// Komponente des mittleren Verschiebungsvektors senkrecht zum E-Feld in cm
	double MovMeanDisp;				// <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	double MovMeanSquaredDisp;		// <x*x + y*y + z*z> = mittlere quadratische Verschiebung in cm^2
	T3DVector MovComDispVec;		// sum(x), sum(y), sum(z) = Schwerpunktverschiebungsvektor in cm

	// Main-Results: Vac-Bewegungsparameter
	double VacMeanJumpCount;		// <jcount> = mittlere Sprunganzahl
	T3DVector VacMeanDispVec;		// <x>, <y>, <z> = mittlerer Verschiebungsvektor in cm
	double VacDispParallel;			// Komponente des mittleren Verschiebungsvektors parallel zum E-Feld in cm
	double VacDispPerpendicular;	// Komponente des mittleren Verschiebungsvektors senkrecht zum E-Feld in cm
	double VacMeanDisp;				// <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	double VacMeanSquaredDisp;		// <x*x + y*y + z*z> = mittlere quadratische Verschiebung in cm^2
	T3DVector VacComDispVec;		// sum(x), sum(y), sum(z) = Schwerpunktverschiebungsvektor in cm

	bool Ready;						// Flag, ob TResults in einem korrekten Zustand, d.h. korrekt erzeugt oder geladen

	// Member functions
public:
	// PUBLISHED

	// NON-PUBLISHED
	int MakeResults();				// Ergebnisse aus abgeschlossener Simulation laden

	TResultsBase(TKMCJob* pJob);				// Constructor, Ready = false setzen
	bool IfReady();							// Rueckgabe von Ready
	bool IfCompleted(long long required_MCSP);	// Rueckgabe von Ready und genuegend MCSP
	int SaveToStream(ostream& output, int offset);		// Ergebnisse in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
	int LoadFromStream(istream& input);				// Ergebnisse aus einem Stream laden

protected:
	~TResultsBase();						// Destructor
};

#endif