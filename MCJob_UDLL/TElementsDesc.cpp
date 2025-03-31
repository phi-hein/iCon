// **************************************************************** //
//																	//
//	Klasse: TElementsDesc											//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Klasse zur Beschreibung der Elementdarstellung				//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TElementsDesc.h"

// Includes:
#include <fstream>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif

// Eigene Includes:
#include "GlobalDefinitions.h"
#include "ErrorCodes.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TElementsDesc::TElementsDesc () {
	Initialize();
	LoadFromFile(KMCPATH_ELEMDESC);
	Ready = true;
}

// Destructor
TElementsDesc::~TElementsDesc () {

}	

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Ready-Status ausgeben
bool TElementsDesc::IfReady () {
	return Ready;
}

// Radius (atomar) und Farbe eines Elements ausgeben
int TElementsDesc::GetElementDesc (string input_symbol, double &radius, double &red, double &green, double &blue) {

	// Element suchen
	int desc_index = -1;
	if (Symbol.size() != 0) {
		for (int i = 0; i < (int) Symbol.size(); i++) {
			if (Symbol[i] == input_symbol) {
				desc_index = i;
				break;
			}
		}
	}

	// Beschreibung ausgeben
	if (desc_index < 0) {
		radius = DefaultAtomicRadius;
		red = DefaultRed;
		green = DefaultGreen;
		blue = DefaultBlue;
	} else {
		radius = AtomicRadius[desc_index];
		red = Red[desc_index];
		green = Green[desc_index];
		blue = Blue[desc_index];
	}

	return KMCERR_OK;
}

// ***************************** PROTECTED ********************************** //

// Standardwerte setzen
void TElementsDesc::Initialize() {
	// Default-Element setzen
	DefaultOrdnungszahl = 0;
	DefaultSymbol = KMCVAR_ARBITRARYSYMBOL;
	DefaultAtomicRadius = 0.80;
	DefaultVdWRadius = 1.00;
	DefaultIonicRadius = 0.80;
	DefaultRed = 0.30;
	DefaultGreen = 0.30;
	DefaultBlue = 0.30;

	// Liste loeschen
	Ordnungszahl.clear();
	Symbol.clear();
	AtomicRadius.clear();
	VdWRadius.clear();
	IonicRadius.clear();
	Red.clear();
	Green.clear();
	Blue.clear();
}


// Versuchen, zusaetzliche Elementbeschreibungen aus der Datei zu lesen
void TElementsDesc::LoadFromFile(string Filename) {

	// Moeglichst Umwandeln in absoluten Pfad (verhindert Probleme beim Aendern des Arbeitsverzeichnisses)
	string t_filepath = PrependAppPath(Filename);

	// Datei oeffnen
	ifstream infile;
	infile.open(Trim(t_filepath));
	if (infile.is_open() == false) return;

	// Eingabevariablen
	vector<int> i_Ordnungszahl;
	vector<string> i_Symbol;
	vector<double> i_AtomicRadius;
	vector<double> i_VdWRadius;
	vector<double> i_IonicRadius;
	vector<double> i_Red;
	vector<double> i_Green;
	vector<double> i_Blue;
	
	// Datei einlesen
	bool if_failed = false;
	while (infile.eof() == false) {
		i_Ordnungszahl.push_back(0);
		if ((infile >> i_Ordnungszahl.back()).fail() == true) {
			if_failed = true;
			break;
		}
		i_Symbol.push_back("");
		if ((infile >> i_Symbol.back()).fail() == true) {
			if_failed = true;
			break;
		}
		i_AtomicRadius.push_back(0);
		if ((infile >> i_AtomicRadius.back()).fail() == true) {
			if_failed = true;
			break;
		}
		if (i_AtomicRadius.back() <= 0) {
			if_failed = true;
			break;
		}
		i_VdWRadius.push_back(0);
		if ((infile >> i_VdWRadius.back()).fail() == true) {
			if_failed = true;
			break;
		}
		if (i_VdWRadius.back() <= 0) {
			if_failed = true;
			break;
		}
		i_IonicRadius.push_back(0);
		if ((infile >> i_IonicRadius.back()).fail() == true) {
			if_failed = true;
			break;
		}
		if (i_IonicRadius.back() <= 0) {
			if_failed = true;
			break;
		}
		i_Red.push_back(0);
		if ((infile >> i_Red.back()).fail() == true) {
			if_failed = true;
			break;
		}
		if (i_Red.back() < 0) i_Red.back() = 0;
		if (i_Red.back() > 1) i_Red.back() = 1;
		i_Green.push_back(0);
		if ((infile >> i_Green.back()).fail() == true) {
			if_failed = true;
			break;
		}
		if (i_Green.back() < 0) i_Green.back() = 0;
		if (i_Green.back() > 1) i_Green.back() = 1;
		i_Blue.push_back(0);
		if ((infile >> i_Blue.back()).fail() == true) {
			if_failed = true;
			break;
		}
		if (i_Blue.back() < 0) i_Blue.back() = 0;
		if (i_Blue.back() > 1) i_Blue.back() = 1;
		infile >> ws;
	}
	infile.close();
	if ((if_failed == true) || 
		((int) i_Ordnungszahl.size() == 0) ||
		(i_Symbol.size() != i_Ordnungszahl.size()) ||
		(i_AtomicRadius.size() != i_Ordnungszahl.size()) ||
		(i_VdWRadius.size() != i_Ordnungszahl.size()) ||
		(i_IonicRadius.size() != i_Ordnungszahl.size()) ||
		(i_Red.size() != i_Ordnungszahl.size()) ||
		(i_Green.size() != i_Ordnungszahl.size()) ||
		(i_Blue.size() != i_Ordnungszahl.size())) return;

	// Copy new data
	Ordnungszahl = i_Ordnungszahl;
	Symbol = i_Symbol;
	AtomicRadius = i_AtomicRadius;
	VdWRadius = i_VdWRadius;
	IonicRadius = i_IonicRadius;
	Red = i_Red;
	Green = i_Green;
	Blue = i_Blue;
}