// **************************************************************** //
//																	//
//	Klasse: TJumpsBase	(TJumps Layer 0)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Erstellung und Beschreibung aller möglichen 		//
//	  Gittersprünge fuer Mov- und Vac- in der Elementarzelle		//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJumpsBase.h"

// Includes:
#include <iomanip>
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "T3DVector.h"
#include "TStructure/TStructure.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJumpsBase::TJumpsBase (TKMCJob * pJob) {
	m_Job = pJob;

	MovShellCount = KMCVAR_MOVSHELL_INIT;
	JumpStartShellCount = KMCVAR_JUMPSTARTSHELL_INIT;
	JumpTSShellCount = KMCVAR_JUMPTSSHELL_INIT;
	JumpDestShellCount = KMCVAR_JUMPDESTSHELL_INIT;
	Jumps.clear();
	Ready = false;
}

// Destructor
TJumpsBase::~TJumpsBase () {
	Jumps.clear();
}

// **************************** PUBLISHED ********************************* //

// mit Parametern MovShellCount und Jump[..]ShellCount alle Gittersprünge und deren Umgebung finden, und Ready = true setzen
int TJumpsBase::MakeJumps (int i_MovShellCount, int i_JumpStartShellCount, int i_JumpTSShellCount, int i_JumpDestShellCount) {
	if (Ready != false) {
		cout << "Critical Error: TJumps ready too early (in TJumpsBase::MakeJumps)" << endl;
		cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
		cout << i_JumpDestShellCount << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Input pruefen
	if ((i_MovShellCount <= 0) || (i_MovShellCount > KMCVAL_TJUMPS_MAX_MOVSHELLCOUNT)) {
		cout << "Error: Invalid number of lattice jump shells: " << i_MovShellCount << endl;
		cout << "       Number of lattice jump shells has to be an integer in the interval (0 , " << KMCVAL_TJUMPS_MAX_MOVSHELLCOUNT << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Prüfen, ob Struktureingabe vollständig ist
	if (m_Job == 0) {
		cout << "Critical Error: m_Job is null pointer (in TJumpsBase::MakeJumps)" << endl;
		cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
		cout << i_JumpDestShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0) {
		cout << "Critical Error: m_Structure is null pointer (in TJumpsBase::MakeJumps)" << endl;
		cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
		cout << i_JumpDestShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false) {
		cout << "Critical Error: TStructure not ready (in TJumpsBase::MakeJumps)" << endl;
		cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
		cout << i_JumpDestShellCount << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// fuer jedes MovVac-Atom in der Elementarzelle Sprungziele suchen:
	T4DLatticeVector StartPos;					// Startposition des Sprungs
	T3DVector center;							// Suchzentrum der Schalenanalyse
	vector<T4DLatticeVector> no_atoms;			// Liste mit nicht-beruecksichtigten Atomen definieren
	vector<int> elem_ids;						// Liste mit erlaubten Elementen definieren
	elem_ids.push_back(0);
	vector<T4DLatticeVector> out_atoms;			// Output-Liste mit absoluten Umgebungspositionen
	vector<vector<TJump>> i_Jumps;
	int ErrorCode = KMCERR_OK;

	// Get number of jump-start positions
	int mov_anz = m_Job->m_Structure->GetMovAtomAnz();
	if (mov_anz == 0) {
		cout << "Critical Error: No moving atoms detected (in TJumpsBase::MakeJumps)" << endl;
		cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
		cout << i_JumpDestShellCount << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	for (int s = 0; s < mov_anz; s++) {
		StartPos.Set(0,0,0,s);

		// -> Schalenanalyse 
		center = m_Job->m_Structure->Get3DVector(StartPos);	// Zentrum setzen
	
		no_atoms.clear();
		no_atoms.push_back(StartPos);		// Startatom nicht beruecksichtigen

		out_atoms.clear();
		ErrorCode = m_Job->m_Structure->NNAnalysis(center,i_MovShellCount,&elem_ids,&no_atoms,&out_atoms);
		if (ErrorCode != KMCERR_OK) {
			cout << "Critical Error: NN-Analysis failed (in TJumpsBase::MakeJumps)" << endl;
			cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
			cout << i_JumpDestShellCount << endl << endl;
			return ErrorCode;
		}
		if ((int) out_atoms.size() == 0) {
			cout << "Critical Error: NN-Analysis found no shells (in TJumpsBase::MakeJumps)" << endl;
			cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
			cout << i_JumpDestShellCount << endl << endl;
			return KMCERR_NO_SHELLS_FOUND;
		}

		// -> Verarbeitung der ermittelten Sprungziele
		i_Jumps.push_back(vector<TJump> ());
		for (int i = 0; i < (int) out_atoms.size(); i++) {
			i_Jumps[s].push_back(TJump (m_Job));
			ErrorCode = i_Jumps[s][i].SetStartPos(StartPos);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			ErrorCode = i_Jumps[s][i].SetDestPos(out_atoms[i]);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
		}
	}

	// Pruefung, ob alle Rueckspruenge fuer jeden ermittelten Sprung vorhanden sind, sonst hinzufuegen
	T4DLatticeVector ij_Start, ij_Dest, t_Start, t_Dest;
	int backjump_dirid = -1;
	for (int i = 0; i < (int) i_Jumps.size(); i++) {
		for (int j = 0; j < (int) i_Jumps[i].size(); j++) {

			// Start- und Zielvektor ermitteln
			ErrorCode = i_Jumps[i][j].GetStartPos(ij_Start);
			if (ErrorCode != KMCERR_OK) {
				cout << "Critical Error: Cannot retrieve start position (in TJumpsBase::MakeJumps)" << endl;
				cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
				cout << i_JumpDestShellCount << endl << endl;
				return ErrorCode;
			}
			ErrorCode = i_Jumps[i][j].GetDestPos(ij_Dest);
			if (ErrorCode != KMCERR_OK) {
				cout << "Critical Error: Cannot retrieve destination position (in TJumpsBase::MakeJumps)" << endl;
				cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
				cout << i_JumpDestShellCount << endl << endl;
				return ErrorCode;
			}
			if ((ij_Dest.s < 0) || (ij_Dest.s >= (int) i_Jumps.size())) {
				cout << "Critical Error: Invalid destination position (in TJumpsBase::MakeJumps)" << endl;
				cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
				cout << i_JumpDestShellCount << endl << endl;
				return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
			}
			if (i_Jumps[ij_Dest.s].size() == 0) {
				cout << "Critical Error: No jumps for this destination (in TJumpsBase::MakeJumps)" << endl;
				cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
				cout << i_JumpDestShellCount << endl << endl;
				return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
			}

			// Pruefung nicht mehr noetig, wenn RuecksprungID schon korrekt gesetzt
			ErrorCode = i_Jumps[i][j].GetJumpBackjumpDirID(backjump_dirid);
			if (ErrorCode != KMCERR_OK) {
				cout << "Critical Error: Cannot retrieve backjump ID (in TJumpsBase::MakeJumps)" << endl;
				cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
				cout << i_JumpDestShellCount << endl << endl;
				return ErrorCode;
			}
			if ((backjump_dirid >= 0) && (backjump_dirid < (int) i_Jumps[ij_Dest.s].size())) continue;

			// Ruecksprung finden
			backjump_dirid = -1;
			for (int t = 0; t < (int) i_Jumps[ij_Dest.s].size(); t++) {

				ErrorCode = i_Jumps[ij_Dest.s][t].GetStartPos(t_Start);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve backjump start position (in TJumpsBase::MakeJumps)" << endl;
					cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
					cout << i_JumpDestShellCount << endl << endl;
					return ErrorCode;
				}
				ErrorCode = i_Jumps[ij_Dest.s][t].GetDestPos(t_Dest);
				if (ErrorCode != KMCERR_OK) {
					cout << "Critical Error: Cannot retrieve backjump destination position (in TJumpsBase::MakeJumps)" << endl;
					cout << "                Input: " << i_MovShellCount << " " << i_JumpStartShellCount << " " << i_JumpTSShellCount << " ";
					cout << i_JumpDestShellCount << endl << endl;
					return ErrorCode;
				}

				// Wenn rel. Zielvektor 1 == - rel. Zielvektor 2, dann Ruecksprung gefunden
				if (ij_Dest - ij_Start == t_Start - t_Dest) {
					backjump_dirid = t;
					break;
				}
			}
			if (backjump_dirid == -1) {
				i_Jumps[ij_Dest.s].push_back(TJump (m_Job));
				ErrorCode = i_Jumps[ij_Dest.s].back().SetStartPos(T4DLatticeVector(0,0,0,ij_Dest.s));
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				ErrorCode = i_Jumps[ij_Dest.s].back().SetDestPos(T4DLatticeVector(0,0,0,ij_Dest.s) - ij_Dest + ij_Start);
				if (ErrorCode != KMCERR_OK) return ErrorCode;
				backjump_dirid = i_Jumps[ij_Dest.s].size() - 1;
			}

			// Ruecksprung-IDs setzen
			ErrorCode = i_Jumps[i][j].SetBackjumpDirID(backjump_dirid);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			ErrorCode = i_Jumps[ij_Dest.s][backjump_dirid].SetBackjumpDirID(j);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
		}
	}

	// MakeJump fuer die Sprungumgebungen aufrufen
	for (int i = 0; i < (int) i_Jumps.size(); i++) {
		for (int j = 0; j < (int) i_Jumps[i].size(); j++) {
			ErrorCode = i_Jumps[i][j].MakeJump(i_JumpStartShellCount, i_JumpTSShellCount, i_JumpDestShellCount);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
		}
	}

	MovShellCount = i_MovShellCount;
	JumpStartShellCount = i_JumpStartShellCount;
	JumpTSShellCount = i_JumpTSShellCount;
	JumpDestShellCount = i_JumpDestShellCount;
	Jumps = i_Jumps;

	Ready = true;
	if (IfReady() == false) Ready = false;
	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //

// in Jumps UniqueJumpIDs setzen und EnvPos entsprechend Mappings umordnen
int TJumpsBase::SetUniqueJumps (vector<vector<int>> *IDs, vector<vector<vector<int>>> *Mappings) {
	if (IfReady() != true) {
		cout << "Critical Error: TJumps not ready (in TJumpsBase::SetUniqueJump)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if ((IDs->size() != Jumps.size()) || (Mappings->size() != Jumps.size())) {
		cout << "Critical Error: Invalid atom vector size (in TJumpsBase::SetUniqueJumps)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	for (int i = 0; i < (int) Jumps.size(); i++) {
		if ((IDs->at(i).size() != Jumps[i].size()) || (Mappings->at(i).size() != Jumps[i].size())) {
			cout << "Critical Error: Invalid direction vector size (in TJumpsBase::SetUniqueJumps)" << endl << endl;
			return KMCERR_INVALID_INPUT_CRIT;
		}
	}

	// Input versuchen zu übertragen
	int ErrorCode = KMCERR_OK;
	vector<vector<TJump>> t_Jumps (Jumps);
	for (int i = 0; i < (int) t_Jumps.size(); i++) {
		for (int j = 0; j < (int) t_Jumps[i].size(); j++) {
			ErrorCode = t_Jumps[i][j].SetUniqueJump(IDs->at(i).at(j), &Mappings->at(i).at(j));
			if (ErrorCode != KMCERR_OK) return ErrorCode;
		}
	}

	Jumps = t_Jumps;
	return KMCERR_OK;
}

// Rueckgabe von Ready
bool TJumpsBase::IfReady () {
	if ((int) Jumps.size() != 0) {
		for (int i = 0; i < (int) Jumps.size(); i++) {
			if ((int) Jumps[i].size() != 0) {
				for (int j = 0; j < (int) Jumps[i].size(); j++) {
					if (Jumps[i][j].IfReady() == false) return false;
				}
			}
		}
	}
	return Ready;
}

// Elemente in einen Stream schreiben, nur bei Ready == true moeglich
int TJumpsBase::SaveToStream (ostream &output, int offset) {
	if (IfReady() != true) {
		cout << "Critical Error: TJumps not ready (in TJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true) {
		cout << "Critical Error: Output stream not ready (in TJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) {
		cout << "Critical Error: Offset out of bounds (in TJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset,' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int) KMCOUT_TJUMPS_OFFSET > 0) sub_offset = string((int) KMCOUT_TJUMPS_OFFSET,' ');

	// Ausgabe aller Jumps ermitteln
	vector<vector<string>> t_outstr;
	int ErrorCode = KMCERR_OK;
	for (int i = 0; i < (int) Jumps.size(); i++) {
		t_outstr.push_back(vector<string> ());
		for (int j = 0; j < (int) Jumps[i].size(); j++) {
			stringstream t_stream;
			t_stream.precision(output.precision());
			ErrorCode = Jumps[i][j].SaveToStream(t_stream,i,j,offset + KMCOUT_TJUMPS_OFFSET);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			t_outstr[i].push_back(t_stream.str());
		}
	}

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile
	output << s_offset << KMCOUT_TJUMPS_START << endl;

	// Ausgabe der Schalenanzahlen
	output << s_offset << sub_offset << KMCOUT_TJUMPS_MOVSHELLCOUNT << " " << MovShellCount << endl;
	output << s_offset << sub_offset << KMCOUT_TJUMPS_JUMPSTARTSHELLCOUNT << " " << JumpStartShellCount << endl;
	output << s_offset << sub_offset << KMCOUT_TJUMPS_JUMPTSSHELLCOUNT << " " << JumpTSShellCount << endl;
	output << s_offset << sub_offset << KMCOUT_TJUMPS_JUMPDESTSHELLCOUNT << " " << JumpDestShellCount << endl;
	output << endl;

	// Ausgabe der Sprunganzahlen pro Mov-Atom der Elementarzelle
	int ID_space = IntToStr((int) Jumps.size()).length();
	if (ID_space < (int) ((string) KMCOUT_TJUMPS_ATOMID).length()) ID_space = ((string) KMCOUT_TJUMPS_ATOMID).length();
	int Count_space = ((string) KMCOUT_TJUMPS_COUNT).length();
	for (int i = 0; i < (int) Jumps.size(); i++) {
		if (Count_space < (int) IntToStr((int) Jumps[i].size()).length()) Count_space = IntToStr((int) Jumps[i].size()).length();
	}
	output << s_offset << sub_offset << setw(((string) KMCOUT_TJUMPS_JUMPCOUNT).length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TJUMPS_ATOMID << " " << setw(Count_space) << KMCOUT_TJUMPS_COUNT << endl;
	for (int i = 0; i < (int) Jumps.size(); i++) {
		output << s_offset << sub_offset << KMCOUT_TJUMPS_JUMPCOUNT << " ";
		output << setw(ID_space) << i << " " << setw(Count_space) << (int) Jumps[i].size() << endl;
	}

	// Ausgabe der TJump
	for (int i = 0; i < (int) t_outstr.size(); i++) {
		for (int j = 0; j < (int) t_outstr[i].size(); j++) {
			output << endl;
			output << t_outstr[i][j];
			output.flush();
		}
	}

	output << s_offset << KMCOUT_TJUMPS_END << endl;

	if (output.fail() == true) {
		cout << "Critical Error: Output stream failure (in TJumpsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Elemente aus einem Stream laden, nur bei Ready == false moeglich
int TJumpsBase::LoadFromStream (istream &input) {
	if (Ready != false) {
		cout << "Critical Error: TJumps ready too early (in TJumpsBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	
	// Input pruefen
	if (input.good() == false) {
		cout << "Critical Error: Input stream not ready (in TJumpsBase::LoadFromStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Inputparameter setzen
	input >> skipws;

	// Bisherigen Input zwischenspeichern und loeschen
	int t_MovShellCount = MovShellCount;
	MovShellCount = KMCVAR_MOVSHELL_INIT;
	int t_JumpStartShellCount = JumpStartShellCount;
	JumpStartShellCount = KMCVAR_JUMPSTARTSHELL_INIT;
	int t_JumpTSShellCount = JumpTSShellCount;
	JumpTSShellCount = KMCVAR_JUMPTSSHELL_INIT;
	int t_JumpDestShellCount = JumpDestShellCount;
	JumpDestShellCount = KMCVAR_JUMPDESTSHELL_INIT;
	vector<vector<TJump>> t_Jumps (Jumps);
	Jumps.clear();

	// Daten einlesen
	int i_MovShellCount = -1;
	int i_JumpStartShellCount = -1;
	int i_JumpTSShellCount = -1;
	int i_JumpDestShellCount = -1;
	vector<int> i_JumpCounts;
	vector<vector<bool>> i_JumpChecked;
	
	string line = "";
	bool if_failed = false;
	bool make_done = false;
	string s_temp = "";
	int i_temp = 0;
	int i2_temp = 0;
	int ErrorCode = KMCERR_OK;
	while (input.good() == true) {
		// Zeile laden
		if (getline(input,line).fail() == true) {
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream (line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TJUMPS_MOVSHELLCOUNT) {
			if ((linestream >> i_MovShellCount).fail() == true) {
				if_failed = true;
				break;
			}
			if ((i_MovShellCount != -1) && (i_JumpStartShellCount != -1) && (i_JumpTSShellCount != -1) && (i_JumpDestShellCount != -1) && (make_done == false)) {
				ErrorCode = MakeJumps(i_MovShellCount,i_JumpStartShellCount,i_JumpTSShellCount,i_JumpDestShellCount);
				if (ErrorCode != KMCERR_OK) {
					if_failed = true;
					break;
				}
				make_done = true;
				i_JumpChecked = vector<vector<bool>> (Jumps.size(),vector<bool> ());
				for (int i = 0; i < (int) Jumps.size(); i++) {
					i_JumpChecked[i] = vector<bool> (Jumps[i].size(), false);
				}
			}
		} else if (s_temp == KMCOUT_TJUMPS_JUMPSTARTSHELLCOUNT) {
			if ((linestream >> i_JumpStartShellCount).fail() == true) {
				if_failed = true;
				break;
			}
			if ((i_MovShellCount != -1) && (i_JumpStartShellCount != -1) && (i_JumpTSShellCount != -1) && (i_JumpDestShellCount != -1) && (make_done == false)) {
				ErrorCode = MakeJumps(i_MovShellCount,i_JumpStartShellCount,i_JumpTSShellCount,i_JumpDestShellCount);
				if (ErrorCode != KMCERR_OK) {
					if_failed = true;
					break;
				}
				make_done = true;
				i_JumpChecked = vector<vector<bool>> (Jumps.size(),vector<bool> ());
				for (int i = 0; i < (int) Jumps.size(); i++) {
					i_JumpChecked[i] = vector<bool> (Jumps[i].size(), false);
				}
			}
		} else if (s_temp == KMCOUT_TJUMPS_JUMPTSSHELLCOUNT) {
			if ((linestream >> i_JumpTSShellCount).fail() == true) {
				if_failed = true;
				break;
			}
			if ((i_MovShellCount != -1) && (i_JumpStartShellCount != -1) && (i_JumpTSShellCount != -1) && (i_JumpDestShellCount != -1) && (make_done == false)) {
				ErrorCode = MakeJumps(i_MovShellCount,i_JumpStartShellCount,i_JumpTSShellCount,i_JumpDestShellCount);
				if (ErrorCode != KMCERR_OK) {
					if_failed = true;
					break;
				}
				make_done = true;
				i_JumpChecked = vector<vector<bool>> (Jumps.size(),vector<bool> ());
				for (int i = 0; i < (int) Jumps.size(); i++) {
					i_JumpChecked[i] = vector<bool> (Jumps[i].size(), false);
				}
			}
		} else if (s_temp == KMCOUT_TJUMPS_JUMPDESTSHELLCOUNT) {
			if ((linestream >> i_JumpDestShellCount).fail() == true) {
				if_failed = true;
				break;
			}
			if ((i_MovShellCount != -1) && (i_JumpStartShellCount != -1) && (i_JumpTSShellCount != -1) && (i_JumpDestShellCount != -1) && (make_done == false)) {
				ErrorCode = MakeJumps(i_MovShellCount,i_JumpStartShellCount,i_JumpTSShellCount,i_JumpDestShellCount);
				if (ErrorCode != KMCERR_OK) {
					if_failed = true;
					break;
				}
				make_done = true;
				i_JumpChecked = vector<vector<bool>> (Jumps.size(),vector<bool> ());
				for (int i = 0; i < (int) Jumps.size(); i++) {
					i_JumpChecked[i] = vector<bool> (Jumps[i].size(), false);
				}
			}
		} else if (s_temp == KMCOUT_TJUMPS_JUMPCOUNT) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp != (int) i_JumpCounts.size()) {
				if_failed = true;
				break;
			}
			i_JumpCounts.push_back(0);
			if ((linestream >> i_JumpCounts.back()).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TJUMP_START1) {
			if (make_done == false) {
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}			
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}			
			if ((linestream >> i2_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if ((i_temp < 0) || (i_temp >= (int) Jumps.size())) {
				if_failed = true;
				break;
			}
			if ((i2_temp < 0) || (i2_temp >= (int) Jumps[i_temp].size())) {
				if_failed = true;
				break;
			}
			if (i_JumpChecked[i_temp][i2_temp] == true) {
				if_failed = true;
				break;
			}
			ErrorCode = Jumps[i_temp][i2_temp].LoadFromStream(input);
			if (ErrorCode != KMCERR_OK) {
				if_failed = true;
				break;
			}
			i_JumpChecked[i_temp][i2_temp] = true;
		} else if (s_temp == KMCOUT_TJUMPS_END) break;
	}
	if (make_done == false) if_failed = true;
	if ((int) i_JumpCounts.size() == 0) if_failed = true;
	if (i_JumpCounts.size() != Jumps.size()) if_failed = true;
	if (if_failed == false) {
		for (int i = 0; i < (int) Jumps.size(); i++) {
			if (i_JumpCounts[i] != (int) Jumps[i].size()) {
				if_failed = true;
				break;
			}
			for (int j = 0; j < (int) Jumps[i].size(); j++) {
				if (i_JumpChecked[i][j] == false) {
					if_failed = true;
					break;
				}
			}
			if (if_failed == true) break;
		}
	}
	if ((if_failed == true) && (ErrorCode == KMCERR_OK)) {
		cout << "Error: Invalid file format" << endl << endl;
		ErrorCode = KMCERR_INVALID_FILE_FORMAT;
	}
	
	// Falls Fehler aufgetreten ist, alte Daten wiederherstellen und Fehlercode ausgeben
	if (if_failed == true) {
		MovShellCount = t_MovShellCount;
		JumpStartShellCount = t_JumpStartShellCount;
		JumpTSShellCount = t_JumpTSShellCount;
		JumpDestShellCount = t_JumpDestShellCount;
		Jumps = t_Jumps;
		Ready = false;
		return ErrorCode;
	}
	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //