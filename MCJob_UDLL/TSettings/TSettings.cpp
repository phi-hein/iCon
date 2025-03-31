// **************************************************************** //
//																	//
//	Klasse: TSettings	(TSettings Layer 3)							//
//	Autor: Philipp Hein												//
//	Datum: 16.10.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Job-Spezifikationen					//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TSettings/TSettings.h"

// Includes:

// Eigene Klassen:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSettings::TSettings (TKMCJob *pJob): TSettingsOut (pJob) {

}

// Destructor
TSettings::~TSettings () {

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //