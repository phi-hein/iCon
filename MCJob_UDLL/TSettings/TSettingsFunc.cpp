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

// Deklarierte Klasse:
#include "TSettings/TSettingsFunc.h"

// Includes:
#include <sstream>

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "TStructure/TStructure.h"
#include "GlobalDefinitions.h"
#include "ErrorCodes.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSettingsFunc::TSettingsFunc(TKMCJob* pJob) : TSettingsBase(pJob)
{

}

// Destructor
TSettingsFunc::~TSettingsFunc()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Dopandenanzahlen ausgeben
int TSettingsFunc::GetDopandCounts(vector<long long>* o_dopcounts)
{

	*o_dopcounts = DopandAnz;

	return KMCERR_OK;
}

// Ausgeben, ob Checkpoint geschrieben werden soll
bool TSettingsFunc::IfWriteCheckpoint()
{

	return WriteCheckpoint;
}

// Ausgeben, ob Checkpoint geladen werden soll
bool TSettingsFunc::IfLoadCheckpoint()
{

	return LoadCheckpoint;
}

// Beschreibung der GetShortSummary-Ausgabe ausgeben
int TSettingsFunc::GetShortSummaryDesc(string i_ValDelimiter, string& o_SummaryDesc)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	int ErrorCode = KMCERR_OK;

	// Leerstellenbeschreibung ermitteln
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TSettingsFunc::GetShortSummaryDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == NULL)
	{
		cout << "Critical Error: m_Structure is null pointer (in TSettingsFunc::GetShortSummaryDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false)
	{
		cout << "Critical Error: TStructure not ready (in TSettingsFunc::GetShortSummaryDesc)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	string t_vacdesc = "";
	ErrorCode = m_Job->m_Structure->GetVacDopingDesc(t_vacdesc);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Dotierungsbeschreibungen ermitteln
	vector<string> t_dopdescs;
	if ((int)DopandConc.size() != 0)
	{
		string t_dopdesc = "";
		for (int i = 0; i < (int)DopandConc.size(); i++)
		{
			ErrorCode = m_Job->m_Structure->GetDopingDesc(i, t_dopdesc);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			t_dopdescs.push_back(t_dopdesc);
		}
	}

	// Einstellungsbeschreibung schreiben
	stringstream t_result;
	t_result << "T [K]" << i_ValDelimiter;
	t_result << "f [Hz]" << i_ValDelimiter;
	t_result << "Ex [V/cm]" << i_ValDelimiter << "Ey [V/cm]" << i_ValDelimiter << "Ez [V/cm]" << i_ValDelimiter;
	t_result << "Lattice size" << i_ValDelimiter;
	t_result << "Additional vac." << i_ValDelimiter;
	t_result << "Vol. conc. of mov. species [cm^-3]" << i_ValDelimiter;
	t_result << "x(" << t_vacdesc << ")" << i_ValDelimiter;
	t_result << "N(" << t_vacdesc << ")";
	if ((int)t_dopdescs.size() != 0)
	{
		for (int i = 0; i < (int)t_dopdescs.size(); i++)
		{
			t_result << i_ValDelimiter << "x(" << t_dopdescs[i] << ")";
			t_result << i_ValDelimiter << "N(" << t_dopdescs[i] << ")";
		}
	}
	if (DoPrerun == true)
	{
		t_result << i_ValDelimiter << "Prerun-MCSP";
	}
	if (DoDynNorm == true)
	{
		t_result << i_ValDelimiter << "Dyn. Norm. Jump Attempts";
	}
	t_result.flush();
	o_SummaryDesc = t_result.str();

	return KMCERR_OK;
}

// Wichtigste Einstellungen mit ValDelimiter getrennt ausgeben
int TSettingsFunc::GetShortSummary(string i_ValDelimiter, string& o_Summary)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	// Einstellungen schreiben
	stringstream t_result;
	t_result.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
	t_result << Temperature << i_ValDelimiter;
	t_result << AttemptFrequency << i_ValDelimiter;
	t_result << EField.x << i_ValDelimiter << EField.y << i_ValDelimiter << EField.z << i_ValDelimiter;
	t_result << LatticeSize << i_ValDelimiter;
	t_result << AdditionalVacAnz << i_ValDelimiter;
	t_result << MovVolConc << i_ValDelimiter;
	t_result << TotalVacConc << i_ValDelimiter;
	t_result << TotalVacAnz;
	if ((int)DopandConc.size() != 0)
	{
		for (int i = 0; i < (int)DopandConc.size(); i++)
		{
			t_result << i_ValDelimiter << DopandConc[i];
			t_result << i_ValDelimiter << DopandAnz[i];
		}
	}
	if (DoPrerun == true)
	{
		t_result << i_ValDelimiter << PreMCSP;
	}
	if (DoDynNorm == true)
	{
		t_result << i_ValDelimiter << DynAttemptAnz;
	}
	t_result.flush();
	o_Summary = t_result.str();

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //