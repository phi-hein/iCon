// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpsFunc	(TUniqueJumps Layer 1)				//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der einzigartigen Gitterspruenge 		//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TUniqueJumps/TUniqueJumpsFunc.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "ErrorCodes.h"
#include "TSimulation/TSimUniqueJump.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TUniqueJumpsFunc::TUniqueJumpsFunc (TKMCJob * pJob): TUniqueJumpsBase (pJob) {
	
}

// Destructor
TUniqueJumpsFunc::~TUniqueJumpsFunc () {
	
}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Minimalbeschreibung aller aktiven, einzigartigen Spruenge fuer die Simulation und Map: (alte UniqueID -> ID in Array) erstellen
int TUniqueJumpsFunc::CreateSimUniqueJumps (TSimUniqueJump *&o_simuniquejumps, vector<int> *o_newuniqueidmap) {
	if (IfJumpsReady() != true) return KMCERR_READY_NOT_TRUE;
	if (IfCodesReady() != true) return KMCERR_READY_NOT_TRUE;

	if (UJumps.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	if (o_newuniqueidmap == NULL) return KMCERR_INVALID_POINTER;

	int ErrorCode = KMCERR_OK;

	// Liste der IDs der aktiven UniqueJumps erstellen (und Map der neuen IDs, inaktive UniqueJumps behalten -1 in Map)
	*o_newuniqueidmap = vector<int> (UJumps.size(), -1);
	bool t_IsActive = false;
	vector<int> t_ActiveIDs;
	for (int i = 0; i < (int) UJumps.size(); i++) {

		ErrorCode = UJumps[i].GetJumpActive(t_IsActive);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		if (t_IsActive == false) continue;
		o_newuniqueidmap->at(i) = (int) t_ActiveIDs.size();
		t_ActiveIDs.push_back(i);
	}
	if (t_ActiveIDs.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;

	// Output-Reset
	delete [] o_simuniquejumps;

	// Array erstellen
	o_simuniquejumps = new (nothrow) TSimUniqueJump [t_ActiveIDs.size()];
	if (o_simuniquejumps == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;

	// Arrayelemente erstellen
	for (int i = 0; i < (int) t_ActiveIDs.size(); i++) {

		ErrorCode = UJumps[t_ActiveIDs[i]].CreateSimUniqueJump(o_simuniquejumps[i]);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
	}

	return KMCERR_OK;
}

// Liste ausgeben, die jede CoordID eines UniqueJump angibt, fuer die das Atom nicht-ignoriert und additiv ist (d.h. Teil der additiven Umgebung)
int TUniqueJumpsFunc::GetAddEnvIDs (int JumpID, vector<int> *o_addenvids) {

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) return KMCERR_INVALID_INPUT_CRIT;

	return UJumps[JumpID].GetAddEnvIDs(o_addenvids);
}

// Liste ausgeben, die jede CoordID eines UniqueJump angibt, fuer die das Atom nicht-ignoriert und nicht-additiv ist (d.h. Teil der kodierten Umgebung)
int TUniqueJumpsFunc::GetCodeEnvIDs (int JumpID, vector<int> *o_codeenvids) {

	// Input prüfen
	if ((JumpID < 0) || (JumpID >= (int) UJumps.size())) return KMCERR_INVALID_INPUT_CRIT;

	return UJumps[JumpID].GetCodeEnvIDs(o_codeenvids);
}

// ***************************** PRIVATE ********************************** //