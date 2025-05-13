// **************************************************************** //
//																	//
//	Class: TSimJump													//
//	Author: Philipp Hein											//
//	Description:													//
//    Data class for a jump environment during the simulation		//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TSimJump_H
#define TSimJump_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "T3DVector.h"
#include "T4DLatticeVector.h"

using namespace std;

// Forward-Deklarationen:
class TSimUniqueJump;

// Klassendeklaration:
class TSimJump
{

public:

	// Variablen:
	TSimJump* back_jump;						// Pointer zum Ruecksprung (NULL wenn Ruecksprung (bzw. zugehoeriger UniqueJump) deaktiviert ist)
	TSimUniqueJump* unique_jump;				// Pointer zum zugehoerigen UniqueJump
	double efield_contrib;						// E-Feld-Beitrag: exp(Skalarprodukt(E-Feld, Sprungvektor)*Ladung / kT)
	T3DVector jump_vec;							// Sprungvektor = Verschiebungsvektor wenn die Leerstelle springt
	T4DLatticeVector destination;				// relative 4D-Zielkoordinaten
	int code_envpos_size;						// Anzahl der Elemente in code_envpos
	int add_envpos_size;						// Anzahl der Elemente in add_envpos
	T4DLatticeVector* code_envpos;				// relative 4D-Umgebungskoordinaten der nicht-additiven, nicht-ignorierten Atome (Reihenfolge passend zur Hash-Funktion des zugehoerigen UniqueJumps)
	T4DLatticeVector* add_envpos;				// relative 4D-Umgebungskoordinaten der additiven, nicht-ignorierten Atome (Reihenfolge passend zu den additiven Energien des zugehoerigen UniqueJumps)

	// Member functions:
	void Clear();								// Alle dynamisch erzeugten Arrays loeschen, uebrige Daten auf Standardwerte setzen

	TSimJump();								// Default-Constructor
	~TSimJump();								// Destructor

};

#endif