// **************************************************************** //
//																	//
//	Klasse: T3DVector												//
//	Autor: Philipp Hein												//
//	Datum: 27.08.2012												//
//  Aufgabe:														//
//    Klasse fuer Vektor im 3D-Raum									//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "T3DVector.h"

// Includes:

// Eigene Includes:
#include "GlobalDefinitions.h"

using namespace std;

// **************** STATIC - INITIALISIERUNG **********************

// Globale Initialisierung von eq_threshold und zero_threshold
double T3DVector::eq_threshold = KMCVAR_EQTHRESHOLD_VEC;

double T3DVector::zero_threshold = KMCVAR_ZEROTHRESHOLD_VEC;


// ******************* STATIC MEMBER FUNCTIONS *************************

// Vektoren vergleichen (mit KMCVAR_EQTHRESHOLD_PERC_LOAD als Grenzwert)
bool T3DVector::LoadCompare(const T3DVector &a, const T3DVector &b, const bool if_periodic) {
	if (a == b) return true;
	if ((abs(a.x - b.x) <= KMCVAR_EQTHRESHOLD_PERC_LOAD*abs(a.x)) &&
		(abs(a.y - b.y) <= KMCVAR_EQTHRESHOLD_PERC_LOAD*abs(a.y))) {
		if (abs(a.z - b.z) <= KMCVAR_EQTHRESHOLD_PERC_LOAD*abs(a.z)) return true;
		if (if_periodic == true) {
			if (abs(a.z + 360.0 - b.z) <= KMCVAR_EQTHRESHOLD_PERC_LOAD*abs(a.z + 360.0)) return true;
			if (abs(a.z - b.z - 360.0) <= KMCVAR_EQTHRESHOLD_PERC_LOAD*abs(a.z)) return true;
		}
	}
	return false;
}

// Betrag eines Vektors a
double T3DVector::Length (const T3DVector &a) {
	return a.Length();
}

// Laengenvergleich (-1 = a kleiner b, 0 = a gleich b, 1 = a groesser b)
int T3DVector::LengthCompare (const T3DVector &a, const T3DVector &b) {
	if (a.Length() - b.Length() > eq_threshold) return 1;
	if (a.Length() - b.Length() < -eq_threshold) return -1;
	return 0;
}

// Winkel zwischen zwei Vektoren a und b in Grad
double T3DVector::Angle (const T3DVector &a, const T3DVector &b) {
	if ((a.Length() == 0) || (b.Length() == 0)) return 0;
	double sav = (a*b)/(a.Length()*b.Length());
	if (sav > 1) sav = 1.0;
	if (sav < -1) sav = -1.0;
	return 180.0/M_PI*acos(sav);
}

// Kreuzprodukt: a x b
T3DVector T3DVector::Cross (const T3DVector &a, const T3DVector &b) {
	return T3DVector(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}

// Laenge der Projektion von a auf b
double T3DVector::Projection (const T3DVector &a, const T3DVector &b) {
	return (a*b)/b.Length();
}

// Vektor senkrecht auf b zu a
T3DVector T3DVector::Normalvector (const T3DVector &a, const T3DVector &b) {
	return a-b*(Projection(a,b)/b.Length());
}

// Rotation von Vector a um Vector b um den Winkel alpha in Grad
T3DVector T3DVector::Rotation (const T3DVector &a, const T3DVector &b, double alpha) {
	return	b*(1/b.Length())*((b*(1/b.Length()))*a) +
			Cross(Cross(b*(1/b.Length()),a),b*(1/b.Length()))*cos(M_PI*alpha/180.0) +
			Cross(b*(1/b.Length()),a)*sin(M_PI*alpha/180.0);
}

// Transformation eines Umgebungsvektors ins Sprungkoordinatensystem
// (Normalvector des 1. Umgebungsvectors als norm_vec)
void T3DVector::Transform2Jump (T3DVector &inp_vec, const T3DVector &jump_vec, const T3DVector &norm_vec) {
	double x, y, z;
	x = Projection(inp_vec,jump_vec);			    
	y = Length(Normalvector(inp_vec,jump_vec));
	if ((norm_vec.Length() < zero_threshold) || (abs(y) < zero_threshold)) {	// System mit nur Atomen auf Sprungachse bzw. Atom auf Sprungachse
		y = 0;
		z = 0;
	} else {
		if (Normalvector(inp_vec,jump_vec)*Cross(norm_vec,jump_vec) >= 0) {  // hier auch noch Grenzen der Funktionen beachten
			z = Angle(Normalvector(inp_vec,jump_vec),norm_vec);
		} else {
			z = 360.0 - Angle(Normalvector(inp_vec,jump_vec),norm_vec);
		}
	}
	if (abs(x) < zero_threshold) x = 0;
	if (abs(y) < zero_threshold) y = 0;
	if (abs(z) < zero_threshold) z = 0;
	if (abs(z) > 360.0 - zero_threshold) z = 0;
	inp_vec.x = x;		// Projektion von inp_vec auf jump_vec (positiv und negativ)
	inp_vec.y = y;		// Laenge des Normalenvektors von jump_vec zu inp_vec, d.h. Atom/Sprungvektor-Abstand
	inp_vec.z = z;		// relativer Winkel zwischen Umgebungsatomen in Grad
}

// Transformation eines Umgebungsvektors ins Raumkoordinatensystem
// (idR: jump_vec = (0,0,1) und norm_vec = (1,0,0)) 
void T3DVector::Transform2Coord (T3DVector &inp_vec, const T3DVector &jump_vec, const T3DVector &norm_vec) {
	if (inp_vec.y == 0) {
		inp_vec =	jump_vec*(inp_vec.x/jump_vec.Length());
	} else {
		inp_vec =	jump_vec*(inp_vec.x/jump_vec.Length()) +
					Rotation(norm_vec,jump_vec,inp_vec.z)*(inp_vec.y/norm_vec.Length());
	}
}

// Test auf lineare Unabhängigkeit von Vektoren
bool T3DVector::LinIndependence (const T3DVector &vec_a, const T3DVector &vec_b, const T3DVector &vec_c) {
	if (abs(vec_a.x*vec_b.y*vec_c.z +
			vec_a.z*vec_b.x*vec_c.y +
			vec_a.y*vec_b.z*vec_c.x -
			vec_a.z*vec_b.y*vec_c.x -
			vec_a.y*vec_b.x*vec_c.z -
			vec_a.x*vec_b.z*vec_c.y) > eq_threshold) return true;
	return false;
}

// Pruefung, ob ein Vector in einer Vectorliste auftritt
// Rueckgabewert: erste Vectorposition oder -1 (gut fuer switch-Struktur)
int T3DVector::FindVector (const T3DVector &vec, const vector<T3DVector> *vectors) {
	for (int i = 0; i < (int) vectors->size(); i++) {
		if (vec == vectors->at(i)) return i;
	}
	return -1;
}
