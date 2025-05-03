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

// Deklarierte Klasse:
#include "TSettingsBase.h"

// Includes:
#include <iomanip>
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"
#include "TStructure/TStructure.h"
#include "TJumps/TJumps.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TSettingsBase::TSettingsBase (TKMCJob * pJob) {
	m_Job = pJob;

	// Memberinitialisierung
	Temperature = KMCVAR_TEMPERATURE_INIT;
	AttemptFrequency = KMCVAR_ATTEMPTFREQ_INIT;
	WriteCheckpoint = true;
	LoadCheckpoint = true;
	EFieldMagnitude = KMCVAR_EFIELDMAGNITUDE_INIT;
	EFieldDirection.x = KMCVAR_EFIELDX_INIT;
	EFieldDirection.y = KMCVAR_EFIELDY_INIT;
	EFieldDirection.z = KMCVAR_EFIELDZ_INIT;
	EField.x = 0;
	EField.y = 0;
	EField.z = 0;

	LatticeSize = KMCVAR_LATTICESIZE_INIT;
	AdditionalVacAnz = 0;
	TotalVacConc = 0;
	TotalVacAnz = 0;
	MovVolConc = 0;
	DopandConc.clear();
	DopandAnz.clear();

	DoPrerun = false;
	PreMCSP = 0;
	PreRecordAnz = 0;

	DoDynNorm = false;
	DynAttemptAnz = 0;		
	DynRecordAnz = 0;
	DynNormNum = 1;
	
	MCSP = KMCVAR_MCSP_INIT;
	MainRecordAnz = 0;

	Ready = false;
}

// Destructor
TSettingsBase::~TSettingsBase () {
	m_Job = 0;
}
		
// **************************** PUBLISHED ********************************* //

// Temperatur einstellen
int TSettingsBase::SetTemperature (double i_Temperature) {
	// Input prüfen
	if ((i_Temperature < KMCVAL_TSETTINGS_MIN_TEMPERATURE) || (i_Temperature > KMCVAL_TSETTINGS_MAX_TEMPERATURE)) {
		cout << "Error: Invalid temperature: " << i_Temperature << " K" << endl;
		cout << "       Temperature has to be a rational number in the interval [" << KMCVAL_TSETTINGS_MIN_TEMPERATURE;
		cout << " , " << KMCVAL_TSETTINGS_MAX_TEMPERATURE << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if (abs(i_Temperature - Temperature) > T3DVector::zero_threshold) {
		Temperature = i_Temperature;
		Ready = false;
	}
	return KMCERR_OK;
}

// Sprungfrequenz einstellen
int TSettingsBase::SetAttemptFrequency (double i_Frequency) {
	// Input prüfen
	if ((i_Frequency < KMCVAL_TSETTINGS_MIN_FREQUENCY) || (i_Frequency > KMCVAL_TSETTINGS_MAX_FREQUENCY)) {
		cout << "Error: Invalid jump attempt frequency: " << i_Frequency << " Hz" << endl;
		cout << "       Jump attempt frequency has to be a rational number in the interval [" << KMCVAL_TSETTINGS_MIN_FREQUENCY;
		cout << " , " << KMCVAL_TSETTINGS_MAX_FREQUENCY << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if (abs(i_Frequency - AttemptFrequency) > T3DVector::zero_threshold) {
		AttemptFrequency = i_Frequency;
		Ready = false;
	}
	return KMCERR_OK;
}

// E-Feld setzen aus MaxEField = maximaler E-Feld-Beitrag zu Aktivierungsenergie [in kBT] und EFieldxyz = E-Feld-Richtungsvektor von plus-Pol zu minus-Pol (Kraftvektorrichtung auf eine positive Testladung)
int TSettingsBase::SetEField (double i_EFieldX, double i_EFieldY, double i_EFieldZ, double i_MaxEField) {
	// Input prüfen
	if ((abs(i_EFieldX) > 1.0E+150) || (abs(i_EFieldY) > 1.0E+150) || (abs(i_EFieldZ) > 1.0E+150)) {
		cout << "Error: Invalid E-field direction vector: (" << i_EFieldX << " , " << i_EFieldY << " , " << i_EFieldZ << ")" << endl;
		cout << "       Vector components have to be rational numbers in the interval [-1.0E+150 , 1.0E+150]." << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_MaxEField < 0.0) || (i_MaxEField > KMCVAL_TSETTINGS_MAX_EFIELD)) {
		cout << "Error: Invalid E-field magnitude: " << i_MaxEField << " kT" << endl;
		cout << "       E-field magnitude has to be a rational number in the interval [0 , " << KMCVAL_TSETTINGS_MAX_EFIELD << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	T3DVector i_EField (i_EFieldX, i_EFieldY, i_EFieldZ);
	if (i_EField.Length() != 0.0) {
		i_EField = i_EField*(1/i_EField.Length());		// Einheitsvektor erzeugen
	}
	if ((i_EField != EFieldDirection) || (abs(i_MaxEField - EFieldMagnitude) > T3DVector::zero_threshold)) {
		EFieldDirection = i_EField;
		EFieldMagnitude = i_MaxEField;
		Ready = false;
	}
	return KMCERR_OK;
}

// Gittergröße einstellen
int TSettingsBase::SetLatticeSize (int i_LatticeSize) {
	// Input prüfen
	if ((i_LatticeSize < KMCVAL_TSETTINGS_MIN_LATTICESIZE) || (i_LatticeSize > KMCVAL_TSETTINGS_MAX_LATTICESIZE)) {
		cout << "Error: Invalid lattice size: " << i_LatticeSize << endl;
		cout << "       Lattice size has to be a number in the interval [" << KMCVAL_TSETTINGS_MIN_LATTICESIZE;
		cout << " , " << KMCVAL_TSETTINGS_MAX_LATTICESIZE << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if (i_LatticeSize != LatticeSize) {
		LatticeSize = i_LatticeSize;
		Ready = false;
	}
	return KMCERR_OK;
}

// Zusätzliche Leerstellen eingeben
int TSettingsBase::SetAdditionalVacAnz (long long i_AdditionalVacAnz) {
	// Input prüfen
	if ((i_AdditionalVacAnz < -KMCVAL_TSETTINGS_MAX_ADDVACANZ) || (i_AdditionalVacAnz > KMCVAL_TSETTINGS_MAX_ADDVACANZ)) {
		cout << "Error: Invalid number of additional vacancies: " << i_AdditionalVacAnz << endl;
		cout << "       Number of additional vacancies has to be a number in the interval [-" << KMCVAL_TSETTINGS_MAX_ADDVACANZ << ", " << KMCVAL_TSETTINGS_MAX_ADDVACANZ << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if (i_AdditionalVacAnz != AdditionalVacAnz) {
		AdditionalVacAnz = i_AdditionalVacAnz;
		Ready = false;
	}
	return KMCERR_OK;
}

// Dotierkonzentration für eine bestimmte Dotierung eingeben
int TSettingsBase::SetDopandConcentration (int i_DopingID, double i_Conc) {
	// Dotierungsanzahl pruefen
	int ErrorCode = SyncDopandVectors();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Input prüfen
	if ((i_DopingID < 0) || (i_DopingID >= (int) DopandConc.size())) {
		cout << "Critical Error: Invalid vector index (in TSettingsBase::SetDopandConcentration)" << endl;
		cout << "                Input: " << i_DopingID << " " << i_Conc << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((i_Conc < 0.0) || (i_Conc > 1.0)) {
		cout << "Error: Invalid concentration: " << i_Conc << endl;
		cout << "       Dopand concentration has to be a rational number in the interval [0.0 , 1.0]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übernehmen
	if (abs(DopandConc[i_DopingID] - i_Conc) > T3DVector::zero_threshold) {
		DopandConc[i_DopingID] = i_Conc;
		Ready = false;
	}
	return KMCERR_OK;
}

// Schreiben und Laden des Checkpoints aktivieren oder deaktivieren
int TSettingsBase::SetCheckpointFlags(bool i_writechk, bool i_loadchk) {

	// Input übertragen
	if ((i_writechk != WriteCheckpoint) || (i_loadchk != LoadCheckpoint)) {
		WriteCheckpoint = i_writechk;
		LoadCheckpoint = i_loadchk;
		Ready = false;
	}
	return KMCERR_OK;
}

// KMC-Vorlaufparameter einstellen
int TSettingsBase::SetPrerunOptions (bool i_DoPrerun, long long i_PreMCSP, int i_PreRecordAnz) {
	// Input prüfen
	if ((i_PreMCSP < 0LL) || (i_PreMCSP > KMCVAL_TSETTINGS_MAX_PREMCSP)) {
		cout << "Error: Invalid prerun monte carlo steps per particle: " << i_PreMCSP << endl;
		cout << "       Prerun-MCSP has to be a number in the interval [0 , " << KMCVAL_TSETTINGS_MAX_PREMCSP << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_PreRecordAnz < 0) || (i_PreRecordAnz > KMCVAL_TSETTINGS_MAX_PRERECORDANZ)) {
		cout << "Error: Invalid prerun record number: " << i_PreRecordAnz << endl;
		cout << "       Prerun record number has to be a number in the interval [0 , " << KMCVAL_TSETTINGS_MAX_PRERECORDANZ << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if ((i_DoPrerun != DoPrerun) || (i_PreMCSP != PreMCSP) ||(i_PreRecordAnz != PreRecordAnz)) {
		DoPrerun = i_DoPrerun;
		PreMCSP = i_PreMCSP;
		PreRecordAnz = i_PreRecordAnz;
		Ready = false;
	}
	return KMCERR_OK;
}

// Parameter für dyn. Normierungssimulation setzen
int TSettingsBase::SetDynNormParameters (bool i_DoDynNorm, long long i_DynNormAttemptAnz, int i_DynNormRecordAnz, int i_DynNormNum) {
	// Input prüfen
	if ((i_DynNormAttemptAnz < 0LL) || (i_DynNormAttemptAnz > KMCVAL_TSETTINGS_MAX_NORMATTEMPTANZ)) {
		cout << "Error: Invalid number of jump attempts for dynamic normalization: " << i_DynNormAttemptAnz << endl;
		cout << "       The jump attempt number has to be in the interval [0 , " << KMCVAL_TSETTINGS_MAX_NORMATTEMPTANZ << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_DynNormRecordAnz < 0) || (i_DynNormRecordAnz > KMCVAL_TSETTINGS_MAX_NORMRECORDANZ)) {
		cout << "Error: Invalid size of dyn. norm. probability list: " << i_DynNormRecordAnz << endl;
		cout << "       The size has to be in the interval [0 , " << KMCVAL_TSETTINGS_MAX_NORMRECORDANZ << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_DynNormNum < 0) || (i_DynNormNum > KMCVAL_TSETTINGS_MAX_NORMNUM)) {
		cout << "Error: Invalid number of the dyn. norm. list entry: " << i_DynNormNum << endl;
		cout << "       The list entry number has to be in the interval [0 , " << KMCVAL_TSETTINGS_MAX_NORMNUM << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if ((i_DoDynNorm != DoDynNorm) || (i_DynNormAttemptAnz != DynAttemptAnz) || (i_DynNormRecordAnz != DynRecordAnz) || (i_DynNormNum != DynNormNum)) {
		DoDynNorm = i_DoDynNorm;
		DynRecordAnz = i_DynNormRecordAnz;
		DynAttemptAnz = i_DynNormAttemptAnz;
		DynNormNum = i_DynNormNum;
		Ready = false;
	}
	return KMCERR_OK;
}

// KMC-Hauptsimulationsparameter eingeben
int TSettingsBase::SetMainKMCOptions (long long i_MCSP, int i_MainRecordAnz) {
	// Input prüfen
	if ((i_MCSP < 1LL) || (i_MCSP > KMCVAL_TSETTINGS_MAX_MCSP)) {
		cout << "Error: Invalid monte carlo steps per particle: " << i_MCSP << endl;
		cout << "       MCSP has to be a number in the interval [1 , " << KMCVAL_TSETTINGS_MAX_MCSP << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_MainRecordAnz < 0) || (i_MainRecordAnz > KMCVAL_TSETTINGS_MAX_RECORDANZ)) {
		cout << "Error: Invalid size of probability list: " << i_MainRecordAnz << endl;
		cout << "       The list size has to be a number in the interval [0 , " << KMCVAL_TSETTINGS_MAX_RECORDANZ << "]" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_INPUT;
	}

	// Input übertragen
	if ((i_MCSP != MCSP) ||(i_MainRecordAnz != MainRecordAnz)) {
		MCSP = i_MCSP;
		MainRecordAnz = i_MainRecordAnz;
		Ready = false;
	}
	return KMCERR_OK;
}

// Einstellungen abschließen, d.h. Parameter validieren, Ready = true setzen
int TSettingsBase::MakeSettings () {

	Ready = false;

	// E-Feld berechnen
	int ErrorCode = CalculateEField();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Dopanden- und Leerstellenanzahlen berechnen
	ErrorCode = CalculateDopands();
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Prerun-Validierung
	if (PreMCSP == 0LL) DoPrerun = false;

	// DynNorm-Validierung
	if (DynAttemptAnz == 0LL) DoDynNorm = false;
	if (DynRecordAnz == 0) DoDynNorm = false;

	Ready = true;
	return KMCERR_OK;
}		

// ***************************** PUBLIC *********************************** //

// Rueckgabe von Ready
bool TSettingsBase::IfReady () {
	return Ready;
}

// Settings in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
int TSettingsBase::SaveToStream (ostream &output, int offset) {
	if (Ready != true) {
		cout << "Critical Error: TSettings not ready (in TSettingsBase::SaveToStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true) {
		cout << "Critical Error: Output stream not ready (in TSettingsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) {
		cout << "Critical Error: Offset out of bounds (in TSettingsBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset,' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int) KMCOUT_TSETTINGS_OFFSET > 0) sub_offset = string((int) KMCOUT_TSETTINGS_OFFSET,' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile und physikalischer Parameter
	output << s_offset << KMCOUT_TSETTINGS_START << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_TEMPERATURE << " " << Temperature << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_LATTICESIZE << " " << LatticeSize << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_ATTEMPTFREQUENCY << " " << AttemptFrequency << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_WRITECHK << " ";
	if (WriteCheckpoint == true) {
		output << KMCOUT_TSETTINGS_DO_TRUE << endl;
	} else {
		output << KMCOUT_TSETTINGS_DO_FALSE << endl;
	}
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_LOADCHK << " ";
	if (LoadCheckpoint == true) {
		output << KMCOUT_TSETTINGS_DO_TRUE << endl;
	} else {
		output << KMCOUT_TSETTINGS_DO_FALSE << endl;
	}
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_ADDVACANZ << " " << AdditionalVacAnz << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_TOTALVACANZ << " " << TotalVacAnz << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_TOTALVACCONC << " " << TotalVacConc << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_MOVVOLCONC << " " << MovVolConc << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_EFIELDSTRENGTH << " " << EFieldMagnitude << endl;
	output << endl;

	// Ausgabe des E-Felds
	int x_space = (int) output.precision() + 7;
	int y_space = (int) output.precision() + 7;
	int z_space = (int) output.precision() + 7;
	if (x_space < (int) KMCOUT_TSETTINGS_EFIELDX.length()) x_space = KMCOUT_TSETTINGS_EFIELDX.length();
	if (y_space < (int) KMCOUT_TSETTINGS_EFIELDY.length()) y_space = KMCOUT_TSETTINGS_EFIELDY.length();
	if (z_space < (int) KMCOUT_TSETTINGS_EFIELDZ.length()) z_space = KMCOUT_TSETTINGS_EFIELDZ.length();
	output << s_offset << sub_offset << setw(KMCOUT_TSETTINGS_EFIELD.front().length() + 1) << " " << "( ";
	output << setw(x_space) << KMCOUT_TSETTINGS_EFIELDX << " ";
	output << setw(y_space) << KMCOUT_TSETTINGS_EFIELDY << " ";
	output << setw(z_space) << KMCOUT_TSETTINGS_EFIELDZ << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_EFIELD << " ( ";
	output << setw(x_space) << EField.x << " " << setw(y_space) << EField.y << " " << setw(z_space) << EField.z << " )" << endl;
	output << endl;

	// Ausgabe der Dotierkonzentrationen
	if (DopandAnz.size() != 0) {
		int ID_space = IntToStr(((int) DopandAnz.size()) - 1).length();
		if (ID_space < (int)KMCOUT_TSETTINGS_DOPING.front().length()) ID_space = KMCOUT_TSETTINGS_DOPING.front().length();
		int conc_space = (int) output.precision() + 7;
		if (conc_space < (int)KMCOUT_TSETTINGS_CONC.front().length()) conc_space = KMCOUT_TSETTINGS_CONC.front().length();
		int anz_space = KMCOUT_TSETTINGS_ANZ.front().length();
		for (int i = 0; i < (int) DopandAnz.size(); i++) {
			if ((int) LongLongToStr(DopandAnz[i]).length() > anz_space) anz_space = LongLongToStr(DopandAnz[i]).length();
		}
		output << s_offset << sub_offset << setw(KMCOUT_TSETTINGS_CONCENTRATION.front().length() + 1) << " ";
		output << setw(ID_space) << KMCOUT_TSETTINGS_DOPING << " ";
		output << setw(conc_space) << KMCOUT_TSETTINGS_CONC << " ";
		output << setw(anz_space) << KMCOUT_TSETTINGS_ANZ << endl;
		for (int i = 0; i < (int) DopandAnz.size(); i++) {
			output << s_offset << sub_offset << KMCOUT_TSETTINGS_CONCENTRATION << " ";
			output << setw(ID_space) << i << " ";
			output << setw(conc_space) << DopandConc[i] << " ";
			output << setw(anz_space) << DopandAnz[i] << endl;
		}
		output << endl;
	}

	// Ausgabe der Optionen zur Simulationssteuerung
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_DOPRERUN << " ";
	if (DoPrerun == true) {
		output << KMCOUT_TSETTINGS_DO_TRUE << endl;
	} else {
		output << KMCOUT_TSETTINGS_DO_FALSE << endl;
	}
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_PREMCSP << " " << PreMCSP << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_PRERECORDANZ << " " << PreRecordAnz << endl;
	output << endl;

	output << s_offset << sub_offset << KMCOUT_TSETTINGS_DODYNNORM << " ";
	if (DoDynNorm == true) {
		output << KMCOUT_TSETTINGS_DO_TRUE << endl;
	} else {
		output << KMCOUT_TSETTINGS_DO_FALSE << endl;
	}
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_DYNATTEMPTANZ << " " << DynAttemptAnz << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_DYNRECORDANZ << " " << DynRecordAnz << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_DYNNORMNUM << " " << DynNormNum << endl;
	output << endl;

	output << s_offset << sub_offset << KMCOUT_TSETTINGS_MAINMCSP << " " << MCSP << endl;
	output << s_offset << sub_offset << KMCOUT_TSETTINGS_MAINRECORDANZ << " " << MainRecordAnz << endl;
	
	output << s_offset << KMCOUT_TSETTINGS_END << endl;
	
	if (output.fail() == true) {
		cout << "Critical Error: Output stream failure (in TSettingsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Settings aus einem Stream laden
int TSettingsBase::LoadFromStream (istream &input) {

	// Input pruefen
	if (input.good() == false) {
		cout << "Critical Error: Input stream not ready (in TSettingsBase::LoadFromStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	double i_Temperature = Temperature;			
	double i_AttemptFrequency = AttemptFrequency;
	bool i_WriteCheckpoint = WriteCheckpoint;
	bool i_LoadCheckpoint = LoadCheckpoint;
	double i_EFieldMagnitude = EFieldMagnitude;	
	T3DVector i_EField = EField;			
	int i_LatticeSize = LatticeSize;
	long long i_AdditionalVacAnz = AdditionalVacAnz;							
	vector<double> i_DopandConc = DopandConc;		
	bool i_DoPrerun = DoPrerun;				
	long long i_PreMCSP = PreMCSP;			
	int i_PreRecordAnz = PreRecordAnz;		
	bool i_DoDynNorm = DoDynNorm;			
	long long i_DynAttemptAnz = DynAttemptAnz;			
	int i_DynRecordAnz = DynRecordAnz;			
	int i_DynNormNum = DynNormNum;	
	long long i_MCSP = MCSP;				
	int i_MainRecordAnz = MainRecordAnz;
	string line = "";
	bool if_failed = false;
	string s_temp = "";
	int i_temp = 0;
	double d_temp = 0;
	long long ll_temp = 0;
	while (input.good() == true) {
		// Zeile laden
		if (getline(input,line).fail() == true) {
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream (line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TSETTINGS_TEMPERATURE) {
			if ((linestream >> d_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_Temperature = d_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_LATTICESIZE) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_LatticeSize = i_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_ATTEMPTFREQUENCY) {
			if ((linestream >> d_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_AttemptFrequency = d_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_ADDVACANZ) {
			if ((linestream >> ll_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_AdditionalVacAnz = ll_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_EFIELDSTRENGTH) {
			if ((linestream >> d_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_EFieldMagnitude = d_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_EFIELD) {
			if ((linestream >> s_temp).fail() == true) {	// "("
				if_failed = true;
				break;
			}
			i_EField.Set(0.0, 0.0, 0.0);
			if ((linestream >> i_EField.x).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_EField.y).fail() == true) {
				if_failed = true;
				break;
			}
			if ((linestream >> i_EField.z).fail() == true) {
				if_failed = true;
				break;
			}
		} else if (s_temp == KMCOUT_TSETTINGS_CONCENTRATION) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp != (int) i_DopandConc.size()) {
				if_failed = true;
				break;
			}
			if ((linestream >> d_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_DopandConc.push_back(d_temp);
		} else if (s_temp == KMCOUT_TSETTINGS_WRITECHK) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TSETTINGS_DO_TRUE) {
				i_WriteCheckpoint = true;
			} else {
				i_WriteCheckpoint = false;
			}
		} else if (s_temp == KMCOUT_TSETTINGS_LOADCHK) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TSETTINGS_DO_TRUE) {
				i_LoadCheckpoint = true;
			} else {
				i_LoadCheckpoint = false;
			}
		} else if (s_temp == KMCOUT_TSETTINGS_DOPRERUN) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TSETTINGS_DO_TRUE) {
				i_DoPrerun = true;
			} else {
				i_DoPrerun = false;
			}
		} else if (s_temp == KMCOUT_TSETTINGS_PREMCSP) {
			if ((linestream >> ll_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_PreMCSP = ll_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_PRERECORDANZ) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_PreRecordAnz = i_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_DODYNNORM) {
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_TSETTINGS_DO_TRUE) {
				i_DoDynNorm = true;
			} else {
				i_DoDynNorm = false;
			}
		} else if (s_temp == KMCOUT_TSETTINGS_DYNATTEMPTANZ) {
			if ((linestream >> ll_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_DynAttemptAnz = ll_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_DYNRECORDANZ) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_DynRecordAnz = i_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_DYNNORMNUM) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_DynNormNum = i_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_MAINMCSP) {
			if ((linestream >> ll_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_MCSP = ll_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_MAINRECORDANZ) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_MainRecordAnz = i_temp;
		} else if (s_temp == KMCOUT_TSETTINGS_END) break;
	}
	if (if_failed == true) {
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Bisherigen Input zwischenspeichern und loeschen
	double t_Temperature = Temperature;
	Temperature = 300;
	double t_AttemptFrequency = AttemptFrequency;
	AttemptFrequency = 1.0E+13;
	bool t_WriteCheckpoint = WriteCheckpoint;
	WriteCheckpoint = true;
	bool t_LoadCheckpoint = LoadCheckpoint;
	LoadCheckpoint = true;
	double t_EFieldMagnitude = EFieldMagnitude;
	EFieldMagnitude = 0.1;
	T3DVector t_EFieldDirection = EFieldDirection;
	EFieldDirection.Set(1.0, 0.0, 0.0);
	T3DVector t_EField = EField;
	EField.Set(0.0, 0.0, 0.0);
	int t_LatticeSize = LatticeSize;
	LatticeSize = 16;
	long long t_AdditionalVacAnz = AdditionalVacAnz;
	AdditionalVacAnz = 0;
	double t_TotalVacConc = TotalVacConc;
	TotalVacConc = 0;
	long long t_TotalVacAnz = TotalVacAnz;
	TotalVacAnz = 0;
	double t_MovVolConc = MovVolConc;
	MovVolConc = 0;
	vector<double> t_DopandConc (DopandConc);
	DopandConc.clear();
	vector<long long> t_DopandAnz (DopandAnz);
	DopandAnz.clear();
	bool t_DoPrerun = DoPrerun;
	DoPrerun = false;
	long long t_PreMCSP = PreMCSP;
	PreMCSP = 0;
	int t_PreRecordAnz = PreRecordAnz;
	PreRecordAnz = 0;
	bool t_DoDynNorm = DoDynNorm;
	DoDynNorm = false;
	long long t_DynAttemptAnz = DynAttemptAnz;
	DynAttemptAnz = 0;
	int t_DynRecordAnz = DynRecordAnz;
	DynRecordAnz = 0;
	int t_DynNormNum = DynNormNum;
	DynNormNum = 1;
	long long t_MCSP = MCSP;
	MCSP = 10000;
	int t_MainRecordAnz = MainRecordAnz;
	MainRecordAnz = 0;
	bool t_Ready = Ready;
	Ready = false;

	// Daten laden
	int ErrorCode = KMCERR_OK;
	if_failed = false;
	ErrorCode = SetTemperature(i_Temperature);
	if (ErrorCode != KMCERR_OK) if_failed = true;
	if (if_failed == false) {
		ErrorCode = SetAttemptFrequency(i_AttemptFrequency);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = SetEField(i_EField.x, i_EField.y, i_EField.z, i_EFieldMagnitude);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = SetLatticeSize(i_LatticeSize);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = SetCheckpointFlags(i_WriteCheckpoint, i_LoadCheckpoint);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = SetAdditionalVacAnz(i_AdditionalVacAnz);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		if (i_DopandConc.size() != 0) {
			for (int i = 0; i < (int) i_DopandConc.size(); i++) {
				ErrorCode = SetDopandConcentration(i, i_DopandConc[i]);
				if (ErrorCode != KMCERR_OK) {
					if_failed = true;
					break;
				}
			}
		}
	}
	if (if_failed == false) {
		ErrorCode = SetPrerunOptions(i_DoPrerun, i_PreMCSP, i_PreRecordAnz);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = SetDynNormParameters(i_DoDynNorm, i_DynAttemptAnz, i_DynRecordAnz, i_DynNormNum);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = SetMainKMCOptions(i_MCSP, i_MainRecordAnz);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false) {
		ErrorCode = MakeSettings();
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}

	// Falls Fehler aufgetreten ist, alte Daten wiederherstellen und Fehlercode ausgeben
	if (if_failed == true) {
		Temperature = t_Temperature;
		AttemptFrequency = t_AttemptFrequency;
		WriteCheckpoint = t_WriteCheckpoint;
		LoadCheckpoint = t_LoadCheckpoint;
		EFieldMagnitude = t_EFieldMagnitude;
		EFieldDirection = t_EFieldDirection;
		EField = t_EField;
		LatticeSize = t_LatticeSize;
		AdditionalVacAnz = t_AdditionalVacAnz;
		TotalVacConc = t_TotalVacConc;
		TotalVacAnz = t_TotalVacAnz;
		MovVolConc = t_MovVolConc;
		DopandConc = t_DopandConc;
		DopandAnz = t_DopandAnz;
		DoPrerun = t_DoPrerun;
		PreMCSP = t_PreMCSP;
		PreRecordAnz = t_PreRecordAnz;
		DoDynNorm = t_DoDynNorm;
		DynAttemptAnz = t_DynAttemptAnz;
		DynRecordAnz =t_DynRecordAnz;
		DynNormNum = t_DynNormNum;
		MCSP = t_MCSP;
		MainRecordAnz = t_MainRecordAnz;
		Ready = t_Ready;
		return ErrorCode;
	}
	
	return KMCERR_OK;
}

// **************************** PROTECTED ********************************* //

// DopandConc und DopandAnz mit Anzahl der Dotierungen in m_Job->m_Structure synchronisieren
int TSettingsBase::SyncDopandVectors() {

	// Dotierungsanzahl ermitteln
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TSettingsBase::SyncDopandVectors)" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure == NULL) {
		cout << "Critical Error: m_Structure is null pointer (in TSettingsBase::SyncDopandVectors)" << endl << endl;
		Ready = false;
		return KMCERR_INVALID_POINTER;
	}
	int DopAnz = 0;
	int ErrorCode = m_Job->m_Structure->GetDopingCount(DopAnz);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// DopandConc.size anpassen
	while (DopAnz < (int) DopandConc.size()) {
		DopandConc.pop_back();
		Ready = false;
	}
	while (DopAnz > (int) DopandConc.size()) {
		DopandConc.push_back(0.0);
		Ready = false;
	}

	// DopandAnz.size anpassen
	while (DopandConc.size() < DopandAnz.size()) {
		DopandAnz.pop_back();
		Ready = false;
	}
	while (DopandConc.size() > DopandAnz.size()) {
		DopandAnz.push_back(0);
		Ready = false;
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //

// E-Feld berechnen
int TSettingsBase::CalculateEField() {

	// Allgemein: Beitrag zur Aktivierungsenergie = Arbeit = Ladung * Skalarprodukt(E-Feld-Vektor * Sprungvektor)
	// Maximaler Energiebeitrag: EFieldMagnitude [kT] -> max,abs(Skalarprodukt(E-Feld-Vektor * Sprungvektor)) = EFieldMagnitude * kB * Temperature/abs(Ladung)
	// Einführung des konstanten, positiven Skalierungsfaktors fuers E-Feld: 
	//      max,abs(Skalarprodukt((Skalierung * E-Feld-Einheitsrichtungsvektor) * Sprungvektor)) = EFieldMagnitude * kB * Temperature/abs(Ladung)
	// ->   max,abs(Skalierung * Skalarprodukt(E-Feld-Einheitsrichtungsvektor * Sprungvektor)) = EFieldMagnitude * kB * Temperature/abs(Ladung)
	// ->   Skalierung * max,abs(Skalarprodukt(E-Feld-Einheitsrichtungsvektor * Sprungvektor)) = EFieldMagnitude * kB * Temperature/abs(Ladung)
	// ->   Skalierung = EFieldMagnitude * kB * Temperature/(max,abs(Skalarprodukt(E-Feld-Einheitsrichtungsvektor * Sprungvektor)) * abs(Ladung))

	// E-Feld = 0 pruefen
	if (EFieldMagnitude < T3DVector::zero_threshold) {
		EField.Set(0.0, 0.0, 0.0);
		return KMCERR_OK;
	}

	// Ladung der beweglichen Spezies abrufen
	int ErrorCode = KMCERR_OK;
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TSettingsBase::CalculateEField)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL) {
		cout << "Critical Error: m_Elements is null pointer (in TSettingsBase::CalculateEField)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() != true) {
		cout << "Critical Error: TElements not ready (in TSettingsBase::CalculateEField)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	double mov_charge = 0.0;
	ErrorCode = m_Job->m_Elements->GetMovCharge(mov_charge);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: Cannot retrieve charge (in TSettingsBase::CalculateEField)" << endl << endl;
		return ErrorCode;
	}

	// MovCharge = 0 pruefen
	if (abs(mov_charge) < T3DVector::zero_threshold) {
		EField.Set(0.0, 0.0, 0.0);
		return KMCERR_OK;
	}

	// Skalierung berechnen
	if (m_Job->m_Jumps == NULL) {
		cout << "Critical Error: m_Jumps is null pointer (in TSettingsBase::CalculateEField)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Jumps->IfReady() != true) {
		cout << "Critical Error: TJumps not ready (in TSettingsBase::CalculateEField)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	double max_proj = 0.0;
	ErrorCode = m_Job->m_Jumps->GetMaxEFieldProjection(EFieldDirection, max_proj);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: Cannot retrieve maximum projection (in TSettingsBase::CalculateEField)" << endl << endl;
		return ErrorCode;
	}
	if (max_proj == 0.0) {
		EField.Set(0.0, 0.0, 0.0);
		return KMCERR_OK;
	}
	double efield_scale = EFieldMagnitude * NATCONST_KB * Temperature / (max_proj * abs(mov_charge));

	// E-Feld-Vektor berechnen
	EField = EFieldDirection * efield_scale;

	return KMCERR_OK;
}

// Dopandenkonzentrationen validieren und ihre Anzahlen berechnen
int TSettingsBase::CalculateDopands() {

	// Temporaere, zu berechnende Daten
	double i_TotalVacConc = 0;
	long long i_TotalVacAnz = 0LL;
	double i_MovVolConc = 0;
	vector<long long> i_DopandAnz;
	vector<double> i_DopandConc (DopandConc);

	// Pointer überprüfen
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TSettingsBase::CalculateDopands)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL) {
		cout << "Critical Error: m_Elements is null pointer (in TSettingsBase::CalculateDopands)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() != true) {
		cout << "Critical Error: TElements not ready (in TSettingsBase::CalculateDopands)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_Structure == NULL) {
		cout << "Critical Error: m_Structure is null pointer (in TSettingsBase::CalculateDopands)" << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Structure->IfReady() != true) {
		cout << "Critical Error: TStructure not ready (in TSettingsBase::CalculateDopands)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Dotierungsanzahl ermitteln und DopandConc korrigieren
	int t_DopCount = 0;
	int ErrorCode = m_Job->m_Structure->GetDopingCount(t_DopCount);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	while (t_DopCount < (int) i_DopandConc.size()) {
		i_DopandConc.pop_back();
	}
	while (t_DopCount > (int) i_DopandConc.size()) {
		i_DopandConc.push_back(0);
	}
	i_DopandAnz = vector<long long> (t_DopCount, 0);

	// Anzahl der Atome jedes Elements im undotierten Gitter berechnen
	int t_ElemCount = 0;
	int t_AtomCount = 0;
	ErrorCode = m_Job->m_Elements->GetElementCount(t_ElemCount);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	ErrorCode = m_Job->m_Structure->GetAtomCount(t_AtomCount);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	vector<long long> t_ElemAnz (t_ElemCount, 0LL);	// = Atome im undotierten Gitter
	int t_ElemID = -1;
	for (int i = 0; i < t_AtomCount; i++) {
		t_ElemID = m_Job->m_Structure->GetElemID(i);
		if ((t_ElemID < 0) || (t_ElemID >= t_ElemCount)) {
			cout << "Critical Error: Invalid ElemID index (in TSettingsBase:CalculateDopands)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
		}
		t_ElemAnz[t_ElemID] += LatticeSize * LatticeSize * LatticeSize;
	}
	vector<long long> t_DopedElemAnz (t_ElemAnz);  // = Atome im dotierten Gitter OHNE die Dopanden

	// Zusaetzliche Leerstellen einberechnen
	if (t_ElemCount < 1) {
		cout << "Critical Error: Invalid number of elements (in TSettingsBase:CalculateDopands)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	t_DopedElemAnz[0] -= AdditionalVacAnz;
	
	// Dotierungen einberechnen (inkl. Dotierungen neu berechnen)
	int t_DopedID = -1;
	int t_DopandID = -1;
	double t_DopRatio = 0;
	if (t_DopCount > 0) {
		for (int i = 0; i < t_DopCount; i++) {
			ErrorCode = m_Job->m_Structure->GetDoping(i,t_DopedID,t_DopandID,t_DopRatio);
			if (ErrorCode != KMCERR_OK) return ErrorCode;
			i_DopandAnz[i] = static_cast<long long>(t_ElemAnz[t_DopedID] * i_DopandConc[i] + 0.5);
			i_DopandConc[i] = double(i_DopandAnz[i])/double(t_ElemAnz[t_DopedID]);
			t_DopedElemAnz[t_DopedID] -= i_DopandAnz[i];
			t_DopedElemAnz[0] -= static_cast<long long>(i_DopandAnz[i] * t_DopRatio + 0.5);
		}
	}

	// Validierung
	for (int i = 0; i < t_ElemCount; i++) {
		if ((t_DopedElemAnz[i] < 0LL) || (t_DopedElemAnz[i] > t_ElemAnz[i])) {
			cout << "Error: Too extreme dopand concentrations, vacancy ratios and/or number of additional vacancies." << endl;
			cout << "       Number of dopands or vacancies exceeds the capacity of the lattice." << endl << endl;
			return KMCERR_INVALID_INPUT;
		}
	}
	if (t_DopedElemAnz[0] == 0LL) {
		cout << "Error: Too high dopand concentrations, vacancy ratios and/or number of additional vacancies." << endl;
		cout << "       There are no moving atoms in the lattice." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (t_DopedElemAnz[0] == t_ElemAnz[0]) {
		cout << "Error: Too low dopand concentrations, vacancy ratios and/or number of additional vacancies." << endl;
		cout << "       There are no vacancies in the lattice." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Leerstellengesamtanzahl und -konzentration berechnen
	i_TotalVacAnz = t_ElemAnz[0] - t_DopedElemAnz[0];
	i_TotalVacConc = double(i_TotalVacAnz)/double(t_ElemAnz[0]);

	// Volumenkonzentration der beweglichen Spezies berechnen
	double uc_volume = 0;
	ErrorCode = m_Job->m_Structure->GetUCVolume(uc_volume);
	if (ErrorCode != KMCERR_OK) {
		cout << "Critical Error: Unit cell volume calculation failed (in TSettingsBase::CalculateDopands)" << endl << endl;
		return ErrorCode;
	}
	i_MovVolConc = double(t_DopedElemAnz[0])/(uc_volume * LatticeSize * LatticeSize * LatticeSize * 1.0E-24);

	// Temporaere Daten uebertragen
	TotalVacAnz = i_TotalVacAnz;
	TotalVacConc = i_TotalVacConc;
	MovVolConc = i_MovVolConc;
	DopandAnz = i_DopandAnz;
	DopandConc = i_DopandConc;

	return KMCERR_OK;
}