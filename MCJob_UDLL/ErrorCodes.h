// **************************************************************** //
//																	//
//	ErrorCodes														//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Zentrale Verwaltung der KMC-Fehlercodes						//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef ERRORCODES_H
#define ERRORCODES_H

// Konvention: nur Fehler, die durch den Input des Users verursacht werden, sind
//			   als nicht-kritisch einzustufen

// OK, d.h. kein Fehler aufgetreten (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_OK = 0;

// KMC Objekt-Member Ready nicht true
[[maybe_unused]] constexpr int KMCERR_READY_NOT_TRUE = -1;

// KMC Objekt-Member Ready nicht false
[[maybe_unused]] constexpr int KMCERR_READY_NOT_FALSE = -2;

// Ungueltiger Input (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_INVALID_INPUT = -3;

// Elementsymbol nicht gefunden
[[maybe_unused]] constexpr int KMCERR_ELEMENTSYMBOL_NOT_FOUND = -4;

// Konsistenzcheck failed
[[maybe_unused]] constexpr int KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY = -5;

// Objektpointer ist NULL
[[maybe_unused]] constexpr int KMCERR_INVALID_POINTER = -6;

// Objekt ist noch nicht Ready
[[maybe_unused]] constexpr int KMCERR_OBJECT_NOT_READY = -7;

// Element nicht gefunden
[[maybe_unused]] constexpr int KMCERR_ELEMENT_NOT_FOUND = -8;

// Input existiert schon (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_INPUT_ALREADY_EXISTS = -9;

// MovingSpecies oder Vacancy als Dotierung eingegeben <- nicht erlaubt (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING = -10;

// Es wurde kein dotiertes Atom in der Elementarzelle plaziert (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_NO_DOPED_ATOM_FOUND = -11;

// Maximaler Input wurde erreicht (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_MAXIMUM_INPUT_REACHED = -12;

// Fehler in NN-Analyse: keine Schalen gefunden
[[maybe_unused]] constexpr int KMCERR_NO_SHELLS_FOUND = -13;

// Es duerfen nicht alle Jumps eines Atoms deaktiviert werden
[[maybe_unused]] constexpr int KMCERR_NO_ACTIVE_JUMP_ON_ATOM = -14;

// Es wurde eine Exception ausgeloest
[[maybe_unused]] constexpr int KMCERR_EXCEPTION_OCCURED = -15;

// Ein fehlerhaftes Dateiformat liegt vor (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_INVALID_FILE_FORMAT = -16;

// Keine Console gefunden
[[maybe_unused]] constexpr int KMCERR_NO_CONSOLE = -17;

// Fehlerhafter Dateiinhalt (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_INVALID_FILE_CONTENT = -18;

// Test lieferte Ungleich
[[maybe_unused]] constexpr int KMCERR_UNEQUAL = -19;

// Test lieferte Gleich
[[maybe_unused]] constexpr int KMCERR_EQUAL = -20;

// Deaktivierung nicht möglich wegen Mindestanzahl (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_DEACTIVATION_IMPOSSIBLE = -21;

// Zu viele Atome aktiviert, sodass zuviele Codes entstehen (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_TOO_MANY_ACTIVE_ATOMS = -22;

// Kritischer, ungueltiger Input
[[maybe_unused]] constexpr int KMCERR_INVALID_INPUT_CRIT = -23;

// Dyn. Norm. Listeneintrag muss gesetzt werden
[[maybe_unused]] constexpr int KMCERR_HAS_TO_SET_DYNNORM_ENTRY = -24;

// Keine Simulationsergebnisse vorhanden (nicht-kritisch)
[[maybe_unused]] constexpr int KMCERR_NO_RESULTS = -25;

#endif