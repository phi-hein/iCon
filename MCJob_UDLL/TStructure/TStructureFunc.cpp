// **************************************************************** //
//																	//
//	Klasse: TStructureFunc	(TStructure Layer 1)					//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der eingegebenen Struktur der			//
//	  untersuchten Verbindung										//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TStructure/TStructureFunc.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TStructureFunc::TStructureFunc (TKMCJob * pJob): TStructureBase (pJob) {
	
}

// Destructor
TStructureFunc::~TStructureFunc () {
	
}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Anzahl der beweglichen Atome in der Elementarzelle ausgeben (d.h. Mov,Vac)
int TStructureFunc::GetMovAtomAnz () {
	int Count = 0;
	if ((Ready == true) && ((int) ElemID.size() != 0)) {
		for (int i = 0; i < (int) ElemID.size(); i++) {
			if (ElemID[i] != 0) {
				Count = i;
				break;
			}
		}
		if (Count == 0) Count = (int) ElemID.size();
	}
	return Count;
}

// ElemID[s] zurueckgeben
int TStructureFunc::GetElemID (int s) {
	if (Ready != true) return -1;
	if ((s < 0) || (s >= (int) ElemID.size())) return -1;
	return ElemID[s];
}

// ElemIDs ausgeben
int TStructureFunc::GetElemIDs (vector<int> *o_elemids) {

	*o_elemids = ElemID;

	return KMCERR_OK;
}

// DopedIDs und DopandIDs ausgeben
int TStructureFunc::GetDopingIDs (vector<int> *o_dopedids, vector<int> *o_dopandids) {

	*o_dopedids = DopedID;
	*o_dopandids = DopandID;

	return KMCERR_OK;
}

// 3D-Vektor zu einem 4D-LatticeVector ausgeben
T3DVector TStructureFunc::Get3DVector (int x, int y, int z, int s) {
	if ((Ready != true) || (s < 0) || (s >= (int) Coord.size())) return T3DVector ();
	return a*x + b*y + c*z + a*Coord[s].x + b*Coord[s].y + c*Coord[s].z;
}

// 3D-Vektor zu einem 4D-LatticeVector ausgeben
T3DVector TStructureFunc::Get3DVector (const T4DLatticeVector &vec) {
	return Get3DVector(vec.x,vec.y,vec.z,vec.s);
}

// Nearest-Neighbor-Schalen-Analyse um einen Raumpunkt, wobei no_atoms nicht beruecksichtigt werden, elem_ids = erlaubte ElemIDs (keine = alle), out_atoms = absolute Umgebungsvektoren
int TStructureFunc::NNAnalysis (const T3DVector &i_center, int shellanz, vector<int> * elem_ids, vector<T4DLatticeVector> * no_atoms, vector<T4DLatticeVector> * out_atoms) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;


	// Input pruefen
	if (shellanz <= 0) return KMCERR_INVALID_INPUT_CRIT;
	out_atoms->clear();


	// 1. Input transformieren, sodass i_center in Elementarzelle liegt (entsprechende Verschiebung bei no_atoms und out_atoms beruecksichtigen)
	T3DVector center_3D;			// in Elementarzelle (0,0,0) gekuerzter i_center
	T4DLatticeVector center_4D;		// Elementarzelle (s = 0), in der sich i_center befindet
	center_4D.x = int (T3DVector::Projection(i_center,a)/a.Length());
	center_4D.y = int (T3DVector::Projection(i_center,b)/b.Length());
	center_4D.z = int (T3DVector::Projection(i_center,c)/c.Length());
	center_4D.s = 0;
	center_3D = i_center - a*center_4D.x - b*center_4D.y - c*center_4D.z;


	// 2. Naechstliegende Wand finden:
	double d_atomwall = 0.0;			// kuerzester Abstand des Atoms zur Wand
	double d_dir = 0.0;					// Elementarzelldicke in kuerzester Ausbreitungsrichtung
	// -> Normaleneinheitsvektoren der Elementarzellseiten berechnen:
	T3DVector n_ab = T3DVector::Cross(a,b)*(1/T3DVector::Cross(a,b).Length());
	T3DVector n_bc = T3DVector::Cross(b,c)*(1/T3DVector::Cross(b,c).Length());
	T3DVector n_ac = T3DVector::Cross(a,c)*(1/T3DVector::Cross(a,c).Length());
	// -> Elementarzelldicken berechnen (via Hessesche Normalenform):
	double d_a = abs(a*n_bc);			// Elementarzellendicke in a-Richtung
	double d_b = abs(b*n_ac);			// Elementarzellendicke in b-Richtung
	double d_c = abs(c*n_ab);			// Elementarzellendicke in c-Richtung
	double d_plusa = abs(abs(center_3D*n_bc) - d_a);		// Abstand des Atoms von der Wand in +a-Richtung
	double d_minusa = abs(center_3D*n_bc);					// Abstand des Atoms von der Wand in -a-Richtung
	double d_plusb = abs(abs(center_3D*n_ac) - d_b);		// Abstand des Atoms von der Wand in +b-Richtung
	double d_minusb = abs(center_3D*n_ac);					// Abstand des Atoms von der Wand in -b-Richtung
	double d_plusc = abs(abs(center_3D*n_ab) - d_c);		// Abstand des Atoms von der Wand in +c-Richtung
	double d_minusc = abs(center_3D*n_ab);					// Abstand des Atoms von der Wand in -c-Richtung
	// -> naechstliegende Wand finden:
	d_atomwall = d_plusa;
	d_dir = d_a;
	if (d_minusa < d_atomwall) {d_atomwall = d_minusa; d_dir = d_a;}
	if (d_plusb < d_atomwall) {d_atomwall = d_plusb; d_dir = d_b;}
	if (d_minusb < d_atomwall) {d_atomwall = d_minusb; d_dir = d_b;}
	if (d_plusc < d_atomwall) {d_atomwall = d_plusc; d_dir = d_c;}
	if (d_minusc < d_atomwall) {d_atomwall = d_minusc; d_dir = d_c;}
		

	// LOOP:
	int shellcount = 0;					// Anzahl an gefundenen Schalen
	int k_Gr = 0;						// Groesse des Kastens, der untersucht wird
	double oldshellradius = 0.0;		// Radius der alten Schale
	double newshellradius = 0.0;		// Radius der neuen Schale
	T4DLatticeVector test;				// 4D-Koordinaten des test-Atoms
	double testdist = 0.0;				// Abstand des test-Atoms vom moving_elem-Atom
	vector<T4DLatticeVector> shell;		// speichert die aktuelle Schale
	bool position_ok = true;			// Flag, ob Position als Umgebungsatom ok ist (in Bezug auf elem_ids und no_atoms)

	do {
		// 3. Kastengroesse k_Gr so waehlen, dass d_atomwall + k_Gr*d_dir > oldshellradius, dann k_Gr + 1, damit mind. 1 neues Umgebungsatom drin
		k_Gr = (int) (abs(oldshellradius - d_atomwall)/d_dir) + 1 + 1;
		newshellradius = d_atomwall + k_Gr*d_dir;										// ausserstes newshell waehlen

		// 4. In Kasten alle Atome pruefen, welches am geringsten ueber oldshellradius liegt
		//    -> alle Atome speichern (als T4DVectorLattice), die in [newshellradius-KMCVAR_EQTHRESHOLD_SHELL,newshellradius+KMCVAR_EQTHRESHOLD_SHELL] liegen
		shell.clear();
		for (int i = -k_Gr; i < k_Gr + 1; i++) {
			for (int j = -k_Gr; j < k_Gr + 1; j++) {
				for (int k = -k_Gr; k < k_Gr + 1; k++) {
					for (int s = 0; s < (int) Coord.size(); s++) {
						test.Set(i,j,k,s);
						
						// 5. Pruefen, ob erlaubtes Element (elem_ids) und erlaubte Position (no_atoms = nicht-erlaubte Positionen)
						position_ok = false;
						if ((int) elem_ids->size() != 0) {
							for (int v = 0; v < (int) elem_ids->size(); v++) {
								if (ElemID[s] == elem_ids->at(v)) {
									position_ok = true;
									break;
								}
							}
						} else position_ok = true;
						if ((int) no_atoms->size() != 0) {
							for (int v = 0; v < (int) no_atoms->size(); v++) {
								if (test + center_4D == no_atoms->at(v)) {
									position_ok = false;
									break;
								}
							}
						}
						if (position_ok == false) continue;
						
						// 6. Abstand pruefen
						testdist = T3DVector::Length(a*test.x + b*test.y + c*test.z + a*Coord[s].x + b*Coord[s].y + c*Coord[s].z - center_3D);	// Abstand berechnen
						if ((testdist <= oldshellradius) && ((testdist != 0) || (shellcount != 0))) continue;	// Atome die zu nah sind -> raus, mit Beruecksichtigung von Atom genau in center_3D
						if (testdist > newshellradius + KMCVAR_EQTHRESHOLD_SHELL) continue;						// Atome die zu fern sind -> raus
						if (testdist >= newshellradius - KMCVAR_EQTHRESHOLD_SHELL) {							// wenn Atom zu aktueller kleinster Schale gehoert
							shell.push_back(test);
							continue;
						}
						// -> alle Faelle außer in (oldshellradius,newshellradius-KMCVAR_EQTHRESHOLD_SHELL) behandelt
						shell.clear();
						shell.push_back(test);
						newshellradius = testdist;
					}
				}
			}
		}
		if ((int) shell.size() == 0) return KMCERR_NO_SHELLS_FOUND;


		// 7. oldshellradius setzen auf abstand des am weitesten entfernten Atoms in der Shell (wegen KMCVAR_EQTHRESHOLD_SHELL)
		oldshellradius = 0.0;
		for (int v = 0; v < (int) shell.size(); v++) {
			if (T3DVector::Length(a*shell[v].x + b*shell[v].y + c*shell[v].z + a*Coord[shell[v].s].x + b*Coord[shell[v].s].y + c*Coord[shell[v].s].z - center_3D) > oldshellradius) {
				oldshellradius = T3DVector::Length(a*shell[v].x + b*shell[v].y + c*shell[v].z + a*Coord[shell[v].s].x + b*Coord[shell[v].s].y + c*Coord[shell[v].s].z - center_3D);
			}
		}
		if ((oldshellradius == 0) && (shellcount != 0)) return KMCERR_NO_SHELLS_FOUND;


		// 8. Shell speichern und shellcount erhoehen 
		for (int v = 0; v < (int) shell.size(); v++) {
			out_atoms->push_back(shell[v] + center_4D);
		}
		shellcount++;
 
	// ENDLOOP until shellcount == shellanz;
	} while (shellcount < shellanz);

	return KMCERR_OK;
}

// Dotierungsmoeglichkeiten fuer eine bestimmte elem_id ermitteln
int TStructureFunc::GetDopands4ElemID (int i_ElemID, bool i_IfActive, vector<int> *o_ElemOut) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	// Input prüfen
	if (i_ElemID < 0) return KMCERR_INVALID_INPUT_CRIT;
	o_ElemOut->clear();

	// Elementliste erstellen
	o_ElemOut->push_back(i_ElemID);
	if ((i_IfActive == true) && ((int) DopedID.size() != 0)) {
		for (int i = 0; i < (int) DopedID.size(); i++) {
			if (DopedID[i] == i_ElemID) o_ElemOut->push_back(DopandID[i]);
		}
	}
	if ((i_IfActive == true) && (i_ElemID == 0)) {
		o_ElemOut->push_back(1);						// Moving Species (0) ist mit Leerstelle (1) dotiert
	}

	return KMCERR_OK;
}

// Volumen [Angstrom^3] der Elementarzelle ausgeben
int TStructureFunc::GetUCVolume (double &volume) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	// Volumen berechnen (Spatprodukt)
	volume = abs(T3DVector::Cross(a,b) * c);

	return KMCERR_OK;
}

// Beschreibung der Leerstellen-"Dotierung" im Format: Vac <- Mov ausgeben
int TStructureFunc::GetVacDopingDesc(string &o_Desc) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	int ErrorCode = KMCERR_OK;

	// Beschreibungen von Mov und Vac ermitteln
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TStructureFunc::GetVacDopingDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL) {
		cout << "Critical Error: m_Elements is null pointer (in TStructureFunc::GetVacDopingDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false) {
		cout << "Critical Error: TElements not ready (in TStructureFunc::GetVacDopingDesc)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	string t_movdesc = "";
	ErrorCode = m_Job->m_Elements->GetElemDesc(0, t_movdesc);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	string t_vacdesc = "";
	ErrorCode = m_Job->m_Elements->GetElemDesc(1, t_vacdesc);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Beschreibung setzen
	o_Desc = t_vacdesc + "<-" + t_movdesc;

	return KMCERR_OK;
}

// Beschreibung der Dotierung im Format: Dopand <- Doped ausgeben
int TStructureFunc::GetDopingDesc(int i_DopID, string &o_Desc) {
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	int ErrorCode = KMCERR_OK;

	// Eingabe pruefen
	if ((i_DopID < 0) || (i_DopID >= (int) DopandID.size())) return KMCERR_INVALID_INPUT_CRIT;

	// Beschreibungen von Dopand und dotierter Spezies ermitteln
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TStructureFunc::GetVacDopingDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL) {
		cout << "Critical Error: m_Elements is null pointer (in TStructureFunc::GetVacDopingDesc)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false) {
		cout << "Critical Error: TElements not ready (in TStructureFunc::GetVacDopingDesc)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	string t_dopanddesc = "";
	ErrorCode = m_Job->m_Elements->GetElemDesc(DopandID[i_DopID], t_dopanddesc);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	string t_dopeddesc = "";
	ErrorCode = m_Job->m_Elements->GetElemDesc(DopedID[i_DopID], t_dopeddesc);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Beschreibung setzen
	o_Desc = t_dopanddesc + "<-" + t_dopeddesc;

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //