// **************************************************************** //
//																	//
//	Klasse: TCustomTime												//
//	Autor: Philipp Hein												//
//	Datum: 14.03.2014												//
//  Aufgabe:														//
//    Klasse zur Verwaltung eines Zeitstempels						//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TCustomTime_H
#define TCustomTime_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:

using namespace std;

// Klassendeklaration:
class TCustomTime {

	public:

		// Variablen:
		int year;									// Jahr
		int day;									// Tag seit Jahresbeginn [0,365]
		int hour;									// Stunde [0,23]
		int min;									// Minute [0,59]
		int sec;									// Sekunde [0,59]
		
		// Constructor/Destructor:
		TCustomTime ();																		// Default-Constructor
		TCustomTime (int set_year, int set_day, int set_hour, int set_min, int set_sec);	// Constructor mit Initialisierung
		~TCustomTime ();																	// Destructor

		// Operators
		TCustomTime operator + (const TCustomTime &param) const;	// Additionsoperator
		TCustomTime& operator += (const TCustomTime &param);		// Additionszuweisungsoperator
		TCustomTime operator - (const TCustomTime &param) const;	// Subtraktionsoperator
		TCustomTime& operator -= (const TCustomTime &param);		// Subtraktionszuweisungsoperator
		TCustomTime operator * (double param) const;				// Multiplikation mit Skalar

		// Member functions
		string ToString() const;									// Zeit in string konvertieren
		string ToFullString() const;								// Zeit in string konvertieren (alle Zeitwerte angeben)
		void CheckOverflow();										// Korrigiert Zeitueberlaufe/-unterlaeufe

		// Static member functions
		static TCustomTime GetCurrentTime(bool if_write_time);		// Aktuelle Systemzeit auslesen und evtl. ausgeben
		static string GetCurrentTimeStr();							// Aktuelle Systemzeit ausgeben
		static void PrintCurrentTime();								// Aktuelle Systemzeit ausgeben
};

#endif