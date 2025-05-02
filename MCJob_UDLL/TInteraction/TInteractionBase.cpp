// **************************************************************** //
//																	//
//	Klasse: TInteractionBase	(TInteraction Layer 0)				//
//	Autor: Philipp Hein												//
//	Datum: 30.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung einer Wechselwirkung		 			//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TInteraction/TInteractionBase.h"

// Includes:
#include <sstream>
#include <iomanip>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TInteraction/TInteraction.h"
#include "TStructure/TStructure.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TInteractionBase::TInteractionBase (TKMCJob * pJob) {
	m_Job = pJob;
	
	ElemID = -1;
	StartDist = 0;
	DestDist = 0;
	StartWWCoord.clear();
	StartWWElemID.clear();
	DestWWCoord.clear();
	DestWWElemID.clear();

	Ready = false;
}

// Destructor
TInteractionBase::~TInteractionBase () {

}

// **************************** PUBLISHED ********************************* //
		
	

// ***************************** PUBLIC *********************************** //

// Interaction erstellen aus drei absoluten 4D-Koordinaten
int TInteractionBase::MakeInteraction (T4DLatticeVector StartPos, T4DLatticeVector WWPos, T4DLatticeVector DestPos, int i_InteractionShellCount) {
	if (Ready != false) {
		cout << "Critical Error: TInteraction ready too early (in TInteractionBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Input pruefen
	if ((i_InteractionShellCount < 0) || (i_InteractionShellCount > KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONSHELLCOUNT)) {
		cout << "Error: Invalid number of interaction shells: " << i_InteractionShellCount << endl;
		cout << "       Number of interaction shells has to be an integer in the interval [0 , " << KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONSHELLCOUNT << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Pointerüberprüfung
	if (m_Job == 0) {
		cout << "Critical Error: m_Job is null pointer (in TInteractionBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == 0) {
		cout << "Critical Error: m_Structure is null pointer (in TInteractionBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false) {
		cout << "Critical Error: TStructure not ready (in TInteractionBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// 3D-Positionen ermitteln
	T3DVector wwvec = m_Job->m_Structure->Get3DVector(WWPos);
	T3DVector startvec = m_Job->m_Structure->Get3DVector(StartPos);
	T3DVector destvec = m_Job->m_Structure->Get3DVector(DestPos);

	// Basis-Charakteristika setzen
	int i_ElemID = m_Job->m_Structure->GetElemID(WWPos.s);
	double i_StartDist = T3DVector::Length(wwvec - startvec);
	double i_DestDist = T3DVector::Length(wwvec - destvec);

	if (i_StartDist <= T3DVector::zero_threshold) {
		cout << "Critical Error: Zero interaction distance 1 (in TUniqueJumpsBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (i_DestDist <= T3DVector::zero_threshold) {
		cout << "Critical Error: Zero interaction distance 2 (in TUniqueJumpsBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	
	if (i_InteractionShellCount == 0) {
		ElemID = i_ElemID;
		StartDist = i_StartDist;
		DestDist = i_DestDist;
		StartWWCoord.clear();
		StartWWElemID.clear();
		DestWWCoord.clear();
		DestWWElemID.clear();
		Ready = true;
		return KMCERR_OK;
	}

	// Schalenanalyse (StartPos->VacPos)
	T3DVector ww_center = startvec + (wwvec - startvec)*0.5;		// Analysenmitte setzen
	
	vector<T4DLatticeVector> no_atoms;		// Liste mit nicht-beruecksichtigten Atomen definieren
	no_atoms.push_back(StartPos);
	no_atoms.push_back(WWPos);

	vector<int> elem_ids;					// Liste mit erlaubten Elementen definieren
	elem_ids.clear();						// leer = alle Elemente erlaubt

	vector<T4DLatticeVector> out_atoms;		// Output-Liste mit absoluten Umgebungspositionen

	int ErrorCode = KMCERR_OK;
	ErrorCode = m_Job->m_Structure->NNAnalysis(ww_center,i_InteractionShellCount,&elem_ids,&no_atoms,&out_atoms);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: NN Analysis 1 failed (in TUniqueJumpsBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return ErrorCode;
	}
	if ((int) out_atoms.size() == 0) {
		cout << "Critical Error: No environment 1 shells found (in TUniqueJumpsBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_NO_SHELLS_FOUND;
	}
	
	vector<T3DVector> i_StartWWCoord;
	vector<int> i_StartWWElemID;
	for (int i = 0; i < (int) out_atoms.size(); i++) {				// 3D-Vektoren von Wechselwirkungsmittelpunkt zu Umgebungsatomen berechnen
		i_StartWWCoord.push_back(m_Job->m_Structure->Get3DVector(out_atoms[i]) - ww_center);
		i_StartWWElemID.push_back(m_Job->m_Structure->GetElemID(out_atoms[i].s));
	}

	// Schalenanalyse (DestPos->VacPos)
	ww_center = destvec + (wwvec - destvec)*0.5;		// Analysenmitte setzen
	
	no_atoms.clear();		// Liste mit nicht-beruecksichtigten Atomen definieren
	no_atoms.push_back(DestPos);
	no_atoms.push_back(WWPos);

	elem_ids.clear();		// Liste mit erlaubten Elementen definieren, leer = alle Elemente erlaubt

	out_atoms.clear();		// Output-Liste mit absoluten Umgebungspositionen

	ErrorCode = m_Job->m_Structure->NNAnalysis(ww_center,i_InteractionShellCount,&elem_ids,&no_atoms,&out_atoms);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: NN Analysis 2 failed (in TUniqueJumpsBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return ErrorCode;
	}
	if ((int) out_atoms.size() == 0) {
		cout << "Critical Error: No environment 2 shells found (in TUniqueJumpsBase::MakeInteraction)" << endl;
		cout << "                Input: " << StartPos << " " << WWPos << " " << DestPos << " " << i_InteractionShellCount << endl << endl;
		return KMCERR_NO_SHELLS_FOUND;
	}

	vector<T3DVector> i_DestWWCoord;
	vector<int> i_DestWWElemID;
	for (int i = 0; i < (int) out_atoms.size(); i++) {		// 3D-Vektoren von Wechselwirkungsmittelpunkt zu Umgebungsatomen berechnen
		i_DestWWCoord.push_back(m_Job->m_Structure->Get3DVector(out_atoms[i]) - ww_center);
		i_DestWWElemID.push_back(m_Job->m_Structure->GetElemID(out_atoms[i].s));
	}

	// Normalenvektor berechnen (= Einheitsvektor senkrecht zur Ebene, die durch wwvec - startvec und wwvec - destvec aufgespannt wird),
	// falls diese auf einer Geraden liegen, wird das erste Umgebungsatom mit y != 0 verwendet (s. UniqueJump)
	// dieser Normalenvektor wird fuer beide Umgebungen verwendet, sodass eine spaetere gemeinsame Darstellung beider Umgebungen moeglich ist
	T3DVector nvec = T3DVector::Cross(wwvec-startvec,wwvec-destvec);
	T3DVector jvec;
	if (nvec.Length() <= T3DVector::zero_threshold) {
		nvec = T3DVector(0,0,0);

		bool t_found = false;
		jvec = (wwvec - startvec)*0.5;
		for (int i = 0; i < (int) i_StartWWCoord.size(); i++) {
			if (T3DVector::Length(T3DVector::Normalvector(i_StartWWCoord[i],jvec)) >= T3DVector::zero_threshold) {
				nvec = T3DVector::Normalvector(i_StartWWCoord[i],jvec);
				t_found = true;
				break;
			}
		}

		if (t_found == false) {
			jvec = (wwvec - destvec)*0.5;
			for (int i = 0; i < (int) i_DestWWCoord.size(); i++) {
				if (T3DVector::Length(T3DVector::Normalvector(i_DestWWCoord[i],jvec)) >= T3DVector::zero_threshold) {
					nvec = T3DVector::Normalvector(i_DestWWCoord[i],jvec);
					break;
				}
			}
		}
	}

	// Koordinaten transformieren
	jvec = (wwvec - startvec)*0.5;	// halben WW-Vektor berechnen
	for (int i = 0; i < (int) i_StartWWCoord.size(); i++) {			// 3D-Sprungumgebungs-Vektoren berechnen
		T3DVector::Transform2Jump(i_StartWWCoord[i],jvec,nvec);
	}
	jvec = (wwvec - destvec)*0.5;	// halben WW-Vektor berechnen
	for (int i = 0; i < (int) i_DestWWCoord.size(); i++) {			// 3D-Sprungumgebungs-Vektoren berechnen
		T3DVector::Transform2Jump(i_DestWWCoord[i],jvec,nvec);
	}

	ElemID = i_ElemID;
	StartDist = i_StartDist;
	DestDist = i_DestDist;
	StartWWCoord = i_StartWWCoord;
	StartWWElemID = i_StartWWElemID;
	DestWWCoord = i_DestWWCoord;
	DestWWElemID = i_DestWWElemID;
	Ready = true;
	return KMCERR_OK;
}

// Interaction mit anderer Interaction vergleichen
int TInteractionBase::Compare (TInteraction &toInteraction) {
	if (Ready != true) {
		cout << "Critical Error: TInteraction 1 not ready (in TInteractionBase::Compare)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (toInteraction.IfReady() != true) {
		cout << "Critical Error: TInteraction 2 not ready (in TInteractionBase::Compare)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// toInteraction-Members laden
	int ErrorCode = KMCERR_OK;
	
	int ElemID2;
	double StartDist2;					
	double DestDist2;					
	vector<T3DVector> StartWWCoord2;	
	vector<int> StartWWElemID2;			
	vector<T3DVector> DestWWCoord2;		
	vector<int> DestWWElemID2;
	ErrorCode = toInteraction.GetWWDesc(ElemID2,StartDist2,DestDist2);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: Cannot retrieve interaction parameters (in TInteractionBase::Compare)" << endl << endl;
		return ErrorCode;
	}
	ErrorCode = toInteraction.GetWWShells(&StartWWCoord2,&StartWWElemID2,&DestWWCoord2,&DestWWElemID2);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: Cannot retrieve interactions shells (in TInteractionBase::Compare)" << endl << endl;
		return ErrorCode;
	}

	// Basis-Charakteristika vergleichen
	if ((ElemID != ElemID2) ||
		(abs(StartDist - StartDist2) > T3DVector::zero_threshold) ||
		(abs(DestDist - DestDist2) > T3DVector::zero_threshold)) {
			return KMCERR_UNEQUAL;
	}
	if ((StartWWCoord.size() != StartWWCoord2.size()) ||
		(StartWWElemID.size() != StartWWElemID2.size()) ||
		(DestWWCoord.size() != DestWWCoord2.size()) ||
		(DestWWElemID.size() != DestWWElemID2.size())) {
			return KMCERR_UNEQUAL;
	}

	// Mapping Variablen initialisieren
	int firstyzeroj1 = -1;
	int firstyzeroj2 = -1;
	int yzerocounterj1 = 0;
	int yzerocounterj2 = 0;
	int elemsumj1 = 0;
	int elemsumj2 = 0;
	vector<int> mapping;
	double dangle = 0.0;

	// --- Schalenvergleich (StartPos->WWPos) ---
	if (StartWWCoord.size() != 0) {
		// Anzahl an y != 0 in Interaction (= j1) und toInteraction (= j2) finden und ersten Vektor in j1 und j2 finden, der y != 0 hat (gleichzeitig ElemIDs aufsummieren)
		firstyzeroj1 = -1;
		firstyzeroj2 = -1;
		yzerocounterj1 = 0;
		yzerocounterj2 = 0;
		elemsumj1 = 0;
		elemsumj2 = 0;
		for (int i = 0; i < (int) StartWWCoord.size(); i++) {
			if ((StartWWCoord[i].y < -T3DVector::eq_threshold) || (StartWWCoord[i].y > T3DVector::eq_threshold)) {
				if (firstyzeroj1 == -1) firstyzeroj1 = i;
				yzerocounterj1++;
			}
			if ((StartWWCoord2[i].y < -T3DVector::eq_threshold) || (StartWWCoord2[i].y > T3DVector::eq_threshold)) {
				if (firstyzeroj2 == -1) firstyzeroj2 = i;
				yzerocounterj2++;
			}
			elemsumj1 += StartWWElemID[i];
			elemsumj2 += StartWWElemID2[i];
		}

		// Falls j1 und j2 unterschiedliche Anzahlen an y != 0 haben, dann kein mapping moeglich
		if (yzerocounterj1 != yzerocounterj2) return KMCERR_UNEQUAL;

		// Falls j1 und j2 unterschiedliche ElemID-Summen haben, dann kein mapping moeglich
		if (elemsumj1 != elemsumj2) return KMCERR_UNEQUAL;

		mapping.clear();

		// Falls in j1 und j2 kein Vektor y != 0 hat, dann besonderes mapping, denn alle Atome auf Sprungachse
		if (yzerocounterj1 == 0) {	// d.h. alle Atome auf Sprungachse
			for (int j = 0; j < (int) StartWWCoord.size(); j++) {
				for (int k = 0; k < (int) StartWWCoord2.size(); k++) {
					if (StartWWElemID[j] != StartWWElemID2[k]) continue;
					if ((StartWWCoord[j].x >= StartWWCoord2[k].x + T3DVector::eq_threshold) || (StartWWCoord[j].x <= StartWWCoord2[k].x - T3DVector::eq_threshold)) continue;
					mapping.push_back(k);
					break;
				}
			}
			if (mapping.size() != StartWWCoord.size()) return KMCERR_UNEQUAL;	// kein passendes Mapping gefunden
		} else {
			// Normales Mapping
			// einen j1-Vektor nacheinander auf alle j2-Vektoren drehen (uebrige j1-Vektoren mitdrehen) und pruefen ob jeder j1-Vektor mit genau einem j2-Vektor uebereinstimmt
			dangle = 0.0;
			for (int i = firstyzeroj2; i < (int) StartWWCoord2.size(); i++) {
				// wenn mapping von j1[firstyzeroj1] auf j2[i] nicht moeglich, dann direkt naechstes i pruefen
				if (StartWWElemID[firstyzeroj1] != StartWWElemID2[i]) continue;
				if ((StartWWCoord[firstyzeroj1].x >= StartWWCoord2[i].x + T3DVector::eq_threshold) || (StartWWCoord[firstyzeroj1].x <= StartWWCoord2[i].x - T3DVector::eq_threshold)) continue;
				if ((StartWWCoord[firstyzeroj1].y >= StartWWCoord2[i].y + T3DVector::eq_threshold) || (StartWWCoord[firstyzeroj1].y <= StartWWCoord2[i].y - T3DVector::eq_threshold)) continue;
				// wenn einer der beiden Vektoren y == 0 hat, dann nicht als Ausgangspunkt fuer mapping nehmen (weil z nicht definiert)
				if ((StartWWCoord2[i].y >= -T3DVector::eq_threshold) && (StartWWCoord2[i].y <= T3DVector::eq_threshold)) continue;
				mapping.clear();
				dangle = StartWWCoord2[i].z - StartWWCoord[firstyzeroj1].z;					// j1.z + dangle = j2.z
				// Paare finden (gleichzeitig mapping)
				for (int j = 0; j < (int) StartWWCoord.size(); j++) {
					for (int k = 0; k < (int) StartWWCoord2.size(); k++) {
						if (StartWWElemID[j] != StartWWElemID2[k]) continue;
						if ((StartWWCoord[j].x >= StartWWCoord2[k].x + T3DVector::eq_threshold) || (StartWWCoord[j].x <= StartWWCoord2[k].x - T3DVector::eq_threshold)) continue;
						if ((StartWWCoord[j].y >= StartWWCoord2[k].y + T3DVector::eq_threshold) || (StartWWCoord[j].y <= StartWWCoord2[k].y - T3DVector::eq_threshold)) continue;
						if ((StartWWCoord2[k].y < -T3DVector::eq_threshold) || (StartWWCoord2[k].y > T3DVector::eq_threshold)) {
							if (CirclePeriodicCompare(StartWWCoord[j].z + dangle, StartWWCoord2[k].z) == false) continue;			// periodische Abfrage wenn y != 0
						}
						mapping.push_back(k);
						break;
					}
				}
				if (mapping.size() == StartWWCoord.size()) break;		// d.h. mapping gefunden
				// Paare finden (gleichzeitig mapping) mit gespiegelten Winkeln (an 0,180-Achse) des j1-Jumps -> axiale Chiralitaet der Umgebung umgehen
				// zuerst dangle in gespiegelter Umgebung berechnen:
				dangle = - StartWWCoord[firstyzeroj1].z;
				while (dangle < 0) dangle += 360.0;
				dangle = StartWWCoord2[i].z - dangle;
				mapping.clear();
				for (int j = 0; j < (int) StartWWCoord.size(); j++) {
					for (int k = 0; k < (int) StartWWCoord2.size(); k++) {
						if (StartWWElemID[j] != StartWWElemID2[k]) continue;
						if ((StartWWCoord[j].x >= StartWWCoord2[k].x + T3DVector::eq_threshold) || (StartWWCoord[j].x <= StartWWCoord2[k].x - T3DVector::eq_threshold)) continue;
						if ((StartWWCoord[j].y >= StartWWCoord2[k].y + T3DVector::eq_threshold) || (StartWWCoord[j].y <= StartWWCoord2[k].y - T3DVector::eq_threshold)) continue;
						if ((StartWWCoord2[k].y < -T3DVector::eq_threshold) || (StartWWCoord2[k].y > T3DVector::eq_threshold)) {
							if (CirclePeriodicCompare(dangle - StartWWCoord[j].z, StartWWCoord2[k].z) == false) continue;		// periodische Abfrage wenn y != 0
						}
						mapping.push_back(k);
						break;
					}
				}
				if (mapping.size() == StartWWCoord.size()) break;
			}
			if (mapping.size() != StartWWCoord.size()) return KMCERR_UNEQUAL;	// kein passendes Mapping gefunden
		}
	}

	// --- Schalenvergleich (DestPos->WWPos) ---
	if (DestWWCoord.size() != 0) {
		// Anzahl an y != 0 in Jump (= j1) und toJump (= j2) finden und ersten Vektor in j1 und j2 finden, der y != 0 hat
		firstyzeroj1 = -1;
		firstyzeroj2 = -1;
		yzerocounterj1 = 0;
		yzerocounterj2 = 0;
		elemsumj1 = 0;
		elemsumj2 = 0;
		for (int i = 0; i < (int) DestWWCoord.size(); i++) {
			if ((DestWWCoord[i].y < -T3DVector::eq_threshold) || (DestWWCoord[i].y > T3DVector::eq_threshold)) {
				if (firstyzeroj1 == -1) firstyzeroj1 = i;
				yzerocounterj1++;
			}
			if ((DestWWCoord2[i].y < -T3DVector::eq_threshold) || (DestWWCoord2[i].y > T3DVector::eq_threshold)) {
				if (firstyzeroj2 == -1) firstyzeroj2 = i;
				yzerocounterj2++;
			}
			elemsumj1 += DestWWElemID[i];
			elemsumj2 += DestWWElemID2[i];
		}

		// Falls j1 und j2 unterschiedliche Anzahlen an y != 0 haben, dann kein mapping moeglich
		if (yzerocounterj1 != yzerocounterj2) return KMCERR_UNEQUAL;

		// Falls j1 und j2 unterschiedliche ElemID-Summen haben, dann kein mapping moeglich
		if (elemsumj1 != elemsumj2) return KMCERR_UNEQUAL;

		mapping.clear();

		// Falls in j1 und j2 kein Vektor y != 0 hat, dann besonderes mapping, denn alle Atome auf Sprungachse
		if (yzerocounterj1 == 0) {	// d.h. alle Atome auf Sprungachse
			for (int j = 0; j < (int) DestWWCoord.size(); j++) {
				for (int k = 0; k < (int) DestWWCoord2.size(); k++) {
					if (DestWWElemID[j] != DestWWElemID2[k]) continue;
					if ((DestWWCoord[j].x >= DestWWCoord2[k].x + T3DVector::eq_threshold) || (DestWWCoord[j].x <= DestWWCoord2[k].x - T3DVector::eq_threshold)) continue;
					mapping.push_back(k);
					break;
				}
			}
			if (mapping.size() != DestWWCoord.size()) return KMCERR_UNEQUAL;	// kein passendes Mapping gefunden
		} else {
			// Normales Mapping
			// einen j1-Vektor nacheinander auf alle j2-Vektoren drehen (uebrige j1-Vektoren mitdrehen) und pruefen ob jeder j1-Vektor mit genau einem j2-Vektor uebereinstimmt
			dangle = 0.0;
			for (int i = firstyzeroj2; i < (int) DestWWCoord2.size(); i++) {
				// wenn mapping von j1[firstyzeroj1] auf j2[i] nicht moeglich, dann direkt naechstes i pruefen
				if (DestWWElemID[firstyzeroj1] != DestWWElemID2[i]) continue;
				if ((DestWWCoord[firstyzeroj1].x >= DestWWCoord2[i].x + T3DVector::eq_threshold) || (DestWWCoord[firstyzeroj1].x <= DestWWCoord2[i].x - T3DVector::eq_threshold)) continue;
				if ((DestWWCoord[firstyzeroj1].y >= DestWWCoord2[i].y + T3DVector::eq_threshold) || (DestWWCoord[firstyzeroj1].y <= DestWWCoord2[i].y - T3DVector::eq_threshold)) continue;
				// wenn einer der beiden Vektoren y == 0 hat, dann nicht als Ausgangspunkt fuer mapping nehmen (weil z nicht definiert)
				if ((DestWWCoord2[i].y >= -T3DVector::eq_threshold) && (DestWWCoord2[i].y <= T3DVector::eq_threshold)) continue;
				mapping.clear();
				dangle = DestWWCoord2[i].z - DestWWCoord[firstyzeroj1].z;					// j1.z + dangle = j2.z
				// Paare finden (gleichzeitig mapping)
				for (int j = 0; j < (int) DestWWCoord.size(); j++) {
					for (int k = 0; k < (int) DestWWCoord2.size(); k++) {
						if (DestWWElemID[j] != DestWWElemID2[k]) continue;
						if ((DestWWCoord[j].x >= DestWWCoord2[k].x + T3DVector::eq_threshold) || (DestWWCoord[j].x <= DestWWCoord2[k].x - T3DVector::eq_threshold)) continue;
						if ((DestWWCoord[j].y >= DestWWCoord2[k].y + T3DVector::eq_threshold) || (DestWWCoord[j].y <= DestWWCoord2[k].y - T3DVector::eq_threshold)) continue;
						if ((DestWWCoord2[k].y < -T3DVector::eq_threshold) || (DestWWCoord2[k].y > T3DVector::eq_threshold)) {
						if (CirclePeriodicCompare(DestWWCoord[j].z + dangle, DestWWCoord2[k].z) == false) continue;			// periodische Abfrage wenn y != 0
						}
						mapping.push_back(k);
						break;
					}
				}
				if (mapping.size() == DestWWCoord.size()) break;		// d.h. mapping gefunden
				// Paare finden (gleichzeitig mapping) mit gespiegelten Winkeln (an 0,180-Achse) des j1-Jumps -> axiale Chiralitaet der Umgebung umgehen
				// zuerst dangle in gespiegelter Umgebung berechnen:
				dangle = - DestWWCoord[firstyzeroj1].z;
				while (dangle < 0) dangle += 360.0;
				dangle = DestWWCoord2[i].z - dangle;
				mapping.clear();
				for (int j = 0; j < (int) DestWWCoord.size(); j++) {
					for (int k = 0; k < (int) DestWWCoord2.size(); k++) {
						if (DestWWElemID[j] != DestWWElemID2[k]) continue;
						if ((DestWWCoord[j].x >= DestWWCoord2[k].x + T3DVector::eq_threshold) || (DestWWCoord[j].x <= DestWWCoord2[k].x - T3DVector::eq_threshold)) continue;
						if ((DestWWCoord[j].y >= DestWWCoord2[k].y + T3DVector::eq_threshold) || (DestWWCoord[j].y <= DestWWCoord2[k].y - T3DVector::eq_threshold)) continue;
						if ((DestWWCoord2[k].y < -T3DVector::eq_threshold) || (DestWWCoord2[k].y > T3DVector::eq_threshold)) {
							if (CirclePeriodicCompare(dangle - DestWWCoord[j].z, DestWWCoord2[k].z) == false) continue;		// periodische Abfrage wenn y != 0
						}
						mapping.push_back(k);
						break;
					}
				}
				if (mapping.size() == DestWWCoord.size()) break;
			}
			if (mapping.size() != DestWWCoord.size()) return KMCERR_UNEQUAL;	// kein passendes Mapping gefunden
		}
	}

	return KMCERR_EQUAL;
}

// Rueckgabe von Ready
bool TInteractionBase::IfReady () {
	return Ready;
}

// Elemente in einen Stream schreiben, nur bei Ready == true moeglich
int TInteractionBase::SaveToStream (ostream &output, int InteractionID, int offset) {
	if (Ready != true) {
		cout << "Critical Error: TInteraction not ready (in TInteractionBase::SaveToStream)" << endl;
		cout << "                Input: " << InteractionID << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true) {
		cout << "Critical Error: Output stream not ready (in TInteractionBase::SaveToStream)" << endl;
		cout << "                Input: " << InteractionID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) {
		cout << "Critical Error: Offset out of bounds (in TInteractionBase::SaveToStream)" << endl;
		cout << "                Input: " << InteractionID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (InteractionID < 0) {
		cout << "Critical Error: Negative interaction ID (in TInteractionBase::SaveToStream)" << endl;
		cout << "                Input: " << InteractionID << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset,' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int) KMCOUT_TINTERACTION_OFFSET > 0) sub_offset = string((int) KMCOUT_TINTERACTION_OFFSET,' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile
	output << s_offset << KMCOUT_TINTERACTION_START1 << " " << KMCOUT_TINTERACTION_ID << " " << InteractionID;
	output << " " << KMCOUT_TINTERACTION_START2 << endl;

	// Ausgabe von ElemID, StartDist und DestDist
	output << s_offset << sub_offset << KMCOUT_TINTERACTION_WWELEMID << " " << ElemID << endl;
	output << s_offset << sub_offset << KMCOUT_TINTERACTION_STARTDIST << " " << StartDist << endl;
	output << s_offset << sub_offset << KMCOUT_TINTERACTION_DESTDIST << " " << DestDist << endl;

	// Ausgabe der Start-WW-Umgebung, d.h. StartWWCoord und StartWWElemID (falls size != 0)
	int ID_space;
	int ElemID_space;
	int x_space;
	int y_space;
	int z_space;
	if (StartWWCoord.size() != 0) {
		ID_space = IntToStr((int) StartWWCoord.size()).length();
		if (ID_space < (int) ((string) KMCOUT_TINTERACTION_WWID).length()) ID_space = ((string) KMCOUT_TINTERACTION_WWID).length();
		ElemID_space = ((string) KMCOUT_TINTERACTION_ELEMID).length();
		for (int i = 0; i < (int) StartWWElemID.size(); i++) {
			if (ElemID_space < (int) IntToStr(StartWWElemID[i]).length()) ElemID_space = IntToStr(StartWWElemID[i]).length();
		}
		x_space = (int) output.precision() + 7;
		y_space = (int) output.precision() + 7;
		z_space = (int) output.precision() + 7;
		if (x_space < (int) ((string) KMCOUT_TINTERACTION_X).length()) x_space = ((string) KMCOUT_TINTERACTION_X).length();
		if (y_space < (int) ((string) KMCOUT_TINTERACTION_Y).length()) y_space = ((string) KMCOUT_TINTERACTION_Y).length();
		if (z_space < (int) ((string) KMCOUT_TINTERACTION_Z).length()) z_space = ((string) KMCOUT_TINTERACTION_Z).length();
		output << endl;
		output << s_offset << sub_offset << setw(KMCOUT_TINTERACTION_STARTWW.front().length() + 1) << " ";
		output << setw(ID_space) << KMCOUT_TINTERACTION_WWID << " ";
		output << setw(ElemID_space) << KMCOUT_TINTERACTION_ELEMID << " ( ";
		output << setw(x_space) << KMCOUT_TINTERACTION_X << " ";
		output << setw(y_space) << KMCOUT_TINTERACTION_Y << " ";
		output << setw(z_space) << KMCOUT_TINTERACTION_Z << " )" << endl;
		for (int i = 0; i < (int) StartWWCoord.size(); i++) {
			output << s_offset << sub_offset << KMCOUT_TINTERACTION_STARTWW << " " << setw(ID_space) << i << " ";
			output << setw(ElemID_space) << StartWWElemID[i] << " ( ";
			output << setw(x_space) << StartWWCoord[i].x << " " << setw(y_space) << StartWWCoord[i].y << " ";
			output << setw(z_space) << StartWWCoord[i].z << " )" << endl;
		}
	}

	// Ausgabe der Dest-WW-Umgebung, d.h. DestWWCoord und DestWWElemID (falls size != 0)
	if (DestWWCoord.size() != 0) {
		ID_space = IntToStr((int) DestWWCoord.size()).length();
		if (ID_space < (int) ((string) KMCOUT_TINTERACTION_WWID).length()) ID_space = ((string) KMCOUT_TINTERACTION_WWID).length();
		ElemID_space = ((string) KMCOUT_TINTERACTION_ELEMID).length();
		for (int i = 0; i < (int) DestWWElemID.size(); i++) {
			if (ElemID_space < (int) IntToStr(DestWWElemID[i]).length()) ElemID_space = IntToStr(DestWWElemID[i]).length();
		}
		x_space = (int) output.precision() + 7;
		y_space = (int) output.precision() + 7;
		z_space = (int) output.precision() + 7;
		if (x_space < (int) ((string) KMCOUT_TINTERACTION_X).length()) x_space = ((string) KMCOUT_TINTERACTION_X).length();
		if (y_space < (int) ((string) KMCOUT_TINTERACTION_Y).length()) y_space = ((string) KMCOUT_TINTERACTION_Y).length();
		if (z_space < (int) ((string) KMCOUT_TINTERACTION_Z).length()) z_space = ((string) KMCOUT_TINTERACTION_Z).length();
		output << endl;
		output << s_offset << sub_offset << setw(KMCOUT_TINTERACTION_DESTWW.front().length() + 1) << " ";
		output << setw(ID_space) << KMCOUT_TINTERACTION_WWID << " ";
		output << setw(ElemID_space) << KMCOUT_TINTERACTION_ELEMID << " ( ";
		output << setw(x_space) << KMCOUT_TINTERACTION_X << " ";
		output << setw(y_space) << KMCOUT_TINTERACTION_Y << " ";
		output << setw(z_space) << KMCOUT_TINTERACTION_Z << " )" << endl;
		for (int i = 0; i < (int) DestWWCoord.size(); i++) {
			output << s_offset << sub_offset << KMCOUT_TINTERACTION_DESTWW << " " << setw(ID_space) << i << " ";
			output << setw(ElemID_space) << DestWWElemID[i] << " ( ";
			output << setw(x_space) << DestWWCoord[i].x << " " << setw(y_space) << DestWWCoord[i].y << " ";
			output << setw(z_space) << DestWWCoord[i].z << " )" << endl;
		}
	}
	output << s_offset << KMCOUT_TINTERACTION_END << endl;

	if (output.fail() == true) {
		cout << "Critical Error: Output stream failure (in TInteractionBase::SaveToStream)" << endl;
		cout << "                Input: " << InteractionID << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Elemente aus einem Stream laden, nur bei Ready == false moeglich
int TInteractionBase::LoadFromStream (istream &input) {
	if (Ready != true) {
		cout << "Critical Error: TInteraction not ready (in TInteractionBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (input.good() == false) {
		cout << "Critical Error: Input stream not ready (in TInteractionBase::LoadFromStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	int i_ElemID = -1;
	double i_StartDist = 0;
	double i_DestDist = 0;
	vector<T3DVector> i_StartWWCoord;
	vector<int> i_StartWWElemID;
	vector<T3DVector> i_DestWWCoord;
	vector<int> i_DestWWElemID;
	
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

		if (s_temp == KMCOUT_TINTERACTION_STARTDIST) {
			if ((linestream >> i_StartDist).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TINTERACTION_WWELEMID) {
			if ((linestream >> i_ElemID).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TINTERACTION_DESTDIST) {
			if ((linestream >> i_DestDist).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TINTERACTION_STARTWW) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp != (int) i_StartWWCoord.size()) {
				if_failed = true;
				break;
			}
			i_StartWWElemID.push_back(-1);
			i_StartWWCoord.push_back(T3DVector ());
			if ((linestream >> i_StartWWElemID.back()).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true) {	// "("
				if_failed = true;
				break;
			}
			if ((linestream >> i_StartWWCoord.back().x).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_StartWWCoord.back().y).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_StartWWCoord.back().z).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TINTERACTION_DESTWW) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp != (int) i_DestWWCoord.size()) {
				if_failed = true;
				break;
			}
			i_DestWWElemID.push_back(-1);
			i_DestWWCoord.push_back(T3DVector ());
			if ((linestream >> i_DestWWElemID.back()).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true) {	// "("
				if_failed = true;
				break;
			}
			if ((linestream >> i_DestWWCoord.back().x).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_DestWWCoord.back().y).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_DestWWCoord.back().z).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TINTERACTION_END) break;
	}
	if (if_failed == true) {
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}
	
	// Wechselwirkungsinformationen abgleichen
	if (abs(StartDist - i_StartDist) > T3DVector::eq_threshold) {
		if (abs(StartDist - i_StartDist) > KMCVAR_EQTHRESHOLD_PERC_LOAD*StartDist) {
			cout << "Error: Inconsistent file content" << endl << endl;
			return KMCERR_INVALID_FILE_CONTENT;
		}
	}
	if (abs(DestDist - i_DestDist) > T3DVector::eq_threshold) {
		if (abs(DestDist - i_DestDist) > KMCVAR_EQTHRESHOLD_PERC_LOAD*DestDist) {
			cout << "Error: Inconsistent file content" << endl << endl;
			return KMCERR_INVALID_FILE_CONTENT;
		}
	}
	if ((ElemID != i_ElemID) ||
		(StartWWCoord.size() != i_StartWWCoord.size()) ||
		(StartWWElemID.size() != i_StartWWElemID.size()) ||
		(DestWWCoord.size() != i_DestWWCoord.size()) ||
		(DestWWElemID.size() != i_DestWWElemID.size())) {
			cout << "Error: Inconsistent file content" << endl << endl;
			return KMCERR_INVALID_FILE_CONTENT;
	}
	if (StartWWCoord.size() != 0) {
		for (int i = 0; i < (int) StartWWCoord.size(); i++) {
			if ((T3DVector::LoadCompare(StartWWCoord[i], i_StartWWCoord[i], true) == false) ||
				(StartWWElemID[i] != i_StartWWElemID[i])) {
					cout << "Error: Inconsistent file content" << endl << endl;
					return KMCERR_INVALID_FILE_CONTENT;
			}
		}
	}
	if (DestWWCoord.size() != 0) {
		for (int i = 0; i < (int) DestWWCoord.size(); i++) {
			if ((T3DVector::LoadCompare(DestWWCoord[i], i_DestWWCoord[i], true) == false) ||
				(DestWWElemID[i] != i_DestWWElemID[i])) {
					cout << "Error: Inconsistent file content" << endl << endl;
					return KMCERR_INVALID_FILE_CONTENT;
			}
		}
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //