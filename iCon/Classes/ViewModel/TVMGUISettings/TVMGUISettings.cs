using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Reflection;
using Microsoft.Win32;
using System.Linq;

namespace iCon_General
{
    /// <summary>
    /// Holds data for misc GUI settings (incl. extended settings tab)
    /// </summary>
    public class TVMGUISettings : ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMGUISettings(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
        }

        #region Properties

        protected ObservableCollection<TVMGUISettingsRemoteProfile> _RemoteProfiles = null;
        /// <summary>
        /// List of all available cluster profiles
        /// </summary>
        public ObservableCollection<TVMGUISettingsRemoteProfile> RemoteProfiles
        {
            get
            {
                return _RemoteProfiles;
            }
            set
            {
                if (value != _RemoteProfiles)
                {
                    _RemoteProfiles = value;
                    Notify("RemoteProfiles");
                }
            }
        }

        protected TVMGUISettingsRemoteProfile _SelectedRemoteProfile = null;
        /// <summary>
        /// Currently used cluster access settings
        /// </summary>
        public TVMGUISettingsRemoteProfile SelectedRemoteProfile
        {
            get
            {
                return _SelectedRemoteProfile;
            }
            set
            {
                if (value != _SelectedRemoteProfile)
                {
                    _SelectedRemoteProfile = value;
                    Notify("SelectedRemoteProfile");
                    Notify("IsRemoteProfileSelected");
                }
            }
        }

        /// <summary>
        /// Indicates whether a remote profile is selected
        /// </summary>
        public bool IsRemoteProfileSelected
        {
            get
            {
                return (_SelectedRemoteProfile != null);
            }
        }

        protected ObservableCollection<string> _SimExeTypes = null;
        /// <summary>
        /// List of all possible simulation executables (without base name and extension)
        /// </summary>
        public ObservableCollection<string> SimExeTypes
        {
            get
            {
                return _SimExeTypes;
            }
            set
            {
                if (value != _SimExeTypes)
                {
                    _SimExeTypes = value;
                    Notify("SimExeTypes");
                }
            }
        }

        protected ObservableCollection<string> _SearchExeTypes = null;
        /// <summary>
        /// List of all possible searcher executables (without base name and extension)
        /// </summary>
        public ObservableCollection<string> SearchExeTypes
        {
            get
            {
                return _SearchExeTypes;
            }
            set
            {
                if (value != _SearchExeTypes)
                {
                    _SearchExeTypes = value;
                    Notify("SearchExeTypes");
                }
            }
        }

        protected string _LocalWorkspace = "";
        /// <summary>
        /// Local workspace (absolute or relative to standard workspace directory)
        /// </summary>
        [StringLength(250, ErrorMessage = "Local workspace directory path has to contain 250 characters or less.")]
        [RegularExpression(@"^[A-Za-z0-9_\\~:\-\(\)\s]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _~:\-() and space.")]
        public string LocalWorkspace
        {
            get
            {
                return _LocalWorkspace;
            }
            set
            {
                ValidateNotify("LocalWorkspace", value, ref _LocalWorkspace);
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Create a path for the folder that contains the settings ini file
        /// </summary>
        /// <returns>Path to the ini file</returns>
        protected string GetIniFolder()
        {
            return Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                ConstantsClass.SC_KMC_APPDATA_OUTERFOLDER, ConstantsClass.SC_KMC_APPDATA_INNERFOLDER);
        }

        /// <summary>
        /// Get paths to all remote profile files
        /// </summary>
        /// <returns>Filenames of the remote profiles</returns>
        protected string [] GetRemoteProfilePaths()
        {
            string i_scriptdir = Path.Combine(GetIniFolder(), ConstantsClass.SC_KMC_USERSCRIPTS_DIR);

            if (Directory.Exists(i_scriptdir) == true)
            {
                return Directory.GetFiles(
                    i_scriptdir, 
                    ConstantsClass.SC_KMC_REMOTEPROFILE_INIFILE,
                    SearchOption.AllDirectories);
            }

            return new string[0];
        }

        /// <summary>
        /// Creates a list of the available simulation executables
        /// </summary>
        /// <returns>List of simulation executable types</returns>
        protected string [] GetSimExeTypes()
        {
            string i_exedir = Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                ConstantsClass.SC_KMC_SIMEXEFOLDER);

            string[] i_exefiles = new string[0];

            // Get a list of executables
            if (Directory.Exists(i_exedir) == true)
            {
                i_exefiles = Directory.GetFiles(
                    i_exedir,
                    ConstantsClass.SC_KMC_BASESIMEXE + "*" + ConstantsClass.SC_KMC_EXTSIMEXE,
                    SearchOption.AllDirectories);
            }

            // Convert executable list to list of exe types
            for (int i = 0; i < i_exefiles.Length; ++i)
            {
                i_exefiles[i] = Path.GetFileName(i_exefiles[i]);
                i_exefiles[i] = i_exefiles[i].Replace(ConstantsClass.SC_KMC_BASESIMEXE, "");
                i_exefiles[i] = i_exefiles[i].Replace(ConstantsClass.SC_KMC_EXTSIMEXE, "");
                i_exefiles[i] = i_exefiles[i].Trim();
            }

            return i_exefiles;
        }

