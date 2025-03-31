// **************************************************************** //
//																	//
//	GlobalDefinitions												//
//	Autor: Philipp Hein												//
//	Datum: 01.09.2012												//
//  Aufgabe:														//
//    Globale Makro-, Funktionen- und Wertdefinitionen				//
//																	//
//	-- Property of Work Group Martin, RWTH Aachen University --		//
//																	//
// **************************************************************** //

#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

// Includes:
#include <string>

using namespace std;

// **************************** VERSION ******************************** //

// Programmversion:
#define KMC_VERSION "0.9.4.0"
// Bezeichner fuer die Programmversion
#define KMCOUT_VERSION "Version:"
// Kompatible Dateiversionen (;-getrennt)
#define KMC_COMP_FILE_VERSIONS "0.9.0.5;0.9.1.0;0.9.2.0;0.9.3.0;0.9.4.0"

// ************************** NATURKONSTANTEN ************************** //

// Pi
#define NATCONST_PI 3.14159265358979
// Boltzmann-Konstante in eV/K
#define NATCONST_KB 0.000086173325
// Eulersche Zahl e
#define NATCONST_E 2.718281828459
// Elementarladung e in C
#define NATCONST_ECHARGE 1.602176565E-19

// **************************** DATEINAMEN ***************************** //

// Datei fuer Elementbeschreibungen
#define KMCPATH_ELEMDESC "ElementsDrawInfo.ini"

// Dateiendung fuer Checkpoint-Dateien
#define KMCPATH_CHK_EXT ".mcp"

// Standard-Summary-Dateiname
#define KMCPATH_DEFAULT_SUMMARY "Summary.csv"

// *************************** STANDARDWERTE *************************** //

// Init-Symbol fuer Moving Species
#define KMCVAR_MOVSYMBOL "H"
// Init-Name fuer Moving Species
#define KMCVAR_MOVNAME ""
// Init-Ladung fuer Moving Species
#define KMCVAR_MOVCHARGE 0
// Symbol fuer Vacancies
#define KMCVAR_VACSYMBOL "Vc"
// Name fuer Vacancies
#define KMCVAR_VACNAME ""
// Ladung fuer Vacancies
#define KMCVAR_VACCHARGE 0.0
// Symbol fuer beliebige Spezies
#define KMCVAR_ARBITRARYSYMBOL "XX"
// Zero-Threshold fuer Vektoren
#define KMCVAR_ZEROTHRESHOLD_VEC 0.000001
// ==-Threshold fuer Vektoren
#define KMCVAR_EQTHRESHOLD_VEC 0.0001
// ==-Threshold fuer Ladungen
#define KMCVAR_EQTHRESHOLD_CHARGE 0.01
// ==-Threshold fuer Schalen (NN-Analyse)
#define KMCVAR_EQTHRESHOLD_SHELL 0.0001
// ==-Threshold (prozentual) fuer Datenvergleich beim Laden
#define KMCVAR_EQTHRESHOLD_PERC_LOAD 0.001
// Standardprojektname
#define KMCVAR_PROJECTNAME_INIT "New Project"
// Standardusername
#define KMCVAR_USERNAME_INIT ""
// Standardprojektname
#define KMCVAR_PROJECTDESC_INIT ""
// Standardstrukturname
#define KMCVAR_STRUCTURENAME_INIT "New Structure"
// Standard-MovShellCount
#define KMCVAR_MOVSHELL_INIT 1
// Standard-JumpStartShellCount
#define KMCVAR_JUMPSTARTSHELL_INIT 1
// Standard-JumpTSShellCount
#define KMCVAR_JUMPTSSHELL_INIT 0
// Standard-JumpDestShellCount
#define KMCVAR_JUMPDESTSHELL_INIT 1
// Standard-InteractionShellCount
#define KMCVAR_INTERACTIONSHELL_INIT 0
// Standard-Temperatur
#define KMCVAR_TEMPERATURE_INIT 300.0
// Standard-Sprungfrequenz
#define KMCVAR_ATTEMPTFREQ_INIT 1.0E+13
// Standard-E-Feld-Beitrag
#define KMCVAR_EFIELDMAGNITUDE_INIT 0.1
// Standard-E-Feld-Koordinate x
#define KMCVAR_EFIELDX_INIT 1.0
// Standard-E-Feld-Koordinate y
#define KMCVAR_EFIELDY_INIT 0.0
// Standard-E-Feld-Koordinate z
#define KMCVAR_EFIELDZ_INIT 0.0
// Standard-Gittergroesse
#define KMCVAR_LATTICESIZE_INIT 16
// Standard-MCSP
#define KMCVAR_MCSP_INIT 10000LL
// Double-Precision für Jobspeicherung (min. 17 ist nötig, weil 17 signifikante Stellen (Mantisse) fuer die Unterscheidung aller double-Werte noetig sind, dadurch sind Wertveränderungen beim Speichern und Laden ausgeschlossen)
#define KMCVAR_SAVEFILE_DOUBLEPRECISION 17
// ==-Threshold fuer Wahrscheinlichkeiten (plus-Multiplikator)
#define KMCVAR_EQTHRESHOLD_PROBPLUS 1.0001
// ==-Threshold fuer Wahrscheinlichkeiten (minus-Multiplikator)
#define KMCVAR_EQTHRESHOLD_PROBMINUS 0.9999

// *************************** ANZAHLENLIMITS ************************** //

// Maximale Anzahl an Elementen
#define KMCLIM_MAX_ELEMENTS 20
// Maximale Anzahl an Atomen in der Elementarzelle
#define KMCLIM_MAX_ATOMS_IN_UC 500
// Maximale Anzahl an Dotierungen
#define KMCLIM_MAX_DOPING 10
// Maximale Anzahl an Umgebungspermutationen
#define KMCLIM_MAX_ENVPERMUT 200000ULL

// ************************** VARIABLENLIMITS ************************** //

// --- TKMCJob ---
// Minimale ProjectName-Laenge
#define KMCVAL_TKMCJOB_PROJECTNAME_MINLENGTH 2
// Maximale ProjectName-Laenge
#define KMCVAL_TKMCJOB_PROJECTNAME_MAXLENGTH 100
// Erlaubte Buchstaben in ProjectName
#define KMCVAL_TKMCJOB_PROJECTNAME_ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-^() "
// Minimale UserName-Laenge
#define KMCVAL_TKMCJOB_USERNAME_MINLENGTH 0
// Maximale UserName-Laenge
#define KMCVAL_TKMCJOB_USERNAME_MAXLENGTH 100
// Erlaubte Buchstaben in UserName
#define KMCVAL_TKMCJOB_USERNAME_ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"
// Minimale ProjectDescription-Laenge
#define KMCVAL_TKMCJOB_PROJECTDESC_MINLENGTH 0
// Maximale ProjectDescription-Laenge
#define KMCVAL_TKMCJOB_PROJECTDESC_MAXLENGTH 10000
// Erlaubte Buchstaben in ProjectDescription
#define KMCVAL_TKMCJOB_PROJECTDESC_ALLOWED_CHARS ""

// --- TElements ---
// Minimale Symbol-Laenge
#define KMCVAL_TELEMENTS_SYMBOL_MINLENGTH 1
// Maximale Symbol-Laenge
#define KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH 2
// Erlaubte Buchstaben in Symbol
#define KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
// Minimale Name-Laenge
#define KMCVAL_TELEMENTS_NAME_MINLENGTH 0
// Maximale Name-Laenge
#define KMCVAL_TELEMENTS_NAME_MAXLENGTH 20
// Erlaubte Buchstaben in Name
#define KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_()"
// Minimale Ladung
#define KMCVAL_TELEMENTS_CHARGE_MIN -1000.0
// Maximale Ladung
#define KMCVAL_TELEMENTS_CHARGE_MAX 1000.0

// --- TStructure ---
// Minimale Strukturnamens-Laenge
#define KMCVAL_TSTRUCTURE_NAME_MINLENGTH 1
// Maximale Strukturnamens-Laenge
#define KMCVAL_TSTRUCTURE_NAME_MAXLENGTH 100
// Erlaubte Buchstaben in Strukturnamen
#define KMCVAL_TSTRUCTURE_NAME_ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-^() "
// Maximale Gitterkonstante
#define KMCVAL_TSTRUCTURE_LENGTHS_MAX 100.0
// Minimales Leerstellen-pro-Dopand-Verhaeltnis
#define KMCVAL_TSTRUCTURE_VACDOPRATIO_MIN -10.0
// Maximales Leerstellen-pro-Dopand-Verhaeltnis
#define KMCVAL_TSTRUCTURE_VACDOPRATIO_MAX 10.0

// --- TJumps ---
// Maximale Anzahl an Sprungziel-Schalen
#define KMCVAL_TJUMPS_MAX_MOVSHELLCOUNT 5
// Maximale Anzahl an Sprungumgebungs-Schalen
#define KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT 150

// --- TUniqueJumps ---
// Maximale Anzahl an WW-Schalen
#define KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONSHELLCOUNT 5
// Minimale Energie für Wechselwirkung
#define KMCVAL_TUNIQUEJUMPS_MIN_INTERACTIONENERGY -1000.0
// Maximale Energie für Wechselwirkung
#define KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONENERGY 1000.0
// Minimale Code-Aktivierungsenergie
#define KMCVAL_TUNIQUEJUMPS_MIN_CODEENERGY -1000.0
// Maximale Code-Aktivierungsenergie
#define KMCVAL_TUNIQUEJUMPS_MAX_CODEENERGY 1000.0

// --- TSettings ---
// Minimale Temperatur [in K]
#define KMCVAL_TSETTINGS_MIN_TEMPERATURE 0.001
// Maximale Temperatur [in K]
#define KMCVAL_TSETTINGS_MAX_TEMPERATURE 10000.0
// Maximaler E-Feldbeitrag [in kBT] zu Sprungaktivierungsenergien
#define KMCVAL_TSETTINGS_MAX_EFIELD 1000.0
// Minimale Sprungversuchsfrequenz
#define KMCVAL_TSETTINGS_MIN_FREQUENCY 0.001
// Maximale Sprungvesuchsfrequenz
#define KMCVAL_TSETTINGS_MAX_FREQUENCY 1.0E+34
// Minimale Gittergröße (>= 1)
#define KMCVAL_TSETTINGS_MIN_LATTICESIZE 1
// Maximale Gittergröße
#define KMCVAL_TSETTINGS_MAX_LATTICESIZE 65
// Maximalanzahl an zusätzlichen Leerstellen
#define KMCVAL_TSETTINGS_MAX_ADDVACANZ 1000000LL
// Maximale MCSP für Vorlauf
#define KMCVAL_TSETTINGS_MAX_PREMCSP 5000000LL
// Maximalanzahl fuer aufzuzeichnende Boltzmann-Wahrscheinlichkeiten beim Vorlauf
#define KMCVAL_TSETTINGS_MAX_PRERECORDANZ 10000
// Maximalanzahl fuer JumpAttempts bei dyn. Normierungssimulation
#define KMCVAL_TSETTINGS_MAX_NORMATTEMPTANZ 5000000000LL
// Maximalanzahl fuer aufzuzeichnende dyn. Normierungswahrscheinlichkeiten
#define KMCVAL_TSETTINGS_MAX_NORMRECORDANZ 10000
// Maximum fuer die Nummer des Listeneintrags für die dyn. Normierung
#define KMCVAL_TSETTINGS_MAX_NORMNUM 10000
// Maximale MCSP fuer Hauptsimulation
#define KMCVAL_TSETTINGS_MAX_MCSP 5000000LL
// Maximalanzahl fuer aufzuzeichnende Boltzmann-Wahrscheinlichkeiten bei Hauptsimulation
#define KMCVAL_TSETTINGS_MAX_RECORDANZ 10000

// --- TSimulation ---
// Maximale Laenge des Checkpoint-Pfads
#define KMCVAL_TSIMULATION_MAX_CHKPATHLENGTH 1000

// ************************* OUTPUT-STRINGS **************************** //

// Leerer string-Platzhalter
#define KMCOUT_NOSTRING "[]"
// Maximale offset-Leerstellen
#define KMCOUT_MAX_OFFSET 10

// --- TKMCJob ---
// Erkennungsstring fuer TKMCJob-Start
#define KMCOUT_TKMCJOB_START "<KMCJob>"
// Erkennungsstring fuer TKMCJob-Ende
#define KMCOUT_TKMCJOB_END "</KMCJob>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TKMCJOB_OFFSET 2
// Bezeichner fuer Projektname
#define KMCOUT_TKMCJOB_NAME "Projektname:"
// Bezeichner fuer Username
#define KMCOUT_TKMCJOB_USERNAME "User:"
// Bezeichner fuer Datum
#define KMCOUT_TKMCJOB_DATE "Erstellungsdatum:"
// Bezeichner fuer Projektpfad
#define KMCOUT_TKMCJOB_PATH "Projektpfad:"
// Bezeichner fuer Projektbeschreibungsstart
#define KMCOUT_TKMCJOB_DESCSTART "<Projektbeschreibung>"
// Bezeichner fuer Projektbeschreibungsende
#define KMCOUT_TKMCJOB_DESCEND "</Projektbeschreibung>"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TKMCJob
#define KMCOUT_TKMCJOB_COUT_PRECISION 6

// --- TElements ---
// Erkennungsstring fuer TElements-Start
#define KMCOUT_TELEMENTS_START "<ElementInfo>"
// Erkennungsstring fuer TElements-Ende
#define KMCOUT_TELEMENTS_END "</ElementInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TELEMENTS_OFFSET 2
// Bezeichner fuer Element
#define KMCOUT_TELEMENTS_ELEMENT "Element:"
// Bezeichner fuer Element-ID bei Stream-Ausgabe
#define KMCOUT_TELEMENTS_ELEMID "ID"
// Bezeichner fuer Symbol bei Stream-Ausgabe
#define KMCOUT_TELEMENTS_SYMBOL "Symbol"
// Bezeichner fuer Name bei Stream-Ausgabe
#define KMCOUT_TELEMENTS_NAME "Name"
// Bezeichner fuer Charge bei Stream-Ausgabe
#define KMCOUT_TELEMENTS_CHARGE "Ladung"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TElements
#define KMCOUT_TELEMENTS_COUT_PRECISION 6

// --- TStructure ---
// Erkennungsstring fuer TStructure-Start
#define KMCOUT_TSTRUCTURE_START "<StrukturInfo>"
// Erkennungsstring fuer TStructure-Ende
#define KMCOUT_TSTRUCTURE_END "</StrukturInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TSTRUCTURE_OFFSET 2
// Bezeichner fuer Strukturname
#define KMCOUT_TSTRUCTURE_NAME "Name:"
// Bezeichner fuer Basisvektoren
#define KMCOUT_TSTRUCTURE_BASIS "Basisvektor:"
// Bezeichner fuer Basisvektor a
#define KMCOUT_TSTRUCTURE_BASISA "a"
// Bezeichner fuer Basisvektor b
#define KMCOUT_TSTRUCTURE_BASISB "b"
// Bezeichner fuer Basisvektor c
#define KMCOUT_TSTRUCTURE_BASISC "c"
// Bezeichner fuer x-Koordinaten
#define KMCOUT_TSTRUCTURE_XCOORD "x"
// Bezeichner fuer y-Koordinaten
#define KMCOUT_TSTRUCTURE_YCOORD "y"
// Bezeichner fuer z-Koordinaten
#define KMCOUT_TSTRUCTURE_ZCOORD "z"
// Bezeichner fuer Atom
#define KMCOUT_TSTRUCTURE_ATOM "Atom:"
// Bezeichner fuer Atom-ID
#define KMCOUT_TSTRUCTURE_ATOMID "ID"
// Bezeichner fuer relative x-Koordinaten
#define KMCOUT_TSTRUCTURE_XREL "a"
// Bezeichner fuer relative y-Koordinaten
#define KMCOUT_TSTRUCTURE_YREL "b"
// Bezeichner fuer relative z-Koordinaten
#define KMCOUT_TSTRUCTURE_ZREL "c"
// Bezeichner fuer ElemID
#define KMCOUT_TSTRUCTURE_ELEMID "ElemID"
// Bezeichner fuer Dotierung
#define KMCOUT_TSTRUCTURE_DOPING "Dotierung:"
// Bezeichner fuer Dotierungs-ID
#define KMCOUT_TSTRUCTURE_DOPINGID "ID"
// Bezeichner fuer dotiertes Element
#define KMCOUT_TSTRUCTURE_DOPEDID "DotiertID"
// Bezeichner fuer Dopand
#define KMCOUT_TSTRUCTURE_DOPANDID "DopandID"
// Bezeichner fuer Dopand-Leerstellen-Verhaeltnis
#define KMCOUT_TSTRUCTURE_VACDOPRATIO "Vac/Dop"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TStructure
#define KMCOUT_TSTRUCTURE_COUT_PRECISION 6

// --- TJump ---
// Erkennungsstring 1 fuer TJump-Start
#define KMCOUT_TJUMP_START1 "<JumpInfo"
// Erkennungsstring 2 fuer TJump-Start
#define KMCOUT_TJUMP_START2 ">"
// Erkennungsstring fuer TJump-Ende
#define KMCOUT_TJUMP_END "</JumpInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TJUMP_OFFSET 2
// Bezeichner fuer Atom-ID
#define KMCOUT_TJUMP_ATOMID "Atom="
// Bezeichner fuer Richtungs-ID
#define KMCOUT_TJUMP_DIRID "Richtung="
// Bezeichner fuer Ruecksprung-DirID
#define KMCOUT_TJUMP_BACKJUMPDIRID "RuecksprungrichtungsID:"
// Bezeichner fuer UniqueJump-ID
#define KMCOUT_TJUMP_UNIQUEID "SprungID:"
// Wert fuer undefinierte UniqueJump-ID
#define KMCOUT_TJUMP_UNIQUEID_UNDEF "unbekannt"
// Bezeichner fuer Startatom
#define KMCOUT_TJUMP_STARTATOM "Startatom:"
// Bezeichner fuer Zielatom
#define KMCOUT_TJUMP_DESTATOM "Zielatom:"
// Bezeichner fuer Umgebungsatom
#define KMCOUT_TJUMP_ENVATOM "Umgebungsatom:"
// Bezeichner fuer Umgebungsatom-ID
#define KMCOUT_TJUMP_ENVATOMID "ID"
// Bezeichner fuer Atom-x-Koordinate in 4D
#define KMCOUT_TJUMP_X "a"
// Bezeichner fuer Atom-y-Koordinate in 4D
#define KMCOUT_TJUMP_Y "b"
// Bezeichner fuer Atom-z-Koordinate in 4D
#define KMCOUT_TJUMP_Z "c"
// Bezeichner fuer Atom-s-Koordinate in 4D
#define KMCOUT_TJUMP_S "s"
// Bezeichner fuer relative Atom-x-Koordinate in 4D
#define KMCOUT_TJUMP_RELX "rel. a"
// Bezeichner fuer relative Atom-y-Koordinate in 4D
#define KMCOUT_TJUMP_RELY "rel. b"
// Bezeichner fuer relative Atom-z-Koordinate in 4D
#define KMCOUT_TJUMP_RELZ "rel. c"
// Bezeichner fuer relative Atom-s-Koordinate in 4D
#define KMCOUT_TJUMP_RELS "rel. s"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TJump
#define KMCOUT_TJUMP_COUT_PRECISION 6

// --- TJumps ---
// Erkennungsstring fuer TJumps-Start
#define KMCOUT_TJUMPS_START "<JumpsInfo>"
// Erkennungsstring fuer TJumps-Ende
#define KMCOUT_TJUMPS_END "</JumpsInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TJUMPS_OFFSET 2
// Bezeichner fuer Sprungrichtungsschalen-Anzahl
#define KMCOUT_TJUMPS_MOVSHELLCOUNT "Sprungrichtungsschalenanzahl:"
// Bezeichner fuer Sprungumgebungsschalen-Anzahl
#define KMCOUT_TJUMPS_JUMPSTARTSHELLCOUNT "Sprungstart-Umgebungsschalenanzahl:"
// Bezeichner fuer Sprungumgebungsschalen-Anzahl
#define KMCOUT_TJUMPS_JUMPTSSHELLCOUNT "Sprungmitte-Umgebungsschalenanzahl:"
// Bezeichner fuer Sprungumgebungsschalen-Anzahl
#define KMCOUT_TJUMPS_JUMPDESTSHELLCOUNT "Sprungziel-Umgebungsschalenanzahl:"
// Bezeichner fuer Sprunganzahl
#define KMCOUT_TJUMPS_JUMPCOUNT "Sprunganzahl:"
// Bezeichner fuer AtomID
#define KMCOUT_TJUMPS_ATOMID "AtomID"
// Bezeichner fuer Anzahl
#define KMCOUT_TJUMPS_COUNT "Anzahl"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TJumps
#define KMCOUT_TJUMPS_COUT_PRECISION 4

// --- TUniqueJump ---
// Erkennungsstring 1 fuer TUniqueJump-Start
#define KMCOUT_TUNIQUEJUMP_START1 "<UniqueJumpInfo"
// Erkennungsstring 2 fuer TUniqueJump-Start
#define KMCOUT_TUNIQUEJUMP_START2 ">"
// Erkennungsstring fuer TUniqueJump-Ende
#define KMCOUT_TUNIQUEJUMP_END "</UniqueJumpInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TUNIQUEJUMP_OFFSET 2
// Bezeichner fuer UniqueJump-ID
#define KMCOUT_TUNIQUEJUMP_JUMPID "UniqueJump="
// Bezeichner fuer Sprunglänge
#define KMCOUT_TUNIQUEJUMP_LENGTH "Laenge:"
// Bezeichner fuer Sprungaktivierungszustand
#define KMCOUT_TUNIQUEJUMP_JUMPACTIVE "Sprung-Zustand:"
// true-Bezeichner fuer JumpActive == true
#define KMCOUT_TUNIQUEJUMP_JUMPACTIVE_TRUE "aktiv"
// false-Bezeichner fuer JumpActive == false
#define KMCOUT_TUNIQUEJUMP_JUMPACTIVE_FALSE "inaktiv"
// Bezeichner fuer Umgebungsatom
#define KMCOUT_TUNIQUEJUMP_COORD "Umgebungsatom:"
// Bezeichner fuer CoordID
#define KMCOUT_TUNIQUEJUMP_COORDID "ID"
// Bezeichner fuer ElemID
#define KMCOUT_TUNIQUEJUMP_ELEMID "ElemID"
// Bezeichner fuer x-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
#define KMCOUT_TUNIQUEJUMP_X "x"
// Bezeichner fuer y-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
#define KMCOUT_TUNIQUEJUMP_Y "y"
// Bezeichner fuer z-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
#define KMCOUT_TUNIQUEJUMP_Z "z"
// Bezeichner fuer Atomignorierungszustand
#define KMCOUT_TUNIQUEJUMP_ATOMIGNORE "Ignoriert"
// true-Bezeichner fuer AtomIgnore == true
#define KMCOUT_TUNIQUEJUMP_ATOMIGNORE_TRUE "ja"
// false-Bezeichner fuer AtomIgnore == false
#define KMCOUT_TUNIQUEJUMP_ATOMIGNORE_FALSE "nein"
// Bezeichner fuer Atomadditivitätszustand
#define KMCOUT_TUNIQUEJUMP_ATOMADDITIVE "Additiv"
// true-Bezeichner fuer AtomActive == true
#define KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_TRUE "ja"
// false-Bezeichner fuer AtomActive == false
#define KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_FALSE "nein"
// Bezeichner fuer Atomaktivierungszustand
#define KMCOUT_TUNIQUEJUMP_ATOMACTIVE "Aktiv"
// true-Bezeichner fuer AtomActive == true
#define KMCOUT_TUNIQUEJUMP_ATOMACTIVE_TRUE "ja"
// false-Bezeichner fuer AtomActive == false
#define KMCOUT_TUNIQUEJUMP_ATOMACTIVE_FALSE "nein"
// Bezeichner fuer WW-Umgebungsatom
#define KMCOUT_TUNIQUEJUMP_INTERACTIONATOM "WW-Atom:"
// Bezeichner fuer WW-Atom-ID
#define KMCOUT_TUNIQUEJUMP_INTERACTIONATOM_ID "ID"
// Bezeichner fuer WW-Map
#define KMCOUT_TUNIQUEJUMP_INTERACTIONMAP "WWID"
// Bezeichner fuer WW-Energie-Anzahl
#define KMCOUT_TUNIQUEJUMP_INTERACTIONENERGYCOUNT "Anz."
// Bezeichner fuer WW-Energie-ElemID
#define KMCOUT_TUNIQUEJUMP_INTERACTIONELEMID "ElemID"
// Bezeichner fuer WW-Energie
#define KMCOUT_TUNIQUEJUMP_INTERACTIONENERGY "Energie"
// Bezeichner fuer UniqueCodes
#define KMCOUT_TUNIQUEJUMP_UNIQUECODE "Unique-Codierung:"
// Bezeichner fuer UniqueJumpID
#define KMCOUT_TUNIQUEJUMP_UNIQUEID "ID"
// Bezeichner fuer UniqueEnergy
#define KMCOUT_TUNIQUEJUMP_UNIQUEENERGY "Aktivierungsenergie"
// Bezeichner fuer Codes
#define KMCOUT_TUNIQUEJUMP_CODE "Code"
// Bezeichner fuer FullCodes
#define KMCOUT_TUNIQUEJUMP_FULLCODE "Codierung:"
// Bezeichner fuer FullCodeID
#define KMCOUT_TUNIQUEJUMP_FULLCODEID "ID"
// Bezeichner fuer FullToUniqueMap
#define KMCOUT_TUNIQUEJUMP_FULLTOUNIQUEMAP "Unique-ID"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TUniqueJump
#define KMCOUT_TUNIQUEJUMP_COUT_PRECISION 6 

// --- TInteraction ---
// Erkennungsstring 1 fuer TInteraction-Start
#define KMCOUT_TINTERACTION_START1 "<InteractionInfo"
// Erkennungsstring 2 fuer TInteraction-Start
#define KMCOUT_TINTERACTION_START2 ">"
// Erkennungsstring fuer TInteraction-Ende
#define KMCOUT_TINTERACTION_END "</InteractionInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TINTERACTION_OFFSET 2
// Bezeichner fuer Interaction-ID
#define KMCOUT_TINTERACTION_ID "Interaction="
// Bezeichner fuer ElemID
#define KMCOUT_TINTERACTION_WWELEMID "Umgebungsatom-ElemID:"
// Bezeichner fuer StartDist
#define KMCOUT_TINTERACTION_STARTDIST "Sprungstart-Abstand:"
// Bezeichner fuer DestDist
#define KMCOUT_TINTERACTION_DESTDIST "Sprungziel-Abstand:"
// Bezeichner fuer Start-VV-WW-Umgebungsatom
#define KMCOUT_TINTERACTION_STARTWW "Sprungstart-WW-Umgebungsatom:"
// Bezeichner fuer Umgebungsatom-ID
#define KMCOUT_TINTERACTION_WWID "ID"
// Bezeichner fuer Umgebungsatom-ElemID
#define KMCOUT_TINTERACTION_ELEMID "ElemID"
// Bezeichner fuer x-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
#define KMCOUT_TINTERACTION_X "x"
// Bezeichner fuer y-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
#define KMCOUT_TINTERACTION_Y "y"
// Bezeichner fuer z-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
#define KMCOUT_TINTERACTION_Z "z"
// Bezeichner fuer Ziel-WW-Umgebungsatom
#define KMCOUT_TINTERACTION_DESTWW "Sprungziel-WW-Umgebungsatom:"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TInteraction
#define KMCOUT_TINTERACTION_COUT_PRECISION 6

// --- TUniqueJumps ---
// Erkennungsstring fuer TUniqueJumps-Start
#define KMCOUT_TUNIQUEJUMPS_START "<UniqueJumpsInfo>"
// Erkennungsstring fuer TUniqueJumps-Ende
#define KMCOUT_TUNIQUEJUMPS_END "</UniqueJumpsInfo>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TUNIQUEJUMPS_OFFSET 2
// Bezeichner fuer Leerstellen-Leerstellen-Wechselwirkungsschalen-Anzahl
#define KMCOUT_TUNIQUEJUMPS_INTERACTIONSHELLCOUNT "WW-Schalenanzahl:"
// Bezeichner, ob Codierung erfolgt ist
#define KMCOUT_TUNIQUEJUMPS_CODESREADY "Codierung:"
// Bezeichner, dass Codierung erfolgt ist
#define KMCOUT_TUNIQUEJUMPS_CODESREADY_TRUE "ja"
// Bezeichner, dass Codierung nicht erfolgt ist
#define KMCOUT_TUNIQUEJUMPS_CODESREADY_FALSE "nein"
// Anzahl der UJump
#define KMCOUT_TUNIQUEJUMPS_UJUMPCOUNT "Sprunganzahl:"
// Anzahl der VVShells
#define KMCOUT_TUNIQUEJUMPS_INTERACTIONCOUNT "WW-Anzahl:"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TUniqueJumps
#define KMCOUT_TUNIQUEJUMPS_COUT_PRECISION 6

// -- TSettings ---
// Erkennungsstring fuer TSettings-Start
#define KMCOUT_TSETTINGS_START "<Settings>"
// Erkennungsstring fuer TSettings-Ende
#define KMCOUT_TSETTINGS_END "</Settings>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TSETTINGS_OFFSET 2
// Bezeichner fuer Simulationstemperatur
#define KMCOUT_TSETTINGS_TEMPERATURE "Temperatur:"
// Bezeichner fuer Sprungversuchsfrequenz
#define KMCOUT_TSETTINGS_ATTEMPTFREQUENCY "Sprungversuchsfrequenz:"
// Bezeichner fuer E-Feld-Staerke [in kT]
#define KMCOUT_TSETTINGS_EFIELDSTRENGTH "E-Feld-Staerke:"
// Bezeichner fuer E-Feld
#define KMCOUT_TSETTINGS_EFIELD "E-Feld:"
// Bezeichner fuer E-Feld x-Koordinate
#define KMCOUT_TSETTINGS_EFIELDX "x"
// Bezeichner fuer E-Feld y-Koordinate
#define KMCOUT_TSETTINGS_EFIELDY "y"
// Bezeichner fuer E-Feld z-Koordinate
#define KMCOUT_TSETTINGS_EFIELDZ "z"
// Bezeichner fuer Gittergroesse
#define KMCOUT_TSETTINGS_LATTICESIZE "Gittergroesse:"
// Bezeichner fuer Zusatzleerstellen
#define KMCOUT_TSETTINGS_ADDVACANZ "Zusatzleerstellen:"
// Bezeichner fuer Leerstellengesamtkonzentration
#define KMCOUT_TSETTINGS_TOTALVACCONC "Leerstellengesamtkonzentration:"
// Bezeichner fuer Leerstellengesamtanzahl
#define KMCOUT_TSETTINGS_TOTALVACANZ "Leerstellengesamtanzahl:"
// Bezeichner fuer Volumenkonzentration der beweglichen Spezies [1/cm^3]
#define KMCOUT_TSETTINGS_MOVVOLCONC "Mov-Volumenkonzentration:"
// Bezeichner fuer Dopandenkonzentration
#define KMCOUT_TSETTINGS_CONCENTRATION "Dopandenkonzentration:"
// Bezeichner fuer Dotierung
#define KMCOUT_TSETTINGS_DOPING "DotierungID"
// Bezeichner fuer Konzentration
#define KMCOUT_TSETTINGS_CONC "Konz."
// Bezeichner fuer Anzahl
#define KMCOUT_TSETTINGS_ANZ "Anzahl"
// Bezeichner fuer Checkpoint-Schreiben
#define KMCOUT_TSETTINGS_WRITECHK "Checkpoint-Speichern:"
// Bezeichner fuer Checkpoint-Laden
#define KMCOUT_TSETTINGS_LOADCHK "Checkpoint-Laden:"
// Bezeichner fuer DoPrerun-Flag
#define KMCOUT_TSETTINGS_DOPRERUN "Vorlauf:"
// Bezeichner fuer "durchfuehren"
#define KMCOUT_TSETTINGS_DO_TRUE "ja"
// Bezeichner fuer "nicht durchfuehren"
#define KMCOUT_TSETTINGS_DO_FALSE "nein"
// Bezeichner fuer Vorlauf-MCSP
#define KMCOUT_TSETTINGS_PREMCSP "Vorlauf-MCSP:"
// Bezeichner fuer Vorlauf-Wahrscheinlichkeitsliste
#define KMCOUT_TSETTINGS_PRERECORDANZ "Vorlauf-Wahrscheinlichkeitsliste:"
// Bezeichner fuer DoDynNorm-Flag
#define KMCOUT_TSETTINGS_DODYNNORM "DynNormierung:"
// Bezeichner fuer DynNorm-Attempt-Anzahl
#define KMCOUT_TSETTINGS_DYNATTEMPTANZ "DynNorm-Sprungversuche:"
// Bezeichner fuer DynNorm-Wahrscheinlichkeitsliste
#define KMCOUT_TSETTINGS_DYNRECORDANZ "DynNorm-Wahrscheinlichkeitsliste:"
// Bezeichner fuer DynNorm-Wahrscheinlichkeitseintrag
#define KMCOUT_TSETTINGS_DYNNORMNUM "DynNorm-Wahrscheinlichkeitseintrag:"
// Bezeichner fuer MCSP der Hauptsimulation
#define KMCOUT_TSETTINGS_MAINMCSP "MCSP:"
// Bezeichner fuer Wahrscheinlichkeitsliste der Hauptsimulation
#define KMCOUT_TSETTINGS_MAINRECORDANZ "Wahrscheinlichkeitsliste:"
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TSettings
#define KMCOUT_TSETTINGS_COUT_PRECISION 6

// -- TSimulation ---
// Erkennungsstring fuer TSimulation-Checkpoint-Start
#define KMCOUT_TSIMULATION_CHK_START "<Checkpoint>"
// Erkennungsstring fuer TSimulation-Checkpoint-Ende
#define KMCOUT_TSIMULATION_CHK_END "</Checkpoint>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TSIMULATION_CHK_OFFSET 2
// Relativer Offset des untergeordneten Phasen-Outputs
#define KMCOUT_TSIMULATION_PHASE_OFFSET 2
// Erkennungsstring fuer Startgitter-Start
#define KMCOUT_TSIMULATION_STARTLATTICE_START "<Startgitterbesetzung>"
// Erkennungsstring fuer TSimPhaseInfo-Gitter-Ende
#define KMCOUT_TSIMULATION_STARTLATTICE_END "</Startgitterbesetzung>"
// Erkennungsstring fuer TSimulation-PrerunPhase-Start
#define KMCOUT_TSIMULATION_PRERUN_START "<Prerun-Phase>"
// Erkennungsstring fuer TSimulation-PrerunPhase-Ende
#define KMCOUT_TSIMULATION_PRERUN_END "</Prerun-Phase>"
// Erkennungsstring fuer TSimulation-DynNormPhase-Start
#define KMCOUT_TSIMULATION_DYNNORM_START "<DynNorm-Phase>"
// Erkennungsstring fuer TSimulation-DynNormPhase-Ende
#define KMCOUT_TSIMULATION_DYNNORM_END "</DynNorm-Phase>"
// Erkennungsstring fuer TSimulation-MainPhase-Start
#define KMCOUT_TSIMULATION_MAIN_START "<Main-Phase>"
// Erkennungsstring fuer TSimulation-MainPhase-Ende
#define KMCOUT_TSIMULATION_MAIN_END "</Main-Phase>"

// -- TSimPhaseInfo ---
// Bezeichner fuer die aktuellen MCSP der Phase
#define KMCOUT_TSIMPHASEINFO_MCSP "MCSP:"
// Bezeichner fuer die aktuellen Sprungversuche der Phase
#define KMCOUT_TSIMPHASEINFO_JUMPATTEMPTS "Sprungversuche:"
// Bezeichner fuer die aktuellen Site-Blocking der Phase
#define KMCOUT_TSIMPHASEINFO_SITEBLOCKING "Site-Blocking:"
// Bezeichner fuer die aktuellen Nonsense-Sprungversuche der Phase
#define KMCOUT_TSIMPHASEINFO_NONSENSE "Undef.-Zielplatz(Eakt,rueck<=0):"
// Bezeichner fuer die aktuellen Overkill-Sprungversuche der Phase
#define KMCOUT_TSIMPHASEINFO_OVERKILL "Undef.-Startplatz(Eakt,hin<=0):"
// Bezeichner fuer die aktuellen Normierung der Phase
#define KMCOUT_TSIMPHASEINFO_NORM "Normierung:"
// Bezeichner fuer die Summe des Attempt-Pfad-Quotienten der Phase
#define KMCOUT_TSIMPHASEINFO_TIMESUM "Versuch-Pfad-Quotient:"
// Bezeichner fuer die aktuellen Laufzeit der Phase
#define KMCOUT_TSIMPHASEINFO_RUNTIME "Laufzeit:"
// Erkennungsstring fuer TSimPhaseInfo-Gitter-Start
#define KMCOUT_TSIMPHASEINFO_LATTICE_START "<Gitterbesetzung>"
// Erkennungsstring fuer TSimPhaseInfo-Gitter-Ende
#define KMCOUT_TSIMPHASEINFO_LATTICE_END "</Gitterbesetzung>"
// Erkennungsstring fuer TSimPhaseInfo-Bewegungsspeicherung-Start
#define KMCOUT_TSIMPHASEINFO_MOVLATTICE_START "<Atombewegung>"
// Erkennungsstring fuer TSimPhaseInfo-Bewegungsspeicherung-Ende
#define KMCOUT_TSIMPHASEINFO_MOVLATTICE_END "</Atombewegung>"
// Erkennungsstring fuer TSimPhaseInfo-Sprungversuchswahrscheinlichkeiten-Start
#define KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_START "<Sprungversuchswahrscheinlichkeiten>"
// Erkennungsstring fuer TSimPhaseInfo-Sprungversuchswahrscheinlichkeiten-Ende
#define KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_END "</Sprungversuchswahrscheinlichkeiten>"
// Erkennungsstring fuer TSimPhaseInfo-Sprungversuchswahrscheinlichkeiten-Start
#define KMCOUT_TSIMPHASEINFO_MCSPROB_START "<MCS-Wahrscheinlichkeiten>"
// Erkennungsstring fuer TSimPhaseInfo-Sprungversuchswahrscheinlichkeiten-Ende
#define KMCOUT_TSIMPHASEINFO_MCSPROB_END "</MCS-Wahrscheinlichkeiten>"

// -- TResults ---
// Erkennungsstring fuer TResults-Start
#define KMCOUT_TRESULTS_START "<Ergebnisse>"
// Erkennungsstring fuer TResults-Ende
#define KMCOUT_TRESULTS_END "</Ergebnisse>"
// Relativer Offset des untergeordneten Outputs
#define KMCOUT_TRESULTS_OFFSET 2
// Bezeichner fuer Conductivity
#define KMCOUT_TRESULTS_COND "Leitfaehigkeit:"
// Bezeichner fuer SimulatedTime
#define KMCOUT_TRESULTS_SIMTIME "Zeitspanne:"
// Bezeichner fuer MCSP
#define KMCOUT_TRESULTS_MCSP "MCSP:"
// Bezeichner fuer JumpAttempts
#define KMCOUT_TRESULTS_JUMPATTEMPTS "Sprungversuche:"
// Bezeichner fuer NonsenseJumpAttempts
#define KMCOUT_TRESULTS_NONSENSEATTEMPTS "Undef.-Zielplatz(Eakt,rueck<=0):"
// Bezeichner fuer OverkillJumpAttempts
#define KMCOUT_TRESULTS_OVERKILLATTEMPTS "Undef.-Startplatz(Eakt,hin<=0):"
// Bezeichner fuer SiteBlocking
#define KMCOUT_TRESULTS_SITEBLOCKING "Site-Blocking:"
// Bezeichner fuer Normierung
#define KMCOUT_TRESULTS_NORMALIZATION "Normierung:"
// Bezeichner fuer Laufzeit
#define KMCOUT_TRESULTS_RUNTIME "Laufzeit:"
// Bezeichner fuer mittlere Sprunganzahl (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_MEANJUMPCOUNT "Mov-<Sprunganzahl>:"
// Bezeichner fuer mittleren Verschiebungsvektor (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_MEANDISPVEC "Mov-(<x>,<y>,<z>):"
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors parallel zum E-Feld (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_DISPPARALLEL "Mov-Verschiebungskomp.(parallel):"
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors senkrecht zum E-Feld (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_DISPPERPENDICULAR "Mov-Verschiebungskomp.(senkrecht):"
// Bezeichner fuer mittlere Verschiebung (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_MEANDISP "Mov-<sqrt(x*x+y*y+z*z)>:"
// Bezeichner fuer mittlere quadratische Verschiebung (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_MEANSQUAREDDISP "Mov-<x*x+y*y+z*z>:"
// Bezeichner fuer Schwerpunktverschiebungsvektor (bewegliche Spezies)
#define KMCOUT_TRESULTS_MOV_COMDISPVEC "Mov-(sum(x),sum(y),sum(z)):"
// Bezeichner fuer mittlere Sprunganzahl (Leerstellen)
#define KMCOUT_TRESULTS_VAC_MEANJUMPCOUNT "Vac-<Sprunganzahl>:"
// Bezeichner fuer mittleren Verschiebungsvektor (Leerstellen)
#define KMCOUT_TRESULTS_VAC_MEANDISPVEC "Vac-(<x>,<y>,<z>):"
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors parallel zum E-Feld (Leerstellen)
#define KMCOUT_TRESULTS_VAC_DISPPARALLEL "Vac-Verschiebungskomp.(parallel):"
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors senkrecht zum E-Feld (Leerstellen)
#define KMCOUT_TRESULTS_VAC_DISPPERPENDICULAR "Vac-Verschiebungskomp.(senkrecht):"
// Bezeichner fuer mittlere Verschiebung (Leerstellen)
#define KMCOUT_TRESULTS_VAC_MEANDISP "Vac-<sqrt(x*x+y*y+z*z)>:"
// Bezeichner fuer mittlere quadratische Verschiebung (Leerstellen)
#define KMCOUT_TRESULTS_VAC_MEANSQUAREDDISP "Vac-<x*x+y*y+z*z>:"
// Bezeichner fuer Schwerpunktverschiebungsvektor (Leerstellen)
#define KMCOUT_TRESULTS_VAC_COMDISPVEC "Vac-(sum(x),sum(y),sum(z)):"

// ************************ EXCEPTION-STRINGS ************************** //

// Text vor Standard-Exception
#define KMCERRSTR_STDERR "Exception occured:"
// Text bei unbekannter Exception
#define KMCERRSTR_UNKNOWNERR "Unknown exception occured!"

// ************************* Hilfsfunktionen *************************** //

// Funktion zum Trimmen von strings (d.h. Leerstellen an Anfang und Ende entfernen)
string Trim (string input);

// Funktion zum Trimmen von strings von der rechten Seite (d.h. Leerstellen am Ende entfernen)
string RightTrim (string input);

// Funktion zum Ueberpruefen eines string, keine allowed chars = alle erlaubt
bool StringCheck (string input, int min_length, int max_length, string allowed_chars = "");

// Funktion zur Umwandlung eines int in einen string
string IntToStr (int input);

// Funktion zur Umwandlung eines long long in einen string
string LongLongToStr (long long input);

// Funktion zur Umwandlung eines unsigned long long in einen string
string UnsignedLongLongToStr (unsigned long long input);

// Funktion zur Umwandlung eines double in einen string
string DoubleToStr (double input);

// Funktion zur Umwandlung eines string in einen int
int StrToInt (string input, int error_val = -1);

// Funktion zur Umwandlung eines string in einen long long
long long StrToLongLong (string input, long long error_val = -1LL);

// Funktion zur Umwandlung eines string in einen unsigned long long
unsigned long long StrToUnsignedLongLong (string input, unsigned long long error_val = 0ULL);

// Funktion zur Umwandlung eines string in einen double
double StrToDouble (string input, double error_val = 0.0);

// Funktion zur Umwandlung eines string in einen int
bool ConvStrToInt (string input, int &val);

// Funktion zur Umwandlung eines string in einen long long
bool ConvStrToLongLong (string input, long long &val);

// Funktion zur Umwandlung eines string in einen unsigned long long
bool ConvStrToUnsignedLongLong (string input, unsigned long long &val);

// Funktion zur Umwandlung eines string in einen double
bool ConvStrToDouble (string input, double &val);

/*
// Test equality of two double values
bool DoubleEqual(const double d1, const double d2);

// Test unequality of two double values
bool DoubleUnequal(const double d1, const double d2);

// Test if first double value is greater than the second one
bool DoubleGreater(const double d1, const double d2);

// Test if first double value is smaller than the second one
bool DoubleSmaller(const double d1, const double d2);

// Test equality of two double values with periodic boundary conditions
bool DoubleEqualPeriodic(const double lower_bound, const double d1, const double d2, const double higher_bound);

// Test inequality of two double values with periodic boundary conditions
bool DoubleUnequalPeriodic(const double lower_bound, const double d1, const double d2, const double higher_bound);
*/

// Vergleich zweier Winkel w1 und w2 auf einem Kreis
bool CirclePeriodicCompare(double w1, double w2);

// Programmverzeichnis ohne Slash bzw. Backslash am Ende ermitteln
bool GetApplicationPath(string &out_str);

// Prepend application path to relative file name
string PrependAppPath(string rel_filename);

#endif