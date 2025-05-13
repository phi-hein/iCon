// **************************************************************** //
//																	//
//	Class: TInteractionOut	(TInteraction Layer 2)					//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for describing a pair interaction						//
//	  Layer 2: Output class = get generated data					//
//    -> no modification of member variables						//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TInteraction/TInteractionOut.h"

// Includes:

// Eigene Includes:
#include "TKMCJob/TKMCJob.h"
#include "TElements/TElements.h"
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TInteractionOut::TInteractionOut(TKMCJob* pJob) : TInteractionFunc(pJob)
{

}

// Destructor
TInteractionOut::~TInteractionOut()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Grundelementbesetzung und Abstände ausgeben
int TInteractionOut::GetInteractionDesc(int& o_ElemID, double& SDist, double& DDist)
{

	// Informationen setzen
	o_ElemID = ElemID;
	SDist = StartDist;
	DDist = DestDist;

	return KMCERR_OK;
}

// Anzahl an Atomen in StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TInteractionOut::GetInteractionStartEnvCount(int& Count)
{

	// Anzahl ausgeben
	Count = (int)StartWWCoord.size();

	return KMCERR_OK;
}

// Anzahl an Atomen in DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TInteractionOut::GetInteractionDestEnvCount(int& Count)
{

	// Anzahl ausgeben
	Count = (int)DestWWCoord.size();

	return KMCERR_OK;
}

// Atom aus der StartAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TInteractionOut::GetInteractionStartEnvAtom(int ID, int& o_ElemID, double& o_X, double& o_Y, double& o_Z)
{

	// Input pruefen
	if ((ID < 0) || (ID >= (int)StartWWCoord.size()) || (ID >= (int)StartWWElemID.size()))
	{
		cout << "Critical Error: Invalid vector index (in TInteractionOut::GetInteractionStartEnvAtom)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Ausgabe
	o_ElemID = StartWWElemID[ID];
	o_X = StartWWCoord[ID].x;
	o_Y = StartWWCoord[ID].y;
	o_Z = StartWWCoord[ID].z;

	return KMCERR_OK;
}

// Atom aus der DestAtom-EnvAtom-Wechselwirkungsumgebung ausgeben
int TInteractionOut::GetInteractionDestEnvAtom(int ID, int& o_ElemID, double& o_X, double& o_Y, double& o_Z)
{

	// Input pruefen
	if ((ID < 0) || (ID >= (int)DestWWCoord.size()) || (ID >= (int)DestWWElemID.size()))
	{
		cout << "Critical Error: Invalid vector index (in TInteractionOut::GetInteractionDestEnvAtom)" << endl;
		return KMCERR_INVALID_INPUT_CRIT;
	}

	// Ausgabe
	o_ElemID = DestWWElemID[ID];
	o_X = DestWWCoord[ID].x;
	o_Y = DestWWCoord[ID].y;
	o_Z = DestWWCoord[ID].z;

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //