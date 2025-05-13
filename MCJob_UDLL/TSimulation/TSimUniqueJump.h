// **************************************************************** //
//																	//
//	Class: TSimUniqueJump											//
//	Author: Philipp Hein											//
//	Description:													//
//    Data class for a UniqueJump in the simulation					//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSimUniqueJump_H
#define TSimUniqueJump_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:

using namespace std;

// Klassendeklaration:
class TSimUniqueJump
{

public:
	// Variablen:
	double** add_energies;						// exp(-additiveEnergie/kt) fuer [additivesCoordID][ElemID]
	double* code_energies;						// exp(-CodeEnergie/kT) fuer [CodeID]
	size_t* hash_mult;							// Multiplikator der Hash-Funktion fuer [nonadditivesCoordID]
	int** hash_map;								// Nummer innerhalb der moeglichen Elem-IDs fuer bestimmte Umgebungsatomposition fuer [nonadditivesCoordID][ElemID]
	int code_coord_size;						// Anzahl der Elemente in hash_mult (fuer Destructor)
	int add_coord_size;							// Anzahl der Elemente in add_energies (fuer Destructor)

	// Member functions:
	void Clear();								// Alle dynamisch erzeugten Arrays loeschen, uebrige Daten auf Standardwerte setzen

	TSimUniqueJump();							// Default-Constructor
	~TSimUniqueJump();							// Destructor

};

#endif