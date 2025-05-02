// **************************************************************** //
//																	//
//	GlobalDefinitions												//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Globale Makro-, Funktionen- und Wertdefinitionen				//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierter Header:
#include "GlobalDefinitions.h"

// Includes:
#include <sstream>

// Windows-Includes
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))

#include <Windows.h>

#endif

// Linux-Includes
#if (defined(__linux__) || defined(__linux))

#include <limits.h>
#include <unistd.h>

#endif

// Eigene Includes:
#include "T3DVector.h"

// ************************* Hilfsfunktionen *************************** //

// Funktion zum Trimmen eines string_view (d.h. Leerstellen an Anfang und Ende entfernen)
std::string_view Trim(std::string_view input)
{
	std::string_view::size_type last = input.find_last_not_of(' ');
	if (last != std::string::npos)
	{
		std::string_view::size_type first = input.find_first_not_of(' ');
		
		return input.substr(first, last - first + 1);
	}
	else
	{
		return input.substr(0, 0);
	}
}

// Funktion zum Trimmen eines string_view von der rechten Seite (d.h. Leerstellen am Ende entfernen)
std::string_view RightTrim(std::string_view input)
{
	std::string_view::size_type last = input.find_last_not_of(' ');
	if (last != std::string::npos)
	{
		return input.substr(0, last + 1);
	}
	else
	{
		return input.substr(0, 0);
	}
}

// Funktion zum Ueberpruefen eines string, Laenge aus [min_length,max_length], keine allowed chars = alle erlaubt
bool StringCheck(std::string_view input, int min_length, int max_length, std::string_view allowed_chars)
{
	// Trim
	input = Trim(input);

	// Check length
	if (((int)input.size() < min_length) || ((int)input.size() > max_length)) return false;

	// Check allowed characters
	if ((allowed_chars.size() > 0) && (input.size() > 0))
	{
		if (input.find_first_not_of(allowed_chars) != std::string::npos) return false;
	}

	return true;
}

// Funktion zur Umwandlung eines int in einen string
std::string IntToStr(int input)
{
	std::stringstream out;
	out << input;
	return out.str();
}

// Funktion zur Umwandlung eines long long in einen string
std::string LongLongToStr(long long input)
{
	std::stringstream out;
	out << input;
	return out.str();
}

// Funktion zur Umwandlung eines unsigned long long in einen string
std::string UnsignedLongLongToStr(unsigned long long input)
{
	std::stringstream out;
	out << input;
	return out.str();
}

// Funktion zur Umwandlung eines double in einen string
std::string DoubleToStr(double input)
{
	std::stringstream out;
	out << input;
	return out.str();
}

// Funktion zur Umwandlung eines string in einen int
int StrToInt (string input, int error_val) 
{
	stringstream inp(input);
	int out = 0;
	if ((inp >> out).fail()) return error_val;
	return out;
}

// Funktion zur Umwandlung eines string in einen long long
long long StrToLongLong(std::string input, long long error_val)
{
	std::stringstream inp(input);
	long long out = 0;
	if ((inp >> out).fail()) return error_val;
	return out;
}

// Funktion zur Umwandlung eines string in einen unsigned long long
unsigned long long StrToUnsignedLongLong(std::string input, unsigned long long error_val)
{
	std::stringstream inp(input);
	unsigned long long out = 0;
	if ((inp >> out).fail()) return error_val;
	return out;
}

// Funktion zur Umwandlung eines string in einen double
double StrToDouble(std::string input, double error_val)
{
	std::stringstream inp(input);
	double out = 0.0;
	if ((inp >> out).fail()) return error_val;
	return out;
}

// Funktion zur Umwandlung eines string in einen int
bool ConvStrToInt(std::string input, int& val)
{
	std::stringstream inp(input);
	int out = 0;
	if ((inp >> out).fail()) return false;
	val = out;
	return true;
}

// Funktion zur Umwandlung eines string in einen long long
bool ConvStrToLongLong(std::string input, long long& val)
{
	std::stringstream inp(input);
	long long out = 0;
	if ((inp >> out).fail()) return false;
	val = out;
	return true;
}

// Funktion zur Umwandlung eines string in einen unsigned long long
bool ConvStrToUnsignedLongLong(std::string input, unsigned long long& val)
{
	std::stringstream inp(input);
	unsigned long long out = 0;
	if ((inp >> out).fail()) return false;
	val = out;
	return true;
}

// Funktion zur Umwandlung eines string in einen double
bool ConvStrToDouble(std::string input, double& val)
{
	std::stringstream inp(input);
	double out = 0.0;
	if ((inp >> out).fail()) return false;
	val = out;
	return true;
}

/*
// Test equality of two double values
bool DoubleEqual(const double d1, const double d2) {

}

// Test unequality of two double values
bool DoubleUnequal(const double d1, const double d2) {

}

// Test if first double value is greater than the second one
bool DoubleGreater(const double d1, const double d2) {

}

// Test if first double value is smaller than the second one
bool DoubleSmaller(const double d1, const double d2) {

}

// Test equality of two double values with periodic boundary conditions
bool DoubleEqualPeriodic(const double lower_bound, const double d1, const double d2, const double higher_bound) {

}

// Test inequality of two double values with periodic boundary conditions
bool DoubleUnequalPeriodic(const double lower_bound, const double d1, const double d2, const double higher_bound) {

}
*/

// Vergleich zweier Winkel w1 und w2 auf einem Kreis
bool CirclePeriodicCompare(double w1, double w2)
{
	double s1 = w1;
	while (s1 < 0.0) s1 += 360.0;
	s1 = s1 - (int(s1 / 360.0)) * 360.0;
	double s2 = w2 - T3DVector::eq_threshold;
	while (s2 < 0.0) s2 += 360.0;
	s2 = s2 - (int(s2 / 360.0)) * 360.0;
	double s3 = w2 + T3DVector::eq_threshold;
	while (s3 < 0.0) s3 += 360.0;
	s3 = s3 - (int(s3 / 360.0)) * 360.0;
	// Pruefbereich [s2,s3]
	if (s2 < s3)
	{
		// -> Pruefbereich [s2,s3]
		if ((s1 > s2) && (s1 < s3)) return true;
	}
	else
	{
		// -> Pruefbereich ]s3,s2[
		if ((s1 > s2) || (s1 < s3)) return true;
	}
	return false;
}

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))

// Programmverzeichnis ohne Slash bzw. Backslash am Ende ermitteln (WINDOWS)
bool GetApplicationPath(std::string& out_str)
{
	vector<char> exe_path(MAX_PATH);
	DWORD result = GetModuleFileNameA(NULL, &exe_path[0], static_cast<DWORD>(exe_path.size()));	// try exe_path with MAX_PATH characters
	while (result == exe_path.size())
	{
		exe_path.resize(exe_path.size() * 2);													// falls exe_path zu kurz, verlaengern bis passt
		result = GetModuleFileNameA(NULL, &exe_path[0], static_cast<DWORD>(exe_path.size()));
	}
	if (result == 0) return false;
	out_str = std::string(exe_path.begin(), exe_path.begin() + result);
	out_str = out_str.substr(0, out_str.find_last_of("\\/"));
	return true;
}

// Prepend application path to relative file name (WINDOWS)
std::string PrependAppPath(std::string_view rel_filename)
{
	std::string t_filepath = "";
	if (GetApplicationPath(t_filepath) == true)
	{
		t_filepath += "\\";

		return t_filepath.append(rel_filename);
	}
	else
	{
		return std::string(rel_filename);
	}
}

#endif

#if (defined(__linux__) || defined(__linux))

// Programmverzeichnis ohne Slash bzw. Backslash am Ende ermitteln (LINUX)
bool GetApplicationPath(std::string& out_str)
{
	char exe_path[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", exe_path, PATH_MAX);
	if (count <= 0) return false;
	out_str = std::string(exe_path);
	out_str = out_str.substr(0, out_str.find_last_of("/"));
	return true;
}

// Prepend application path to relative file name (LINUX)
std::string PrependAppPath(std::string_view rel_filename)
{
	std::string t_filepath = "";
	if (GetApplicationPath(t_filepath) == true)
	{
		t_filepath += "/";

		return t_filepath.append(rel_filename);
	}
	else
	{
		return std::string(rel_filename);
	}
}

#endif