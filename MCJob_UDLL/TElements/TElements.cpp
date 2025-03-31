// **************************************************************** //
//																	//
//	Klasse: TElements	(TElements Layer 3)							//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der verwendeten Elemente/Spezies		//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TElements/TElements.h"

// Includes:

// Eigene Klassen:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TElements::TElements (TKMCJob *pJob): TElementsOut (pJob) {

}

// Destructor
TElements::~TElements () {

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //