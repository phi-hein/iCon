// **************************************************************** //
//																	//
//	Class: TUniqueJumpBase	(TUniqueJump Layer 0)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a unique jump 							//
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
#include "TUniqueJump/TUniqueJumpBase.h"

// Includes:
#include <sstream>
#include <iomanip>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TJumps/TJumps.h"
#include "TStructure/TStructure.h"
#include "TUniqueJump/TUniqueJump.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "TInteraction/TInteraction.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TUniqueJumpBase::TUniqueJumpBase(TKMCJob* pJob)
{
	m_Job = pJob;

	Length = 0;
	Coord.clear();
	ElemID.clear();

	JumpActive = true;
	AtomIgnore.clear();
	AtomActive.clear();
	AtomAdditive.clear();

	InteractionMap.clear();
	InteractionElemIDs.clear();
	InteractionEnergies.clear();

	UniqueCodes.clear();
	UniqueEnergies.clear();
	FullCodes.clear();
	FullToUniqueMap.clear();

	EnvReady = false;
	JumpReady = false;
	CodesReady = false;
}

// Destructor
TUniqueJumpBase::~TUniqueJumpBase()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Setzt Length, Coord und ElemID aus bestimmtem TJump in TJumps
int TUniqueJumpBase::SetEnv(int AtomID, int DirID)
{
	if (EnvReady != false)
	{
		cout << "Critical Error: TUniqueJump->Env ready too early (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	if (JumpReady != false)
	{
		cout << "Critical Error: TUniqueJump->Jump ready too early (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	if (CodesReady != false)
	{
		cout << "Critical Error: TUniqueJump->Codes ready too early (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Pointerüberprüfung
	if (m_Job == 0)
	{
		cout << "Critical Error: m_Job is null pointer (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps == 0)
	{
		cout << "Critical Error: m_Jumps is null pointer (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0)
	{
		cout << "Critical Error: m_Structure is null pointer (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps->IfReady() == false)
	{
		cout << "Critical Error: TJumps not ready (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Structure->IfReady() == false)
	{
		cout << "Critical Error: TStructure not ready (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Sprungumgebung in absoluten 4D-Koordinaten aus TJumps übertragen
	int ErrorCode = KMCERR_OK;
	T4DLatticeVector JumpStart4D;
	T4DLatticeVector JumpDest4D;
	vector<T4DLatticeVector> JumpEnv4D;
	ErrorCode = m_Job->m_Jumps->GetStartPos(AtomID, DirID, JumpStart4D);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve starting position (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return ErrorCode;
	}
	ErrorCode = m_Job->m_Jumps->GetDestPos(AtomID, DirID, JumpDest4D);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve destination position (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return ErrorCode;
	}
	ErrorCode = m_Job->m_Jumps->GetEnvPos(AtomID, DirID, &JumpEnv4D);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve environment positions (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return ErrorCode;
	}
	if (JumpEnv4D.size() == 0)
	{
		cout << "Critical Error: No environment atoms (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// halben Jump-Vektor berechnen
	T3DVector jvec = (m_Job->m_Structure->Get3DVector(JumpDest4D) - m_Job->m_Structure->Get3DVector(JumpStart4D)) * (0.5);
	if (jvec == T3DVector(0, 0, 0))
	{
		cout << "Critical Error: Jump vector has zero length (in TUniqueJumpBase::SetEnv)" << endl;
		cout << "                Input: " << AtomID << " " << DirID << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	double i_Length = 2 * T3DVector::Length(jvec);

	// 3D-Vektoren von Jumpmittelpunkt zu Umgebungsatomen berechnen
	vector<T3DVector> i_Coord;
	vector<int> i_ElemID;
	vector<bool> i_AtomIgnore;
	vector<bool> i_AtomActive;
	vector<bool> i_AtomAdditive;
	for (int i = 0; i < (int)JumpEnv4D.size(); i++)
	{
		i_Coord.push_back(m_Job->m_Structure->Get3DVector(JumpEnv4D[i]) - (m_Job->m_Structure->Get3DVector(JumpStart4D) + jvec));
		i_ElemID.push_back(m_Job->m_Structure->GetElemID(JumpEnv4D[i].s));
		i_AtomIgnore.push_back(false);
		i_AtomActive.push_back(true);
		if (i_ElemID.back() == 0)
		{
			i_AtomAdditive.push_back(true);			// Leerstellen standardmaessig additiv
		}
		else
		{
			i_AtomAdditive.push_back(false);
		}
	}

	// ersten Vektor in Coord finden fuer den y im Sprungkoord. != 0 waere
	int firstyzero = -1;
	for (int i = 0; i < (int)i_Coord.size(); i++)
	{
		if (T3DVector::Length(T3DVector::Normalvector(i_Coord[i], jvec)) >= T3DVector::zero_threshold)
		{
			firstyzero = i;
			break;
		}
	}

	// Normalenvektor berechnen (wenn fuer alle Vektoren y == 0 waere, dann Normalenvektor = (0,0,0) lassen durch Default-Constructor)
	T3DVector nvec;
	if (firstyzero != -1)
	{
		nvec = T3DVector::Normalvector(i_Coord[firstyzero], jvec);
	}

	// 3D-Sprungumgebungs-Vektoren berechnen
	for (int i = 0; i < (int)i_Coord.size(); i++)
	{
		T3DVector::Transform2Jump(i_Coord[i], jvec, nvec);
	}

	// Werte übertragen
	Length = i_Length;
	Coord = i_Coord;
	ElemID = i_ElemID;
	AtomIgnore = i_AtomIgnore;
	AtomActive = i_AtomActive;
	AtomAdditive = i_AtomAdditive;
	JumpActive = true;

	// Aufbauende Inhalte löschen
	InteractionMap.clear();
	InteractionElemIDs.clear();
	InteractionEnergies.clear();
	UniqueCodes.clear();
	UniqueEnergies.clear();
	FullCodes.clear();
	FullToUniqueMap.clear();

	EnvReady = true;
	return KMCERR_OK;
}

// Sortieren (ElemID (groß zuerst), Abstand von Sprungmitte, x-Koordinate, y-Koordinate, z-Koordinate (klein zuerst))
int TUniqueJumpBase::Sort()
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::Sort)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != false)
	{
		cout << "Critical Error: TUniqueJump->Jump ready too early (in TUniqueJumpBase::Sort)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	if (CodesReady != false)
	{
		cout << "Critical Error: TUniqueJump->Codes ready too early (in TUniqueJumpBase::Sort)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Sprung sortieren (BubbleSort)
	int i_swaphelp;
	bool b_swaphelp;
	T3DVector vec_swaphelp;
	vector<T3DVector> t_Coord(Coord);
	vector<int> t_ElemID(ElemID);
	vector<bool> t_AtomIgnore(AtomIgnore);
	vector<bool> t_AtomActive(AtomActive);
	vector<bool> t_AtomAdditive(AtomAdditive);
	if ((int)t_Coord.size() > 1)
	{
		for (int i = (int)t_Coord.size(); i > 1; i--)
		{
			for (int j = 0; j < i - 1; j++)
			{
				if ((
					t_ElemID[j] < t_ElemID[j + 1]
					) || (
						(t_ElemID[j] == t_ElemID[j + 1]) &&
						(t_Coord[j].x * t_Coord[j].x + t_Coord[j].y * t_Coord[j].y > t_Coord[j + 1].x * t_Coord[j + 1].x + t_Coord[j + 1].y * t_Coord[j + 1].y + T3DVector::eq_threshold)
						) || (
							(t_ElemID[j] == t_ElemID[j + 1]) &&
							(abs(t_Coord[j].x * t_Coord[j].x + t_Coord[j].y * t_Coord[j].y - t_Coord[j + 1].x * t_Coord[j + 1].x - t_Coord[j + 1].y * t_Coord[j + 1].y) <= T3DVector::eq_threshold) &&
							(t_Coord[j].x > t_Coord[j + 1].x + T3DVector::eq_threshold)
							) || (
								(t_ElemID[j] == t_ElemID[j + 1]) &&
								(abs(t_Coord[j].x * t_Coord[j].x + t_Coord[j].y * t_Coord[j].y - t_Coord[j + 1].x * t_Coord[j + 1].x - t_Coord[j + 1].y * t_Coord[j + 1].y) <= T3DVector::eq_threshold) &&
								(abs(t_Coord[j].x - t_Coord[j + 1].x) <= T3DVector::eq_threshold) &&
								(t_Coord[j].y > t_Coord[j + 1].y + T3DVector::eq_threshold)
								) || (
									(t_ElemID[j] == t_ElemID[j + 1]) &&
									(abs(t_Coord[j].x * t_Coord[j].x + t_Coord[j].y * t_Coord[j].y - t_Coord[j + 1].x * t_Coord[j + 1].x - t_Coord[j + 1].y * t_Coord[j + 1].y) <= T3DVector::eq_threshold) &&
									(abs(t_Coord[j].x - t_Coord[j + 1].x) <= T3DVector::eq_threshold) &&
									(abs(t_Coord[j].y - t_Coord[j + 1].y) <= T3DVector::eq_threshold) &&
									(t_Coord[j].z > t_Coord[j + 1].z + T3DVector::eq_threshold)
									))
				{
					vec_swaphelp = t_Coord[j];
					t_Coord[j] = t_Coord[j + 1];
					t_Coord[j + 1] = vec_swaphelp;
					i_swaphelp = t_ElemID[j];
					t_ElemID[j] = t_ElemID[j + 1];
					t_ElemID[j + 1] = i_swaphelp;
					b_swaphelp = t_AtomIgnore[j];
					t_AtomIgnore[j] = t_AtomIgnore[j + 1];
					t_AtomIgnore[j + 1] = b_swaphelp;
					b_swaphelp = t_AtomActive[j];
					t_AtomActive[j] = t_AtomActive[j + 1];
					t_AtomActive[j + 1] = b_swaphelp;
					b_swaphelp = t_AtomAdditive[j];
					t_AtomAdditive[j] = t_AtomAdditive[j + 1];
					t_AtomAdditive[j + 1] = b_swaphelp;
				}
			}
		}
	}

	// sortierte Inhalte übertragen
	Coord = t_Coord;
	ElemID = t_ElemID;
	AtomIgnore = t_AtomIgnore;
	AtomActive = t_AtomActive;
	AtomAdditive = t_AtomAdditive;

	// Aufbauende Inhalte löschen
	ClearInteractions();

	// UniqueJump-Konsistenz prüfen
	if ((Length > T3DVector::zero_threshold) &&
		(Coord.size() != 0) &&
		(Coord.size() == ElemID.size()) &&
		(Coord.size() == AtomIgnore.size()) &&
		(Coord.size() == AtomActive.size()) &&
		(Coord.size() == AtomAdditive.size()))
	{
		JumpReady = true;
	}

	return KMCERR_OK;
}

// Sprung auf toJump mappen -> mapping = map: Jump.CoordID -> toJump.CoordID
int TUniqueJumpBase::MapToJump(TUniqueJump& toJump, vector<int>* jMap)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::MapToJump)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (toJump.IfEnvReady() != true)
	{
		cout << "Critical Error: Environment of other jump is not ready (in TUniqueJumpBase::MapToJump)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Coord und ElemID von toJump übertragen
	int ErrorCode = KMCERR_OK;
	double Length2 = 0;
	vector<T3DVector> Coord2;
	vector<int> ElemID2;
	ErrorCode = toJump.GetLength(Length2);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump length (in TUniqueJumpBase::MapToJump)" << endl << endl;
		return ErrorCode;
	}
	ErrorCode = toJump.GetCoords(&Coord2);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump environment positions (in TUniqueJumpBase::MapToJump)" << endl << endl;
		return ErrorCode;
	}
	ErrorCode = toJump.GetElemIDs(&ElemID2);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump environment elements (in TUniqueJumpBase::MapToJump)" << endl << endl;
		return ErrorCode;
	}

	// Vergleich der Sprünge
	if ((Length >= Length2 + T3DVector::eq_threshold) || (Length <= Length2 - T3DVector::eq_threshold)) return KMCERR_UNEQUAL;
	if (Coord.size() != Coord2.size()) return KMCERR_UNEQUAL;

	// Anzahl an y != 0 in Jump (= j1) und toJump (= j2) finden und ersten Vektor in j1 und j2 finden, der y != 0 hat (gleichzeitig ElemIDs aufsummieren)
	int elemsumj1 = 0;
	int elemsumj2 = 0;
	int firstyzeroj1 = -1;
	int firstyzeroj2 = -1;
	int yzerocounterj1 = 0;
	int yzerocounterj2 = 0;
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		if ((Coord[i].y < -T3DVector::eq_threshold) || (Coord[i].y > T3DVector::eq_threshold))
		{
			if (firstyzeroj1 == -1) firstyzeroj1 = i;
			yzerocounterj1++;
		}
		if ((Coord2[i].y < -T3DVector::eq_threshold) || (Coord2[i].y > T3DVector::eq_threshold))
		{
			if (firstyzeroj2 == -1) firstyzeroj2 = i;
			yzerocounterj2++;
		}
		elemsumj1 += ElemID[i];
		elemsumj2 += ElemID2[i];
	}

	// Falls j1 und j2 unterschiedliche Anzahlen an y != 0 haben, dann kein mapping moeglich
	if (yzerocounterj1 != yzerocounterj2) return KMCERR_UNEQUAL;

	// Falls Summe der ElemIDs unterschiedlich ist, dann kein mapping moeglich
	if (elemsumj1 != elemsumj2) return KMCERR_UNEQUAL;

	// --- MAPPING --- //
	jMap->clear();

	// Falls in j1 und j2 kein Vektor y != 0 hat, dann besonderes mapping, denn alle Atome auf Sprungachse
	if (yzerocounterj1 == 0)
	{	// d.h. alle Atome auf Sprungachse
		for (int j = 0; j < (int)Coord.size(); j++)
		{
			for (int k = 0; k < (int)Coord2.size(); k++)
			{
				if (ElemID[j] != ElemID2[k]) continue;
				if ((Coord[j].x >= Coord2[k].x + T3DVector::eq_threshold) || (Coord[j].x <= Coord2[k].x - T3DVector::eq_threshold)) continue;
				jMap->push_back(k);
				break;
			}
		}
		if (jMap->size() == Coord.size())
		{
			return KMCERR_EQUAL;					// passendes Mapping gefunden
		}
		else
		{
			return KMCERR_UNEQUAL;					// kein passendes Mapping gefunden
		}
	}

	// Normales Mapping
	// einen j1-Vektor nacheinander auf alle j2-Vektoren drehen (uebrige j1-Vektoren mitdrehen) und pruefen ob jeder j1-Vektor mit genau einem j2-Vektor uebereinstimmt
	double dangle = 0.0;
	for (int i = firstyzeroj2; i < (int)Coord2.size(); i++)
	{
		// wenn mapping von j1[firstyzeroj1] auf j2[i] nicht moeglich, dann direkt naechstes i pruefen
		if (ElemID[firstyzeroj1] != ElemID2[i]) continue;
		if ((Coord[firstyzeroj1].x >= Coord2[i].x + T3DVector::eq_threshold) || (Coord[firstyzeroj1].x <= Coord2[i].x - T3DVector::eq_threshold)) continue;
		if ((Coord[firstyzeroj1].y >= Coord2[i].y + T3DVector::eq_threshold) || (Coord[firstyzeroj1].y <= Coord2[i].y - T3DVector::eq_threshold)) continue;
		// wenn einer der beiden Vektoren y == 0 hat, dann nicht als Ausgangspunkt fuer mapping nehmen (weil z nicht definiert)
		if ((Coord2[i].y >= -T3DVector::eq_threshold) && (Coord2[i].y <= T3DVector::eq_threshold)) continue;
		jMap->clear();
		dangle = Coord2[i].z - Coord[firstyzeroj1].z;					// j1.z + dangle = j2.z
		// Paare finden (gleichzeitig mapping)
		for (int j = 0; j < (int)Coord.size(); j++)
		{
			for (int k = 0; k < (int)Coord2.size(); k++)
			{
				if (ElemID[j] != ElemID2[k]) continue;
				if ((Coord[j].x >= Coord2[k].x + T3DVector::eq_threshold) || (Coord[j].x <= Coord2[k].x - T3DVector::eq_threshold)) continue;
				if ((Coord[j].y >= Coord2[k].y + T3DVector::eq_threshold) || (Coord[j].y <= Coord2[k].y - T3DVector::eq_threshold)) continue;
				if ((Coord2[k].y < -T3DVector::eq_threshold) || (Coord2[k].y > T3DVector::eq_threshold))
				{
					if (CirclePeriodicCompare(Coord[j].z + dangle, Coord2[k].z) == false) continue;			// periodische Abfrage wenn y != 0
				}
				jMap->push_back(k);
				break;
			}
		}
		if (jMap->size() == Coord.size()) break;		// d.h. mapping gefunden
		// Paare finden (gleichzeitig mapping) mit gespiegelten Winkeln (an 0,180-Achse) des j1-Jumps -> axiale Chiralitaet der Umgebung umgehen
		// zuerst dangle in gespiegelter Umgebung berechnen:
		dangle = -Coord[firstyzeroj1].z;
		while (dangle < 0) dangle += 360.0;
		dangle = Coord2[i].z - dangle;
		jMap->clear();
		for (int j = 0; j < (int)Coord.size(); j++)
		{
			for (int k = 0; k < (int)Coord2.size(); k++)
			{
				if (ElemID[j] != ElemID2[k]) continue;
				if ((Coord[j].x >= Coord2[k].x + T3DVector::eq_threshold) || (Coord[j].x <= Coord2[k].x - T3DVector::eq_threshold)) continue;
				if ((Coord[j].y >= Coord2[k].y + T3DVector::eq_threshold) || (Coord[j].y <= Coord2[k].y - T3DVector::eq_threshold)) continue;
				if ((Coord2[k].y < -T3DVector::eq_threshold) || (Coord2[k].y > T3DVector::eq_threshold))
				{
					if (CirclePeriodicCompare(dangle - Coord[j].z, Coord2[k].z) == false) continue;		// periodische Abfrage wenn y != 0
				}
				jMap->push_back(k);
				break;
			}
		}
		if (jMap->size() == Coord.size()) break;
	}
	if (jMap->size() == Coord.size())
	{
		return KMCERR_EQUAL;							// passendes Mapping gefunden
	}
	else
	{
		return KMCERR_UNEQUAL;							// kein passendes Mapping gefunden
	}
}

// Sprung aktivieren oder deaktivieren
int TUniqueJumpBase::SetJumpActive(bool State)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetJumpActive)" << endl;
		cout << "                Input: ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetJumpActive)" << endl;
		cout << "                Input: ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	if (JumpActive != State)
	{
		ClearInteractions();		// damit CharacterizeInteractions noch vor MakeUniqueCodes aufgerufen werden muss
		CodesReady = false;
		JumpActive = State;
	}

	return KMCERR_OK;
}

// Umgebungsatom ignorieren oder nicht
int TUniqueJumpBase::SetAtomIgnore(int CoordID, bool State)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetAtomActive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetAtomActive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int)AtomIgnore.size()))
	{
		cout << "Critical Error: Invalid vector index (in TUniqueJumpBase::SetAtomActive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	if (AtomIgnore[CoordID] != State)
	{
		ClearInteractions();		// damit CharacterizeInteractions noch vor MakeUniqueCodes aufgerufen werden muss
		CodesReady = false;
		AtomIgnore[CoordID] = State;
	}

	return KMCERR_OK;
}

// Umgebungsatom aktivieren oder deaktivieren
int TUniqueJumpBase::SetAtomActive(int CoordID, bool State)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetAtomActive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetAtomActive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int)AtomActive.size()))
	{
		cout << "Critical Error: Invalid vector index (in TUniqueJumpBase::SetAtomActive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	if (AtomActive[CoordID] != State)
	{
		ClearInteractions();		// damit CharacterizeInteractions noch vor MakeUniqueCodes aufgerufen werden muss
		CodesReady = false;
		AtomActive[CoordID] = State;
	}

	return KMCERR_OK;
}

// Umgebungsatom additiv oder nicht
int TUniqueJumpBase::SetAtomAdditive(int CoordID, bool State)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetAtomAdditive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetAtomAdditive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if ((CoordID < 0) || (CoordID >= (int)AtomActive.size()))
	{
		cout << "Critical Error: Invalid vector index (in TUniqueJumpBase::SetAtomAdditive)" << endl;
		cout << "                Input: " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	if (AtomAdditive[CoordID] != State)
	{
		ClearInteractions();		// damit CharacterizeInteractions noch vor MakeUniqueCodes aufgerufen werden muss
		CodesReady = false;
		AtomAdditive[CoordID] = State;
	}

	return KMCERR_OK;
}

// Wechselwirkungen charakterisieren
int TUniqueJumpBase::CharacterizeInteractions(int UniqueJumpID, int InteractionShellCount, vector<TInteraction>* t_Interactions)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (m_Job == 0)
	{
		cout << "Critical Error: m_Job is null pointer (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0)
	{
		cout << "Critical Error: m_Structure is null pointer (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false)
	{
		cout << "Critical Error: TStructure not ready (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Jumps == 0)
	{
		cout << "Critical Error: m_Jumps is null pointer (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps->IfReady() == false)
	{
		cout << "Critical Error: TJumps not ready (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Pruefen ob keine nicht-ignorierten, additiven Atome in der Umgebung vorhanden sind -> nichts zu tun
	int t_additivecount = 0;
	if (JumpActive == true)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true)) t_additivecount++;
		}
	}
	if (t_additivecount == 0)
	{
		InteractionMap.clear();
		InteractionElemIDs.clear();
		InteractionEnergies.clear();
		CodesReady = false;
		return KMCERR_OK;
	}

	// Beispiel-TJumps-Umgebung für diesen UniqueJump finden
	int AtomID = -1;
	int DirID = -1;
	int ErrorCode = KMCERR_OK;
	ErrorCode = m_Job->m_Jumps->FindUniqueJumpID(UniqueJumpID, AtomID, DirID);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump example (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return ErrorCode;
	}
	if ((AtomID == -1) || (DirID == -1))
	{
		cout << "Critical Error: Invalid jump example (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Sprungumgebung in absoluten 4D-Koordinaten aus TJumps übertragen
	T4DLatticeVector JumpStart4D;
	T4DLatticeVector JumpDest4D;
	vector<T4DLatticeVector> JumpEnv4D;
	ErrorCode = m_Job->m_Jumps->GetStartPos(AtomID, DirID, JumpStart4D);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump start (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return ErrorCode;
	}
	ErrorCode = m_Job->m_Jumps->GetDestPos(AtomID, DirID, JumpDest4D);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump destination (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return ErrorCode;
	}
	ErrorCode = m_Job->m_Jumps->GetEnvPos(AtomID, DirID, &JumpEnv4D);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve jump environment (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return ErrorCode;
	}
	if (JumpEnv4D.size() == 0)
	{
		cout << "Critical Error: Invalid jump environment (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
		cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Aktive, additive Atome in der Umgebung charakterisieren
	int i_help = -1;
	vector<int> i_InteractionMap(t_additivecount, -1);
	vector<vector<int>> i_InteractionElemIDs(t_additivecount, vector<int>());
	vector<vector<double>> i_InteractionEnergies(t_additivecount, vector<double>());
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true))
		{

			// Index des additiven Atoms berechnen und Energien initialisieren
			i_help++;
			ErrorCode = m_Job->m_Structure->GetDopands4ElemID(ElemID[i], AtomActive[i], &i_InteractionElemIDs[i_help]);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "Critical Error: Cannot retrieve dopands (in TUniqueJumpBase::CharacterizeInteractions)" << endl;
				cout << "                Input: " << UniqueJumpID << " " << InteractionShellCount << endl << endl;
				return ErrorCode;
			}
			i_InteractionEnergies[i_help] = vector<double>(i_InteractionElemIDs[i_help].size(), 0.0);

			// Wechselwirkung charakterisieren
			TInteraction Interaction(m_Job);
			int ErrorCode = Interaction.MakeInteraction(JumpStart4D, JumpEnv4D[i], JumpDest4D, InteractionShellCount);
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			// Mit bisherigen Wechselwirkungen vergleichen
			bool if_found = false;
			if (t_Interactions->size() != 0)
			{
				for (int j = 0; j < (int)t_Interactions->size(); j++)
				{
					ErrorCode = t_Interactions->at(j).Compare(Interaction);
					if (ErrorCode == KMCERR_EQUAL)
					{
						if_found = true;
						i_InteractionMap[i_help] = j;
						break;
					}
					else if (ErrorCode != KMCERR_UNEQUAL) return ErrorCode;
				}
			}

			// Wechselwirkung speichern falls noch nicht vorhanden
			if (if_found == false)
			{
				t_Interactions->push_back(Interaction);
				i_InteractionMap[i_help] = t_Interactions->size() - 1;
			}
		}
	}

	InteractionMap = i_InteractionMap;
	InteractionElemIDs = i_InteractionElemIDs;
	InteractionEnergies = i_InteractionEnergies;
	CodesReady = false;
	return KMCERR_OK;
}

// Wechselwirkungsinformationen löschen (incl. Codes)
void TUniqueJumpBase::ClearInteractions()
{
	InteractionMap.clear();
	InteractionElemIDs.clear();
	InteractionEnergies.clear();
	ClearCodes();
	CodesReady = false;
}

// InteractionMap nach Sortierung der Interactions in TUniqueJumps aktualisieren
int TUniqueJumpBase::UpdateInteractionMaps(vector<int>* old_id_map)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::UpdateInteractionMaps)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::UpdateInteractionMaps)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != false)
	{
		cout << "Critical Error: TUniqueJump->Codes ready too early (in TUniqueJumpBase::UpdateInteractionMaps)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Prüfen, ob nicht-ignorierte, additive Atome vorhanden
	if (InteractionMap.size() == 0) return KMCERR_OK;

	// Input prüfen
	if (old_id_map->size() == 0)
	{
		cout << "Critical Error: ID map has no entries (in TUniqueJumpBase::UpdateInteractionMaps)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// InteractionMap aktualisieren
	bool shell_found;
	vector<int> t_InteractionMap(InteractionMap);
	for (int i = 0; i < (int)t_InteractionMap.size(); i++)
	{
		if (t_InteractionMap[i] != -1)
		{
			shell_found = false;
			for (int j = 0; j < (int)old_id_map->size(); j++)
			{
				if (t_InteractionMap[i] == old_id_map->at(j))
				{
					t_InteractionMap[i] = j;
					shell_found = true;
					break;
				}
			}
			if (shell_found == false)
			{
				cout << "Critical Error: No corresponding ID found (in TUniqueJumpBase::UpdateInteractionMaps)" << endl << endl;
				return KMCERR_INVALID_INPUT_CRIT;
			}
		}
	}

	InteractionMap = t_InteractionMap;
	return KMCERR_OK;
}

// Mögliche Umgebungsbesetzungen bestimmen und vergleichen (UniqueJumpID ist nur für Fehlermeldungen)
int TUniqueJumpBase::MakeUniqueCodes(int UniqueJumpID)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Prüfen, ob CharacterizeInteractions aufgerufen wurde
	int t_additivecount = 0;
	if (JumpActive == true)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true)) t_additivecount++;
		}
	}
	if (((int)InteractionMap.size() != t_additivecount) ||
		(InteractionMap.size() != InteractionEnergies.size()) ||
		(InteractionMap.size() != InteractionElemIDs.size()))
	{
		cout << "Critical Error: Interactions not characterized (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Wenn keine nicht-ignorierten, nicht-additiven-Atome in der Umgebung oder wenn JumpActive == false, dann keine Codierung nötig
	int t_codogencount = 0;
	if (JumpActive == true)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if ((AtomIgnore[i] == false) && (AtomAdditive[i] == false)) t_codogencount++;
		}
	}
	if (t_codogencount == 0)
	{
		FullCodes.clear();
		UniqueCodes.clear();
		UniqueEnergies.clear();
		FullToUniqueMap.clear();
		CodesReady = true;
		return KMCERR_OK;
	}

	// Pointer prüfen
	// Input pruefen
	if (m_Job == 0)
	{
		cout << "Critical Error: m_Job is null pointer (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0)
	{
		cout << "Critical Error: m_Structure is null pointer (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false)
	{
		cout << "Critical Error: TStructure not ready (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Dotierungsmap erstellen
	int ErrorCode = KMCERR_OK;
	int t_i = 0;
	vector<vector<int>> dop_map(t_codogencount, vector<int>());
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		if ((AtomIgnore[i] == false) && (AtomAdditive[i] == false))
		{
			ErrorCode = m_Job->m_Structure->GetDopands4ElemID(ElemID[i], AtomActive[i], &dop_map[t_i]);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "Critical Error: Cannot retrieve possible dopands (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
				cout << "                Input: " << UniqueJumpID << endl << endl;
				return ErrorCode;
			}
			t_i++;
		}
	}

	// Anzahl aller Dotierungsmöglichkeiten bestimmen
	unsigned long long permut_anz = 1;
	for (int i = 0; i < (int)dop_map.size(); i++)
	{
		permut_anz *= dop_map[i].size();
	}
	if (permut_anz > KMCLIM_MAX_ENVPERMUT)
	{
		cout << "Error: There are too many active environment atoms in jump " << UniqueJumpID << "." << endl;
		cout << "       This would produce too many possible environments (> " << KMCLIM_MAX_ENVPERMUT << ")." << endl;
		cout << "       Deactivate some atoms and try again." << endl << endl;
		return KMCERR_TOO_MANY_ACTIVE_ATOMS;
	}

	// Alle Dotierungsmöglichkeiten erstellen
	string test_code = "";
	vector<string> i_FullCodes;
	try
	{
		RecursivePermutCodeGen(test_code, 0, &i_FullCodes, &dop_map);
	}
	catch (bad_alloc&)
	{
		cout << "Error: There are too many active environment atoms in jump " << UniqueJumpID << "." << endl;
		cout << "       This would produce too many possible environments." << endl;
		cout << "       Deactivate some atoms and try again." << endl << endl;
		return KMCERR_TOO_MANY_ACTIVE_ATOMS;
	}
	if (i_FullCodes.size() == 0)
	{
		cout << "Critical Error: Code permutation failed (in TUniqueJumpBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << UniqueJumpID << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Unique-Suche in FullCodes
	int upos = -1;
	vector<string> i_UniqueCodes;
	vector<double> i_UniqueEnergies;
	vector<int> i_FullToUniqueMap;
	vector<int> mapping;
	TUniqueJump jump1 = *static_cast<TUniqueJump*> (this);
	TUniqueJump jump2 = *static_cast<TUniqueJump*> (this);
	for (int i = 0; i < (int)i_FullCodes.size(); i++)
	{
		if (i_UniqueCodes.size() == 0)
		{
			i_UniqueCodes.push_back(i_FullCodes[i]);
			i_UniqueEnergies.push_back(0.0);
			i_FullToUniqueMap.push_back(0);
		}
		else
		{
			ErrorCode = jump1.ApplyCode(i_FullCodes[i]);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			upos = -1;
			for (int j = 0; j < (int)i_UniqueCodes.size(); j++)
			{
				ErrorCode = jump2.ApplyCode(i_UniqueCodes[j]);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				ErrorCode = jump1.MapToJump(jump2, &mapping);
				if ((ErrorCode != KMCERR_EQUAL) && (ErrorCode != KMCERR_UNEQUAL)) return ErrorCode;
				if (ErrorCode == KMCERR_EQUAL)
				{
					upos = j;
					break;
				}
			}
			if (upos != -1)
			{
				i_UniqueCodes[upos] = i_UniqueCodes[upos] + i_FullCodes[i];
				i_FullToUniqueMap.push_back(upos);
			}
			else
			{
				i_UniqueCodes.push_back(i_FullCodes[i]);
				i_UniqueEnergies.push_back(0.0);
				i_FullToUniqueMap.push_back(int(i_UniqueCodes.size()) - 1);
			}
		}
	}

	FullCodes = i_FullCodes;
	UniqueCodes = i_UniqueCodes;
	UniqueEnergies = i_UniqueEnergies;
	FullToUniqueMap = i_FullToUniqueMap;
	CodesReady = true;
	return KMCERR_OK;
}

// Codes löschen
void TUniqueJumpBase::ClearCodes()
{
	FullCodes.clear();
	UniqueCodes.clear();
	UniqueEnergies.clear();
	FullToUniqueMap.clear();
	CodesReady = false;
}

// ElemIDs entsprechend Code ändern
int TUniqueJumpBase::ApplyCode(string i_code)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::ApplyCode)" << endl;
		cout << "                Input: " << i_code << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::ApplyCode)" << endl;
		cout << "                Input: " << i_code << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Wenn keine nicht-ignorierten, nicht-additiven Atome vorliegen dann nichts zu tun
	int t_codogencount = 0;
	if (JumpActive == true)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if ((AtomIgnore[i] == false) && (AtomAdditive[i] == false)) t_codogencount++;
		}
	}
	if (t_codogencount == 0) return KMCERR_OK;

	// Codierung lesen
	vector<int> t_ElemID;
	stringstream s_in(i_code);
	int temp_int = 0;
	if (s_in.good() != true)
	{
		cout << "Critical Error: Code string not valid (in TUniqueJumpBase::ApplyCode)" << endl;
		cout << "                Input: " << i_code << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((s_in >> temp_int).fail())
	{
		cout << "Critical Error: Cannot read first elem ID (in TUniqueJumpBase::ApplyCode)" << endl;
		cout << "                Input: " << i_code << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	while ((temp_int != -1) && (s_in.good() == true))
	{
		t_ElemID.push_back(temp_int);
		if ((s_in >> temp_int).fail())
		{
			cout << "Critical Error: Cannot read elem ID (in TUniqueJumpBase::ApplyCode)" << endl;
			cout << "                Input: " << i_code << endl << endl;
			return KMCERR_INVALID_INPUT_CRIT;
		}
	}
	if ((int)t_ElemID.size() != t_codogencount)
	{
		cout << "Critical Error: Invalid number of IDs in code (in TUniqueJumpBase::ApplyCode)" << endl;
		cout << "                Input: " << i_code << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Codierung übertragen
	int j = -1;
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		if ((AtomIgnore[i] == false) && (AtomAdditive[i] == false))
		{
			j++;
			ElemID[i] = t_ElemID[j];
		}
	}

	return KMCERR_OK;
}

// WW-Energie für bestimmte additive, nicht-ignorierte CoordID und eine bestimmte (Elem)ID-Besetzung setzen
int TUniqueJumpBase::SetInteractionEnergyByCoord(int CoordID, int ID, double Energy)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true)
	{
		cout << "Critical Error: TUniqueJump->Codes not ready (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((CoordID < 0) || (CoordID >= (int)Coord.size()))
	{
		cout << "Critical Error: Invalid vector index (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((AtomIgnore[CoordID] != false) || (AtomAdditive[CoordID] != true))
	{
		cout << "Critical Error: Invalid atom state (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((Energy < KMCVAL_TUNIQUEJUMPS_MIN_INTERACTIONENERGY) || (Energy > KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONENERGY))
	{
		cout << "Error: Invalid energy for interaction: " << Energy << endl;
		cout << "       Energy has to be a rational number in the interval [" << KMCVAL_TUNIQUEJUMPS_MIN_INTERACTIONENERGY;
		cout << " , " << KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONENERGY << "]." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Index in InteractionElemIDs und -Energies ermitteln
	int ww_index = -1;
	for (int i = 0; i <= CoordID; i++)
	{
		if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true)) ww_index++;
	}
	if ((ww_index == -1) || (ww_index >= (int)InteractionElemIDs.size()))
	{
		cout << "Critical Error: Invalid translated index (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	int elem_index = -1;
	if (InteractionElemIDs[ww_index].size() != 0)
	{
		for (int i = 0; i < (int)InteractionElemIDs[ww_index].size(); i++)
		{
			if (InteractionElemIDs[ww_index][i] == ID)
			{
				elem_index = i;
				break;
			}
		}
	}
	if (elem_index == -1)
	{
		cout << "Critical Error: Invalid elementary index (in TUniqueJumpBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Energie setzen
	InteractionEnergies[ww_index][elem_index] = Energy;

	return KMCERR_OK;
}

// WW-Energie für bestimmte InteractionID und eine bestimmte (Elem)ID setzen
int TUniqueJumpBase::SetInteractionEnergyByInteraction(int WWID, int ID, double Energy)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true)
	{
		cout << "Critical Error: TUniqueJump->Codes not ready (in TUniqueJumpBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if (WWID < 0)
	{
		cout << "Critical Error: Negative shell ID (in TUniqueJumpBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((Energy < KMCVAL_TUNIQUEJUMPS_MIN_INTERACTIONENERGY) || (Energy > KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONENERGY))
	{
		cout << "Error: Invalid energy for interaction: " << Energy << endl;
		cout << "       Energy has to be a rational number in the interval [" << KMCVAL_TUNIQUEJUMPS_MIN_INTERACTIONENERGY;
		cout << " , " << KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONENERGY << "]." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Energien setzen
	if (InteractionMap.size() != 0)
	{
		for (int i = 0; i < (int)InteractionMap.size(); i++)
		{
			if (InteractionMap[i] == WWID)
			{
				int elem_index = -1;
				if (InteractionElemIDs.size() != 0)
				{
					for (int j = 0; j < (int)InteractionElemIDs.size(); j++)
					{
						if (InteractionElemIDs[i][j] == ID)
						{
							elem_index = j;
							break;
						}
					}
				}
				if (elem_index == -1)
				{
					cout << "Critical Error: Invalid elementary index (in TUniqueJumpBase::SetInteractionEnergyByInteraction)" << endl;
					cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
				}
				InteractionEnergies[i][elem_index] = Energy;
			}
		}
	}

	return KMCERR_OK;
}

// Code-Energie für bestimmte UniqueCodeID setzen
int TUniqueJumpBase::SetCodeEnergy(int UCodeID, double Energy)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true)
	{
		cout << "Critical Error: TUniqueJump->Codes not ready (in TUniqueJumpBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((UCodeID < 0) || (UCodeID >= (int)UniqueEnergies.size()))
	{
		cout << "Critical Error: Invalid vector index (in TUniqueJumpBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((Energy < KMCVAL_TUNIQUEJUMPS_MIN_CODEENERGY) || (Energy > KMCVAL_TUNIQUEJUMPS_MAX_CODEENERGY))
	{
		cout << "Error: Invalid activation energy: " << Energy << endl;
		cout << "       Energy has to be a rational number in the interval [" << KMCVAL_TUNIQUEJUMPS_MIN_CODEENERGY;
		cout << " , " << KMCVAL_TUNIQUEJUMPS_MAX_CODEENERGY << "]." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Energie setzen
	UniqueEnergies[UCodeID] = Energy;

	return KMCERR_OK;
}

// Rueckgabe von EnvReady
bool TUniqueJumpBase::IfEnvReady()
{
	return EnvReady;
}

// Rueckgabe von JumpReady
bool TUniqueJumpBase::IfJumpReady()
{
	return JumpReady;
}

// Rueckgabe von CodesReady
bool TUniqueJumpBase::IfCodesReady()
{
	return CodesReady;
}

// UniqueJump in einen Stream schreiben
int TUniqueJumpBase::SaveToStream(ostream& output, int JumpID, int offset)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true)
	{
		cout << "Critical Error: Output stream not ready (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET))
	{
		cout << "Critical Error: Offset out of bounds (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (JumpID < 0)
	{
		cout << "Critical Error: Negative jump ID (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset, ' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int)KMCOUT_TUNIQUEJUMP_OFFSET > 0) sub_offset = string((int)KMCOUT_TUNIQUEJUMP_OFFSET, ' ');

	// Pointer prüfen
	if (m_Job == 0)
	{
		cout << "Critical Error: m_Job is null pointer (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_UniqueJumps == 0)
	{
		cout << "Critical Error: m_UniqueJumps is null pointer (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_UniqueJumps->IfJumpsReady() == false)
	{
		cout << "Critical Error: TUniqueJumps->Jumps not ready (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile
	output << s_offset << KMCOUT_TUNIQUEJUMP_START1 << " " << KMCOUT_TUNIQUEJUMP_JUMPID << " " << JumpID;
	output << " " << KMCOUT_TUNIQUEJUMP_START2 << endl;

	// Ausgabe von Sprunglänge, RücksprungID und JumpActive
	output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMP_LENGTH << " " << Length << endl;
	output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMP_JUMPACTIVE << " ";
	if (JumpActive == true)
	{
		output << KMCOUT_TUNIQUEJUMP_JUMPACTIVE_TRUE << endl;
	}
	else
	{
		output << KMCOUT_TUNIQUEJUMP_JUMPACTIVE_FALSE << endl;
	}
	output << endl;

	// Ausgabe der Umgebungsatome, d.h. Coord, ElemID, AtomIgnore, AtomAdditive, AtomActive und BackjumpMap
	int ID_space = IntToStr((int)Coord.size()).length();
	if (ID_space < (int)KMCOUT_TUNIQUEJUMP_COORDID.length()) ID_space = KMCOUT_TUNIQUEJUMP_COORDID.length();
	int ElemID_space = KMCOUT_TUNIQUEJUMP_ELEMID.length();
	for (int i = 0; i < (int)ElemID.size(); i++)
	{
		if (ElemID_space < (int)IntToStr(ElemID[i]).length()) ElemID_space = IntToStr(ElemID[i]).length();
	}
	int x_space = (int)output.precision() + 7;
	int y_space = (int)output.precision() + 7;
	int z_space = (int)output.precision() + 7;
	if (x_space < (int)KMCOUT_TUNIQUEJUMP_X.length()) x_space = KMCOUT_TUNIQUEJUMP_X.length();
	if (y_space < (int)KMCOUT_TUNIQUEJUMP_Y.length()) y_space = KMCOUT_TUNIQUEJUMP_Y.length();
	if (z_space < (int)KMCOUT_TUNIQUEJUMP_Z.length()) z_space = KMCOUT_TUNIQUEJUMP_Z.length();
	int Ignore_space = KMCOUT_TUNIQUEJUMP_ATOMIGNORE.front().length();
	if (Ignore_space < (int)KMCOUT_TUNIQUEJUMP_ATOMIGNORE_TRUE.front().length()) Ignore_space = KMCOUT_TUNIQUEJUMP_ATOMIGNORE_TRUE.front().length();
	if (Ignore_space < (int)KMCOUT_TUNIQUEJUMP_ATOMIGNORE_FALSE.front().length()) Ignore_space = KMCOUT_TUNIQUEJUMP_ATOMIGNORE_FALSE.front().length();
	int Additive_space = KMCOUT_TUNIQUEJUMP_ATOMADDITIVE.front().length();
	if (Additive_space < (int)KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_TRUE.front().length()) Additive_space = KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_TRUE.front().length();
	if (Additive_space < (int)KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_FALSE.front().length()) Additive_space = KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_FALSE.front().length();
	int Active_space = KMCOUT_TUNIQUEJUMP_ATOMACTIVE.front().length();
	if (Active_space < (int)KMCOUT_TUNIQUEJUMP_ATOMACTIVE_TRUE.front().length()) Active_space = KMCOUT_TUNIQUEJUMP_ATOMACTIVE_TRUE.front().length();
	if (Active_space < (int)KMCOUT_TUNIQUEJUMP_ATOMACTIVE_FALSE.front().length()) Active_space = KMCOUT_TUNIQUEJUMP_ATOMACTIVE_FALSE.front().length();
	output << s_offset << sub_offset << setw(KMCOUT_TUNIQUEJUMP_COORD.front().length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TUNIQUEJUMP_COORDID << " ";
	output << setw(ElemID_space) << KMCOUT_TUNIQUEJUMP_ELEMID << " ( ";
	output << setw(x_space) << KMCOUT_TUNIQUEJUMP_X << " ";
	output << setw(y_space) << KMCOUT_TUNIQUEJUMP_Y << " ";
	output << setw(z_space) << KMCOUT_TUNIQUEJUMP_Z << " ) ";
	output << setw(Ignore_space) << KMCOUT_TUNIQUEJUMP_ATOMIGNORE << " ";
	output << setw(Additive_space) << KMCOUT_TUNIQUEJUMP_ATOMADDITIVE << " ";
	output << setw(Active_space) << KMCOUT_TUNIQUEJUMP_ATOMACTIVE << endl;
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMP_COORD << " " << setw(ID_space) << i << " ";
		output << setw(ElemID_space) << ElemID[i] << " ( ";
		output << setw(x_space) << Coord[i].x << " " << setw(y_space) << Coord[i].y << " ";
		output << setw(z_space) << Coord[i].z << " ) ";
		if (AtomIgnore[i] == true)
		{
			output << setw(Ignore_space) << KMCOUT_TUNIQUEJUMP_ATOMIGNORE_TRUE << " ";
		}
		else
		{
			output << setw(Ignore_space) << KMCOUT_TUNIQUEJUMP_ATOMIGNORE_FALSE << " ";
		}
		if (AtomAdditive[i] == true)
		{
			output << setw(Additive_space) << KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_TRUE << " ";
		}
		else
		{
			output << setw(Additive_space) << KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_FALSE << " ";
		}
		if (AtomActive[i] == true)
		{
			output << setw(Active_space) << KMCOUT_TUNIQUEJUMP_ATOMACTIVE_TRUE << endl;
		}
		else
		{
			output << setw(Active_space) << KMCOUT_TUNIQUEJUMP_ATOMACTIVE_FALSE << endl;
		}
	}

	// Prüfen, ob Codierung schon komplett erfolgt
	if ((CodesReady == false) || (m_Job->m_UniqueJumps->IfCodesReady() == false) || (JumpActive == false))
	{
		output << s_offset << KMCOUT_TUNIQUEJUMP_END << endl;
		if (output.fail() == true)
		{
			cout << "Critical Error: Output stream failure 1 (in TUniqueJumpBase::SaveToStream)" << endl;
			cout << "                Input: " << JumpID << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}
		return KMCERR_OK;
	}

	// Ausgabe von InteractionMap, InteractionElemIDs und InteractionEnergies (falls size != 0)
	int Map_space = 0;
	int Anz_space = 0;
	int Energy_space = 0;
	if (InteractionMap.size() != 0)
	{
		ID_space = IntToStr((int)InteractionMap.size()).length();
		if (ID_space < (int)KMCOUT_TUNIQUEJUMP_INTERACTIONATOM_ID.length()) ID_space = KMCOUT_TUNIQUEJUMP_INTERACTIONATOM_ID.length();
		Map_space = KMCOUT_TUNIQUEJUMP_INTERACTIONMAP.front().length();
		for (int i = 0; i < (int)InteractionMap.size(); i++)
		{
			if (Map_space < (int)IntToStr(InteractionMap[i]).length()) Map_space = IntToStr(InteractionMap[i]).length();
		}
		Anz_space = KMCOUT_TUNIQUEJUMP_INTERACTIONENERGYCOUNT.front().length();
		for (int i = 0; i < (int)InteractionElemIDs.size(); i++)
		{
			if (Anz_space < (int)IntToStr(InteractionElemIDs[i].size()).length()) Anz_space = IntToStr(InteractionElemIDs[i].size()).length();
		}
		ElemID_space = KMCOUT_TUNIQUEJUMP_INTERACTIONELEMID.length();
		for (int i = 0; i < (int)InteractionElemIDs.size(); i++)
		{
			for (int j = 0; j < (int)InteractionElemIDs[i].size(); j++)
			{
				if (ElemID_space < (int)IntToStr(InteractionElemIDs[i][j]).length()) ElemID_space = IntToStr(InteractionElemIDs[i][j]).length();
			}
		}
		Energy_space = (int)output.precision() + 7;
		if (Energy_space < (int)KMCOUT_TUNIQUEJUMP_INTERACTIONENERGY.front().length()) Energy_space = KMCOUT_TUNIQUEJUMP_INTERACTIONENERGY.front().length();
		output << endl;
		output << s_offset << sub_offset << setw(KMCOUT_TUNIQUEJUMP_INTERACTIONATOM.front().length() + 1) << " ";
		output << setw(ID_space) << KMCOUT_TUNIQUEJUMP_INTERACTIONATOM_ID << " ";
		output << setw(Map_space) << KMCOUT_TUNIQUEJUMP_INTERACTIONMAP << " ";
		output << setw(Anz_space) << KMCOUT_TUNIQUEJUMP_INTERACTIONENERGYCOUNT << " ";
		output << setw(ElemID_space) << KMCOUT_TUNIQUEJUMP_INTERACTIONELEMID << " ";
		output << setw(Energy_space) << KMCOUT_TUNIQUEJUMP_INTERACTIONENERGY << " ..." << endl;
		for (int i = 0; i < (int)InteractionMap.size(); i++)
		{
			output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMP_INTERACTIONATOM << " " << setw(ID_space) << i << " ";
			output << setw(Map_space) << InteractionMap[i] << " " << setw(Anz_space) << InteractionElemIDs[i].size();
			for (int j = 0; j < (int)InteractionElemIDs[i].size(); j++)
			{
				output << " " << setw(ElemID_space) << InteractionElemIDs[i][j] << " ";
				output << setw(Energy_space) << InteractionEnergies[i][j];
			}
			output << endl;
		}
	}

	// Prüfung ob Codes vorhanden
	if ((UniqueCodes.size() == 0) || (FullCodes.size() == 0))
	{
		output << s_offset << KMCOUT_TUNIQUEJUMP_END << endl;
		if (output.fail() == true)
		{
			cout << "Critical Error: Output stream failure 2 (in TUniqueJumpBase::SaveToStream)" << endl;
			cout << "                Input: " << JumpID << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}
		return KMCERR_OK;
	}

	// Ausgabe von UniqueCodes und UniqueEnergies
	ID_space = IntToStr((int)UniqueCodes.size()).length();
	if (ID_space < (int)KMCOUT_TUNIQUEJUMP_UNIQUEID.length()) ID_space = KMCOUT_TUNIQUEJUMP_UNIQUEID.length();
	Energy_space = (int)output.precision() + 7;
	if (Energy_space < (int)KMCOUT_TUNIQUEJUMP_UNIQUEENERGY.front().length()) Energy_space = KMCOUT_TUNIQUEJUMP_UNIQUEENERGY.front().length();
	output << endl;
	output << s_offset << sub_offset << setw(KMCOUT_TUNIQUEJUMP_UNIQUECODE.front().length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TUNIQUEJUMP_UNIQUEID << " ";
	output << setw(Energy_space) << KMCOUT_TUNIQUEJUMP_UNIQUEENERGY << " ";
	output << KMCOUT_TUNIQUEJUMP_CODE << endl;
	for (int i = 0; i < (int)UniqueCodes.size(); i++)
	{
		output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMP_UNIQUECODE << " " << setw(ID_space) << i << " ";
		output << setw(Energy_space) << UniqueEnergies[i] << " " << UniqueCodes[i] << endl;
	}
	output << endl;

	// Ausgabe von FullCodes und FullToUniqueMap
	ID_space = IntToStr((int)FullCodes.size()).length();
	if (ID_space < (int)KMCOUT_TUNIQUEJUMP_FULLCODEID.length()) ID_space = KMCOUT_TUNIQUEJUMP_FULLCODEID.length();
	Map_space = KMCOUT_TUNIQUEJUMP_FULLTOUNIQUEMAP.length();
	for (int i = 0; i < (int)FullToUniqueMap.size(); i++)
	{
		if (Map_space < (int)IntToStr(FullToUniqueMap[i]).length()) Map_space = IntToStr(FullToUniqueMap[i]).length();
	}
	output << s_offset << sub_offset << setw(KMCOUT_TUNIQUEJUMP_FULLCODE.front().length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TUNIQUEJUMP_FULLCODEID << " ";
	output << setw(Map_space) << KMCOUT_TUNIQUEJUMP_FULLTOUNIQUEMAP << " ";
	output << KMCOUT_TUNIQUEJUMP_CODE << endl;
	for (int i = 0; i < (int)FullCodes.size(); i++)
	{
		output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMP_FULLCODE << " " << setw(ID_space) << i << " ";
		output << setw(Map_space) << FullToUniqueMap[i] << " " << FullCodes[i] << endl;
	}
	output << s_offset << KMCOUT_TUNIQUEJUMP_END << endl;

	if (output.fail() == true)
	{
		cout << "Critical Error: Output stream failure 3 (in TUniqueJumpBase::SaveToStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// UniqueJump - Sprunginformationen aus einem Stream laden
int TUniqueJumpBase::LoadJumpFromStream(istream& input, int JumpID)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::LoadJumpFromStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::LoadJumpFromStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (input.good() == false)
	{
		cout << "Critical Error: Input stream not ready (in TUniqueJumpBase::LoadJumpFromStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (m_Job == 0)
	{
		cout << "Critical Error: m_Job is null pointer (in TUniqueJumpBase::LoadJumpFromStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_UniqueJumps == 0)
	{
		cout << "Critical Error: m_UniqueJumps is null pointer (in TUniqueJumpBase::LoadJumpFromStream)" << endl;
		cout << "                Input: " << JumpID << endl << endl;
		return KMCERR_INVALID_POINTER;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	double i_Length = 0;
	vector<T3DVector> i_Coord;
	vector<int> i_ElemID;
	bool i_JumpActive = true;
	vector<bool> i_AtomIgnore;
	vector<bool> i_AtomAdditive;
	vector<bool> i_AtomActive;

	string line = "";
	bool if_failed = false;
	string s_temp = "";
	int i_temp = 0;
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

		if (s_temp == KMCOUT_TUNIQUEJUMP_LENGTH)
		{
			if ((linestream >> i_Length).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TUNIQUEJUMP_JUMPACTIVE)
		{
			if ((linestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TUNIQUEJUMP_JUMPACTIVE_TRUE)
			{
				i_JumpActive = true;
			}
			else
			{
				i_JumpActive = false;
			}
		}
		else if (s_temp == KMCOUT_TUNIQUEJUMP_COORD)
		{
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_temp != (int)i_Coord.size())
			{
				if_failed = true;
				break;
			}
			i_ElemID.push_back(-1);
			i_Coord.push_back(T3DVector());
			i_AtomIgnore.push_back(false);
			i_AtomAdditive.push_back(false);
			i_AtomActive.push_back(true);
			if ((linestream >> i_ElemID.back()).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			if ((linestream >> i_Coord.back().x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_Coord.back().y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_Coord.back().z).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// ")"
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TUNIQUEJUMP_ATOMIGNORE_TRUE)
			{
				i_AtomIgnore.back() = true;
			}
			else
			{
				i_AtomIgnore.back() = false;
			}
			if ((linestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_TRUE)
			{
				i_AtomAdditive.back() = true;
			}
			else
			{
				i_AtomAdditive.back() = false;
			}
			if ((linestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TUNIQUEJUMP_ATOMACTIVE_TRUE)
			{
				i_AtomActive.back() = true;
			}
			else
			{
				i_AtomActive.back() = false;
			}
		}
		else if (s_temp == KMCOUT_TUNIQUEJUMP_END) break;
	}
	if (if_failed == true)
	{
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Sprunginformationen abgleichen
	if (abs(Length - i_Length) > T3DVector::eq_threshold)
	{
		if (abs(Length - i_Length) > KMCVAR_EQTHRESHOLD_PERC_LOAD * Length)
		{
			cout << "Error: Inconsistent file content" << endl << endl;
			return KMCERR_INVALID_FILE_CONTENT;
		}
	}
	if ((Coord.size() != i_Coord.size()) ||
		(ElemID.size() != i_ElemID.size()) ||
		(AtomIgnore.size() != i_AtomIgnore.size()) ||
		(AtomAdditive.size() != i_AtomAdditive.size()) ||
		(AtomActive.size() != i_AtomActive.size()))
	{
		cout << "Error: Inconsistent file content" << endl << endl;
		return KMCERR_INVALID_FILE_CONTENT;
	}
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		if ((ElemID[i] != i_ElemID[i]) ||
			(T3DVector::LoadCompare(Coord[i], i_Coord[i], true) == false))
		{
			cout << "Error: Inconsistent file content" << endl << endl;
			return KMCERR_INVALID_FILE_CONTENT;
		}
	}

	// Zustände übertragen
	int ErrorCode = m_Job->m_UniqueJumps->SetJumpActive(JumpID, i_JumpActive);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	for (int i = 0; i < (int)Coord.size(); i++)
	{
		ErrorCode = m_Job->m_UniqueJumps->SetAtomIgnore(JumpID, i, i_AtomIgnore[i]);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
		ErrorCode = m_Job->m_UniqueJumps->SetAtomAdditive(JumpID, i, i_AtomAdditive[i]);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
		ErrorCode = m_Job->m_UniqueJumps->SetAtomActive(JumpID, i, i_AtomActive[i]);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
	}

	return KMCERR_OK;
}

// UniqueJump - Codierung aus einem Stream laden
int TUniqueJumpBase::LoadCodesFromStream(istream& input)
{
	if (EnvReady != true)
	{
		cout << "Critical Error: TUniqueJump->Env not ready (in TUniqueJumpBase::LoadCodesFromStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (JumpReady != true)
	{
		cout << "Critical Error: TUniqueJump->Jump not ready (in TUniqueJumpBase::LoadCodesFromStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true)
	{
		cout << "Critical Error: TUniqueJump->Codes not ready (in TUniqueJumpBase::LoadCodesFromStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (input.good() == false)
	{
		cout << "Critical Error: Input stream not ready (in TUniqueJumpBase::LoadCodesFromStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	vector<int> i_InteractionMap;
	vector<vector<int>> i_InteractionElemIDs;
	vector<vector<double>> i_InteractionEnergies;
	vector<string> i_UniqueCodes;
	vector<double> i_UniqueEnergies;
	vector<string> i_FullCodes;
	vector<int> i_FullToUniqueMap;

	string line = "";
	bool if_failed = false;
	string s_temp = "";
	int i_temp = 0;
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

		if (s_temp == KMCOUT_TUNIQUEJUMP_INTERACTIONATOM)
		{
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_temp != (int)i_InteractionMap.size())
			{
				if_failed = true;
				break;
			}
			i_InteractionMap.push_back(-1);
			if ((linestream >> i_InteractionMap.back()).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_temp < 1)
			{
				if_failed = true;
				break;
			}
			i_InteractionElemIDs.push_back(vector<int>(i_temp, -1));
			i_InteractionEnergies.push_back(vector<double>(i_temp, 0));
			for (int k = 0; k < i_temp; k++)
			{
				if ((linestream >> i_InteractionElemIDs.back().at(k)).fail() == true)
				{
					if_failed = true;
					break;
				}
				if ((linestream >> i_InteractionEnergies.back().at(k)).fail() == true)
				{
					if_failed = true;
					break;
				}
			}
		}
		else if (s_temp == KMCOUT_TUNIQUEJUMP_UNIQUECODE)
		{
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_temp != (int)i_UniqueCodes.size())
			{
				if_failed = true;
				break;
			}
			i_UniqueCodes.push_back("");
			i_UniqueEnergies.push_back(0);
			if ((linestream >> i_UniqueEnergies.back()).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (getline(linestream, i_UniqueCodes.back()).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TUNIQUEJUMP_FULLCODE)
		{
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (i_temp != (int)i_FullCodes.size())
			{
				if_failed = true;
				break;
			}
			i_FullCodes.push_back("");
			i_FullToUniqueMap.push_back(-1);
			if ((linestream >> i_FullToUniqueMap.back()).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (getline(linestream, i_FullCodes.back()).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TUNIQUEJUMP_END) break;
	}
	if (if_failed == true)
	{
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Codierungsinformationen abgleichen
	if ((InteractionMap.size() != i_InteractionMap.size()) ||
		(InteractionElemIDs.size() != i_InteractionElemIDs.size()) ||
		(InteractionEnergies.size() != i_InteractionEnergies.size()) ||
		(UniqueCodes.size() != i_UniqueCodes.size()) ||
		(UniqueEnergies.size() != i_UniqueEnergies.size()) ||
		(FullCodes.size() != i_FullCodes.size()) ||
		(FullToUniqueMap.size() != i_FullToUniqueMap.size()))
	{
		cout << "Error: Inconsistent file content" << endl << endl;
		return KMCERR_INVALID_FILE_CONTENT;
	}
	if (InteractionMap.size() != 0)
	{
		for (int i = 0; i < (int)InteractionMap.size(); i++)
		{
			if (InteractionMap[i] != i_InteractionMap[i])
			{
				cout << "Error: Inconsistent file content" << endl << endl;
				return KMCERR_INVALID_FILE_CONTENT;
			}
			if (InteractionElemIDs[i].size() != i_InteractionElemIDs[i].size())
			{
				cout << "Error: Inconsistent file content" << endl << endl;
				return KMCERR_INVALID_FILE_CONTENT;
			}
			if (InteractionEnergies[i].size() != i_InteractionEnergies[i].size())
			{
				cout << "Error: Inconsistent file content" << endl << endl;
				return KMCERR_INVALID_FILE_CONTENT;
			}
		}
	}
	if (UniqueCodes.size() != 0)
	{
		for (int i = 0; i < (int)UniqueCodes.size(); i++)
		{
			if (Trim(UniqueCodes[i]) != Trim(i_UniqueCodes[i]))
			{
				cout << "Error: Inconsistent file content" << endl << endl;
				return KMCERR_INVALID_FILE_CONTENT;
			}
		}
	}
	if (FullCodes.size() != 0)
	{
		for (int i = 0; i < (int)FullCodes.size(); i++)
		{
			if ((Trim(FullCodes[i]) != Trim(i_FullCodes[i])) ||
				(FullToUniqueMap[i] != i_FullToUniqueMap[i]))
			{
				cout << "Error: Inconsistent file content" << endl << endl;
				return KMCERR_INVALID_FILE_CONTENT;
			}
		}
	}

	// Energien übertragen
	int ErrorCode = KMCERR_OK;
	int help_index = -1;
	if (i_InteractionElemIDs.size() != 0)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true))
			{
				help_index++;
				for (int j = 0; j < (int)i_InteractionElemIDs[help_index].size(); j++)
				{
					ErrorCode = SetInteractionEnergyByCoord(i, i_InteractionElemIDs[help_index][j], i_InteractionEnergies[help_index][j]);
					if (ErrorCode != KMCERR_OK) return ErrorCode;
				}
			}
		}
	}
	if (i_UniqueEnergies.size() != 0)
	{
		for (int i = 0; i < (int)i_UniqueEnergies.size(); i++)
		{
			ErrorCode = SetCodeEnergy(i, i_UniqueEnergies[i]);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
		}
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //

// Liefert rekursiv alle Code-Permutationen
void TUniqueJumpBase::RecursivePermutCodeGen(string recstr, int depth, vector<string>* outstr, vector<vector<int>>* dmap)
{
	if (depth >= (int)dmap->size())
	{
		outstr->push_back(recstr + " -1");			// -1 als Trennzeichen zwischen verschiedenen Codes
		return;
	}
	for (int i = 0; i < (int)dmap->at(depth).size(); i++)
	{
		RecursivePermutCodeGen(recstr + " " + IntToStr(dmap->at(depth).at(i)), depth + 1, outstr, dmap);
	}
}