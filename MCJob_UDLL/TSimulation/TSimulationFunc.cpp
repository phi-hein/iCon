// **************************************************************** //
//																	//
//	Class: TSimulationFunc	(TSimulation Layer 1)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for carrying out the simulation							//
//	  Layer 1: Functionality class = helper methods 				//
//	  -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TSimulation/TSimulationFunc.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TSimulationFunc::TSimulationFunc(TKMCJob* pJob) : TSimulationBase(pJob)
{

}

// Destructor
TSimulationFunc::~TSimulationFunc()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Ergebnisse der Hauptsimulation ausgeben
int TSimulationFunc::GetMovementResults(vector<double>* o_results) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationFunc::GetMovementResults)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (Completed != true)
	{
		cout << "Critical Error: Simulation incomplete (TSimulationFunc::GetMovementResults)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (MainPhaseData.HasValidData == false)
	{
		cout << "Critical Error: Invalid simulation data (TSimulationFunc::GetMovementResults)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (MainPhaseData.CurrentJumpAttempts == 0ULL)
	{
		cout << "Critical Error: Invalid simulation progress (TSimulationFunc::GetMovementResults)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (o_results == NULL)
	{
		cout << "Critical Error: Invalid vector pointer (TSimulationFunc::GetMovementResults)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}

	int ErrorCode = KMCERR_OK;

	// Analyse des MovLattice
	// Konvention:
	// fuer bewegliche Spezies:
	// [0]          <jcount>                = mittlere Sprunganzahl
	// [1, 2, 3]    <x>, <y>, <z>           = mittlerer Verschiebungsvektor in cm
	// [4]          <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	// [5]          <x*x + y*y + z*z>       = mittlere quadratische Verschiebung in cm^2
	// [6, 7, 8]    sum(x), sum(y), sum(z)  = Schwerpunktverschiebungsvektor in cm
	// fuer Leerstellen:
	// [9]          <jcount>                = mittlere Sprunganzahl
	// [10, 11, 12] <x>, <y>, <z>           = mittlerer Verschiebungsvektor in cm
	// [13]         <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	// [14]         <x*x + y*y + z*z>       = mittlere quadratische Verschiebung in cm^2
	// [15, 16, 17] sum(x), sum(y), sum(z)  = Schwerpunktverschiebungsvektor in cm
	vector<double> t_MovAnalysis;
	ErrorCode = MovLatticeAnalysis(&(MainPhaseData.CurrentLattice), &(MainPhaseData.CurrentMovLattice), &t_MovAnalysis);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Projektionen aufs E-Feld berechnen (jeweils Komponente des Verschiebungsvektors parallel und senkrecht zum E-Feld)
	double t_mov_comp_parallel = 0.0;
	double t_mov_comp_perpendicular = 0.0;
	double t_vac_comp_parallel = 0.0;
	double t_vac_comp_perpendicular = 0.0;
	if (spCanCalcCond == true)
	{
		ErrorCode = GetProjOnEField(T3DVector(t_MovAnalysis[1], t_MovAnalysis[2], t_MovAnalysis[3]), t_mov_comp_parallel, t_mov_comp_perpendicular);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
		ErrorCode = GetProjOnEField(T3DVector(t_MovAnalysis[10], t_MovAnalysis[11], t_MovAnalysis[12]), t_vac_comp_parallel, t_vac_comp_perpendicular);
		if (ErrorCode != KMCERR_OK) return ErrorCode;
	}

	// Simulierte Zeitspanne berechnen
	double t_simtime = MainPhaseData.CurrentAttemptPathRatioSum * MainPhaseData.UsedNorm / spFrequency;

	// Leitfaehigkeit berechnen (sigma = spCondFactor * Projection((<x>, <y>, <z>), E-Feld-Vektor) / Zeit)
	double t_conductivity = 0.0;
	if (spCanCalcCond == true)
	{
		t_conductivity = spCondFactor * spFrequency * t_mov_comp_parallel / (MainPhaseData.CurrentAttemptPathRatioSum * MainPhaseData.UsedNorm);
	}

	// Output erstellen
	// Konvention:
	// fuer bewegliche Spezies:
	// [0]          <jcount>                = mittlere Sprunganzahl
	// [1, 2, 3]    <x>, <y>, <z>           = mittlerer Verschiebungsvektor in cm
	// [4]          <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	// [5]          <x*x + y*y + z*z>       = mittlere quadratische Verschiebung in cm^2
	// [6, 7, 8]    sum(x), sum(y), sum(z)  = Schwerpunktverschiebungsvektor in cm
	// fuer Leerstellen:
	// [9]          <jcount>                = mittlere Sprunganzahl
	// [10, 11, 12] <x>, <y>, <z>           = mittlerer Verschiebungsvektor in cm
	// [13]         <sqrt(x*x + y*y + z*z)> = mittlere Verschiebung in cm
	// [14]         <x*x + y*y + z*z>       = mittlere quadratische Verschiebung in cm^2
	// [15, 16, 17] sum(x), sum(y), sum(z)  = Schwerpunktverschiebungsvektor in cm
	// fuer bewegliche Spezies:
	// [18]			Komponente des Verschiebungsvektors parallel zum E-Feld in cm
	// [19]			Komponente des Verschiebungsvektors senkrecht zum E-Feld in cm
	// fuer Leerstellen:
	// [20]			Komponente des Verschiebungsvektors parallel zum E-Feld in cm
	// [21]			Komponente des Verschiebungsvektors senkrecht zum E-Feld in cm
	// allgemeine Ergebnisse:
	// [22]			Simulierte Zeitspanne in s
	// [23]			Leitfaehigkeit in V/cm
	*o_results = t_MovAnalysis;
	o_results->push_back(t_mov_comp_parallel);
	o_results->push_back(t_mov_comp_perpendicular);
	o_results->push_back(t_vac_comp_parallel);
	o_results->push_back(t_vac_comp_perpendicular);
	o_results->push_back(t_simtime);
	o_results->push_back(t_conductivity);

	return KMCERR_OK;
}

// Status der Hauptsimulation ausgeben
int TSimulationFunc::GetMainSimStatus(TSimPhaseInfo& o_phase) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationFunc::GetMainSimStatus)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	o_phase = MainPhaseData;

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //