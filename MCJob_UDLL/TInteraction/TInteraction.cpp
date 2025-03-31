// **************************************************************** //
//																	//
//	Klasse: TInteraction	(TInteraction Layer 3)					//
//	Autor: Philipp Hein												//
//	Datum: 30.03.2013												//
//  Aufgabe:														//
//    Klasse zur Beschreibung einer Wechselwirkung		 			//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TInteraction/TInteraction.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TInteraction::TInteraction (TKMCJob * pJob): TInteractionOut (pJob) {

}

// Destructor
TInteraction::~TInteraction () {

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //