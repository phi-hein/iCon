// **************************************************************** //
//																	//
//	Klasse: TSimulationBase	(TSimulation Layer 0)					//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Klasse zur Durchfuehrung der Simulation						//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TSimulationBase.h"

// Includes:
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>
#include <fstream>
#include <time.h>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"
#include "TStructure/TStructure.h"
#include "TJumps/TJumps.h"
#include "TUniqueJumps/TUniqueJumps.h"
#include "TSettings/TSettings.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TSimulationBase::TSimulationBase(TKMCJob* pJob)
{
	m_Job = pJob;

	Ready = false;
	Completed = false;
	CheckPointPath = "";

	spTotalMovAnz = 0ULL;
	spTotalVacAnz = 0ULL;
	spLatticeSize = 0;
	spStackSize = 0;
	spMovStackSize = 0;
	spDirSelCount = 0;
	spDirCount = NULL;
	spDirLink = NULL;
	spUniqueJumps = NULL;
	spJumps = NULL;
	spHighestProb = 1.0;
	spIfBackjumpRelevant = false;
	spFrequency = 1.0;
	spCanCalcCond = false;
	spCondFactor = 0.0;

	spMCSP = 0ULL;
	spTargetMCSP = 0ULL;
	spJumpAttempts = 0ULL;
	spTargetJumpAttempts = 0ULL;
	spNonsenseAttempts = 0ULL;
	spOverkillAttempts = 0ULL;
	spSiteBlockingCounter = 0ULL;
	spNormalization = 1.0;
	spAttemptPathRatioSum = 0.0;
	spLattice = NULL;
	spMovLattice = NULL;
	spVacListSizes = NULL;
	spVacLists = NULL;
	spVacMaxIndex = 0;
	spProbListSize = 0;
	spAttemptProbList = NULL;
	spAcceptedProbList = NULL;
}

// Destructor
TSimulationBase::~TSimulationBase()
{
	m_Job = 0;

	Clear();
}

// **************************** PUBLISHED ********************************* //

// Simulationsdaten initialisieren, Ready = true setzen
int TSimulationBase::InitializeSimulation()
{

	int ErrorCode = KMCERR_OK;

	// Daten zuruecksetzen (incl. Ready = false)
	Clear();

	cout << endl;
	cout << "Initializing simulation data ..." << endl;

	// Alle uebrigen Klassen auf Ready = true pruefen
	ErrorCode = IfAllClassesReady();
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	cout << "  Input data is valid." << endl;

	// Konstante Simulationsparameter ermitteln
	ErrorCode = m_Job->m_Settings->GetLatticeSize(spLatticeSize);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	long long t_TotalVacAnz = 0LL;
	ErrorCode = m_Job->m_Settings->GetTotalVacancyAnz(t_TotalVacAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	spTotalVacAnz = static_cast<unsigned long long>(t_TotalVacAnz);

	ErrorCode = m_Job->m_Structure->GetAtomCount(spStackSize);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	ErrorCode = m_Job->m_Structure->GetMovCount(spMovStackSize);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	spTotalMovAnz = static_cast<unsigned long long>(spLatticeSize) * static_cast<unsigned long long>(spLatticeSize) *
		static_cast<unsigned long long>(spLatticeSize) * static_cast<unsigned long long>(spMovStackSize) - spTotalVacAnz;
	cout << "  Lattice settings:" << endl;
	cout << "    Size: " << spLatticeSize << endl;
	cout << "    Number of atom positions per unit cell: " << spStackSize << endl;
	cout << "    Number of positions of the moving species per unit cell: " << spMovStackSize << endl;
	cout << "    Total number of atoms of the moving species in the lattice: " << spTotalMovAnz << endl;
	cout << "    Total number of vacancies in the lattice: " << spTotalVacAnz << endl;

	spProbDist = uniform_real_distribution<double>(0.0, 1.0);
	cout << "  Creation of random number distribution complete." << endl;

	ErrorCode = m_Job->m_Jumps->CreateJumpDirList(spDirSelCount, spDirCount, spDirLink);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot create jump direction lists (in TSimulationBase::InitializeSimulation)" << endl << endl;
		return ErrorCode;
	}
	cout << "  Creation of jump direction lists complete." << endl;

	int seed_seq_rdcount = 5;
	random_device rd;
	vector<seed_seq::result_type> rdseeds;
	cout << "  Random number seed sequence:" << endl;
	for (int i = 0; i < seed_seq_rdcount; i++)
	{
		rdseeds.push_back(static_cast<seed_seq::result_type>(rd()));
		cout << "    " << i + 1 << ". Seed: " << rdseeds.back() << endl;
	}
	seed_seq sseq(rdseeds.begin(), rdseeds.end());
	spRNG.seed(sseq);
	cout << "  Random number generator is ready." << endl;
	cout << "    1. Random Number: " << spRNG() << endl;

	ErrorCode = m_Job->m_Jumps->CreateSimJumps(0, spJumps, spUniqueJumps);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot create jump objects (in TSimulationBase::InitializeSimulation)" << endl << endl;
		return ErrorCode;
	}
	cout << "  Creation of jump and unique jump objects complete." << endl;

	// -> Objekt ist jetzt prinzipiell bereit fuer die Simulation (spHighestProb steht auf 1.0, spCondFactor auf 0.0/false)
	Ready = true;

	// Normierung berechnen
	ErrorCode = GetHighestProb(spHighestProb);
	if (ErrorCode != KMCERR_OK)
	{
		spHighestProb = 1.0;
		return ErrorCode;
	}
	cout << "  Standard normalization complete." << endl;
	cout << "    Highest jump probability: " << spHighestProb << " (without normalization)" << endl;

	// Pruefung auf NonsenseJumpAttempts setzen
	if (spHighestProb >= 1.0)
	{
		spIfBackjumpRelevant = true;
		cout << "  Backjump evaluation enabled." << endl;
	}
	else
	{
		spIfBackjumpRelevant = false;
		cout << "  Backjump evaluation disabled." << endl;
	}

	// Auswertungsparameter berechnen
	ErrorCode = GetCondFactor(spCanCalcCond, spFrequency, spCondFactor);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (spCanCalcCond == true)
	{
		cout << "  Conductivity factor calculated: " << spCondFactor * spFrequency << " S/cm^2" << endl;
	}
	else
	{
		cout << "  No electrical field present -> no conductivity calculation." << endl;
		cout << "  Mean squared movement distance will be calculated instead." << endl;
	}

	cout << "Initialization finished successfully." << endl;

	return KMCERR_OK;
}

// CheckPoint-Dateipfad setzen
int TSimulationBase::SetCheckPointPath(string i_CheckPointPath)
{

	if (StringCheck(i_CheckPointPath, 0, KMCVAL_TSIMULATION_MAX_CHKPATHLENGTH) == false)
	{
		cout << "Error: Invalid checkpoint path: " << Trim(i_CheckPointPath) << endl;
		cout << "       Path must have less than " << KMCVAL_TSIMULATION_MAX_CHKPATHLENGTH << " characters." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	CheckPointPath = Trim(i_CheckPointPath);

	return KMCERR_OK;
}

// Daten aus CheckPoint-Datei laden
int TSimulationBase::LoadCheckPoint()
{

	Completed = false;
	return LoadCheckPoint(CheckPointPath);
}

// Simulation durchfuehren
int TSimulationBase::MakeSimulation()
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready for simulation (TSimulationBase::MakeSimulation)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	Completed = false;
	int ErrorCode = KMCERR_OK;

	try
	{

		// Startvorbereitungen
		ErrorCode = SPStartPreparations();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// CheckPoint-Save
		SaveCheckPoint("", CheckPointPath);

		// Vorlauf-Vorbereitungen
		ErrorCode = SPPrerunPreparations();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// CheckPoint-Save
		SaveCheckPoint("", CheckPointPath);

	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}

	// Vorlauf
	if ((spTargetMCSP > 0ULL) && (spMCSP < spTargetMCSP))
	{

		// Simulation
		ErrorCode = SPSimulate(PrerunPhaseData);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		try
		{

			// Nachbereitung
			ErrorCode = SPPrerunPostProcessing();
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			// CheckPoint-Save
			SaveCheckPoint("", CheckPointPath);

		}
		catch (exception& e)
		{
			return StdExceptionHandler(e);
		}
		catch (...)
		{
			return ExceptionHandler();
		}
	}

	try
	{

		// DynNorm-Vorbereitungen
		ErrorCode = SPDynNormPreparations();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// CheckPoint-Save
		SaveCheckPoint("", CheckPointPath);

	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}

	// DynNorm
	if ((spTargetJumpAttempts > 0ULL) && (spJumpAttempts < spTargetJumpAttempts))
	{

		// Simulation
		ErrorCode = SPSimulate(DynNormPhaseData);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		try
		{

			// Nachbereitung
			ErrorCode = SPDynNormPostProcessing();
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			// CheckPoint-Save
			SaveCheckPoint("", CheckPointPath);

		}
		catch (exception& e)
		{
			return StdExceptionHandler(e);
		}
		catch (...)
		{
			return ExceptionHandler();
		}
	}

	try
	{

		// DynNorm-Vorbereitungen
		ErrorCode = SPMainPreparations();
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// CheckPoint-Save
		SaveCheckPoint("", CheckPointPath);

	}
	catch (exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}

	// Hauptsimulation
	if ((spTargetMCSP > 0ULL) && (spMCSP < spTargetMCSP))
	{

		// Simulation
		ErrorCode = SPSimulate(MainPhaseData);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		try
		{

			// Nachbereitung
			ErrorCode = SPMainPostProcessing();
			if (ErrorCode != KMCERR_OK) return ErrorCode;

			// CheckPoint-Save
			SaveCheckPoint("", CheckPointPath);

		}
		catch (exception& e)
		{
			return StdExceptionHandler(e);
		}
		catch (...)
		{
			return ExceptionHandler();
		}
	}

	Completed = true;
	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //

// Rueckgabe von Ready
bool TSimulationBase::IfReady()
{
	return Ready;
}

// Rueckgabe von Completed
bool TSimulationBase::IfCompleted()
{
	return Completed;
}

// ********************* PRIVATE - SIMULATION PHASES ********************** //

// Startvorbereitungen
int TSimulationBase::SPStartPreparations()
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::SPStartPreparations)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	cout << endl;
	cout << "Start preparations ..." << endl;

	// StartLattice erstellen (falls noetig)
	if (StartLattice.size() == 0)
	{

		ErrorCode = CreateRandomLattice(&StartLattice);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// -> daraus folgt, dass von hier an neu simuliert wird
		PrerunPhaseData.Clear();
		DynNormPhaseData.Clear();
		MainPhaseData.Clear();

		cout << "  Initial lattice setup complete." << endl;
	}
	else
	{
		cout << "  Loaded initial lattice is used." << endl;
	}

	// Gitteranalyse ausgeben
	cout << "  Initial lattice analysis:" << endl;
	ErrorCode = LatticeAnalysis("    ", &StartLattice);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	cout << "Start preparations finished successfully." << endl;

	return KMCERR_OK;
}

// Vorbereitungen fuer die Vorlauf-Simulation
int TSimulationBase::SPPrerunPreparations()
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::SPPrerunPreparations)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	cout << endl;
	cout << "Prerun preparations ..." << endl;

	// Simulationsdaten zuruecksetzen
	ClearCurrentSim();

	// Prerun-Parameter ermitteln
	bool DoPrerun = false;
	long long PrerunTargetMCSP = 0LL;
	int PrerunRecordAnz = 0;
	ErrorCode = m_Job->m_Settings->GetPrerunOptions(DoPrerun, PrerunTargetMCSP, PrerunRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Feststellen, ob Prerun aktiviert ist
	if ((DoPrerun == false) || (PrerunTargetMCSP <= 0LL))
	{

		PrerunPhaseData.Clear();	// -> HasValidData == false -> Signal an nachfolgende Routinen, dass kein Prerun durchgefuehrt wurde
		spTargetMCSP = 0ULL;		// Signal an Hauptroutine, dass Prerun nicht durchzufuehren ist

		cout << "  Prerun is disabled." << endl;
		cout << "Prerun skipped." << endl;

		return KMCERR_OK;
	}
	cout << "  Prerun is enabled." << endl;

	// Feststellen, ob Prerun schon komplett durchgefuehrt wurde
	if ((PrerunPhaseData.HasValidData == true) && (PrerunPhaseData.CurrentMCSP >= (unsigned long long) PrerunTargetMCSP))
	{

		spMCSP = PrerunPhaseData.CurrentMCSP;	// Signal an Hauptroutine, dass Prerun schon fertig ist
		spTargetMCSP = (unsigned long long)PrerunTargetMCSP;

		cout << "  Prerun is already complete (" << PrerunPhaseData.CurrentMCSP << " of " << PrerunTargetMCSP << " MCSP)." << endl;
		cout << "Prerun skipped." << endl;

		return KMCERR_OK;
	}

	// PhaseData erstellen (wenn nicht vorher bereits geladen)
	if (PrerunPhaseData.HasValidData == false)
	{

		PrerunPhaseData.Clear();

		PrerunPhaseData.CurrentLattice = StartLattice;

		ErrorCode = CreateMovLattice(&(PrerunPhaseData.CurrentMovLattice));
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		if (PrerunRecordAnz > 0)
		{
			PrerunPhaseData.CurrentAttemptProbList = vector<TSimProb>(PrerunRecordAnz, TSimProb());
			PrerunPhaseData.CurrentAcceptedProbList = vector<TSimProb>(PrerunRecordAnz, TSimProb());
		}

		if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
		{
			PrerunPhaseData.UsedNorm = 1.0 / spHighestProb;
		}
		else
		{
			PrerunPhaseData.UsedNorm = 1.0;
		}

		PrerunPhaseData.HasValidData = true;

		cout << "  New prerun data was created." << endl;
	}
	else
	{
		cout << "  Loaded prerun data is used." << endl;
	}

	// Prerun noch nicht vollstaendig -> daraus folgt, dass von hier an neu simuliert wird
	DynNormPhaseData.Clear();
	MainPhaseData.Clear();

	// Simulationsstand ausgeben
	cout << "  Current status:" << endl;
	cout << "    Target-MCSP: " << PrerunTargetMCSP << endl;
	ErrorCode = PhaseAnalysis("    ", false, PrerunPhaseData, true);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Simulationsdaten setzen
	ErrorCode = SetSimulationData(PrerunPhaseData);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Zielparameter setzen
	spTargetMCSP = (unsigned long long) PrerunTargetMCSP;
	spTargetJumpAttempts = 0ULL;

	// Normierung setzen 
	if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
	{
		spNormalization = 1.0 / spHighestProb;
	}
	else
	{
		spNormalization = 1.0;
	}
	cout << "  Normalization: " << spNormalization << endl;

	cout << "  Data transfer complete." << endl;

	cout << "Prerun preparations finished successfully." << endl;

	cout << endl;
	cout << "Start: ---------- SIMULATION PHASE: PRERUN ---------- " << endl;

	return KMCERR_OK;
}

// Nachbereitungen der Vorlauf-Simulation
int TSimulationBase::SPPrerunPostProcessing()
{

	cout << "End: ------------ SIMULATION PHASE: PRERUN ---------- " << endl;
	cout << endl;

	// Analyse der Prerun-Ergebnisse
	cout << "RESULTS: Prerun" << endl;
	int ErrorCode = PhaseAnalysis("  ", false, PrerunPhaseData, true);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	return KMCERR_OK;
}

// Vorbereitungen fuer die DynNorm-Simulation
int TSimulationBase::SPDynNormPreparations()
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::SPDynNormPreparations)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	cout << endl;
	cout << "Dynamic normalization preparations ..." << endl;

	// Simulationsdaten zuruecksetzen
	ClearCurrentSim();

	// DynNorm-Parameter ermitteln
	bool DoDynNorm = false;
	long long DynNormTargetAttempts = 0LL;
	int DynNormRecordAnz = 0;
	int DynNormListEntry = 0;
	ErrorCode = m_Job->m_Settings->GetDynNormParameters(DoDynNorm, DynNormTargetAttempts, DynNormRecordAnz, DynNormListEntry);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Feststellen, ob DynNorm aktiviert ist
	if ((DoDynNorm == false) || (DynNormTargetAttempts <= 0LL) || (DynNormRecordAnz <= 0))
	{

		DynNormPhaseData.Clear();		// -> HasValidData == false -> Signal an nachfolgende Routinen, dass kein DynNorm durchgefuehrt wurde
		spTargetJumpAttempts = 0ULL;	// Signal an Hauptroutine, dass DynNorm nicht durchzufuehren ist

		cout << "  Dynamic normalization is disabled." << endl;
		cout << "Dynamic normalization skipped." << endl;

		return KMCERR_OK;
	}
	cout << "  Dynamic normalization is enabled." << endl;

	// Feststellen, ob DynNorm schon komplett durchgefuehrt wurde
	if ((DynNormPhaseData.HasValidData == true) && (DynNormPhaseData.CurrentJumpAttempts >= (unsigned long long) DynNormTargetAttempts))
	{

		spJumpAttempts = DynNormPhaseData.CurrentJumpAttempts;				// Signal an Hauptroutine, dass DynNorm schon fertig ist
		spTargetJumpAttempts = (unsigned long long)DynNormTargetAttempts;

		cout << "  Dynamic normalization is already complete (" << DynNormPhaseData.CurrentJumpAttempts << " of " << DynNormTargetAttempts << " Jump Attempts)." << endl;
		cout << "Dynamic normalization skipped." << endl;

		return KMCERR_OK;
	}

	// PhaseData erstellen (wenn nicht vorher bereits geladen)
	if (DynNormPhaseData.HasValidData == false)
	{

		DynNormPhaseData.Clear();

		// Gitter vom Prerun uebertragen, falls vorhanden
		if (PrerunPhaseData.HasValidData == true)
		{
			DynNormPhaseData.CurrentLattice = PrerunPhaseData.CurrentLattice;
		}
		else
		{
			DynNormPhaseData.CurrentLattice = StartLattice;
		}

		ErrorCode = CreateMovLattice(&(DynNormPhaseData.CurrentMovLattice));
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		DynNormPhaseData.CurrentAttemptProbList = vector<TSimProb>(DynNormRecordAnz, TSimProb());
		DynNormPhaseData.CurrentAcceptedProbList = vector<TSimProb>(DynNormRecordAnz, TSimProb());

		if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
		{
			DynNormPhaseData.UsedNorm = 1.0 / spHighestProb;
		}
		else
		{
			DynNormPhaseData.UsedNorm = 1.0;
		}

		DynNormPhaseData.HasValidData = true;

		cout << "  New dyn. norm. data was created." << endl;
	}
	else
	{
		cout << "  Loaded dyn. norm. data is used." << endl;
	}

	// DynNorm noch nicht vollstaendig -> daraus folgt, dass von hier an neu simuliert wird
	MainPhaseData.Clear();

	// Simulationsstand ausgeben
	cout << "  Current status:" << endl;
	cout << "    Target-Jump Attempts: " << DynNormTargetAttempts << endl;
	ErrorCode = PhaseAnalysis("    ", false, DynNormPhaseData, true);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Simulationsdaten setzen
	ErrorCode = SetSimulationData(DynNormPhaseData);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Zielparameter setzen
	spTargetMCSP = 0ULL;
	spTargetJumpAttempts = (unsigned long long) DynNormTargetAttempts;

	// Normierung setzen 
	if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
	{
		spNormalization = 1.0 / spHighestProb;
	}
	else
	{
		spNormalization = 1.0;
	}
	cout << "  Normalization: " << spNormalization << endl;

	cout << "  Data transfer complete." << endl;

	cout << "Dyn norm. preparations finished successfully." << endl;

	cout << endl;
	cout << "Start: ---------- SIMULATION PHASE: DYNAMIC NORMALIZATION ---------- " << endl;

	return KMCERR_OK;
}

// Nachbereitungen der DynNorm-Simulation
int TSimulationBase::SPDynNormPostProcessing()
{

	cout << "End: ------------ SIMULATION PHASE: DYNAMIC NORMALIZATION ---------- " << endl;
	cout << endl;

	// Analyse der DynNorm-Ergebnisse
	cout << "RESULTS: Dynamic normalization" << endl;
	int ErrorCode = PhaseAnalysis("  ", false, DynNormPhaseData, true);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	return KMCERR_OK;
}

// Vorbereitungen fuer die Hauptsimulation 
int TSimulationBase::SPMainPreparations()
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::SPMainPreparations)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	cout << endl;
	cout << "Main simulation preparations ..." << endl;

	// Simulationsdaten zuruecksetzen
	ClearCurrentSim();

	// Main-Parameter ermitteln
	long long MainTargetMCSP = 0LL;
	int MainRecordAnz = 0;
	ErrorCode = m_Job->m_Settings->GetMainKMCOptions(MainTargetMCSP, MainRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Feststellen, ob Target-MCSP gueltig sind
	if (MainTargetMCSP <= 0LL)
	{
		cout << "Critical Error: Number of MCSP too low (TSimulationBase::SPMainPreparations)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Feststellen, ob Simulation schon komplett durchgefuehrt wurde
	if ((MainPhaseData.HasValidData == true) && (MainPhaseData.CurrentMCSP >= (unsigned long long) MainTargetMCSP))
	{

		spMCSP = MainPhaseData.CurrentMCSP;						// Signal an Hauptroutine, dass Simulation schon fertig ist
		spTargetMCSP = (unsigned long long) MainTargetMCSP;

		cout << "  Main simulation is already complete (" << MainPhaseData.CurrentMCSP << " of " << MainTargetMCSP << " MCSP)." << endl;
		cout << "Main simulation skipped." << endl;

		return KMCERR_OK;
	}

	// PhaseData erstellen (wenn nicht vorher bereits geladen)
	if (MainPhaseData.HasValidData == false)
	{
		MainPhaseData.Clear();

		// Gitter aus DynNorm oder Prerun uebertragen, falls moeglich
		if (DynNormPhaseData.HasValidData == true)
		{
			MainPhaseData.CurrentLattice = DynNormPhaseData.CurrentLattice;
		}
		else
		{
			if (PrerunPhaseData.HasValidData == true)
			{
				MainPhaseData.CurrentLattice = PrerunPhaseData.CurrentLattice;
			}
			else
			{
				MainPhaseData.CurrentLattice = StartLattice;
			}
		}

		ErrorCode = CreateMovLattice(&(MainPhaseData.CurrentMovLattice));
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		if (MainRecordAnz > 0)
		{
			MainPhaseData.CurrentAttemptProbList = vector<TSimProb>(MainRecordAnz, TSimProb());
			MainPhaseData.CurrentAcceptedProbList = vector<TSimProb>(MainRecordAnz, TSimProb());
		}

		if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
		{
			MainPhaseData.UsedNorm = 1.0 / spHighestProb;
		}
		else
		{
			MainPhaseData.UsedNorm = 1.0;
		}

		MainPhaseData.HasValidData = true;

		cout << "  New main simulation data was created." << endl;
	}
	else
	{
		cout << "  Loaded main simulation data is used." << endl;
	}

	// Simulationsstand ausgeben
	cout << "  Current status:" << endl;
	cout << "    Target-MCSP: " << MainTargetMCSP << endl;
	ErrorCode = PhaseAnalysis("    ", false, MainPhaseData, true);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Simulationsdaten setzen
	ErrorCode = SetSimulationData(MainPhaseData);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Zielparameter setzen
	spTargetMCSP = (unsigned long long) MainTargetMCSP;
	spTargetJumpAttempts = 0ULL;

	// Normierung setzen (aus dyn. Normierung falls moeglich)
	if ((DynNormPhaseData.HasValidData == false) || (DynNormPhaseData.CurrentAttemptProbList.size() == 0))
	{
		if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
		{
			spNormalization = 1.0 / spHighestProb;
		}
		else
		{
			spNormalization = 1.0;
		}
		cout << "  No dynamic normalization detected." << endl;
	}
	else
	{

		// Nummer des Listeneintrags ermitteln, der fuer die dyn. Normierung verwendet werden soll
		bool DoDynNorm = false;
		long long DynNormTargetAttempts = 0ULL;
		int DynNormRecordAnz = 0;
		int DynNormEntryNum = 0;
		ErrorCode = m_Job->m_Settings->GetDynNormParameters(DoDynNorm, DynNormTargetAttempts, DynNormRecordAnz, DynNormEntryNum);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		// Listeneintrag pruefen
		if (DynNormEntryNum <= 0)
		{

			// -> Listeneintrag muss gesetzt und dann die Simulation neu gestartet werden
			cout << "  Dynamic normalization list entry number is zero." << endl;
			cout << "  -> The main simulation will be skipped. Afterwards, please select an entry from the" << endl;
			cout << "     jump attempt probability list from the dynamic normalization results above and" << endl;
			cout << "     enter the entry number (>= 1) in the job file (DynNorm-Wahrscheinlichkeitseintrag)." << endl;
			cout << "     Then, simply restart the simulation to continue at this point with the chosen" << endl;
			cout << "     dynamic normalization faktor." << endl;

			return KMCERR_HAS_TO_SET_DYNNORM_ENTRY;

		}
		else
		{

			// -> Listeneintrag wurde gesetzt und wird herausgesucht (falls kein geeigneter Listeneintrag vorhanden ist, wird normal normiert)
			int t_maxprobindex = DynNormEntryNum - 1;
			if (t_maxprobindex >= (int)DynNormPhaseData.CurrentAttemptProbList.size())
			{
				t_maxprobindex = int(DynNormPhaseData.CurrentAttemptProbList.size()) - 1;
			}
			int t_normid = -1;
			for (int i = t_maxprobindex; i >= 0; i--)
			{
				if ((DynNormPhaseData.CurrentAttemptProbList[i].count != 0ULL) &&
					(DynNormPhaseData.CurrentAttemptProbList[i].prob > 0.0) &&
					(DynNormPhaseData.CurrentAttemptProbList[i].prob <= 1.0))
				{
					t_normid = i;
					break;
				}
			}
			if (t_normid != -1)
			{
				if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
				{
					spNormalization = (1.0 / spHighestProb) / DynNormPhaseData.CurrentAttemptProbList[t_normid].prob;
				}
				else
				{
					spNormalization = 1.0 / DynNormPhaseData.CurrentAttemptProbList[t_normid].prob;
				}
				cout << "  New normalization was calculated from the " << t_normid + 1 << ". entry (";
				cout << DynNormPhaseData.CurrentAttemptProbList[t_normid].prob << ") in" << endl;
				cout << "  the dyn. norm. jump attempt probability list." << endl;
			}
			else
			{
				if ((spHighestProb > 0.0) && (spHighestProb < 1.0))
				{
					spNormalization = 1.0 / spHighestProb;
				}
				else
				{
					spNormalization = 1.0;
				}
				cout << "  No valid dyn. norm. list entry (entry number <= " << DynNormEntryNum << ", prob. in (0.0, 1.0], count > 0) found." << endl;
			}
		}

	}
	cout << "  Normalization: " << spNormalization << endl;

	cout << "  Data transfer complete." << endl;

	cout << "Main simulation preparations finished successfully." << endl;

	cout << endl;
	cout << "Start: ---------- SIMULATION PHASE: MAIN ---------- " << endl;

	return KMCERR_OK;
}

// Nachbereitungen der Hauptsimulation
int TSimulationBase::SPMainPostProcessing()
{

	cout << "End: ------------ SIMULATION PHASE: MAIN ---------- " << endl;
	cout << endl;

	// Analyse der Hauptsimulationsergebnisse
	cout << "FINAL RESULTS: Main" << endl;
	int ErrorCode = PhaseAnalysis("  ", false, MainPhaseData, true);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	return KMCERR_OK;
}

// Simulationsroutine: KMC-Simulation durchfuehren (Achtung: alle Parameter muessen korrekt gesetzt sein -> keine weitere Pruefung)
int TSimulationBase::SPSimulate(TSimPhaseInfo& ispPhase)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::SPSimulate)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Zusaetzliche Variablen initialisieren (isp = internal simulation parameters)
	int ispErrorCode = KMCERR_OK;					// Fehlercode
	streamsize ispCoutPrecision = cout.precision();	// Standardanzahl an Stellen bei der Double-Ausgabe
	unsigned long long ispMaxMCSP = 0ULL;			// Maximale MCSP
	unsigned long long ispMaxAttempts = 0ULL;		// Maximale JumpAttempts
	double ispNextShortProgress = 0.01;				// Gibt die Prozentzahl an, bei der der naechste kurze Forschrittsbericht ausgegeben wird
	double ispNextLongProgress = 0.1;				// Gibt die Prozentzahl an, bei der der naechste lange Forschrittsbericht ausgegeben wird
	bool ispIsMCSPLimited = false;					// Flag: true = MCSP sind limitiert, false = JumpAttempts sind limitiert
	bool ispIsSavingProbs = false;					// Flag fuer Wahrscheinlichkeitsaufzeichnung: true = wird durchgefuehrt, false = deaktiviert

	// Loop-Zielwerte aufsetzen
	if (numeric_limits<unsigned long long>::is_specialized == false)
	{
		cout << "Critical Error: Unsigned long long limit unavailable (TSimulationBase::SPSimulate)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (((spTargetMCSP == 0ULL) && (spTargetJumpAttempts == 0ULL)) ||
		((spTargetMCSP > 0ULL) && (spTargetJumpAttempts > 0ULL)))
	{
		cout << "Critical Error: Inconsistent target values (TSimulationBase::SPSimulate)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	ispMaxMCSP = (unsigned long long)(0.9 * double(numeric_limits<unsigned long long>::max()));		// Faktor 0.9 um etwas Ueberlaufpuffer zu haben (insb. bei JumpAttempts noetig)
	ispMaxAttempts = (unsigned long long)(0.9 * double(numeric_limits<unsigned long long>::max()));
	if (spTargetMCSP > 0ULL)
	{
		if (spMCSP > spTargetMCSP)
		{
			cout << "Critical Error: TargetMCSP smaller than MCSP (TSimulationBase::SPSimulate)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		ispMaxMCSP = spTargetMCSP;
		ispIsMCSPLimited = true;
	}
	if (spTargetJumpAttempts > 0ULL)
	{
		if (spJumpAttempts > spTargetJumpAttempts)
		{
			cout << "Critical Error: TargetJumpAttempts smaller than JumpAttempts (TSimulationBase::SPSimulate)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		ispMaxAttempts = spTargetJumpAttempts;
		ispIsMCSPLimited = false;
	}

	// Wahrscheinlichkeitsspeicherflag setzen
	ispIsSavingProbs = (spProbListSize > 0);

	// Start-Zeit speichern, Laufzeit setzen
	cout << "Simulation started at ";
	spStartTime = TCustomTime::GetCurrentTime(true);
	cout << endl;
	spRunTime = spPreviousRunTime;

	// Start: ------------- Aeussere KMC-Schleife --------------
	while ((spMCSP < ispMaxMCSP) && (spJumpAttempts < ispMaxAttempts))
	{
		// Forschritt anzeigen und zwischenspeichern (wird maximal 100-mal durchlaufen, daher nicht Performance-relevant und nicht optimiert)
		if (ispIsMCSPLimited == true)
		{
			if ((spMCSP >= ispNextShortProgress * ispMaxMCSP) || (spMCSP >= ispNextLongProgress * ispMaxMCSP))
			{
				// ShortProgress wird in jedem Fall hochgesetzt, d.h. es kann zwischendurch als Fortschrittspeicher verwendet werden
				ispNextShortProgress = double(spMCSP) / double(ispMaxMCSP);
				cout << "Progress: ";
				cout << fixed << setprecision(1) << ispNextShortProgress * 100.0 << resetiosflags(ios::floatfield) << setprecision(ispCoutPrecision);
				cout << " % (" << spMCSP << " of " << ispMaxMCSP << " MCSP)" << endl;

				// Laufzeit setzen
				spRunTime = spPreviousRunTime + (TCustomTime::GetCurrentTime(false) - spStartTime);

				// Zwischenstand in ispPhase uebertragen
				ispErrorCode = GetSimulationData(ispPhase);
				if (ispErrorCode != KMCERR_OK) return ispErrorCode;

				// Zwischenstand ausgeben (und ggf. 10 % hinzufuegen fuer naechsten langen Zwischenstand)
				if (spMCSP >= ispNextLongProgress * ispMaxMCSP)
				{
					ispErrorCode = PhaseAnalysis("  ", false, ispPhase, false);
					if (ispErrorCode != KMCERR_OK) return ispErrorCode;

					ispNextLongProgress = ispNextShortProgress + 0.1;
				}
				else
				{
					ispErrorCode = PhaseAnalysis("  ", true, ispPhase, false);
					if (ispErrorCode != KMCERR_OK) return ispErrorCode;
				}

				// Zwischenstand speichern
				SaveCheckPoint("  ", CheckPointPath);

				// 1 % hinzufuegen fuer naechsten kurzen Zwischenstand
				ispNextShortProgress += 0.01;
			}
		}
		else
		{
			if ((spJumpAttempts > ispNextShortProgress * ispMaxAttempts) || (spJumpAttempts > ispNextLongProgress * ispMaxAttempts))
			{

				// ShortProgress wird in jedem Fall hochgesetzt, d.h. es kann zwischendurch als Fortschrittsspeicher verwendet werden
				ispNextShortProgress = double(spJumpAttempts) / double(ispMaxAttempts);
				cout << "Progress: ";
				cout << fixed << setprecision(1) << ispNextShortProgress * 100 << resetiosflags(ios::floatfield) << setprecision(ispCoutPrecision);
				cout << " % (" << spJumpAttempts << " of " << ispMaxAttempts << " Jump Attempts)" << endl;

				// Laufzeit setzen
				spRunTime = spPreviousRunTime + (TCustomTime::GetCurrentTime(false) - spStartTime);

				// Zwischenstand in ispPhase uebertragen
				ispErrorCode = GetSimulationData(ispPhase);
				if (ispErrorCode != KMCERR_OK) return ispErrorCode;

				// Zwischenstand ausgeben (und ggf. 10 % hinzufuegen fuer naechsten langen Zwischenstand)
				if (spJumpAttempts > ispNextLongProgress * ispMaxAttempts)
				{
					ispErrorCode = PhaseAnalysis("  ", false, ispPhase, false);
					if (ispErrorCode != KMCERR_OK) return ispErrorCode;

					ispNextLongProgress = ispNextShortProgress + 0.1;
				}
				else
				{
					ispErrorCode = PhaseAnalysis("  ", true, ispPhase, false);
					if (ispErrorCode != KMCERR_OK) return ispErrorCode;
				}

				// Zwischenstand speichern
				SaveCheckPoint("  ", CheckPointPath);

				// 1 % hinzufuegen fuer naechsten kurzen Zwischenstand
				ispNextShortProgress += 0.01;
			}
		}

		// Start: ------------- Innere KMC-Schleife --------------
		// Performance-relevanter Bereich: alle Sprungablehnungen finden in dieser inneren Schleife statt, 
		// sodass diese in der Regel deutlich haeufiger durchlaufen wird als die aeussere Schleife.
		unsigned long long ispMovCounter = 0ULL;		// Zaehler fuer die Atome der beweglichen Spezies (fuer MCS = MCSP * TotalMovAnz)
		unsigned long long ispCurrAttempts = 0ULL;	// Anzahl der Sprungversuche fuer den aktuellen MCS (fuer spAttemptPathRatioSum)
		while ((ispMovCounter < spTotalMovAnz) && (spJumpAttempts < ispMaxAttempts))
		{

			// Leerstelle und Sprungrichtung wuerfeln
			unsigned long long ispVacSel = spDirDist(spRNG);
			int ispDirType = 0;
			while (ispVacSel >= spVacListSizes[ispDirType])
			{
				ispVacSel -= spVacListSizes[ispDirType];
				ispDirType++;
			}
			T4DLatticeVector* ispCurrVac = (*(spVacLists + ispDirType)) + (ispVacSel / spDirCount[ispDirType]);		// Pointer, der auf die aktuell untersuchte Leerstelle zeigt
			TSimJump* ispCurrJump = (*(spJumps + ispCurrVac->s)) + (ispVacSel % spDirCount[ispDirType]);		// Pointer, der auf den aktuell untersuchten Jump zeigt

			// Pruefen ob Leerstelle an Zielposition
			// -> Anmerkung zu den periodischen Randbedingungen: Eine alternative Methode waere jeweils:
			// isp4DDestX -= (isp4DDestX/spLatticeSize)*spLatticeSize;		-> isp4DDestX ist im Interval [-(spLatticeSize-1), spLatticeSize-1]
			// if (isp4DDestX < 0) isp4DDestX += spLatticeSize;				-> isp4DDestX ist im Interval [0, spLatticeSize-1]
			// Die /-Operation ist allerdings relativ teuer und die Ueberschreitung der Grenzen ist bei groesseren
			// Gittern relativ selten, sodass eine while-Schleife guenstiger sein sollte. Einige durchgefuehrte 
			// Profiling-Messungen unterstuetzen diese Annahme. Im guenstigsten (und haeufigsten) Fall von keiner 
			// Grenzueberschreitung werden bei der Methode unten pro Koordinate zwei if-Anweisungen ausgefuehrt 
			// (jeweils der Beginn der while-Schleife), waehrend bei der alternativen Methode oben mindestens eine 
			// Ganzzahldivision (teuer), eine Multiplikation und eine if-Anweisung ausgefuehrt werden.
			// -> Anmerkung zur size_t-Konvertierung: Diese ist leider noetig, denn falls die Anzahl der Elemente
			// im Gitter den int-Bereich uebersteigt, so wuerde die Multiplikation und Addition der int-Variablen zu einem
			// int-Ueberlauf zwischendurch und damit zu einem falschen Index fuehren.
			int isp4DDestX = ispCurrVac->x + ispCurrJump->destination.x;		// x-Koordinate der Zielposition
			int isp4DDestY = ispCurrVac->y + ispCurrJump->destination.y;		// y-Koordinate der Zielposition
			int isp4DDestZ = ispCurrVac->z + ispCurrJump->destination.z;		// z-Koordinate der Zielposition
			int isp4DDestS = ispCurrVac->s + ispCurrJump->destination.s;		// s-Koordinate der Zielposition
			while (isp4DDestX < 0) isp4DDestX += spLatticeSize;
			while (isp4DDestX >= spLatticeSize) isp4DDestX -= spLatticeSize;
			while (isp4DDestY < 0) isp4DDestY += spLatticeSize;
			while (isp4DDestY >= spLatticeSize) isp4DDestY -= spLatticeSize;
			while (isp4DDestZ < 0) isp4DDestZ += spLatticeSize;
			while (isp4DDestZ >= spLatticeSize) isp4DDestZ -= spLatticeSize;
			while (isp4DDestS < 0) isp4DDestS += spStackSize;
			while (isp4DDestS >= spStackSize) isp4DDestS -= spStackSize;
			size_t ispCurrDestIndex = (((size_t)isp4DDestX * spLatticeSize + isp4DDestY) * spLatticeSize +		// Aktueller spLattice-Index der Sprungzielposition
				isp4DDestZ) * spStackSize + isp4DDestS;
			if (spLattice[ispCurrDestIndex] == 1)
			{
				++spSiteBlockingCounter;
				++spJumpAttempts;				// Site-Blocking: Sprung immer abgelehnt + zaehlt als Sprungversuch
				++ispCurrAttempts;
				continue;
			}

			// ------ Hinsprung berechnen ------

			// E-Feld setzen
			double ispCurrProb = ispCurrJump->efield_contrib;		// Sprungwahrscheinlichkeit (fuer Hinsprung)

			// UniqueJump setzen
			TSimUniqueJump* ispCurrUniqueJump = ispCurrJump->unique_jump;		// Pointer, der auf den aktuell untersuchten UniqueJump zeigt

			// Kodierte Umgebung untersuchen
			int ispMaxCounter = ispCurrJump->code_envpos_size;		// Temporaere Zaehlergrenze
			if (ispMaxCounter > 0)
			{

				size_t ispHashIndex = 0;		// CodeID, die aus der Hash-Funktion berechnet wird

				// Hash-Index berechnen
				for (int ispCounter = 0; ispCounter < ispMaxCounter; ++ispCounter)
				{

					T4DLatticeVector* ispCurrPos = ispCurrJump->code_envpos + ispCounter;		// Pointer, der auf die aktuell untersuchte Position zeigt
					int isp4DCoordX = ispCurrVac->x + ispCurrPos->x;							// Temporaere x-Koordinate
					int isp4DCoordY = ispCurrVac->y + ispCurrPos->y;							// Temporaere y-Koordinate
					int isp4DCoordZ = ispCurrVac->z + ispCurrPos->z;							// Temporaere z-Koordinate
					int isp4DCoordS = ispCurrVac->s + ispCurrPos->s;							// Temporaere s-Koordinate
					while (isp4DCoordX < 0) isp4DCoordX += spLatticeSize;
					while (isp4DCoordX >= spLatticeSize) isp4DCoordX -= spLatticeSize;
					while (isp4DCoordY < 0) isp4DCoordY += spLatticeSize;
					while (isp4DCoordY >= spLatticeSize) isp4DCoordY -= spLatticeSize;
					while (isp4DCoordZ < 0) isp4DCoordZ += spLatticeSize;
					while (isp4DCoordZ >= spLatticeSize) isp4DCoordZ -= spLatticeSize;
					while (isp4DCoordS < 0) isp4DCoordS += spStackSize;
					while (isp4DCoordS >= spStackSize) isp4DCoordS -= spStackSize;
					ispHashIndex += ispCurrUniqueJump->hash_mult[ispCounter] *
						ispCurrUniqueJump->hash_map[ispCounter][spLattice[(((size_t)isp4DCoordX * spLatticeSize +
							isp4DCoordY) * spLatticeSize + isp4DCoordZ) * spStackSize + isp4DCoordS]];
				}

				// Code-Energie hinzufuegen
				ispCurrProb *= ispCurrUniqueJump->code_energies[ispHashIndex];
			}

			// Additive Umgebung untersuchen
			ispMaxCounter = ispCurrJump->add_envpos_size;
			if (ispMaxCounter > 0)
			{

				// Additive Beitraege hinzufuegen
				for (int ispCounter = 0; ispCounter < ispMaxCounter; ++ispCounter)
				{

					T4DLatticeVector* ispCurrPos = ispCurrJump->add_envpos + ispCounter;		// Pointer, der auf die aktuell untersuchte Position zeigt
					int isp4DCoordX = ispCurrVac->x + ispCurrPos->x;							// Temporaere x-Koordinate
					int isp4DCoordY = ispCurrVac->y + ispCurrPos->y;							// Temporaere y-Koordinate
					int isp4DCoordZ = ispCurrVac->z + ispCurrPos->z;							// Temporaere z-Koordinate
					int isp4DCoordS = ispCurrVac->s + ispCurrPos->s;							// Temporaere s-Koordinate
					while (isp4DCoordX < 0) isp4DCoordX += spLatticeSize;
					while (isp4DCoordX >= spLatticeSize) isp4DCoordX -= spLatticeSize;
					while (isp4DCoordY < 0) isp4DCoordY += spLatticeSize;
					while (isp4DCoordY >= spLatticeSize) isp4DCoordY -= spLatticeSize;
					while (isp4DCoordZ < 0) isp4DCoordZ += spLatticeSize;
					while (isp4DCoordZ >= spLatticeSize) isp4DCoordZ -= spLatticeSize;
					while (isp4DCoordS < 0) isp4DCoordS += spStackSize;
					while (isp4DCoordS >= spStackSize) isp4DCoordS -= spStackSize;
					ispCurrProb *= ispCurrUniqueJump->add_energies[ispCounter][spLattice[(((size_t)isp4DCoordX * spLatticeSize +
						isp4DCoordY) * spLatticeSize + isp4DCoordZ) * spStackSize + isp4DCoordS]];
				}
			}

			// Wahrscheinlichkeitspruefung
			// Anmerkungen:
			// - ein Gitterplatz ist nicht definiert, wenn der (bzw. ein) Sprung von dort weg eine Wahrscheinlichkeit >= 1.0 hat
			//   (d.h. wenn die Aktivierungsenergie <= 0 eV ist)
			// - Die Definiertheit eines Gitterplatzes bezieht sich auf die echte Migrationsenergie, d.h. ohne Normierung
			// - Startplatz definiert + Zielplatz definiert -> RNG[0,1) < Sprungwahrscheinlichkeit pruefen + zaehlt als Sprungversuch
			// - Startplatz definiert + Zielplatz nicht definiert -> Sprung immer abgelehnt + zaehlt als Sprungversuch
			// - Startplatz nicht definiert + Zielplatz definiert -> Sprung immer angenommen + zaehlt nicht als Sprungversuch
			// - Startplatz nicht definiert + Zielplatz nicht definiert -> Sprung immer angenommen + zaehlt nicht als Sprungversuch
			// - Jeder erfolgreiche Sprungversuch (!) ist ein Monte-Carlo-Step
			if (ispCurrProb < 1.0)
			{
				// -> Startplatz ist definiert

				// ab hier: Startplatz definiert -> Sprungversuch
				++spJumpAttempts;
				++ispCurrAttempts;

				// ------ Ruecksprung berechnen ------
				if (spIfBackjumpRelevant == true)
				{

					// Ruecksprung setzen
					TSimJump* ispCurrBackjump = ispCurrJump->back_jump;

					// E-Feld setzen
					double ispCurrBackjumpProb = ispCurrBackjump->efield_contrib;		// Sprungwahrscheinlichkeit fuer den Ruecksprung

					// UniqueJump setzen
					TSimUniqueJump* ispCurrUniqueBackjump = ispCurrBackjump->unique_jump;		// Pointer, der auf den aktuell untersuchten UniqueJump zeigt

					// Kodierte Umgebung untersuchen
					ispMaxCounter = ispCurrBackjump->code_envpos_size;		// Temporaere Zaehlergrenze
					if (ispMaxCounter > 0)
					{

						size_t ispHashIndex = 0;		// CodeID, die aus der Hash-Funktion berechnet wird

						// Hash-Index berechnen
						for (int ispCounter = 0; ispCounter < ispMaxCounter; ++ispCounter)
						{

							T4DLatticeVector* ispCurrPos = ispCurrBackjump->code_envpos + ispCounter;	// Pointer, der auf die aktuell untersuchte Position zeigt
							int isp4DCoordX = isp4DDestX + ispCurrPos->x;								// Temporaere x-Koordinate
							int isp4DCoordY = isp4DDestY + ispCurrPos->y;								// Temporaere y-Koordinate
							int isp4DCoordZ = isp4DDestZ + ispCurrPos->z;								// Temporaere z-Koordinate
							int isp4DCoordS = isp4DDestS + ispCurrPos->s;								// Temporaere s-Koordinate
							while (isp4DCoordX < 0) isp4DCoordX += spLatticeSize;
							while (isp4DCoordX >= spLatticeSize) isp4DCoordX -= spLatticeSize;
							while (isp4DCoordY < 0) isp4DCoordY += spLatticeSize;
							while (isp4DCoordY >= spLatticeSize) isp4DCoordY -= spLatticeSize;
							while (isp4DCoordZ < 0) isp4DCoordZ += spLatticeSize;
							while (isp4DCoordZ >= spLatticeSize) isp4DCoordZ -= spLatticeSize;
							while (isp4DCoordS < 0) isp4DCoordS += spStackSize;
							while (isp4DCoordS >= spStackSize) isp4DCoordS -= spStackSize;
							ispHashIndex += ispCurrUniqueBackjump->hash_mult[ispCounter] *
								ispCurrUniqueBackjump->hash_map[ispCounter][spLattice[(((size_t)isp4DCoordX * spLatticeSize +
									isp4DCoordY) * spLatticeSize + isp4DCoordZ) * spStackSize + isp4DCoordS]];
						}

						// Code-Energie hinzufuegen
						ispCurrBackjumpProb *= ispCurrUniqueBackjump->code_energies[ispHashIndex];
					}

					// Additive Umgebung untersuchen
					ispMaxCounter = ispCurrBackjump->add_envpos_size;
					if (ispMaxCounter > 0)
					{

						// Additive Beitraege hinzufuegen
						for (int ispCounter = 0; ispCounter < ispMaxCounter; ++ispCounter)
						{

							T4DLatticeVector* ispCurrPos = ispCurrBackjump->add_envpos + ispCounter;	// Pointer, der auf die aktuell untersuchte Position zeigt
							int isp4DCoordX = isp4DDestX + ispCurrPos->x;								// Temporaere x-Koordinate
							int isp4DCoordY = isp4DDestY + ispCurrPos->y;								// Temporaere y-Koordinate
							int isp4DCoordZ = isp4DDestZ + ispCurrPos->z;								// Temporaere z-Koordinate
							int isp4DCoordS = isp4DDestS + ispCurrPos->s;								// Temporaere s-Koordinate
							while (isp4DCoordX < 0) isp4DCoordX += spLatticeSize;
							while (isp4DCoordX >= spLatticeSize) isp4DCoordX -= spLatticeSize;
							while (isp4DCoordY < 0) isp4DCoordY += spLatticeSize;
							while (isp4DCoordY >= spLatticeSize) isp4DCoordY -= spLatticeSize;
							while (isp4DCoordZ < 0) isp4DCoordZ += spLatticeSize;
							while (isp4DCoordZ >= spLatticeSize) isp4DCoordZ -= spLatticeSize;
							while (isp4DCoordS < 0) isp4DCoordS += spStackSize;
							while (isp4DCoordS >= spStackSize) isp4DCoordS -= spStackSize;
							ispCurrBackjumpProb *= ispCurrUniqueBackjump->add_energies[ispCounter][spLattice[(((size_t)isp4DCoordX * spLatticeSize +
								isp4DCoordY) * spLatticeSize + isp4DCoordZ) * spStackSize + isp4DCoordS]];
						}
					}

					// Pruefen ob Zielplatz nicht definiert ist
					if (ispCurrBackjumpProb >= 1.0)
					{
						++spNonsenseAttempts;
						continue;
					}
				}

				// Normierung hinzufuegen
				ispCurrProb *= spNormalization;

				// Wahrscheinlichkeit speichern
				if (ispIsSavingProbs == true)
				{
					if (ispCurrProb >= KMCVAR_EQTHRESHOLD_PROBMINUS * spAttemptProbList[spProbListSize - 1].prob)
					{
						bool ispProbFound = false;
						if (spProbListSize != 1)
						{
							for (int ispListCounter = spProbListSize - 1; ispListCounter > 0; --ispListCounter)
							{
								if (spAttemptProbList[ispListCounter].count == 0ULL)
								{
									// -> Listeneintrag unbesetzt
									if (spAttemptProbList[ispListCounter - 1].count == 0ULL) continue;		// -> naechster Listenplatz auch noch unbesetzt
									if (ispCurrProb < KMCVAR_EQTHRESHOLD_PROBMINUS * spAttemptProbList[ispListCounter - 1].prob)
									{
										// -> Wahrscheinlichkeit kleiner als letzter besetzter Listenplatz -> letzten unbesetzten Listenplatz besetzen
										spAttemptProbList[ispListCounter].prob = ispCurrProb;
										spAttemptProbList[ispListCounter].count = 1ULL;
										ispProbFound = true;
										break;
									}
								}
								else
								{
									// ->Listenplatz ist besetzt
									if ((ispCurrProb >= KMCVAR_EQTHRESHOLD_PROBMINUS * spAttemptProbList[ispListCounter].prob) &&
										(ispCurrProb <= KMCVAR_EQTHRESHOLD_PROBPLUS * spAttemptProbList[ispListCounter].prob))
									{
										// -> Wahrscheinlichkeit gehoert zu diesem Listeneintrag -> Zaehler hochzaehlen
										++(spAttemptProbList[ispListCounter].count);
										ispProbFound = true;
										break;
									}
									if (ispCurrProb < KMCVAR_EQTHRESHOLD_PROBMINUS* spAttemptProbList[ispListCounter - 1].prob)
									{
										// -> Wahrscheinlichkeit gehoert zwischen diesen und den naechsten Listenplatz -> neuen Listeneintrag einfuegen
										if (ispListCounter != spProbListSize - 1)
										{
											// -> uebrige Listeneintraege um einen Platz nach unten verschieben
											for (int ispShiftListCounter = spProbListSize - 1; ispShiftListCounter > ispListCounter; --ispShiftListCounter)
											{
												spAttemptProbList[ispShiftListCounter].prob = spAttemptProbList[ispShiftListCounter - 1].prob;
												spAttemptProbList[ispShiftListCounter].count = spAttemptProbList[ispShiftListCounter - 1].count;
											}
										}
										// -> neuen Listeneintrag einfuegen
										spAttemptProbList[ispListCounter].prob = ispCurrProb;
										spAttemptProbList[ispListCounter].count = 1ULL;
										ispProbFound = true;
										break;
									}
								}
							}
						}
						if (ispProbFound == false)
						{
							// -> hoechsten Listeneintrag pruefen
							if ((ispCurrProb >= KMCVAR_EQTHRESHOLD_PROBMINUS * spAttemptProbList[0].prob) &&
								(ispCurrProb <= KMCVAR_EQTHRESHOLD_PROBPLUS * spAttemptProbList[0].prob))
							{
								// -> Wahrscheinlichkeit gehoert zum hoechsten Listeneintrag -> Zaehler hochzaehlen
								++(spAttemptProbList[0].count);
							}
							if (ispCurrProb > KMCVAR_EQTHRESHOLD_PROBPLUS * spAttemptProbList[0].prob)
							{
								// -> Wahrscheinlichkeit gehoert noch vor den hoechsten Listeneintrag -> neuen Listeneintrag vor dem hoechsten Listeneintrag einfuegen
								if (spProbListSize != 1)
								{
									// -> uebrige Listeneintrage um einen Platz nach unten verschieben
									for (int ispShiftListCounter = spProbListSize - 1; ispShiftListCounter > 0; --ispShiftListCounter)
									{
										spAttemptProbList[ispShiftListCounter].prob = spAttemptProbList[ispShiftListCounter - 1].prob;
										spAttemptProbList[ispShiftListCounter].count = spAttemptProbList[ispShiftListCounter - 1].count;
									}
								}
								// -> neuen hoechsten Listeneintrag einfuegen
								spAttemptProbList[0].prob = ispCurrProb;
								spAttemptProbList[0].count = 1ULL;
							}
						}
					}
				}

				// Wahrscheinlichkeit pruefen (= Sprungversuch)
				// Sprung wird durchgefuehrt, wenn ispCurrProb > RNG[0,1)
				// -> Ablehnung bei ispCurrProb <= RNG[0,1)
				if (ispCurrProb <= spProbDist(spRNG)) continue;

				// ab hier: Sprung ist angenommen, d.h. Monte-Carlo-Step wird durchgefuehrt
				++ispMovCounter;
				spAttemptPathRatioSum += double(ispCurrAttempts) / (double(spVacMaxIndex) + 1.0);
				ispCurrAttempts = 0ULL;

				// Wahrscheinlichkeit speichern
				if (ispIsSavingProbs == true)
				{
					if (ispCurrProb >= KMCVAR_EQTHRESHOLD_PROBMINUS * spAcceptedProbList[spProbListSize - 1].prob)
					{
						bool ispProbFound = false;
						if (spProbListSize != 1)
						{
							for (int ispListCounter = spProbListSize - 1; ispListCounter > 0; --ispListCounter)
							{
								if (spAcceptedProbList[ispListCounter].count == 0ULL)
								{
									// -> Listeneintrag unbesetzt
									if (spAcceptedProbList[ispListCounter - 1].count == 0ULL) continue;		// -> naechster Listenplatz auch noch unbesetzt
									if (ispCurrProb < KMCVAR_EQTHRESHOLD_PROBMINUS * spAcceptedProbList[ispListCounter - 1].prob)
									{
										// -> Wahrscheinlichkeit kleiner als letzter besetzter Listenplatz -> letzten unbesetzten Listenplatz besetzen
										spAcceptedProbList[ispListCounter].prob = ispCurrProb;
										spAcceptedProbList[ispListCounter].count = 1ULL;
										ispProbFound = true;
										break;
									}
								}
								else
								{
									// ->Listenplatz ist besetzt
									if ((ispCurrProb >= KMCVAR_EQTHRESHOLD_PROBMINUS * spAcceptedProbList[ispListCounter].prob) &&
										(ispCurrProb <= KMCVAR_EQTHRESHOLD_PROBPLUS * spAcceptedProbList[ispListCounter].prob))
									{
										// -> Wahrscheinlichkeit gehoert zu diesem Listeneintrag -> Zaehler hochzaehlen
										++(spAcceptedProbList[ispListCounter].count);
										ispProbFound = true;
										break;
									}
									if (ispCurrProb < KMCVAR_EQTHRESHOLD_PROBMINUS* spAcceptedProbList[ispListCounter - 1].prob)
									{
										// -> Wahrscheinlichkeit gehoert zwischen diesen und den naechsten Listenplatz -> neuen Listeneintrag einfuegen
										if (ispListCounter != spProbListSize - 1)
										{
											// -> uebrige Listeneintraege um einen Platz nach unten verschieben
											for (int ispShiftListCounter = spProbListSize - 1; ispShiftListCounter > ispListCounter; --ispShiftListCounter)
											{
												spAcceptedProbList[ispShiftListCounter].prob = spAcceptedProbList[ispShiftListCounter - 1].prob;
												spAcceptedProbList[ispShiftListCounter].count = spAcceptedProbList[ispShiftListCounter - 1].count;
											}
										}
										// -> neuen Listeneintrag einfuegen
										spAcceptedProbList[ispListCounter].prob = ispCurrProb;
										spAcceptedProbList[ispListCounter].count = 1ULL;
										ispProbFound = true;
										break;
									}
								}
							}
						}
						if (ispProbFound == false)
						{
							// -> hoechsten Listeneintrag pruefen
							if ((ispCurrProb >= KMCVAR_EQTHRESHOLD_PROBMINUS * spAcceptedProbList[0].prob) &&
								(ispCurrProb <= KMCVAR_EQTHRESHOLD_PROBPLUS * spAcceptedProbList[0].prob))
							{
								// -> Wahrscheinlichkeit gehoert zum hoechsten Listeneintrag -> Zaehler hochzaehlen
								++(spAcceptedProbList[0].count);
							}
							if (ispCurrProb > KMCVAR_EQTHRESHOLD_PROBPLUS * spAcceptedProbList[0].prob)
							{
								// -> Wahrscheinlichkeit gehoert noch vor den hoechsten Listeneintrag -> neuen Listeneintrag vor dem hoechsten Listeneintrag einfuegen
								if (spProbListSize != 1)
								{
									// -> uebrige Listeneintrage um einen Platz nach unten verschieben
									for (int ispShiftListCounter = spProbListSize - 1; ispShiftListCounter > 0; --ispShiftListCounter)
									{
										spAcceptedProbList[ispShiftListCounter].prob = spAcceptedProbList[ispShiftListCounter - 1].prob;
										spAcceptedProbList[ispShiftListCounter].count = spAcceptedProbList[ispShiftListCounter - 1].count;
									}
								}
								// -> neuen hoechsten Listeneintrag einfuegen
								spAcceptedProbList[0].prob = ispCurrProb;
								spAcceptedProbList[0].count = 1ULL;
							}
						}
					}
				}

			}
			else
			{
				// -> Startplatz ist nicht definiert -> Sprung wird durchgefuehrt aber spJumpAttempts, ispCurrAttempts und ispMovCounter werden nicht erhoeht
				++spOverkillAttempts;
			}

			// ------ Sprung durchfuehren ------
			// Bewegung speichern
			T3DMovVector* ispCurrStartMov = spMovLattice + ((((size_t)ispCurrVac->x * spLatticeSize + ispCurrVac->y) * spLatticeSize +
				ispCurrVac->z) * spMovStackSize + ispCurrVac->s);
			T3DMovVector* ispCurrDestMov = spMovLattice + ((((size_t)isp4DDestX * spLatticeSize + isp4DDestY) * spLatticeSize +
				isp4DDestZ) * spMovStackSize + isp4DDestS);
			++(ispCurrStartMov->jcount);
			++(ispCurrDestMov->jcount);
			ispCurrStartMov->x += ispCurrJump->jump_vec.x;
			ispCurrDestMov->x -= ispCurrJump->jump_vec.x;
			ispCurrStartMov->y += ispCurrJump->jump_vec.y;
			ispCurrDestMov->y -= ispCurrJump->jump_vec.y;
			ispCurrStartMov->z += ispCurrJump->jump_vec.z;
			ispCurrDestMov->z -= ispCurrJump->jump_vec.z;

			// Positionen im Gitter zur Bewegungsspeicherung tauschen
			T3DMovVector ispSwapMov = *ispCurrStartMov;
			*ispCurrStartMov = *ispCurrDestMov;
			*ispCurrDestMov = ispSwapMov;

			// Gitterpositionen tauschen
			spLattice[(((size_t)ispCurrVac->x * spLatticeSize + ispCurrVac->y) * spLatticeSize + ispCurrVac->z) * spStackSize + ispCurrVac->s] = 0;
			spLattice[ispCurrDestIndex] = 1;

			// Auswahlarrays und Leerstellenposition aktualisieren
			int ispDestDirType = spDirLink[isp4DDestS];
			if (ispDirType != ispDestDirType)
			{
				T4DLatticeVector* ispDestVac = (*(spVacLists + ispDestDirType)) + (spVacListSizes[ispDestDirType] / spDirCount[ispDestDirType]);
				ispDestVac->x = isp4DDestX;
				ispDestVac->y = isp4DDestY;
				ispDestVac->z = isp4DDestZ;
				ispDestVac->s = isp4DDestS;
				spVacListSizes[ispDestDirType] += spDirCount[ispDestDirType];
				if (ispVacSel < spVacListSizes[ispDirType] - spDirCount[ispDirType])
				{
					T4DLatticeVector* ispLastVac = (*(spVacLists + ispDirType)) + (spVacListSizes[ispDirType] / spDirCount[ispDirType] - 1);
					*ispCurrVac = *ispLastVac;
					ispLastVac->x = 0;
					ispLastVac->y = 0;
					ispLastVac->z = 0;
					ispLastVac->s = 0;
				}
				else
				{
					ispCurrVac->x = 0;
					ispCurrVac->y = 0;
					ispCurrVac->z = 0;
					ispCurrVac->s = 0;
				}
				spVacListSizes[ispDirType] -= spDirCount[ispDirType];
				spVacMaxIndex += spDirCount[ispDestDirType] - spDirCount[ispDirType];
				spDirDist = uniform_int_distribution<unsigned long long>(0, spVacMaxIndex);
			}
			else
			{
				ispCurrVac->x = isp4DDestX;
				ispCurrVac->y = isp4DDestY;
				ispCurrVac->z = isp4DDestZ;
				ispCurrVac->s = isp4DDestS;
			}
		}
		// Ende: -------------- Innere KMC-Schleife --------------

		// MCSP hochzaehlen
		if (ispMovCounter >= spTotalMovAnz)
		{
			++spMCSP;
		}
	}
	// Ende: -------------- Aeussere KMC-Schleife --------------

	// Ergebnis ausgeben
	if (ispIsMCSPLimited == true)
	{
		if (spMCSP != ispMaxMCSP)
		{
			cout << "Critical Error: JumpAttempts counter variable limit reached (TSimulationBase::SPSimulate)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		cout << "Progress: 100 % (" << spMCSP << " of " << ispMaxMCSP << " MCSP)" << endl;
	}
	else
	{
		if (spJumpAttempts != ispMaxAttempts)
		{
			cout << "Critical Error: MCSP counter variable limit reached (TSimulationBase::SPSimulate)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		cout << "Progress: 100 % (" << spJumpAttempts << " of " << ispMaxAttempts << " Jump Attempts)" << endl;
	}

	// Zeit ausgeben, Laufzeit speichern
	cout << "Simulation completed at ";
	spRunTime = spPreviousRunTime + (TCustomTime::GetCurrentTime(true) - spStartTime);
	cout << endl;

	// Endstand in ispPhase uebertragen
	ispErrorCode = GetSimulationData(ispPhase);
	if (ispErrorCode != KMCERR_OK) return ispErrorCode;

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //

// Alle dynamisch erzeugten Arrays loeschen, uebrige Daten auf Standardwerte setzen, Ready = false setzen
void TSimulationBase::Clear()
{

	Ready = false;
	Completed = false;

	delete[] spDirCount;
	spDirCount = NULL;

	delete[] spDirLink;
	spDirLink = NULL;

	delete[] spUniqueJumps;
	spUniqueJumps = NULL;

	if (spJumps != NULL)
	{
		if (spMovStackSize > 0)
		{
			for (int i = spMovStackSize - 1; i >= 0; i--)
			{
				delete[] spJumps[i];
				spJumps[i] = NULL;
			}
		}
		delete[] spJumps;
		spJumps = NULL;
	}

	CheckPointPath = "";
	StartLattice.clear();

	PrerunPhaseData.Clear();
	DynNormPhaseData.Clear();
	MainPhaseData.Clear();

	spTotalMovAnz = 0ULL;
	spTotalVacAnz = 0ULL;
	spLatticeSize = 0;
	spStackSize = 0;
	spMovStackSize = 0;
	spProbDist.reset();
	spDirSelCount = 0;
	spHighestProb = 1.0;
	spIfBackjumpRelevant = false;
	spFrequency = 1.0;
	spCanCalcCond = false;
	spCondFactor = 0.0;

	ClearCurrentSim();

}

// Alle variablen Simulationsparameter zuruecksetzen
void TSimulationBase::ClearCurrentSim()
{

	delete[] spLattice;
	spLattice = NULL;
	delete[] spMovLattice;
	spMovLattice = NULL;
	delete[] spVacListSizes;
	spVacListSizes = NULL;
	delete[] spAttemptProbList;
	spAttemptProbList = NULL;
	delete[] spAcceptedProbList;
	spAcceptedProbList = NULL;
	if (spVacLists != NULL)
	{
		if (spDirSelCount > 0)
		{
			for (int i = spDirSelCount - 1; i >= 0; i--)
			{
				delete[] spVacLists[i];
				spVacLists[i] = NULL;
			}
		}
		delete[] spVacLists;
		spVacLists = NULL;
	}

	spMCSP = 0ULL;
	spTargetMCSP = 0ULL;
	spJumpAttempts = 0ULL;
	spTargetJumpAttempts = 0ULL;
	spNonsenseAttempts = 0ULL;
	spOverkillAttempts = 0ULL;
	spSiteBlockingCounter = 0ULL;
	spVacMaxIndex = 0ULL;
	spDirDist.reset();
	spNormalization = 1.0;
	spAttemptPathRatioSum = 0.0;
	spProbListSize = 0;
	spPreviousRunTime = TCustomTime();
	spStartTime = TCustomTime();
	spRunTime = TCustomTime();

}

// Alle Einstellungsklassen auf Existenz und Ready = true pruefen
int TSimulationBase::IfAllClassesReady() const
{

	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL)
	{
		cout << "Critical Error: m_Elements is null pointer (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false)
	{
		cout << "Critical Error: TElements not ready (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Structure == NULL)
	{
		cout << "Critical Error: m_Structure is null pointer (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() == false)
	{
		cout << "Critical Error: TStructure not ready (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Jumps == NULL)
	{
		cout << "Critical Error: m_Jumps is null pointer (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps->IfReady() == false)
	{
		cout << "Critical Error: TJumps not ready (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_UniqueJumps == NULL)
	{
		cout << "Critical Error: m_UniqueJumps is null pointer (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_UniqueJumps->IfJumpsReady() == false)
	{
		cout << "Critical Error: TUniqueJumps not ready 1 (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_UniqueJumps->IfCodesReady() == false)
	{
		cout << "Critical Error: TUniqueJumps not ready 2 (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Settings == NULL)
	{
		cout << "Critical Error: m_Settings is null pointer (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Settings->IfReady() == false)
	{
		cout << "Critical Error: TSettings not ready (in TSimulationBase::IfAllClassesReady)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	return KMCERR_OK;
}

// Hoechstmoegliche Wahrscheinlichkeit (ohne Normierung) berechnen
int TSimulationBase::GetHighestProb(double& o_prob) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready for normalization (TSimulationBase::GetHighestProb)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	// fuer jeden Jump die hoechstmoegliche Wahrscheinlichkeit berechnen
	vector<int> t_possible_elemids;
	size_t t_max_hash_index = 0;
	int t_max_mapentry = 0;
	double t_highest_code = 0.0;
	double t_highest_add = 0.0;
	double t_highest_elemprob = 0.0;
	double t_highest_total_prob = 0.0;
	for (int i = 0; i < spMovStackSize; i++)
	{
		for (int j = 0; j < spDirCount[spDirLink[i]]; j++)
		{
			// hoechsten additiven Beitrag berechnen
			t_highest_add = 1.0;
			if (spJumps[i][j].add_envpos_size > 0)
			{
				for (int k = 0; k < spJumps[i][j].add_envpos_size; k++)
				{

					ErrorCode = m_Job->m_Structure->GetDopands4ElemID(m_Job->m_Structure->GetElemID(i + spJumps[i][j].add_envpos[k].s),
						true, &t_possible_elemids);
					if (ErrorCode != KMCERR_OK)
					{
						cout << "Critical Error: Cannot retrieve possible elements 1 (TSimulationBase::GetHighestProb)" << endl << endl;
						return ErrorCode;
					}

					t_highest_elemprob = 0.0;
					for (int l = 0; l < (int)t_possible_elemids.size(); l++)
					{
						if (spJumps[i][j].unique_jump->add_energies[k][t_possible_elemids[l]] > t_highest_elemprob)
							t_highest_elemprob = spJumps[i][j].unique_jump->add_energies[k][t_possible_elemids[l]];
					}
					if (t_highest_elemprob == 0.0)
					{
						cout << "Critical Error: Invalid additive energy data (TSimulationBase::GetHighestProb)" << endl << endl;
						return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
					}

					t_highest_add *= t_highest_elemprob;
				}
			}

			// hoechsten Code-Beitrag berechnen
			t_highest_code = 1.0;
			if (spJumps[i][j].code_envpos_size > 0)
			{
				// Code-Anzahl berechnen
				t_max_hash_index = 0;
				for (int k = 0; k < spJumps[i][j].code_envpos_size; k++)
				{

					ErrorCode = m_Job->m_Structure->GetDopands4ElemID(m_Job->m_Structure->GetElemID(i + spJumps[i][j].code_envpos[k].s),
						true, &t_possible_elemids);
					if (ErrorCode != KMCERR_OK)
					{
						cout << "Critical Error: Cannot retrieve possible elements 2 (TSimulationBase::GetHighestProb)" << endl << endl;
						return ErrorCode;
					}

					t_max_mapentry = -1;
					for (int l = 0; l < (int)t_possible_elemids.size(); l++)
					{
						if (spJumps[i][j].unique_jump->hash_map[k][t_possible_elemids[l]] > t_max_mapentry)
							t_max_mapentry = spJumps[i][j].unique_jump->hash_map[k][t_possible_elemids[l]];
					}
					if (t_max_mapentry == -1)
					{
						cout << "Critical Error: Invalid hash map data (TSimulationBase::GetHighestProb)" << endl << endl;
						return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
					}

					t_max_hash_index += t_max_mapentry * spJumps[i][j].unique_jump->hash_mult[k];
				}

				// Code mit der hoechsten Wahrscheinlichkeit suchen
				t_highest_code = 0.0;
				for (size_t k = 0; k < t_max_hash_index + 1; k++)
				{
					if (spJumps[i][j].unique_jump->code_energies[k] > t_highest_code)
						t_highest_code = spJumps[i][j].unique_jump->code_energies[k];
				}
				if (t_highest_code == 0.0)
				{
					cout << "Critical Error: Invalid code energy data (TSimulationBase::GetHighestProb)" << endl << endl;
					return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
				}
			}

			// hoechstmoegliche Sprungwahrscheinlichkeit speichern
			if (spJumps[i][j].efield_contrib * t_highest_add * t_highest_code > t_highest_total_prob)
				t_highest_total_prob = spJumps[i][j].efield_contrib * t_highest_add * t_highest_code;
		}
	}

	// Wahrscheinlichkeit setzen
	o_prob = t_highest_total_prob;

	return KMCERR_OK;
}

// Pruefen ob Leitfaehigkeitsberechnung moeglich und Auswertungsparameter berechnen
int TSimulationBase::GetCondFactor(bool& o_cancalccond, double& o_freq, double& o_condfactor) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::GetCondFactor)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	// Sprungversuchsfrequenz laden (in 1/s)
	double t_freq = 1.0;
	ErrorCode = m_Job->m_Settings->GetAttemptFrequency(t_freq);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	o_freq = t_freq;

	// E-Feld-Betrag laden (in V/cm)
	T3DVector t_efield;
	ErrorCode = m_Job->m_Settings->GetEField(t_efield.x, t_efield.y, t_efield.z);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	double t_efieldstrength = t_efield.Length();

	// CanCalc-Flag setzen
	if (t_efieldstrength < T3DVector::zero_threshold)
	{
		o_cancalccond = false;
		o_condfactor = 0.0;
		return KMCERR_OK;
	}

	// Ladung der beweglichen Spezies laden (in e)
	double t_movcharge = 0.0;
	ErrorCode = m_Job->m_Elements->GetMovCharge(t_movcharge);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve charge of moving species (TSimulationBase::GetCondFactor)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Volumenkonzentration der beweglichen Spezies laden (in cm^-3)
	double t_movvolconc = 0.0;
	ErrorCode = m_Job->m_Settings->GetMovVolConc(t_movvolconc);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Leitfaehigkeitsvorfaktor (= MovCharge * MovVolConc / E-Feld-Betrag in C/(Vcm^2)) berechnen
	o_condfactor = t_movcharge * double(NATCONST_ECHARGE) * t_movvolconc / t_efieldstrength;
	o_cancalccond = true;

	return KMCERR_OK;
}

// Gitter mit zufaelliger Verteilung der Dopanden und Leerstellen erzeugen
int TSimulationBase::CreateRandomLattice(vector<vector<vector<vector<int>>>>* o_lattice)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	// ElemIDs der Elementarzelle ermitteln
	vector<int> t_UCElemIDs;
	ErrorCode = m_Job->m_Structure->GetElemIDs(&t_UCElemIDs);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve element data (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return ErrorCode;
	}
	if ((int)t_UCElemIDs.size() != spStackSize)
	{
		cout << "Critical Error: Invalid element count (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Dotierungen ermitteln
	vector<int> t_DopedIDs;
	vector<int> t_DopandIDs;
	ErrorCode = m_Job->m_Structure->GetDopingIDs(&t_DopedIDs, &t_DopandIDs);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve doping data (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return ErrorCode;
	}
	if (t_DopedIDs.size() != t_DopandIDs.size())
	{
		cout << "Critical Error: Inconsistent doping vectors (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Dotierungsanzahlen ermitteln
	vector<long long> t_DopandAnz;
	ErrorCode = m_Job->m_Settings->GetDopandCounts(&t_DopandAnz);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve doping counts (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return ErrorCode;
	}
	if (t_DopedIDs.size() != t_DopandAnz.size())
	{
		cout << "Critical Error: Inconsistent vector sizes (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Undotiertes Gitter erstellen
	vector<vector<vector<vector<int>>>> t_lattice(spLatticeSize, vector<vector<vector<int>>>(spLatticeSize,
		vector<vector<int>>(spLatticeSize, vector<int>(spStackSize, -1))));
	for (int x = 0; x < spLatticeSize; x++)
	{
		for (int y = 0; y < spLatticeSize; y++)
		{
			for (int z = 0; z < spLatticeSize; z++)
			{
				for (int s = 0; s < spStackSize; s++)
				{
					t_lattice[x][y][z][s] = t_UCElemIDs[s];
				}
			}
		}
	}

	// Fuer jede ElemID die Anzahl in der undotierten Elementarzelle bestimmen
	vector<int> t_UCElemCount((*max_element(t_UCElemIDs.begin(), t_UCElemIDs.end())) + 1, 0);
	for (int i = 0; i < (int)t_UCElemIDs.size(); i++)
	{
		t_UCElemCount[t_UCElemIDs[i]] += 1;
	}

	// Dotierungen hinzufuegen (falls vorhanden)
	size_t t_TotalDopAnz;							// Gesamtanzahl an Dopanden fuer eine ElemID
	size_t t_TotalPosAnz;							// Gesamtanzahl an Positionen fuer eine ElemID im undotierten Gitter
	vector<size_t> t_SelectedPositions;				// zufaellige Positionsnummern im Gitter, in aufsteigender Reihenfolge
	vector<size_t> t_SelDopPos;						// Positionsnummern fuer eine Dotierung
	uniform_int_distribution<size_t> t_SelDist;		// Random-Distribution zur Auswahl der Positionen aus t_SelectedPositions
	size_t t_SelPos;
	size_t t_x;
	size_t t_y;
	size_t t_z;
	size_t t_s;
	bool t_found;
	if (t_DopedIDs.size() != 0)
	{
		for (int i = 0; i < (int)t_UCElemCount.size(); i++)
		{
			if (t_UCElemCount[i] == 0) continue;
			t_TotalPosAnz = size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(t_UCElemCount[i]);

			// Gesamtanzahl an Dopanden fuer die ElemID i berechnen
			t_TotalDopAnz = 0;
			for (int j = 0; j < (int)t_DopedIDs.size(); j++)
			{
				if (t_DopedIDs[j] == i) t_TotalDopAnz += (size_t)t_DopandAnz[j];
			}
			if (t_TotalDopAnz == 0) continue;

			// Positionen ermitteln
			ErrorCode = UniqueSampling(t_TotalDopAnz, t_TotalPosAnz, &t_SelectedPositions);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "Critical Error: Invalid dopand counts (TSimulationBase::CreateRandomLattice)" << endl << endl;
				return ErrorCode;
			}

			// Fuer jede Dotierung mit DopedID = i zufaellige Positionen aus t_SelectedPositions besetzen
			for (int j = 0; j < (int)t_DopedIDs.size(); j++)
			{
				if ((t_DopedIDs[j] == i) && (t_DopandAnz[j] != 0))
				{

					// Positionen zufaellig aus t_SelectedPositions waehlen
					t_SelDopPos.clear();
					for (size_t k = 0; k < (size_t)t_DopandAnz[j]; k++)
					{
						t_SelDist = uniform_int_distribution<size_t>(0, size_t(t_SelectedPositions.size()) - 1);
						t_SelPos = t_SelDist(spRNG);
						t_SelDopPos.push_back(t_SelectedPositions[t_SelPos]);
						t_SelectedPositions[t_SelPos] = t_SelectedPositions.back();
						t_SelectedPositions.pop_back();
					}

					// Dopanden platzieren
					for (size_t k = 0; k < (size_t)t_SelDopPos.size(); k++)
					{

						t_SelPos = t_SelDopPos[k] / size_t(t_UCElemCount[i]);		// Anzahl an ganzen Elementarzellen vor der Position
						t_x = t_SelPos / (size_t(spLatticeSize) * size_t(spLatticeSize));
						t_SelPos -= t_x * size_t(spLatticeSize) * size_t(spLatticeSize);
						t_y = t_SelPos / size_t(spLatticeSize);
						t_SelPos -= t_y * size_t(spLatticeSize);
						t_z = t_SelPos;
						t_SelPos = t_SelDopPos[k] - ((t_x * size_t(spLatticeSize) + t_y) * size_t(spLatticeSize) + t_z) * size_t(t_UCElemCount[i]);
						t_s = 0;
						t_found = false;
						for (int l = 0; l < (int)t_UCElemIDs.size(); l++)
						{
							if (t_UCElemIDs[l] == i)
							{
								if (t_SelPos == 0)
								{
									t_s = size_t(l);
									t_found = true;
									break;
								}
								else
								{
									t_SelPos--;
								}
							}
						}
						if (t_found == false)
						{
							cout << "Critical Error: Invalid stack position (TSimulationBase::CreateRandomLattice)" << endl << endl;
							return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
						}
						t_lattice[t_x][t_y][t_z][t_s] = t_DopandIDs[j];
					}
				}
			}
		}
	}

	// Leerstellen berechnen
	t_TotalDopAnz = size_t(spTotalVacAnz);
	t_TotalPosAnz = size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spMovStackSize);

	// Positionen ermitteln
	ErrorCode = UniqueSampling(t_TotalDopAnz, t_TotalPosAnz, &t_SelectedPositions);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Invalid vacancy counts (TSimulationBase::CreateRandomLattice)" << endl << endl;
		return ErrorCode;
	}

	// Leerstellen platzieren
	for (size_t i = 0LL; i < (size_t)t_SelectedPositions.size(); i++)
	{

		t_SelPos = t_SelectedPositions[i] / size_t(spMovStackSize);
		t_x = t_SelPos / (size_t(spLatticeSize) * size_t(spLatticeSize));
		t_SelPos -= t_x * size_t(spLatticeSize) * size_t(spLatticeSize);
		t_y = t_SelPos / size_t(spLatticeSize);
		t_SelPos -= t_y * size_t(spLatticeSize);
		t_z = t_SelPos;
		t_SelPos = t_SelectedPositions[i] - ((t_x * size_t(spLatticeSize) + t_y) * size_t(spLatticeSize) + t_z) * size_t(spMovStackSize);
		t_s = 0;
		t_found = false;
		for (int j = 0; j < (int)t_UCElemIDs.size(); j++)
		{
			if (t_UCElemIDs[j] == 0)
			{
				if (t_SelPos == 0)
				{
					t_s = size_t(j);
					t_found = true;
					break;
				}
				else
				{
					t_SelPos--;
				}
			}
		}
		if (t_found == false)
		{
			cout << "Critical Error: Invalid vacancy stack position (TSimulationBase::CreateRandomLattice)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		t_lattice[t_x][t_y][t_z][t_s] = 1;
	}

	// Gitter uebertragen
	*o_lattice = t_lattice;

	return KMCERR_OK;
}

// Leeres Gitter zur Bewegungsspeicherung erstellen
int TSimulationBase::CreateMovLattice(vector<vector<vector<vector<T3DMovVector>>>>* o_movlattice) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::CreateMovLattice)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Leeres Gitter erstellen
	vector<vector<vector<vector<T3DMovVector>>>> t_movlattice(spLatticeSize, vector<vector<vector<T3DMovVector>>>(spLatticeSize,
		vector<vector<T3DMovVector>>(spLatticeSize, vector<T3DMovVector>(spMovStackSize, T3DMovVector()))));

	// Gitter zuweisen
	*o_movlattice = t_movlattice;

	return KMCERR_OK;
}

// Gitterstatistik ausgeben
int TSimulationBase::LatticeAnalysis(string i_space, const vector<vector<vector<vector<int>>>>* i_lattice) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::LatticeAnalysis)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	// Elementanzahl ermitteln
	int t_ElemCount = 0;
	ErrorCode = m_Job->m_Elements->GetElementCount(t_ElemCount);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "Critical Error: Cannot retrieve element count (TSimulationBase::LatticeAnalysis)" << endl << endl;
		return ErrorCode;
	}

	// Elemente auszaehlen
	vector<long long> t_AtomCount(t_ElemCount, 0LL);
	if (i_lattice->size() != 0)
	{
		for (int x = 0; x < (int)i_lattice->size(); x++)
		{
			if (i_lattice->at(x).size() != 0)
			{
				for (int y = 0; y < (int)i_lattice->at(x).size(); y++)
				{
					if (i_lattice->at(x)[y].size() != 0)
					{
						for (int z = 0; z < (int)i_lattice->at(x)[y].size(); z++)
						{
							if (i_lattice->at(x)[y][z].size() != 0)
							{
								for (int s = 0; s < (int)i_lattice->at(x)[y][z].size(); s++)
								{

									// Auf gueltige ElemID pruefen
									if ((i_lattice->at(x)[y][z][s] < 0) || (i_lattice->at(x)[y][z][s] >= t_ElemCount))
									{
										cout << "Critical Error: Invalid ElemID in the lattice (TSimulationBase::LatticeAnalysis)" << endl << endl;
										return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
									}

									// Zaehler erhoehen
									t_AtomCount[i_lattice->at(x)[y][z][s]] += 1LL;
								}
							}
						}
					}
				}
			}
		}
	}

	// Anzahlen ausgeben
	string t_symbol = "";
	string t_name = "";
	double t_charge = 0.0;
	long long t_total = 0LL;
	for (int i = 0; i < (int)t_AtomCount.size(); i++)
	{

		ErrorCode = m_Job->m_Elements->GetElement(i, t_symbol, t_name, t_charge);
		if (ErrorCode != KMCERR_OK) return ErrorCode;

		if (t_name != "")
		{
			cout << i_space << t_symbol << " [" << t_name << ", " << t_charge << "]: " << t_AtomCount[i] << endl;
		}
		else
		{
			cout << i_space << t_symbol << " [" << t_charge << "]: " << t_AtomCount[i] << endl;
		}
		t_total += t_AtomCount[i];
	}
	cout << i_space << "Total: " << t_total << endl;

	return KMCERR_OK;
}

// Statistik aller aktuellen Sprungwahrscheinlichkeiten ausgeben (ohne Normierung)
int TSimulationBase::LatticeProbabilitiesAnalysis(string i_space, const vector<vector<vector<vector<int>>>>* i_lattice) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::LatticeProbabilitiesAnalysis)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Remark: This analysis cannot be used to evaluate all probabilities that could occur in a simulation
	// because the jump environments typically change because they also contain moving species positions
	// -> Therefore, this analysis just reflects the momentary state of the lattice (and it cannot be used for normalization)
	
	// Initialize analysis results (see further below for their meaning)
	unsigned long long blocked_jumps = 0;
	unsigned long long normal_jumps = 0;
	unsigned long long nonsense_jumps = 0;
	unsigned long long overkill_jumps = 0;
	double lowest_prob = 1.0;
	double highest_prob = 0.0;

	// Unit cell positions (index s) are sorted by ElemID -> Iterate over all 4D positions of the moving species
	for (int x_ini = 0; x_ini < spLatticeSize; ++x_ini)
	{
		for (int y_ini = 0; y_ini < spLatticeSize; ++y_ini)
		{
			for (int z_ini = 0; z_ini < spLatticeSize; ++z_ini)
			{
				for (int s_ini = 0; s_ini < spMovStackSize; ++s_ini)
				{
					// Skip if no vacancy at jump start position
					if (i_lattice->at(x_ini)[y_ini][z_ini][s_ini] != 1) continue;

					// Iterate over all possible directions dir
					for (int dir = 0; dir < spDirCount[spDirLink[s_ini]]; ++dir)
					{
						// Pointer to the respective jump object
						const TSimJump* analyzed_jump = spJumps[s_ini] + dir;

						// Get destination position
						int x_end = x_ini + analyzed_jump->destination.x;
						int y_end = y_ini + analyzed_jump->destination.y;
						int z_end = z_ini + analyzed_jump->destination.z;
						int s_end = s_ini + analyzed_jump->destination.s;
						while (x_end < 0) x_end += spLatticeSize;
						while (x_end >= spLatticeSize) x_end -= spLatticeSize;
						while (y_end < 0) y_end += spLatticeSize;
						while (y_end >= spLatticeSize) y_end -= spLatticeSize;
						while (z_end < 0) z_end += spLatticeSize;
						while (z_end >= spLatticeSize) z_end -= spLatticeSize;
						while (s_end < 0) s_end += spStackSize;
						while (s_end >= spStackSize) s_end -= spStackSize;

						// Skip if vacancy at jump end position
						if (i_lattice->at(x_end)[y_end][z_end][s_end] == 1) 
						{
							blocked_jumps++;
							continue;
						}

						// Probability for forward jump (initialized to E-field contribution)
						double jump_prob = analyzed_jump->efield_contrib;

						// Get unique jump
						const TSimUniqueJump* analyzed_unique_jump = analyzed_jump->unique_jump;

						// Calculate environment hash
						size_t hash_idx = 0;
						for (int i_env = 0; i_env < analyzed_jump->code_envpos_size; ++i_env)
						{
							T4DLatticeVector* env_pos = analyzed_jump->code_envpos + i_env;
							int x_env = x_ini + env_pos->x;
							int y_env = y_ini + env_pos->y;
							int z_env = z_ini + env_pos->z;
							int s_env = s_ini + env_pos->s;
							while (x_env < 0) x_env += spLatticeSize;
							while (x_env >= spLatticeSize) x_env -= spLatticeSize;
							while (y_env < 0) y_env += spLatticeSize;
							while (y_env >= spLatticeSize) y_env -= spLatticeSize;
							while (z_env < 0) z_env += spLatticeSize;
							while (z_env >= spLatticeSize) z_env -= spLatticeSize;
							while (s_env < 0) s_env += spStackSize;
							while (s_env >= spStackSize) s_env -= spStackSize;
							hash_idx += analyzed_unique_jump->hash_mult[i_env] *
								analyzed_unique_jump->hash_map[i_env][i_lattice->at(x_env)[y_env][z_env][s_env]];
						}

						// Add contribution from non-additive environment
						if (analyzed_jump->code_envpos_size > 0)
						{
							jump_prob *= analyzed_unique_jump->code_energies[hash_idx];
						}

						// Add additive contributions
						for (int i_env = 0; i_env < analyzed_jump->add_envpos_size; ++i_env)
						{
							T4DLatticeVector* env_pos = analyzed_jump->add_envpos + i_env;
							int x_env = x_ini + env_pos->x;
							int y_env = y_ini + env_pos->y;
							int z_env = z_ini + env_pos->z;
							int s_env = s_ini + env_pos->s;
							while (x_env < 0) x_env += spLatticeSize;
							while (x_env >= spLatticeSize) x_env -= spLatticeSize;
							while (y_env < 0) y_env += spLatticeSize;
							while (y_env >= spLatticeSize) y_env -= spLatticeSize;
							while (z_env < 0) z_env += spLatticeSize;
							while (z_env >= spLatticeSize) z_env -= spLatticeSize;
							while (s_env < 0) s_env += spStackSize;
							while (s_env >= spStackSize) s_env -= spStackSize;

							jump_prob *= analyzed_unique_jump->add_energies[i_env][i_lattice->at(x_env)[y_env][z_env][s_env]];
						}
						
						// Pointer to the respective reverse jump object
						const TSimJump* analyzed_backjump = analyzed_jump->back_jump;

						// Probability for forward jump (initialized to E-field contribution)
						double backjump_prob = analyzed_backjump->efield_contrib;

						// Get unique reverse jump
						const TSimUniqueJump* analyzed_unique_backjump = analyzed_backjump->unique_jump;

						// Calculate environment hash
						hash_idx = 0;
						for (int i_env = 0; i_env < analyzed_backjump->code_envpos_size; ++i_env)
						{
							T4DLatticeVector* env_pos = analyzed_backjump->code_envpos + i_env;
							int x_env = x_end + env_pos->x;
							int y_env = y_end + env_pos->y;
							int z_env = z_end + env_pos->z;
							int s_env = s_end + env_pos->s;
							while (x_env < 0) x_env += spLatticeSize;
							while (x_env >= spLatticeSize) x_env -= spLatticeSize;
							while (y_env < 0) y_env += spLatticeSize;
							while (y_env >= spLatticeSize) y_env -= spLatticeSize;
							while (z_env < 0) z_env += spLatticeSize;
							while (z_env >= spLatticeSize) z_env -= spLatticeSize;
							while (s_env < 0) s_env += spStackSize;
							while (s_env >= spStackSize) s_env -= spStackSize;
							hash_idx += analyzed_unique_backjump->hash_mult[i_env] *
								analyzed_unique_backjump->hash_map[i_env][i_lattice->at(x_env)[y_env][z_env][s_env]];
						}

						// Add contribution from non-additive environment
						if (analyzed_backjump->code_envpos_size > 0)
						{
							backjump_prob *= analyzed_unique_backjump->code_energies[hash_idx];
						}
						
						// Add additive contributions
						for (int i_env = 0; i_env < analyzed_backjump->add_envpos_size; ++i_env)
						{
							T4DLatticeVector* env_pos = analyzed_backjump->add_envpos + i_env;
							int x_env = x_end + env_pos->x;
							int y_env = y_end + env_pos->y;
							int z_env = z_end + env_pos->z;
							int s_env = s_end + env_pos->s;
							while (x_env < 0) x_env += spLatticeSize;
							while (x_env >= spLatticeSize) x_env -= spLatticeSize;
							while (y_env < 0) y_env += spLatticeSize;
							while (y_env >= spLatticeSize) y_env -= spLatticeSize;
							while (z_env < 0) z_env += spLatticeSize;
							while (z_env >= spLatticeSize) z_env -= spLatticeSize;
							while (s_env < 0) s_env += spStackSize;
							while (s_env >= spStackSize) s_env -= spStackSize;

							backjump_prob *= analyzed_unique_backjump->add_energies[i_env][i_lattice->at(x_env)[y_env][z_env][s_env]];
						}

						// Analyze jump category
						// Explanation: a lattice position is undefined, if a jump away from it has a probability >= 1.0
						//   (i.e. activation energy <= 0 eV, actual migration energy without normalization)
						// 
						// Category 1: Start defined + End defined 
						// -> counts as jump attempt, counts as Monte-Carlo step if jump happens
						// 
						// Category 2: Start defined + End not defined (= "nonsense attempt")
						// -> counts as jump attempt, but jump always declined
						// 
						// Category 3: Start not defined (= "overkill attempt") (independent of whether end position defined or not)
						// -> counts NOT as jump attempt, jump always occurs but does not count as Monte-Carlo step
						//
						if (jump_prob < 1.0)
						{
							if (backjump_prob < 1.0)
							{
								// -> Category 1
								normal_jumps++;
							}
							else
							{
								// -> Category 2
								nonsense_jumps++;
							}
						}
						else
						{
							// -> Category 3
							overkill_jumps++;
						}
						
						// Analyze jump probability 
						// (only category 1 = normal jumps that can be a Monte-Carlo step)
						if ((jump_prob < 1.0) && (backjump_prob < 1.0))
						{
							if (jump_prob < lowest_prob) lowest_prob = jump_prob;
							if (jump_prob > highest_prob) highest_prob = jump_prob;
						}
					}
				}
			}
		}
	}
	
	// Print statistics
	cout << i_space << "Normal jumps: " << normal_jumps << endl;
	cout << i_space << "Blocked (vacancy on destination): " << blocked_jumps << endl;
	cout << i_space << "Invalid start position (jump probability >= 1.0): " << overkill_jumps << endl;
	cout << i_space << "Invalid end position (backjump probability >= 1.0): " << nonsense_jumps << endl;
	if (normal_jumps > 0)
	{
		cout << i_space << "Min/max jump probability of all normal jumps: [" << lowest_prob << ", " << highest_prob << "] (without normalization)" << endl;
	}

	// Signal if no normal jumps in this lattice
	if (normal_jumps == 0) return KMCERR_INVALID_INPUT;

	return KMCERR_OK;
}

// Simulationsphase auswerten
int TSimulationBase::PhaseAnalysis(string i_space, bool is_short, const TSimPhaseInfo& i_phase, bool show_probs) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::PhaseAnalysis)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (i_phase.HasValidData == false) return KMCERR_OK;

	int ErrorCode = KMCERR_OK;

	// Simulationsstand ausgeben
	cout << i_space << "MCSP: " << i_phase.CurrentMCSP << " (" << i_phase.CurrentMCSP * spTotalMovAnz << " MCS)" << endl;
	cout << i_space << "Jump attempts: " << i_phase.CurrentJumpAttempts << endl;
	if (is_short == false)
	{
		cout << i_space << "Site blocking: " << i_phase.CurrentSiteBlockingCounter << endl;
		cout << i_space << "Jump probability >= 1.0: " << i_phase.CurrentOverkillJumpAttempts << endl;
		cout << i_space << "Backjump probability >= 1.0: " << i_phase.CurrentNonsenseJumpAttempts << endl;
		cout << i_space << "Runtime: " << i_phase.CurrentRunTime.ToString() << endl;
		cout << i_space << "Simulated timespan: " << i_phase.CurrentAttemptPathRatioSum * i_phase.UsedNorm / spFrequency << " s" << endl;
	}

	if (i_phase.CurrentJumpAttempts != 0ULL)
	{
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
		ErrorCode = MovLatticeAnalysis(&(i_phase.CurrentLattice), &(i_phase.CurrentMovLattice), &t_MovAnalysis);
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

		// Leitfaehigkeit ausgeben (sigma = spCondFactor * spTimeFactor * Projection((<x>, <y>, <z>), E-Feld-Vektor) / (Sprungversuche * Normierung))
		if (spCanCalcCond == true)
		{
			double t_conductivity = spCondFactor * spFrequency * t_mov_comp_parallel / (i_phase.CurrentAttemptPathRatioSum * i_phase.UsedNorm);
			cout << i_space << "Conductivity: " << t_conductivity << " S/cm" << endl;
		}

		// Analysedaten ausgeben
		// Zusaetzlich:
		// sqrt(<x>*<x> + <y>*<y> + <z>*<z>) = Laenge des mittleren Verschiebungsvektors
		// sqrt(sum(x)*sum(x) + sum(y)*sum(y) + sum(z)*sum(z)) = Laenge des Schwerpunktverschiebungsvektors
		if (is_short == false)
		{
			cout << i_space << "Moving Species (<> = mean = average over all atoms of the moving species):" << endl;
			cout << i_space << "  Mean jump count: " << t_MovAnalysis[0] << endl;
			cout << i_space << "  Mean displacement vector (<x>, <y>, <z>): (" << t_MovAnalysis[1] << " , " << t_MovAnalysis[2] << " , ";
			cout << t_MovAnalysis[3] << ") cm" << endl;
			cout << i_space << "  Length of the mean disp. vector (sqrt(<x>^2 + <y>^2 + <z>^2)): " << sqrt(t_MovAnalysis[1] * t_MovAnalysis[1] +
				t_MovAnalysis[2] * t_MovAnalysis[2] + t_MovAnalysis[3] * t_MovAnalysis[3]) << " cm" << endl;
			if (spCanCalcCond == true)
			{
				cout << i_space << "  Component of mean disp. vector parallel to E-field (Projection): " << t_mov_comp_parallel << " cm" << endl;
				cout << i_space << "  Component of mean disp. vector perpendicular to E-field (Rejection): " << t_mov_comp_perpendicular << " cm" << endl;
			}
			cout << i_space << "  Mean displacement (<sqrt(x^2 + y^2 + z^2)>): " << t_MovAnalysis[4] << " cm" << endl;
			cout << i_space << "  Mean squared displacement (<x^2 + y^2 + z^2>): " << t_MovAnalysis[5] << " cm^2" << endl;
			cout << i_space << "  Center of mass displacement vector (sum(x), sum(y), sum(z)): (" << t_MovAnalysis[6] << " , " << t_MovAnalysis[7] << " , ";
			cout << t_MovAnalysis[8] << ") cm" << endl;
			cout << i_space << "  Length of the center of mass disp. vector (sqrt(sum(x)^2 + sum(y)^2 + sum(z)^2)): " << sqrt(t_MovAnalysis[6] * t_MovAnalysis[6] +
				t_MovAnalysis[7] * t_MovAnalysis[7] + t_MovAnalysis[8] * t_MovAnalysis[8]) << " cm" << endl;

			cout << i_space << "Vacancies (<> = mean = average over all vacancies):" << endl;
			cout << i_space << "  Mean jump count: " << t_MovAnalysis[9] << endl;
			cout << i_space << "  Mean displacement vector (<x>, <y>, <z>): (" << t_MovAnalysis[10] << " , " << t_MovAnalysis[11] << " , ";
			cout << t_MovAnalysis[12] << ") cm" << endl;
			cout << i_space << "  Length of the mean disp. vector (sqrt(<x>^2 + <y>^2 + <z>^2)): " << sqrt(t_MovAnalysis[10] * t_MovAnalysis[10] +
				t_MovAnalysis[11] * t_MovAnalysis[11] + t_MovAnalysis[12] * t_MovAnalysis[12]) << " cm" << endl;
			if (spCanCalcCond == true)
			{
				cout << i_space << "  Component of mean disp. vector parallel to E-field (Projection): " << t_vac_comp_parallel << " cm" << endl;
				cout << i_space << "  Component of mean disp. vector perpendicular to E-field (Rejection): " << t_vac_comp_perpendicular << " cm" << endl;
			}
			cout << i_space << "  Mean displacement (<sqrt(x^2 + y^2 + z^2)>): " << t_MovAnalysis[13] << " cm" << endl;
			cout << i_space << "  Mean squared displacement (<x^2 + y^2 + z^2>): " << t_MovAnalysis[14] << " cm^2" << endl;
			cout << i_space << "  Center of mass displacement vector (sum(x), sum(y), sum(z)): (" << t_MovAnalysis[15] << " , " << t_MovAnalysis[16] << " , ";
			cout << t_MovAnalysis[17] << ") cm" << endl;
			cout << i_space << "  Length of the center of mass disp. vector (sqrt(sum(x)^2 + sum(y)^2 + sum(z)^2)): " << sqrt(t_MovAnalysis[15] * t_MovAnalysis[15] +
				t_MovAnalysis[16] * t_MovAnalysis[16] + t_MovAnalysis[17] * t_MovAnalysis[17]) << " cm" << endl;
		}
		else
		{
			if (spCanCalcCond == true)
			{
				cout << i_space << "Component of mean mov. species disp. vector parallel to E-field (Projection): " << t_mov_comp_parallel << " cm" << endl;
				cout << i_space << "Component of mean mov. species disp. vector perpendicular to E-field (Rejection): " << t_mov_comp_perpendicular << " cm" << endl;
			}
			else
			{
				cout << i_space << "Mean mov. species displacement vector (<x>, <y>, <z>): (" << t_MovAnalysis[1] << " , " << t_MovAnalysis[2] << " , ";
				cout << t_MovAnalysis[3] << ") cm" << endl;
			}
			cout << i_space << "Mean squared mov. species displacement (<x^2 + y^2 + z^2>): " << t_MovAnalysis[5] << " cm^2" << endl;
		}

		// Sprungwahrscheinlichkeiten ausgeben
		if (show_probs == true)
		{
			// Wahrscheinlichkeiten der Sprungversuche
			if (i_phase.CurrentAttemptProbList.size() != 0)
			{
				if (i_phase.CurrentAttemptProbList[0].prob != 0.0)
				{
					cout << i_space << "Jump attempt probability list:" << endl;
					unsigned long long t_countsum = 0ULL;
					for (int i = 0; i < (int)i_phase.CurrentAttemptProbList.size(); i++)
					{
						if (i_phase.CurrentAttemptProbList[i].prob == 0.0) break;
						t_countsum += i_phase.CurrentAttemptProbList[i].count;
						cout << i_space << "  " << i + 1 << ". Prob.: " << i_phase.CurrentAttemptProbList[i].prob << " -> Count: ";
						cout << i_phase.CurrentAttemptProbList[i].count << " (";
						cout << double(i_phase.CurrentAttemptProbList[i].count) / double(i_phase.CurrentJumpAttempts) * 100.0 << " %)" << endl;
					}
					cout << i_space << "  Sum of counts: " << t_countsum << " (" << double(t_countsum) / double(i_phase.CurrentJumpAttempts) * 100.0 << " %)" << endl;
					cout << i_space << "  -> " << 100.0 - double(t_countsum) / double(i_phase.CurrentJumpAttempts) * 100.0 << " % lower probabilities." << endl;
				}
				else
				{
					cout << i_space << "No jump attempt probabilities recorded." << endl;
				}
			}
			else
			{
				cout << i_space << "No jump attempt probabilities recorded." << endl;
			}

			// Wahrscheinlichkeiten der Monte-Carlo-steps
			if (i_phase.CurrentAcceptedProbList.size() != 0)
			{
				if (i_phase.CurrentAcceptedProbList[0].prob != 0.0)
				{
					cout << i_space << "Monte-Carlo step probability list:" << endl;
					unsigned long long t_countsum = 0ULL;
					for (int i = 0; i < (int)i_phase.CurrentAcceptedProbList.size(); i++)
					{
						if (i_phase.CurrentAcceptedProbList[i].prob == 0.0) break;
						t_countsum += i_phase.CurrentAcceptedProbList[i].count;
						cout << i_space << "  " << i + 1 << ". Prob.: " << i_phase.CurrentAcceptedProbList[i].prob << " -> Count: ";
						cout << i_phase.CurrentAcceptedProbList[i].count << " (";
						cout << double(i_phase.CurrentAcceptedProbList[i].count) / double(i_phase.CurrentJumpAttempts) * 100.0 << " %)" << endl;
					}
					cout << i_space << "  Sum of counts: " << t_countsum << " (" << double(t_countsum) / double(i_phase.CurrentJumpAttempts) * 100.0 << " %)" << endl;
					cout << i_space << "  -> " << 100.0 - double(t_countsum) / double(i_phase.CurrentJumpAttempts) * 100.0 << " % lower probabilities." << endl;
				}
				else
				{
					cout << i_space << "No Monte-Carlo step probabilities recorded." << endl;
				}
			}
			else
			{
				cout << i_space << "No Monte-Carlo step probabilities recorded." << endl;
			}
		}
	}

	// Lattice probability analysis
	if (is_short == false)
	{
		cout << i_space << "Analysis of the possible jumps in this moment:" << endl;
		ErrorCode = LatticeProbabilitiesAnalysis(i_space + "  ", &(i_phase.CurrentLattice));
		if (ErrorCode != KMCERR_OK)
		{
			if (ErrorCode == KMCERR_INVALID_INPUT)
			{
				cout << i_space << "Error: No normal jumps possible. Aborting simulation ..." << endl;
				cout << i_space << "Likely reason: The specified activation energies are all zero or too small." << endl;

				// Remark: In very rare cases, this criterion could prevent valid simulations
				// (at the moment no normal jumps are possible but in principle this could change due to the overkill jumps)
			}
			return ErrorCode;
		}
	}

	return KMCERR_OK;
}

// id_count Zahlen aus dem Interval [0, pos_count-1] zufaellig auswaehlen (keine Zahl doppelt, in aufsteigender Ordnung)
int TSimulationBase::UniqueSampling(size_t id_count, size_t pos_count, vector<size_t>* o_selected)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	if ((id_count < 1) || (pos_count < 1) || (id_count > pos_count)) return KMCERR_INVALID_INPUT_CRIT;

	// Der folgende Algorithmus ist dem Buch "The Art of Computer Programming Vol.2 - Seminumerical Algorithms" 
	// von Donald E. Knuth (3. Ausgabe, 1997) entnommen. Er ist in Kapitel 3.4.2 (Seite 142) als "Algorithmus S" beschrieben.
	// Im Anhang dieses Buchs erfolgt auch der Beweis durch Induktion, dass bei dieser Methode fuer jede Position
	// die gleiche Wahrscheinlichkeit (id_count/(max_id+1)) besteht, ausgewaehlt zu werden.

	size_t restpos;					// Anzahl uebriger Positionsnummern
	size_t resttodraw;				// Anzahl noch zu wuerfelnder Positionen
	o_selected->clear();
	for (size_t posnr = 0; (posnr < pos_count) && (size_t(o_selected->size()) < id_count); ++posnr)
	{
		restpos = pos_count - posnr;
		resttodraw = id_count - size_t(o_selected->size());
		if (spProbDist(spRNG) < double(resttodraw) / double(restpos)) o_selected->push_back(posnr);		// Position mit Wahrscheinlichkeit resttodraw/restpos auswaehlen
	}
	if ((size_t)o_selected->size() != id_count) return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;

	return KMCERR_OK;
}

// Synchronisation: i_phase -> variable Simulationsparameter
int TSimulationBase::SetSimulationData(const TSimPhaseInfo& i_phase)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (i_phase.HasValidData == false)
	{
		cout << "Critical Error: Invalid phase data (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Zaehler setzen
	spMCSP = i_phase.CurrentMCSP;
	spJumpAttempts = i_phase.CurrentJumpAttempts;
	spNonsenseAttempts = i_phase.CurrentNonsenseJumpAttempts;
	spOverkillAttempts = i_phase.CurrentOverkillJumpAttempts;
	spSiteBlockingCounter = i_phase.CurrentSiteBlockingCounter;

	size_t t_size = 0;

	// Linearisiertes Gitter erstellen
	t_size = size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spStackSize);
	delete[] spLattice;
	spLattice = new (nothrow) int[t_size];
	if (spLattice == NULL)
	{
		cout << "Critical Error: Cannot create lattice array (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}
	for (int x = 0; x < spLatticeSize; x++)
	{
		for (int y = 0; y < spLatticeSize; y++)
		{
			for (int z = 0; z < spLatticeSize; z++)
			{
				for (int s = 0; s < spStackSize; s++)
				{
					spLattice[(((size_t)x * spLatticeSize + y) * spLatticeSize + z) * spStackSize + s] = i_phase.CurrentLattice[x][y][z][s];
				}
			}
		}
	}

	// Linearisiertes Gitter zur Bewegungsspeicherung erstellen
	t_size = size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spMovStackSize);
	delete[] spMovLattice;
	spMovLattice = new (nothrow) T3DMovVector[t_size];
	if (spMovLattice == NULL)
	{
		cout << "Critical Error: Cannot create movement detection array (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}
	for (int x = 0; x < spLatticeSize; x++)
	{
		for (int y = 0; y < spLatticeSize; y++)
		{
			for (int z = 0; z < spLatticeSize; z++)
			{
				for (int s = 0; s < spMovStackSize; s++)
				{
					spMovLattice[(((size_t)x * spLatticeSize + y) * spLatticeSize + z) * spMovStackSize + s] = i_phase.CurrentMovLattice[x][y][z][s];
				}
			}
		}
	}

	// Liste der Leerstellenpositionen erstellen
	delete[] spVacListSizes;
	spVacListSizes = new (nothrow) unsigned long long[size_t(spDirSelCount)];
	if (spVacListSizes == NULL)
	{
		cout << "Critical Error: Cannot create vacancy position counters (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}
	for (int i = 0; i < spDirSelCount; i++)
	{
		spVacListSizes[i] = 0ULL;
	}

	if (spVacLists != NULL)
	{
		for (int i = spDirSelCount - 1; i >= 0; i--)
		{
			delete[] spVacLists[i];
		}
		delete[] spVacLists;
	}
	spVacLists = new (nothrow) T4DLatticeVector * [size_t(spDirSelCount)];
	if (spVacLists == NULL)
	{
		cout << "Critical Error: Cannot create vacancy position array (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}

	vector<int> t_atomcount(size_t(spDirSelCount), 0);
	for (int i = 0; i < spMovStackSize; i++)
	{
		t_atomcount[spDirLink[i]] += 1;
	}
	for (int i = 0; i < spDirSelCount; i++)
	{
		if (t_atomcount[i] == 0)
		{
			cout << "Critical Error: Inconsistent direction counts (TSimulationBase::SetSimulationData)" << endl << endl;
			if (i > 0)
			{
				for (int j = i - 1; j >= 0; j--)
				{
					delete[] spVacLists[j];
				}
			}
			delete[] spVacLists;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		size_t t_maxvac = size_t(t_atomcount[i]) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize);
		if (t_maxvac > spTotalVacAnz) t_maxvac = size_t(spTotalVacAnz);

		spVacLists[i] = new (nothrow) T4DLatticeVector[t_maxvac];
		if (spVacLists[i] == NULL)
		{
			cout << "Critical Error: Cannot create vacancy position sub-array (TSimulationBase::SetSimulationData)" << endl << endl;
			for (int j = i; j >= 0; j--)
			{
				delete[] spVacLists[j];
			}
			delete[] spVacLists;
			return KMCERR_MAXIMUM_INPUT_REACHED;
		}
	}

	for (int x = 0; x < spLatticeSize; x++)
	{
		for (int y = 0; y < spLatticeSize; y++)
		{
			for (int z = 0; z < spLatticeSize; z++)
			{
				for (int s = 0; s < spMovStackSize; s++)
				{
					if (spLattice[(((size_t)x * spLatticeSize + y) * spLatticeSize + z) * spStackSize + s] == 1)
					{
						spVacLists[spDirLink[s]][spVacListSizes[spDirLink[s]]] = T4DLatticeVector(x, y, z, s);
						spVacListSizes[spDirLink[s]] += 1;
					}
				}
			}
		}
	}

	// Leerstellen- und Richtungsauswahl-Verteilung erstellen
	spVacMaxIndex = 0ULL;
	for (int i = 0; i < spDirSelCount; i++)
	{
		spVacListSizes[i] *= spDirCount[i];
		spVacMaxIndex += spVacListSizes[i];
	}
	spVacMaxIndex -= 1;
	spDirDist = uniform_int_distribution<unsigned long long>(0, spVacMaxIndex);

	// Wahrscheinlichkeitslisten erstellen
	spProbListSize = (int)i_phase.CurrentAttemptProbList.size();
	if (spProbListSize != (int)i_phase.CurrentAcceptedProbList.size())
	{
		cout << "Critical Error: Inconsistent probability vectors (TSimulationBase::SetSimulationData)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	delete[] spAttemptProbList;
	spAttemptProbList = NULL;
	delete[] spAcceptedProbList;
	spAcceptedProbList = NULL;
	if (spProbListSize > 0)
	{

		spAttemptProbList = new (nothrow) TSimProb[spProbListSize];
		if (spAttemptProbList == NULL)
		{
			cout << "Critical Error: Cannot create attempt probability array (TSimulationBase::SetSimulationData)" << endl << endl;
			return KMCERR_MAXIMUM_INPUT_REACHED;
		}

		spAcceptedProbList = new (nothrow) TSimProb[spProbListSize];
		if (spAcceptedProbList == NULL)
		{
			cout << "Critical Error: Cannot create accepted probability array (TSimulationBase::SetSimulationData)" << endl << endl;
			return KMCERR_MAXIMUM_INPUT_REACHED;
		}

		for (int i = 0; i < spProbListSize; i++)
		{
			spAttemptProbList[i] = i_phase.CurrentAttemptProbList[i];
			spAcceptedProbList[i] = i_phase.CurrentAcceptedProbList[i];
		}
	}

	// Laufzeit setzen
	spPreviousRunTime = i_phase.CurrentRunTime;

	// Summe der Versuch-Pfad-Quotienten setzen
	spAttemptPathRatioSum = i_phase.CurrentAttemptPathRatioSum;

	return KMCERR_OK;
}

// Synchronisation: variable Simulationsparameter -> o_phase
int TSimulationBase::GetSimulationData(TSimPhaseInfo& o_phase) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::GetSimulationData)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Daten ueberschreiben
	o_phase.HasValidData = false;
	o_phase.UsedNorm = spNormalization;
	o_phase.CurrentAttemptPathRatioSum = spAttemptPathRatioSum;
	o_phase.CurrentMCSP = spMCSP;
	o_phase.CurrentJumpAttempts = spJumpAttempts;
	o_phase.CurrentNonsenseJumpAttempts = spNonsenseAttempts;
	o_phase.CurrentOverkillJumpAttempts = spOverkillAttempts;
	o_phase.CurrentSiteBlockingCounter = spSiteBlockingCounter;
	o_phase.CurrentRunTime = spRunTime;

	// Gitter uebertragen
	if ((int)o_phase.CurrentLattice.size() != spLatticeSize)
	{
		cout << "Critical Error: Invalid 1st dimension lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	for (int x = 0; x < spLatticeSize; x++)
	{
		if ((int)o_phase.CurrentLattice[x].size() != spLatticeSize)
		{
			cout << "Critical Error: Invalid 2nd dimension lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		for (int y = 0; y < spLatticeSize; y++)
		{
			if ((int)o_phase.CurrentLattice[x][y].size() != spLatticeSize)
			{
				cout << "Critical Error: Invalid 3rd dimension lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
				return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
			}
			for (int z = 0; z < spLatticeSize; z++)
			{
				if ((int)o_phase.CurrentLattice[x][y][z].size() != spStackSize)
				{
					cout << "Critical Error: Invalid 4th dimension lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
					return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
				}
				for (int s = 0; s < spStackSize; s++)
				{
					o_phase.CurrentLattice[x][y][z][s] = spLattice[(((size_t)x * spLatticeSize + y) * spLatticeSize + z) * spStackSize + s];
				}
			}
		}
	}

	// Gitter zur Bewegungsspeicherung uebertragen
	if ((int)o_phase.CurrentMovLattice.size() != spLatticeSize)
	{
		cout << "Critical Error: Invalid 1st dimension mov lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	for (int x = 0; x < spLatticeSize; x++)
	{
		if ((int)o_phase.CurrentMovLattice[x].size() != spLatticeSize)
		{
			cout << "Critical Error: Invalid 2nd dimension mov lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		for (int y = 0; y < spLatticeSize; y++)
		{
			if ((int)o_phase.CurrentMovLattice[x][y].size() != spLatticeSize)
			{
				cout << "Critical Error: Invalid 3rd dimension mov lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
				return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
			}
			for (int z = 0; z < spLatticeSize; z++)
			{
				if ((int)o_phase.CurrentMovLattice[x][y][z].size() != spMovStackSize)
				{
					cout << "Critical Error: Invalid 4th dimension mov lattice size (TSimulationBase::GetSimulationData)" << endl << endl;
					return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
				}
				for (int s = 0; s < spMovStackSize; s++)
				{
					o_phase.CurrentMovLattice[x][y][z][s] = spMovLattice[(((size_t)x * spLatticeSize + y) * spLatticeSize + z) * spMovStackSize + s];
				}
			}
		}
	}

	// Wahrscheinlichkeitslisten uebertragen
	if ((int)o_phase.CurrentAttemptProbList.size() != spProbListSize)
	{
		cout << "Critical Error: Invalid attempt prob. list size (TSimulationBase::GetSimulationData)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if ((int)o_phase.CurrentAcceptedProbList.size() != spProbListSize)
	{
		cout << "Critical Error: Invalid accepted prob. list size (TSimulationBase::GetSimulationData)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (spProbListSize > 0)
	{
		for (int i = 0; i < spProbListSize; i++)
		{
			o_phase.CurrentAttemptProbList[i] = spAttemptProbList[i];
			o_phase.CurrentAcceptedProbList[i] = spAcceptedProbList[i];
		}
	}

	o_phase.HasValidData = true;
	return KMCERR_OK;
}

// PrerunPhaseData validieren
int TSimulationBase::ValidatePrerunData(TSimPhaseInfo& i_phase)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::ValidatePrerunData)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;
	i_phase.HasValidData = false;

	// Prerun-Einstellungen ermitteln
	bool DoPrerun = false;
	long long PrerunTargetMCSP = 0LL;
	int PrerunRecordAnz = 0;
	ErrorCode = m_Job->m_Settings->GetPrerunOptions(DoPrerun, PrerunTargetMCSP, PrerunRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Normierung pruefen
	if (i_phase.UsedNorm < 1.0) return KMCERR_INVALID_INPUT;

	// Versuch-Pfad-Quotient pruefen
	if (i_phase.CurrentAttemptPathRatioSum < 0.0) return KMCERR_INVALID_INPUT;

	// Laufzeit pruefen
	i_phase.CurrentRunTime.CheckOverflow();
	if (i_phase.CurrentRunTime.year < 0) return KMCERR_INVALID_INPUT;

	// Gitter pruefen
	ErrorCode = ValidateLattice(&(i_phase.CurrentLattice));
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Bewegungsgitter pruefen
	if ((int)i_phase.CurrentMovLattice.size() != spLatticeSize) return KMCERR_INVALID_INPUT;
	for (int x = 0; x < spLatticeSize; x++)
	{
		if ((int)i_phase.CurrentMovLattice[x].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
		for (int y = 0; y < spLatticeSize; y++)
		{
			if ((int)i_phase.CurrentMovLattice[x][y].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
			for (int z = 0; z < spLatticeSize; z++)
			{
				if ((int)i_phase.CurrentMovLattice[x][y][z].size() != spMovStackSize) return KMCERR_INVALID_INPUT;
			}
		}
	}

	// Wahrscheinlichkeitslisten pruefen
	ErrorCode = ValidateProbLists(i_phase, PrerunRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	i_phase.HasValidData = true;
	return KMCERR_OK;
}

// DynNormPhaseData validieren
int TSimulationBase::ValidateDynNormData(TSimPhaseInfo& i_phase)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::ValidateDynNormData)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;
	i_phase.HasValidData = false;

	// DynNorm-Einstellungen ermitteln
	bool DoDynNorm = false;
	long long DynNormTargetAttempts = 0LL;
	int DynNormRecordAnz = 0;
	int DynNormListEntry = 0;
	ErrorCode = m_Job->m_Settings->GetDynNormParameters(DoDynNorm, DynNormTargetAttempts, DynNormRecordAnz, DynNormListEntry);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Normierung pruefen
	if (i_phase.UsedNorm < 1.0) return KMCERR_INVALID_INPUT;

	// Versuch-Pfad-Quotient pruefen
	if (i_phase.CurrentAttemptPathRatioSum < 0.0) return KMCERR_INVALID_INPUT;

	// Laufzeit pruefen
	i_phase.CurrentRunTime.CheckOverflow();
	if (i_phase.CurrentRunTime.year < 0) return KMCERR_INVALID_INPUT;

	// Gitter pruefen
	ErrorCode = ValidateLattice(&(i_phase.CurrentLattice));
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Bewegungsgitter pruefen
	if ((int)i_phase.CurrentMovLattice.size() != spLatticeSize) return KMCERR_INVALID_INPUT;
	for (int x = 0; x < spLatticeSize; x++)
	{
		if ((int)i_phase.CurrentMovLattice[x].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
		for (int y = 0; y < spLatticeSize; y++)
		{
			if ((int)i_phase.CurrentMovLattice[x][y].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
			for (int z = 0; z < spLatticeSize; z++)
			{
				if ((int)i_phase.CurrentMovLattice[x][y][z].size() != spMovStackSize) return KMCERR_INVALID_INPUT;
			}
		}
	}

	// Wahrscheinlichkeitslisten pruefen
	ErrorCode = ValidateProbLists(i_phase, DynNormRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	i_phase.HasValidData = true;
	return KMCERR_OK;
}

// MainPhaseData validieren
int TSimulationBase::ValidateMainData(TSimPhaseInfo& i_phase)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::ValidateMainData)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;
	i_phase.HasValidData = false;

	// Einstellungen ermitteln
	long long MainTargetMCSP = 0LL;
	int MainRecordAnz = 0;
	ErrorCode = m_Job->m_Settings->GetMainKMCOptions(MainTargetMCSP, MainRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Normierung pruefen
	if (i_phase.UsedNorm < 1.0) return KMCERR_INVALID_INPUT;

	// Versuch-Pfad-Quotient pruefen
	if (i_phase.CurrentAttemptPathRatioSum < 0.0) return KMCERR_INVALID_INPUT;

	// Laufzeit pruefen
	i_phase.CurrentRunTime.CheckOverflow();
	if (i_phase.CurrentRunTime.year < 0) return KMCERR_INVALID_INPUT;

	// Gitter pruefen
	ErrorCode = ValidateLattice(&(i_phase.CurrentLattice));
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Bewegungsgitter pruefen
	if ((int)i_phase.CurrentMovLattice.size() != spLatticeSize) return KMCERR_INVALID_INPUT;
	for (int x = 0; x < spLatticeSize; x++)
	{
		if ((int)i_phase.CurrentMovLattice[x].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
		for (int y = 0; y < spLatticeSize; y++)
		{
			if ((int)i_phase.CurrentMovLattice[x][y].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
			for (int z = 0; z < spLatticeSize; z++)
			{
				if ((int)i_phase.CurrentMovLattice[x][y][z].size() != spMovStackSize) return KMCERR_INVALID_INPUT;
			}
		}
	}

	// Wahrscheinlichkeitslisten pruefen
	ErrorCode = ValidateProbLists(i_phase, MainRecordAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	i_phase.HasValidData = true;
	return KMCERR_OK;
}

// Gitter validieren
int TSimulationBase::ValidateLattice(vector<vector<vector<vector<int>>>>* i_lattice)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::ValidateLattice)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (i_lattice == NULL)
	{
		cout << "Critical Error: Invalid lattice pointer (TSimulationBase::ValidateLattice)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}

	int ErrorCode = KMCERR_OK;

	// Gittergroesse validieren
	if ((int)i_lattice->size() != spLatticeSize) return KMCERR_INVALID_INPUT;
	for (int x = 0; x < spLatticeSize; x++)
	{
		if ((int)i_lattice->at(x).size() != spLatticeSize) return KMCERR_INVALID_INPUT;
		for (int y = 0; y < spLatticeSize; y++)
		{
			if ((int)i_lattice->at(x)[y].size() != spLatticeSize) return KMCERR_INVALID_INPUT;
			for (int z = 0; z < spLatticeSize; z++)
			{
				if ((int)i_lattice->at(x)[y][z].size() != spStackSize) return KMCERR_INVALID_INPUT;
			}
		}
	}

	// ElemIDs in der undotierten Elementarzelle ermitteln
	vector<int> t_UCElemID;
	ErrorCode = m_Job->m_Structure->GetElemIDs(&t_UCElemID);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if ((int)t_UCElemID.size() != spStackSize)
	{
		cout << "Critical Error: Invalid number of unit cell atoms (TSimulationBase::ValidateLattice)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Dotierungen ermitteln
	vector<int> t_DopedID;
	vector<int> t_DopandID;
	ErrorCode = m_Job->m_Structure->GetDopingIDs(&t_DopedID, &t_DopandID);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (t_DopedID.size() != t_DopandID.size())
	{
		cout << "Critical Error: Inconsistent doping vectors (TSimulationBase::ValidateLattice)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Dotierungsanzahlen ermitteln
	vector<long long> t_TargetDopandAnz;
	ErrorCode = m_Job->m_Settings->GetDopandCounts(&t_TargetDopandAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if (t_DopedID.size() != t_TargetDopandAnz.size())
	{
		cout << "Critical Error: Inconsistent doping count vectors (TSimulationBase::ValidateLattice)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Zaehler aufsetzen
	unsigned long long i_VacAnz = 0LL;
	vector<long long> i_DopandAnz;
	if (t_DopedID.size() != 0) i_DopandAnz = vector<long long>(t_DopedID.size(), 0LL);

	// Gitter pruefen
	for (int x = 0; x < spLatticeSize; x++)
	{
		for (int y = 0; y < spLatticeSize; y++)
		{
			for (int z = 0; z < spLatticeSize; z++)
			{
				for (int s = 0; s < spStackSize; s++)
				{

					// Auf Standardbesetzung pruefen
					if (i_lattice->at(x)[y][z][s] == t_UCElemID[s])
					{
						if (i_lattice->at(x)[y][z][s] == 1) i_VacAnz++;
						continue;
					}

					// Auf Leerstelle pruefen
					if ((i_lattice->at(x)[y][z][s] == 1) && ((t_UCElemID[s] == 0) || (t_UCElemID[s] == 1)))
					{
						i_VacAnz++;
						continue;
					}

					// Auf Dotierung pruefen
					if (t_DopedID.size() != 0)
					{
						bool t_dopand_found = false;
						for (int i = 0; i < (int)t_DopedID.size(); i++)
						{
							if ((t_DopedID[i] == t_UCElemID[s]) && (i_lattice->at(x)[y][z][s] == t_DopandID[i]))
							{
								(i_DopandAnz[i])++;
								t_dopand_found = true;
								break;
							}
						}
						if (t_dopand_found == true) continue;
					}

					return KMCERR_INVALID_INPUT;
				}
			}
		}
	}

	// Anzahlen vergleichen
	if (i_VacAnz != spTotalVacAnz) return KMCERR_INVALID_INPUT;
	if (i_DopandAnz.size() != 0)
	{
		for (int i = 0; i < (int)i_DopandAnz.size(); i++)
		{
			if (i_DopandAnz[i] != t_TargetDopandAnz[i]) return KMCERR_INVALID_INPUT;
		}
	}

	return KMCERR_OK;
}

// Wahrscheinlichkeitslisten einer Phase validieren
int TSimulationBase::ValidateProbLists(TSimPhaseInfo& i_phase, int i_listsize)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::ValidateProbLists)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (i_listsize < 0)
	{
		cout << "Critical Error: Negative list size (TSimulationBase::ValidateProbLists)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Falls noetig Listen verkleinern
	while ((int)i_phase.CurrentAttemptProbList.size() > i_listsize)
	{
		i_phase.CurrentAttemptProbList.pop_back();
	}
	while ((int)i_phase.CurrentAcceptedProbList.size() > i_listsize)
	{
		i_phase.CurrentAcceptedProbList.pop_back();
	}

	// Falls noetig Listen vergroessern
	while ((int)i_phase.CurrentAttemptProbList.size() < i_listsize)
	{
		i_phase.CurrentAttemptProbList.push_back(TSimProb());
	}
	while ((int)i_phase.CurrentAcceptedProbList.size() < i_listsize)
	{
		i_phase.CurrentAcceptedProbList.push_back(TSimProb());
	}

	// Pruefen, ob Wahrscheinlichkeiten absteigend geordnet, >= 0.0 und mit den Anzahlen konsistent (prob 0.0 -> count 0ULL) sind
	if (i_listsize != 0)
	{
		if (i_listsize > 1)
		{
			for (int i = 0; i < i_listsize - 1; i++)
			{
				if (i_phase.CurrentAttemptProbList[i].prob < 0.0) return KMCERR_INVALID_INPUT;
				if (i_phase.CurrentAttemptProbList[i].prob == 0.0)
				{
					if (i_phase.CurrentAttemptProbList[i].count != 0ULL) return KMCERR_INVALID_INPUT;
					if (i_phase.CurrentAttemptProbList[i + 1].prob != 0.0) return KMCERR_INVALID_INPUT;
				}
				else
				{
					if (i_phase.CurrentAttemptProbList[i].count == 0ULL) return KMCERR_INVALID_INPUT;
					if (i_phase.CurrentAttemptProbList[i + 1].prob >= KMCVAR_EQTHRESHOLD_PROBMINUS * i_phase.CurrentAttemptProbList[i].prob) return KMCERR_INVALID_INPUT;
				}

				if (i_phase.CurrentAcceptedProbList[i].prob < 0.0) return KMCERR_INVALID_INPUT;
				if (i_phase.CurrentAcceptedProbList[i].prob == 0.0)
				{
					if (i_phase.CurrentAcceptedProbList[i].count != 0ULL) return KMCERR_INVALID_INPUT;
					if (i_phase.CurrentAcceptedProbList[i + 1].prob != 0.0) return KMCERR_INVALID_INPUT;
				}
				else
				{
					if (i_phase.CurrentAcceptedProbList[i].count == 0ULL) return KMCERR_INVALID_INPUT;
					if (i_phase.CurrentAcceptedProbList[i + 1].prob >= KMCVAR_EQTHRESHOLD_PROBMINUS * i_phase.CurrentAcceptedProbList[i].prob) return KMCERR_INVALID_INPUT;
				}
			}
		}
		if (i_phase.CurrentAttemptProbList.back().prob < 0.0) return KMCERR_INVALID_INPUT;
		if (i_phase.CurrentAttemptProbList.back().prob == 0.0)
		{
			if (i_phase.CurrentAttemptProbList.back().count != 0ULL) return KMCERR_INVALID_INPUT;
		}
		else
		{
			if (i_phase.CurrentAttemptProbList.back().count == 0ULL) return KMCERR_INVALID_INPUT;
		}
		if (i_phase.CurrentAcceptedProbList.back().prob < 0.0) return KMCERR_INVALID_INPUT;
		if (i_phase.CurrentAcceptedProbList.back().prob == 0.0)
		{
			if (i_phase.CurrentAcceptedProbList.back().count != 0ULL) return KMCERR_INVALID_INPUT;
		}
		else
		{
			if (i_phase.CurrentAcceptedProbList.back().count == 0ULL) return KMCERR_INVALID_INPUT;
		}
	}

	return KMCERR_OK;
}

// Daten in CheckPoint-Datei speichern
void TSimulationBase::SaveCheckPoint(string i_space, string i_chkpath)
{
	if (Ready != true) return;
	int ErrorCode = KMCERR_OK;

	// Pruefen, ob Checkpoint geschrieben werden soll
	if (m_Job->m_Settings->IfWriteCheckpoint() == false) return;
	if (Trim(i_chkpath) == "") return;

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int)KMCOUT_TSIMULATION_CHK_OFFSET > 0) sub_offset = string((int)KMCOUT_TSIMULATION_CHK_OFFSET, ' ');

	// Startgitter speichern
	bool t_has_startlattice = false;
	stringstream t_startlattice;
	t_startlattice.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
	if (StartLattice.size() != 0)
	{
		t_startlattice << sub_offset << KMCOUT_TSIMULATION_STARTLATTICE_START << endl;
		for (int x = 0; x < (int)StartLattice.size(); x++)
		{
			for (int y = 0; y < (int)StartLattice[x].size(); y++)
			{
				for (int z = 0; z < (int)StartLattice[x][y].size(); z++)
				{
					t_startlattice << sub_offset;
					for (int s = 0; s < (int)StartLattice[x][y][z].size(); s++)
					{
						t_startlattice << StartLattice[x][y][z][s] << " ";
					}
					t_startlattice << endl;
				}
			}
		}
		t_startlattice << sub_offset << KMCOUT_TSIMULATION_STARTLATTICE_END << endl;
		t_has_startlattice = true;
	}

	// Prerun-Phase speichern
	bool t_has_prerun = true;
	stringstream t_prerun_stream;
	t_prerun_stream.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
	ErrorCode = PrerunPhaseData.SaveToStream(t_prerun_stream, KMCOUT_TSIMULATION_CHK_OFFSET, KMCOUT_TSIMULATION_PRERUN_START, KMCOUT_TSIMULATION_PRERUN_END);
	if (ErrorCode != KMCERR_OK) t_has_prerun = false;

	// DynNorm-Phase speichern
	bool t_has_dynnorm = true;
	stringstream t_dynnorm_stream;
	t_dynnorm_stream.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
	ErrorCode = DynNormPhaseData.SaveToStream(t_dynnorm_stream, KMCOUT_TSIMULATION_CHK_OFFSET, KMCOUT_TSIMULATION_DYNNORM_START, KMCOUT_TSIMULATION_DYNNORM_END);
	if (ErrorCode != KMCERR_OK) t_has_dynnorm = false;

	// Main-Phase speichern
	bool t_has_main = true;
	stringstream t_main_stream;
	t_main_stream.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);
	ErrorCode = MainPhaseData.SaveToStream(t_main_stream, KMCOUT_TSIMULATION_CHK_OFFSET, KMCOUT_TSIMULATION_MAIN_START, KMCOUT_TSIMULATION_MAIN_END);
	if (ErrorCode != KMCERR_OK) t_has_main = false;

	// Pruefen ob ueberhaupt Daten vorhanden
	if ((t_has_startlattice == false) && (t_has_prerun == false) && (t_has_dynnorm == false) && (t_has_main == false)) return;

	// Datei oeffnen
	ofstream outfile(std::string(Trim(i_chkpath)));
	if (outfile.is_open() == false) return;

	// Double-Precision setzen
	outfile.precision(KMCVAR_SAVEFILE_DOUBLEPRECISION);

	// Dateiheader schreiben
	outfile << "CHECKPOINT FILE" << endl;
	outfile << "iCon - Kinetic Monte-Carlo Simulation of Ionic Conductivity" << endl;
	outfile << "Copyright 2016-2025, P. Hein, Workgroup Martin, IPC, RWTH Aachen" << endl;
	outfile << "Distributed under GPL v3 license. Publication of results requires proper attribution." << endl;
	outfile << KMCOUT_VERSION << " " << KMC_VERSION << endl;
	outfile << "File creation time: " << TCustomTime::GetCurrentTimeStr() << endl;
	outfile << endl;
	outfile << KMCOUT_TSIMULATION_CHK_START;

	// Startgitter schreiben
	if (t_has_startlattice == true)
	{
		outfile << endl;
		outfile << t_startlattice.str();
		outfile.flush();
	}

	// Prerun schreiben
	if (t_has_prerun == true)
	{
		outfile << endl;
		outfile << t_prerun_stream.str();
		outfile.flush();
	}

	// DynNorm schreiben
	if (t_has_dynnorm == true)
	{
		outfile << endl;
		outfile << t_dynnorm_stream.str();
		outfile.flush();
	}

	// Main schreiben
	if (t_has_main == true)
	{
		outfile << endl;
		outfile << t_main_stream.str();
		outfile.flush();
	}

	// Abschlusszeile schreiben
	outfile << KMCOUT_TSIMULATION_CHK_END << endl;

	// Datei schliessen
	outfile.close();

	cout << i_space << "Checkpoint saved." << endl;
}

// Daten aus CheckPoint-Datei laden
int TSimulationBase::LoadCheckPoint(string i_chkpath)
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::LoadCheckPoint)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	// Pruefen, ob Checkpoint geladen werden soll
	if (m_Job->m_Settings->IfLoadCheckpoint() == false)
	{
		cout << "Loading checkpoint is disabled." << endl;
		return KMCERR_OK;
	}

	// Versuchen die Checkpoint-Datei zu oeffnen
	ifstream infile(std::string(Trim(i_chkpath)));
	if (infile.is_open() == false)
	{
		cout << "No checkpoint file found." << endl;
		return KMCERR_OK;
	}

	// Zeilenenden korrigieren
	string infilestr((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
	infile.close();
	infilestr.erase(remove(infilestr.begin(), infilestr.end(), '\r'), infilestr.end());
	stringstream infilestream(infilestr);

	// Inputparameter setzen
	infilestream >> skipws;

	cout << endl;
	cout << "Loading checkpoint ..." << endl;

	vector<int> t_lin_startlattice;
	string i_prerun_str = "";
	string i_dynnorm_str = "";
	string i_main_str = "";

	string line = "";
	bool chk_found = false;
	bool if_failed = false;
	string s_temp = "";
	while (infilestream.good() == true)
	{
		// Zeile laden
		if (getline(infilestream, line).fail() == true)
		{
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream(line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TSIMULATION_CHK_START)
		{
			chk_found = true;
		}
		else if (s_temp == KMCOUT_VERSION)
		{
			if ((linestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if (s_temp != KMC_VERSION)
			{
				if_failed = true;
				break;
			}
		}
		else if ((s_temp == KMCOUT_TSIMULATION_STARTLATTICE_START) && (chk_found == true))
		{
			if ((infilestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			while (s_temp != KMCOUT_TSIMULATION_STARTLATTICE_END)
			{
				t_lin_startlattice.push_back(-1);
				if ((size_t)t_lin_startlattice.size() > size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spStackSize))
				{
					if_failed = true;
					break;
				}
				if (ConvStrToInt(s_temp, t_lin_startlattice.back()) == false)
				{
					if_failed = true;
					break;
				}
				if ((t_lin_startlattice.back() < 0) || (t_lin_startlattice.back() > KMCLIM_MAX_ELEMENTS))
				{
					if_failed = true;
					break;
				}
				if ((infilestream >> s_temp).fail() == true)
				{
					if_failed = true;
					break;
				}
			}
			if (if_failed == true) break;
		}
		else if ((s_temp == KMCOUT_TSIMULATION_PRERUN_START) && (chk_found == true))
		{
			s_temp = "";
			stringstream temp_stream("");
			while (s_temp != KMCOUT_TSIMULATION_PRERUN_END)
			{
				if (getline(infilestream, line).fail() == true)
				{
					if_failed = true;
					break;
				}
				temp_stream << line << endl;
				stringstream linestream2(line);
				if ((linestream2 >> s_temp).fail() == true) s_temp = "";
			}
			if (if_failed == true) break;
			i_prerun_str = temp_stream.str();
		}
		else if ((s_temp == KMCOUT_TSIMULATION_DYNNORM_START) && (chk_found == true))
		{
			s_temp = "";
			stringstream temp_stream("");
			while (s_temp != KMCOUT_TSIMULATION_DYNNORM_END)
			{
				if (getline(infilestream, line).fail() == true)
				{
					if_failed = true;
					break;
				}
				temp_stream << line << endl;
				stringstream linestream2(line);
				if ((linestream2 >> s_temp).fail() == true) s_temp = "";
			}
			if (if_failed == true) break;
			i_dynnorm_str = temp_stream.str();
		}
		else if ((s_temp == KMCOUT_TSIMULATION_MAIN_START) && (chk_found == true))
		{
			s_temp = "";
			stringstream temp_stream("");
			while (s_temp != KMCOUT_TSIMULATION_MAIN_END)
			{
				if (getline(infilestream, line).fail() == true)
				{
					if_failed = true;
					break;
				}
				temp_stream << line << endl;
				stringstream linestream2(line);
				if ((linestream2 >> s_temp).fail() == true) s_temp = "";
			}
			if (if_failed == true) break;
			i_main_str = temp_stream.str();
		}
		else if (s_temp == KMCOUT_TSIMULATION_CHK_END) break;
	}
	if ((size_t)t_lin_startlattice.size() != size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spLatticeSize) * size_t(spStackSize)) if_failed = true;
	if (chk_found == false) if_failed = true;
	if (if_failed == true)
	{
		cout << "  Invalid checkpoint file format." << endl;
		cout << "Checkpoint loading aborted." << endl;
		return KMCERR_OK;
	}
	cout << "  Checkpoint data found." << endl;

	// Daten loeschen
	StartLattice.clear();
	PrerunPhaseData.Clear();
	DynNormPhaseData.Clear();
	MainPhaseData.Clear();
	ClearCurrentSim();
	cout << "  Previous simulation data deleted." << endl;

	// Temporaeres StartLattice erstellen und validieren
	vector<vector<vector<vector<int>>>> i_StartLattice(spLatticeSize, vector<vector<vector<int>>>(spLatticeSize,
		vector<vector<int>>(spLatticeSize, vector<int>(spStackSize, -1))));
	for (int x = 0; x < spLatticeSize; x++)
	{
		for (int y = 0; y < spLatticeSize; y++)
		{
			for (int z = 0; z < spLatticeSize; z++)
			{
				for (int s = 0; s < spStackSize; s++)
				{
					i_StartLattice[x][y][z][s] = t_lin_startlattice[(((size_t)spLatticeSize * x + y) * spLatticeSize + z) * spStackSize + s];
				}
			}
		}
	}
	ErrorCode = ValidateLattice(&i_StartLattice);
	if (ErrorCode != KMCERR_OK)
	{
		cout << "  No valid start lattice found." << endl;
		cout << "Checkpoint loading aborted." << endl;
		return KMCERR_OK;
	}
	else
	{
		StartLattice = i_StartLattice;
		cout << "  Valid start lattice loaded." << endl;
	}

	// Temporaere PrerunPhaseData erstellen und validieren
	if (i_prerun_str != "")
	{
		TSimPhaseInfo i_PrerunPhaseData;
		stringstream t_prerun_stream(i_prerun_str);
		ErrorCode = i_PrerunPhaseData.LoadFromStream(t_prerun_stream, spLatticeSize, spStackSize, spMovStackSize, KMCOUT_TSIMULATION_PRERUN_END);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "  No valid prerun data found." << endl;
		}
		else
		{
			ErrorCode = ValidatePrerunData(i_PrerunPhaseData);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "  Prerun data is invalid." << endl;
			}
			else
			{
				PrerunPhaseData = i_PrerunPhaseData;
				cout << "  Valid prerun data loaded." << endl;
			}
		}
	}
	else
	{
		cout << "  No prerun data present." << endl;
	}

	// Temporaere DynNormPhaseData erstellen und validieren
	if (i_dynnorm_str != "")
	{
		TSimPhaseInfo i_DynNormPhaseData;
		stringstream t_dynnorm_stream(i_dynnorm_str);
		ErrorCode = i_DynNormPhaseData.LoadFromStream(t_dynnorm_stream, spLatticeSize, spStackSize, spMovStackSize, KMCOUT_TSIMULATION_DYNNORM_END);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "  No valid dyn. norm. data found." << endl;
		}
		else
		{
			ErrorCode = ValidateDynNormData(i_DynNormPhaseData);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "  Dyn. norm. data is invalid." << endl;
			}
			else
			{
				DynNormPhaseData = i_DynNormPhaseData;
				cout << "  Valid dyn. norm. data loaded." << endl;
			}
		}
	}
	else
	{
		cout << "  No dyn. norm. data present." << endl;
	}

	// Temporaere MainPhaseData erstellen und validieren
	if (i_main_str != "")
	{
		TSimPhaseInfo i_MainPhaseData;
		stringstream t_main_stream(i_main_str);
		ErrorCode = i_MainPhaseData.LoadFromStream(t_main_stream, spLatticeSize, spStackSize, spMovStackSize, KMCOUT_TSIMULATION_MAIN_END);
		if (ErrorCode != KMCERR_OK)
		{
			cout << "  No valid main simulation data found." << endl;
		}
		else
		{
			ErrorCode = ValidateMainData(i_MainPhaseData);
			if (ErrorCode != KMCERR_OK)
			{
				cout << "  Main simulation data is invalid." << endl;
			}
			else
			{
				MainPhaseData = i_MainPhaseData;
				cout << "  Valid main simulation data loaded." << endl;
			}
		}
	}
	else
	{
		cout << "  No main simulation data present." << endl;
	}

	cout << "Checkpoint loading complete." << endl;
	return KMCERR_OK;
}

// Standard Exception Handler
int TSimulationBase::StdExceptionHandler(exception& e)
{
	cout << KMCERRSTR_STDERR << " " << e.what() << endl;
	return KMCERR_EXCEPTION_OCCURED;
}

// Exception Handler for unknown Exceptions
int TSimulationBase::ExceptionHandler()
{
	cout << KMCERRSTR_UNKNOWNERR << endl;
	return KMCERR_EXCEPTION_OCCURED;
}

// ***************************** PROTECTED ********************************** //

// Bewegungsstatistik ausgeben
int TSimulationBase::MovLatticeAnalysis(const vector<vector<vector<vector<int>>>>* i_lattice,
	const vector<vector<vector<vector<T3DMovVector>>>>* i_movlattice, vector<double>* o_results) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::MovLatticeAnalysis)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Ergebnisvektor erstellen
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
	*o_results = vector<double>(18, 0.0);

	// Bewegungsvektoren aufsummieren
	int t_idshift = 0;
	long long t_mov_count = 0LL;
	long long t_vac_count = 0LL;
	if (i_movlattice->size() != 0)
	{
		for (int x = 0; x < (int)i_movlattice->size(); x++)
		{
			if (i_movlattice->at(x).size() != 0)
			{
				for (int y = 0; y < (int)i_movlattice->at(x).size(); y++)
				{
					if (i_movlattice->at(x)[y].size() != 0)
					{
						for (int z = 0; z < (int)i_movlattice->at(x)[y].size(); z++)
						{
							if (i_movlattice->at(x)[y][z].size() != 0)
							{
								for (int s = 0; s < (int)i_movlattice->at(x)[y][z].size(); s++)
								{

									// ElemID ueberpruefen
									if (i_lattice->at(x)[y][z][s] == 0)
									{
										t_idshift = 0;
										t_mov_count += 1LL;
									}
									if (i_lattice->at(x)[y][z][s] == 1)
									{
										t_idshift = 9;
										t_vac_count += 1LL;
									}
									if ((i_lattice->at(x)[y][z][s] != 0) && (i_lattice->at(x)[y][z][s] != 1))
									{
										cout << "Critical Error: Invalid ElemID in the lattice (TSimulationBase::MovLatticeAnalysis)" << endl << endl;
										return KMCERR_INVALID_INPUT_CRIT;
									}

									// Bewegung einrechnen
									o_results->at(0 + t_idshift) += double(i_movlattice->at(x)[y][z][s].jcount);
									o_results->at(1 + t_idshift) += i_movlattice->at(x)[y][z][s].x;
									o_results->at(2 + t_idshift) += i_movlattice->at(x)[y][z][s].y;
									o_results->at(3 + t_idshift) += i_movlattice->at(x)[y][z][s].z;
									o_results->at(4 + t_idshift) += sqrt(i_movlattice->at(x)[y][z][s].x * i_movlattice->at(x)[y][z][s].x +
										i_movlattice->at(x)[y][z][s].y * i_movlattice->at(x)[y][z][s].y + i_movlattice->at(x)[y][z][s].z *
										i_movlattice->at(x)[y][z][s].z);
									o_results->at(5 + t_idshift) += i_movlattice->at(x)[y][z][s].x * i_movlattice->at(x)[y][z][s].x +
										i_movlattice->at(x)[y][z][s].y * i_movlattice->at(x)[y][z][s].y + i_movlattice->at(x)[y][z][s].z *
										i_movlattice->at(x)[y][z][s].z;
								}
							}
						}
					}
				}
			}
		}
	}
	if ((t_mov_count <= 0) || (t_vac_count <= 0))
	{
		cout << "Critical Error: No moving atoms or vacancies found (TSimulationBase::MovLatticeAnalysis)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Summen uebertragen und Einheit umrechnen (1 angstrom = 1.0E-8 cm)
	o_results->at(6) = o_results->at(1) * 1.0E-08;
	o_results->at(7) = o_results->at(2) * 1.0E-08;
	o_results->at(8) = o_results->at(3) * 1.0E-08;
	o_results->at(15) = o_results->at(10) * 1.0E-08;
	o_results->at(16) = o_results->at(11) * 1.0E-08;
	o_results->at(17) = o_results->at(12) * 1.0E-08;

	// Mittelwerte berechnen und Einheit umrechnen (1 angstrom = 1.0E-8 cm)
	o_results->at(0) /= double(t_mov_count);
	o_results->at(1) *= 1.0E-08 / double(t_mov_count);
	o_results->at(2) *= 1.0E-08 / double(t_mov_count);
	o_results->at(3) *= 1.0E-08 / double(t_mov_count);
	o_results->at(4) *= 1.0E-08 / double(t_mov_count);
	o_results->at(5) *= 1.0E-16 / double(t_mov_count);
	o_results->at(9) /= double(t_vac_count);
	o_results->at(10) *= 1.0E-08 / double(t_vac_count);
	o_results->at(11) *= 1.0E-08 / double(t_vac_count);
	o_results->at(12) *= 1.0E-08 / double(t_vac_count);
	o_results->at(13) *= 1.0E-08 / double(t_vac_count);
	o_results->at(14) *= 1.0E-16 / double(t_vac_count);

	return KMCERR_OK;
}

// Verschiebungsvektor in eine Komponente parallel zum E-Feld und eine Komponente senkrecht zum E-Feld zerlegen (in der Ebene, die durch Verschiebungsvektor und E-Feld-Vektor aufgespannt wird)
int TSimulationBase::GetProjOnEField(const T3DVector& i_disp_vec, double& o_comp_parallel, double& o_comp_perpendicular) const
{
	if (Ready != true)
	{
		cout << "Critical Error: Object is not ready (TSimulationBase::GetProjOnEField)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	int ErrorCode = KMCERR_OK;

	// E-Feld ermitteln
	T3DVector t_efield;
	ErrorCode = m_Job->m_Settings->GetEField(t_efield.x, t_efield.y, t_efield.z);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// E-Feld pruefen
	if (t_efield.Length() < T3DVector::zero_threshold)
	{
		o_comp_parallel = 0.0;
		o_comp_perpendicular = 0.0;
		return KMCERR_OK;
	}

	// Projektionen berechnen
	o_comp_parallel = (i_disp_vec * t_efield) / t_efield.Length();
	o_comp_perpendicular = (i_disp_vec - t_efield * ((i_disp_vec * t_efield) / (t_efield * t_efield))).Length();

	return KMCERR_OK;
}
