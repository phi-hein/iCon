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

// Deklarierte Klasse:
#include "TSettings/TSettingsOut.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "TStructure/TStructure.h"
#include "ErrorCodes.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSettingsOut::TSettingsOut(TKMCJob* pJob) : TSettingsFunc(pJob)
{

}

// Destructor
TSettingsOut::~TSettingsOut()
{

}

// **************************** PUBLISHED ********************************* //

// Temperatur ausgeben
int TSettingsOut::GetTemperature(double& o_Temperature)
{

	o_Temperature = Temperature;

	return KMCERR_OK;
}

// Sprungfrequenz ausgeben
int TSettingsOut::GetAttemptFrequency(double& o_Frequency)
{

	o_Frequency = AttemptFrequency;

	return KMCERR_OK;
}

// Checkpoint-Flags ausgeben
int TSettingsOut::GetCheckpointFlags(bool& o_WriteCheckpoint, bool& o_LoadCheckpoint)
{

	o_WriteCheckpoint = WriteCheckpoint;
	o_LoadCheckpoint = LoadCheckpoint;

	return KMCERR_OK;
}

// E-Feld-Richtung und maximaler Beitrag [kT] zur Aktivierungsenergie ausgeben
int TSettingsOut::GetEFieldSettings(double& o_EFieldDirX, double& o_EFieldDirY, double& o_EFieldDirZ, double& o_MaxEField)
{

	o_EFieldDirX = EFieldDirection.x;
	o_EFieldDirY = EFieldDirection.y;
	o_EFieldDirZ = EFieldDirection.z;
	o_MaxEField = EFieldMagnitude;

	return KMCERR_OK;
}

// Berechnetes E-Feld ausgeben
int TSettingsOut::GetEField(double& o_EFieldX, double& o_EFieldY, double& o_EFieldZ)
{

	o_EFieldX = EField.x;
	o_EFieldY = EField.y;
	o_EFieldZ = EField.z;

	return KMCERR_OK;
}

// Gittergröße ausgeben
int TSettingsOut::GetLatticeSize(int& o_LatticeSize)
{

	o_LatticeSize = LatticeSize;

	return KMCERR_OK;
}

// Zusätzliche Leerstellen ausgeben
int TSettingsOut::GetAdditionalVacAnz(long long& o_AdditionalVacAnz)
{

	o_AdditionalVacAnz = AdditionalVacAnz;

	return KMCERR_OK;
}

// Leerstellenkonzentration ausgeben
int TSettingsOut::GetTotalVacancyConc(double& o_Conc)
{

	o_Conc = TotalVacConc;

	return KMCERR_OK;
}

// Anzahl an Leerstellen ausgeben
int TSettingsOut::GetTotalVacancyAnz(long long& o_Anz)
{

	o_Anz = TotalVacAnz;

	return KMCERR_OK;
}

// Volumenkonzentration der beweglichen Spezies ausgeben
int TSettingsOut::GetMovVolConc(double& o_MovVolConc)
{

	o_MovVolConc = MovVolConc;

	return KMCERR_OK;
}

// Dotierkonzentration für eine bestimmte Dotierung ausgeben
int TSettingsOut::GetDopandConc(int ID, double& o_Conc)
{

	// Dotierungsanzahl pruefen
	int ErrorCode = SyncDopandVectors();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Input pruefen
	if (ID < 0)
	{
		cout << "Critical Error: Negative vector index (in TSettingsOut::GetDopandConc)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (ID >= (int)DopandConc.size())
	{
		cout << "Critical Error: Index exceeds vector size (in TSettingsOut::GetDopandConc)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	o_Conc = DopandConc[ID];

	return KMCERR_OK;
}

// Anzahl der Dopanden bei einer bestimmten Dotierung ausgeben
int TSettingsOut::GetDopandAnz(int ID, long long& o_Anz)
{

	// Dotierungsanzahl pruefen
	int ErrorCode = SyncDopandVectors();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Input pruefen
	if (ID < 0)
	{
		cout << "Critical Error: Negative vector index (in TSettingsOut::GetDopandAnz)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (ID >= (int)DopandAnz.size())
	{
		cout << "Critical Error: Index exceeds vector size (in TSettingsOut::GetDopandAnz)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	o_Anz = DopandAnz[ID];

	return KMCERR_OK;
}

// KMC-Vorlaufparameter ausgeben
int TSettingsOut::GetPrerunOptions(bool& o_DoPrerun, long long& o_PreMCSP, int& o_PreRecordAnz)
{

	o_DoPrerun = DoPrerun;
	o_PreMCSP = PreMCSP;
	o_PreRecordAnz = PreRecordAnz;

	return KMCERR_OK;
}

// Parameter für dyn. Normierungssimulation ausgeben
int TSettingsOut::GetDynNormParameters(bool& o_DoDynNorm, long long& o_DynNormAttemptAnz, int& o_DynNormRecordAnz, int& o_DynNormNum)
{

	o_DoDynNorm = DoDynNorm;
	o_DynNormAttemptAnz = DynAttemptAnz;
	o_DynNormRecordAnz = DynRecordAnz;
	o_DynNormNum = DynNormNum;

	return KMCERR_OK;
}

// KMC-Hauptsimulationsparameter ausgeben
int TSettingsOut::GetMainKMCOptions(long long& o_MCSP, int& o_MainRecordAnz)
{

	o_MCSP = MCSP;
	o_MainRecordAnz = MainRecordAnz;

	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //