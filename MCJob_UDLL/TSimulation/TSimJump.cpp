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

// Deklarierte Klasse:
#include "TSimJump.h"

// Includes:

// Eigene Includes:
#include "TSimulation/TSimUniqueJump.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimJump::TSimJump()
{

	back_jump = NULL;
	unique_jump = NULL;
	efield_contrib = 1.0;
	code_envpos_size = 0;
	add_envpos_size = 0;
	code_envpos = NULL;
	add_envpos = NULL;

}

// Destructor
TSimJump::~TSimJump()
{

	Clear();
}

// ************************* MEMBER FUNCTIONS****************************** //

// Alle dynamisch erzeugten Arrays loeschen, uebrige Daten auf Standardwerte setzen
void TSimJump::Clear()
{

	delete[] code_envpos;
	code_envpos = NULL;
	delete[] add_envpos;
	add_envpos = NULL;

	// Do not delete, these objects belong to other objects, only clear references
	back_jump = NULL;
	unique_jump = NULL;

	jump_vec.Set(0.0, 0.0, 0.0);
	destination.Set(0, 0, 0, 0);
	efield_contrib = 1.0;
	code_envpos_size = 0;
	add_envpos_size = 0;

}