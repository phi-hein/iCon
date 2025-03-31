// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpsBase	(TUniqueJumps Layer 0)				//
//	Autor: Philipp Hein												//
//	Datum: 31.03.2013												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der einzigartigen Gitterspruenge 		//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TUniqueJumps/TUniqueJumpsBase.h"

// Includes:
#include <sstream>
#include <iomanip>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TJumps/TJumps.h"
#include "TStructure/TStructure.h"
#include "TInteraction/TInteraction.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TUniqueJumpsBase::TUniqueJumpsBase (TKMCJob * pJob) {
	m_Job = pJob;

	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;
	UJumps.clear();
	Interactions.clear();

	JumpsReady = false;
	CodesReady = false;
}

// Destructor
TUniqueJumpsBase::~TUniqueJumpsBase () {

}

// **************************** PUBLISHED ********************************* //

// Aus Gittersprungumgebungen von TJumps die einzigartigen Sprünge finden
int TUniqueJumpsBase::MakeUniqueJumps () {
	if (JumpsReady != false) {
		cout << "Critical Error: TUniqueJumps->Jumps ready too early (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	if (CodesReady != false) {
		cout << "Critical Error: TUniqueJumps->Codes ready too early (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Input prüfen
	if (m_Job == 0) {
		cout << "Critical Error: m_Job is null pointer (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps == 0) {
		cout << "Critical Error: m_Jumps is null pointer (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0) {
		cout << "Critical Error: m_Structure is null pointer (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps->IfReady() == false) {
		cout << "Critical Error: TJumps not ready (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Structure->IfReady() == false) {
		cout << "Critical Error: TStructure not ready (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Sprunganzahlen in TJumps ermitteln
	int ErrorCode = KMCERR_OK;
	int AtomCount = 0;
	vector<int> DirCount;
	ErrorCode = m_Job->m_Structure->GetMovCount(AtomCount);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (AtomCount <= 0) {
		cout << "Critical Error: Invalid number of jump start positions (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	for (int i = 0; i < AtomCount; i++) {
		DirCount.push_back(0);
		ErrorCode = m_Job->m_Jumps->GetJumpDirCount(i,DirCount.back());
		if (ErrorCode != KMCERR_OK) return ErrorCode;
		if (DirCount.back() <= 0) {
			cout << "Critical Error: Invalid number of jump directions (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
	}

	// Ersten einzigartigen Sprung setzen
	TUniqueJump jhelp (m_Job);
	vector<vector<vector<int>>> mapping (AtomCount, vector<vector<int>> ());	// Mapping für alle i,j speichern und erst am Ende übertragen -> Klasse bleibt gültig, auch bei Abbruch zwischendrin
	vector<vector<int>> IDs (AtomCount, vector<int> ());						// UniqueIDs für alle i,j speichern und erst am Ende übertragen
	vector<TUniqueJump> i_UJumps;
	ErrorCode = jhelp.SetEnv(0,0);							// ersten einzigartigen Sprung erstellen
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	i_UJumps.push_back(jhelp);								// Sprung hinzufügen
	ErrorCode = i_UJumps.back().Sort();						// Sprung sortieren
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	mapping[0].push_back(vector<int> ());
	ErrorCode = jhelp.MapToJump(i_UJumps.back(),&mapping[0][0]);	// mapping: unsortiert -> sortiert
	if (ErrorCode != KMCERR_EQUAL) return ErrorCode;
	IDs[0].push_back(0);											// ID speichern

	// Einzigartige Spruenge finden
	bool jump_found = false;
	for (int i = 0; i < AtomCount; i++) {
		for (int j = 0; j < DirCount[i]; j++) {
			if ((i == 0) && (j == 0)) continue;
			mapping[i].push_back(vector<int> ());
			IDs[i].push_back(0);
			jhelp = TUniqueJump (m_Job);
			ErrorCode = jhelp.SetEnv(i,j);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			jump_found = false;
			for (int k = 0; k < (int) i_UJumps.size(); k++) {
				ErrorCode = jhelp.MapToJump(i_UJumps[k],&mapping[i][j]);
				if ((ErrorCode != KMCERR_EQUAL) && (ErrorCode != KMCERR_UNEQUAL)) return ErrorCode;
				if (ErrorCode == KMCERR_EQUAL) {
					IDs[i][j] = k;
					jump_found = true;
					break;
				}
			}
			if (jump_found == false) {
				i_UJumps.push_back(jhelp);								// Sprung hinzufügen
				ErrorCode = i_UJumps.back().Sort();						// Sprung sortieren
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				ErrorCode = jhelp.MapToJump(i_UJumps.back(),&mapping[i][j]);	// mapping: unsortiert -> sortiert
				if (ErrorCode != KMCERR_EQUAL) return ErrorCode;
				IDs[i][j] = (int) i_UJumps.size() - 1;
			}
		}
	}

	// i_UJumps sortieren (nach Sprunglaenge, klein zuerst) und Referenzen in IDs aktualisieren
	vector<int> sort_map;
	int i_help;
	if ((int) i_UJumps.size() > 1) {
		sort_map = vector<int> (i_UJumps.size(),0);
		for (int i = 0; i < (int) sort_map.size(); i++) {
			sort_map[i] = i;
		}
		double Length1, Length2;
		for (int n = i_UJumps.size(); n > 1; n--) {			// BubbleSort
			for (int i = 0; i < n - 1; i++) {
				ErrorCode = i_UJumps[i].GetLength(Length1);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve jump length (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
					return ErrorCode;
				}
				ErrorCode = i_UJumps[i + 1].GetLength(Length2);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve jump length (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
					return ErrorCode;
				}
				if (Length1 > Length2 + T3DVector::eq_threshold) {
					jhelp = i_UJumps[i];
					i_UJumps[i] = i_UJumps[i + 1];
					i_UJumps[i + 1] = jhelp;
					i_help = sort_map[i];
					sort_map[i] = sort_map[i + 1];
					sort_map[i + 1] = i_help;
				} 
			} 
		}
		for (int i = 0; i < AtomCount; i++) {				
			for (int j = 0; j < DirCount[i]; j++) {
				jump_found = false;
				for (int k = 0; k < (int) sort_map.size(); k++) {
					if (sort_map[k] == IDs[i][j]) {
						IDs[i][j] = k;
						jump_found = true;
						break;
					}
				}
				if (jump_found == false) {
					cout << "Critical Error: Unique ID not found (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
					return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
				}
			}
		}
	}

	// IDs und mappings auf TJumps übertragen
	ErrorCode = m_Job->m_Jumps->SetUniqueJumps(&IDs,&mapping);		// EnvPos in zugehörigem TJump umsortieren und UniqueJumpIDs setzen
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	
	JumpsReady = true;
	for (int i = 0; i < (int) i_UJumps.size(); i++) {
		if (i_UJumps[i].IfJumpReady() == false) JumpsReady = false;
	}
	if (JumpsReady != true) {
		cout << "Critical Error: Unique jumps are not ready (in TUniqueJumpsBase::MakeUniqueJumps)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	UJumps = i_UJumps;
	Interactions.clear();
	return KMCERR_OK;
}

// Sprung aktivieren (= Sprung möglich) oder deaktivieren (= Sprung unmöglich), KMCERR_DEACTIVATION_IMPOSSIBLE zeigt, dass Sprung aktiviert bleiben muss
int TUniqueJumpsBase::SetJumpActive (int JumpID, bool State) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps not ready (in TUniqueJumpsBase::SetJumpActive)" << endl;
		cout << "                Input: " << JumpID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) {
		cout << "Critical Error: Invalid jump ID (in TUniqueJumpsBase::SetJumpActive)" << endl;
		cout << "                Input: " << JumpID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Sicherstellen, dass trotzdem jedes Atom in der Elementarzelle mind. eine aktive Sprungrichtung hat
	bool ActualState = true;
	int ErrorCode = UJumps[JumpID].GetJumpActive(ActualState);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: Cannot retrieve current active state (in TUniqueJumpsBase::SetJumpActive)" << endl;
		cout << "                Input: " << JumpID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return ErrorCode;
	}
	if ((ActualState == true) && (State == false)) {
		if ((int) UJumps.size() == 1) {
			cout << "Error: Cannot deactivate unique jump number " << JumpID << endl;
			cout << "       There has to be at least one active unique jump" << endl << endl;
			return KMCERR_DEACTIVATION_IMPOSSIBLE;
		}
		if (m_Job == 0) {
			cout << "Critical Error: m_Job is null pointer (in TUniqueJumpsBase::SetJumpActive)" << endl;
			cout << "                Input: " << JumpID << " ";
			if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Job->m_Jumps == 0) {
			cout << "Critical Error: m_Jumps is null pointer (in TUniqueJumpsBase::SetJumpActive)" << endl;
			cout << "                Input: " << JumpID << " ";
			if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Job->m_Structure == 0) {
			cout << "Critical Error: m_Structure is null pointer (in TUniqueJumpsBase::SetJumpActives)" << endl;
			cout << "                Input: " << JumpID << " ";
			if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
			return KMCERR_INVALID_POINTER;
		}
		if (m_Job->m_Jumps->IfReady() == false) {
			cout << "Critical Error: TJumps not ready (in TUniqueJumpsBase::SetJumpActive)" << endl;
			cout << "                Input: " << JumpID << " ";
			if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}
		if (m_Job->m_Structure->IfReady() == false) {
			cout << "Critical Error: TStructure not ready (in TUniqueJumpsBase::SetJumpActive)" << endl;
			cout << "                Input: " << JumpID << " ";
			if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
			return KMCERR_OBJECT_NOT_READY;
		}
		int AtomCount = 0;
		vector<int> DirCount;
		ErrorCode = m_Job->m_Structure->GetMovCount(AtomCount);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
		if (AtomCount <= 0) {
			cout << "Critical Error: Invalid number of jump start positions (in TUniqueJumpsBase::SetJumpActive)" << endl;
			cout << "                Input: " << JumpID << " ";
			if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		for (int i = 0; i < AtomCount; i++) {
			DirCount.push_back(0);
			ErrorCode = m_Job->m_Jumps->GetJumpDirCount(i,DirCount.back());
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			if (DirCount.back() <= 0) {
				cout << "Critical Error: Invalid number of jump directions (in TUniqueJumpsBase::SetJumpActive)" << endl;
				cout << "                Input: " << JumpID << " ";
				if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
				return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
			}
		}
		vector<bool> UJumpsState;
		for (int i = 0; i < (int) UJumps.size(); i++) {
			ErrorCode = UJumps[i].GetJumpActive(ActualState);
			if (ErrorCode != KMCERR_OK) {
				cout << "Critical Error: Cannot retrieve active state (in TUniqueJumpsBase::SetJumpActive)" << endl;
				cout << "                Input: " << JumpID << " ";
				if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
				return ErrorCode;
			}
			UJumpsState.push_back(ActualState);
		}
		int ActiveDir;
		int DirJumpID;
		for (int i = 0; i < AtomCount; i++) {
			ActiveDir = 0;
			for (int j = 0; j < DirCount[i]; j++) {
				ErrorCode = m_Job->m_Jumps->GetUniqueJumpID(i,j,DirJumpID);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve unique jump ID (in TUniqueJumpsBase::SetJumpActive)" << endl;
					cout << "                Input: " << JumpID << " ";
					if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
					return ErrorCode;
				}
				if (JumpID == DirJumpID) continue;
				if (UJumpsState[DirJumpID] == true) ActiveDir++;
			}
			if (ActiveDir < 1) {
				cout << "Error: Cannot deactivate unique jump number " << JumpID << endl;
				cout << "       There has to be at least one active jump direction for every moving unit cell atom" << endl << endl;
				return KMCERR_DEACTIVATION_IMPOSSIBLE;
			}
		}
	}

	// Propagate state change to unique jump
	ErrorCode = UJumps[JumpID].SetJumpActive(State);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	CodesReady = false;
	for (int i = 0; i < (int) UJumps.size(); i++) {
		UJumps[i].ClearInteractions();
	}
	Interactions.clear();
	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;

	return KMCERR_OK;
}

// Umgebungsatom in einem Sprung aktivieren oder deaktivieren
int TUniqueJumpsBase::SetAtomActive (int JumpID, int CoordID, bool State) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps not ready (in TUniqueJumpsBase::SetAtomActive)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) {
		cout << "Critical Error: Invalid jump ID (in TUniqueJumpsBase::SetAtomActive)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Propagate state change to unique jump
	int ErrorCode = UJumps[JumpID].SetAtomActive(CoordID,State);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	CodesReady = false;
	for (int i = 0; i < (int) UJumps.size(); i++) {
		UJumps[i].ClearInteractions();
	}
	Interactions.clear();
	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;

	return KMCERR_OK;
}

// Umgebungsatom in einem Sprung als additiv (true) oder nicht additiv (false) markieren
int TUniqueJumpsBase::SetAtomAdditive (int JumpID, int CoordID, bool State) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps not ready (in TUniqueJumpsBase::SetAtomAdditive)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) {
		cout << "Critical Error: Invalid jump ID (in TUniqueJumpsBase::SetAtomAdditive)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Propagate state change to unique jump
	int ErrorCode = UJumps[JumpID].SetAtomAdditive(CoordID,State);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	CodesReady = false;
	for (int i = 0; i < (int) UJumps.size(); i++) {
		UJumps[i].ClearInteractions();
	}
	Interactions.clear();
	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;

	return KMCERR_OK;
}

// Umgebungsatom ignorieren oder nicht-ignorieren
int TUniqueJumpsBase::SetAtomIgnore (int JumpID, int CoordID, bool State) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps not ready (in TUniqueJumpsBase::SetAtomIgnore)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) {
		cout << "Critical Error: Invalid jump ID (in TUniqueJumpsBase::SetAtomIgnore)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " ";
		if (State == true) cout << "true" << endl << endl; else cout << "false" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Propagate state change to unique jump
	int ErrorCode = UJumps[JumpID].SetAtomIgnore(CoordID,State);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	CodesReady = false;
	for (int i = 0; i < (int) UJumps.size(); i++) {
		UJumps[i].ClearInteractions();
	}
	Interactions.clear();
	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;

	return KMCERR_OK;
}

// Codierung loeschen
int TUniqueJumpsBase::ClearUniqueCodes () {
	CodesReady = false;
	if ((int) UJumps.size() != 0) {
		for (int i = 0; i < (int) UJumps.size(); i++) {
			UJumps[i].ClearInteractions();
		}
	}
	Interactions.clear();
	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;

	return KMCERR_OK;
}

// Mögliche Umgebungsbesetzungen bestimmen und vergleichen
int TUniqueJumpsBase::MakeUniqueCodes (int i_InteractionShellCount) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps not ready (in TUniqueJumpsBase::MakeUniqueCodes)" << endl;
		cout << "                Input: " << i_InteractionShellCount << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen und übertragen
	if ((i_InteractionShellCount < 0) || (i_InteractionShellCount > KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONSHELLCOUNT)) {
		cout << "Error: Invalid number of interaction shells: " << i_InteractionShellCount << endl;
		cout << "       Number of interaction shells has to be an integer in the interval [0 , " << KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONSHELLCOUNT << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Wechselwirkungen aller additiven Atome mit der springenden Leerstelle in allen UJumps charakterisieren
	vector<TUniqueJump> t_UJumps (UJumps);
	vector<TInteraction> i_Interactions;
	int ErrorCode = KMCERR_OK;
	for (int i = 0; i < (int) t_UJumps.size(); i++) {
		ErrorCode = t_UJumps[i].CharacterizeInteractions(i,i_InteractionShellCount,&i_Interactions);		// Wechselwirkungen charakterisieren
		if (ErrorCode != KMCERR_OK) return ErrorCode;
	}
	
	// Wechselwirkungen sortieren (nach ElemID und Abstand zu Start- und Zielatom, klein zuerst) und Referenzen in allen TUniqueJump aktualisieren
	int i_help = 0;
	vector<int> sort_map;
	if ((int) i_Interactions.size() > 1) {
		sort_map = vector<int> (i_Interactions.size(),0);
		for (int i = 0; i < (int) sort_map.size(); i++) {
			sort_map[i] = i;
		}
		TInteraction vhelp (m_Job);
		int ElemID1, ElemID2;
		double StartDist1, DestDist1;
		double StartDist2, DestDist2;
		for (int n = i_Interactions.size(); n > 1; n--) {			// BubbleSort
			for (int i = 0; i < n - 1; i++) {
				ErrorCode = i_Interactions[i].GetWWDesc(ElemID1,StartDist1,DestDist1);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve interaction description 1 (in TUniqueJumpsBase::MakeUniqueCodes)" << endl;
					cout << "                Input: " << i_InteractionShellCount << endl << endl;
					return ErrorCode;
				}
				ErrorCode = i_Interactions[i + 1].GetWWDesc(ElemID2,StartDist2,DestDist2);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve interaction description 2 (in TUniqueJumpsBase::MakeUniqueCodes)" << endl;
					cout << "                Input: " << i_InteractionShellCount << endl << endl;
					return ErrorCode;
				}
				if ((ElemID1 > ElemID2) ||
					((ElemID1 == ElemID2) && (StartDist1 > StartDist2 + T3DVector::eq_threshold)) ||
					((ElemID1 == ElemID2) && (abs(StartDist1 - StartDist2) <= T3DVector::eq_threshold) &&
					 (DestDist1 > DestDist2 + T3DVector::eq_threshold))) {
					vhelp = i_Interactions[i];
					i_Interactions[i] = i_Interactions[i + 1];
					i_Interactions[i + 1] = vhelp;
					i_help = sort_map[i];
					sort_map[i] = sort_map[i + 1];
					sort_map[i + 1] = i_help;
				} 
			} 
		}
		for (int i = 0; i < (int) t_UJumps.size(); i++) {		// InteractionMaps in allen TUniqueJump aktualisieren
			ErrorCode = t_UJumps[i].UpdateInteractionMaps(&sort_map);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
		}
	}

	// MakeUniqueCodes der UJumps aufrufen
	for (int i = 0; i < (int) t_UJumps.size(); i++) {
		ErrorCode = t_UJumps[i].MakeUniqueCodes(i);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
	}

	CodesReady = true;
	for (int i = 0; i < (int) t_UJumps.size(); i++) {
		if (t_UJumps[i].IfCodesReady() == false) CodesReady = false;
	}
	if (CodesReady != true) {
		cout << "Critical Error: Unique codes are not ready (in TUniqueJumpsBase::MakeUniqueCodes)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	UJumps = t_UJumps;
	Interactions = i_Interactions;
	InteractionShellCount = i_InteractionShellCount;
	return KMCERR_OK;
}

// WW-Energie [eV] für bestimmte CoordID (muss additiv und nicht-ignoriert sein) für eine bestimmte (Elem)ID setzen
int TUniqueJumpsBase::SetInteractionEnergyByCoord (int JumpID, int CoordID, int ID, double Energy) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps->Jumps not ready (in TUniqueJumpsBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true) {
		cout << "Critical Error: TUniqueJumps->Codes not ready (in TUniqueJumpsBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) {
		cout << "Critical Error: Invalid jump ID (in TUniqueJumpsBase::SetInteractionEnergyByCoord)" << endl;
		cout << "                Input: " << JumpID << " " << CoordID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].SetInteractionEnergyByCoord(CoordID,ID,Energy);
}

// WW-Energie [eV] für bestimmte InteractionID in allen UJumps für eine bestimmte (Elem)ID setzen
int TUniqueJumpsBase::SetInteractionEnergyByInteraction (int WWID, int ID, double Energy) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps->Jumps not ready (in TUniqueJumpsBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true) {
		cout << "Critical Error: TUniqueJumps->Codes not ready (in TUniqueJumpsBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((WWID < 0) || (WWID >= (int) Interactions.size())) {
		cout << "Critical Error: Invalid interaction ID (in TUniqueJumpsBase::SetInteractionEnergyByInteraction)" << endl;
		cout << "                Input: " << WWID << " " << ID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// SetInteractionEnergyByInteraction für alle UJumps aufrufen
	vector<TUniqueJump> t_UJumps (UJumps);
	int ErrorCode = KMCERR_OK;
	for (int i = 0; i < (int) t_UJumps.size(); i++) {
		ErrorCode = t_UJumps[i].SetInteractionEnergyByInteraction(WWID,ID,Energy);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
	}

	UJumps = t_UJumps;
	return KMCERR_OK;
}

// Code-Energie [eV] für bestimmte UniqueCodeID in bestimmtem UJump setzen
int TUniqueJumpsBase::SetCodeEnergy (int JumpID, int UCodeID, double Energy) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps->Jumps not ready (in TUniqueJumpsBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << JumpID << " " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (CodesReady != true) {
		cout << "Critical Error: TUniqueJumps->Codes not ready (in TUniqueJumpsBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << JumpID << " " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) {
		cout << "Critical Error: Invalid jump ID (in TUniqueJumpsBase::SetCodeEnergy)" << endl;
		cout << "                Input: " << JumpID << " " << UCodeID << " " << Energy << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return UJumps[JumpID].SetCodeEnergy(UCodeID,Energy);
}


// ***************************** PUBLIC *********************************** //

// Rueckgabe von JumpsReady, incl. UJumps
bool TUniqueJumpsBase::IfJumpsReady () {
	if ((int) UJumps.size() != 0) {
		for (int i = 0; i < (int) UJumps.size(); i++) {
			if (UJumps[i].IfJumpReady() == false) return false;
		}
	}
	return JumpsReady;
}

// Rueckgabe von CodesReady, incl. UJumps
bool TUniqueJumpsBase::IfCodesReady () {
	if ((int) UJumps.size() != 0) {
		for (int i = 0; i < (int) UJumps.size(); i++) {
			if (UJumps[i].IfCodesReady() == false) return false;
		}
	}
	return CodesReady;
}

// Elemente in einen Stream schreiben
int TUniqueJumpsBase::SaveToStream (ostream &output, int offset) {
	if (JumpsReady != true) {
		cout << "Critical Error: TUniqueJumps->Jumps not ready (in TUniqueJumpsBase::SaveToStream)" << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	
	// Input pruefen
	if (output.fail() == true) {
		cout << "Critical Error: Output stream not ready (in TUniqueJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) {
		cout << "Critical Error: Offset out of bounds (in TUniqueJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset,' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int) KMCOUT_TUNIQUEJUMPS_OFFSET > 0) sub_offset = string((int) KMCOUT_TUNIQUEJUMPS_OFFSET,' ');

	// Ausgabe aller UniqueJumps ermitteln
	vector<string> t_jumpstr;
	int ErrorCode = KMCERR_OK;
	for (int i = 0; i < (int) UJumps.size(); i++) {
		stringstream t_jump_stream;
		t_jump_stream.precision(output.precision());
		ErrorCode = UJumps[i].SaveToStream(t_jump_stream,i,offset + KMCOUT_TUNIQUEJUMPS_OFFSET);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
		t_jumpstr.push_back(t_jump_stream.str());
	}

	// Ausgabe aller Interactions ermitteln
	vector<string> t_wwstr;
	ErrorCode = KMCERR_OK;
	if ((int) Interactions.size() != 0) {
		for (int i = 0; i < (int) Interactions.size(); i++) {
			stringstream t_ww_stream;
			t_ww_stream.precision(output.precision());
			ErrorCode = Interactions[i].SaveToStream(t_ww_stream,i,offset + KMCOUT_TUNIQUEJUMPS_OFFSET);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			t_wwstr.push_back(t_ww_stream.str());
		}
	}

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile
	output << s_offset << KMCOUT_TUNIQUEJUMPS_START << endl;

	// Ausgabe der Wechselwirkungsschalenanzahl
	output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMPS_INTERACTIONSHELLCOUNT << " " << InteractionShellCount << endl;

	// Ausgabe, ob Codierung erfolgt ist
	output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMPS_CODESREADY << " ";
	if (IfCodesReady() == true) {
		output << KMCOUT_TUNIQUEJUMPS_CODESREADY_TRUE << endl;
	} else {
		output << KMCOUT_TUNIQUEJUMPS_CODESREADY_FALSE << endl;
	}

	// Ausgabe von UJump-Anzahl und VVShell-Anzahl
	output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMPS_UJUMPCOUNT << " " << UJumps.size() << endl;
	output << s_offset << sub_offset << KMCOUT_TUNIQUEJUMPS_INTERACTIONCOUNT << " " << Interactions.size() << endl;

	// Ausgabe der TUniqueJump
	for (int i = 0; i < (int) t_jumpstr.size(); i++) {
		output << endl;
		output << t_jumpstr[i];
		output.flush();
	}

	// Ausgabe der TVVShell
	if ((int) t_wwstr.size() != 0) {
		for (int i = 0; i < (int) t_wwstr.size(); i++) {
			output << endl;
			output << t_wwstr[i];
			output.flush();
		}
	}

	// Abschlusszeile schreiben
	output << s_offset << KMCOUT_TUNIQUEJUMPS_END << endl;

	if (output.fail() == true) {
		cout << "Critical Error: Output stream failure (in TUniqueJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Elemente aus einem Stream laden
int TUniqueJumpsBase::LoadFromStream (istream &input) {
	if (JumpsReady != false) {
		cout << "Critical Error: TUniqueJumps->Jumps ready too early (in TUniqueJumpsBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	if (CodesReady != false) {
		cout << "Critical Error: TUniqueJumps->Jumps ready too early (in TUniqueJumpsBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Input pruefen
	if (input.good() == false) {
		cout << "Critical Error: Input stream not ready (in TUniqueJumpsBase::LoadFromStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Inputparameter setzen
	input >> skipws;

	// Bisherigen Input zwischenspeichern und loeschen
	int t_InteractionShellCount = InteractionShellCount;
	InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;
	vector<TUniqueJump> t_UJumps (UJumps);
	UJumps.clear();
	vector<TInteraction> t_Interactions (Interactions);
	Interactions.clear();

	// UniqueJumps generieren
	int ErrorCode = MakeUniqueJumps();
	if ((ErrorCode != KMCERR_OK) || (IfJumpsReady() == false)) {
		InteractionShellCount = t_InteractionShellCount;
		UJumps = t_UJumps;
		Interactions = t_Interactions;
		JumpsReady = false;
		CodesReady = false;
		if (ErrorCode == KMCERR_OK) {
			cout << "Critical Error: Unique jumps are not ready (in TUniqueJumpsBase::LoadFromStream)" << endl << endl;
			ErrorCode = KMCERR_INVALID_INPUT_CRIT;
		}
		return ErrorCode;
	}

	// Daten einlesen
	int i_InteractionShellCount = KMCVAR_INTERACTIONSHELL_INIT;
	bool i_Coding = false;
	int i_UJumpCount = -1;
	int i_InteractionCount = -1;
	vector<bool> i_UJumpChecked (UJumps.size(), false);
	vector<bool> i_InteractionChecked;
	vector<string> i_UJumpStream (UJumps.size(), "");
	vector<string> i_InteractionStream;
	
	string line = "";
	bool if_failed = false;
	string s_temp = "";
	int i_temp = 0;
	while (input.good() == true) {
		// Zeile laden
		if (getline(input,line).fail() == true) {
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream (line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TUNIQUEJUMPS_INTERACTIONSHELLCOUNT) {
			if ((linestream >> i_InteractionShellCount).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TUNIQUEJUMPS_CODESREADY) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TUNIQUEJUMPS_CODESREADY_TRUE) {
				i_Coding = true;
			} else {
				i_Coding = false;
			}
		} else if (s_temp == KMCOUT_TUNIQUEJUMPS_UJUMPCOUNT) {
			if ((linestream >> i_UJumpCount).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TUNIQUEJUMPS_INTERACTIONCOUNT) {
			if ((linestream >> i_InteractionCount).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TUNIQUEJUMP_START1) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}			
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if ((i_temp < 0) || (i_temp >= (int) UJumps.size())) {
				if_failed = true;
				break;
			}
			if (i_UJumpChecked[i_temp] == true) {
				if_failed = true;
				break;
			}
			s_temp = "";
			stringstream temp_stream ("");
			while (s_temp != KMCOUT_TUNIQUEJUMP_END) {
				if (getline(input,line).fail() == true) {
					if_failed = true;
					break;
				}
				temp_stream << line << endl;
				stringstream linestream2 (line);
				if ((linestream2 >> s_temp).fail() == true) s_temp = "";
			}
			if (if_failed == true) break;
			i_UJumpStream[i_temp] = temp_stream.str();
			i_UJumpChecked[i_temp] = true;
		} else if (s_temp == KMCOUT_TINTERACTION_START1) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}			
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp < 0) {
				if_failed = true;
				break;
			}
			while ((i_temp >= (int) i_InteractionChecked.size()) || (i_temp >= (int) i_InteractionStream.size())) {
				i_InteractionChecked.push_back(false);
				i_InteractionStream.push_back("");
			}
			if (i_InteractionChecked[i_temp] == true) {
				if_failed = true;
				break;
			}
			s_temp = "";
			stringstream temp_stream ("");
			while (s_temp != KMCOUT_TINTERACTION_END) {
				if (getline(input,line).fail() == true) {
					if_failed = true;
					break;
				}
				temp_stream << line << endl;
				stringstream linestream2 (line);
				if ((linestream2 >> s_temp).fail() == true) s_temp = "";
			}
			if (if_failed == true) break;
			i_InteractionStream[i_temp] = temp_stream.str();
			i_InteractionChecked[i_temp] = true;
		} else if (s_temp == KMCOUT_TUNIQUEJUMPS_END) break;
	}
	if (i_UJumpCount != (int)UJumps.size()) if_failed = true;
	if (i_InteractionCount != (int)i_InteractionChecked.size()) if_failed = true;
	if (i_InteractionCount != (int)i_InteractionStream.size()) if_failed = true;
	if ((i_Coding == false) && (i_InteractionCount != 0)) if_failed = true;
	if (if_failed == false) {
		for (int i = 0; i < (int) UJumps.size(); i++) {
			if (i_UJumpChecked[i] == false) {
				if_failed = true;
				break;
			}
		}
	}
	if ((if_failed == false) && (i_InteractionChecked.size() != 0)) {
		for (int i = 0; i < (int) i_InteractionChecked.size(); i++) {
			if (i_InteractionChecked[i] == false) {
				if_failed = true;
				break;
			}
		}
	}
	
	// Falls Fehler aufgetreten ist, alte Daten wiederherstellen und Fehlercode ausgeben
	if (if_failed == true) {
		InteractionShellCount = t_InteractionShellCount;
		UJumps = t_UJumps;
		Interactions = t_Interactions;
		JumpsReady = false;
		CodesReady = false;
		if (ErrorCode == KMCERR_OK) {
			cout << "Error: Invalid file format" << endl << endl;
			ErrorCode = KMCERR_INVALID_FILE_FORMAT;
		}
		return ErrorCode;
	}

	// LoadJumpFromStream(Stream-Copy) für alle UJumps aufrufen -> Umgebungen vergleichen und Aktivierungszustände setzen
	for (int i = 0; i < (int) UJumps.size(); i++) {
		stringstream i_jumpstream (i_UJumpStream[i]);
		ErrorCode = UJumps[i].LoadJumpFromStream(i_jumpstream,i);
		if (ErrorCode != KMCERR_OK) {
			if_failed = true;
			break;
		}
	}

	// MakeUniqueCodes(i_InteractionShellCount) aufrufen -> Codierung erstellen
	if ((i_Coding == true) && (if_failed == false)) {
		ErrorCode = MakeUniqueCodes(i_InteractionShellCount);
		if (ErrorCode != KMCERR_OK) if_failed = true;
		if (IfCodesReady() != true) if_failed = true;
		if (i_InteractionCount != (int)Interactions.size()) if_failed = true;
	}

	// LoadCodesFromStream für alle UJumps aufrufen -> Codierung vergleichen und Energien setzen
	if ((i_Coding == true) && (if_failed == false)) {
		for (int i = 0; i < (int) UJumps.size(); i++) {
			stringstream i_jumpstream (i_UJumpStream[i]);
			ErrorCode = UJumps[i].LoadCodesFromStream(i_jumpstream);
			if (ErrorCode != KMCERR_OK) {
				if_failed = true;
				break;
			}
		}
	}

	// LoadFromStream für alle Interactions aufrufen
	if ((i_Coding == true) && (if_failed == false) && (Interactions.size() != 0)) {
		for (int i = 0; i < (int) Interactions.size(); i++) {
			stringstream i_shellstream (i_InteractionStream[i]);
			ErrorCode = Interactions[i].LoadFromStream(i_shellstream);
			if (ErrorCode != KMCERR_OK) {
				if_failed = true;
				break;
			}
		}
	}

	// Falls Fehler aufgetreten ist, alte Daten wiederherstellen und Fehlercode ausgeben
	if (if_failed == true) {
		InteractionShellCount = t_InteractionShellCount;
		UJumps = t_UJumps;
		Interactions = t_Interactions;
		JumpsReady = false;
		CodesReady = false;
		if (ErrorCode == KMCERR_OK) {
			cout << "Critical Error: Loading file failed (in TUniqueJumpsBase::LoadFromStream)" << endl << endl;
			ErrorCode = KMCERR_INVALID_INPUT_CRIT;
		}
		return ErrorCode;
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //