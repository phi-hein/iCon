// **************************************************************** //
//																	//
//	Klasse: TJumpBase	(TJump Layer 0)								//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines Gittersprungs eines Atoms in  	//
//	  der Elementarzelle und Erstellung der Sprungumgebung			//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJump/TJumpBase.h"

// Includes:
#include <iomanip>
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TStructure/TStructure.h"
#include "TJumps/TJumps.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TJumpBase::TJumpBase (TKMCJob * pJob) {
	m_Job = pJob;

	StartPos.Set(0,0,0,-1);
	DestPos.Set(0,0,0,0);
	BackjumpDirID = -1;
	EnvPos.clear();
	UniqueJumpID = -1;
	Ready = false;
}

// Destructor
TJumpBase::~TJumpBase () {

}

// **************************** PUBLISHED ********************************* //
		
	

// ***************************** PUBLIC *********************************** //

// absolute 4D-Position des Startatoms setzen
int TJumpBase::SetStartPos (T4DLatticeVector pos) {
	if (Ready != false) {
		cout << "Critical Error: TJump ready too early (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Gueltigkeit des Inputs pruefen
	if ((pos.x != 0) || (pos.y != 0) || (pos.z != 0)) {
		cout << "Critical Error: Start position not in (0,0,0)-unit cell (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (pos.s < 0) {
		cout << "Critical Error: Negative vector index (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (m_Job == 0) {
		cout << "Critical Error: m_Job is null pointer (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0) {
		cout << "Critical Error: m_Structure is null pointer (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false) {
		cout << "Critical Error: TStructure not ready (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (pos.s >= m_Job->m_Structure->GetMovAtomAnz()) {
		cout << "Critical Error: Index s exceeds size (in TJumpBase::SetStartPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Absolute Startposition speichern (und Zielposition auf Null setzen)
	StartPos = pos;
	DestPos.Set(0,0,0,0);
	BackjumpDirID = -1;

	return KMCERR_OK;
}

// absolute 4D-Position des Zielatoms setzen
int TJumpBase::SetDestPos (T4DLatticeVector pos) {
	if (Ready != false) {
		cout << "Critical Error: TJump ready too early (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Gueltigkeit des Inputs pruefen
	if (StartPos.s == -1) {
		cout << "Critical Error: Start position not specified (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}		
	if (pos.s < 0) {
		cout << "Critical Error: Negative vector index (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (m_Job == 0) {
		cout << "Critical Error: m_Job is null pointer (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0) {
		cout << "Critical Error: m_Structure is null pointer (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false) {
		cout << "Critical Error: TStructure not ready (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (pos.s >= m_Job->m_Structure->GetMovAtomAnz()) {
		cout << "Critical Error: Index s exceeds size (in TJumpBase::SetDestPos)" << endl;
		cout << "                Input: " << pos.x << " " << pos.y << " " << pos.z << " " << pos.s << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Relative Zielposition speichern
	DestPos = pos - StartPos;
	BackjumpDirID = -1;

	return KMCERR_OK;
}

// DirID des Ruecksprungs setzen
int TJumpBase::SetBackjumpDirID (int dirID) {
	if (Ready != false) {
		cout << "Critical Error: TJump ready too early (in TJumpBase::SetBackjumpDirID)" << endl;
		cout << "                Input: " << dirID << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}
	if (StartPos.s == -1) {
		cout << "Critical Error: Start position not specified (in TJumpBase::SetBackjumpDirID)" << endl;
		cout << "                Input: " << dirID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (DestPos == T4DLatticeVector(0,0,0,0)) {
		cout << "Critical Error: Destination position not specified (in TJumpBase::SetBackjumpDirID)" << endl;
		cout << "                Input: " << dirID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dirID < 0) {
		cout << "Critical Error: Negative vector index (in TJumpBase::SetBackjumpDirID)" << endl;
		cout << "                Input: " << dirID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Ruecksprung-Richtungs-ID speichern
	BackjumpDirID = dirID;

	return KMCERR_OK;
}

// Schalenanalyse zur Bestimmung der Sprungumgebungs-4D-Vektoren, Ready = true setzen
int TJumpBase::MakeJump (int JumpStartShellCount, int JumpTSShellCount, int JumpDestShellCount) {
	if (Ready != false) {
		cout << "Critical Error: TJump ready too early (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Konsistenz des Inputs pruefen (ShellCount ok, StartPos und DestPos gesetzt, TStructure ready)
	if ((JumpStartShellCount < 0) || (JumpStartShellCount > KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT)) {
		cout << "Error: Invalid number of environment shells around jump start position: " << JumpStartShellCount << endl;
		cout << "       Number of environment shells has to be an integer in the interval (0 , " << KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((JumpTSShellCount < 0) || (JumpTSShellCount > KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT)) {
		cout << "Error: Invalid number of environment shells around jump transition state: " << JumpTSShellCount << endl;
		cout << "       Number of environment shells has to be an integer in the interval (0 , " << KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((JumpDestShellCount < 0) || (JumpDestShellCount > KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT)) {
		cout << "Error: Invalid number of environment shells around jump destination: " << JumpDestShellCount << endl;
		cout << "       Number of environment shells has to be an integer in the interval (0 , " << KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (JumpStartShellCount + JumpTSShellCount + JumpDestShellCount <= 0) {
		cout << "Error: Invalid sum of environment shells: " << JumpStartShellCount + JumpTSShellCount + JumpDestShellCount << endl;
		cout << "       Sum of the jump environment shells has to be greater than zero." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (StartPos.s == -1) {
		cout << "Critical Error: Start position not set (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if ((DestPos.x == 0) && (DestPos.y == 0) && (DestPos.z == 0) && (DestPos.s == 0)) {
		cout << "Critical Error: Destination position not set (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (BackjumpDirID < 0) {
		cout << "Critical Error: Backjump not set (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (m_Job == 0) {
		cout << "Critical Error: m_Job is null pointer (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0) {
		cout << "Critical Error: m_Structure is null pointer (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false) {
		cout << "Critical Error: TStructure not ready (in TJumpBase::MakeJump)" << endl;
		cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Schalenanalyse
	T3DVector jump_center;
	vector<T4DLatticeVector> no_atoms;			// Liste mit nicht-beruecksichtigten Atomen definieren
	no_atoms.push_back(StartPos);
	no_atoms.push_back(StartPos + DestPos);
	vector<int> elem_ids;						// Liste mit erlaubten Elementen definieren, leer = alle Elemente erlaubt
	vector<T4DLatticeVector> out_atoms;
	vector<T4DLatticeVector> env_atoms;
	int ErrorCode = KMCERR_OK;
	bool if_found = false;

	// Schalenanalyse um Startposition
	if (JumpStartShellCount != 0) {
		jump_center = m_Job->m_Structure->Get3DVector(StartPos);

		out_atoms.clear();							// Output-Liste mit absoluten Umgebungspositionen
		ErrorCode = m_Job->m_Structure->NNAnalysis(jump_center,JumpStartShellCount,&elem_ids,&no_atoms,&out_atoms);
		if (ErrorCode != KMCERR_OK) {
			cout << "Critical Error: NN-Analysis around start failed (in TJumpBase::MakeJump)" << endl;
			cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
			return ErrorCode;
		}
		if ((int) out_atoms.size() == 0) {
			cout << "Critical Error: NN-Analysis around start found no shells (in TJumpBase::MakeJump)" << endl;
			cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
			return KMCERR_NO_SHELLS_FOUND;
		}

		for (int i = 0; i < (int) out_atoms.size(); i++) {
			if_found = false;
			if ((int) env_atoms.size() != 0) {
				for (int j = 0; j < (int) env_atoms.size(); j++) {
					if (env_atoms[j] == out_atoms[i] - StartPos) {
						if_found = true;
						break;
					}
				}
			}
			if (if_found == false) {
				env_atoms.push_back(out_atoms[i] - StartPos);		// absolute Positionen zu relativen Positionen umsetzen
			}
		}
	}

	// Schalenanalyse um Uebergangszustand
	if (JumpTSShellCount != 0) {
		jump_center = m_Job->m_Structure->Get3DVector(StartPos) + 
			(m_Job->m_Structure->Get3DVector(StartPos + DestPos) - m_Job->m_Structure->Get3DVector(StartPos))*0.5;

		out_atoms.clear();						// Output-Liste mit absoluten Umgebungspositionen
		ErrorCode = m_Job->m_Structure->NNAnalysis(jump_center,JumpTSShellCount,&elem_ids,&no_atoms,&out_atoms);
		if (ErrorCode != KMCERR_OK) {
			cout << "Critical Error: NN-Analysis around TS failed (in TJumpBase::MakeJump)" << endl;
			cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
			return ErrorCode;
		}
		if ((int) out_atoms.size() == 0) {
			cout << "Critical Error: NN-Analysis around TS found no shells (in TJumpBase::MakeJump)" << endl;
			cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
			return KMCERR_NO_SHELLS_FOUND;
		}

		for (int i = 0; i < (int) out_atoms.size(); i++) {
			if_found = false;
			if ((int) env_atoms.size() != 0) {
				for (int j = 0; j < (int) env_atoms.size(); j++) {
					if (env_atoms[j] == out_atoms[i] - StartPos) {
						if_found = true;
						break;
					}
				}
			}
			if (if_found == false) {
				env_atoms.push_back(out_atoms[i] - StartPos);		// absolute Positionen zu relativen Positionen umsetzen
			}
		}
	}

	// Schalenanalyse um Endposition
	if (JumpDestShellCount != 0) {
		jump_center = m_Job->m_Structure->Get3DVector(StartPos + DestPos);

		out_atoms.clear();						// Output-Liste mit absoluten Umgebungspositionen
		ErrorCode = m_Job->m_Structure->NNAnalysis(jump_center,JumpDestShellCount,&elem_ids,&no_atoms,&out_atoms);
		if (ErrorCode != KMCERR_OK) {
			cout << "Critical Error: NN-Analysis around destination failed (in TJumpBase::MakeJump)" << endl;
			cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
			return ErrorCode;
		}
		if ((int) out_atoms.size() == 0) {
			cout << "Critical Error: NN-Analysis around destination found no shells (in TJumpBase::MakeJump)" << endl;
			cout << "                Input: " << JumpStartShellCount << " " << JumpTSShellCount << " " << JumpDestShellCount << endl << endl;
			return KMCERR_NO_SHELLS_FOUND;
		}

		for (int i = 0; i < (int) out_atoms.size(); i++) {
			if_found = false;
			if ((int) env_atoms.size() != 0) {
				for (int j = 0; j < (int) env_atoms.size(); j++) {
					if (env_atoms[j] == out_atoms[i] - StartPos) {
						if_found = true;
						break;
					}
				}
			}
			if (if_found == false) {
				env_atoms.push_back(out_atoms[i] - StartPos);		// absolute Positionen zu relativen Positionen umsetzen
			}
		}
	}

	EnvPos = env_atoms;
	Ready = true;
	return KMCERR_OK;
}

// UniqueJumpID setzen und EnvPos entsprechend mapping umordnen
int TJumpBase::SetUniqueJump (int ID, vector<int> *mapping) {
	if (Ready != true) {
		cout << "Critical Error: TJump not ready (in TJumpBase::SetUniqueJump)" << endl;
		cout << "                Input: " << ID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input prüfen
	if (ID < 0) {
		cout << "Critical Error: Negative vector index (in TJumpBase::SetUniqueJump)" << endl;
		cout << "                Input: " << ID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((mapping->size() != 0) && (mapping->size() != EnvPos.size())) {
		cout << "Critical Error: Inconsistent vector sizes (in TJumpBase::SetUniqueJump)" << endl;
		cout << "                Input: " << ID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// UniqueJumpID setzen
	UniqueJumpID = ID;

	// EnvPos umordnen
	if (mapping->size() != 0) {
		vector<T4DLatticeVector> sort_help ((int) EnvPos.size(),T4DLatticeVector ());
		for (int i = 0; i < (int) EnvPos.size(); i++) {
			sort_help[mapping->at(i)] = EnvPos[i];
		}
		EnvPos = sort_help;
	}

	return KMCERR_OK;
}

// Rueckgabe von Ready
bool TJumpBase::IfReady () {
	return Ready;
}

// Elemente in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
int TJumpBase::SaveToStream (ostream &output, int AtomID, int DirID, int offset) {
	if (Ready != true) {
		cout << "Critical Error: TJump not ready (in TJumpBase::SaveToStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if ((AtomID < 0) || (DirID < 0)) {
		cout << "Critical Error: Negative vector indices (in TJumpBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (output.fail() == true) {
		cout << "Critical Error: Output stream not ready (in TJumpBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) {
		cout << "Critical Error: Offset out of bounds (in TJumpBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset,' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int) KMCOUT_TJUMP_OFFSET > 0) sub_offset = string((int) KMCOUT_TJUMP_OFFSET,' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile
	output << s_offset << KMCOUT_TJUMP_START1 << " " << KMCOUT_TJUMP_ATOMID << " " << AtomID;
	output << " " << KMCOUT_TJUMP_DIRID << " " << DirID << " " << KMCOUT_TJUMP_START2 << endl;

	// Ausgabe von Backjump-ID und UniqueJump-ID
	output << s_offset << sub_offset << KMCOUT_TJUMP_BACKJUMPDIRID << " " << BackjumpDirID << endl;
	output << s_offset << sub_offset << KMCOUT_TJUMP_UNIQUEID << " ";
	if (UniqueJumpID == -1) {
		output << KMCOUT_TJUMP_UNIQUEID_UNDEF << endl;
	} else {
		output << UniqueJumpID << endl;
	}
	output << endl;

	// Ausgabe des Startatomvektors
	int x_space = IntToStr(StartPos.x).length();
	int y_space = IntToStr(StartPos.y).length();
	int z_space = IntToStr(StartPos.z).length();
	int s_space = IntToStr(StartPos.s).length();
	if (x_space < (int) ((string) KMCOUT_TJUMP_X).length()) x_space = ((string) KMCOUT_TJUMP_X).length();
	if (y_space < (int) ((string) KMCOUT_TJUMP_Y).length()) y_space = ((string) KMCOUT_TJUMP_Y).length();
	if (z_space < (int) ((string) KMCOUT_TJUMP_Z).length()) z_space = ((string) KMCOUT_TJUMP_Z).length();
	if (s_space < (int) ((string) KMCOUT_TJUMP_S).length()) s_space = ((string) KMCOUT_TJUMP_S).length();
	output << s_offset << sub_offset << setw(KMCOUT_TJUMP_STARTATOM.front().length() + 1) << " " << "( ";
	output << setw(x_space) << KMCOUT_TJUMP_X << " ";
	output << setw(y_space) << KMCOUT_TJUMP_Y << " ";
	output << setw(z_space) << KMCOUT_TJUMP_Z << " ";
	output << setw(s_space) << KMCOUT_TJUMP_S << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TJUMP_STARTATOM << " " << "( ";
	output << setw(x_space) << StartPos.x << " " << setw(y_space) << StartPos.y << " ";
	output << setw(z_space) << StartPos.z << " " << setw(s_space) << StartPos.s << " )" << endl;
	output << endl;

	// Ausgabe des Zielatomvektors
	x_space = IntToStr(DestPos.x).length();
	y_space = IntToStr(DestPos.y).length();
	z_space = IntToStr(DestPos.z).length();
	s_space = IntToStr(DestPos.s).length();
	if (x_space < (int) ((string) KMCOUT_TJUMP_RELX).length()) x_space = ((string) KMCOUT_TJUMP_RELX).length();
	if (y_space < (int) ((string) KMCOUT_TJUMP_RELY).length()) y_space = ((string) KMCOUT_TJUMP_RELY).length();
	if (z_space < (int) ((string) KMCOUT_TJUMP_RELZ).length()) z_space = ((string) KMCOUT_TJUMP_RELZ).length();
	if (s_space < (int) ((string) KMCOUT_TJUMP_RELS).length()) s_space = ((string) KMCOUT_TJUMP_RELS).length();
	output << s_offset << sub_offset << setw(KMCOUT_TJUMP_DESTATOM.front().length() + 1) << " " << "( ";
	output << setw(x_space) << KMCOUT_TJUMP_RELX << " ";
	output << setw(y_space) << KMCOUT_TJUMP_RELY << " ";
	output << setw(z_space) << KMCOUT_TJUMP_RELZ << " ";
	output << setw(s_space) << KMCOUT_TJUMP_RELS << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TJUMP_DESTATOM << " " << "( ";
	output << setw(x_space) << DestPos.x << " " << setw(y_space) << DestPos.y << " ";
	output << setw(z_space) << DestPos.z << " " << setw(s_space) << DestPos.s << " )" << endl;
	output << endl;

	// Ausgabe der Umgebungsatomvektoren
	x_space = ((string) KMCOUT_TJUMP_RELX).length();
	y_space = ((string) KMCOUT_TJUMP_RELY).length();
	z_space = ((string) KMCOUT_TJUMP_RELZ).length();
	s_space = ((string) KMCOUT_TJUMP_RELS).length();
	for (int i = 0; i < (int) EnvPos.size(); i++) {
		if (x_space < (int) IntToStr(EnvPos[i].x).length()) x_space = IntToStr(EnvPos[i].x).length();
		if (y_space < (int) IntToStr(EnvPos[i].y).length()) y_space = IntToStr(EnvPos[i].y).length();
		if (z_space < (int) IntToStr(EnvPos[i].z).length()) z_space = IntToStr(EnvPos[i].z).length();
		if (s_space < (int) IntToStr(EnvPos[i].s).length()) s_space = IntToStr(EnvPos[i].s).length();
	}
	int ID_space = IntToStr((int) EnvPos.size()).length();
	if (ID_space < (int) ((string) KMCOUT_TJUMP_ENVATOMID).length()) ID_space = ((string) KMCOUT_TJUMP_ENVATOMID).length();
	output << s_offset << sub_offset << setw(KMCOUT_TJUMP_ENVATOM.front().length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TJUMP_ENVATOMID << " ( ";
	output << setw(x_space) << KMCOUT_TJUMP_RELX << " ";
	output << setw(y_space) << KMCOUT_TJUMP_RELY << " ";
	output << setw(z_space) << KMCOUT_TJUMP_RELZ << " ";
	output << setw(s_space) << KMCOUT_TJUMP_RELS << " )" << endl;
	for (int i = 0; i < (int) EnvPos.size(); i++) {
		output << s_offset << sub_offset << KMCOUT_TJUMP_ENVATOM << " " << setw(ID_space) << i << " ( ";
		output << setw(x_space) << EnvPos[i].x << " " << setw(y_space) << EnvPos[i].y << " ";
		output << setw(z_space) << EnvPos[i].z << " " << setw(s_space) << EnvPos[i].s << " )" << endl;
	}	
	output << s_offset << KMCOUT_TJUMP_END << endl;

	if (output.fail() == true) {
		cout << "Critical Error: Output stream failure (in TJumpBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Elemente aus einem Stream laden, nur bei Ready == true moeglich
int TJumpBase::LoadFromStream (istream &input) {
	if (Ready != true) {
		cout << "Critical Error: TJump not ready (in TJumpBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (input.good() == false) {
		cout << "Critical Error: Input stream not ready (in TJumpBase::LoadFromStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	int i_BackjumpDirID = -1;
	int i_UniqueJumpID = -1;
	T4DLatticeVector i_StartPos (0,0,0,-1);
	T4DLatticeVector i_DestPos (0,0,0,0);
	vector<T4DLatticeVector> i_EnvPos;
	
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

		if (s_temp == KMCOUT_TJUMP_UNIQUEID) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TJUMP_UNIQUEID_UNDEF) {
				i_UniqueJumpID = -1;
			} else {
				i_UniqueJumpID = StrToInt(s_temp,-1);
			}
		} else if (s_temp == KMCOUT_TJUMP_BACKJUMPDIRID) {
			if ((linestream >> i_BackjumpDirID).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TJUMP_STARTATOM) {
			if ((linestream >> s_temp).fail() == true) {	// "("
				if_failed = true;
				break;
			}			
			if ((linestream >> i_StartPos.x).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_StartPos.y).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_StartPos.z).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_StartPos.s).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TJUMP_DESTATOM) {
			if ((linestream >> s_temp).fail() == true) {	// "("
				if_failed = true;
				break;
			}			
			if ((linestream >> i_DestPos.x).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_DestPos.y).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_DestPos.z).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_DestPos.s).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TJUMP_ENVATOM) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp != (int) i_EnvPos.size()) {
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true) {	// "("
				if_failed = true;
				break;
			}
			i_EnvPos.push_back(T4DLatticeVector ());
			if ((linestream >> i_EnvPos.back().x).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_EnvPos.back().y).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_EnvPos.back().z).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_EnvPos.back().s).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TJUMP_END) break;
	}
	if ((int) i_EnvPos.size() == 0) if_failed = true;
	if (if_failed == true) {
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Mit bisherigem Input vergleichen
	if ((i_StartPos != StartPos) || (i_DestPos != DestPos) || (i_EnvPos.size() != EnvPos.size()) || (i_BackjumpDirID != BackjumpDirID)) {
		cout << "Error: Inconsistent file content" << endl << endl;
		return KMCERR_INVALID_FILE_CONTENT;
	}
	if (i_UniqueJumpID == -1) {		// weil: nur dann wurde Umgebung unsortiert gespeichert
		for (int i = 0; i < (int) EnvPos.size(); i++) {
			if (i_EnvPos[i] != EnvPos[i]) {
				cout << "Error: Inconsistent file content" << endl << endl;
				return KMCERR_INVALID_FILE_CONTENT;
			}
		}
	}
	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //