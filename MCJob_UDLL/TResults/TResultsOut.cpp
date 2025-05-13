// **************************************************************** //
//																	//
//	Class: TResultsOut		(TResults Layer 2)						//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the simulation results						//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TResults/TResultsOut.h"

// Includes:
#include <sstream>

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "TStructure/TStructure.h"
#include "TSettings/TSettings.h"
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TResultsOut::TResultsOut(TKMCJob* pJob) : TResultsFunc(pJob)
{

}

// Destructor
TResultsOut::~TResultsOut()
{

}

// **************************** PUBLISHED ********************************* //

// Beschreibung der GetSummary-Ausgabe ausgeben
int TResultsOut::GetSummaryDesc(string i_ValDelimiter, string& o_SummaryDesc)
{
	if (Ready != true)
	{
		o_SummaryDesc = "No results available.";
		return KMCERR_NO_RESULTS;
	}

	int ErrorCode = KMCERR_OK;

	// Dotierungsbeschreibungen ermitteln
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TResultsOut::GetSummaryDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Settings == NULL)
	{
		cout << "Critical Error: m_Settings is null pointer (in TResultsOut::GetSummaryDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Settings->IfReady() == false)
	{
		cout << "Critical Error: TSettings not ready (in TResultsOut::GetSummaryDesc)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	string t_settingssummarydesc = "";
	ErrorCode = m_Job->m_Settings->GetShortSummaryDesc(i_ValDelimiter, t_settingssummarydesc);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Settings desc retrieval failed (in TResultsOut::GetSummaryDesc)" << endl << endl;
		return ErrorCode;
	}

	// Haupteinstellungen und -ergebnisse ausgeben
	stringstream t_result;
	t_result << "Project" << i_ValDelimiter;
	t_result << "Structure" << i_ValDelimiter;
	t_result << t_settingssummarydesc << i_ValDelimiter;
	t_result << "MCSP" << i_ValDelimiter;
	t_result << "Conductivity [S/cm]" << i_ValDelimiter;
	t_result << "Simulated timespan [s]" << i_ValDelimiter;
	t_result << "Jump attempts" << i_ValDelimiter;
	t_result << "Site-Blocking" << i_ValDelimiter;
	t_result << "Endpos. nondef. (Eakt,back<=0)" << i_ValDelimiter;
	t_result << "Startpos. nondef. (Eakt,for<=0)" << i_ValDelimiter;
	t_result << "Normalization" << i_ValDelimiter;
	t_result << "Runtime" << i_ValDelimiter;

	// Mov-Bewegung ausgeben
	t_result << "Mov-<Sprunganzahl>" << i_ValDelimiter;
	t_result << "Mov-<x> [cm]" << i_ValDelimiter << "Mov-<y> [cm]" << i_ValDelimiter << "Mov-<z> [cm]" << i_ValDelimiter;
	t_result << "Mov-Disp.-Comp.(parallel) [cm]" << i_ValDelimiter;
	t_result << "Mov-Disp.-Comp.(perpendicular) [cm]" << i_ValDelimiter;
	t_result << "Mov-<sqrt(x*x+y*y+z*z)> [cm]" << i_ValDelimiter;
	t_result << "Mov-<x*x+y*y+z*z> [cm^2]" << i_ValDelimiter;
	t_result << "Mov-sum(x) [cm]" << i_ValDelimiter << "Mov-sum(y) [cm]" << i_ValDelimiter << "Mov-sum(z) [cm]" << i_ValDelimiter;

	// Vac-Bewegung ausgeben
	t_result << "Vac-<Sprunganzahl>" << i_ValDelimiter;
	t_result << "Vac-<x> [cm]" << i_ValDelimiter << "Vac-<y> [cm]" << i_ValDelimiter << "Vac-<z> [cm]" << i_ValDelimiter;
	t_result << "Vac-Disp.-Comp.(parallel) [cm]" << i_ValDelimiter;
	t_result << "Vac-Disp.-Comp.(perpendicular) [cm]" << i_ValDelimiter;
	t_result << "Vac-<sqrt(x*x+y*y+z*z)> [cm]" << i_ValDelimiter;
	t_result << "Vac-<x*x+y*y+z*z> [cm^2]" << i_ValDelimiter;
	t_result << "Vac-sum(x) [cm]" << i_ValDelimiter << "Vac-sum(y) [cm]" << i_ValDelimiter << "Vac-sum(z) [cm]";

	t_result.flush();
	o_SummaryDesc = t_result.str();

	return KMCERR_OK;
}

// Wichtigste Einstellungen und Ergebnisse mit ValDelimiter getrennt ausgeben
int TResultsOut::GetSummary(string i_ValDelimiter, string& o_Summary)
{
	if (Ready != true)
	{
		o_Summary = "No results available.";
		return KMCERR_NO_RESULTS;
	}

	int ErrorCode = KMCERR_OK;

	// Projektname ermitteln
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TResultsOut::GetSummary)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	string t_projectname = "";
	ErrorCode = m_Job->GetProjectName(t_projectname);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Strukturname ermitteln
	if (m_Job->m_Structure == NULL)
	{
		cout << "Critical Error: m_Structure is null pointer (in TResultsOut::GetSummary)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false)
	{
		cout << "Critical Error: TStructure not ready (in TResultsOut::GetSummary)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	string t_structurename = "";
	ErrorCode = m_Job->m_Structure->GetStructureName(t_structurename);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Einstellungen ermitteln
	if (m_Job->m_Settings == NULL)
	{
		cout << "Critical Error: m_Settings is null pointer (in TResultsOut::GetSummary)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Settings->IfReady() == false)
	{
		cout << "Critical Error: TSettings not ready (in TResultsOut::GetSummary)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	string t_settingssummary = "";
	ErrorCode = m_Job->m_Settings->GetShortSummary(i_ValDelimiter, t_settingssummary);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Settings retrieval failed (in TResultsOut::GetSummary)" << endl << endl;
		return ErrorCode;
	}

	// Haupteinstellungen und -ergebnisse ausgeben
	stringstream t_result;
	t_result.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
	t_result << t_projectname << i_ValDelimiter;
	t_result << t_structurename << i_ValDelimiter;
	t_result << t_settingssummary << i_ValDelimiter;
	t_result << MCSP << i_ValDelimiter;
	t_result << Conductivity << i_ValDelimiter;
	t_result << SimulatedTime << i_ValDelimiter;
	t_result << JumpAttempts << i_ValDelimiter;
	t_result << SiteBlockingCounter << i_ValDelimiter;
	t_result << NonsenseJumpAttempts << i_ValDelimiter;
	t_result << OverkillJumpAttempts << i_ValDelimiter;
	t_result << Normalization << i_ValDelimiter;
	t_result << Runtime.ToString() << i_ValDelimiter;

	// Mov-Bewegung ausgeben
	t_result << MovMeanJumpCount << i_ValDelimiter;
	t_result << MovMeanDispVec.x << i_ValDelimiter << MovMeanDispVec.y << i_ValDelimiter << MovMeanDispVec.z << i_ValDelimiter;
	t_result << MovDispParallel << i_ValDelimiter;
	t_result << MovDispPerpendicular << i_ValDelimiter;
	t_result << MovMeanDisp << i_ValDelimiter;
	t_result << MovMeanSquaredDisp << i_ValDelimiter;
	t_result << MovComDispVec.x << i_ValDelimiter << MovComDispVec.y << i_ValDelimiter << MovComDispVec.z << i_ValDelimiter;

	// Vac-Bewegung ausgeben
	t_result << VacMeanJumpCount << i_ValDelimiter;
	t_result << VacMeanDispVec.x << i_ValDelimiter << VacMeanDispVec.y << i_ValDelimiter << VacMeanDispVec.z << i_ValDelimiter;
	t_result << VacDispParallel << i_ValDelimiter;
	t_result << VacDispPerpendicular << i_ValDelimiter;
	t_result << VacMeanDisp << i_ValDelimiter;
	t_result << VacMeanSquaredDisp << i_ValDelimiter;
	t_result << VacComDispVec.x << i_ValDelimiter << VacComDispVec.y << i_ValDelimiter << VacComDispVec.z;

	t_result.flush();
	o_Summary = t_result.str();

	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //