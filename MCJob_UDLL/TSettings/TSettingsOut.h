// **************************************************************** //
//																	//
//	Class: TSettingsOut	(TSettings Layer 2)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the job settings							//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSettingsOut_H
#define TSettingsOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TSettings/TSettingsFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSettingsOut : public TSettingsFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetTemperature(double& o_Temperature);																		// Temperatur ausgeben
	int GetAttemptFrequency(double& o_Frequency);																	// Sprungfrequenz ausgeben
	int GetCheckpointFlags(bool& o_WriteCheckpoint, bool& o_LoadCheckpoint);										// Checkpoint-Flags ausgeben
	int GetEFieldSettings(double& o_EFieldDirX, double& o_EFieldDirY, double& o_EFieldDirZ, double& o_MaxEField);	// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
	int GetEField(double& o_EFieldX, double& o_EFieldY, double& o_EFieldZ);										// Berechnetes E-Feld ausgeben

	int GetLatticeSize(int& o_LatticeSize);									// Gittergröße ausgeben
	int GetAdditionalVacAnz(long long& o_AdditionalVacAnz);					// Zusätzliche Leerstellen ausgeben
	int GetTotalVacancyConc(double& o_Conc);									// Leerstellenkonzentration ausgeben
	int GetTotalVacancyAnz(long long& o_Anz);									// Anzahl an Leerstellen ausgeben
	int GetMovVolConc(double& o_MovVolConc);									// Volumenkonzentration der beweglichen Spezies ausgeben
	int GetDopandConc(int ID, double& o_Conc);									// Dotierkonzentration für eine bestimmte Dotierung ausgeben
	int GetDopandAnz(int ID, long long& o_Anz);								// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben

	int GetPrerunOptions(bool& o_DoPrerun, long long& o_PreMCSP, int& o_PreRecordAnz);												// KMC-Vorlaufparameter ausgeben
	int GetDynNormParameters(bool& o_DoDynNorm, long long& o_DynNormAttemptAnz, int& o_DynNormRecordAnz, int& o_DynNormNum);		// Parameter für dyn. Normierungssimulation ausgeben
	int GetMainKMCOptions(long long& o_MCSP, int& o_MainRecordAnz);																// KMC-Hauptsimulationsparameter ausgeben

	// NON-PUBLISHED
	TSettingsOut(TKMCJob* pJob);		// Constructor

protected:
	~TSettingsOut();					// Destructor
};

#endif