// **************************************************************** //
//																	//
//	Klasse: TStructureOut	(TStructure Layer 2)					//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der eingegebenen Struktur der			//
//	  untersuchten Verbindung										//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TStructure/TStructureOut.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"
#include "TElementsDesc.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TStructureOut::TStructureOut (TKMCJob * pJob): TStructureFunc (pJob) {
	
}

// Destructor
TStructureOut::~TStructureOut () {
	
}

// **************************** PUBLISHED ********************************* //

// Name der Struktur ausgeben
int TStructureOut::GetStructureName (string &o_name) {

	// Strukturnamen ausgeben
	o_name = Name;

	return KMCERR_OK;
}

// Basis ausgeben als 6 double (Längen, Winkel)
int TStructureOut::GetBasisByLatticeParam (double &length_a, double &length_b, double &length_c, double &angle_alpha, double &angle_beta, double &angle_gamma) {

	// Gitterparameter ausgeben
	length_a = a.Length();
	length_b = b.Length();
	length_c = c.Length();
	angle_alpha = T3DVector::Angle(b,c);
	angle_beta = T3DVector::Angle(a,c);
	angle_gamma = T3DVector::Angle(a,b);

	return KMCERR_OK;
}

// Basis ausgeben als 9 double (Vektoren)
int TStructureOut::GetBasisByLatticeVectors (double &ax, double &ay, double &az, double &bx, double &by, double &bz, double &cx, double &cy, double &cz) {

	// Gitterparameter ausgeben
	ax = a.x;
	ay = a.y;
	az = a.z;
	bx = b.x;
	by = b.y;
	bz = b.z;
	cx = c.x;
	cy = c.y;
	cz = c.z;

	return KMCERR_OK;
}

// Anzahl an Elementarzellatomen ausgeben
int TStructureOut::GetAtomCount (int &o_count) {

	// Atomanzahl ausgeben
	o_count = (int) Coord.size();

	return KMCERR_OK;
}

// Anzahl der Mov- und Vac-Atome in der Elementarzelle ausgeben
int TStructureOut::GetMovCount (int &o_count) {

	// Mov- und Vac-Atome zaehlen
	int Count = 0;
	if ((int) ElemID.size() != 0) {
		for (int i = 0; i < (int) ElemID.size(); i++) {
			if (ElemID[i] == 0) Count++;
		}
	}

	// Anzahl ausgeben
	o_count = Count;

	return KMCERR_OK;
}

// Atom ausgeben
int TStructureOut::GetAtom (int i_atom, double &xrel, double &yrel, double &zrel, int &elemid) {

	// Input pruefen
	if (i_atom < 0) {
		cout << "Critical Error: Negative vector index (in TStructureOut::GetAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((i_atom >= (int) ElemID.size()) || (i_atom >= (int) Coord.size())) {
		cout << "Critical Error: Index exceeds vector size (in TStructureOut::GetAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Daten ausgeben
	xrel = Coord[i_atom].x;
	yrel = Coord[i_atom].y;
	zrel = Coord[i_atom].z;
	elemid = ElemID[i_atom];

	return KMCERR_OK;
}

// Anzahl an Dotierungen ausgeben
int TStructureOut::GetDopingCount (int &o_count) {

	// Dotierungsanzahl ausgeben
	o_count = (int) DopandID.size();

	return KMCERR_OK;
}

// Dotierung ausgeben
int TStructureOut::GetDoping (int i_doping, int &doped_elemid, int &dopand_elemid, double &vac_dop_ratio) {

	// Input pruefen
	if (i_doping < 0) {
		cout << "Critical Error: Negative vector index (in TStructureOut::GetDoping)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((i_doping >= (int) DopedID.size()) || (i_doping >= (int) DopandID.size()) || (i_doping >= (int) VacDopRatio.size())) {
		cout << "Critical Error: Index exceeds vector size (in TStructureOut::GetDoping)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Daten ausgeben
	doped_elemid = DopedID[i_doping];
	dopand_elemid = DopandID[i_doping];
	vac_dop_ratio = VacDopRatio[i_doping];

	return KMCERR_OK;
}
		
// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //