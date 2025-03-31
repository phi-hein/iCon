// **************************************************************** //
//																	//
//	Klasse: TSimulationOut	(TSimulation Layer 2)					//
//	Autor: Philipp Hein												//
//	Datum: 19.03.2014												//
//  Aufgabe:														//
//    Klasse zur Durchfuehrung der Simulation						//
//	  Layer 2: Output class, d.h. Ausgabe generierter Daten		 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TSimulation/TSimulationOut.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimulationOut::TSimulationOut (TKMCJob * pJob): TSimulationFunc (pJob) {
	
}

// Destructor
TSimulationOut::~TSimulationOut () {
	
}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //