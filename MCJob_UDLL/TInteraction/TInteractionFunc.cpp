// **************************************************************** //
//																	//
//	Class: TInteractionFunc	(TInteraction Layer 1)					//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for describing a pair interaction						//
//	  Layer 1: Functionality class = helper methods					//
//    -> no modification of member variables						//
//	  -> no published methods										//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TInteraction/TInteractionFunc.h"

// Includes:

// Eigene Includes:
#include "ErrorCodes.h"
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TInteractionFunc::TInteractionFunc(TKMCJob* pJob) : TInteractionBase(pJob)
{

}

// Destructor
TInteractionFunc::~TInteractionFunc()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// Basis-Charakteristika der WW zurückgeben
int TInteractionFunc::GetWWDesc(int& o_ElemID, double& o_StartDist, double& o_DestDist)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	o_ElemID = ElemID;
	o_StartDist = StartDist;
	o_DestDist = DestDist;

	return KMCERR_OK;
}

// WW-Schalen zurückgeben
int TInteractionFunc::GetWWShells(vector<T3DVector>* o_StartWWCoord, vector<int>* o_StartWWElemID, vector<T3DVector>* o_DestWWCoord, vector<int>* o_DestWWElemID)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	*o_StartWWCoord = StartWWCoord;
	*o_StartWWElemID = StartWWElemID;
	*o_DestWWCoord = DestWWCoord;
	*o_DestWWElemID = DestWWElemID;

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //