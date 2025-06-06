// **************************************************************** //
//																	//
//	Class: TElementsFunc	(TElements Layer 1)						//
//	Author: Philipp Hein											//
//  Description:													//
//    Class for managing the used elements							//
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
#include "TElements/TElementsFunc.h"

// Includes:
#include <sstream>

// Eigene Includes:
#include "ErrorCodes.h"
#include "GlobalDefinitions.h"
#include "TKMCJob/TKMCJob.h"

using namespace std;

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATOREN ******************** //

// Constructor
TElementsFunc::TElementsFunc(TKMCJob* pJob) : TElementsBase(pJob)
{

}

// Destructor
TElementsFunc::~TElementsFunc()
{

}

// **************************** PUBLISHED ********************************* //



// ***************************** PUBLIC *********************************** //

// ElemID zu Symbol, Name, Charge finden
int TElementsFunc::GetElemID(string_view i_symbol, string_view i_name, double i_charge)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	int ID = -1;
	for (int i = 0; i < (int)Symbol.size(); i++)
	{
		if ((Trim(i_symbol) == Symbol[i]) &&
			(Trim(i_name) == Name[i]) &&
			(i_charge > Charge[i] - KMCVAR_EQTHRESHOLD_CHARGE) &&
			(i_charge < Charge[i] + KMCVAR_EQTHRESHOLD_CHARGE))
		{
			ID = i;
			break;
		}
	}
	if (ID == -1) return KMCERR_ELEMENT_NOT_FOUND;

	return ID;
}

// Ladung der beweglichen Spezies ausgeben
int TElementsFunc::GetMovCharge(double& o_charge)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	o_charge = MovCharge;

	return KMCERR_OK;
}

// Elementbeschreibung ausgeben
int TElementsFunc::GetElemDesc(int i_ElemID, string& o_Desc)
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;

	if ((i_ElemID < 0) || (i_ElemID >= (int)Symbol.size())) return KMCERR_INVALID_INPUT_CRIT;

	stringstream t_Desc;
	t_Desc.precision(2);
	t_Desc << Symbol[i_ElemID] << "[";
	if (Name[i_ElemID] != "")
	{
		t_Desc << Name[i_ElemID] << ",";
	}
	t_Desc << Charge[i_ElemID] << "]";
	t_Desc.flush();
	o_Desc = t_Desc.str();

	return KMCERR_OK;
}

// ***************************** PRIVATE ********************************** //