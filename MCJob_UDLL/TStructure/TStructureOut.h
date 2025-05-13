// **************************************************************** //
//																	//
//	Class: TStructureOut	(TStructure Layer 2)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the structure of the investigated material	//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TStructureOut : public TStructureFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetStructureName(string& o_name);																													// Name der Struktur ausgeben
	int GetBasisByLatticeParam(double& length_a, double& length_b, double& length_c, double& angle_alpha, double& angle_beta, double& angle_gamma);		// Basis ausgeben als 6 double (Längen, Winkel)
	int GetBasisByLatticeVectors(double& ax, double& ay, double& az, double& bx, double& by, double& bz, double& cx, double& cy, double& cz);				// Basis ausgeben als 9 double (Vektoren)
	int GetAtomCount(int& o_count);																														// Anzahl an Elementarzellatomen ausgeben
	int GetMovCount(int& o_count);																	// Anzahl der Mov-Atome in der Elementarzelle ausgeben
	int GetAtom(int i_atom, double& xrel, double& yrel, double& zrel, int& elemid);				// Atom ausgeben
	int GetDopingCount(int& o_count);																// Anzahl an Dotierungen ausgeben
	int GetDoping(int i_doping, int& doped_elemid, int& dopand_elemid, double& vac_dop_ratio);		// Dotierung ausgeben

	// NON-PUBLISHED
	TStructureOut(TKMCJob* pJob);		// Constructor

protected:
	~TStructureOut();					// Destructor
};

#endif