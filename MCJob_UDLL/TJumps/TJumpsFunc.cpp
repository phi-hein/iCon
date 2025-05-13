// **************************************************************** //
//																	//
//	Class: TJumpsFunc	(TJumps Layer 1)							//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for creating and describing all possible jumps	 		//
//	  for moving species and vacancies in the unit cell				//
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
#include "TJumps/TJumpsFunc.h"

// Includes:
#include<algorithm>

// Eigene Includes:
#include "ErrorCodes.h"
#include "TKMCJob/TKMCJob.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "TSimulation/TSimJump.h"
#include "TSimulation/TSimUniqueJump.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJumpsFunc::TJumpsFunc(TKMCJob* pJob) : TJumpsBase(pJob)
{

}

// Destructor
TJumpsFunc::~TJumpsFunc()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// absolute 4D-Position des Startatoms zurueckgeben (Vektor von (0,0,0,0) zu Startatom)
int TJumpsFunc::GetStartPos(int AtomID, int DirID, T4DLatticeVector& pos)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if ((AtomID < 0) || (AtomID >= (int)Jumps.size())) return KMCERR_INVALID_INPUT_CRIT;
	if ((DirID < 0) || (DirID >= (int)Jumps[AtomID].size())) return KMCERR_INVALID_INPUT_CRIT;

	return Jumps[AtomID][DirID].GetStartPos(pos);
}

// absolute 4D-Position des Zielatoms zurueckgeben (Vektor von (0,0,0,0) zu Zielatom)
int TJumpsFunc::GetDestPos(int AtomID, int DirID, T4DLatticeVector& pos)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if ((AtomID < 0) || (AtomID >= (int)Jumps.size())) return KMCERR_INVALID_INPUT_CRIT;
	if ((DirID < 0) || (DirID >= (int)Jumps[AtomID].size())) return KMCERR_INVALID_INPUT_CRIT;

	return Jumps[AtomID][DirID].GetDestPos(pos);
}

// absolute 4D-Positionen der Umgebungsatome zurueckgeben (Vektoren von (0,0,0,0) zu Umgebungsatomen)
int TJumpsFunc::GetEnvPos(int AtomID, int DirID, vector<T4DLatticeVector>* envpos)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if ((AtomID < 0) || (AtomID >= (int)Jumps.size())) return KMCERR_INVALID_INPUT_CRIT;
	if ((DirID < 0) || (DirID >= (int)Jumps[AtomID].size())) return KMCERR_INVALID_INPUT_CRIT;

	return Jumps[AtomID][DirID].GetEnvPos(envpos);
}

// UniqueJumpID ausgeben
int TJumpsFunc::GetUniqueJumpID(int AtomID, int DirID, int& ID)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if ((AtomID < 0) || (AtomID >= (int)Jumps.size())) return KMCERR_INVALID_INPUT_CRIT;
	if ((DirID < 0) || (DirID >= (int)Jumps[AtomID].size())) return KMCERR_INVALID_INPUT_CRIT;

	return Jumps[AtomID][DirID].GetUniqueJumpID(ID);
}

// Beispielumgebung mit einer bestimmten UniqueJumpID finden
int TJumpsFunc::FindUniqueJumpID(int ID, int& AtomID, int& DirID)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	// Sprungumgebung finden
	AtomID = -1;
	DirID = -1;
	int SearchID = -1;
	int ErrorCode = KMCERR_OK;
	for (int i = 0; i < (int)Jumps.size(); i++)
	{
		for (int j = 0; j < (int)Jumps[i].size(); j++)
		{
			ErrorCode = Jumps[i][j].GetUniqueJumpID(SearchID);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			if (SearchID == ID)
			{
				AtomID = i;
				DirID = j;
				break;
			}
		}
		if (SearchID == ID) break;
	}

	return KMCERR_OK;
}

// Skalarprodukt [V] von E-Feld-Vektor [V/cm] und halbem Sprungvektor [cm] ausgeben
int TJumpsFunc::GetEFieldProjection(int AtomID, int DirID, T3DVector& efield, double& proj)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if ((AtomID < 0) || (AtomID >= (int)Jumps.size())) return KMCERR_INVALID_INPUT_CRIT;
	if ((DirID < 0) || (DirID >= (int)Jumps[AtomID].size())) return KMCERR_INVALID_INPUT_CRIT;

	return Jumps[AtomID][DirID].GetEFieldProjection(efield, proj);
}

// Betragsmaessig maximales Skalarprodukt [V] von E-Feld-Vektor [V/cm] und allen halben Sprungvektoren [cm] ausgeben
int TJumpsFunc::GetMaxEFieldProjection(T3DVector& efield, double& max_proj)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	double t_proj = 0;
	double t_max_proj = 0;
	int ErrorCode = KMCERR_OK;
	for (int i = 0; i < (int)Jumps.size(); i++)
	{
		for (int j = 0; j < (int)Jumps[i].size(); j++)
		{
			ErrorCode = Jumps[i][j].GetEFieldProjection(efield, t_proj);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			if (abs(t_proj) > t_max_proj) t_max_proj = abs(t_proj);
		}
	}

	max_proj = t_max_proj;
	return KMCERR_OK;
}

// Listen mit Richtungsanzahlen entsprechend der moeglichen, aktiven Sprungrichtungen fuer jedes bewegliche Elementarzellatom erstellen
int TJumpsFunc::CreateJumpDirList(int& o_count, int*& o_dircount, int*& o_dirlink)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if (Jumps.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	if (m_Job == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_UniqueJumps == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_UniqueJumps->IfCodesReady() == false) return KMCERR_OBJECT_NOT_READY;

	int ErrorCode = KMCERR_OK;
	int t_UniqueID = -1;
	bool t_IsActive = false;
	vector<int> t_dircount(Jumps.size(), 0);
	vector<pair<int, int> > t_dirtype;

	// Alle aktivierten Spruenge zaehlen
	for (int i = 0; i < (int)Jumps.size(); i++)
	{
		if (Jumps[i].size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		for (int j = 0; j < (int)Jumps[i].size(); j++)
		{

			ErrorCode = Jumps[i][j].GetJumpUniqueJumpID(t_UniqueID);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			ErrorCode = m_Job->m_UniqueJumps->GetUJumpActive(t_UniqueID, t_IsActive);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			if (t_IsActive == false) continue;
			++(t_dircount[i]);
		}
		if (t_dircount[i] == 0) return KMCERR_NO_ACTIVE_JUMP_ON_ATOM;

		auto t_It = find_if(t_dirtype.begin(), t_dirtype.end(), [i, &t_dircount](const pair<int, int>& val)
			{
				return (val.first == t_dircount[i]);
			});
		if (t_It == t_dirtype.end())
		{
			t_dirtype.emplace_back(t_dircount[i], 1);
		}
		else
		{
			t_It->second += 1;
		}
	}

	// Sortieren nach Kombination aus Richtungsanzahl und Anzahl an entsprechenden Atomen pro Elementarzelle
	sort(t_dirtype.begin(), t_dirtype.end(), [](const pair<int, int>& a, const pair<int, int>& b) -> bool
		{
			return (a.first * a.second > b.first * b.second);
		});

	// Output-Reset
	delete[] o_dircount;
	delete[] o_dirlink;

	// Richtungstyp-Anzahl setzen
	o_count = (int)t_dirtype.size();

	// Richtungsanzahlarray erstellen
	o_dircount = new (nothrow) int[t_dirtype.size()];
	if (o_dircount == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
	for (int i = 0; i < (int)t_dirtype.size(); i++)
	{
		o_dircount[i] = t_dirtype[i].first;
	}

	// Zuordnungsarray erstellen
	o_dirlink = new (nothrow) int[t_dircount.size()];
	if (o_dirlink == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
	for (int i = 0; i < (int)t_dircount.size(); i++)
	{
		int t_Index = -1;
		for (int j = 0; j < (int)t_dirtype.size(); j++)
		{
			if (t_dircount[i] == t_dirtype[j].first)
			{
				t_Index = j;
				break;
			}
		}
		if (t_Index == -1) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		o_dirlink[i] = t_Index;
	}

	return KMCERR_OK;
}

// Minimalbeschreibung aller aktiven Spruenge und einzigartigen Spruenge fuer die Simulation erstellen
int TJumpsFunc::CreateSimJumps(int last_movstacksize, TSimJump**& o_simjumps, TSimUniqueJump*& o_simuniquejumps)
{
	if (IfReady() != true) return KMCERR_READY_NOT_TRUE;

	if (Jumps.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	if (m_Job == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_UniqueJumps == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_UniqueJumps->IfCodesReady() == false) return KMCERR_OBJECT_NOT_READY;

	int ErrorCode = KMCERR_OK;

	// Sim-Objekte der UniqueJumps erstellen
	vector<int> t_NewUniqueIDs;
	ErrorCode = m_Job->m_UniqueJumps->CreateSimUniqueJumps(o_simuniquejumps, &t_NewUniqueIDs);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Objekt-Reset
	if (o_simjumps != NULL)
	{
		if (last_movstacksize > 0)
		{
			for (int i = last_movstacksize - 1; i >= 0; i--)
			{
				delete[] o_simjumps[i];
				o_simjumps[i] = NULL;
			}
		}
		delete[] o_simjumps;
		o_simjumps = NULL;
	}

	// Array erstellen
	o_simjumps = new (nothrow) TSimJump * [Jumps.size()];
	if (o_simjumps == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;

	// Liste der IDs der aktiven Jumps erstellen (und Map der neuen IDs, inaktive Jumps behalten -1 in Map)
	int t_UniqueID = -1;
	bool t_IsActive = false;
	vector<vector<int>> t_ActiveIDs(Jumps.size(), vector<int>());
	vector<vector<int>> t_NewDirIDs(Jumps.size(), vector<int>());
	for (int i = 0; i < (int)Jumps.size(); i++)
	{
		if (Jumps[i].size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		t_NewDirIDs[i] = vector<int>(Jumps[i].size(), -1);
		for (int j = 0; j < (int)Jumps[i].size(); j++)
		{

			ErrorCode = Jumps[i][j].GetJumpUniqueJumpID(t_UniqueID);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			ErrorCode = m_Job->m_UniqueJumps->GetUJumpActive(t_UniqueID, t_IsActive);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			if (t_IsActive == false) continue;
			t_NewDirIDs[i][j] = (int)t_ActiveIDs[i].size();
			t_ActiveIDs[i].push_back(j);
		}
		if (t_ActiveIDs[i].size() == 0) return KMCERR_NO_ACTIVE_JUMP_ON_ATOM;
	}

	// Sprungbeschreibungen erstellen
	for (int i = 0; i < (int)Jumps.size(); i++)
	{

		// Array fuer die aktiven Sprungrichtungen erstellen
		o_simjumps[i] = new (nothrow) TSimJump[t_ActiveIDs[i].size()];
		if (o_simjumps[i] == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;

		// Sprungbeschreibungen fuer die aktiven Sprungrichtungen erstellen
		for (int j = 0; j < (int)t_ActiveIDs[i].size(); j++)
		{

			ErrorCode = Jumps[i][t_ActiveIDs[i][j]].CreateSimJump(o_simjumps[i][j]);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			ErrorCode = Jumps[i][t_ActiveIDs[i][j]].GetJumpUniqueJumpID(t_UniqueID);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			// Pointer zu UniqueJump setzen
			o_simjumps[i][j].unique_jump = &(o_simuniquejumps[t_NewUniqueIDs[t_UniqueID]]);
		}
	}

	// Rueckspruenge setzen
	int t_BackjumpSecondID = -1;
	T4DLatticeVector t_DestPos;
	for (int i = 0; i < (int)t_ActiveIDs.size(); i++)
	{
		for (int j = 0; j < (int)t_ActiveIDs[i].size(); j++)
		{

			ErrorCode = Jumps[i][t_ActiveIDs[i][j]].GetJumpBackjumpDirID(t_BackjumpSecondID);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			ErrorCode = Jumps[i][t_ActiveIDs[i][j]].GetDestPos(t_DestPos);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			o_simjumps[i][j].back_jump = &(o_simjumps[t_DestPos.s][t_NewDirIDs[t_DestPos.s][t_BackjumpSecondID]]);
		}
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //