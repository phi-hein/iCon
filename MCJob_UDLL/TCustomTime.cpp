// **************************************************************** //
//																	//
//	Class: TCustomTime												//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for a time stamp										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TCustomTime.h"

// Includes:
#include <time.h>
#include <iostream>
#include <sstream>
#include <iomanip>

// Eigene Includes:
#include "GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TCustomTime::TCustomTime() : year(0), day(0), hour(0), min(0), sec(0)
{

}

// Constructor mit Initialisierung
TCustomTime::TCustomTime(int set_year, int set_day, int set_hour, int set_min, int set_sec) :
	year(set_year), day(set_day), hour(set_hour), min(set_min), sec(set_sec)
{

}

// Destructor
TCustomTime::~TCustomTime()
{

}

// Additionsoperator
TCustomTime TCustomTime::operator + (const TCustomTime& param) const
{
	TCustomTime temp = TCustomTime(year + param.year, day + param.day, hour + param.hour, min + param.min, sec + param.sec);
	temp.CheckOverflow();
	return temp;
}

// Additionszuweisungsoperator
TCustomTime& TCustomTime::operator += (const TCustomTime& param)
{
	year += param.year;
	day += param.day;
	hour += param.hour;
	min += param.min;
	sec += param.sec;
	CheckOverflow();
	return *this;
}

// Subtraktionsoperator
TCustomTime TCustomTime::operator - (const TCustomTime& param) const
{
	TCustomTime temp = TCustomTime(year - param.year, day - param.day, hour - param.hour, min - param.min, sec - param.sec);
	temp.CheckOverflow();
	return temp;
}

// Subtraktionszuweisungsoperator
TCustomTime& TCustomTime::operator -= (const TCustomTime& param)
{
	year -= param.year;
	day -= param.day;
	hour -= param.hour;
	min -= param.min;
	sec -= param.sec;
	CheckOverflow();
	return *this;
}

// Multiplikation mit Skalar
TCustomTime TCustomTime::operator * (double param) const
{
	TCustomTime temp = TCustomTime((int)(year * param), (int)(day * param), (int)(hour * param), (int)(min * param), (int)(sec * param));
	temp.CheckOverflow();
	return temp;
}

// ************************* MEMBER FUNCTIONS ****************************** //

// Zeit in string konvertieren
string TCustomTime::ToString() const
{
	bool write_rest = false;
	string temp = "";
	if (year != 0)
	{
		temp += IntToStr(year) + " y, ";
		write_rest = true;
	}
	if ((write_rest == true) || (day != 0))
	{
		temp += IntToStr(day) + " d, ";
		write_rest = true;
	}
	if ((write_rest == true) || (hour != 0))
	{
		temp += IntToStr(hour) + " h, ";
		write_rest = true;
	}
	if ((write_rest == true) || (min != 0))
	{
		temp += IntToStr(min) + " m, ";
		write_rest = true;
	}
	temp += IntToStr(sec) + " s";
	return temp;
}

// Zeit in string konvertieren (alle Zeitwerte angeben)
string TCustomTime::ToFullString() const
{
	string temp = "";
	temp += IntToStr(year) + " y, ";
	temp += IntToStr(day) + " d, ";
	temp += IntToStr(hour) + " h, ";
	temp += IntToStr(min) + " m, ";
	temp += IntToStr(sec) + " s";
	return temp;
}

// Korrigiert Zeitueberlaufe/-unterlaeufe
void TCustomTime::CheckOverflow()
{
	while (sec < 0)
	{
		min -= 1;
		sec += 60;
	}
	while (sec > 59)
	{
		min += 1;
		sec -= 60;
	}
	while (min < 0)
	{
		hour -= 1;
		min += 60;
	}
	while (min > 59)
	{
		hour += 1;
		min -= 60;
	}
	while (hour < 0)
	{
		day -= 1;
		hour += 24;
	}
	while (hour > 23)
	{
		day += 1;
		hour -= 23;
	}
	while (day < 0)
	{
		year -= 1;
		day += 366;
	}
	while (day > 365)
	{
		year += 1;
		day -= 366;
	}
}

// *********************** STATIC MEMBER FUNCTIONS ************************* //

// Aktuelle Systemzeit auslesen und evtl. ausgeben
TCustomTime TCustomTime::GetCurrentTime(bool if_write_time)
{

	TCustomTime temp;
	time_t raw_time = time(NULL);
	if (raw_time == -1) return temp;

	tm current_time;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
	localtime_s(&current_time, &raw_time);
#else
	localtime_r(&raw_time, &current_time);
#endif

	temp.year = current_time.tm_year + 1900;
	temp.day = current_time.tm_yday;
	temp.hour = current_time.tm_hour;
	temp.min = current_time.tm_min;
	temp.sec = current_time.tm_sec;
	temp.CheckOverflow();

	if (if_write_time == true)
	{
		cout << setfill('0') << setw(2) << current_time.tm_mday << ".";
		cout << setw(2) << current_time.tm_mon + 1 << "." << setw(4) << current_time.tm_year + 1900;
		cout << " - " << setw(2) << current_time.tm_hour << ":" << setw(2) << current_time.tm_min << ":";
		cout << setw(2) << current_time.tm_sec << setfill(' ');
	}

	return temp;
}

// Aktuelle Systemzeit ausgeben
string TCustomTime::GetCurrentTimeStr()
{

	time_t raw_time = time(NULL);
	if (raw_time == -1) return "(time error)";

	tm current_time;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
	localtime_s(&current_time, &raw_time);
#else
	localtime_r(&raw_time, &current_time);
#endif

	stringstream t_stream;
	t_stream << setfill('0') << setw(2) << current_time.tm_mday << ".";
	t_stream << setw(2) << current_time.tm_mon + 1 << "." << setw(4) << current_time.tm_year + 1900;
	t_stream << " - " << setw(2) << current_time.tm_hour << ":" << setw(2) << current_time.tm_min << ":";
	t_stream << setw(2) << current_time.tm_sec << setfill(' ');
	t_stream.flush();

	return t_stream.str();
}

// Aktuelle Systemzeit ausgeben
void TCustomTime::PrintCurrentTime()
{

	cout << GetCurrentTimeStr();
}