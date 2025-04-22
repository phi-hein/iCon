// **************************************************************** //
//																	//
//	Klasse: TSettingsBase	(TSettings Layer 0)						//
//	Autor: Philipp Hein												//
//	Datum: 16.10.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der Job-Spezifikationen					//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TSettingsBase_H
#define TSettingsBase_H

// Includes:
#include <string>
#include <iostream>
#include <vector>

// Eigene Includes:
#include "T3DVector.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TSettingsBase {
	// Member variables
	protected:
		TKMCJob * m_Job;			// Pointer zur uebergeordneten KMCJob-Instanz

		// Allgemeine Einstellungen
		double Temperature;			// Simulationstemperatur [in K]
		double AttemptFrequency;	// Frequenz der Sprungversuche [in 1/s]
		double EFieldMagnitude;		// Maximaler E-Feld-Beitrag zur Aktivierungsenergie [in kT]
		T3DVector EFieldDirection;	// E-Feld-Einheitsrichtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
		T3DVector EField;			// E-Feld-Vektor von plus-Pol zu minus-Pol [in V/cm]
		bool WriteCheckpoint;		// Checkpoint schreiben
		bool LoadCheckpoint;		// Checkpoint laden

		// Gittereinstellungen
		int LatticeSize;					// Gittergröße (kubisches Gitter)
		long long AdditionalVacAnz;			// Zusätzliche Leerstellen
		double TotalVacConc;				// Leerstellengesamtkonzentration
		long long TotalVacAnz;				// Leerstellengesamtanzahl
		double MovVolConc;					// Volumenkonzentration (Anzahl/Volumen) der beweglichen Spezies [in cm^-3]
		vector<double> DopandConc;			// Dopandenkonzentrationen für jede Dotierung in m_Structure
		vector<long long> DopandAnz;		// Anzahl an Dopanden für jede Dotierung in m_Structure

		// Prerun-Einstellungen
		bool DoPrerun;				// Schalter: true = Vorlauf-Equilibrierung durchführen
		long long PreMCSP;			// Ziel-MCSP des Vorlaufs
		int PreRecordAnz;			// Anzahl an aufzuzeichnenden, hoechsten Boltzmann-Wahrscheinlichkeiten bei Vorlauf

		// DynNorm-Einstellungen
		bool DoDynNorm;				// Schalter: true = dyn. Normierungssimulation durchführen
		long long DynAttemptAnz;	// Anzahl an Sprungversuchen bei dyn. Normierungssimulation			
		int DynRecordAnz;			// Anzahl an aufzuzeichnenden, hoechsten Sprungversuchswahrscheinlichkeiten bei dyn. Normierungssimulation
		int DynNormNum;				// Nummer des positiven Eintrags in der Liste der aufgezeichneten Wahrscheinlichkeiten, der für die dyn. Normierung verwendet wird (0 = manuell setzen nach DynNorm-Iterationen)

		// KMC-Simulationseinstellungen
		long long MCSP;				// Ziel-MCSP der Hauptsimulation: Monte-Carlo-Steps per Particle (d.h. per Mov-Elem im Gitter)
		int MainRecordAnz;			// Anzahl an aufzuzeichnenden, hoechsten Boltzmann-Wahrscheinlichkeiten bei Hauptsimulation

		bool Ready;					// Flag, ob alle Einstellungen gesetzt wurden

	// Member functions
	public:
		// PUBLISHED
		int SetTemperature (double i_Temperature);													// Temperatur einstellen
		int SetAttemptFrequency (double i_Frequency);												// Sprungfrequenz einstellen
		int SetEField (double i_EFieldX, double i_EFieldY, double i_EFieldZ, double i_MaxEField);	// E-Feld setzen aus MaxEField = maximaler E-Feld-Beitrag zu Aktivierungsenergie [in kBT] und EFieldxyz = E-Feld-Richtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
		
		int SetLatticeSize (int i_LatticeSize);														// Gittergröße einstellen
		int SetAdditionalVacAnz (long long i_AdditionalVacAnz);										// Zusätzliche Leerstellen eingeben
		int SetDopandConcentration (int i_DopingID, double i_Conc);									// Dotierkonzentration für eine bestimmte Dotierung eingeben
		int SetCheckpointFlags(bool i_writechk, bool i_loadchk);									// Schreiben und Laden des Checkpoints aktivieren oder deaktivieren
		
		int SetPrerunOptions (bool i_DoPrerun, long long i_PreMCSP, int i_PreRecordAnz);											// KMC-Vorlaufparameter einstellen
		int SetDynNormParameters (bool i_DoDynNorm, long long i_DynNormAttemptAnz, int i_DynNormRecordAnz, int i_DynNormNum);		// Parameter für dyn. Normierungssimulation setzen
		int SetMainKMCOptions (long long i_MCSP, int i_MainRecordAnz);																// KMC-Hauptsimulationsparameter eingeben
		
		int MakeSettings ();																		// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen
		
		// NON-PUBLISHED
		TSettingsBase (TKMCJob * pJob);						// Constructor, Ready = false setzen
		bool IfReady ();									// Rueckgabe von Ready
		int SaveToStream (ostream &output, int offset);		// Einstellungen in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
		int LoadFromStream (istream &input);				// Einstellungen aus einem Stream laden, nur bei Ready == false moeglich

	protected:
		// PROTECTED
		~TSettingsBase();						// Destructor
		int SyncDopandVectors();				// DopandConc und DopandAnz mit Anzahl der Dotierungen in m_Job->m_Structure synchronisieren

	private:
		// PRIVATE
		int CalculateEField();					// E-Feld berechnen
		int CalculateDopands();					// Dopandenkonzentrationen validieren und ihre Anzahlen berechnen
};

#endif