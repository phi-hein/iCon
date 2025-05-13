// **************************************************************** //
//																	//
//	Class: TSimProb													//
//	Author: Philipp Hein											//
//	Description:													//
//    Data class for counting jumps with a certain probability 		//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
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
class TSimProb
{

public:

	// Variablen:
	double prob;								// Wahrscheinlichkeit
	unsigned long long count;					// Absolute Haeufigkeit

	// Member functions:
	TSimProb();								// Default-Constructor
	~TSimProb();								// Destructor

};

#endif