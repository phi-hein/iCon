// **************************************************************** //
//																	//
//	Class: TElementsOut	(TElements Layer 2)							//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing the used elements							//
//	  Layer 2: Output class = get generated data					//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TElementsOut_H
#define TElementsOut_H

// Includes:
#include <string>

// Eigene Includes:
#include "TElements/TElementsFunc.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TElementsOut : public TElementsFunc
{
	// Member functions
public:
	// PUBLISHED
	int GetElementCount(int& o_count);													// Anzahl der Elemente ausgeben
	int GetElement(int i_elemid, string& o_symbol, string& o_name, double& o_charge);	// Ausgabe eines Elements
	int GetElement(int i_elemid, string& o_symbol, string& o_name, double& o_charge,
		double& o_radius, double& o_red, double& o_green, double& o_blue);				// Ausgabe eines Elements
	int GetMovingElement(string& o_symbol, string& o_name, double& o_charge);			// Ausgabe des MovingElement
	int GetMovingElement(string& o_symbol, string& o_name, double& o_charge,
		double& o_radius, double& o_red, double& o_green, double& o_blue);				// Ausgabe des MovingElement
	int GetVacancyElement(string& o_symbol, string& o_name, double& o_charge);			// Ausgabe des VacancyElement
	int GetVacancyElement(string& o_symbol, string& o_name, double& o_charge,
		double& o_radius, double& o_red, double& o_green, double& o_blue);				// Ausgabe des VacancyElement

	// NON-PUBLISHED
	TElementsOut(TKMCJob* pJob);		// Constructor

protected:
	~TElementsOut();					// Destructor
};

#endif