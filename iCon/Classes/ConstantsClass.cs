namespace iCon_General
{
    /// <summary> Global constant provider </summary>
    public class ConstantsClass
    {
        // ********************** String Constants ********************** //
        #region String Constants

        /// <summary> Directory name for outer AppData-Folder where settings etc. should be stored (Manufacturer name) </summary>
        public const string SC_KMC_APPDATA_OUTERFOLDER = @"RWTH";

        /// <summary> Directory name for inner AppData-Folder where settings etc. should be stored (Program name)</summary>
        public const string SC_KMC_APPDATA_INNERFOLDER = @"iCon";

        /// <summary> Directory name where simulation executables are stored (relative to application folder) </summary>
        public const string SC_KMC_SIMEXEFOLDER = @"SimExecutables";

        /// <summary> Directory name where searcher executables are stored (relative to application folder) </summary>
        public const string SC_KMC_SEARCHEXEFOLDER = @"SearchExecutables";

        /// <summary> Directory name where default simulation scripts are stored (relative to application folder) </summary>
        public const string SC_KMC_SIMSCRIPTSFOLDER = @"SimScripts";

        /// <summary> Directory name where default searcher scripts are stored (relative to application folder) </summary>
        public const string SC_KMC_SEARCHSCRIPTSFOLDER = @"SearchScripts";

        /// <summary> Filename for saving input between sessions </summary>
        public const string SC_KMC_LASTSESSION_FILE = @"Current.kmc";

        /// <summary> Standard error log file </summary>
        public const string SC_KMC_STD_ERRORLOG = @"Error.log";

        /// <summary> Standard ini file </summary>
        public const string SC_KMC_STD_INIFILE = @"Settings.ini";

        /// <summary> Name of the scripts folder of the user </summary>
        public const string SC_KMC_USERSCRIPTS_DIR = @"UserScripts";

        /// <summary> Prefix to the remote profile scripts folder </summary>
        public const string SC_KMC_REMOTEPROFILE_DIR = @"RemoteProfile_";

        /// <summary> Format string for remote profile ID </summary>
        public const string SC_KMC_REMOTEPROFILE_ID_FORMAT = @"D3";

        /// <summary> Filename of the remote profile ini file </summary>
        public const string SC_KMC_REMOTEPROFILE_INIFILE = @"RemoteProfile.ini";

        /// <summary> Name of the job submission script </summary>
        public const string SC_KMC_SUBMITSCRIPT = @"SubmitScript";

        /// <summary> Name of the job script </summary>
        public const string SC_KMC_JOBSCRIPT = @"JobScript";

        /// <summary> Name of the searcher script </summary>
        public const string SC_KMC_SEARCHSCRIPT = @"SearchScript";

        /// <summary> Base name of the simulation executables </summary>
        public const string SC_KMC_BASESIMEXE = @"iConSimulator_";

        /// <summary> Extension of the simulation executables </summary>
        public const string SC_KMC_EXTSIMEXE = @".exe";

        /// <summary> Base name of the searcher executables </summary>
        public const string SC_KMC_BASESEARCHEXE = @"iConSearcher_";

        /// <summary> Extension of the searcher executables </summary>
        public const string SC_KMC_EXTSEARCHEXE = @".exe";

        /// <summary> Filename of simulation executable for local execution </summary>
        public const string SC_KMC_LOCALSIMEXE = @"iConSimulator_Win.exe";

        /// <summary> Filename of searcher executable for local execution </summary>
        public const string SC_KMC_LOCALSEARCHEXE = @"iConSearcher_Win.exe";

        /// <summary> Standard working directory </summary>
        public const string SC_KMC_STD_WORKING_DIR = @"iCon-Workspace";

        /// <summary> Format string for long double values </summary>
        public const string SC_KMC_LONG_DOUBLEFORMAT = @"{0:G8}";

        /// <summary> Format string for short charge double values </summary>
        public const string SC_KMC_SHORT_CHARGE_DOUBLEFORMAT = @"{0:#0.##}";

        #endregion String Constants

        // ********************** Output Constants ********************** //
        #region Output Constants

        /// <summary> Offset of inifile data </summary>
        public const string SC_KMC_OUT_INI_OFFSET = @"  ";

        /// <summary> Inifile-Identifier for beginning of data </summary>
        public const string SC_KMC_OUT_INI_START = @"<GUISettings>";

        /// <summary> Inifile-Identifier for end of data </summary>
        public const string SC_KMC_OUT_INI_END = @"</GUISettings>";

        /// <summary> Inifile-Identifier for local workspace path </summary>
        public const string SC_KMC_OUT_INI_LOCALWORKSPACE = @"Workspace(local):";

        /// <summary> Inifile-Identifier for ID of selected remote profile </summary>
        public const string SC_KMC_OUT_INI_SELREMOTEPROFILE = @"SelectedRemoteProfile:";

        /// <summary> Offset of cluster profile data </summary>
        public const string SC_KMC_OUT_CPROFILE_OFFSET = @"  ";

        /// <summary> Inifile-Identifier for beginning of cluster profile data </summary>
        public const string SC_KMC_OUT_CPROFILE_START = @"<RemoteProfile>";

        /// <summary> Inifile-Identifier for end of cluster profile data </summary>
        public const string SC_KMC_OUT_CPROFILE_END = @"</RemoteProfile>";

        /// <summary> Inifile-Identifier for cluster profile name </summary>
        public const string SC_KMC_OUT_CPROFILE_NAME = @"Name:";

        /// <summary> Inifile-Identifier for cluster profile simulation executable </summary>
        public const string SC_KMC_OUT_CPROFILE_SIMEXE = @"SimExe:";

        /// <summary> Inifile-Identifier for cluster profile searcher executable </summary>
        public const string SC_KMC_OUT_CPROFILE_SEARCHEXE = @"SearchExe:";

        /// <summary> Inifile-Identifier for cluster profile remote workspace </summary>
        public const string SC_KMC_OUT_CPROFILE_WORKSPACE = @"Workspace(remote):";

        /// <summary> Inifile-Identifier for cluster profile host adress </summary>
        public const string SC_KMC_OUT_CPROFILE_HOSTADRESS = @"Host:";

        /// <summary> Inifile-Identifier for cluster profile port number </summary>
        public const string SC_KMC_OUT_CPROFILE_PORT = @"Port:";

        /// <summary> Inifile-Identifier for cluster profile user name </summary>
        public const string SC_KMC_OUT_CPROFILE_USERNAME = @"User:";

        /// <summary> Inifile-Identifier for cluster profile boolean: true </summary>
        public const string SC_KMC_OUT_CPROFILE_TRUE = @"yes";

        /// <summary> Inifile-Identifier for cluster profile boolean: false </summary>
        public const string SC_KMC_OUT_CPROFILE_FALSE = @"no";

        /// <summary> Inifile-Identifier for cluster profile: if with password auth </summary>
        public const string SC_KMC_OUT_CPROFILE_WITHPW = @"WithPassword:";

        /// <summary> Inifile-Identifier for cluster profile: if with private key auth </summary>
        public const string SC_KMC_OUT_CPROFILE_WITHKEY = @"WithKey:";

        /// <summary> Inifile-Identifier for cluster profile key file path </summary>
        public const string SC_KMC_OUT_CPROFILE_PRIVATEKEY = @"PrivateKey:";

        /// <summary> Inifile-Identifier for cluster profile: if with keyboard-interactive auth </summary>
        public const string SC_KMC_OUT_CPROFILE_WITHINTERACT = @"WithInteractive:";

        #endregion Output Constants

        // ********************** Numeric Values ********************** //
        #region Numeric Values

        /// <summary> Millisekunden-Wartezeit zwischen zwei Thread-Arbeitsschritten </summary>
        public const int THREAD_READING_DELAY = 100;

        /// <summary> Millisekunden-Wartezeit wenn Thread fertig </summary>
        public const int THREAD_FINISH_DELAY = 500;

        /// <summary> Maximale Anzahl an Atomen in der Elementarzelle </summary>
        public const int MAX_ATOM_COUNT = 500;

        /// <summary> Maximale Anzahl an Dotierungen </summary>
        public const int MAX_DOPING_COUNT = 10;

        /// <summary> Maximale Anzahl an Jobs in der JobList </summary>
        public const int MAX_JOB_COUNT = 9999;

        /// <summary> Maximale Anzahl an Remote Profiles </summary>
        public const int MAX_REMOTEPROFILE_COUNT = 999;

        #endregion Numeric Values

        // ********************** KMC Error Codes ********************** //
        #region KMC Error Codes

        /// <summary> KMC_UDLL-Fehlercode: OK, d.h. kein Fehler aufgetreten </summary>
        public const int KMCERR_OK = 0;

        /// <summary> KMC_UDLL-Fehlercode: KMC Objekt-Member Ready nicht true </summary>
        public const int KMCERR_READY_NOT_TRUE = -1;

        /// <summary> KMC_UDLL-Fehlercode: KMC Objekt-Member Ready nicht false </summary>
        public const int KMCERR_READY_NOT_FALSE = -2;

        /// <summary> KMC_UDLL-Fehlercode: Ungueltiger Input </summary>
        public const int KMCERR_INVALID_INPUT = -3;

        /// <summary> KMC_UDLL-Fehlercode: Elementsymbol nicht gefunden </summary>
        public const int KMCERR_ELEMENTSYMBOL_NOT_FOUND = -4;

        /// <summary> KMC_UDLL-Fehlercode: Konsistenzcheck failed </summary>
        public const int KMCERR_MAKE_FAILED_DUE_TO_INCONSISTENCY = -5;

        /// <summary> KMC_UDLL-Fehlercode: Objektpointer ist NULL </summary>
        public const int KMCERR_INVALID_POINTER = -6;

        /// <summary> KMC_UDLL-Fehlercode: Objekt ist noch nicht Ready </summary>
        public const int KMCERR_OBJECT_NOT_READY = -7;

        /// <summary> KMC_UDLL-Fehlercode: Element nicht gefunden </summary>
        public const int KMCERR_ELEMENT_NOT_FOUND = -8;

        /// <summary> KMC_UDLL-Fehlercode: Input existiert schon </summary>
        public const int KMCERR_INPUT_ALREADY_EXISTS = -9;

        /// <summary> KMC_UDLL-Fehlercode: MovingSpecies oder Vacancy als Dotierung eingegeben </summary>
        public const int KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING = -10;

        /// <summary> KMC_UDLL-Fehlercode: Es wurde kein dotiertes Atom in der Elementarzelle plaziert </summary>
        public const int KMCERR_NO_DOPED_ATOM_FOUND = -11;

        /// <summary> KMC_UDLL-Fehlercode: Maximaler Input wurde erreicht </summary>
        public const int KMCERR_MAXIMUM_INPUT_REACHED = -12;

        /// <summary> KMC_UDLL-Fehlercode: Fehler in NN-Analyse: keine Schalen gefunden </summary>
        public const int KMCERR_NO_SHELLS_FOUND = -13;

        /// <summary> KMC_UDLL-Fehlercode: Es duerfen nicht alle Jumps eines Atoms deaktiviert werden </summary>
        public const int KMCERR_NO_ACTIVE_JUMP_ON_ATOM = -14;

        /// <summary> KMC_UDLL-Fehlercode: Es wurde eine Exception ausgeloest </summary>
        public const int KMCERR_EXCEPTION_OCCURED = -15;

        /// <summary> KMC_UDLL-Fehlercode: Ein fehlerhaftes Dateiformat liegt vor </summary>
        public const int KMCERR_INVALID_FILE_FORMAT = -16;

        /// <summary> KMC_UDLL-Fehlercode: Keine Console gefunden </summary>
        public const int KMCERR_NO_CONSOLE = -17;

        /// <summary> KMC_UDLL-Fehlercode: Fehlerhafter Dateiinhalt </summary>
        public const int KMCERR_INVALID_FILE_CONTENT = -18;

        /// <summary> KMC_UDLL-Fehlercode: Test lieferte Ungleich </summary>
        public const int KMCERR_UNEQUAL = -19;

        /// <summary> KMC_UDLL-Fehlercode: Test lieferte Gleich </summary>
        public const int KMCERR_EQUAL = -20;

        /// <summary> KMC_UDLL-Fehlercode: Deaktivierung nicht möglich wegen Mindestanzahl </summary>
        public const int KMCERR_DEACTIVATION_IMPOSSIBLE = -21;

        /// <summary> KMC_UDLL-Fehlercode: Zu viele Atome aktiviert, sodass zuviele Codes entstehen </summary>
        public const int KMCERR_TOO_MANY_ACTIVE_ATOMS = -22;

        /// <summary> KMC_UDLL-Fehlercode: Kritischer, ungueltiger Input </summary> 
        public const int KMCERR_INVALID_INPUT_CRIT = -23;

        /// <summary> KMC_UDLL-Fehlercode: Dyn. Norm. Listeneintrag muss gesetzt werden </summary>
        public const int KMCERR_HAS_TO_SET_DYNNORM_ENTRY = -24;

        /// <summary> KMC_UDLL-Fehlercode: Keine Simulationsergebnisse vorhanden </summary>
        public const int KMCERR_NO_RESULTS = -25;

        #endregion KMC Error Codes

        // ********************** Validation Messages ********************** //
        
    }
}
