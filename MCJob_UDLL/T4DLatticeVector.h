// **************************************************************** //
//																	//
//	Class: T4DLatticeVector											//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for a vector in the 4D lattice (rel. and absolute)		//
//	  Lattice[x][y][z][s] (s = stack of unit cell atoms)			//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#ifndef T4DLatticeVector_H
#define T4DLatticeVector_H

// Includes:
#include <string>
#include <iostream>

// Eigene Includes:

using namespace std;

// Klassendeklaration:
class T4DLatticeVector
{
	friend ostream& operator << (ostream& out, const T4DLatticeVector& param)
	{		// Vektorausgabe
		out << "(" << param.x << "," << param.y << "," << param.z << "," << param.s << ")";
		return out;
	}
public:
	// Variablen
	int x;						// Verschiebung der UC in a-Richtung
	int y;						// Verschiebung der UC in b-Richtung
	int z;						// Verschiebung der UC in c-Richtung
	int s;						// Verschiebung innerhalb des Atom-Stapels der UC

	// Member functions
	T4DLatticeVector() : x(0), y(0), z(0), s(0)
	{
	}
	T4DLatticeVector(int set_x, int set_y, int set_z, int set_s) :
		x(set_x), y(set_y), z(set_z), s(set_s)
	{
	}
	void Set(int set_x, int set_y, int set_z, int set_s)
	{		// Koordinaten gebuendelt setzen
		x = set_x; y = set_y; z = set_z; s = set_s;
	}
	T4DLatticeVector operator + (const T4DLatticeVector& param) const
	{		// Vektoraddition
		return T4DLatticeVector(x + param.x, y + param.y, z + param.z, s + param.s);
	}
	T4DLatticeVector operator - (const T4DLatticeVector& param) const
	{		// Vektorsubtraktion
		return T4DLatticeVector(x - param.x, y - param.y, z - param.z, s - param.s);
	}
	bool operator == (const T4DLatticeVector& param) const
	{				// Vectorvergleich
		if ((x == param.x) && (y == param.y) &&
			(z == param.z) && (s == param.s)) return true;
		return false;
	}
	bool operator != (const T4DLatticeVector& param) const
	{				// Vectorungleich
		if (*this == param) return false;
		return true;
	}
};

#endif