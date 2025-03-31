// **************************************************************** //
//																	//
//	Klasse: TSimProb												//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Klasse fuer eine Sprungwahrscheinlichkeitshaeufigkeit 		//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSimProb_H
#define TSimProb_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:

using namespace std;

// Klassendeklaration:
class TSimProb {

	public:

		// Variablen:
		double prob;								// Wahrscheinlichkeit
		unsigned long long count;					// Absolute Haeufigkeit

		// Member functions:
		TSimProb ();								// Default-Constructor
		~TSimProb ();								// Destructor

};

#endif