        /// <summary>
        /// Creates a list of the available search executables
        /// </summary>
        /// <returns>List of search executable types</returns>
        protected string[] GetSearchExeTypes()
        {
            string i_exedir = Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                ConstantsClass.SC_KMC_SEARCHEXEFOLDER);

            string[] i_exefiles = new string[0];

            // Get a list of executables
            if (Directory.Exists(i_exedir) == true)
            {
                i_exefiles = Directory.GetFiles(
                    i_exedir,
                    ConstantsClass.SC_KMC_BASESEARCHEXE + "*" + ConstantsClass.SC_KMC_EXTSEARCHEXE,
                    SearchOption.AllDirectories);
            }

            // Convert executable list to list of exe types
            for (int i = 0; i < i_exefiles.Length; ++i)
            {
                i_exefiles[i] = Path.GetFileName(i_exefiles[i]);
                i_exefiles[i] = i_exefiles[i].Replace(ConstantsClass.SC_KMC_BASESEARCHEXE, "");
                i_exefiles[i] = i_exefiles[i].Replace(ConstantsClass.SC_KMC_EXTSEARCHEXE, "");
                i_exefiles[i] = i_exefiles[i].Trim();
            }

            return i_exefiles;
        }

        /// <summary>
        /// Adjust the remote profile IDs so there are no unused IDs inbetween
        /// </summary>
        protected void AdjustProfileIDs()
        {
            if ((_RemoteProfiles != null) && (_RemoteProfiles.Count > 0))
            {
                // Save selected remote profile object and clear it (would be cleared anyways when profile list is cleared)
                TVMGUISettingsRemoteProfile t_SelectedProfile = _SelectedRemoteProfile;
                SelectedRemoteProfile = null;

                // Create list of sorted profiles (array prevents deletion when clearing list below)
                TVMGUISettingsRemoteProfile[] t_SortedProfiles = _RemoteProfiles.OrderBy(x => x.ID).ToArray();

                // Add the sorted profiles again and update their ID if necessary
                RemoteProfiles.Clear();
                for (int i = 0; i < t_SortedProfiles.Length; ++i)
                {
                    t_SortedProfiles[i].ID = i;
                    RemoteProfiles.Add(t_SortedProfiles[i]);
                }

                // Reselect remote profile
                SelectedRemoteProfile = t_SelectedProfile;
            }
            else
            {
                SelectedRemoteProfile = null;
            }            
        }

        /// <summary>
        /// Creates a path to the local simulation executable
        /// </summary>
        public string GetLocalSimExecutablePath()
        {
            return Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                ConstantsClass.SC_KMC_SIMEXEFOLDER,
                ConstantsClass.SC_KMC_LOCALSIMEXE);
        }

        /// <summary>
        /// Creates a path to the local searcher executable
        /// </summary>
        public string GetLocalSearchExecutablePath()
        {
            return Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                ConstantsClass.SC_KMC_SEARCHEXEFOLDER,
                ConstantsClass.SC_KMC_LOCALSEARCHEXE);
        }

        /// <summary>
        /// Creates full path for local workspace
        /// </summary>
        public string GetLocalWorkspacePath()
        {
            string t_workspace = _LocalWorkspace;

            if (string.IsNullOrWhiteSpace(t_workspace) || (Path.IsPathRooted(t_workspace) == false))
            {
                t_workspace = Path.Combine(
                    Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments),
                    ConstantsClass.SC_KMC_STD_WORKING_DIR,
                    t_workspace.Trim());
            }

            return t_workspace.Trim();
        }

        /// <summary>
        /// Show file open dialog for private key selection
        /// </summary>
        public void SelectPrivateKeyFile()
        {
            // Check for valid profile
            if (_SelectedRemoteProfile == null) return;

            // Get key file path
            OpenFileDialog opendlg = new OpenFileDialog();
            opendlg.InitialDirectory = Directory.GetCurrentDirectory();
            opendlg.Title = "Select OpenSSH Private Key File";
            opendlg.Multiselect = false;
            opendlg.ValidateNames = true;
            if (opendlg.ShowDialog() == true)
            {
                _SelectedRemoteProfile.PrivateKeyPath = opendlg.FileName;
            }
        }

        /// <summary>
        /// Adds a new profile for remote simulation
        /// </summary>
        public void AddRemoteProfile()
        {
            // Check for profile list
            if (_RemoteProfiles == null)
            {
                _RemoteProfiles = new ObservableCollection<TVMGUISettingsRemoteProfile>();
            }

            // Sort current profiles
            AdjustProfileIDs();

            // Check if would exceed profile limit
            if (_RemoteProfiles.Count >= ConstantsClass.MAX_REMOTEPROFILE_COUNT) return;

            // Determine next ID value
            int t_NextID = 0;
            if (_RemoteProfiles.Count > 0)
            {
                t_NextID = _RemoteProfiles.Last<TVMGUISettingsRemoteProfile>().ID + 1;
            }

            // Add an new profile
            RemoteProfiles.Add(new TVMGUISettingsRemoteProfile(_ViewModel, t_NextID));

            // Select the added profile
            SelectedRemoteProfile = _RemoteProfiles.Last<TVMGUISettingsRemoteProfile>();
        }

        /// <summary>
        /// Creates a new remote profile based on the selected profile
        /// </summary>
        public void CopyRemoteProfile()
        {
            // Check for valid selected profile
            if (_SelectedRemoteProfile == null) return;
            if ((_RemoteProfiles == null) || (_RemoteProfiles.Count == 0))
            {
                SelectedRemoteProfile = null;
                return;
            }

            // Sort current profiles
            AdjustProfileIDs();

            // Check if would exceed profile limit
            if (_RemoteProfiles.Count >= ConstantsClass.MAX_REMOTEPROFILE_COUNT) return;

            // Determine next ID value
            int t_NextID = _RemoteProfiles.Last<TVMGUISettingsRemoteProfile>().ID + 1;

            // Add a copy of selected profile
            RemoteProfiles.Add(new TVMGUISettingsRemoteProfile(_ViewModel, _SelectedRemoteProfile, t_NextID));

            // Select the added profile
            SelectedRemoteProfile = _RemoteProfiles.Last<TVMGUISettingsRemoteProfile>();
        }

        /// <summary>
        /// Deletes the selected remote profile (with confirmation)
        /// </summary>
        public void DeleteRemoteProfile()
        {
            // Check for valid selected profile
            if (_SelectedRemoteProfile == null) return;
            if ((_RemoteProfiles == null) || (_RemoteProfiles.Count == 0))
            {
                SelectedRemoteProfile = null;
                return;
            }

            // Determine next selected profile
            int t_Index = _RemoteProfiles.IndexOf(_SelectedRemoteProfile);
            if (t_Index < 0)
            {
                throw new ApplicationException("Cannot find selected profile in profile list (TVMGUISettings.DeleteRemoteProfile)");
            }
            TVMGUISettingsRemoteProfile t_NextSelection = null;
            if (t_Index < _RemoteProfiles.Count - 1)
            {
                t_NextSelection = _RemoteProfiles[t_Index + 1];
            }
            else
            {
                if (t_Index > 0)
                {
                    t_NextSelection = _RemoteProfiles[t_Index - 1];
                }
            }

            // Delete selected profile
            _SelectedRemoteProfile.DeleteFiles();
            RemoteProfiles.Remove(_SelectedRemoteProfile);

            // Select the next profile
            SelectedRemoteProfile = t_NextSelection;

            // Sort the profiles
            AdjustProfileIDs();
        }

        /// <summary>
        /// Validate the job-submit-related properties of this object and of all required sub-objects
        /// </summary>
        /// <param name="OnlyLocalSimulation">true = check if valid for local simulation only</param>
        public bool ValidateForSubmit(bool OnlyLocalSimulation)
        {
            if (ValidateProperty("LocalWorkspace", _LocalWorkspace) == false) return false;
            try
            {
                Directory.CreateDirectory(GetLocalWorkspacePath());
            }
            catch (Exception e)
            {
                throw new ApplicationException("Failed to create local workspace directory (TVMGUISettings.ValidateForSubmit)", e);
            }

            if (OnlyLocalSimulation == false)
            {
                // Check if everything ready for remote simulation
                if (_SelectedRemoteProfile != null)
                {
                    if (_SelectedRemoteProfile.ValidateForSubmit() == false) return false;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                // Check if everything ready for local simulation
                if (File.Exists(GetLocalSimExecutablePath()) == false) return false;
                if (File.Exists(GetLocalSearchExecutablePath()) == false) return false;
            }

            return true;
        }

        /// <summary>
        /// Write GUI settings to ini-File
        /// </summary>
        public void SaveToIniFile()
        {
            try
            {
                using (StreamWriter swriter = new StreamWriter(Path.Combine(GetIniFolder(), ConstantsClass.SC_KMC_STD_INIFILE)))
                {
                    // Write file header
                    swriter.WriteLine("GUI INI FILE");
                    swriter.WriteLine("iCon - Kinetic Monte-Carlo Simulation of Ionic Conductivity");
                    swriter.WriteLine("Copyright 2014");
                    swriter.WriteLine("RWTH Aachen, IPC, Workgroup Martin");
                    swriter.WriteLine("Version: " + Assembly.GetExecutingAssembly().GetName().Version.ToString());
                    swriter.WriteLine("File creation time: " + DateTime.Now.ToString());
                    swriter.WriteLine();

                    // Write settings start
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_INI_START);

                    // Write information
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_INI_OFFSET + ConstantsClass.SC_KMC_OUT_INI_LOCALWORKSPACE + " " + _LocalWorkspace.Trim());
                    if (_SelectedRemoteProfile != null)
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_INI_OFFSET + ConstantsClass.SC_KMC_OUT_INI_SELREMOTEPROFILE + " " + _SelectedRemoteProfile.ID.ToString());
                    }

                    // Write settings end
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_INI_END);
                }
            }
            catch (Exception e)
            {
                throw new ApplicationException("Failed to write GUI settings ini file (TVMGUISettings.SaveToIniFile)", e);
            }

            // Save remote profiles
            if (_RemoteProfiles != null)
            {
                for (int i = 0; i < _RemoteProfiles.Count; i++)
                {
                    _RemoteProfiles[i].SaveToIniFile();
                }
            }
        }

        /// <summary>
        /// Load GUI settings from ini-File
        /// </summary>
        public void LoadFromIniFile()
        {
            // Create settings file path
            string i_settingsfile = Path.Combine(GetIniFolder(), ConstantsClass.SC_KMC_STD_INIFILE);

            // Initialize new data
            string t_LocalWorkspace = "";
            string t_SelectedRemoteProfile = "0";

            try
            {
                if (File.Exists(i_settingsfile) == true)
                {
                    using (StreamReader sreader = new StreamReader(i_settingsfile))
                    {
                        bool t_hasinifile = false;
                        string t_line = "";
                        while ((t_line = sreader.ReadLine()) != null)
                        {
                            t_line = t_line.Trim();

                            // Load information
                            if (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_INI_START) == true)
                            {
                                t_hasinifile = true;
                            }
                            if ((t_hasinifile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_INI_END) == true))
                            {
                                break;
                            }
                            if ((t_hasinifile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_INI_LOCALWORKSPACE) == true))
                            {
                                t_LocalWorkspace = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_INI_LOCALWORKSPACE.Length).Trim();
                            }
                            if ((t_hasinifile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_INI_SELREMOTEPROFILE) == true))
                            {
                                t_SelectedRemoteProfile = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_INI_SELREMOTEPROFILE.Length).Trim();
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                throw new ApplicationException("Error during reading of GUI ini file (TVMGUISettings.LoadFromIniFile)", e);
            }

            // Validate workspace path
            if (ValidateProperty("LocalWorkspace", t_LocalWorkspace) == false)
            {
                t_LocalWorkspace = "";
            }

            // Parse selected remote profile
            int t_SelProfileID = 0;
            try
            {
                t_SelProfileID = int.Parse(t_SelectedRemoteProfile);
            }
            catch (Exception e)
            {
                throw new ApplicationException("Error during parsing of selected profile ID (TVMGUISettings.LoadFromIniFile)", e);
            }

            // Load remote profiles
            string[] t_ProfileFilepaths = GetRemoteProfilePaths();
            if (t_ProfileFilepaths.Length > ConstantsClass.MAX_REMOTEPROFILE_COUNT)
            {
                throw new ApplicationException("Too many remote profiles (TVMGUISettings.LoadFromIniFile)");
            }
            ObservableCollection<TVMGUISettingsRemoteProfile> t_Profiles = new ObservableCollection<TVMGUISettingsRemoteProfile>();
            for (int i = 0; i < t_ProfileFilepaths.Length; ++i)
            {
                TVMGUISettingsRemoteProfile t_RemoteProfile = new TVMGUISettingsRemoteProfile(_ViewModel, 0);
                t_RemoteProfile.LoadFromIniFile(t_ProfileFilepaths[i]);
                t_Profiles.Add(t_RemoteProfile);
            }

            // Collect simulation and searcher executable types
            ObservableCollection<string> t_SimExeTypes = new ObservableCollection<string>(GetSimExeTypes());
            ObservableCollection<string> t_SearchExeTypes = new ObservableCollection<string>(GetSearchExeTypes());

            // Transfer data
            SimExeTypes = t_SimExeTypes;
            SearchExeTypes = t_SearchExeTypes;
            RemoteProfiles = t_Profiles;
            LocalWorkspace = t_LocalWorkspace;

            // Select remote profile
            if ((t_SelProfileID >= 0) && (t_SelProfileID < _RemoteProfiles.Count))
            {
                SelectedRemoteProfile = _RemoteProfiles[t_SelProfileID];
            }
            else
            {
                if (_RemoteProfiles.Count > 0)
                {
                    SelectedRemoteProfile = _RemoteProfiles[0];
                }
                else
                {
                    SelectedRemoteProfile = null;
                }
            }

            // Correct profile ordering and IDs
            AdjustProfileIDs();

            // Set current directory to local workspace
            string t_locpath = GetLocalWorkspacePath();
            if (Directory.Exists(t_locpath) == true)
            {
                Directory.SetCurrentDirectory(t_locpath);
            }
        }

        #endregion Methods
    }
}
