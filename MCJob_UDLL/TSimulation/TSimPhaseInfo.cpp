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

// Deklarierte Klasse:
#include "TSimPhaseInfo.h"

// Includes:
#include <sstream>

// Eigene Includes:
#include "GlobalDefinitions.h"
#include "ErrorCodes.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimPhaseInfo::TSimPhaseInfo()
{

	Clear();
}

// Destructor
TSimPhaseInfo::~TSimPhaseInfo()
{

}

// ************************* MEMBER FUNCTIONS****************************** //

// Alle Daten auf Standardwerte setzen
void TSimPhaseInfo::Clear()
{

	HasValidData = false;
	UsedNorm = 1.0;
	CurrentAttemptPathRatioSum = 0.0;
	CurrentMCSP = 0ULL;
	CurrentJumpAttempts = 0ULL;
	CurrentNonsenseJumpAttempts = 0ULL;
	CurrentOverkillJumpAttempts = 0ULL;
	CurrentSiteBlockingCounter = 0ULL;
	CurrentLattice.clear();
	CurrentMovLattice.clear();
	CurrentAttemptProbList.clear();
	CurrentAcceptedProbList.clear();
	CurrentRunTime = TCustomTime();

}

// Simulationsphase in einen Stream schreiben
int TSimPhaseInfo::SaveToStream(ostream& output, int offset, string_view start_key, string_view end_key)
{
	if (HasValidData != true) return KMCERR_READY_NOT_TRUE;

	// Input pruefen
	if (output.fail() == true) return KMCERR_INVALID_INPUT;
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) return KMCERR_INVALID_INPUT_CRIT;
	string s_offset = "";
	if (offset > 0) s_offset = string(offset, ' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int)KMCOUT_TSIMULATION_PHASE_OFFSET > 0) sub_offset = string((int)KMCOUT_TSIMULATION_PHASE_OFFSET, ' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile
	output << s_offset << start_key << endl;

	// Ausgabe der Einzelparameter
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_MCSP << " " << CurrentMCSP << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_JUMPATTEMPTS << " " << CurrentJumpAttempts << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_SITEBLOCKING << " " << CurrentSiteBlockingCounter << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_NONSENSE << " " << CurrentNonsenseJumpAttempts << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_OVERKILL << " " << CurrentOverkillJumpAttempts << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_NORM << " " << UsedNorm << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_TIMESUM << " " << CurrentAttemptPathRatioSum << endl;
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_RUNTIME << " " << CurrentRunTime.ToFullString() << endl;
	output << endl;

	// Ausgabe des Gitters
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_LATTICE_START << endl;
	for (int x = 0; x < (int)CurrentLattice.size(); x++)
	{
		for (int y = 0; y < (int)CurrentLattice[x].size(); y++)
		{
			for (int z = 0; z < (int)CurrentLattice[x][y].size(); z++)
			{
				output << s_offset << sub_offset;
				for (int s = 0; s < (int)CurrentLattice[x][y][z].size(); s++)
				{
					output << CurrentLattice[x][y][z][s] << " ";
				}
				output << endl;
			}
		}
	}
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_LATTICE_END << endl;
	output << endl;

	// Ausgabe der Bewegungsspeicherung
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_MOVLATTICE_START << endl;
	for (int x = 0; x < (int)CurrentMovLattice.size(); x++)
	{
		for (int y = 0; y < (int)CurrentMovLattice[x].size(); y++)
		{
			for (int z = 0; z < (int)CurrentMovLattice[x][y].size(); z++)
			{
				for (int s = 0; s < (int)CurrentMovLattice[x][y][z].size(); s++)
				{
					output << s_offset << sub_offset << CurrentMovLattice[x][y][z][s].jcount << " ";
					output << CurrentMovLattice[x][y][z][s].x << " ";
					output << CurrentMovLattice[x][y][z][s].y << " ";
					output << CurrentMovLattice[x][y][z][s].z << endl;
				}
			}
		}
	}
	output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_MOVLATTICE_END << endl;

	// Ausgabe der Sprungversuchswahrscheinlichkeiten
	if (CurrentAttemptProbList.size() != 0)
	{
		bool t_hasvalues = false;
		for (int i = 0; i < (int)CurrentAttemptProbList.size(); i++)
		{
			if (CurrentAttemptProbList[i].count != 0ULL)
			{
				t_hasvalues = true;
				break;
			}
		}
		if (t_hasvalues == true)
		{
			output << endl;
			output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_START << endl;
			for (int i = 0; i < (int)CurrentAttemptProbList.size(); i++)
			{
				if (CurrentAttemptProbList[i].count == 0ULL) break;
				output << s_offset << sub_offset << CurrentAttemptProbList[i].prob << " " << CurrentAttemptProbList[i].count << endl;
			}
			output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_END << endl;
		}
	}

	// Ausgabe der Sprungversuchswahrscheinlichkeiten
	if (CurrentAcceptedProbList.size() != 0)
	{
		bool t_hasvalues = false;
		for (int i = 0; i < (int)CurrentAcceptedProbList.size(); i++)
		{
			if (CurrentAcceptedProbList[i].count != 0ULL)
			{
				t_hasvalues = true;
				break;
			}
		}
		if (t_hasvalues == true)
		{
			output << endl;
			output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_MCSPROB_START << endl;
			for (int i = 0; i < (int)CurrentAcceptedProbList.size(); i++)
			{
				if (CurrentAcceptedProbList[i].count == 0ULL) break;
				output << s_offset << sub_offset << CurrentAcceptedProbList[i].prob << " " << CurrentAcceptedProbList[i].count << endl;
			}
			output << s_offset << sub_offset << KMCOUT_TSIMPHASEINFO_MCSPROB_END << endl;
		}
	}

	// Ausgabe der Endzeile
	output << s_offset << end_key << endl;

	if (output.fail() == true) return KMCERR_OBJECT_NOT_READY;
	return KMCERR_OK;
}

// Simulationsphase aus einem Stream laden
int TSimPhaseInfo::LoadFromStream(istream& input, int lattice_size, int stack_size, int movstack_size, string_view end_key)
{
	if (HasValidData != false) return KMCERR_READY_NOT_FALSE;

	// Input pruefen
	if (input.good() == false) return KMCERR_OBJECT_NOT_READY;

	// Inputparameter setzen
	input >> skipws;

	// Temporaere Objekte deklarieren
	double t_norm = 0.0;
	double t_timesum = 0.0;
	unsigned long long t_mcsp = 0ULL;
	unsigned long long t_jumpattempts = 0ULL;
	unsigned long long t_siteblocking = 0ULL;
	unsigned long long t_nonsenseattempts = 0ULL;
	unsigned long long t_overkillattempts = 0ULL;
	vector<int> t_lin_lattice;
	vector<T3DMovVector> t_lin_movlattice;
	vector<TSimProb> t_attemptprobs;
	vector<TSimProb> t_acceptedprobs;
	TCustomTime t_runtime;

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

		if (s_temp == KMCOUT_TSIMPHASEINFO_MCSP)
		{
			if ((linestream >> t_mcsp).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_JUMPATTEMPTS)
		{
			if ((linestream >> t_jumpattempts).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_SITEBLOCKING)
		{
			if ((linestream >> t_siteblocking).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_NONSENSE)
		{
			if ((linestream >> t_nonsenseattempts).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_OVERKILL)
		{
			if ((linestream >> t_overkillattempts).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_NORM)
		{
			if ((linestream >> t_norm).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_TIMESUM)
		{
			if ((linestream >> t_timesum).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_RUNTIME)
		{
			if ((linestream >> t_runtime.year).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "y,"
				if_failed = true;
				break;
			}
			if ((linestream >> t_runtime.day).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "d,"
				if_failed = true;
				break;
			}
			if ((linestream >> t_runtime.hour).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "h,"
				if_failed = true;
				break;
			}
			if ((linestream >> t_runtime.min).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "m,"
				if_failed = true;
				break;
			}
			if ((linestream >> t_runtime.sec).fail() == true)
			{
				if_failed = true;
				break;
			}
			t_runtime.CheckOverflow();
			if (t_runtime.year < 0)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_LATTICE_START)
		{
			if ((input >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			while (s_temp != KMCOUT_TSIMPHASEINFO_LATTICE_END)
			{
				t_lin_lattice.push_back(-1);
				if ((size_t)t_lin_lattice.size() > size_t(lattice_size) * size_t(lattice_size) * size_t(lattice_size) * size_t(stack_size))
				{
					if_failed = true;
					break;
				}
				if (ConvStrToInt(s_temp, t_lin_lattice.back()) == false)
				{
					if_failed = true;
					break;
				}
				if ((t_lin_lattice.back() < 0) || (t_lin_lattice.back() > KMCLIM_MAX_ELEMENTS))
				{
					if_failed = true;
					break;
				}
				if ((input >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
			}
			if (if_failed == true) break;
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_MOVLATTICE_START)
		{
			if ((input >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			while (s_temp != KMCOUT_TSIMPHASEINFO_MOVLATTICE_END)
			{
				t_lin_movlattice.push_back(T3DMovVector());
				if ((size_t)t_lin_movlattice.size() > size_t(lattice_size) * size_t(lattice_size) * size_t(lattice_size) * size_t(movstack_size))
				{
					if_failed = true;
					break;
				}
				if (ConvStrToUnsignedLongLong(s_temp, t_lin_movlattice.back().jcount) == false)
				{
					if_failed = true;
					break;
				}
				if ((input >> t_lin_movlattice.back().x).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((input >> t_lin_movlattice.back().y).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((input >> t_lin_movlattice.back().z).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((input >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
			}
			if (if_failed == true) break;
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_START)
		{
			if ((input >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			while (s_temp != KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_END)
			{
				t_attemptprobs.push_back(TSimProb());
				if (((size_t)t_attemptprobs.size() > (size_t)KMCVAL_TSETTINGS_MAX_PRERECORDANZ) &&
					((size_t)t_attemptprobs.size() > (size_t)KMCVAL_TSETTINGS_MAX_NORMRECORDANZ) &&
					((size_t)t_attemptprobs.size() > (size_t)KMCVAL_TSETTINGS_MAX_RECORDANZ))
				{
					if_failed = true;
					break;
				}
				if (ConvStrToDouble(s_temp, t_attemptprobs.back().prob) == false)
				{
					if_failed = true;
					break;
				}
				if (t_attemptprobs.back().prob <= 0.0)
				{
					if_failed = true;
					break;
				}
				if ((int)t_attemptprobs.size() > 1)
				{
					if (t_attemptprobs.back().prob >= KMCVAR_EQTHRESHOLD_PROBMINUS * t_attemptprobs[t_attemptprobs.size() - 2].prob)
					{
						if_failed = true;
						break;
					}
				}
				if ((input >> t_attemptprobs.back().count).fail() == true)
				{
					if_failed = true;
					break;
				}
				if (t_attemptprobs.back().count == 0ULL)
				{
					if_failed = true;
					break;
				}
				if ((input >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
			}
			if (if_failed == true) break;
		}
		else if (s_temp == KMCOUT_TSIMPHASEINFO_MCSPROB_START)
		{
			if ((input >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			while (s_temp != KMCOUT_TSIMPHASEINFO_MCSPROB_END)
			{
				t_acceptedprobs.push_back(TSimProb());
				if (((size_t)t_acceptedprobs.size() > (size_t)KMCVAL_TSETTINGS_MAX_PRERECORDANZ) &&
					((size_t)t_acceptedprobs.size() > (size_t)KMCVAL_TSETTINGS_MAX_NORMRECORDANZ) &&
					((size_t)t_acceptedprobs.size() > (size_t)KMCVAL_TSETTINGS_MAX_RECORDANZ))
				{
					if_failed = true;
					break;
				}
				if (ConvStrToDouble(s_temp, t_acceptedprobs.back().prob) == false)
				{
					if_failed = true;
					break;
				}
				if (t_acceptedprobs.back().prob <= 0.0)
				{
					if_failed = true;
					break;
				}
				if ((int)t_acceptedprobs.size() > 1)
				{
					if (t_acceptedprobs.back().prob >= KMCVAR_EQTHRESHOLD_PROBMINUS * t_acceptedprobs[t_acceptedprobs.size() - 2].prob)
					{
						if_failed = true;
						break;
					}
				}
				if ((input >> t_acceptedprobs.back().count).fail() == true)
				{
					if_failed = true;
					break;
				}
				if (t_acceptedprobs.back().count == 0ULL)
				{
					if_failed = true;
					break;
				}
				if ((input >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
			}
			if (if_failed == true) break;
		}
		else if (s_temp == end_key) break;
	}
	if ((size_t)t_lin_lattice.size() != size_t(lattice_size) * size_t(lattice_size) * size_t(lattice_size) * size_t(stack_size)) if_failed = true;
	if ((size_t)t_lin_movlattice.size() != size_t(lattice_size) * size_t(lattice_size) * size_t(lattice_size) * size_t(movstack_size)) if_failed = true;
	if (t_norm <= 0.0) if_failed = true;
	if (t_timesum <= 0.0) if_failed = true;
	if (if_failed == true) return KMCERR_INVALID_FILE_FORMAT;

	// Daten uebertragen (HasValidData = true erst nach Validierung in TSimulationBase)
	UsedNorm = t_norm;
	CurrentAttemptPathRatioSum = t_timesum;
	CurrentMCSP = t_mcsp;
	CurrentJumpAttempts = t_jumpattempts;
	CurrentNonsenseJumpAttempts = t_nonsenseattempts;
	CurrentOverkillJumpAttempts = t_overkillattempts;
	CurrentSiteBlockingCounter = t_siteblocking;
	CurrentRunTime = t_runtime;
	CurrentLattice = vector<vector<vector<vector<int>>>>(lattice_size, vector<vector<vector<int>>>(lattice_size,
		vector<vector<int>>(lattice_size, vector<int>(stack_size, -1))));
	for (int x = 0; x < lattice_size; x++)
	{
		for (int y = 0; y < lattice_size; y++)
		{
			for (int z = 0; z < lattice_size; z++)
			{
				for (int s = 0; s < stack_size; s++)
				{
					CurrentLattice[x][y][z][s] = t_lin_lattice[(((size_t)lattice_size * x + y) * lattice_size + z) * stack_size + s];
				}
			}
		}
	}
	CurrentMovLattice = vector<vector<vector<vector<T3DMovVector>>>>(lattice_size, vector<vector<vector<T3DMovVector>>>(lattice_size,
		vector<vector<T3DMovVector>>(lattice_size, vector<T3DMovVector>(movstack_size, T3DMovVector()))));
	for (int x = 0; x < lattice_size; x++)
	{
		for (int y = 0; y < lattice_size; y++)
		{
			for (int z = 0; z < lattice_size; z++)
			{
				for (int s = 0; s < movstack_size; s++)
				{
					CurrentMovLattice[x][y][z][s] = t_lin_movlattice[(((size_t)lattice_size * x + y) * lattice_size + z) * movstack_size + s];
				}
			}
		}
	}
	if (t_attemptprobs.size() != 0)
	{
		CurrentAttemptProbList = t_attemptprobs;
	}
	else
	{
		CurrentAttemptProbList.clear();
	}
	if (t_acceptedprobs.size() != 0)
	{
		CurrentAcceptedProbList = t_acceptedprobs;
	}
	else
	{
		CurrentAcceptedProbList.clear();
	}

	return KMCERR_OK;
}