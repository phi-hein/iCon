// **************************************************************** //
//																	//
//	Klasse: TStructure	(TStructure Layer 3)						//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der eingegebenen Struktur der			//
//	  untersuchten Verbindung										//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TStructure/TStructure.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TStructure::TStructure (TKMCJob * pJob): TStructureOut (pJob) {

}

// Destructor
TStructure::~TStructure () {

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //
