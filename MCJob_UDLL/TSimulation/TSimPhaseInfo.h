// **************************************************************** //
//																	//
//	Klasse: TSimPhaseInfo											//
//	Autor: Philipp Hein												//
//	Datum: 17.03.2014												//
//  Aufgabe:														//
//    Klasse, die den letzten CheckPoint-Stand einer				//
//	  Simulationsphase enthaelt										//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSimPhaseInfo_H
#define TSimPhaseInfo_H

// Includes:
#include <string>
#include <string_view>
#include <vector>

// Eigene Includes:
#include "T3DMovVector.h"
#include "TSimulation/TSimProb.h"
#include "TCustomTime.h"

using namespace std;

// Klassendeklaration:
class TSimPhaseInfo
{
public:

	// Variablen:
	bool HasValidData;													// Flag: true = Daten sind vorhanden
	double UsedNorm;													// verwendete Normierung der Wahrscheinlichkeiten
	double CurrentAttemptPathRatioSum;									// aktuelle Summe des Quotienten aus Sprungversuchen und Sprungmoeglichkeiten (ueber alle MCS)
	unsigned long long CurrentMCSP;										// aktuelle MCSP
	unsigned long long CurrentJumpAttempts;								// aktuelle Sprungversuchsanzahl
	unsigned long long CurrentNonsenseJumpAttempts;						// aktuelle Anzahl an Sprungversuchen mit Ruecksprungwahrscheinlichkeit >= 1 (sind nicht in CurrentJumpAttempts enthalten)
	unsigned long long CurrentOverkillJumpAttempts;						// aktuelle Anzahl an Sprungversuchen mit Hinsprungwahrscheinlichkeit >= 1 (sind nicht in CurrentJumpAttempts enthalten)
	unsigned long long CurrentSiteBlockingCounter;						// aktuelle Anzahl an SiteBlocking, d.h. Zielort schon von Leerstelle besetzt (sind in CurrentJumpAttempts enthalten)
	vector<vector<vector<vector<int>>>> CurrentLattice;					// aktueller Gitterzustand
	vector<vector<vector<vector<T3DMovVector>>>> CurrentMovLattice;		// aktuelle Atomverschiebungsvektoren
	vector<TSimProb> CurrentAttemptProbList;							// aktuelle Liste der Sprungversuchswahrscheinlichkeiten 
	vector<TSimProb> CurrentAcceptedProbList;							// aktuelle Liste der Wahrscheinlichkeiten der akzeptierten Spruenge
	TCustomTime CurrentRunTime;											// aktuelle Laufzeit

	// Member functions:
	void Clear();																			// Alle Daten auf Standardwerte setzen
	int SaveToStream(ostream& output, int offset, string_view start_key, string_view end_key);						// Simulationsphase in einen Stream schreiben
	int LoadFromStream(istream& input, int lattice_size, int stack_size, int movstack_size, string_view end_key);	// Simulationsphase aus einem Stream laden

	TSimPhaseInfo();				// Constructor
	~TSimPhaseInfo();				// Destructor

};

#endif