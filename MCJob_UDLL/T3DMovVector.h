// **************************************************************** //
//																	//
//	Class: T3DMovVector												//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for a displacement vector (incl. jump counter)			//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef T3DMovVector_H
#define T3DMovVector_H

// Includes:
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>

// Eigene Includes:

using namespace std;

// Klassendeklaration:
class T3DMovVector
{

public:
	// Variablen:
	double x, y, z;
	unsigned long long jcount;

	// Member functions:
	T3DMovVector() : x(0.0), y(0.0), z(0.0), jcount(0ULL)
	{									// Default-Constructor
	}
	T3DMovVector(double set_x, double set_y, double set_z, unsigned long long set_jcount) :		// Constructor mit Initialisierung
		x(set_x), y(set_y), z(set_z), jcount(set_jcount)
	{
	}
	void Set(double set_x, double set_y, double set_z, unsigned long long set_jcount)
	{			// Initialisierung
		x = set_x; y = set_y; z = set_z; jcount = set_jcount;
	}
	double Length() const
	{										// Vektorbetrag
		return sqrt(x * x + y * y + z * z);
	}
	T3DMovVector operator + (const T3DMovVector& param) const
	{		// Vektoraddition
		return T3DMovVector(x + param.x, y + param.y, z + param.z, jcount + param.jcount);
	}
	T3DMovVector& operator += (const T3DMovVector& param)
	{			// Vektoraddition
		x += param.x;
		y += param.y;
		z += param.z;
		jcount += param.jcount;
		return *this;
	}
	T3DMovVector operator - (const T3DMovVector& param) const
	{		// Vektorsubtraktion
		return T3DMovVector(x - param.x, y - param.y, z - param.z, jcount - param.jcount);
	}
	T3DMovVector operator * (double param) const
	{					// Multiplikation mit Skalar
		return T3DMovVector(x * param, y * param, z * param, (unsigned long long) (jcount * param));
	}

	// Static member functions:
	static double Length(const T3DMovVector& a);					// Betrag des Verschiebungsvektors eines MovVektors a

};

#endif