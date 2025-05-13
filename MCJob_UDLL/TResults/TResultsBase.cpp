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

// Deklarierte Klasse:
#include "TResultsBase.h"

// Includes:
#include <iomanip>
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TSimulation/TSimulation.h"
#include "TSimulation/TSimPhaseInfo.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TResultsBase::TResultsBase(TKMCJob* pJob)
{
	m_Job = pJob;

	Ready = false;

	MCSP = 0ULL;
	JumpAttempts = 0ULL;
	NonsenseJumpAttempts = 0ULL;
	OverkillJumpAttempts = 0ULL;
	SiteBlockingCounter = 0ULL;
	Normalization = 0.0;
	SimulatedTime = 0.0;
	Conductivity = 0.0;

	MovMeanJumpCount = 0.0;
	MovDispParallel = 0.0;
	MovDispPerpendicular = 0.0;
	MovMeanDisp = 0.0;
	MovMeanSquaredDisp = 0.0;

	VacMeanJumpCount = 0.0;
	VacDispParallel = 0.0;
	VacDispPerpendicular = 0.0;
	VacMeanDisp = 0.0;
	VacMeanSquaredDisp = 0.0;
}

// Destructor
TResultsBase::~TResultsBase()
{
	m_Job = 0;
}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Rueckgabe von Ready
bool TResultsBase::IfReady()
{
	return Ready;
}

// Rueckgabe von Ready und genuegend MCSP
bool TResultsBase::IfCompleted(long long required_MCSP)
{
	if (Ready == false) return false;

	return (MCSP >= static_cast<unsigned long long>(required_MCSP));
}

// Ergebnisse aus abgeschlossener Simulation laden
int TResultsBase::MakeResults()
{
	Ready = false;

	// Pruefen, ob Simulation abgeschlossen ist
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TResultsBase::MakeResults)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Simulation == NULL)
	{
		cout << "Critical Error: m_Simulation is null pointer (in TResultsBase::MakeResults)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Simulation->IfReady() == false)
	{
		cout << "Critical Error: TSimulation not ready (in TResultsBase::MakeResults)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Simulation->IfCompleted() == false)
	{
		cout << "Critical Error: TSimulation not complete (in TResultsBase::MakeResults)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	int ErrorCode = KMCERR_OK;

	// Daten aus TSimulation extrahieren
	vector<double> t_results;
	ErrorCode = m_Job->m_Simulation->GetMovementResults(&t_results);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	// Konvention fuer t_results:
	// fuer bewegliche Spezies:
	// [0]          <jcount>                = mittlere Sprunganzahl
	// [1, 2, 3]    <x>, <y>, <z>           = mittlerer Verschiebungsvektor in cm
	// [4]          <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	// [5]          <x*x + y*y + z*z>       = mittlere quadratische Verschiebung in cm^2
	// [6, 7, 8]    sum(x), sum(y), sum(z)  = Schwerpunktverschiebungsvektor in cm
	// fuer Leerstellen:
	// [9]          <jcount>                = mittlere Sprunganzahl
	// [10, 11, 12] <x>, <y>, <z>           = mittlerer Verschiebungsvektor in cm
	// [13]         <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	// [14]         <x*x + y*y + z*z>       = mittlere quadratische Verschiebung in cm^2
	// [15, 16, 17] sum(x), sum(y), sum(z)  = Schwerpunktverschiebungsvektor in cm
	// fuer bewegliche Spezies:
	// [18]			Komponente des Verschiebungsvektors parallel zum E-Feld in cm
	// [19]			Komponente des Verschiebungsvektors senkrecht zum E-Feld in cm
	// fuer Leerstellen:
	// [20]			Komponente des Verschiebungsvektors parallel zum E-Feld in cm
	// [21]			Komponente des Verschiebungsvektors senkrecht zum E-Feld in cm
	// allgemeine Ergebnisse:
	// [22]			Simulierte Zeitspanne in s
	// [23]			Leitfaehigkeit in V/cm
	TSimPhaseInfo t_mainphase;
	ErrorCode = m_Job->m_Simulation->GetMainSimStatus(t_mainphase);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (t_mainphase.HasValidData == false)
	{
		cout << "Critical Error: Invalid simulation data (in TResultsBase::MakeResults)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Daten einlesen
	MCSP = t_mainphase.CurrentMCSP;
	JumpAttempts = t_mainphase.CurrentJumpAttempts;
	NonsenseJumpAttempts = t_mainphase.CurrentNonsenseJumpAttempts;
	OverkillJumpAttempts = t_mainphase.CurrentOverkillJumpAttempts;
	SiteBlockingCounter = t_mainphase.CurrentSiteBlockingCounter;
	Normalization = t_mainphase.UsedNorm;
	Runtime = t_mainphase.CurrentRunTime;
	SimulatedTime = t_results[22];
	Conductivity = t_results[23];
	MovMeanJumpCount = t_results[0];
	MovMeanDispVec.Set(t_results[1], t_results[2], t_results[3]);
	MovDispParallel = t_results[18];
	MovDispPerpendicular = t_results[19];
	MovMeanDisp = t_results[4];
	MovMeanSquaredDisp = t_results[5];
	MovComDispVec.Set(t_results[6], t_results[7], t_results[8]);
	VacMeanJumpCount = t_results[9];
	VacMeanDispVec.Set(t_results[10], t_results[11], t_results[12]);
	VacDispParallel = t_results[20];
	VacDispPerpendicular = t_results[21];
	VacMeanDisp = t_results[13];
	VacMeanSquaredDisp = t_results[14];
	VacComDispVec.Set(t_results[15], t_results[16], t_results[17]);

	Ready = true;
	return KMCERR_OK;
}

// Ergebnisse in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
int TResultsBase::SaveToStream(ostream& output, int offset)
{
	if (Ready != true)
	{
		cout << "Critical Error: TResults not ready (in TResultsBase::SaveToStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true)
	{
		cout << "Critical Error: Output stream not ready (in TResultsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET))
	{
		cout << "Critical Error: Offset out of bounds (in TResultsBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset, ' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int)KMCOUT_TRESULTS_OFFSET > 0) sub_offset = string((int)KMCOUT_TRESULTS_OFFSET, ' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der allgemeinen Ergebnisse
	output << s_offset << KMCOUT_TRESULTS_START << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_COND << " " << Conductivity << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_SIMTIME << " " << SimulatedTime << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MCSP << " " << MCSP << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_JUMPATTEMPTS << " " << JumpAttempts << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_SITEBLOCKING << " " << SiteBlockingCounter << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_NONSENSEATTEMPTS << " " << NonsenseJumpAttempts << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_OVERKILLATTEMPTS << " " << OverkillJumpAttempts << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_NORMALIZATION << " " << Normalization << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_RUNTIME << " " << Runtime.ToFullString() << endl;
	output << endl;

	// Ausgabe der Bewegung der Atome der beweglichen Spezies
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_MEANJUMPCOUNT << " " << MovMeanJumpCount << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_MEANDISPVEC << " ( " << MovMeanDispVec.x << " " << MovMeanDispVec.y << " " << MovMeanDispVec.z << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_DISPPARALLEL << " " << MovDispParallel << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_DISPPERPENDICULAR << " " << MovDispPerpendicular << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_MEANDISP << " " << MovMeanDisp << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_MEANSQUAREDDISP << " " << MovMeanSquaredDisp << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_MOV_COMDISPVEC << " ( " << MovComDispVec.x << " " << MovComDispVec.y << " " << MovComDispVec.z << " )" << endl;
	output << endl;

	// Ausgabe der Bewegung der Leerstellen
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_MEANJUMPCOUNT << " " << VacMeanJumpCount << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_MEANDISPVEC << " ( " << VacMeanDispVec.x << " " << VacMeanDispVec.y << " " << VacMeanDispVec.z << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_DISPPARALLEL << " " << VacDispParallel << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_DISPPERPENDICULAR << " " << VacDispPerpendicular << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_MEANDISP << " " << VacMeanDisp << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_MEANSQUAREDDISP << " " << VacMeanSquaredDisp << endl;
	output << s_offset << sub_offset << KMCOUT_TRESULTS_VAC_COMDISPVEC << " ( " << VacComDispVec.x << " " << VacComDispVec.y << " " << VacComDispVec.z << " )" << endl;

	output << s_offset << KMCOUT_TRESULTS_END << endl;

	if (output.fail() == true)
	{
		cout << "Critical Error: Output stream failure (in TResultsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Ergebnisse aus einem Stream laden
int TResultsBase::LoadFromStream(istream& input)
{

	// Input pruefen
	if (input.good() == false)
	{
		cout << "Critical Error: Input stream not ready (in TResultsBase::LoadFromStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	unsigned long long i_MCSP = MCSP;
	unsigned long long i_JumpAttempts = JumpAttempts;
	unsigned long long i_NonsenseJumpAttempts = NonsenseJumpAttempts;
	unsigned long long i_OverkillJumpAttempts = OverkillJumpAttempts;
	unsigned long long i_SiteBlockingCounter = SiteBlockingCounter;
	double i_Normalization = Normalization;
	TCustomTime i_Runtime = Runtime;
	double i_SimulatedTime = SimulatedTime;
	double i_Conductivity = Conductivity;

	double i_MovMeanJumpCount = MovMeanJumpCount;
	T3DVector i_MovMeanDispVec = MovMeanDispVec;
	double i_MovDispParallel = MovDispParallel;
	double i_MovDispPerpendicular = MovDispPerpendicular;
	double i_MovMeanDisp = MovMeanDisp;
	double i_MovMeanSquaredDisp = MovMeanSquaredDisp;
	T3DVector i_MovComDispVec = MovComDispVec;

	double i_VacMeanJumpCount = VacMeanJumpCount;
	T3DVector i_VacMeanDispVec = VacMeanDispVec;
	double i_VacDispParallel = VacDispParallel;
	double i_VacDispPerpendicular = VacDispPerpendicular;
	double i_VacMeanDisp = VacMeanDisp;
	double i_VacMeanSquaredDisp = VacMeanSquaredDisp;
	T3DVector i_VacComDispVec = VacComDispVec;

	string line = "";
	bool if_failed = false;
	string s_temp = "";
	while (input.good() == true)
	{
		// Zeile laden
		if (getline(input, line).fail() == true)
		{
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream(line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TRESULTS_COND)
		{
			if ((linestream >> i_Conductivity).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_SIMTIME)
		{
			if ((linestream >> i_SimulatedTime).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_SimulatedTime < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MCSP)
		{
			if ((linestream >> i_MCSP).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_JUMPATTEMPTS)
		{
			if ((linestream >> i_JumpAttempts).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_SITEBLOCKING)
		{
			if ((linestream >> i_SiteBlockingCounter).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_NONSENSEATTEMPTS)
		{
			if ((linestream >> i_NonsenseJumpAttempts).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_OVERKILLATTEMPTS)
		{
			if ((linestream >> i_OverkillJumpAttempts).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_NORMALIZATION)
		{
			if ((linestream >> i_Normalization).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_Normalization < 1.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_RUNTIME)
		{
			if ((linestream >> i_Runtime.year).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "y,"
				if_failed = true;
				break;
			}
			if ((linestream >> i_Runtime.day).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "d,"
				if_failed = true;
				break;
			}
			if ((linestream >> i_Runtime.hour).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "h,"
				if_failed = true;
				break;
			}
			if ((linestream >> i_Runtime.min).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "m,"
				if_failed = true;
				break;
			}
			if ((linestream >> i_Runtime.sec).fail() == true)
			{
				if_failed = true;
				break;
			}
			i_Runtime.CheckOverflow();
			if (i_Runtime.year < 0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_MEANJUMPCOUNT)
		{
			if ((linestream >> i_MovMeanJumpCount).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_MovMeanJumpCount < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_MEANDISPVEC)
		{
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			i_MovMeanDispVec.Set(0.0, 0.0, 0.0);
			if ((linestream >> i_MovMeanDispVec.x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_MovMeanDispVec.y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_MovMeanDispVec.z).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_DISPPARALLEL)
		{
			if ((linestream >> i_MovDispParallel).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_DISPPERPENDICULAR)
		{
			if ((linestream >> i_MovDispPerpendicular).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_MEANDISP)
		{
			if ((linestream >> i_MovMeanDisp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_MovMeanDisp < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_MEANSQUAREDDISP)
		{
			if ((linestream >> i_MovMeanSquaredDisp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_MovMeanSquaredDisp < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_MOV_COMDISPVEC)
		{
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			i_MovComDispVec.Set(0.0, 0.0, 0.0);
			if ((linestream >> i_MovComDispVec.x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_MovComDispVec.y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_MovComDispVec.z).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_MEANJUMPCOUNT)
		{
			if ((linestream >> i_VacMeanJumpCount).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_VacMeanJumpCount < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_MEANDISPVEC)
		{
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			i_VacMeanDispVec.Set(0.0, 0.0, 0.0);
			if ((linestream >> i_VacMeanDispVec.x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_VacMeanDispVec.y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_VacMeanDispVec.z).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_DISPPARALLEL)
		{
			if ((linestream >> i_VacDispParallel).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_DISPPERPENDICULAR)
		{
			if ((linestream >> i_VacDispPerpendicular).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_MEANDISP)
		{
			if ((linestream >> i_VacMeanDisp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_VacMeanDisp < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_MEANSQUAREDDISP)
		{
			if ((linestream >> i_VacMeanSquaredDisp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_VacMeanSquaredDisp < 0.0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_VAC_COMDISPVEC)
		{
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			i_VacComDispVec.Set(0.0, 0.0, 0.0);
			if ((linestream >> i_VacComDispVec.x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_VacComDispVec.y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_VacComDispVec.z).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TRESULTS_END) break;
	}
	if (if_failed == true)
	{
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Daten uebertragen
	MCSP = i_MCSP;
	JumpAttempts = i_JumpAttempts;
	NonsenseJumpAttempts = i_NonsenseJumpAttempts;
	OverkillJumpAttempts = i_OverkillJumpAttempts;
	SiteBlockingCounter = i_SiteBlockingCounter;
	Normalization = i_Normalization;
	Runtime = i_Runtime;
	SimulatedTime = i_SimulatedTime;
	Conductivity = i_Conductivity;

	MovMeanJumpCount = i_MovMeanJumpCount;
	MovMeanDispVec = i_MovMeanDispVec;
	MovDispParallel = i_MovDispParallel;
	MovDispPerpendicular = i_MovDispPerpendicular;
	MovMeanDisp = i_MovMeanDisp;
	MovMeanSquaredDisp = i_MovMeanSquaredDisp;
	MovComDispVec = i_MovComDispVec;

	VacMeanJumpCount = i_VacMeanJumpCount;
	VacMeanDispVec = i_VacMeanDispVec;
	VacDispParallel = i_VacDispParallel;
	VacDispPerpendicular = i_VacDispPerpendicular;
	VacMeanDisp = i_VacMeanDisp;
	VacMeanSquaredDisp = i_VacMeanSquaredDisp;
	VacComDispVec = i_VacComDispVec;

	Ready = true;
	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //