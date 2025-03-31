// **************************************************************** //
//																	//
//	Klasse: TSimulation	(TSimulation Layer 3)						//
//	Autor: Philipp Hein												//
//	Datum: 19.03.2014												//
//  Aufgabe:														//
//    Klasse zur Durchfuehrung der Simulation						//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TSimulation/TSimulation.h"

// Includes:

// Eigene Klassen:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimulation::TSimulation (TKMCJob *pJob): TSimulationOut (pJob) {

}

// Destructor
TSimulation::~TSimulation () {

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //