// **************************************************************** //
//																	//
//	Class: T3DVector												//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for a 3D vector											//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef T3DVector_H
#define T3DVector_H

// Includes:
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <iostream>
#include <vector>

// Eigene Includes:

using namespace std;

// Klassendeklaration:
class T3DVector
{
	friend ostream& operator << (ostream& out, const T3DVector& param)
	{		// Vektorausgabe
		if (abs(param.x) < T3DVector::zero_threshold)
		{
			out << "(0,";
		}
		else
		{
			out << "(" << param.x << ",";
		}
		if (abs(param.y) < T3DVector::zero_threshold)
		{
			out << "0,";
		}
		else
		{
			out << param.y << ",";
		}
		if (abs(param.z) < T3DVector::zero_threshold)
		{
			out << "0)";
		}
		else
		{
			out << param.z << ")";
		}
		return out;
	}

public:
	// Variablen:
	double x, y, z;
	static double eq_threshold;									// zum Ausgleich von Rundungsfehlern
	static double zero_threshold;								// zum Null setzen

	// Member functions:
	T3DVector() : x(0.0), y(0.0), z(0.0)
	{						// Default-Constructor
	}
	T3DVector(double set_x, double set_y, double set_z) :		// Constructor mit Initialisierung
		x(set_x), y(set_y), z(set_z)
	{
	}
	void Set(double set_x, double set_y, double set_z)
	{			// Initialisierung
		x = set_x; y = set_y; z = set_z;
	}
	double Length() const
	{									// Vektorbetrag
		return sqrt(x * x + y * y + z * z);
	}
	T3DVector operator + (const T3DVector& param) const
	{		// Vektoraddition
		return T3DVector(x + param.x, y + param.y, z + param.z);
	}
	T3DVector& operator += (const T3DVector& param)
	{			// Vektoraddition
		x += param.x;
		y += param.y;
		z += param.z;
		return *this;
	}
	T3DVector operator - (const T3DVector& param) const
	{		// Vektorsubtraktion
		return T3DVector(x - param.x, y - param.y, z - param.z);
	}
	T3DVector operator * (double param) const
	{					// Multiplikation mit Skalar
		return T3DVector(x * param, y * param, z * param);
	}
	double operator * (const T3DVector& param) const
	{			// Skalarprodukt
		return x * param.x + y * param.y + z * param.z;
	}
	bool operator == (const T3DVector& param) const
	{			// Vectorvergleich mit Threshold
		if ((abs(x - param.x) <= eq_threshold) &&
			(abs(y - param.y) <= eq_threshold) &&
			(abs(z - param.z) <= eq_threshold)) return true;
		return false;
	}
	bool operator != (const T3DVector& param) const
	{			// Vectorungleich mit Threshold
		if (*this == param) return false;
		return true;
	}
	bool operator > (const T3DVector& param) const
	{			// Groesser-Operator mit Threshold
		return ((x > param.x + eq_threshold) ||
			((abs(x - param.x) <= eq_threshold) && (y > param.y + eq_threshold)) ||
			((abs(x - param.x) <= eq_threshold) && (abs(y - param.y) <= eq_threshold) && (z > param.z + eq_threshold)));
	}

	// Static member functions:
	static bool LoadCompare(const T3DVector& a, const T3DVector& b, const bool if_periodic = false);	// Vektoren vergleichen (mit KMCVAR_EQTHRESHOLD_PERC_LOAD als Grenzwert)

	static double Length(const T3DVector& a);								// Betrag eines Vektors a

	static int LengthCompare(const T3DVector& a, const T3DVector& b);		// Laengenvergleich (-1 = a kleiner b, 0 = a gleich b, 1 = a groesser b)

	static double Angle(const T3DVector& a, const T3DVector& b);			// Winkel zwischen zwei Vektoren a und b in Grad

	static T3DVector Cross(const T3DVector& a, const T3DVector& b);		// Kreuzprodukt: a x b

	static double Projection(const T3DVector& a, const T3DVector& b);		// Laenge der Projektion von a auf b

	static T3DVector Normalvector(const T3DVector& a, const T3DVector& b);	// Vektor senkrecht auf b zu a

	static T3DVector Rotation(const T3DVector& a, const T3DVector& b, double alpha);	// Rotation von Vector a um Vector b um den Winkel alpha in Grad

	static void Transform2Jump(T3DVector& inp_vec, const T3DVector& jump_vec, const T3DVector& norm_vec);		// Transformation eines Umgebungsvektors ins Sprungkoordinatensystem (Normalvector des 1. Umgebungsvectors als norm_vec)

	static void Transform2Coord(T3DVector& inp_vec, const T3DVector& jump_vec, const T3DVector& norm_vec);		// Transformation eines Umgebungsvektors ins Raumkoordinatensystem (idR: jump_vec = (0,0,1) und norm_vec = (1,0,0))

	static bool LinIndependence(const T3DVector& vec_a, const T3DVector& vec_b, const T3DVector& vec_c);		// Test auf lineare Unabhängigkeit von Vektoren

	static int FindVector(const T3DVector& vec, const vector<T3DVector>* vectors);		// Pruefung, ob ein Vector in einer Vectorliste auftritt, Rueckgabewert: erste Vectorposition oder -1 (gut fuer switch-Struktur)
};

#endif