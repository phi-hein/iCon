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

// Deklarierte Klasse:
#include "TSimProb.h"

// Includes:

// Eigene Includes:

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimProb::TSimProb() : prob(0.0), count(0ULL)
{

}

// Destructor
TSimProb::~TSimProb()
{

}

// ************************* MEMBER FUNCTIONS****************************** //