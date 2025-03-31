// **************************************************************** //
//																	//
//	Klasse: TUniqueJumpFunc	(TUniqueJump Layer 1)					//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines einzigartigen Gittersprungs 	//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TUniqueJump/TUniqueJumpFunc.h"

// Includes:
#include <algorithm>

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "TSimulation/TSimUniqueJump.h"
#include "TStructure/TStructure.h"
#include "TSettings/TSettings.h"
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TUniqueJumpFunc::TUniqueJumpFunc (TKMCJob * pJob): TUniqueJumpBase (pJob) {
	
}

// Destructor
TUniqueJumpFunc::~TUniqueJumpFunc () {
	
}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Coord ausgeben
int TUniqueJumpFunc::GetCoords (vector<T3DVector> *o_Coord) {
	if (EnvReady != true) return KMCERR_READY_NOT_TRUE;

	*o_Coord = Coord;

	return KMCERR_OK;
}

// ElemID ausgeben
int TUniqueJumpFunc::GetElemIDs (vector<int> *o_ElemID) {
	if (EnvReady != true) return KMCERR_READY_NOT_TRUE;

	*o_ElemID = ElemID;

	return KMCERR_OK;
}

// Minimalbeschreibung fuer die Simulation erstellen
int TUniqueJumpFunc::CreateSimUniqueJump (TSimUniqueJump &o_simuniquejump) {
	if (EnvReady != true) return KMCERR_READY_NOT_TRUE;
	if (JumpReady != true) return KMCERR_READY_NOT_TRUE;
	if (CodesReady != true) return KMCERR_READY_NOT_TRUE;

	if (JumpActive == false) return KMCERR_INVALID_INPUT_CRIT;
	if (Coord.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	if (m_Job == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Structure == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Structure->IfReady() == false) return KMCERR_OBJECT_NOT_READY;
	if (m_Job->m_Settings == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Settings->IfReady() == false) return KMCERR_OBJECT_NOT_READY;

	int ErrorCode = KMCERR_OK;
	vector<int> t_possible_elemid;
	int t_max_elemid = 0;

	// Temperatur ermitteln
	double t_temperature = 0.0;
	ErrorCode = m_Job->m_Settings->GetTemperature(t_temperature);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Output-Reset
	o_simuniquejump.Clear();

	// Behandlung der additiven Umgebungsbeitraege
	if (InteractionEnergies.size() != 0) {

		// Array erstellen
		o_simuniquejump.add_energies = new (nothrow) double* [InteractionEnergies.size()];
		if (o_simuniquejump.add_energies == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
		o_simuniquejump.add_coord_size = (int) InteractionEnergies.size();

		// Additive Energien setzen
		for (int i = 0; i < (int) InteractionElemIDs.size(); i++) {

			if (InteractionElemIDs[i].size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
			ErrorCode = m_Job->m_Structure->GetDopands4ElemID(InteractionElemIDs[i][0], true, &t_possible_elemid);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			t_max_elemid = *max_element(t_possible_elemid.begin(), t_possible_elemid.end());

			o_simuniquejump.add_energies[i] = new (nothrow) double [t_max_elemid + 1];
			if (o_simuniquejump.add_energies[i] == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;

			for (int j = 0; j < t_max_elemid + 1; j++) {
				o_simuniquejump.add_energies[i][j] = -1.0;
			}

			if ((int) InteractionElemIDs[i].size() > 1) {
				for (int j = 0; j < (int) InteractionElemIDs[i].size(); j++) {
					o_simuniquejump.add_energies[i][InteractionElemIDs[i][j]] = 
						pow(double(NATCONST_E), -InteractionEnergies[i][j] /( double(NATCONST_KB)*t_temperature ));
				}
			} else {
				for (int j = 0; j < (int) t_possible_elemid.size(); j++) {
					o_simuniquejump.add_energies[i][t_possible_elemid[j]] =
						pow(double(NATCONST_E), -InteractionEnergies[i][0] /( double(NATCONST_KB)*t_temperature ));
				}
			}
		}
	}

	// Behandlung der einziartigen Umgebungen
	if (FullToUniqueMap.size() != 0) {

		// Energie-Array erstellen
		o_simuniquejump.code_energies = new (nothrow) double [FullToUniqueMap.size()];
		if (o_simuniquejump.code_energies == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;

		// Code-Energien setzen
		for (int i = 0; i < (int) FullToUniqueMap.size(); i++) {

			o_simuniquejump.code_energies[i] = 
				pow(double(NATCONST_E), -UniqueEnergies[FullToUniqueMap[i]] /( double(NATCONST_KB)*t_temperature ));
		}

		// Code-Hashfunktion erstellen (fuer nicht-additive, nicht-ignorierte Atome)
		vector<size_t> t_hash_mult;
		vector<vector<int>> t_hash_map;
		for (int i = 0; i < (int) Coord.size(); i++) {
			if ((AtomIgnore[i] == true) || (AtomAdditive[i] == true)) continue;

			ErrorCode = m_Job->m_Structure->GetDopands4ElemID(ElemID[i], true, &t_possible_elemid);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			t_max_elemid = *max_element(t_possible_elemid.begin(), t_possible_elemid.end());

			if (AtomActive[i] == true) {
				t_hash_mult.push_back((size_t) t_possible_elemid.size());
			} else {
				t_hash_mult.push_back(1);
			}
			t_hash_map.push_back(vector<int> (t_max_elemid + 1, -1));

			for (int j = 0; j < (int) t_possible_elemid.size(); j++) {
				if (AtomActive[i] == true) {
					t_hash_map.back().at(t_possible_elemid[j]) = j;
				} else {
					t_hash_map.back().at(t_possible_elemid[j]) = 0;
				}
			}
		}

		// Hash-Multiplikator berechnen
		if ((int) t_hash_mult.size() > 1) {
			for (int i = 0; i < (int) t_hash_mult.size() - 1; i++) {
				t_hash_mult[i] = 1;
				for (int j = i + 1; j < (int) t_hash_mult.size(); j++) {
					t_hash_mult[i] *= t_hash_mult[j];
				}
			}
		}
		t_hash_mult.back() = 1;

		// Hash-Funktion-Arrays erstellen
		o_simuniquejump.hash_mult = new (nothrow) size_t [t_hash_mult.size()];
		if (o_simuniquejump.hash_mult == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
		for (int i = 0; i < (int) t_hash_mult.size(); i++) {
			o_simuniquejump.hash_mult[i] = t_hash_mult[i];
		}
		o_simuniquejump.hash_map = new (nothrow) int* [t_hash_map.size()];
		if (o_simuniquejump.hash_map == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
		o_simuniquejump.code_coord_size = (int) t_hash_map.size();
		for (int i = 0; i < (int) t_hash_map.size(); i++) {
			o_simuniquejump.hash_map[i] = new (nothrow) int [t_hash_map[i].size()];
			if (o_simuniquejump.hash_map[i] == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
			for (int j = 0; j < (int) t_hash_map[i].size(); j++) {
				o_simuniquejump.hash_map[i][j] = t_hash_map[i][j];
			}
		}
	}

	return KMCERR_OK;
}			

// Liste ausgeben, die jede CoordID angibt, fuer die das Atom nicht-ignoriert und additiv ist (d.h. Teil der additiven Umgebung)
int TUniqueJumpFunc::GetAddEnvIDs (vector<int> *o_addenvids) {
	if (EnvReady != true) return KMCERR_READY_NOT_TRUE;
	if (JumpReady != true) return KMCERR_READY_NOT_TRUE;
	if (CodesReady != true) return KMCERR_READY_NOT_TRUE;

	if (JumpActive == false) return KMCERR_INVALID_INPUT_CRIT;
	if (Coord.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;

	o_addenvids->clear();
	for (int i = 0; i < (int) Coord.size(); i++) {
		if ((AtomIgnore[i] == false) && (AtomAdditive[i] == true)) {
			o_addenvids->push_back(i);
		}
	}

	return KMCERR_OK;
}

// Liste ausgeben, die jede CoordID angibt, fuer die das Atom nicht-ignoriert und nicht-additiv ist (d.h. Teil der kodierten Umgebung)
int TUniqueJumpFunc::GetCodeEnvIDs (vector<int> *o_codeenvids) {
	if (EnvReady != true) return KMCERR_READY_NOT_TRUE;
	if (JumpReady != true) return KMCERR_READY_NOT_TRUE;
	if (CodesReady != true) return KMCERR_READY_NOT_TRUE;

	if (JumpActive == false) return KMCERR_INVALID_INPUT_CRIT;
	if (Coord.size() == 0) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;

	o_codeenvids->clear();
	for (int i = 0; i < (int) Coord.size(); i++) {
		if ((AtomIgnore[i] == false) && (AtomAdditive[i] == false)) {
			o_codeenvids->push_back(i);
		}
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //