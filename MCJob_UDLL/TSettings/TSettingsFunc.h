// **************************************************************** //
//																	//
//	Klasse: TSettingsFunc	(TSettings Layer 1)						//
//	Autor: Philipp Hein												//
//	Datum: 16.10.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Job-Spezifikationen					//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSettingsFunc_H
#define TSettingsFunc_H

// Includes:
#include <string>

// Eigene Includes:
#include "TSettings/TSettingsBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSettingsFunc: public TSettingsBase {
	// Member functions
	public:
		// NON-PUBLISHED
		int GetDopandCounts(vector<long long> *o_dopcounts);	// Dopandenanzahlen ausgeben
		bool IfWriteCheckpoint();								// Ausgeben, ob Checkpoint geschrieben werden soll
		bool IfLoadCheckpoint();								// Ausgeben, ob Checkpoint geladen werden soll
		int GetShortSummaryDesc(string i_ValDelimiter, string &o_SummaryDesc);	// Beschreibung der GetShortSummary-Ausgabe ausgeben
		int GetShortSummary(string i_ValDelimiter, string &o_Summary);			// Wichtigste Einstellungen mit ValDelimiter getrennt ausgeben

		TSettingsFunc (TKMCJob * pJob);			// Constructor

	protected:
		~TSettingsFunc ();						// Destructor
};

#endif