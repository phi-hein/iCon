// **************************************************************** //
//																	//
//	Class: TStructureBase	(TStructure Layer 0)					//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for managing the structure of the investigated material	//
//	  Layer 0: Base class = protected input of member variables,	//
//	  methods required for Ready == true, saving and loading		//
//	  to/from stream, signaling if Ready							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TStructure/TStructureBase.h"

// Includes:
#include <iomanip>
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor, Ready = false setzen
TStructureBase::TStructureBase(TKMCJob* pJob)
{
	m_Job = pJob;

	Ready = false;
	Name = KMCVAR_STRUCTURENAME_INIT;
	a.Set(0, 0, 0);
	b.Set(0, 0, 0);
	c.Set(0, 0, 0);
	ElemID.clear();
	Coord.clear();
	DopedID.clear();
	DopandID.clear();
	VacDopRatio.clear();
}

// Destructor
TStructureBase::~TStructureBase()
{
	m_Job = 0;
}

// **************************** PUBLISHED ********************************* //

// Name der Struktur eingeben, max. 40 Zeichen
int TStructureBase::SetStructureName(string i_name)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::SetStructureName)" << endl;
		cout << "                Input: " << Trim(i_name) << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	if (StringCheck(i_name, KMCVAL_TSTRUCTURE_NAME_MINLENGTH, KMCVAL_TSTRUCTURE_NAME_MAXLENGTH, KMCVAL_TSTRUCTURE_NAME_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid structure name: " << Trim(i_name) << endl;
		cout << "       Name must have " << KMCVAL_TSTRUCTURE_NAME_MINLENGTH << " to " << KMCVAL_TSTRUCTURE_NAME_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TSTRUCTURE_NAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TSTRUCTURE_NAME_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	Name = Trim(i_name);

	return KMCERR_OK;
}

// Basis setzen aus 6 double (Längen, Winkel)
int TStructureBase::SetBasisByLatticeParam(double length_a, double length_b, double length_c, double angle_alpha, double angle_beta, double angle_gamma)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::SetBasisByLatticeParam)" << endl;
		cout << "                Input: " << length_a << " " << length_b << " " << length_c << " " << angle_alpha << " ";
		cout << angle_beta << " " << angle_gamma << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	if ((length_a <= T3DVector::zero_threshold) || (length_b <= T3DVector::zero_threshold) || (length_c <= T3DVector::zero_threshold) ||
		(length_a > KMCVAL_TSTRUCTURE_LENGTHS_MAX) || (length_b > KMCVAL_TSTRUCTURE_LENGTHS_MAX) || (length_c > KMCVAL_TSTRUCTURE_LENGTHS_MAX))
	{
		cout << "Error: Invalid lattice lengths: " << length_a << ", " << length_b << ", " << length_c << endl;
		cout << "       Lattice lengths have to be rational numbers in the interval (0 , " << KMCVAL_TSTRUCTURE_LENGTHS_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((angle_alpha <= T3DVector::zero_threshold) || (angle_beta <= T3DVector::zero_threshold) || (angle_gamma <= T3DVector::zero_threshold) ||
		(angle_alpha >= 180 - T3DVector::zero_threshold) || (angle_beta >= 180 - T3DVector::zero_threshold) || (angle_gamma >= 180 - T3DVector::zero_threshold))
	{
		cout << "Error: Invalid lattice angles: " << angle_alpha << ", " << angle_beta << ", " << angle_gamma << endl;
		cout << "       Lattice angles have to be rational numbers in the interval (0 , 180)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((cos(NATCONST_PI * (angle_beta - angle_gamma) / 180.0) - cos(NATCONST_PI * angle_alpha / 180.0)) *
		(cos(NATCONST_PI * angle_alpha / 180.0) - cos(NATCONST_PI * (angle_beta + angle_gamma) / 180.0)) <= T3DVector::zero_threshold)
	{
		cout << "Error: Impossible lattice angle combination: " << angle_alpha << ", " << angle_beta << ", " << angle_gamma << endl;
		cout << "       Three linear independent vectors cannot have these angles inbetween." << endl;
		cout << "       Conditions for valid angles: cos(beta + gamma) < cos(alpha) < cos(beta - gamma)  or" << endl;
		cout << "                                    cos(beta - gamma) < cos(alpha) < cos(beta + gamma)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	T3DVector vec_a, vec_b, vec_c;
	vec_a.x = length_a;												// vec_a = (len_a,0,0)
	vec_b.x = length_b * cos(NATCONST_PI * angle_gamma / 180.0);
	vec_b.y = length_b * sin(NATCONST_PI * angle_gamma / 180.0);			// vec_b = (len_b*cos(gamma),len_b*sin(gamma),0)
	vec_c.x = length_c * cos(NATCONST_PI * angle_beta / 180.0);
	vec_c.y = length_c * (cos(NATCONST_PI * angle_alpha / 180.0) - cos(NATCONST_PI * angle_gamma / 180.0) * cos(NATCONST_PI * angle_beta / 180.0)) / sin(NATCONST_PI * angle_gamma / 180.0);
	vec_c.z = sqrt(length_c * length_c - vec_c.x * vec_c.x - vec_c.y * vec_c.y);
	if (abs(vec_a.x) < T3DVector::zero_threshold) vec_a.x = 0;
	if (abs(vec_a.y) < T3DVector::zero_threshold) vec_a.y = 0;
	if (abs(vec_a.z) < T3DVector::zero_threshold) vec_a.z = 0;
	if (abs(vec_b.x) < T3DVector::zero_threshold) vec_b.x = 0;
	if (abs(vec_b.y) < T3DVector::zero_threshold) vec_b.y = 0;
	if (abs(vec_b.z) < T3DVector::zero_threshold) vec_b.z = 0;
	if (abs(vec_c.x) < T3DVector::zero_threshold) vec_c.x = 0;
	if (abs(vec_c.y) < T3DVector::zero_threshold) vec_c.y = 0;
	if (abs(vec_c.z) < T3DVector::zero_threshold) vec_c.z = 0;
	if ((vec_a.Length() == 0) || (vec_b.Length() == 0) || (vec_c.Length() == 0))
	{
		cout << "Critical Error: Zero lattice vector (in TStructureBase::SetBasisByLatticeParam)" << endl;
		cout << "                Input: " << length_a << " " << length_b << " " << length_c << " " << angle_alpha << " ";
		cout << angle_beta << " " << angle_gamma << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (T3DVector::LinIndependence(vec_a, vec_b, vec_c) == false)
	{
		cout << "Critical Error: Lattice vectors not linear independent (in TStructureBase::SetBasisByLatticeParam)" << endl;
		cout << "                Input: " << length_a << " " << length_b << " " << length_c << " " << angle_alpha << " ";
		cout << angle_beta << " " << angle_gamma << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	a = vec_a;
	b = vec_b;
	c = vec_c;

	return KMCERR_OK;
}

// Basis setzen aus 9 double (Vektoren)
int TStructureBase::SetBasisByLatticeVectors(double ax, double ay, double az, double bx, double by, double bz, double cx, double cy, double cz)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::SetBasisByLatticeVectors)" << endl;
		cout << "                Input: " << ax << " " << ay << " " << az << " " << bx << " " << by << " " << bz << " " << cx << " ";
		cout << cy << " " << cz << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	T3DVector vec_a, vec_b, vec_c;
	vec_a.Set(ax, ay, az);
	vec_b.Set(bx, by, bz);
	vec_c.Set(cx, cy, cz);
	if (vec_a.Length() == 0)
	{
		cout << "Error: Invalid lattice vector a: " << vec_a.x << ", " << vec_a.y << ", " << vec_a.z << endl;
		cout << "       Vector length has to be greater than zero." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (vec_b.Length() == 0)
	{
		cout << "Error: Invalid lattice vector b: " << vec_b.x << ", " << vec_b.y << ", " << vec_b.z << endl;
		cout << "       Vector length has to be greater than zero." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (vec_c.Length() == 0)
	{
		cout << "Error: Invalid lattice vector c: " << vec_c.x << ", " << vec_c.y << ", " << vec_c.z << endl;
		cout << "       Vector length has to be greater than zero." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (T3DVector::LinIndependence(vec_a, vec_b, vec_c) == false)
	{
		cout << "Error: Invalid lattice vectors" << endl;
		cout << "       Lattice vectors have to be linear independent." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	a = vec_a;
	b = vec_b;
	c = vec_c;

	return KMCERR_OK;
}

// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
int TStructureBase::AddAtom(double xrel, double yrel, double zrel, string i_symbol, string i_name, double i_charge)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << i_symbol << " " << i_name << " " << i_charge << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Pruefung, ob Atomanzahl-Limit erreicht
	if ((int)Coord.size() >= KMCLIM_MAX_ATOMS_IN_UC)
	{
		cout << "Error: Too many atoms" << endl;
		cout << "       Maximum number of atoms is " << KMCLIM_MAX_ATOMS_IN_UC << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}

	// Gueltigkeit des Inputs pruefen
	if (StringCheck(i_symbol, KMCVAL_TELEMENTS_SYMBOL_MINLENGTH, KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH, KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid element symbol: " << Trim(i_symbol) << endl;
		cout << "       Symbol must have " << KMCVAL_TELEMENTS_SYMBOL_MINLENGTH << " to " << KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (StringCheck(i_name, KMCVAL_TELEMENTS_NAME_MINLENGTH, KMCVAL_TELEMENTS_NAME_MAXLENGTH, KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid optional element name: " << Trim(i_name) << endl;
		cout << "       Name must have " << KMCVAL_TELEMENTS_NAME_MINLENGTH << " to " << KMCVAL_TELEMENTS_NAME_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((i_charge < KMCVAL_TELEMENTS_CHARGE_MIN) || (i_charge > KMCVAL_TELEMENTS_CHARGE_MAX))
	{
		cout << "Error: Invalid element charge: " << i_charge << endl;
		cout << "       Charge has to be a rational number in the interval [" << KMCVAL_TELEMENTS_CHARGE_MIN << " , " << KMCVAL_TELEMENTS_CHARGE_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((xrel < 0) || (xrel >= 1))
	{
		cout << "Error: Invalid relative coordinate x (relative to lattice vector a): " << xrel << endl;
		cout << "       Relative coordinates are rational numbers in the interval [0 , 1)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((yrel < 0) || (yrel >= 1))
	{
		cout << "Error: Invalid relative coordinate y (relative to lattice vector b): " << yrel << endl;
		cout << "       Relative coordinates are rational numbers in the interval [0 , 1)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((zrel < 0) || (zrel >= 1))
	{
		cout << "Error: Invalid relative coordinate z (relative to lattice vector c): " << zrel << endl;
		cout << "       Relative coordinates are rational numbers in the interval [0 , 1)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Spezies in m_Job->Elements finden
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << i_symbol << " " << i_name << " " << i_charge << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL)
	{
		cout << "Critical Error: m_Elements is null pointer (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << i_symbol << " " << i_name << " " << i_charge << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false)
	{
		cout << "Critical Error: TElements not ready (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << i_symbol << " " << i_name << " " << i_charge << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	int ID = m_Job->m_Elements->GetElemID(Trim(i_symbol), Trim(i_name), i_charge);
	if (ID == KMCERR_READY_NOT_TRUE)
	{
		cout << "Critical Error: GetElemID not ready (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << i_symbol << " " << i_name << " " << i_charge << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (ID == KMCERR_ELEMENT_NOT_FOUND)
	{
		cout << "Critical Error: Element not found (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << i_symbol << " " << i_name << " " << i_charge << endl << endl;
		return KMCERR_ELEMENT_NOT_FOUND;
	}

	// Leerstellen sind nicht als Atom erlaubt
	if (ID == 1)
	{
		cout << "Error: Invalid atom species: " << Trim(i_symbol) << ", " << Trim(i_name) << ", " << i_charge << endl;
		cout << "       Vacancies are not allowed as unit cell content." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Pruefung, ob Vector schon vorhanden, d.h. keine zwei Atome am gleichen Ort
	T3DVector vec(xrel, yrel, zrel);
	if ((int)Coord.size() != 0)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if (vec == Coord[i])
			{
				cout << "Error: Invalid atom position: " << xrel << ", " << yrel << ", " << zrel << endl;
				cout << "       All atoms must have different positions." << endl << endl;
				return KMCERR_INPUT_ALREADY_EXISTS;
			}
		}
	}

	// Atom hinzufuegen
	Coord.push_back(vec);
	ElemID.push_back(ID);

	return KMCERR_OK;
}

// Atom hinzufügen, relative Atomkoordinaten aus [0,1)
int TStructureBase::AddAtom(double xrel, double yrel, double zrel, int elemid)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << elemid << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Pruefung, ob Atomanzahl-Limit erreicht
	if ((int)Coord.size() >= KMCLIM_MAX_ATOMS_IN_UC)
	{
		cout << "Error: Too many atoms" << endl;
		cout << "       Maximum number of atoms is " << KMCLIM_MAX_ATOMS_IN_UC << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}

	// Gueltigkeit des Inputs pruefen
	if ((xrel < 0) || (xrel >= 1))
	{
		cout << "Error: Invalid relative coordinate x (relative to lattice vector a): " << xrel << endl;
		cout << "       Relative coordinates are rational numbers in the interval [0 , 1)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((yrel < 0) || (yrel >= 1))
	{
		cout << "Error: Invalid relative coordinate y (relative to lattice vector b): " << yrel << endl;
		cout << "       Relative coordinates are rational numbers in the interval [0 , 1)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((zrel < 0) || (zrel >= 1))
	{
		cout << "Error: Invalid relative coordinate z (relative to lattice vector c): " << zrel << endl;
		cout << "       Relative coordinates are rational numbers in the interval [0 , 1)" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << elemid << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL)
	{
		cout << "Critical Error: m_Elements is null pointer (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << elemid << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false)
	{
		cout << "Critical Error: TElements not ready (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << elemid << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	int elemcount = 0;
	int ErrorCode = m_Job->m_Elements->GetElementCount(elemcount);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if ((elemid < 0) || (elemid >= elemcount))
	{
		cout << "Critical Error: Invalid element ID (in TStructureBase::AddAtom)" << endl;
		cout << "                Input: " << xrel << " " << yrel << " " << zrel << " " << elemid << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Leerstellen sind nicht als Atom erlaubt
	if (elemid == 1)
	{
		cout << "Error: Invalid atom species: " << elemid << endl;
		cout << "       Vacancies are not allowed as unit cell content." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Pruefung, ob Vector schon vorhanden, d.h. keine zwei Atome am gleichen Ort
	T3DVector vec(xrel, yrel, zrel);
	if ((int)Coord.size() != 0)
	{
		for (int i = 0; i < (int)Coord.size(); i++)
		{
			if (vec == Coord[i])
			{
				cout << "Error: Invalid atom position: " << xrel << ", " << yrel << ", " << zrel << endl;
				cout << "       All atoms must have different positions." << endl << endl;
				return KMCERR_INPUT_ALREADY_EXISTS;
			}
		}
	}

	// Atom hinzufuegen
	Coord.push_back(vec);
	ElemID.push_back(elemid);

	return KMCERR_OK;
}

// Dotierung hinzufügen, noch keine Konzentrationen
int TStructureBase::AddDoping(string doped_symbol, string doped_name, double doped_charge, string dopand_symbol,
	string dopand_name, double dopand_charge, double vac_dop_ratio)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << Trim(doped_symbol) << " " << Trim(doped_name) << " " << doped_charge << " ";
		cout << Trim(dopand_symbol) << " " << Trim(dopand_name) << " " << dopand_charge << " " << vac_dop_ratio << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Pruefung, ob maximale Dotierungsanzahl erreicht
	if ((int)DopedID.size() >= KMCLIM_MAX_DOPING)
	{
		cout << "Error: Too many dopings" << endl;
		cout << "       Maximum number of dopings is " << KMCLIM_MAX_DOPING << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}

	// Gueltigkeit des Inputs pruefen
	if (StringCheck(doped_symbol, KMCVAL_TELEMENTS_SYMBOL_MINLENGTH, KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH, KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid symbol of doped element: " << Trim(doped_symbol) << endl;
		cout << "       Symbol must have " << KMCVAL_TELEMENTS_SYMBOL_MINLENGTH << " to " << KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (StringCheck(dopand_symbol, KMCVAL_TELEMENTS_SYMBOL_MINLENGTH, KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH, KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid dopand element symbol: " << Trim(dopand_symbol) << endl;
		cout << "       Symbol must have " << KMCVAL_TELEMENTS_SYMBOL_MINLENGTH << " to " << KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (StringCheck(doped_name, KMCVAL_TELEMENTS_NAME_MINLENGTH, KMCVAL_TELEMENTS_NAME_MAXLENGTH, KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid optional name of doped element: " << Trim(doped_name) << endl;
		cout << "       Name must have " << KMCVAL_TELEMENTS_NAME_MINLENGTH << " to " << KMCVAL_TELEMENTS_NAME_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (StringCheck(dopand_name, KMCVAL_TELEMENTS_NAME_MINLENGTH, KMCVAL_TELEMENTS_NAME_MAXLENGTH, KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS) == false)
	{
		cout << "Error: Invalid optional dopand element name: " << Trim(dopand_name) << endl;
		cout << "       Name must have " << KMCVAL_TELEMENTS_NAME_MINLENGTH << " to " << KMCVAL_TELEMENTS_NAME_MAXLENGTH << " characters." << endl;
		if (KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS != "") cout << "       Allowed characters: [" << KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((doped_charge < KMCVAL_TELEMENTS_CHARGE_MIN) || (doped_charge > KMCVAL_TELEMENTS_CHARGE_MAX))
	{
		cout << "Error: Invalid charge of doped element: " << doped_charge << endl;
		cout << "       Charge has to be a rational number in the interval [" << KMCVAL_TELEMENTS_CHARGE_MIN << " , " << KMCVAL_TELEMENTS_CHARGE_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((dopand_charge < KMCVAL_TELEMENTS_CHARGE_MIN) || (dopand_charge > KMCVAL_TELEMENTS_CHARGE_MAX))
	{
		cout << "Error: Invalid dopand element charge: " << dopand_charge << endl;
		cout << "       Charge has to be a rational number in the interval [" << KMCVAL_TELEMENTS_CHARGE_MIN << " , " << KMCVAL_TELEMENTS_CHARGE_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((vac_dop_ratio < KMCVAL_TSTRUCTURE_VACDOPRATIO_MIN) || (vac_dop_ratio > KMCVAL_TSTRUCTURE_VACDOPRATIO_MAX))
	{
		cout << "Error: Invalid ratio of vacancies per dopand atom: " << vac_dop_ratio << endl;
		cout << "       Vacancy/Dopand ratio has to be a rational number in the interval [" << KMCVAL_TSTRUCTURE_VACDOPRATIO_MIN << " , " << KMCVAL_TSTRUCTURE_VACDOPRATIO_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Pruefen, ob Elemente in TElements vorhanden, aber nicht Moving oder Vac, und doped in ElemID vorhanden, d.h. Spezies definiert und doped-Atome vorhanden in Elementarzelle
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << Trim(doped_symbol) << " " << Trim(doped_name) << " " << doped_charge << " ";
		cout << Trim(dopand_symbol) << " " << Trim(dopand_name) << " " << dopand_charge << " " << vac_dop_ratio << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL)
	{
		cout << "Critical Error: m_Elements is null pointer (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << Trim(doped_symbol) << " " << Trim(doped_name) << " " << doped_charge << " ";
		cout << Trim(dopand_symbol) << " " << Trim(dopand_name) << " " << dopand_charge << " " << vac_dop_ratio << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false)
	{
		cout << "Critical Error: TElements not ready (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << Trim(doped_symbol) << " " << Trim(doped_name) << " " << doped_charge << " ";
		cout << Trim(dopand_symbol) << " " << Trim(dopand_name) << " " << dopand_charge << " " << vac_dop_ratio << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	int doped_ID = m_Job->m_Elements->GetElemID(Trim(doped_symbol), Trim(doped_name), doped_charge);
	int dopand_ID = m_Job->m_Elements->GetElemID(Trim(dopand_symbol), Trim(dopand_name), dopand_charge);
	if ((doped_ID == KMCERR_READY_NOT_TRUE) || (dopand_ID == KMCERR_READY_NOT_TRUE))
	{
		cout << "Critical Error: GetElemID not ready (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << Trim(doped_symbol) << " " << Trim(doped_name) << " " << doped_charge << " ";
		cout << Trim(dopand_symbol) << " " << Trim(dopand_name) << " " << dopand_charge << " " << vac_dop_ratio << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}
	if (doped_ID == KMCERR_ELEMENT_NOT_FOUND)
	{
		cout << "Error: Invalid doped species: " << Trim(doped_symbol) << ", " << Trim(doped_name) << ", " << doped_charge << endl;
		cout << "       There has to be at least one atom of the doped species in the unit cell." << endl << endl;
		return KMCERR_NO_DOPED_ATOM_FOUND;
	}
	if (dopand_ID == KMCERR_ELEMENT_NOT_FOUND)
	{
		cout << "Critical Error: Dopand element not found (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << Trim(doped_symbol) << " " << Trim(doped_name) << " " << doped_charge << " ";
		cout << Trim(dopand_symbol) << " " << Trim(dopand_name) << " " << dopand_charge << " " << vac_dop_ratio << endl << endl;
		return KMCERR_ELEMENT_NOT_FOUND;
	}
	if ((doped_ID == 0) || (doped_ID == 1))
	{
		cout << "Error: Invalid doped species: " << Trim(doped_symbol) << ", " << Trim(doped_name) << ", " << doped_charge << endl;
		cout << "       Moving species and vacancies are not allowed in doping." << endl << endl;
		return KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING;
	}
	if ((dopand_ID == 0) || (dopand_ID == 1))
	{
		cout << "Error: Invalid dopand species: " << Trim(dopand_symbol) << ", " << Trim(dopand_name) << ", " << dopand_charge << endl;
		cout << "       Moving species and vacancies are not allowed in doping." << endl << endl;
		return KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING;
	}
	if (doped_ID == dopand_ID)
	{
		cout << "Error: Invalid doping: " << Trim(doped_symbol) << ", " << Trim(doped_name) << ", " << doped_charge << " -> ";
		cout << Trim(dopand_symbol) << ", " << Trim(dopand_name) << ", " << dopand_charge << endl;
		cout << "       Doped element and dopand element have to be different." << endl;
		return KMCERR_INVALID_INPUT;
	}
	bool doped_found = false;
	if ((int)ElemID.size() != 0)
	{
		for (int i = 0; i < (int)ElemID.size(); i++)
		{
			if (doped_ID == ElemID[i])
			{
				doped_found = true;
				break;
			}
		}
	}
	if (doped_found == false)
	{
		cout << "Error: Invalid doped species: " << Trim(doped_symbol) << ", " << Trim(doped_name) << ", " << doped_charge << endl;
		cout << "       There has to be at least one atom of the doped species in the unit cell." << endl << endl;
		return KMCERR_NO_DOPED_ATOM_FOUND;
	}

	// Pruefen, ob Dotierung schon vorhanden
	if ((int)DopedID.size() != 0)
	{
		for (int i = 0; i < (int)DopedID.size(); i++)
		{
			if ((doped_ID == DopedID[i]) && (dopand_ID == DopandID[i]))
			{
				cout << "Error: Invalid doping: " << Trim(doped_symbol) << ", " << Trim(doped_name) << ", " << doped_charge << " -> ";
				cout << Trim(dopand_symbol) << ", " << Trim(dopand_name) << ", " << dopand_charge << endl;
				cout << "       This kind of doping is already defined." << endl << endl;
				return KMCERR_INPUT_ALREADY_EXISTS;
			}
		}
	}

	// Dotierung hinzufuegen
	DopedID.push_back(doped_ID);
	DopandID.push_back(dopand_ID);
	VacDopRatio.push_back(vac_dop_ratio);

	return KMCERR_OK;
}

// Dotierung hinzufügen, noch keine Konzentrationen
int TStructureBase::AddDoping(int doped_elemid, int dopand_elemid, double vac_dop_ratio)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << doped_elemid << " " << dopand_elemid << " " << vac_dop_ratio << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Pruefung, ob maximale Dotierungsanzahl erreicht
	if ((int)DopedID.size() >= KMCLIM_MAX_DOPING)
	{
		cout << "Error: Too many dopings" << endl;
		cout << "       Maximum number of dopings is " << KMCLIM_MAX_DOPING << endl << endl;
		return KMCERR_MAXIMUM_INPUT_REACHED;
	}

	// Gueltigkeit des Inputs pruefen
	if (m_Job == NULL)
	{
		cout << "Critical Error: m_Job is null pointer (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << doped_elemid << " " << dopand_elemid << " " << vac_dop_ratio << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements == NULL)
	{
		cout << "Critical Error: m_Elements is null pointer (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << doped_elemid << " " << dopand_elemid << " " << vac_dop_ratio << endl << endl;
		return KMCERR_INVALID_POINTER;
	}
	if (m_Job->m_Elements->IfReady() == false)
	{
		cout << "Critical Error: TElements not ready (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << doped_elemid << " " << dopand_elemid << " " << vac_dop_ratio << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	int elemcount = 0;
	int ErrorCode = m_Job->m_Elements->GetElementCount(elemcount);
	if (ErrorCode != KMCERR_OK) return ErrorCode;
	if ((doped_elemid < 0) || (doped_elemid >= elemcount))
	{
		cout << "Critical Error: Invalid doped element ID (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << doped_elemid << " " << dopand_elemid << " " << vac_dop_ratio << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((dopand_elemid < 0) || (dopand_elemid >= elemcount))
	{
		cout << "Critical Error: Invalid dopand element ID (in TStructureBase::AddDoping)" << endl;
		cout << "                Input: " << doped_elemid << " " << dopand_elemid << " " << vac_dop_ratio << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if ((vac_dop_ratio < KMCVAL_TSTRUCTURE_VACDOPRATIO_MIN) || (vac_dop_ratio > KMCVAL_TSTRUCTURE_VACDOPRATIO_MAX))
	{
		cout << "Error: Invalid ratio of vacancies per dopand atom: " << vac_dop_ratio << endl;
		cout << "       Vacancy/Dopand ratio has to be a rational number in the interval [" << KMCVAL_TSTRUCTURE_VACDOPRATIO_MIN << " , " << KMCVAL_TSTRUCTURE_VACDOPRATIO_MAX << "]" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Keine MovingSpecies oder Vacancy als dotierte Spezies oder Dopand erlaubt und letztere muessen unterschiedlich sein
	if ((doped_elemid == 0) || (doped_elemid == 1))
	{
		cout << "Error: Invalid doped species: " << doped_elemid << endl;
		cout << "       Moving species and vacancies are not allowed in doping." << endl << endl;
		return KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING;
	}
	if ((dopand_elemid == 0) || (dopand_elemid == 1))
	{
		cout << "Error: Invalid dopand species: " << dopand_elemid << endl;
		cout << "       Moving species and vacancies are not allowed in doping." << endl << endl;
		return KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING;
	}
	if (doped_elemid == dopand_elemid)
	{
		cout << "Error: Invalid doping: " << doped_elemid << " == " << dopand_elemid << endl;
		cout << "       Doped element and dopand element have to be different." << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Mindestens eine Atom der dotierten Spezies muss in der UnitCell sein
	bool doped_found = false;
	if ((int)ElemID.size() != 0)
	{
		for (int i = 0; i < (int)ElemID.size(); i++)
		{
			if (doped_elemid == ElemID[i])
			{
				doped_found = true;
				break;
			}
		}
	}
	if (doped_found == false)
	{
		cout << "Error: Invalid doped species: " << doped_elemid << endl;
		cout << "       There has to be at least one atom of the doped species in the unit cell." << endl << endl;
		return KMCERR_NO_DOPED_ATOM_FOUND;
	}

	// Pruefen, ob Dotierung schon vorhanden
	if ((int)DopedID.size() != 0)
	{
		for (int i = 0; i < (int)DopedID.size(); i++)
		{
			if ((doped_elemid == DopedID[i]) && (dopand_elemid == DopandID[i]))
			{
				cout << "Error: Invalid doping: " << doped_elemid << ", " << dopand_elemid << endl;
				cout << "       This kind of doping is already defined." << endl << endl;
				return KMCERR_INPUT_ALREADY_EXISTS;
			}
		}
	}

	// Dotierung hinzufuegen
	DopedID.push_back(doped_elemid);
	DopandID.push_back(dopand_elemid);
	VacDopRatio.push_back(vac_dop_ratio);

	return KMCERR_OK;
}

// Struktur sortieren und ueberpruefen, dann Ready = true setzen
int TStructureBase::MakeStructure()
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::MakeStructure)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Ueberpruefen auf Konsistenz
	if (Name.length() == 0)
	{
		cout << "Error: Structure name has to be specified" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if ((int)Coord.size() == 0)
	{
		cout << "Error: There has to be at least one atom in the unit cell" << endl << endl;
		return KMCERR_INVALID_INPUT;
	}
	if (Coord.size() != ElemID.size())
	{
		cout << "Critical Error: Unequal number of coords and elemids (in TStructureBase::MakeStructure)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if ((DopedID.size() != DopandID.size()) ||
		(DopedID.size() != VacDopRatio.size()) ||
		(DopandID.size() != VacDopRatio.size()))
	{
		cout << "Critical Error: Unequal sizes of doping vectors (in TStructureBase::MakeStructure)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	if (a.Length() == 0)
	{
		cout << "Critical Error: No lattice basis set (in TStructureBase::MakeStructure)" << endl << endl;
		return KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY;
	}
	bool mov_found = false;
	for (int i = 0; i < (int)ElemID.size(); i++)
	{
		if (ElemID[i] == 0)
		{
			mov_found = true;
			break;
		}
	}
	if (mov_found == false)
	{
		cout << "Error: There has to be at least one atom of the moving species in the unit cell." << endl << endl;
		return KMCERR_INVALID_INPUT;
	}

	// Atome sortieren (BubbleSort), d.h. nach ElemID, Atome einer ElemID sind nach relativen x,y,z sortiert (klein vor groß)
	int i_swaphelp;
	T3DVector v_swaphelp;
	if ((int)Coord.size() > 1)
	{
		for (int i = (int)Coord.size(); i > 1; i--)
		{
			for (int j = 0; j < i - 1; j++)
			{
				if ((ElemID[j] > ElemID[j + 1]) ||
					((ElemID[j] == ElemID[j + 1]) && (Coord[j] > Coord[j + 1])))
				{
					i_swaphelp = ElemID[j];
					ElemID[j] = ElemID[j + 1];
					ElemID[j + 1] = i_swaphelp;
					v_swaphelp = Coord[j];
					Coord[j] = Coord[j + 1];
					Coord[j + 1] = v_swaphelp;
				}
			}
		}
	}

	// Dotierungen sortieren (BubbleSort), d.h klein vor groß, Priorität: DopedID, DopandID, VacDopRatio
	double d_swaphelp;
	if ((int)DopedID.size() > 1)
	{
		for (int i = (int)DopedID.size(); i > 1; i--)
		{
			for (int j = 0; j < i - 1; j++)
			{
				if ((DopedID[j] > DopedID[j + 1]) ||
					((DopedID[j] == DopedID[j + 1]) && (DopandID[j] > DopandID[j + 1])) ||
					((DopedID[j] == DopedID[j + 1]) && (DopandID[j] == DopandID[j + 1]) && (VacDopRatio[j] > VacDopRatio[j + 1])))
				{
					i_swaphelp = DopedID[j];
					DopedID[j] = DopedID[j + 1];
					DopedID[j + 1] = i_swaphelp;
					i_swaphelp = DopandID[j];
					DopandID[j] = DopandID[j + 1];
					DopandID[j + 1] = i_swaphelp;
					d_swaphelp = VacDopRatio[j];
					VacDopRatio[j] = VacDopRatio[j + 1];
					VacDopRatio[j + 1] = d_swaphelp;
				}
			}
		}
	}

	Ready = true;
	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //

// Rueckgabe von Ready
bool TStructureBase::IfReady()
{
	return Ready;
}

// Struktur in einen Stream schreiben, nur bei Ready == true moeglich (offset = zusaetzliche Leerstellen vor jeder Zeile)
int TStructureBase::SaveToStream(ostream& output, int offset)
{
	if (Ready != true)
	{
		cout << "Critical Error: TStructure not ready (in TStructureBase::SaveToStream)" << endl << endl;
		return KMCERR_READY_NOT_TRUE;
	}

	// Input pruefen
	if (output.fail() == true)
	{
		cout << "Critical Error: Output stream not ready (in TElementsBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	if ((offset < 0) || (offset > KMCOUT_MAX_OFFSET))
	{
		cout << "Critical Error: Offset out of bounds (in TElementsBase::SaveToStream)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	string s_offset = "";
	if (offset > 0) s_offset = string(offset, ' ');

	// Offset des untergeordneten Outputs definieren
	string sub_offset = "";
	if ((int)KMCOUT_TSTRUCTURE_OFFSET > 0) sub_offset = string((int)KMCOUT_TSTRUCTURE_OFFSET, ' ');

	// Ausgabeparameter setzen
	output.unsetf(ios::floatfield);
	output.setf(ios::right, ios::adjustfield);

	// Ausgabe der Erkennungszeile und des Strukturnamens
	output << s_offset << KMCOUT_TSTRUCTURE_START << endl;
	output << s_offset << sub_offset << KMCOUT_TSTRUCTURE_NAME << " " << Name << endl;
	output << endl;

	// Ausgabe der Basisvektoren
	int x_space = (int)output.precision() + 7;
	int y_space = (int)output.precision() + 7;
	int z_space = (int)output.precision() + 7;
	if (x_space < (int)KMCOUT_TSTRUCTURE_XCOORD.length()) x_space = KMCOUT_TSTRUCTURE_XCOORD.length();
	if (y_space < (int)KMCOUT_TSTRUCTURE_YCOORD.length()) y_space = KMCOUT_TSTRUCTURE_YCOORD.length();
	if (z_space < (int)KMCOUT_TSTRUCTURE_ZCOORD.length()) z_space = KMCOUT_TSTRUCTURE_ZCOORD.length();
	int basis_space = (int)KMCOUT_TSTRUCTURE_BASISA.length();
	if (basis_space < (int)KMCOUT_TSTRUCTURE_BASISB.length()) basis_space = KMCOUT_TSTRUCTURE_BASISB.length();
	if (basis_space < (int)KMCOUT_TSTRUCTURE_BASISC.length()) basis_space = KMCOUT_TSTRUCTURE_BASISC.length();
	output << s_offset << sub_offset << setw(KMCOUT_TSTRUCTURE_BASIS.front().length() + basis_space + 4) << " " << "( ";
	output << setw(x_space) << KMCOUT_TSTRUCTURE_XCOORD << " ";
	output << setw(y_space) << KMCOUT_TSTRUCTURE_YCOORD << " ";
	output << setw(z_space) << KMCOUT_TSTRUCTURE_ZCOORD << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TSTRUCTURE_BASIS << " " << setw(basis_space) << KMCOUT_TSTRUCTURE_BASISA << " = ( ";
	output << setw(x_space) << a.x << " " << setw(y_space) << a.y << " " << setw(z_space) << a.z << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TSTRUCTURE_BASIS << " " << setw(basis_space) << KMCOUT_TSTRUCTURE_BASISB << " = ( ";
	output << setw(x_space) << b.x << " " << setw(y_space) << b.y << " " << setw(z_space) << b.z << " )" << endl;
	output << s_offset << sub_offset << KMCOUT_TSTRUCTURE_BASIS << " " << setw(basis_space) << KMCOUT_TSTRUCTURE_BASISC << " = ( ";
	output << setw(x_space) << c.x << " " << setw(y_space) << c.y << " " << setw(z_space) << c.z << " )" << endl;
	output << endl;

	// Ausgabe der Atome
	int ID_space = IntToStr(((int)ElemID.size()) - 1).length();
	if (ID_space < (int)KMCOUT_TSTRUCTURE_ATOMID.length()) ID_space = KMCOUT_TSTRUCTURE_ATOMID.length();
	int relx_space = (int)output.precision() + 7;
	int rely_space = (int)output.precision() + 7;
	int relz_space = (int)output.precision() + 7;
	if (relx_space < (int)KMCOUT_TSTRUCTURE_XREL.length()) relx_space = KMCOUT_TSTRUCTURE_XREL.length();
	if (rely_space < (int)KMCOUT_TSTRUCTURE_YREL.length()) rely_space = KMCOUT_TSTRUCTURE_YREL.length();
	if (relz_space < (int)KMCOUT_TSTRUCTURE_ZREL.length()) relz_space = KMCOUT_TSTRUCTURE_ZREL.length();
	int elemID_space = KMCOUT_TSTRUCTURE_ELEMID.length();
	for (int i = 0; i < (int)ElemID.size(); i++)
	{
		if ((int)IntToStr(ElemID[i]).length() > elemID_space) elemID_space = IntToStr(ElemID[i]).length();
	}
	output << s_offset << sub_offset << setw(KMCOUT_TSTRUCTURE_ATOM.length() + 1) << " ";
	output << setw(ID_space) << KMCOUT_TSTRUCTURE_ATOMID << " ( ";
	output << setw(relx_space) << KMCOUT_TSTRUCTURE_XREL << " ";
	output << setw(rely_space) << KMCOUT_TSTRUCTURE_YREL << " ";
	output << setw(relz_space) << KMCOUT_TSTRUCTURE_ZREL << " ) ";
	output << setw(elemID_space) << KMCOUT_TSTRUCTURE_ELEMID << endl;
	for (int i = 0; i < (int)ElemID.size(); i++)
	{
		output << s_offset << sub_offset << KMCOUT_TSTRUCTURE_ATOM << " ";
		output << setw(ID_space) << i << " ( ";
		output << setw(relx_space) << Coord[i].x << " ";
		output << setw(rely_space) << Coord[i].y << " ";
		output << setw(relz_space) << Coord[i].z << " ) ";
		output << setw(elemID_space) << ElemID[i] << endl;
	}
	output << endl;

	// Ausgabe der Dotierungen (falls vorhanden)
	if ((int)DopedID.size() != 0)
	{
		ID_space = IntToStr(((int)DopedID.size()) - 1).length();
		if (ID_space < (int)KMCOUT_TSTRUCTURE_DOPINGID.length()) ID_space = KMCOUT_TSTRUCTURE_DOPINGID.length();
		int dopedID_space = KMCOUT_TSTRUCTURE_DOPEDID.front().length();
		for (int i = 0; i < (int)DopedID.size(); i++)
		{
			if ((int)IntToStr(DopedID[i]).length() > dopedID_space) dopedID_space = IntToStr(DopedID[i]).length();
		}
		int dopandID_space = KMCOUT_TSTRUCTURE_DOPANDID.length();
		for (int i = 0; i < (int)DopandID.size(); i++)
		{
			if ((int)IntToStr(DopandID[i]).length() > dopandID_space) dopandID_space = IntToStr(DopandID[i]).length();
		}
		int Ratio_space = (int)output.precision() + 7;
		if (Ratio_space < (int)KMCOUT_TSTRUCTURE_VACDOPRATIO.length()) Ratio_space = KMCOUT_TSTRUCTURE_VACDOPRATIO.length();
		output << s_offset << sub_offset << setw(KMCOUT_TSTRUCTURE_DOPING.front().length() + 1) << " ";
		output << setw(ID_space) << KMCOUT_TSTRUCTURE_DOPINGID << " ";
		output << setw(dopedID_space) << KMCOUT_TSTRUCTURE_DOPEDID << " ";
		output << setw(dopandID_space) << KMCOUT_TSTRUCTURE_DOPANDID << " ";
		output << setw(Ratio_space) << KMCOUT_TSTRUCTURE_VACDOPRATIO << endl;
		for (int i = 0; i < (int)DopedID.size(); i++)
		{
			output << s_offset << sub_offset << KMCOUT_TSTRUCTURE_DOPING << " ";
			output << setw(ID_space) << i << " ";
			output << setw(dopedID_space) << DopedID[i] << " ";
			output << setw(dopandID_space) << DopandID[i] << " ";
			output << setw(Ratio_space) << VacDopRatio[i] << endl;
		}
	}

	output << s_offset << KMCOUT_TSTRUCTURE_END << endl;

	if (output.fail() == true)
	{
		cout << "Critical Error: Output stream failure (in TStructureBase::SaveToStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}
	return KMCERR_OK;
}

// Struktur aus einem Stream laden, nur bei Ready == false moeglich
int TStructureBase::LoadFromStream(istream& input)
{
	if (Ready != false)
	{
		cout << "Critical Error: TStructure ready too early (in TStructureBase::LoadFromStream)" << endl << endl;
		return KMCERR_READY_NOT_FALSE;
	}

	// Input pruefen
	if (input.good() == false)
	{
		cout << "Critical Error: Input stream not ready (in TStructureBase::LoadFromStream)" << endl << endl;
		return KMCERR_OBJECT_NOT_READY;
	}

	// Inputparameter setzen
	input >> skipws;

	// Daten einlesen
	string i_Name = "";
	vector<T3DVector> i_Basis(3, T3DVector());
	vector<T3DVector> i_Coord;
	vector<int> i_ElemID;
	vector<int> i_DopedID;
	vector<int> i_DopandID;
	vector<double> i_VacDopRatio;
	string line = "";
	bool if_failed = false;
	string s_temp = "";
	int i_temp = 0;
	double d_temp = 0;
	while (input.good() == true)
	{
		// Zeile laden
		if (getline(input, line).fail() == true)
		{
			if_failed = true;
			break;
		}
		// Zeile interpretieren
		stringstream linestream(line);
		if ((linestream >> s_temp).fail() == true) s_temp = "";

		if (s_temp == KMCOUT_TSTRUCTURE_NAME)
		{
			if (getline(linestream, s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			i_Name = s_temp;
		}
		else if (s_temp == KMCOUT_TSTRUCTURE_BASIS)
		{
			if ((linestream >> s_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			int index = -1;
			if (s_temp == KMCOUT_TSTRUCTURE_BASISA) index = 0;
			if (s_temp == KMCOUT_TSTRUCTURE_BASISB) index = 1;
			if (s_temp == KMCOUT_TSTRUCTURE_BASISC) index = 2;
			if (index == -1)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "="
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			if ((linestream >> i_Basis[index].x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_Basis[index].y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_Basis[index].z).fail() == true)
			{
				if_failed = true;
				break;
			}
		}
		else if (s_temp == KMCOUT_TSTRUCTURE_ATOM)
		{
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((i_temp != (int)i_Coord.size()) || (i_temp != (int)i_ElemID.size()))
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// "("
				if_failed = true;
				break;
			}
			i_Coord.push_back(T3DVector());
			if ((linestream >> i_Coord.back().x).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_Coord.back().y).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_Coord.back().z).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((linestream >> s_temp).fail() == true)
			{	// ")"
				if_failed = true;
				break;
			}
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			i_ElemID.push_back(i_temp);
		}
		else if (s_temp == KMCOUT_TSTRUCTURE_DOPING)
		{
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			if ((i_temp != (int)i_DopedID.size()) || (i_temp != (int)i_DopandID.size()) || (i_temp != (int)i_VacDopRatio.size()))
			{
				if_failed = true;
				break;
			}
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			i_DopedID.push_back(i_temp);
			if ((linestream >> i_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			i_DopandID.push_back(i_temp);
			if ((linestream >> d_temp).fail() == true)
			{
				if_failed = true;
				break;
			}
			i_VacDopRatio.push_back(d_temp);
		}
		else if (s_temp == KMCOUT_TSTRUCTURE_END) break;
	}
	if ((int)i_Coord.size() == 0) if_failed = true;
	if (i_Coord.size() != i_ElemID.size()) if_failed = true;
	if ((i_DopedID.size() != i_DopandID.size()) || (i_DopedID.size() != i_VacDopRatio.size())) if_failed = true;
	if (if_failed == true)
	{
		cout << "Error: Invalid file format" << endl << endl;
		return KMCERR_INVALID_FILE_FORMAT;
	}

	// Bisherigen Input zwischenspeichern und loeschen
	string t_Name = Name;
	Name = KMCVAR_STRUCTURENAME_INIT;
	T3DVector t_a = a;
	a = T3DVector(0, 0, 0);
	T3DVector t_b = b;
	b = T3DVector(0, 0, 0);
	T3DVector t_c = c;
	c = T3DVector(0, 0, 0);
	vector<int> t_ElemID(ElemID);
	ElemID.clear();
	vector<T3DVector> t_Coord(Coord);
	Coord.clear();
	vector<int> t_DopedID(DopedID);
	DopedID.clear();
	vector<int> t_DopandID(DopandID);
	DopandID.clear();
	vector<double> t_VacDopRatio(VacDopRatio);
	VacDopRatio.clear();

	// Daten laden
	int ErrorCode = KMCERR_OK;
	if_failed = false;
	ErrorCode = SetStructureName(i_Name);
	if (ErrorCode != KMCERR_OK) if_failed = true;
	if (if_failed == false)
	{
		ErrorCode = SetBasisByLatticeVectors(i_Basis[0].x, i_Basis[0].y, i_Basis[0].z, i_Basis[1].x, i_Basis[1].y, i_Basis[1].z, i_Basis[2].x, i_Basis[2].y, i_Basis[2].z);
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}
	if (if_failed == false)
	{
		for (int i = 0; i < (int)i_Coord.size(); i++)
		{
			ErrorCode = AddAtom(i_Coord[i].x, i_Coord[i].y, i_Coord[i].z, i_ElemID[i]);
			if (ErrorCode != KMCERR_OK)
			{
				if_failed = true;
				break;
			}
		}
	}
	if (if_failed == false)
	{
		if ((int)i_DopedID.size() != 0)
		{
			for (int i = 0; i < (int)i_DopedID.size(); i++)
			{
				ErrorCode = AddDoping(i_DopedID[i], i_DopandID[i], i_VacDopRatio[i]);
				if (ErrorCode != KMCERR_OK)
				{
					if_failed = true;
					break;
				}
			}
		}
	}
	if (if_failed == false)
	{
		ErrorCode = MakeStructure();
		if (ErrorCode != KMCERR_OK) if_failed = true;
	}

	// Falls Fehler aufgetreten ist, alte Daten wiederherstellen und Fehlercode ausgeben
	if (if_failed == true)
	{
		Name = t_Name;
		a = t_a;
		b = t_b;
		c = t_c;
		ElemID = t_ElemID;
		Coord = t_Coord;
		DopedID = t_DopedID;
		DopandID = t_DopandID;
		VacDopRatio = t_VacDopRatio;
		Ready = false;
		return ErrorCode;
	}

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //