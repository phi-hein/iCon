// **************************************************************** //
//																	//
//	Class: TElementsDesc											//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for providing visualization info for elements			//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef TElementsDesc_H
#define TElementsDesc_H

// Includes:
#include <string>
#include <string_view>
#include <vector>

// Eigene Includes:

using namespace std;

class TElementsDesc
{
	// Member variables
protected:
	int DefaultOrdnungszahl;		// Standard-Ordnungszahl
	string DefaultSymbol;			// Standard-Symbol
	double DefaultAtomicRadius;		// Standard-AtomicRadius
	double DefaultVdWRadius;		// Standard-VdWRadius
	double DefaultIonicRadius;		// Standard-IonicRadius
	double DefaultRed;				// Standard R-Wert von RGB-Color
	double DefaultGreen;			// Standard G-Wert von RGB-Color
	double DefaultBlue;				// Standard B-Wert von RGB-Color
	vector<int> Ordnungszahl;		// Ordnungszahl im PSE, auﬂer fuer Vc
	vector<string> Symbol;			// Elementsymbol, alle aus PSE + Vc (Vacancy)
	vector<double> AtomicRadius;	// Atomarer Radius in Angstroem
	vector<double> VdWRadius;		// Van-der-Waals-Radius in Angstroem
	vector<double> IonicRadius;		// Ionischer Radius in Angstroem
	vector<double> Red;				// R von RGB-Color (0 bis 1)
	vector<double> Green;			// G von RGB-Color (0 bis 1)
	vector<double> Blue;			// B von RGB-Color (0 bis 1)

	bool Ready;						// Flag, ob die Klasse erfolgreich geladen wurde

	// Member functions
public:
	// NON-PUBLISHED
	TElementsDesc();							// Constructor
	~TElementsDesc();							// Destructor
	bool IfReady();							// Ready ausgeben

	int GetElementDesc(string_view input_symbol, double& radius, double& red, double& green, double& blue);		// Radius (atomar) und Farbe eines Elements ausgeben

protected:
	void Initialize();							// Standardwerte setzen
	void LoadFromFile(string_view Filename);	// Versuchen, zusaetzliche Elementbeschreibungen aus der Datei zu lesen
};

#endif