// **************************************************************** //
//																	//
//	Klasse: TJump	(TJump Layer 3)									//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung eines Gittersprungs eines Atoms in  	//
//	  der Elementarzelle und Erstellung der Sprungumgebung			//
//	  Layer 3: Top class										 	//
//																	//
//	  -> keine Veraenderung von Member-Variablen !!					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJump/TJump.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJump::TJump (TKMCJob * pJob): TJumpOut (pJob) {

}

// Destructor
TJump::~TJump () {

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //