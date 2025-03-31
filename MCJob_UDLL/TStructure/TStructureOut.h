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

#ifndef TStructureOut_H
#define TStructureOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TStructure/TStructureFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TStructureOut: public TStructureFunc {
	// Member functions
	public:
		// PUBLISHED
		int GetStructureName (string &o_name);																													// Name der Struktur ausgeben
		int GetBasisByLatticeParam (double &length_a, double &length_b, double &length_c, double &angle_alpha, double &angle_beta, double &angle_gamma);		// Basis ausgeben als 6 double (Längen, Winkel)
		int GetBasisByLatticeVectors (double &ax, double &ay, double &az, double &bx, double &by, double &bz, double &cx, double &cy, double &cz);				// Basis ausgeben als 9 double (Vektoren)
		int GetAtomCount (int &o_count);																														// Anzahl an Elementarzellatomen ausgeben
		int GetMovCount (int &o_count);																	// Anzahl der Mov-Atome in der Elementarzelle ausgeben
		int GetAtom (int i_atom, double &xrel, double &yrel, double &zrel, int &elemid);				// Atom ausgeben
		int GetDopingCount (int &o_count);																// Anzahl an Dotierungen ausgeben
		int GetDoping (int i_doping, int &doped_elemid, int &dopand_elemid, double &vac_dop_ratio);		// Dotierung ausgeben
		
		// NON-PUBLISHED
		TStructureOut (TKMCJob * pJob);		// Constructor
		~TStructureOut ();					// Destructor
};

#endif