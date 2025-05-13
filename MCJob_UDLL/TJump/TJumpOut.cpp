// **************************************************************** //
//																	//
//	Class: TJumpOut	(TJump Layer 2)									//
//	Author: Philipp Hein											//
//	Description:													//
//    Class for describing a jump of an atom in the unit cell 		//
//	  and its jump environment										//
//	  Layer 2: Output class = get generated data					//
//	  -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//	Distributed under GPL v3 license								//
//	(see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TJump/TJumpOut.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "ErrorCodes.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TJumpOut::TJumpOut(TKMCJob* pJob) : TJumpFunc(pJob)
{

}

// Destructor
TJumpOut::~TJumpOut()
{

}

// **************************** PUBLISHED ********************************* //

// 4D-Vektor des Sprungstartatoms ausgeben
int TJumpOut::GetJumpStartPos(int& x, int& y, int& z, int& s)
{

	// Ausgabe
	x = StartPos.x;
	y = StartPos.y;
	z = StartPos.z;
	s = StartPos.s;

	return KMCERR_OK;
}

// 4D-Vektor des Sprungzielatoms ausgeben
int TJumpOut::GetJumpDestPos(int& x, int& y, int& z, int& s)
{

	// Ausgabe
	x = DestPos.x;
	y = DestPos.y;
	z = DestPos.z;
	s = DestPos.s;

	return KMCERR_OK;

}

// Ruecksprung-DirID ausgeben
int TJumpOut::GetJumpBackjumpDirID(int& dirID)
{

	// Ausgabe
	dirID = BackjumpDirID;

	return KMCERR_OK;
}

// Anzahl an Umgebungsatomen ausgeben
int TJumpOut::GetJumpEnvCount(int& count)
{
	if (Ready == false)
	{
		count = 0;
		return KMCERR_OK;
	}

	// Ausgabe
	count = EnvPos.size();

	return KMCERR_OK;
}

// Umgebungsatom ausgeben
int TJumpOut::GetJumpEnvAtom(int id, int& x, int& y, int& z, int& s)
{

	// Input pruefen
	if (id < 0)
	{
		cout << "Critical Error: Negative vector index (in TJumpOut::GetJumpEnvAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}
	if (id >= (int)EnvPos.size())
	{
		cout << "Critical Error: Index exceeds vector size (in TJumpsOut::GetJumpEnvAtom)" << endl << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Ausgabe
	x = EnvPos[id].x;
	y = EnvPos[id].y;
	z = EnvPos[id].z;
	s = EnvPos[id].s;

	return KMCERR_OK;
}

// ID des zugehörigen UniqueJump ausgeben
int TJumpOut::GetJumpUniqueJumpID(int& uniqueid)
{

	// Ausgabe
	uniqueid = UniqueJumpID;

	return KMCERR_OK;
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //