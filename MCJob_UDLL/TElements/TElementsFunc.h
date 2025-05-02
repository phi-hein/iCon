// **************************************************************** //
//																	//
//	Klasse: TElementsFunc	(TElements Layer 1)						//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der verwendeten Elemente/Spezies		//
//	  Layer 1: Functionality class, d.h. Hilfsfunktionen		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//	  -> keine published-Funktionen !!								//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TElementsFunc_H
#define TElementsFunc_H

// Includes:
#include <string>
#include <string_view>

// Eigene Includes:
#include "TElements/TElementsBase.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TElementsFunc : public TElementsBase
{
	// Member functions
public:
	// NON-PUBLISHED
	TElementsFunc(TKMCJob* pJob);			// Constructor

	int GetElemID(string_view i_symbol, string_view i_name, double i_charge);	// ElemID zu Symbol, Name, Charge finden
	int GetMovCharge(double& o_charge);											// Ladung der beweglichen Spezies ausgeben
	int GetElemDesc(int i_ElemID, string& o_Desc);								// Elementbeschreibung ausgeben

protected:
	~TElementsFunc();						// Destructor
};

#endif