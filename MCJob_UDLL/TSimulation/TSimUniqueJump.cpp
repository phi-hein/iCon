// **************************************************************** //
//																	//
//	Klasse: TSimUniqueJump											//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Minimalklasse fuer einen UniqueJump wahrend der Simulation	//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TSimUniqueJump.h"

// Includes:

// Eigene Includes:

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimUniqueJump::TSimUniqueJump () {

	add_energies = NULL;						
	code_energies = NULL;						
	hash_mult = NULL;				
	hash_map = NULL;			
	code_coord_size = 0;
	add_coord_size = 0;

}

// Destructor
TSimUniqueJump::~TSimUniqueJump () {

	Clear();
}
		
// ************************* MEMBER FUNCTIONS****************************** //

// Alle dynamisch erzeugten Arrays loeschen, uebrige Daten auf Standardwerte setzen
void TSimUniqueJump::Clear() {

	if (add_energies != NULL) {
		if (add_coord_size > 0) {
			for (int i = add_coord_size - 1; i >= 0 ; i--) {
				delete [] add_energies[i];
				add_energies[i] = NULL;
			}
		}
		delete [] add_energies;
		add_energies = NULL;
	}

	if (hash_map != NULL) {
		if (code_coord_size > 0) {
			for (int i = code_coord_size - 1; i >= 0 ; i--) {
				delete [] hash_map[i];
				hash_map[i] = NULL;
			}
		}
		delete [] hash_map;
		hash_map = NULL;
	}
	
	delete [] code_energies;
	code_energies = NULL;

	delete [] hash_mult;
	hash_mult = NULL;

	code_coord_size = 0;
	add_coord_size = 0;

}