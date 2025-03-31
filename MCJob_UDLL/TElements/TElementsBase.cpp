// **************************************************************** //
//																	//
//	Klasse: TElementsBase	(TElements Layer 0)						//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der verwendeten Elemente/Spezies		//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TElementsBase.h"

// Includes:
#include <iomanip>
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElementsDesc.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TElementsBase::TElementsBase (TKMCJob * pJob) {
	m_Job = pJob;

	Ready = false;
	MovSymbol = KMCVAR_MOVSYMBOL;
	MovName = KMCVAR_MOVNAME;
	MovCharge = KMCVAR_MOVCHARGE;
	MovRadius = 1.0;
	MovColorR = 1.0;
	MovColorG = 1.0;
	MovColorB = 1.0;
	VacSymbol = KMCVAR_VACSYMBOL;
	VacName = KMCVAR_VACNAME;
	VacCharge = KMCVAR_VACCHARGE;
	VacRadius = 1.0;
	VacColorR = 1.0;
	VacColorG = 1.0;
	VacColorB = 1.0;
	Symbol.clear();
	Name.clear();
	Charge.clear();
	Radius.clear();
	ColorR.clear();
	ColorG.clear();
	ColorB.clear();

	if (m_Job != NULL) {
		if (m_Job->m_ElementsDesc != NULL) {
			if (m_Job->m_ElementsDesc->IfReady() == true) {
				m_Job->m_ElementsDesc->GetElementDesc(MovSymbol,MovRadius,MovColorR,MovColorG,MovColorB);
				m_Job->m_ElementsDesc->GetElementDesc(VacSymbol,VacRadius,VacColorR,VacColorG,VacColorB);
			}
		}
	}
}

// Destructor
TElementsBase::~TElementsBase () {
	m_Job = 0;
}
		
// **************************** PUBLISHED ********************************* //

// Element falls noch nicht vorhanden hinzufügen, nur bei Ready == false moeglich
int TElementsBase::AddElement (string i_symbol, string i_name, double i_charge) {
	if (Ready != false) {
		cout << "Critical Error: TElements ready too early (in TElementsBase::AddElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Pruefung, ob maximale Anzahl an Elementen definiert
	if ((int) Symbol.size() >= KMCLIM_MAX_ELEMENTS) {
		cout << "Error: Too many elements" << endl;
		cout << "       Input: " << Trim(i_symbol) << ", " << Trim(i_name) << ", " << i_charge << endl << endl;
		cout << "       Maximum number of elements: " << KMCLIM_MAX_ELEMENTS << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}

	// Tests auf korrekte Eingabe
	if (StringCheck(i_symbol,KMCVAL_TELEMENTS_SYMBOL_MINLENGTH,KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH,KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS) == false) {
		cout << "Error: Invalid element symbol: " << Trim(i_symbol) << endl;
		cout << "       Symbol must have " << KMCVAL_TELEMENTS_SYMBOL_MINLENGTH << " to " << KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (StringCheck(i_name,KMCVAL_TELEMENTS_NAME_MINLENGTH,KMCVAL_TELEMENTS_NAME_MAXLENGTH,KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS) == false) {
		cout << "Error: Invalid optional element name: " << Trim(i_name) << endl;
		cout << "       Name must have " << KMCVAL_TELEMENTS_NAME_MINLENGTH << " to " << KMCVAL_TELEMENTS_NAME_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_charge < KMCVAL_TELEMENTS_CHARGE_MIN) || (i_charge > KMCVAL_TELEMENTS_CHARGE_MAX)) {
		cout << "Error: Invalid element charge: " << i_charge << endl;
		cout << "       Charge has to be a rational number in the interval [" << KMCVAL_TELEMENTS_CHARGE_MIN << " , " << KMCVAL_TELEMENTS_CHARGE_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Test, ob Eingabe schon vorhanden
	if ((int) Symbol.size() != 0) {
		for (int i = 0; i < (int) Symbol.size(); i++) {
			if ((Trim(i_symbol) == Symbol[i]) &&
				(Trim(i_name) == Name[i]) &&
				(i_charge > Charge[i] - KMCVAR_EQTHRESHOLD_CHARGE) &&
				(i_charge < Charge[i] + KMCVAR_EQTHRESHOLD_CHARGE)) {
					return KMCERR_OK;
			}
		}
	}

	// Zusaetzliche Beschreibung ermitteln
	double i_radius = 1.0;
	double i_red = 0.0;
	double i_green = 0.0;
	double i_blue = 0.0;
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TElementsBase::AddElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_ElementsDesc == NULL) {
		cout << "Critical Error: m_ElementsDesc is null pointer (in TElementsBase::AddElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_ElementsDesc->IfReady() == false) {
		cout << "Critical Error: TElementsDesc not ready (in TElementsBase::AddElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if (m_Job->m_ElementsDesc->GetElementDesc(Trim(i_symbol),i_radius,i_red,i_green,i_blue) != KMCERR_OK) {
		cout << "Critical Error: Cannot get element description (in TElementsBase::AddElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Eingabe hinzufuegen
	Symbol.push_back(Trim(i_symbol));
	Name.push_back(Trim(i_name));
	Charge.push_back(i_charge);
	Radius.push_back(i_radius);
	ColorR.push_back(i_red);
	ColorG.push_back(i_green);
	ColorB.push_back(i_blue);

	return KMCERR_OK;
}

// Element als bewegliche Spezies setzen
int TElementsBase::SetMovingElement (string i_symbol, string i_name, double i_charge) {
	if (Ready != false) {
		cout << "Critical Error: TElements ready too early (in TElementsBase::SetMovingElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// MovingSpecies falls noch nicht erfolgt, hinzufuegen (gleichzeitig Format-Test)
	int ErrorCode = AddElement(i_symbol,i_name,i_charge);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Vacancy falls noch nicht erfolgt, hinzufuegen
	ErrorCode = AddElement(KMCVAR_VACSYMBOL,KMCVAR_VACNAME,KMCVAR_VACCHARGE);
	if (ErrorCode != KMCERR_OK) return ErrorCode;

	// Pointertests
	if (m_Job == NULL) {
		cout << "Critical Error: m_Job is null pointer (in TElementsBase::SetMovingElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_ElementsDesc == NULL) {
		cout << "Critical Error: m_ElementsDesc is null pointer (in TElementsBase::SetMovingElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_ElementsDesc->IfReady() == false) {
		cout << "Critical Error: TElementsDesc not ready (in TElementsBase::SetMovingElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Zusaetzliche Mov-Beschreibung ermitteln
	double i_movradius = 1.0;
	double i_movred = 0.0;
	double i_movgreen = 0.0;
	double i_movblue = 0.0;
	if (m_Job->m_ElementsDesc->GetElementDesc(Trim(i_symbol),i_movradius,i_movred,i_movgreen,i_movblue) != KMCERR_OK) {
		cout << "Critical Error: Cannot get moving element description (in TElementsBase::SetMovingElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Zusaetzliche Vac-Beschreibung ermitteln
	double i_vacradius = 1.0;
	double i_vacred = 0.0;
	double i_vacgreen = 0.0;
	double i_vacblue = 0.0;
	if (m_Job->m_ElementsDesc->GetElementDesc(KMCVAR_VACSYMBOL,i_vacradius,i_vacred,i_vacgreen,i_vacblue) != KMCERR_OK) {
		cout << "Critical Error: Cannot get vacancy element description (in TElementsBase::SetMovingElement)" << endl;
		cout << "                Input: " << Trim(i_symbol) << " " << Trim(i_name) << " " << i_charge << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// MovingSpecies setzen
	MovSymbol = Trim(i_symbol);
	MovName = Trim(i_name);
	MovCharge = i_charge;
	MovRadius = i_movradius;
	MovColorR = i_movred;
	MovColorG = i_movgreen;
	MovColorB = i_movblue;

	// Vacancy setzen
	VacSymbol = KMCVAR_VACSYMBOL;
	VacName = KMCVAR_VACNAME;
	VacCharge = KMCVAR_VACCHARGE;
	VacRadius = i_vacradius;
	VacColorR = i_vacred;
	VacColorG = i_vacgreen;
	VacColorB = i_vacblue;

	return KMCERR_OK;
}

// Elemente sortieren und Ready = true setzen, nur bei Ready == false moeglich
int TElementsBase::MakeElements () {
	if (Ready != false) {
		cout << "Critical Error: TElements ready too early (in TElementsBase::MakeElements)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Ueberpruefen auf Konsistenz
	if (MovSymbol.length() == 0) {
		cout << "Critical Error: No moving species defined (in TElementsBase::MakeElements)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (VacSymbol.length() == 0) {
		cout << "Critical Error: No vacancy element defined (in TElementsBase::MakeElements)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if ((Symbol.size() != Name.size()) ||
		(Symbol.size() != Charge.size()) ||
		(Symbol.size() != Radius.size()) ||
		(Symbol.size() != ColorR.size()) ||
		(Symbol.size() != ColorG.size()) ||
		(Symbol.size() != ColorB.size())) {
			cout << "Critical Error: Unequal number of symbols, names, charges, etc. (in TElementsBase::MakeElements)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if ((int) Symbol.size() <= 1) {
		cout << "Critical Error: Too few elements defined (in TElementsBase::MakeElements)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}	
	
	// MovingSpecies auf ElemID = 0 setzen, Vacancy auf ElemID = 1 setzen
	string s_swaphelp;
	double d_swaphelp;
	for (int i = 0; i < (int) Symbol.size(); i++) {
		if ((MovSymbol == Symbol[i]) &&
			(MovName == Name[i]) &&
			(MovCharge > Charge[i] - KMCVAR_EQTHRESHOLD_CHARGE) &&
			(MovCharge < Charge[i] + KMCVAR_EQTHRESHOLD_CHARGE)) {
				s_swaphelp = Symbol[i];
				Symbol[i] = Symbol[0];
				Symbol[0] = s_swaphelp;
				s_swaphelp = Name[i];
				Name[i] = Name[0];
				Name[0] = s_swaphelp;
				d_swaphelp = Charge[i];
				Charge[i] = Charge[0];
				Charge[0] = d_swaphelp;
				d_swaphelp = Radius[i];
				Radius[i] = Radius[0];
				Radius[0] = d_swaphelp;
				d_swaphelp = ColorR[i];
				ColorR[i] = ColorR[0];
				ColorR[0] = d_swaphelp;
				d_swaphelp = ColorG[i];
				ColorG[i] = ColorG[0];
				ColorG[0] = d_swaphelp;
				d_swaphelp = ColorB[i];
				ColorB[i] = ColorB[0];
				ColorB[0] = d_swaphelp;
		}
		if ((VacSymbol == Symbol[i]) &&
			(VacName == Name[i]) &&
			(VacCharge > Charge[i] - KMCVAR_EQTHRESHOLD_CHARGE) &&
			(VacCharge < Charge[i] + KMCVAR_EQTHRESHOLD_CHARGE)) {
				s_swaphelp = Symbol[i];
				Symbol[i] = Symbol[1];
				Symbol[1] = s_swaphelp;
				s_swaphelp = Name[i];
				Name[i] = Name[1];
				Name[1] = s_swaphelp;
				d_swaphelp = Charge[i];
				Charge[i] = Charge[1];
				Charge[1] = d_swaphelp;
				d_swaphelp = Radius[i];
				Radius[i] = Radius[1];
				Radius[1] = d_swaphelp;
				d_swaphelp = ColorR[i];
				ColorR[i] = ColorR[1];
				ColorR[1] = d_swaphelp;
				d_swaphelp = ColorG[i];
				ColorG[i] = ColorG[1];
				ColorG[1] = d_swaphelp;
				d_swaphelp = ColorB[i];
				ColorB[i] = ColorB[1];
				ColorB[1] = d_swaphelp;
		}
	}
	if ((MovSymbol != Symbol[0]) ||	(MovName != Name[0]) ||
		(MovCharge < Charge[0] - KMCVAR_EQTHRESHOLD_CHARGE) ||
		(MovCharge > Charge[0] + KMCVAR_EQTHRESHOLD_CHARGE)) {
			cout << "Critical Error: Moving species not in list of elements (in TElementsBase::MakeElements)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if ((VacSymbol != Symbol[1]) ||	(VacName != Name[1]) ||
		(VacCharge < Charge[1] - KMCVAR_EQTHRESHOLD_CHARGE) ||
		(VacCharge > Charge[1] + KMCVAR_EQTHRESHOLD_CHARGE)) {
			cout << "Critical Error: Vacancy element not in list of elements (in TElementsBase::MakeElements)" << endl << endl;
			return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}

	// Elemente sortieren (BubbleSort)
	if ((int) Symbol.size() > 3) {
		for (int i = (int) Symbol.size(); i > 3; i--) {
			for (int j = 2; j < i - 1; j++) {
				if ((Symbol[j] > Symbol[j + 1]) ||
					((Symbol[j] == Symbol[j + 1]) && (Name[j] > Name[j + 1])) ||
					((Symbol[j] == Symbol[j + 1]) && (Name[j] == Name[j + 1]) && (Charge[j] > Charge[j + 1]))) {
					s_swaphelp = Symbol[j];
					Symbol[j] = Symbol[j + 1];
					Symbol[j + 1] = s_swaphelp;
					s_swaphelp = Name[j];
					Name[j] = Name[j + 1];
					Name[j + 1] = s_swaphelp;
					d_swaphelp = Charge[j];
					Charge[j] = Charge[j + 1];
					Charge[j + 1] = d_swaphelp;
					d_swaphelp = Radius[j];
					Radius[j] = Radius[j + 1];
					Radius[j + 1] = d_swaphelp;
					d_swaphelp = ColorR[j];
					ColorR[j] = ColorR[j + 1];
					ColorR[j + 1] = d_swaphelp;
					d_swaphelp = ColorG[j];
					ColorG[j] = ColorG[j + 1];
					ColorG[j + 1] = d_swaphelp;
					d_swaphelp = ColorB[j];
					ColorB[j] = ColorB[j + 1];
					ColorB[j + 1] = d_swaphelp;
				}
			}
		}
	}

	Ready = true;
	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //

// Rueckgabe von Ready
bool TElementsBase::IfReady () {
	return Ready;
}

// Elemente in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
int TElementsBase::SaveToStream (ostream &output, int offset) {
	if (Ready != true) {
		cout << "Critical Error: TElements not ready (in TElementsBase::SaveToStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true) {
		cout << "Critical Error: Output stream not ready (in TElementsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET)) {
		cout << "Critical Error: Offset out of bounds (in TElementsBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset,' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int) KMCOUT_TELEMENTS_OFFSET > 0) sub_offset = string((int) KMCOUT_TELEMENTS_OFFSET,' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);
	int ID_space = IntToStr(((int) Symbol.size()) - 1).length();
	if (ID_space < (int) ((string) KMCOUT_TELEMENTS_ELEMID).length()) ID_space = ((string) KMCOUT_TELEMENTS_ELEMID).length();
	int Sym_space = ((string) KMCOUT_TELEMENTS_SYMBOL).length();
	if (Sym_space < (int) ((string) KMCOUT_NOSTRING).length()) Sym_space = ((string) KMCOUT_NOSTRING).length();
	int Name_space = ((string) KMCOUT_TELEMENTS_NAME).length();
	if (Name_space < (int) ((string) KMCOUT_NOSTRING).length()) Name_space = ((string) KMCOUT_NOSTRING).length();
	for (int i = 0; i < (int) Symbol.size(); i++) {
		if ((int) Symbol[i].length() > Sym_space) Sym_space = Symbol[i].length();
		if ((int) Name[i].length() > Name_space) Name_space = Name[i].length();
	}
	int Charge_space = (int) output.precision() + 7;
	if (Charge_space < (int) ((string) KMCOUT_TELEMENTS_CHARGE).length()) Charge_space = ((string) KMCOUT_TELEMENTS_CHARGE).length();
	
	// Ausgabe der Erkennungszeile und der Kopfzeile
	output << s_offset << KMCOUT_TELEMENTS_START << endl;
	output << s_offset << sub_offset << setw(((string) KMCOUT_TELEMENTS_ELEMENT).length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TELEMENTS_ELEMID << " ";
	output << setw(Sym_space) << KMCOUT_TELEMENTS_SYMBOL << " ";
	output << setw(Name_space) << KMCOUT_TELEMENTS_NAME << " ";
	output << setw(Charge_space) << KMCOUT_TELEMENTS_CHARGE << endl;

	// Ausgabe der Elemente
	for (int i = 0; i < (int) Symbol.size(); i++) {
		output << s_offset << sub_offset << KMCOUT_TELEMENTS_ELEMENT << " ";
		output << setw(ID_space) << i << " ";
		if (Symbol[i] == "") {
			output << setw(Sym_space) << KMCOUT_NOSTRING << " ";
		} else {
			output << setw(Sym_space) << Symbol[i] << " ";
		}
		if (Name[i] == "") {
			output << setw(Name_space) << KMCOUT_NOSTRING << " ";
		} else {
			output << setw(Name_space) << Name[i] << " ";
		}
		output << setw(Charge_space) << Charge[i];
		if (i == 0) output << " (Mov)";
		if (i == 1) output << " (Vac)";
		output << endl;
	}
	output << s_offset << KMCOUT_TELEMENTS_END << endl;

	if (output.fail() == true) {
		cout << "Critical Error: Output stream failure (in TElementsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Elemente aus einem Stream laden, nur bei Ready == false moeglich
int TElementsBase::LoadFromStream (istream &input) {
	if (Ready != false) {
		cout << "Critical Error: TElements ready too early (in TElementsBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Input pruefen
	if (input.good() == false) {
		cout << "Critical Error: Input stream not ready (in TElementsBase::LoadFromStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	vector<string> i_Symbol;
	vector<string> i_Name;
	vector<double> i_Charge;
	string line = "";
	bool if_failed = false;
	string s_temp = "";
	int i_temp = 0;
	double d_temp = 0;
	while (input.good() == true) {
		// Zeile laden
		if (getline(input,line).fail() == true) {
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream (line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TELEMENTS_ELEMENT) {
			if ((linestream >> i_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (i_temp != (int)i_Symbol.size()) {
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp.length() == 0) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_NOSTRING) {
				i_Symbol.push_back("");
			} else {
				i_Symbol.push_back(s_temp);
			}
			if ((linestream >> s_temp).fail() == true) {
				if_failed = true;
				break;
			}
			if (s_temp.length() == 0) {
				if_failed = true;
				break;
			}
			if (s_temp == KMCOUT_NOSTRING) {
				i_Name.push_back("");
			} else {
				i_Name.push_back(s_temp);
			}
			if ((linestream >> d_temp).fail() == true) {
				if_failed = true;
				break;
			}
			i_Charge.push_back(d_temp);
		} else if (s_temp == KMCOUT_TELEMENTS_END) break;
	}
	if ((int) i_Symbol.size() == 0) if_failed = true;
	if ((i_Symbol.size() != i_Name.size()) || (i_Symbol.size() != i_Charge.size())) if_failed = true;
	if (if_failed == true) {
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Bisherigen Input zwischenspeichern und loeschen
	string t_MovSymbol = MovSymbol;
	MovSymbol = KMCVAR_MOVSYMBOL;
	string t_MovName = MovName;
	MovName = KMCVAR_MOVNAME;
	double t_MovCharge = MovCharge;
	MovCharge = KMCVAR_MOVCHARGE;
	string t_VacSymbol = VacSymbol;
	VacSymbol = KMCVAR_VACSYMBOL;
	string t_VacName = VacName;
	VacName = KMCVAR_VACNAME;
	double t_VacCharge = VacCharge;
	VacCharge = KMCVAR_VACCHARGE;
	vector<string> t_Symbol (Symbol);
	Symbol.clear();
	vector<string> t_Name (Name);
	Name.clear();
	vector<double> t_Charge (Charge);
	Charge.clear();

	// Daten laden
	int ErrorCode = KMCERR_OK;
	if_failed = false;
	for (int i = 0; i < (int) i_Symbol.size(); i++) {
		ErrorCode = AddElement(i_Symbol[i],i_Name[i],i_Charge[i]);
		if (ErrorCode != KMCERR_OK) {
			if_failed = true;
			break;
		}
	}
	if (if_failed == false) {
		ErrorCode = SetMovingElement(i_Symbol[0],i_Name[0],i_Charge[0]);
		if (ErrorCode != KMCERR_OK) {
			if_failed = true;
		} else {
			ErrorCode = MakeElements();
			if (ErrorCode != KMCERR_OK) if_failed = true;
		}
	}

	// Falls Fehler aufgetreten ist, alte Daten wiederherstellen und Fehlercode ausgeben
	if (if_failed == true) {
		MovSymbol = t_MovSymbol;
		MovName = t_MovName;
		MovCharge = t_MovCharge;
		VacSymbol = t_VacSymbol;
		VacName = t_VacName;
		VacCharge = t_VacCharge;
		Symbol = t_Symbol;
		Name = t_Name;
		Charge = t_Charge;
		Ready = false;
		return ErrorCode;
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //