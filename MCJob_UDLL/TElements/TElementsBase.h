// **************************************************************** //
//																	//
//	Class: TElementsBase	(TElements Layer 0)						//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing the used elements							//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TElementsBase_H
#define TElementsBase_H

// Includes:
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

// Eigene Includes:

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TElementsBase
{
	// Member variables
protected:
	TKMCJob* m_Job;			// Pointer zur uebergeordneten KMCJob-Instanz

	string MovSymbol;			// Symbol der MovingSpecies
	string MovName;				// Name der MovingSpecies
	double MovCharge;			// Ladung der MovingSpecies
	double MovRadius;			// Radius der MovingSpecies (in Angstroem)
	double MovColorR;			// Rot-Wert der RGB-Farbe der MovingSpecies aus [0,1]
	double MovColorG;			// Gruen-Wert der RGB-Farbe der MovingSpecies aus [0,1]
	double MovColorB;			// Blau-Wert der RGB-Farbe der MovingSpecies aus [0,1]
	string VacSymbol;			// Symbol der Vacancies
	string VacName;				// Name der Vacancies
	double VacCharge;			// Ladung der Vacancies
	double VacRadius;			// Radius der Vacancies (in Angstroem)
	double VacColorR;			// Rot-Wert der RGB-Farbe der Vacancies aus [0,1]
	double VacColorG;			// Gruen-Wert der RGB-Farbe der Vacancies aus [0,1]
	double VacColorB;			// Blau-Wert der RGB-Farbe der Vacancies aus [0,1]
	vector<string> Symbol;		// Elementsymbol, nur Symbole des PSE + Vc (Vacancy) + Xx (beliebig) erlaubt
	vector<string> Name;		// Elementname, beliebiger Name oder Kuerzel mit max. 20 Zeichen
	vector<double> Charge;		// Ladung, beliebige Ladung zwischen [-1000,+1000], in e
	vector<double> Radius;		// Radius des Elements (in Angstroem)
	vector<double> ColorR;		// Rot-Wert der RGB-Farbe des Elements aus [0,1]
	vector<double> ColorG;		// Gruen-Wert der RGB-Farbe des Elements aus [0,1]
	vector<double> ColorB;		// Blau-Wert der RGB-Farbe des Elements aus [0,1]
	bool Ready;					// Flag, ob Elements sortiert sind = fertig sind

	// Member functions
public:
	// PUBLISHED
	int AddElement(string_view i_symbol, string_view i_name, double i_charge);			// Element falls noch nicht vorhanden hinzufügen, nur bei Ready == false moeglich
	int SetMovingElement(string_view i_symbol, string_view i_name, double i_charge);	// Element als bewegliche Spezies setzen
	int MakeElements();																// Elemente sortieren und Ready = true setzen, nur bei Ready == false moeglich

	// NON-PUBLISHED
	TElementsBase(TKMCJob* pJob);			// Constructor, Ready = false setzen
	bool IfReady();						// Rueckgabe von Ready
	int SaveToStream(ostream& output, int offset);		// Elemente in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
	int LoadFromStream(istream& input);				// Elemente aus einem Stream laden, nur bei Ready == false moeglich

protected:
	~TElementsBase();						// Destructor
};

#endif