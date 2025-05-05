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
#include <string_view>
#include <array>

using namespace std::literals;

// **************************** VERSION ******************************** //

// Programmversion:
[[maybe_unused]] constexpr std::string_view KMC_VERSION = "0.9.4.0"sv;
// Bezeichner fuer die Programmversion
[[maybe_unused]] constexpr std::string_view KMCOUT_VERSION = "Version:"sv;
// Kompatible Dateiversionen (;-getrennt)
[[maybe_unused]] constexpr std::string_view KMC_COMP_FILE_VERSIONS = "0.9.0.5;0.9.1.0;0.9.2.0;0.9.3.0;0.9.4.0"sv;

// ************************** NATURKONSTANTEN ************************** //

// Pi
[[maybe_unused]] constexpr double NATCONST_PI = 3.14159265358979;
// Boltzmann-Konstante in eV/K
[[maybe_unused]] constexpr double NATCONST_KB = 0.000086173325;
// Eulersche Zahl e
[[maybe_unused]] constexpr double NATCONST_E = 2.718281828459;
// Elementarladung e in C
[[maybe_unused]] constexpr double NATCONST_ECHARGE = 1.602176565E-19;

// **************************** DATEINAMEN ***************************** //

// Datei fuer Elementbeschreibungen
[[maybe_unused]] constexpr std::string_view KMCPATH_ELEMDESC = "ElementsDrawInfo.ini"sv;
// Dateiendung fuer Checkpoint-Dateien
[[maybe_unused]] constexpr std::string_view KMCPATH_CHK_EXT = ".mcp"sv;
// Standard-Summary-Dateiname
[[maybe_unused]] constexpr std::string_view KMCPATH_DEFAULT_SUMMARY = "Summary.csv"sv;

// *************************** STANDARDWERTE *************************** //

// Init-Symbol fuer Moving Species
[[maybe_unused]] constexpr std::string_view KMCVAR_MOVSYMBOL = "H"sv;
// Init-Name fuer Moving Species
[[maybe_unused]] constexpr std::string_view KMCVAR_MOVNAME = ""sv;
// Init-Ladung fuer Moving Species
[[maybe_unused]] constexpr double KMCVAR_MOVCHARGE = 0.0;
// Symbol fuer Vacancies
[[maybe_unused]] constexpr std::string_view KMCVAR_VACSYMBOL = "Vc"sv;
// Name fuer Vacancies
[[maybe_unused]] constexpr std::string_view KMCVAR_VACNAME = ""sv;
// Ladung fuer Vacancies
[[maybe_unused]] constexpr double KMCVAR_VACCHARGE = 0.0;
// Symbol fuer beliebige Spezies
[[maybe_unused]] constexpr std::string_view KMCVAR_ARBITRARYSYMBOL = "XX"sv;
// Zero-Threshold fuer Vektoren
[[maybe_unused]] constexpr double KMCVAR_ZEROTHRESHOLD_VEC = 0.000001;
// ==-Threshold fuer Vektoren
[[maybe_unused]] constexpr double KMCVAR_EQTHRESHOLD_VEC = 0.0001;
// ==-Threshold fuer Ladungen
[[maybe_unused]] constexpr double KMCVAR_EQTHRESHOLD_CHARGE = 0.01;
// ==-Threshold fuer Schalen (NN-Analyse)
[[maybe_unused]] constexpr double KMCVAR_EQTHRESHOLD_SHELL = 0.0001;
// ==-Threshold (prozentual) fuer Datenvergleich beim Laden
[[maybe_unused]] constexpr double KMCVAR_EQTHRESHOLD_PERC_LOAD = 0.001;
// Standardprojektname
[[maybe_unused]] constexpr std::string_view KMCVAR_PROJECTNAME_INIT = "New Project"sv;
// Standardusername
[[maybe_unused]] constexpr std::string_view KMCVAR_USERNAME_INIT = ""sv;
// Standardprojektname
[[maybe_unused]] constexpr std::string_view KMCVAR_PROJECTDESC_INIT = ""sv;
// Standardstrukturname
[[maybe_unused]] constexpr std::string_view KMCVAR_STRUCTURENAME_INIT = "New Structure"sv;
// Standard-MovShellCount
[[maybe_unused]] constexpr int KMCVAR_MOVSHELL_INIT = 1;
// Standard-JumpStartShellCount
[[maybe_unused]] constexpr int KMCVAR_JUMPSTARTSHELL_INIT = 1;
// Standard-JumpTSShellCount
[[maybe_unused]] constexpr int KMCVAR_JUMPTSSHELL_INIT = 0;
// Standard-JumpDestShellCount
[[maybe_unused]] constexpr int KMCVAR_JUMPDESTSHELL_INIT = 1;
// Standard-InteractionShellCount
[[maybe_unused]] constexpr int KMCVAR_INTERACTIONSHELL_INIT = 0;
// Standard-Temperatur
[[maybe_unused]] constexpr double KMCVAR_TEMPERATURE_INIT = 300.0;
// Standard-Sprungfrequenz
[[maybe_unused]] constexpr double KMCVAR_ATTEMPTFREQ_INIT = 1.0E+13;
// Standard-E-Feld-Beitrag
[[maybe_unused]] constexpr double KMCVAR_EFIELDMAGNITUDE_INIT = 0.1;
// Standard-E-Feld-Koordinate x
[[maybe_unused]] constexpr double KMCVAR_EFIELDX_INIT = 1.0;
// Standard-E-Feld-Koordinate y
[[maybe_unused]] constexpr double KMCVAR_EFIELDY_INIT = 0.0;
// Standard-E-Feld-Koordinate z
[[maybe_unused]] constexpr double KMCVAR_EFIELDZ_INIT = 0.0;
// Standard-Gittergroesse
[[maybe_unused]] constexpr int KMCVAR_LATTICESIZE_INIT = 16;
// Standard-MCSP
[[maybe_unused]] constexpr long long KMCVAR_MCSP_INIT = 10000LL;
// Double-Precision für Jobspeicherung (min. 17 ist nötig, weil 17 signifikante Stellen (Mantisse) fuer die Unterscheidung aller double-Werte noetig sind, dadurch sind Wertveränderungen beim Speichern und Laden ausgeschlossen)
[[maybe_unused]] constexpr int KMCVAR_SAVEFILE_DOUBLEPRECISION = 17;
// ==-Threshold fuer Wahrscheinlichkeiten (plus-Multiplikator)
[[maybe_unused]] constexpr double KMCVAR_EQTHRESHOLD_PROBPLUS = 1.0001;
// ==-Threshold fuer Wahrscheinlichkeiten (minus-Multiplikator)
[[maybe_unused]] constexpr double KMCVAR_EQTHRESHOLD_PROBMINUS = 0.9999;

// *************************** ANZAHLENLIMITS ************************** //

// Maximale Anzahl an Elementen
[[maybe_unused]] constexpr int KMCLIM_MAX_ELEMENTS = 20;
// Maximale Anzahl an Atomen in der Elementarzelle
[[maybe_unused]] constexpr int KMCLIM_MAX_ATOMS_IN_UC = 500;
// Maximale Anzahl an Dotierungen
[[maybe_unused]] constexpr int KMCLIM_MAX_DOPING = 10;
// Maximale Anzahl an Umgebungspermutationen
[[maybe_unused]] constexpr unsigned long long KMCLIM_MAX_ENVPERMUT = 200000ULL;

// ************************** VARIABLENLIMITS ************************** //

// --- TKMCJob ---
// Minimale ProjectName-Laenge
[[maybe_unused]] constexpr int KMCVAL_TKMCJOB_PROJECTNAME_MINLENGTH = 2;
// Maximale ProjectName-Laenge
[[maybe_unused]] constexpr int KMCVAL_TKMCJOB_PROJECTNAME_MAXLENGTH = 100;
// Erlaubte Buchstaben in ProjectName
[[maybe_unused]] constexpr std::string_view KMCVAL_TKMCJOB_PROJECTNAME_ALLOWED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-^() "sv;
// Minimale UserName-Laenge
[[maybe_unused]] constexpr int KMCVAL_TKMCJOB_USERNAME_MINLENGTH = 0;
// Maximale UserName-Laenge
[[maybe_unused]] constexpr int KMCVAL_TKMCJOB_USERNAME_MAXLENGTH = 100;
// Erlaubte Buchstaben in UserName
[[maybe_unused]] constexpr std::string_view KMCVAL_TKMCJOB_USERNAME_ALLOWED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"sv;
// Minimale ProjectDescription-Laenge
[[maybe_unused]] constexpr int KMCVAL_TKMCJOB_PROJECTDESC_MINLENGTH = 0;
// Maximale ProjectDescription-Laenge
[[maybe_unused]] constexpr int KMCVAL_TKMCJOB_PROJECTDESC_MAXLENGTH = 10000;
// Erlaubte Buchstaben in ProjectDescription
[[maybe_unused]] constexpr std::string_view KMCVAL_TKMCJOB_PROJECTDESC_ALLOWED_CHARS = ""sv;

// --- TElements ---
// Minimale Symbol-Laenge
[[maybe_unused]] constexpr int KMCVAL_TELEMENTS_SYMBOL_MINLENGTH = 1;
// Maximale Symbol-Laenge
[[maybe_unused]] constexpr int KMCVAL_TELEMENTS_SYMBOL_MAXLENGTH = 2;
// Erlaubte Buchstaben in Symbol
[[maybe_unused]] constexpr std::string_view KMCVAL_TELEMENTS_SYMBOL_ALLOWED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"sv;
// Minimale Name-Laenge
[[maybe_unused]] constexpr int KMCVAL_TELEMENTS_NAME_MINLENGTH = 0;
// Maximale Name-Laenge
[[maybe_unused]] constexpr int KMCVAL_TELEMENTS_NAME_MAXLENGTH = 20;
// Erlaubte Buchstaben in Name
[[maybe_unused]] constexpr std::string_view KMCVAL_TELEMENTS_NAME_ALLOWED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_()"sv;
// Minimale Ladung
[[maybe_unused]] constexpr double KMCVAL_TELEMENTS_CHARGE_MIN = -1000.0;
// Maximale Ladung
[[maybe_unused]] constexpr double KMCVAL_TELEMENTS_CHARGE_MAX = 1000.0;

// --- TStructure ---
// Minimale Strukturnamens-Laenge
[[maybe_unused]] constexpr int KMCVAL_TSTRUCTURE_NAME_MINLENGTH = 1;
// Maximale Strukturnamens-Laenge
[[maybe_unused]] constexpr int KMCVAL_TSTRUCTURE_NAME_MAXLENGTH = 100;
// Erlaubte Buchstaben in Strukturnamen
[[maybe_unused]] constexpr std::string_view KMCVAL_TSTRUCTURE_NAME_ALLOWED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-^() "sv;
// Maximale Gitterkonstante
[[maybe_unused]] constexpr double KMCVAL_TSTRUCTURE_LENGTHS_MAX = 100.0;
// Minimales Leerstellen-pro-Dopand-Verhaeltnis
[[maybe_unused]] constexpr double KMCVAL_TSTRUCTURE_VACDOPRATIO_MIN = -10.0;
// Maximales Leerstellen-pro-Dopand-Verhaeltnis
[[maybe_unused]] constexpr double KMCVAL_TSTRUCTURE_VACDOPRATIO_MAX = 10.0;

// --- TJumps ---
// Maximale Anzahl an Sprungziel-Schalen
[[maybe_unused]] constexpr int KMCVAL_TJUMPS_MAX_MOVSHELLCOUNT = 5;
// Maximale Anzahl an Sprungumgebungs-Schalen
[[maybe_unused]] constexpr int KMCVAL_TJUMPS_MAX_JUMPSHELLCOUNT = 150;

// --- TUniqueJumps ---
// Maximale Anzahl an WW-Schalen
[[maybe_unused]] constexpr int KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONSHELLCOUNT = 5;
// Minimale Energie für Wechselwirkung
[[maybe_unused]] constexpr double KMCVAL_TUNIQUEJUMPS_MIN_INTERACTIONENERGY = -1000.0;
// Maximale Energie für Wechselwirkung
[[maybe_unused]] constexpr double KMCVAL_TUNIQUEJUMPS_MAX_INTERACTIONENERGY = 1000.0;
// Minimale Code-Aktivierungsenergie
[[maybe_unused]] constexpr double KMCVAL_TUNIQUEJUMPS_MIN_CODEENERGY = -1000.0;
// Maximale Code-Aktivierungsenergie
[[maybe_unused]] constexpr double KMCVAL_TUNIQUEJUMPS_MAX_CODEENERGY = 1000.0;

// --- TSettings ---
// Minimale Temperatur [in K]
[[maybe_unused]] constexpr double KMCVAL_TSETTINGS_MIN_TEMPERATURE = 0.001;
// Maximale Temperatur [in K]
[[maybe_unused]] constexpr double KMCVAL_TSETTINGS_MAX_TEMPERATURE = 10000.0;
// Maximaler E-Feldbeitrag [in kBT] zu Sprungaktivierungsenergien
[[maybe_unused]] constexpr double KMCVAL_TSETTINGS_MAX_EFIELD = 1000.0;
// Minimale Sprungversuchsfrequenz
[[maybe_unused]] constexpr double KMCVAL_TSETTINGS_MIN_FREQUENCY = 0.001;
// Maximale Sprungvesuchsfrequenz
[[maybe_unused]] constexpr double KMCVAL_TSETTINGS_MAX_FREQUENCY = 1.0E+34;
// Minimale Gittergröße (>= 1)
[[maybe_unused]] constexpr int KMCVAL_TSETTINGS_MIN_LATTICESIZE = 1;
// Maximale Gittergröße
[[maybe_unused]] constexpr int KMCVAL_TSETTINGS_MAX_LATTICESIZE = 65;
// Maximalanzahl an zusätzlichen Leerstellen
[[maybe_unused]] constexpr long long KMCVAL_TSETTINGS_MAX_ADDVACANZ = 1000000LL;
// Maximale MCSP für Vorlauf
[[maybe_unused]] constexpr long long KMCVAL_TSETTINGS_MAX_PREMCSP = 5000000LL;
// Maximalanzahl fuer aufzuzeichnende Boltzmann-Wahrscheinlichkeiten beim Vorlauf
[[maybe_unused]] constexpr int KMCVAL_TSETTINGS_MAX_PRERECORDANZ = 10000;
// Maximalanzahl fuer JumpAttempts bei dyn. Normierungssimulation
[[maybe_unused]] constexpr long long KMCVAL_TSETTINGS_MAX_NORMATTEMPTANZ = 5000000000LL;
// Maximalanzahl fuer aufzuzeichnende dyn. Normierungswahrscheinlichkeiten
[[maybe_unused]] constexpr int KMCVAL_TSETTINGS_MAX_NORMRECORDANZ = 10000;
// Maximum fuer die Nummer des Listeneintrags für die dyn. Normierung
[[maybe_unused]] constexpr int KMCVAL_TSETTINGS_MAX_NORMNUM = 10000;
// Maximale MCSP fuer Hauptsimulation
[[maybe_unused]] constexpr long long KMCVAL_TSETTINGS_MAX_MCSP = 5000000LL;
// Maximalanzahl fuer aufzuzeichnende Boltzmann-Wahrscheinlichkeiten bei Hauptsimulation
[[maybe_unused]] constexpr int KMCVAL_TSETTINGS_MAX_RECORDANZ = 10000;

// --- TSimulation ---
// Maximale Laenge des Checkpoint-Pfads
[[maybe_unused]] constexpr int KMCVAL_TSIMULATION_MAX_CHKPATHLENGTH = 1000;

// ************************* OUTPUT-STRINGS **************************** //

// Leerer string-Platzhalter
[[maybe_unused]] constexpr std::string_view KMCOUT_NOSTRING = "[]"sv;
// Maximale offset-Leerstellen
[[maybe_unused]] constexpr int KMCOUT_MAX_OFFSET = 10;

// --- TKMCJob ---
// Erkennungsstring fuer TKMCJob-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TKMCJOB_START = "<KMCJob>"sv;
// Erkennungsstring fuer TKMCJob-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TKMCJOB_END = "</KMCJob>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TKMCJOB_OFFSET = 2;
// Bezeichner fuer Projektname
[[maybe_unused]] constexpr std::array KMCOUT_TKMCJOB_NAME = { "ProjectName:"sv, "Projektname:"sv };
// Bezeichner fuer Username
[[maybe_unused]] constexpr std::string_view KMCOUT_TKMCJOB_USERNAME = "User:"sv;
// Bezeichner fuer Datum
[[maybe_unused]] constexpr std::array KMCOUT_TKMCJOB_DATE = { "CreationDate:"sv, "Erstellungsdatum:"sv };
// Bezeichner fuer Projektpfad
[[maybe_unused]] constexpr std::array KMCOUT_TKMCJOB_PATH = { "ProjectPath:"sv, "Projektpfad:"sv };
// Bezeichner fuer Projektbeschreibungsstart
[[maybe_unused]] constexpr std::array KMCOUT_TKMCJOB_DESCSTART = { "<ProjectDescription>"sv, "<Projektbeschreibung>"sv };
// Bezeichner fuer Projektbeschreibungsende
[[maybe_unused]] constexpr std::array KMCOUT_TKMCJOB_DESCEND = { "</ProjectDescription>"sv, "</Projektbeschreibung>"sv };
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TKMCJob
[[maybe_unused]] constexpr int KMCOUT_TKMCJOB_COUT_PRECISION = 6;

// --- TElements ---
// Erkennungsstring fuer TElements-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TELEMENTS_START = "<ElementInfo>"sv;
// Erkennungsstring fuer TElements-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TELEMENTS_END = "</ElementInfo>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TELEMENTS_OFFSET = 2;
// Bezeichner fuer Element
[[maybe_unused]] constexpr std::string_view KMCOUT_TELEMENTS_ELEMENT = "Element:"sv;
// Bezeichner fuer Element-ID bei Stream-Ausgabe
[[maybe_unused]] constexpr std::string_view KMCOUT_TELEMENTS_ELEMID = "ID"sv;
// Bezeichner fuer Symbol bei Stream-Ausgabe
[[maybe_unused]] constexpr std::string_view KMCOUT_TELEMENTS_SYMBOL = "Symbol"sv;
// Bezeichner fuer Name bei Stream-Ausgabe
[[maybe_unused]] constexpr std::string_view KMCOUT_TELEMENTS_NAME = "Name"sv;
// Bezeichner fuer Charge bei Stream-Ausgabe
[[maybe_unused]] constexpr std::array KMCOUT_TELEMENTS_CHARGE = { "Charge"sv, "Ladung"sv };
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TElements
[[maybe_unused]] constexpr int KMCOUT_TELEMENTS_COUT_PRECISION = 6;

// --- TStructure ---
// Erkennungsstring fuer TStructure-Start
[[maybe_unused]] constexpr std::array KMCOUT_TSTRUCTURE_START = { "<StructureInfo>"sv, "<StrukturInfo>"sv };
// Erkennungsstring fuer TStructure-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TSTRUCTURE_END = { "</StructureInfo>"sv, "</StrukturInfo>"sv };
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TSTRUCTURE_OFFSET = 2;
// Bezeichner fuer Strukturname
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_NAME = "Name:"sv;
// Bezeichner fuer Basisvektoren
[[maybe_unused]] constexpr std::array KMCOUT_TSTRUCTURE_BASIS = { "LatticeVector:"sv, "Basisvektor:"sv };
// Bezeichner fuer Basisvektor a
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_BASISA = "a"sv;
// Bezeichner fuer Basisvektor b
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_BASISB = "b"sv;
// Bezeichner fuer Basisvektor c
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_BASISC = "c"sv;
// Bezeichner fuer x-Koordinaten
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_XCOORD = "x"sv;
// Bezeichner fuer y-Koordinaten
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_YCOORD = "y"sv;
// Bezeichner fuer z-Koordinaten
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_ZCOORD = "z"sv;
// Bezeichner fuer Atom
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_ATOM = "Atom:"sv;
// Bezeichner fuer Atom-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_ATOMID = "ID"sv;
// Bezeichner fuer relative x-Koordinaten
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_XREL = "a"sv;
// Bezeichner fuer relative y-Koordinaten
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_YREL = "b"sv;
// Bezeichner fuer relative z-Koordinaten
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_ZREL = "c"sv;
// Bezeichner fuer ElemID
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_ELEMID = "ElemID"sv;
// Bezeichner fuer Dotierung
[[maybe_unused]] constexpr std::array KMCOUT_TSTRUCTURE_DOPING = { "Doping:"sv, "Dotierung:"sv };
// Bezeichner fuer Dotierungs-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_DOPINGID = "ID"sv;
// Bezeichner fuer dotiertes Element
[[maybe_unused]] constexpr std::array KMCOUT_TSTRUCTURE_DOPEDID = { "DopedID"sv, "DotiertID"sv };
// Bezeichner fuer Dopand
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_DOPANDID = "DopandID"sv;
// Bezeichner fuer Dopand-Leerstellen-Verhaeltnis
[[maybe_unused]] constexpr std::string_view KMCOUT_TSTRUCTURE_VACDOPRATIO = "Vac/Dop"sv;
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TStructure
[[maybe_unused]] constexpr int KMCOUT_TSTRUCTURE_COUT_PRECISION = 6;

// --- TJump ---
// Erkennungsstring 1 fuer TJump-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_START1 = "<JumpInfo"sv;
// Erkennungsstring 2 fuer TJump-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_START2 = ">"sv;
// Erkennungsstring fuer TJump-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_END = "</JumpInfo>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TJUMP_OFFSET = 2;
// Bezeichner fuer Atom-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_ATOMID = "Atom="sv;
// Bezeichner fuer Richtungs-ID
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_DIRID = { "Direction="sv, "Richtung="sv };
// Bezeichner fuer Ruecksprung-DirID
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_BACKJUMPDIRID = { "BackjumpDirectionID:"sv, "RuecksprungrichtungsID:"sv };
// Bezeichner fuer UniqueJump-ID
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_UNIQUEID = { "JumpID:"sv, "SprungID:"sv };
// Wert fuer undefinierte UniqueJump-ID
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_UNIQUEID_UNDEF = { "undefined"sv, "unbekannt"sv };
// Bezeichner fuer Startatom
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_STARTATOM = { "StartAtom:"sv, "Startatom:"sv };
// Bezeichner fuer Zielatom
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_DESTATOM = { "EndAtom:"sv, "Zielatom:"sv };
// Bezeichner fuer Umgebungsatom
[[maybe_unused]] constexpr std::array KMCOUT_TJUMP_ENVATOM = { "EnvAtom:"sv, "Umgebungsatom:"sv };
// Bezeichner fuer Umgebungsatom-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_ENVATOMID = "ID"sv;
// Bezeichner fuer Atom-x-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_X = "a"sv;
// Bezeichner fuer Atom-y-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_Y = "b"sv;
// Bezeichner fuer Atom-z-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_Z = "c"sv;
// Bezeichner fuer Atom-s-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_S = "s"sv;
// Bezeichner fuer relative Atom-x-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_RELX = "rel. a"sv;
// Bezeichner fuer relative Atom-y-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_RELY = "rel. b"sv;
// Bezeichner fuer relative Atom-z-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_RELZ = "rel. c"sv;
// Bezeichner fuer relative Atom-s-Koordinate in 4D
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMP_RELS = "rel. s"sv;
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TJump
[[maybe_unused]] constexpr int KMCOUT_TJUMP_COUT_PRECISION = 6;

// --- TJumps ---
// Erkennungsstring fuer TJumps-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMPS_START = "<JumpsInfo>"sv;
// Erkennungsstring fuer TJumps-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMPS_END = "</JumpsInfo>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TJUMPS_OFFSET = 2;
// Bezeichner fuer Sprungrichtungsschalen-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TJUMPS_MOVSHELLCOUNT = { "MovShellCount:"sv, "Sprungrichtungsschalenanzahl:"sv };
// Bezeichner fuer Sprungumgebungsschalen-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TJUMPS_JUMPSTARTSHELLCOUNT = { "EnvShellCount-Start:"sv, "Sprungstart-Umgebungsschalenanzahl:"sv };
// Bezeichner fuer Sprungumgebungsschalen-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TJUMPS_JUMPTSSHELLCOUNT = { "EnvShellCount-Center:"sv, "Sprungmitte-Umgebungsschalenanzahl:"sv };
// Bezeichner fuer Sprungumgebungsschalen-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TJUMPS_JUMPDESTSHELLCOUNT = { "EnvShellCount-End:"sv, "Sprungziel-Umgebungsschalenanzahl:"sv };
// Bezeichner fuer Sprunganzahl
[[maybe_unused]] constexpr std::array KMCOUT_TJUMPS_JUMPCOUNT = { "JumpCount:"sv, "Sprunganzahl:"sv };
// Bezeichner fuer AtomID
[[maybe_unused]] constexpr std::string_view KMCOUT_TJUMPS_ATOMID = "AtomID"sv;
// Bezeichner fuer Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TJUMPS_COUNT = { "Count"sv, "Anzahl"sv };
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TJumps
[[maybe_unused]] constexpr int KMCOUT_TJUMPS_COUT_PRECISION = 4;

// --- TUniqueJump ---
// Erkennungsstring 1 fuer TUniqueJump-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_START1 = "<UniqueJumpInfo"sv;
// Erkennungsstring 2 fuer TUniqueJump-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_START2 = ">"sv;
// Erkennungsstring fuer TUniqueJump-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_END = "</UniqueJumpInfo>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TUNIQUEJUMP_OFFSET = 2;
// Bezeichner fuer UniqueJump-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_JUMPID = "UniqueJump="sv;
// Bezeichner fuer Sprunglänge
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_LENGTH = { "Length:"sv, "Laenge:"sv };
// Bezeichner fuer Sprungaktivierungszustand
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_JUMPACTIVE = { "JumpStatus:"sv, "Sprung-Zustand:"sv };
// true-Bezeichner fuer JumpActive == true
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_JUMPACTIVE_TRUE = { "active"sv, "aktiv"sv };
// false-Bezeichner fuer JumpActive == false
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_JUMPACTIVE_FALSE = { "inactive"sv, "inaktiv"sv };
// Bezeichner fuer Umgebungsatom
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_COORD = { "EnvAtom:"sv, "Umgebungsatom:"sv };
// Bezeichner fuer CoordID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_COORDID = "ID"sv;
// Bezeichner fuer ElemID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_ELEMID = "ElemID"sv;
// Bezeichner fuer x-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_X = "x"sv;
// Bezeichner fuer y-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_Y = "y"sv;
// Bezeichner fuer z-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_Z = "z"sv;
// Bezeichner fuer Atomignorierungszustand
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMIGNORE = { "Ignored"sv, "Ignoriert"sv };
// true-Bezeichner fuer AtomIgnore == true
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMIGNORE_TRUE = { "yes"sv, "ja"sv };
// false-Bezeichner fuer AtomIgnore == false
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMIGNORE_FALSE = { "no"sv, "nein"sv };
// Bezeichner fuer Atomadditivitätszustand
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMADDITIVE = { "Additive"sv, "Additiv"sv };
// true-Bezeichner fuer AtomActive == true
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_TRUE = { "yes"sv, "ja"sv };
// false-Bezeichner fuer AtomActive == false
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMADDITIVE_FALSE = { "no"sv, "nein"sv };
// Bezeichner fuer Atomaktivierungszustand
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMACTIVE = { "Active"sv, "Aktiv"sv };
// true-Bezeichner fuer AtomActive == true
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMACTIVE_TRUE = { "yes"sv, "ja"sv };
// false-Bezeichner fuer AtomActive == false
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_ATOMACTIVE_FALSE = { "no"sv, "nein"sv };
// Bezeichner fuer WW-Umgebungsatom
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_INTERACTIONATOM = { "InteractAtom:"sv, "WW-Atom:"sv };
// Bezeichner fuer WW-Atom-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_INTERACTIONATOM_ID = "ID"sv;
// Bezeichner fuer WW-Map
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_INTERACTIONMAP = { "InteractID"sv, "WWID"sv };
// Bezeichner fuer WW-Energie-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_INTERACTIONENERGYCOUNT = { "Count"sv, "Anz."sv };
// Bezeichner fuer WW-Energie-ElemID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_INTERACTIONELEMID = "ElemID"sv;
// Bezeichner fuer WW-Energie
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_INTERACTIONENERGY = { "Energy"sv, "Energie"sv };
// Bezeichner fuer UniqueCodes
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_UNIQUECODE = { "Unique-Code:"sv, "Unique-Codierung:"sv };
// Bezeichner fuer UniqueJumpID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_UNIQUEID = "ID"sv;
// Bezeichner fuer UniqueEnergy
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_UNIQUEENERGY = { "ActivationEnergy"sv, "Aktivierungsenergie"sv };
// Bezeichner fuer Codes
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_CODE = "Code"sv;
// Bezeichner fuer FullCodes
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMP_FULLCODE = { "FullCode:"sv, "Codierung:"sv };
// Bezeichner fuer FullCodeID
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_FULLCODEID = "ID"sv;
// Bezeichner fuer FullToUniqueMap
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMP_FULLTOUNIQUEMAP = "Unique-ID"sv;
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TUniqueJump
[[maybe_unused]] constexpr int KMCOUT_TUNIQUEJUMP_COUT_PRECISION = 6;

// --- TInteraction ---
// Erkennungsstring 1 fuer TInteraction-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_START1 = "<InteractionInfo"sv;
// Erkennungsstring 2 fuer TInteraction-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_START2 = ">"sv;
// Erkennungsstring fuer TInteraction-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_END = "</InteractionInfo>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TINTERACTION_OFFSET = 2;
// Bezeichner fuer Interaction-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_ID = "Interaction="sv;
// Bezeichner fuer ElemID
[[maybe_unused]] constexpr std::array KMCOUT_TINTERACTION_WWELEMID = { "EnvAtom-ElemID:"sv, "Umgebungsatom-ElemID:"sv };
// Bezeichner fuer StartDist
[[maybe_unused]] constexpr std::array KMCOUT_TINTERACTION_STARTDIST = { "DistanceStart:"sv, "Sprungstart-Abstand:"sv };
// Bezeichner fuer DestDist
[[maybe_unused]] constexpr std::array KMCOUT_TINTERACTION_DESTDIST = { "DistanceEnd:"sv, "Sprungziel-Abstand:"sv };
// Bezeichner fuer Start-VV-WW-Umgebungsatom
[[maybe_unused]] constexpr std::array KMCOUT_TINTERACTION_STARTWW = { "StartInteraction-EnvAtom:"sv, "Sprungstart-WW-Umgebungsatom:"sv };
// Bezeichner fuer Umgebungsatom-ID
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_WWID = "ID"sv;
// Bezeichner fuer Umgebungsatom-ElemID
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_ELEMID = "ElemID"sv;
// Bezeichner fuer x-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_X = "x"sv;
// Bezeichner fuer y-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_Y = "y"sv;
// Bezeichner fuer z-Koordinate in Sprungkoordinatensystem (Zylinderkoordinatensystem)
[[maybe_unused]] constexpr std::string_view KMCOUT_TINTERACTION_Z = "z"sv;
// Bezeichner fuer Ziel-WW-Umgebungsatom
[[maybe_unused]] constexpr std::array KMCOUT_TINTERACTION_DESTWW = { "EndInteraction-EnvAtom:"sv, "Sprungziel-WW-Umgebungsatom:"sv };
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TInteraction
[[maybe_unused]] constexpr int KMCOUT_TINTERACTION_COUT_PRECISION = 6;

// --- TUniqueJumps ---
// Erkennungsstring fuer TUniqueJumps-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMPS_START = "<UniqueJumpsInfo>"sv;
// Erkennungsstring fuer TUniqueJumps-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TUNIQUEJUMPS_END = "</UniqueJumpsInfo>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TUNIQUEJUMPS_OFFSET = 2;
// Bezeichner fuer Leerstellen-Leerstellen-Wechselwirkungsschalen-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMPS_INTERACTIONSHELLCOUNT = { "InteractionShellCount:"sv, "WW-Schalenanzahl:"sv };
// Bezeichner, ob Codierung erfolgt ist
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMPS_CODESREADY = { "CodesReady:"sv, "Codierung:"sv };
// Bezeichner, dass Codierung erfolgt ist
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMPS_CODESREADY_TRUE = { "yes"sv, "ja"sv };
// Bezeichner, dass Codierung nicht erfolgt ist
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMPS_CODESREADY_FALSE = { "no"sv, "nein"sv };
// Anzahl der UJump
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMPS_UJUMPCOUNT = { "UniqueJumpCount:"sv, "Sprunganzahl:"sv };
// Anzahl der VVShells
[[maybe_unused]] constexpr std::array KMCOUT_TUNIQUEJUMPS_INTERACTIONCOUNT = { "InteractionCount:"sv, "WW-Anzahl:"sv };
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TUniqueJumps
[[maybe_unused]] constexpr int KMCOUT_TUNIQUEJUMPS_COUT_PRECISION = 6;

// -- TSettings ---
// Erkennungsstring fuer TSettings-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TSETTINGS_START = "<Settings>"sv;
// Erkennungsstring fuer TSettings-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TSETTINGS_END = "</Settings>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TSETTINGS_OFFSET = 2;
// Bezeichner fuer Simulationstemperatur
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_TEMPERATURE = { "Temperature:"sv, "Temperatur:"sv };
// Bezeichner fuer Sprungversuchsfrequenz
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_ATTEMPTFREQUENCY = { "AttemptFrequency:"sv, "Sprungversuchsfrequenz:"sv };
// Bezeichner fuer E-Feld-Staerke [in kT]
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_EFIELDSTRENGTH = { "E-Field-Strength:"sv, "E-Feld-Staerke:"sv };
// Bezeichner fuer E-Feld
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_EFIELD = { "E-Field:"sv, "E-Feld:"sv };
// Bezeichner fuer E-Feld x-Koordinate
[[maybe_unused]] constexpr std::string_view KMCOUT_TSETTINGS_EFIELDX = "x"sv;
// Bezeichner fuer E-Feld y-Koordinate
[[maybe_unused]] constexpr std::string_view KMCOUT_TSETTINGS_EFIELDY = "y"sv;
// Bezeichner fuer E-Feld z-Koordinate
[[maybe_unused]] constexpr std::string_view KMCOUT_TSETTINGS_EFIELDZ = "z"sv;
// Bezeichner fuer Gittergroesse
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_LATTICESIZE = { "LatticeSize:"sv, "Gittergroesse:"sv };
// Bezeichner fuer Zusatzleerstellen
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_ADDVACANZ = { "AdditionalVacancies:"sv, "Zusatzleerstellen:"sv };
// Bezeichner fuer Leerstellengesamtkonzentration
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_TOTALVACCONC = { "TotalVacancyConcentration:"sv, "Leerstellengesamtkonzentration:"sv };
// Bezeichner fuer Leerstellengesamtanzahl
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_TOTALVACANZ = { "TotalVacancyCount:"sv, "Leerstellengesamtanzahl:"sv };
// Bezeichner fuer Volumenkonzentration der beweglichen Spezies [1/cm^3]
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_MOVVOLCONC = { "MovingSpeciesConcentration:"sv, "Mov-Volumenkonzentration:"sv };
// Bezeichner fuer Dopandenkonzentration
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_CONCENTRATION = { "DopingConcentration:"sv, "Dopandenkonzentration:"sv };
// Bezeichner fuer Dotierung
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DOPING = { "DopingID"sv, "DotierungID"sv };
// Bezeichner fuer Konzentration
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_CONC = { "Conc."sv, "Konz."sv };
// Bezeichner fuer Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_ANZ = { "Count"sv, "Anzahl"sv };
// Bezeichner fuer Checkpoint-Schreiben
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_WRITECHK = { "SaveCheckpoint:"sv, "Checkpoint-Speichern:"sv };
// Bezeichner fuer Checkpoint-Laden
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_LOADCHK = { "LoadCheckpoint:"sv, "Checkpoint-Laden:"sv };
// Bezeichner fuer DoPrerun-Flag
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DOPRERUN = { "PreRun:"sv, "Vorlauf:"sv };
// Bezeichner fuer "durchfuehren"
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DO_TRUE = { "yes"sv, "ja"sv };
// Bezeichner fuer "nicht durchfuehren"
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DO_FALSE = { "no"sv, "nein"sv };
// Bezeichner fuer Vorlauf-MCSP
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_PREMCSP = { "PreRun-MCSP:"sv, "Vorlauf-MCSP:"sv };
// Bezeichner fuer Vorlauf-Wahrscheinlichkeitsliste
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_PRERECORDANZ = { "PreRun-ProbabilityList:"sv, "Vorlauf-Wahrscheinlichkeitsliste:"sv };
// Bezeichner fuer DoDynNorm-Flag
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DODYNNORM = { "DynamicNormalization:"sv, "DynNormierung:"sv };
// Bezeichner fuer DynNorm-Attempt-Anzahl
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DYNATTEMPTANZ = { "DynNorm-JumpAttempts:"sv, "DynNorm-Sprungversuche:"sv };
// Bezeichner fuer DynNorm-Wahrscheinlichkeitsliste
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DYNRECORDANZ = { "DynNorm-ProbabilityList:"sv, "DynNorm-Wahrscheinlichkeitsliste:"sv };
// Bezeichner fuer DynNorm-Wahrscheinlichkeitseintrag
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_DYNNORMNUM = { "DynNorm-ListEntry:"sv, "DynNorm-Wahrscheinlichkeitseintrag:"sv };
// Bezeichner fuer MCSP der Hauptsimulation
[[maybe_unused]] constexpr std::string_view KMCOUT_TSETTINGS_MAINMCSP = "MCSP:"sv;
// Bezeichner fuer Wahrscheinlichkeitsliste der Hauptsimulation
[[maybe_unused]] constexpr std::array KMCOUT_TSETTINGS_MAINRECORDANZ = { "ProbabilityList:"sv, "Wahrscheinlichkeitsliste:"sv };
// Maximalanzahl an signifikanten Stellen von double-Variablen bei cout-Ausgabe von TSettings
[[maybe_unused]] constexpr int KMCOUT_TSETTINGS_COUT_PRECISION = 6;

// -- TSimulation ---
// Erkennungsstring fuer TSimulation-Checkpoint-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_CHK_START = "<Checkpoint>"sv;
// Erkennungsstring fuer TSimulation-Checkpoint-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_CHK_END = "</Checkpoint>"sv;
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TSIMULATION_CHK_OFFSET = 2;
// Relativer Offset des untergeordneten Phasen-Outputs
[[maybe_unused]] constexpr int KMCOUT_TSIMULATION_PHASE_OFFSET = 2;
// Erkennungsstring fuer Startgitter-Start
[[maybe_unused]] constexpr std::array KMCOUT_TSIMULATION_STARTLATTICE_START = { "<InitialLattice>"sv, "<Startgitterbesetzung>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Gitter-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TSIMULATION_STARTLATTICE_END = { "</InitialLattice>"sv, "</Startgitterbesetzung>"sv };
// Erkennungsstring fuer TSimulation-PrerunPhase-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_PRERUN_START = "<Prerun-Phase>"sv;
// Erkennungsstring fuer TSimulation-PrerunPhase-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_PRERUN_END = "</Prerun-Phase>"sv;
// Erkennungsstring fuer TSimulation-DynNormPhase-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_DYNNORM_START = "<DynNorm-Phase>"sv;
// Erkennungsstring fuer TSimulation-DynNormPhase-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_DYNNORM_END = "</DynNorm-Phase>"sv;
// Erkennungsstring fuer TSimulation-MainPhase-Start
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_MAIN_START = "<Main-Phase>"sv;
// Erkennungsstring fuer TSimulation-MainPhase-Ende
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMULATION_MAIN_END = "</Main-Phase>"sv;

// -- TSimPhaseInfo ---
// Bezeichner fuer die aktuellen MCSP der Phase
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMPHASEINFO_MCSP = "MCSP:"sv;
// Bezeichner fuer die aktuellen Sprungversuche der Phase
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_JUMPATTEMPTS = { "JumpAttempts:"sv, "Sprungversuche:"sv };
// Bezeichner fuer die aktuellen Site-Blocking der Phase
[[maybe_unused]] constexpr std::string_view KMCOUT_TSIMPHASEINFO_SITEBLOCKING = "Site-Blocking:"sv;
// Bezeichner fuer die aktuellen Nonsense-Sprungversuche der Phase
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_NONSENSE = { "UndefinedEndPosition(dE_backward<=0):"sv, "Undef.-Zielplatz(Eakt,rueck<=0):"sv };
// Bezeichner fuer die aktuellen Overkill-Sprungversuche der Phase
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_OVERKILL = { "UndefinedStartPosition(dE_forward<=0):"sv, "Undef.-Startplatz(Eakt,hin<=0):"sv };
// Bezeichner fuer die aktuellen Normierung der Phase
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_NORM = { "Normalization:"sv, "Normierung:"sv };
// Bezeichner fuer die Summe des Attempt-Pfad-Quotienten der Phase
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_TIMESUM = { "Attempts-Paths-Ratio:"sv, "Versuch-Pfad-Quotient:"sv };
// Bezeichner fuer die aktuellen Laufzeit der Phase
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_RUNTIME = { "Runtime:"sv, "Laufzeit:"sv };
// Erkennungsstring fuer TSimPhaseInfo-Gitter-Start
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_LATTICE_START = { "<CurrentLattice>"sv, "<Gitterbesetzung>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Gitter-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_LATTICE_END = { "</CurrentLattice>"sv, "</Gitterbesetzung>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Bewegungsspeicherung-Start
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_MOVLATTICE_START = { "<AtomMovements>"sv, "<Atombewegung>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Bewegungsspeicherung-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_MOVLATTICE_END = { "</AtomMovements>"sv, "</Atombewegung>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Sprungversuchswahrscheinlichkeiten-Start
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_START = { "<AttemptProbabilities>"sv, "<Sprungversuchswahrscheinlichkeiten>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Sprungversuchswahrscheinlichkeiten-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_ATTEMPTPROB_END = { "</AttemptProbabilities>"sv, "</Sprungversuchswahrscheinlichkeiten>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Sprungwahrscheinlichkeiten-Start
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_MCSPROB_START = { "<JumpProbabilities>"sv, "<MCS-Wahrscheinlichkeiten>"sv };
// Erkennungsstring fuer TSimPhaseInfo-Sprungwahrscheinlichkeiten-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TSIMPHASEINFO_MCSPROB_END = { "</JumpProbabilities>"sv, "</MCS-Wahrscheinlichkeiten>"sv };

// -- TResults ---
// Erkennungsstring fuer TResults-Start
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_START = { "<Results>"sv, "<Ergebnisse>"sv };
// Erkennungsstring fuer TResults-Ende
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_END = { "</Results>"sv, "</Ergebnisse>"sv };
// Relativer Offset des untergeordneten Outputs
[[maybe_unused]] constexpr int KMCOUT_TRESULTS_OFFSET = 2;
// Bezeichner fuer Conductivity
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_COND = { "Conductivity:"sv, "Leitfaehigkeit:"sv };
// Bezeichner fuer SimulatedTime
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_SIMTIME = { "SimulatedTimespan:"sv, "Zeitspanne:"sv };
// Bezeichner fuer MCSP
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_MCSP = "MCSP:"sv;
// Bezeichner fuer JumpAttempts
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_JUMPATTEMPTS = { "JumpAttempts:"sv, "Sprungversuche:"sv };
// Bezeichner fuer NonsenseJumpAttempts
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_NONSENSEATTEMPTS = { "UndefinedEndPosition(dE_backward<=0):"sv, "Undef.-Zielplatz(Eakt,rueck<=0):"sv };
// Bezeichner fuer OverkillJumpAttempts
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_OVERKILLATTEMPTS = { "UndefinedStartPosition(dE_forward<=0):"sv, "Undef.-Startplatz(Eakt,hin<=0):"sv };
// Bezeichner fuer SiteBlocking
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_SITEBLOCKING = "Site-Blocking:"sv;
// Bezeichner fuer Normierung
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_NORMALIZATION = { "Normalization:"sv, "Normierung:"sv };
// Bezeichner fuer Laufzeit
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_RUNTIME = { "Runtime:"sv, "Laufzeit:"sv };
// Bezeichner fuer mittlere Sprunganzahl (bewegliche Spezies)
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_MOV_MEANJUMPCOUNT = { "Mov-<JumpCount>:"sv, "Mov-<Sprunganzahl>:"sv };
// Bezeichner fuer mittleren Verschiebungsvektor (bewegliche Spezies)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_MOV_MEANDISPVEC = "Mov-(<x>,<y>,<z>):"sv;
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors parallel zum E-Feld (bewegliche Spezies)
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_MOV_DISPPARALLEL = { "Mov-<ParallelDisplacement>:"sv, "Mov-Verschiebungskomp.(parallel):"sv };
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors senkrecht zum E-Feld (bewegliche Spezies)
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_MOV_DISPPERPENDICULAR = { "Mov-<PerpendicularDisplacement>:"sv, "Mov-Verschiebungskomp.(senkrecht):"sv };
// Bezeichner fuer mittlere Verschiebung (bewegliche Spezies)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_MOV_MEANDISP = "Mov-<sqrt(x*x+y*y+z*z)>:"sv;
// Bezeichner fuer mittlere quadratische Verschiebung (bewegliche Spezies)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_MOV_MEANSQUAREDDISP = "Mov-<x*x+y*y+z*z>:"sv;
// Bezeichner fuer Schwerpunktverschiebungsvektor (bewegliche Spezies)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_MOV_COMDISPVEC = "Mov-(sum(x),sum(y),sum(z)):"sv;
// Bezeichner fuer mittlere Sprunganzahl (Leerstellen)
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_VAC_MEANJUMPCOUNT = { "Vac-<JumpCount>:"sv, "Vac-<Sprunganzahl>:"sv };
// Bezeichner fuer mittleren Verschiebungsvektor (Leerstellen)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_VAC_MEANDISPVEC = "Vac-(<x>,<y>,<z>):"sv;
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors parallel zum E-Feld (Leerstellen)
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_VAC_DISPPARALLEL = { "Vac-<ParallelDisplacement>:"sv, "Vac-Verschiebungskomp.(parallel):"sv };
// Bezeichner fuer Komponente des mittleren Verschiebungsvektors senkrecht zum E-Feld (Leerstellen)
[[maybe_unused]] constexpr std::array KMCOUT_TRESULTS_VAC_DISPPERPENDICULAR = { "Vac-<PerpendicularDisplacement>:"sv, "Vac-Verschiebungskomp.(senkrecht):"sv };
// Bezeichner fuer mittlere Verschiebung (Leerstellen)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_VAC_MEANDISP = "Vac-<sqrt(x*x+y*y+z*z)>:"sv;
// Bezeichner fuer mittlere quadratische Verschiebung (Leerstellen)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_VAC_MEANSQUAREDDISP = "Vac-<x*x+y*y+z*z>:"sv;
// Bezeichner fuer Schwerpunktverschiebungsvektor (Leerstellen)
[[maybe_unused]] constexpr std::string_view KMCOUT_TRESULTS_VAC_COMDISPVEC = "Vac-(sum(x),sum(y),sum(z)):"sv;

// ************************ EXCEPTION-STRINGS ************************** //

// Text vor Standard-Exception
[[maybe_unused]] constexpr std::string_view KMCERRSTR_STDERR = "Exception occured:"sv;
// Text bei unbekannter Exception
[[maybe_unused]] constexpr std::string_view KMCERRSTR_UNKNOWNERR = "Unknown exception occured!"sv;

// ************************ Translate-Fallback ************************* //

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<std::string_view, N>& arr)
{
    // Use the first identifier for output
    os << arr.front();
    return os;
}

template<std::size_t N>
constexpr bool operator==(std::string_view lhs, const std::array<std::string_view, N>& rhs)
{
    // Use all identifiers for input comparison (true if any match)
    for (std::string_view rstr : rhs)
        if (lhs == rstr)
            return true;

    return false;
}

template<std::size_t N>
constexpr bool operator!=(std::string_view lhs, const std::array<std::string_view, N>& rhs)
{
    // Use all identifiers for input comparison (false if any match)
    return !(lhs == rhs);
}

// ************************* Hilfsfunktionen *************************** //

// Funktion zum Trimmen eines string_view (d.h. Leerstellen an Anfang und Ende entfernen)
std::string_view Trim(std::string_view input);

// Funktion zum Trimmen eines string_view von der rechten Seite (d.h. Leerstellen am Ende entfernen)
std::string_view RightTrim(std::string_view input);

// Funktion zum Ueberpruefen eines string, keine allowed chars = alle erlaubt
bool StringCheck(std::string_view input, int min_length, int max_length, std::string_view allowed_chars = "");

// Funktion zur Umwandlung eines int in einen string
std::string IntToStr(int input);

// Funktion zur Umwandlung eines long long in einen string
std::string LongLongToStr(long long input);

// Funktion zur Umwandlung eines unsigned long long in einen string
std::string UnsignedLongLongToStr(unsigned long long input);

// Funktion zur Umwandlung eines double in einen string
std::string DoubleToStr(double input);

// Funktion zur Umwandlung eines string in einen int
int StrToInt(std::string input, int error_val = -1);

// Funktion zur Umwandlung eines string in einen long long
long long StrToLongLong(std::string input, long long error_val = -1LL);

// Funktion zur Umwandlung eines string in einen unsigned long long
unsigned long long StrToUnsignedLongLong(std::string input, unsigned long long error_val = 0ULL);

// Funktion zur Umwandlung eines string in einen double
double StrToDouble(std::string input, double error_val = 0.0);

// Funktion zur Umwandlung eines string in einen int
bool ConvStrToInt(std::string input, int& val);

// Funktion zur Umwandlung eines string in einen long long
bool ConvStrToLongLong(std::string input, long long& val);

// Funktion zur Umwandlung eines string in einen unsigned long long
bool ConvStrToUnsignedLongLong(std::string input, unsigned long long& val);

// Funktion zur Umwandlung eines string in einen double
bool ConvStrToDouble(std::string input, double& val);

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
bool GetApplicationPath(std::string& out_str);

// Prepend application path to relative file name
std::string PrependAppPath(std::string_view rel_filename);

#endif