// **************************************************************** //
//																	//
//	Klasse: IConsole												//
//	Autor: Philipp Hein												//
//	Datum: 15.09.2012												//
//  Aufgabe:														//
//    Abstract base class zur Abstraktion der internen Klassen von	//
//	  den aufrufbaren Methoden (sodass kein Export aller internen	//
//	  Klassen noetig)												//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

// Deklarierte Klasse:
#include "TConsole\IConsole.h"

// Eigene Includes:
#include "TConsole\TConsole.h"

using namespace std;

#pragma comment (linker, "/export:CreateConsole=_CreateConsole@0")

// Exportierte Funktion zur Erstellung einer Instanz der Klasse
DECLSPEC IConsole * __stdcall CreateConsole () {
	return new TConsole;
}