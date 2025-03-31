// **************************************************************** //
//																	//
//	Klasse: T3DMovVector											//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Klasse fuer Verschiebungsvektor incl. Sprungzaehler			//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "T3DMovVector.h"

// Includes:

// Eigene Includes:
#include "GlobalDefinitions.h"

using namespace std;


// ******************* STATIC MEMBER FUNCTIONS *************************

// Betrag eines Vektors a
double T3DMovVector::Length (const T3DMovVector &a) {
	return a.Length();
}