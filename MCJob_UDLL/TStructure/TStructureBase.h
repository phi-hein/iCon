// **************************************************************** //
//																	//
//	Klasse: TStructureBase	(TStructure Layer 0)					//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Klasse zur Verwaltung der eingegebenen Struktur der			//
//	  untersuchten Verbindung										//
//	  Layer 0: Base class, d.h. Member-Variablen, abgesicherter 	//
//	  Input, Funktionen die zu Ready == true fuehren, Speichern		//
//	  und Laden zu/von Stream, Rueckgabe von Ready					//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef TStructureBase_H
#define TStructureBase_H

// Includes:
#include <string>
#include <vector>

// Eigene Includes:
#include "T3DVector.h"

using namespace std;

// Forward-Deklarationen:
class TKMCJob;

// Klassendeklaration:
class TStructureBase {
	// Member variables
	protected:
		TKMCJob * m_Job;				// Pointer zur uebergeordneten KMCJob-Instanz

		string Name;					// Name der Struktur
		T3DVector a, b, c;				// Basisvektoren der Struktur [Angstrom]

		vector<int> ElemID;				// ElemID des Atoms i
		vector<T3DVector> Coord;		// relative Koordinaten des Atoms i

		vector<int> DopedID;			// ElemID des dotierten Elements für die Dotierung i
		vector<int> DopandID;			// ElemID des Dotierelements für die Dotierung i
		vector<double> VacDopRatio;		// Verhältnis Leerstellen/Dotieratome, beliebig, z.B. auch kein Einfluss auf Leerstellen oder Leerstellen-vermindernd
		bool Ready;						// Flag, ob Struktur vollständig und konsistent

	// Member functions
	public:
		// PUBLISHED
		int SetStructureName (string i_name);																																// Name der Struktur eingeben, max. 40 Zeichen
		int SetBasisByLatticeParam (double length_a, double length_b, double length_c, double angle_alpha, double angle_beta, double angle_gamma);							// Basis setzen aus 6 double (Längen, Winkel)
		int SetBasisByLatticeVectors (double ax, double ay, double az, double bx, double by, double bz, double cx, double cy, double cz);									// Basis setzen aus 9 double (Vektoren)
		int AddAtom (double xrel, double yrel, double zrel, string symbol, string name, double charge);																		// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
		int AddAtom (double xrel, double yrel, double zrel, int elemid);																									// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
		int AddDoping (string doped_symbol, string doped_name, double doped_charge, string dopand_symbol, string dopand_name, double dopand_charge, double vac_dop_ratio);	// Dotierung hinzufügen, noch keine Konzentrationen
		int AddDoping (int doped_elemid, int dopand_elemid, double vac_dop_ratio);																							// Dotierung hinzufügen, noch keine Konzentrationen
		int MakeStructure ();																																				// Struktur sortieren und überprüfen, dann Ready = true setzen

		// NON-PUBLISHED
		TStructureBase (TKMCJob * pJob);		// Constructor, Ready = false setzen
		bool IfReady ();						// Rueckgabe von Ready
		int SaveToStream (ostream &output, int offset);		// Struktur in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
		int LoadFromStream (istream &input);				// Struktur aus einem Stream laden, nur bei Ready == false moeglich

	protected:
		~TStructureBase();						// Destructor
};

#endif