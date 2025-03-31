// **************************************************************** //
//																	//
//	Klasse: TJumpFunc	(TJump Layer 1)								//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines Gittersprungs eines Atoms in  	//
//	  der Elementarzelle und Erstellung der Sprungumgebung			//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJump/TJumpFunc.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "TStructure/TStructure.h"
#include "TSettings/TSettings.h"
#include "TSimulation/TSimJump.h"
#include "TElements/TElements.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJumpFunc::TJumpFunc (TKMCJob * pJob): TJumpBase (pJob) {
	
}

// Destructor
TJumpFunc::~TJumpFunc () {
	
}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// absolute 4D-Position des Startatoms zurueckgeben (Vektor von (0,0,0,0) zu Startatom)
int TJumpFunc::GetStartPos (T4DLatticeVector &pos) {
	if (StartPos.s == -1) return KMCERR_INVALID_INPUT_CRIT;
	pos = StartPos;

	return KMCERR_OK;
}

// absolute 4D-Position des Zielatoms zurueckgeben (Vektor von (0,0,0,0) zu Zielatom)
int TJumpFunc::GetDestPos (T4DLatticeVector &pos) {
	if (StartPos.s == -1) return KMCERR_INVALID_INPUT_CRIT;
	if ((DestPos.x == 0) && (DestPos.y == 0) && (DestPos.z == 0) && (DestPos.s == 0)) return KMCERR_INVALID_INPUT_CRIT;
	pos = StartPos + DestPos;

	return KMCERR_OK;
}

// absolute 4D-Positionen der Umgebungsatome zurueckgeben (Vektoren von (0,0,0,0) zu Umgebungsatomen)
int TJumpFunc::GetEnvPos (vector<T4DLatticeVector> *envpos) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	if (envpos == NULL) return KMCERR_INVALID_INPUT_CRIT;

	envpos->clear();
	for (int i = 0; i < (int) EnvPos.size(); i++) {
		envpos->push_back(StartPos + EnvPos[i]);
	}

	return KMCERR_OK;
}

// UniqueJumpID ausgeben
int TJumpFunc::GetUniqueJumpID (int &ID) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	if (UniqueJumpID < 0) return KMCERR_READY_NOT_TRUE;

	ID = UniqueJumpID;

	return KMCERR_OK;
}

// Skalarprodukt [V] von E-Feld-Vektor [V/cm] und halbem Sprungvektor [cm] ausgeben
int TJumpFunc::GetEFieldProjection (T3DVector &efield, double &proj) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	if (efield.Length() < T3DVector::zero_threshold) {
		proj = 0;
		return KMCERR_OK;
	}

	if (m_Job == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Structure == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Structure->IfReady() != true) return KMCERR_OBJECT_NOT_READY;
	
	// Anmerkung: Sprungvektor-Umrechnung: Angstrom [1.0E-10 m] -> cm [1.0E-2 m]
	// Anmerkung: Faktor 0.5 noetig, da Aktivierungsenergiesenkung durch das E-Feld nur bis zum Uebergangszustand (ca. Sprungmitte) erfolgt
	proj = 0.5 * (efield * (m_Job->m_Structure->Get3DVector(StartPos + DestPos) - m_Job->m_Structure->Get3DVector(StartPos))) * 1.0E-8;
	return KMCERR_OK;
}

// Minimalbeschreibung fuer die Simulation erstellen
int TJumpFunc::CreateSimJump(TSimJump &o_simjump) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	if (m_Job == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Elements == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Elements->IfReady() != true) return KMCERR_OBJECT_NOT_READY;
	if (m_Job->m_Structure == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Structure->IfReady() != true) return KMCERR_OBJECT_NOT_READY;
	if (m_Job->m_UniqueJumps == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_UniqueJumps->IfCodesReady() != true) return KMCERR_OBJECT_NOT_READY;
	if (m_Job->m_Settings == NULL) return KMCERR_INVALID_POINTER;
	if (m_Job->m_Settings->IfReady() != true) return KMCERR_OBJECT_NOT_READY;

	int ErrorCode = KMCERR_OK;

	// Objekt-Reset
	o_simjump.Clear();

	// Sprungziel und Sprungvektor setzen
	o_simjump.destination = DestPos;
	o_simjump.jump_vec = m_Job->m_Structure->Get3DVector(StartPos + DestPos) - m_Job->m_Structure->Get3DVector(StartPos);

	// E-Feld-Beitrag berechnen
	T3DVector t_efield;
	double t_charge = 0.0;
	double t_proj = 0.0;
	double t_temperature = 0.0;

	ErrorCode = m_Job->m_Elements->GetMovCharge(t_charge);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	ErrorCode = m_Job->m_Settings->GetEField(t_efield.x, t_efield.y, t_efield.z);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	ErrorCode = m_Job->m_Settings->GetTemperature(t_temperature);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	ErrorCode = GetEFieldProjection(t_efield, t_proj);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	o_simjump.efield_contrib = pow(double(NATCONST_E), -t_proj * t_charge /( double(NATCONST_KB)*t_temperature ));
	
	// Additive Sprungumgebung setzen
	vector<int> t_addenvids;
	ErrorCode = m_Job->m_UniqueJumps->GetAddEnvIDs(UniqueJumpID, &t_addenvids);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (t_addenvids.size() != 0) {
		o_simjump.add_envpos = new (nothrow) T4DLatticeVector [t_addenvids.size()];
		if (o_simjump.add_envpos == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
		o_simjump.add_envpos_size = (int) t_addenvids.size();

		for (int i = 0; i < (int) t_addenvids.size(); i++) {
			o_simjump.add_envpos[i] = EnvPos[t_addenvids[i]];
		}
	}

	// Kodierte Sprungumgebung setzen
	vector<int> t_codeenvids;
	ErrorCode = m_Job->m_UniqueJumps->GetCodeEnvIDs(UniqueJumpID, &t_codeenvids);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (t_codeenvids.size() != 0) {
		o_simjump.code_envpos = new (nothrow) T4DLatticeVector [t_codeenvids.size()];
		if (o_simjump.code_envpos == NULL) return KMCERR_MAXIMUM_INPUT_REACHED;
		o_simjump.code_envpos_size = (int) t_codeenvids.size();

		for (int i = 0; i < (int) t_codeenvids.size(); i++) {
			o_simjump.code_envpos[i] = EnvPos[t_codeenvids[i]];
		}
	}
	
	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //