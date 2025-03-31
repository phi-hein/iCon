// **************************************************************** //
//																	//
//	Klasse: TElementsOut	(TElements Layer 2)						//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der verwendeten Elemente/Spezies		//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TElements/TElementsOut.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TElementsOut::TElementsOut (TKMCJob * pJob): TElementsFunc (pJob) {
	
}

// Destructor
TElementsOut::~TElementsOut () {
	
}

// **************************** PUBLISHED ********************************* //

// Anzahl der Elemente ausgeben
int TElementsOut::GetElementCount (int &o_count) {

	// Elementanzahl ausgeben
	o_count = (int) Symbol.size();

	return KMCERR_OK;
}

// Ausgabe eines Elements
int TElementsOut::GetElement (int i_elemid, string &o_symbol, string &o_name, double &o_charge) {

	// Check, ob Element vorhanden
	if (i_elemid < 0) {
		cout << "Critical Error: Negative vector index (in TElementsOut::GetElement)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (i_elemid >= (int) Symbol.size()) {
		cout << "Critical Error: Index exceeds vector size (in TElementsOut::GetElement)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Element ausgeben
	o_symbol = Symbol[i_elemid];
	o_name = Name[i_elemid];
	o_charge = Charge[i_elemid];

	return KMCERR_OK;
}

// Ausgabe eines Elements
int TElementsOut::GetElement (int i_elemid, string &o_symbol, string &o_name, double &o_charge, double &o_radius, double &o_red, double &o_green, double &o_blue) {

	// Check, ob Element vorhanden
	if (i_elemid < 0) {
		cout << "Critical Error: Negative vector index (in TElementsOut::GetElement)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (i_elemid >= (int) Symbol.size()) {
		cout << "Critical Error: Index exceeds vector size (in TElementsOut::GetElement)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Element ausgeben
	o_symbol = Symbol[i_elemid];
	o_name = Name[i_elemid];
	o_charge = Charge[i_elemid];
	o_radius = Radius[i_elemid];
	o_red = ColorR[i_elemid];
	o_green = ColorG[i_elemid];
	o_blue = ColorB[i_elemid];

	return KMCERR_OK;
}

// Ausgabe des MovingElement
int TElementsOut::GetMovingElement (string &o_symbol, string &o_name, double &o_charge) {

	// MovingElement ausgeben
	o_symbol = MovSymbol;
	o_name = MovName;
	o_charge = MovCharge;

	return KMCERR_OK;
}

// Ausgabe des MovingElement
int TElementsOut::GetMovingElement (string &o_symbol, string &o_name, double &o_charge, double &o_radius, double &o_red, double &o_green, double &o_blue) {

	// MovingElement ausgeben
	o_symbol = MovSymbol;
	o_name = MovName;
	o_charge = MovCharge;
	o_radius = MovRadius;
	o_red = MovColorR;
	o_green = MovColorG;
	o_blue = MovColorB;

	return KMCERR_OK;
}

// Ausgabe des VacancyElement
int TElementsOut::GetVacancyElement (string &o_symbol, string &o_name, double &o_charge) {

	// MovingElement ausgeben
	o_symbol = VacSymbol;
	o_name = VacName;
	o_charge = VacCharge;

	return KMCERR_OK;
}

// Ausgabe des VacancyElement
int TElementsOut::GetVacancyElement (string &o_symbol, string &o_name, double &o_charge, double &o_radius, double &o_red, double &o_green, double &o_blue) {

	// MovingElement ausgeben
	o_symbol = VacSymbol;
	o_name = VacName;
	o_charge = VacCharge;
	o_radius = VacRadius;
	o_red = VacColorR;
	o_green = VacColorG;
	o_blue = VacColorB;

	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //