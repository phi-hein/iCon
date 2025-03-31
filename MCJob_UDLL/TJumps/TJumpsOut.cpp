// **************************************************************** //
//																	//
//	Klasse: TJumpsOut	(TJumps Layer 2)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Erstellung und Beschreibung aller möglichen 		//
//	  Gittersprünge fuer Mov- und Vac- in der Elementarzelle		//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJumps/TJumpsOut.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJumpsOut::TJumpsOut (TKMCJob * pJob): TJumpsFunc (pJob) {
	
}

// Destructor
TJumpsOut::~TJumpsOut () {
	
}

// **************************** PUBLISHED ********************************* //

// Schalenanzahlen fuer Sprungrichtungen und Sprungumgebungen ausgeben
int TJumpsOut::GetShellCounts (int &movcount, int &jumpstartcount, int &jumptscount, int &jumpdestcount) {

	// Ausgabe
	movcount = MovShellCount;
	jumpstartcount = JumpStartShellCount;
	jumptscount = JumpTSShellCount;
	jumpdestcount = JumpDestShellCount;

	return KMCERR_OK;
}

// Anzahl an Sprungrichtungen fuer bestimmtes Mov-Atom in Elementarzelle
int TJumpsOut::GetJumpDirCount (int atom, int &count) {
	if ((IfReady() == false) || ((int) Jumps.size() == 0)) {
		count = 0;
		return KMCERR_OK;
	}

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative vector index (in TJumpsOut::GetJumpDirCount)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds vector size (in TJumpsOut::GetJumpDirCount)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Ausgabe
	count = (int) Jumps[atom].size();

	return KMCERR_OK;
}

// 4D-Vektor des Sprungstartatoms ausgeben
int TJumpsOut::GetJumpStartPos(int atom, int dir, int &x, int &y, int &z, int &s) {

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative atom vector index (in TJumpsOut::GetJumpStartPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds atom vector size (in TJumpsOut::GetJumpStartPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir < 0) {
		cout << "Critical Error: Negative direction vector index (in TJumpsOut::GetJumpStartPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir >= (int) Jumps[atom].size()) {
		cout << "Critical Error: Index exceeds direction vector size (in TJumpsOut::GetJumpStartPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Jumps[atom][dir].GetJumpStartPos(x,y,z,s);
}

// 4D-Vektor des Sprungzielatoms ausgeben
int TJumpsOut::GetJumpDestPos(int atom, int dir, int &x, int &y, int &z, int &s) {

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative atom vector index (in TJumpsOut::GetJumpDestPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds atom vector size (in TJumpsOut::GetJumpDestPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir < 0) {
		cout << "Critical Error: Negative direction vector index (in TJumpsOut::GetJumpDestPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir >= (int) Jumps[atom].size()) {
		cout << "Critical Error: Index exceeds direction vector size (in TJumpsOut::GetJumpDestPos)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Jumps[atom][dir].GetJumpDestPos(x,y,z,s);
}

// Ruecksprung-DirID ausgeben
int TJumpsOut::GetJumpBackjumpDirID(int atom, int dir, int &dirID) {

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative atom vector index (in TJumpsOut::GetJumpBackjumpDirID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds atom vector size (in TJumpsOut::GetJumpBackjumpDirID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir < 0) {
		cout << "Critical Error: Negative direction vector index (in TJumpsOut::GetJumpBackjumpDirID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir >= (int) Jumps[atom].size()) {
		cout << "Critical Error: Index exceeds direction vector size (in TJumpsOut::GetJumpBackjumpDirID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Jumps[atom][dir].GetJumpBackjumpDirID(dirID);
}

// Anzahl an Umgebungsatomen ausgeben
int TJumpsOut::GetJumpEnvCount(int atom, int dir, int &count) {

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative atom vector index (in TJumpsOut::GetJumpEnvCount)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds atom vector size (in TJumpsOut::GetJumpEnvCount)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir < 0) {
		cout << "Critical Error: Negative direction vector index (in TJumpsOut::GetJumpEnvCount)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir >= (int) Jumps[atom].size()) {
		cout << "Critical Error: Index exceeds direction vector size (in TJumpsOut::GetJumpEnvCount)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Jumps[atom][dir].GetJumpEnvCount(count);
}

// Umgebungsatom ausgeben
int TJumpsOut::GetJumpEnvAtom(int atom, int dir, int id, int &x, int &y, int &z, int &s) {

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative atom vector index (in TJumpsOut::GetJumpEnvAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds atom vector size (in TJumpsOut::GetJumpEnvAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir < 0) {
		cout << "Critical Error: Negative direction vector index (in TJumpsOut::GetJumpEnvAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir >= (int) Jumps[atom].size()) {
		cout << "Critical Error: Index exceeds direction vector size (in TJumpsOut::GetJumpEnvAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Jumps[atom][dir].GetJumpEnvAtom(id,x,y,z,s);
}

// ID des zugehörigen UniqueJump ausgeben
int TJumpsOut::GetJumpUniqueJumpID(int atom, int dir, int &uniqueid) {

	// Input pruefen
	if (atom < 0) {
		cout << "Critical Error: Negative atom vector index (in TJumpsOut::GetJumpUniqueJumpID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (atom >= (int) Jumps.size()) {
		cout << "Critical Error: Index exceeds atom vector size (in TJumpsOut::GetJumpUniqueJumpID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir < 0) {
		cout << "Critical Error: Negative direction vector index (in TJumpsOut::GetJumpUniqueJumpID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (dir >= (int) Jumps[atom].size()) {
		cout << "Critical Error: Index exceeds direction vector size (in TJumpsOut::GetJumpUniqueJumpID)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	return Jumps[atom][dir].GetJumpUniqueJumpID(uniqueid);
}
		
// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //