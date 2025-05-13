// **************************************************************** //
//																	//
//	Class: TKMCJob	(TKMCJob Layer 3)								//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing a KMC job									//
//	  Layer 3: Top class											//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TKMCJob/TKMCJob.h"

// Includes:

// Eigene Includes:
#include "TElements/TElements.h"
#include "TStructure/TStructure.h"
#include "TJumps/TJumps.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "TSettings/TSettings.h"
#include "TSimulation/TSimulation.h"
#include "TResults/TResults.h"
#include "TElementsDesc.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor -> Initialisierung, falls erfolgreich, Ready = true setzen
TKMCJob::TKMCJob() : TKMCJobOut()
{
	try
	{
		// Laden der Elementdarstellungsbeschreibungen
		m_ElementsDesc = new TElementsDesc();
		if (m_ElementsDesc == NULL) return;
		if (m_ElementsDesc->IfReady() == false) return;

		// Initialisierung der Elemente
		m_Elements = new TElements(this);
		if (m_Elements == NULL) return;

		// Initialisierung der Struktur
		m_Structure = new TStructure(this);
		if (m_Structure == NULL) return;

		// Initialisierung der Jumps
		m_Jumps = new TJumps(this);
		if (m_Jumps == NULL) return;

		// Initialisierung der UniqueJumps
		m_UniqueJumps = new TUniqueJumps(this);
		if (m_UniqueJumps == NULL) return;

		// Initialisierung der Einstellungen
		m_Settings = new TSettings(this);
		if (m_Settings == NULL) return;

		// Initialisierung der Simulationsklasse
		m_Simulation = new TSimulation(this);
		if (m_Simulation == NULL) return;

		// Initialisierung der Ergebnisklasse
		m_Results = new TResults(this);
		if (m_Results == NULL) return;

		Ready = true;
	}
	catch (exception& e)
	{
		Ready = false;
		StdExceptionHandler(e);
		throw;
	}
	catch (...)
	{
		Ready = false;
		ExceptionHandler();
		throw;
	}
}

// Destructor
TKMCJob::~TKMCJob()
{

}

// **************************** PUBLISHED ********************************* //

// Wichtig: Destructor-Methode gegen Memory Leaks (Instance suicide)
void TKMCJob::Release()
{
	try
	{
		delete this;
	}
	catch (exception& e)
	{
		StdExceptionHandler(e);
	}
	catch (...)
	{
		ExceptionHandler();
	}
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //