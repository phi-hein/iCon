// **************************************************************** //
//																	//
//	Class: TSettingsFunc	(TSettings Layer 1)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the job settings							//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TSettingsFunc : public TSettingsBase
{
	// Member functions
public:
	// NON-PUBLISHED
	int GetDopandCounts(vector<long long>* o_dopcounts);	// Dopandenanzahlen ausgeben
	bool IfWriteCheckpoint();								// Ausgeben, ob Checkpoint geschrieben werden soll
	bool IfLoadCheckpoint();								// Ausgeben, ob Checkpoint geladen werden soll
	int GetShortSummaryDesc(string i_ValDelimiter, string& o_SummaryDesc);	// Beschreibung der GetShortSummary-Ausgabe ausgeben
	int GetShortSummary(string i_ValDelimiter, string& o_Summary);			// Wichtigste Einstellungen mit ValDelimiter getrennt ausgeben

	TSettingsFunc(TKMCJob* pJob);			// Constructor

protected:
	~TSettingsFunc();						// Destructor
};

#endif