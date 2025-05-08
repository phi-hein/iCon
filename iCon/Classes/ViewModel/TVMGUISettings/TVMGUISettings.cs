using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Reflection;
using Microsoft.Win32;
using System.Linq;
using System.Security.Cryptography;

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
                Constants.SC_KMC_APPDATA_OUTERFOLDER, Constants.SC_KMC_APPDATA_INNERFOLDER);
        }

        /// <summary>
        /// Get paths to all remote profile files
        /// </summary>
        /// <returns>Filenames of the remote profiles</returns>
        protected string [] GetRemoteProfilePaths()
        {
            string i_scriptdir = Path.Combine(GetIniFolder(), Constants.SC_KMC_USERSCRIPTS_DIR);

            if (Directory.Exists(i_scriptdir) == true)
            {
                return Directory.GetFiles(
                    i_scriptdir, 
                    Constants.SC_KMC_REMOTEPROFILE_INIFILE,
                    SearchOption.AllDirectories);
            }

            return new string[0];
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
                Constants.SC_KMC_LOCALSIMEXE);
        }

        /// <summary>
        /// Creates a path to the local searcher executable
        /// </summary>
        public string GetLocalSearchExecutablePath()
        {
            return Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                Constants.SC_KMC_LOCALSEARCHEXE);
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
                    Constants.SC_KMC_STD_WORKING_DIR,
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
            if (_RemoteProfiles.Count >= Constants.MAX_REMOTEPROFILE_COUNT) return;

            // Determine next ID value
            int t_NextID = 0;
            if (_RemoteProfiles.Count > 0)
            {
                t_NextID = _RemoteProfiles.Last().ID + 1;
            }

            // Add an new profile
            RemoteProfiles.Add(new TVMGUISettingsRemoteProfile(_ViewModel, t_NextID));

            // Load default scripts
            RemoteProfiles.Last().LoadDefaultScripts();

            // Select the added profile
            SelectedRemoteProfile = _RemoteProfiles.Last();
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
            if (_RemoteProfiles.Count >= Constants.MAX_REMOTEPROFILE_COUNT) return;

            // Determine next ID value
            int t_NextID = _RemoteProfiles.Last().ID + 1;

            // Add a copy of selected profile
            RemoteProfiles.Add(new TVMGUISettingsRemoteProfile(_ViewModel, _SelectedRemoteProfile, t_NextID));

            // Select the added profile
            SelectedRemoteProfile = _RemoteProfiles.Last();
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
        /// <param name="isRemoteSimulation">true = check if valid for remote simulation</param>
        public bool ValidateForSubmit(bool isRemoteSimulation)
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

            if (isRemoteSimulation == true)
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
                using (StreamWriter swriter = new StreamWriter(Path.Combine(GetIniFolder(), Constants.SC_KMC_STD_INIFILE)))
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
                    swriter.WriteLine(Constants.SC_KMC_OUT_INI_START);

                    // Write information
                    swriter.WriteLine(Constants.SC_KMC_OUT_INI_OFFSET + Constants.SC_KMC_OUT_INI_LOCALWORKSPACE + " " + _LocalWorkspace.Trim());
                    if (_SelectedRemoteProfile != null)
                    {
                        swriter.WriteLine(Constants.SC_KMC_OUT_INI_OFFSET + Constants.SC_KMC_OUT_INI_SELREMOTEPROFILE + " " + _SelectedRemoteProfile.ID.ToString());
                    }

                    // Write settings end
                    swriter.WriteLine(Constants.SC_KMC_OUT_INI_END);
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
            string i_settingsfile = Path.Combine(GetIniFolder(), Constants.SC_KMC_STD_INIFILE);

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
                            if (t_line.StartsWith(Constants.SC_KMC_OUT_INI_START) == true)
                            {
                                t_hasinifile = true;
                            }
                            if ((t_hasinifile == true) && (t_line.StartsWith(Constants.SC_KMC_OUT_INI_END) == true))
                            {
                                break;
                            }
                            if ((t_hasinifile == true) && (t_line.StartsWith(Constants.SC_KMC_OUT_INI_LOCALWORKSPACE) == true))
                            {
                                t_LocalWorkspace = t_line.Remove(0, Constants.SC_KMC_OUT_INI_LOCALWORKSPACE.Length).Trim();
                            }
                            if ((t_hasinifile == true) && (t_line.StartsWith(Constants.SC_KMC_OUT_INI_SELREMOTEPROFILE) == true))
                            {
                                t_SelectedRemoteProfile = t_line.Remove(0, Constants.SC_KMC_OUT_INI_SELREMOTEPROFILE.Length).Trim();
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
            int t_SelProfileID;
            try
            {
                t_SelProfileID = int.Parse(t_SelectedRemoteProfile);
            }
            catch (Exception e)
            {
                throw new ApplicationException("Error during parsing of selected profile ID (TVMGUISettings.LoadFromIniFile)", e);
            }

            // Load remote profiles (and find selected profile)
            string[] t_ProfileFilepaths = GetRemoteProfilePaths();
            if (t_ProfileFilepaths.Length > Constants.MAX_REMOTEPROFILE_COUNT)
            {
                throw new ApplicationException("Too many remote profiles (TVMGUISettings.LoadFromIniFile)");
            }
            TVMGUISettingsRemoteProfile t_SelProfile = null;
            ObservableCollection<TVMGUISettingsRemoteProfile> t_Profiles = new ObservableCollection<TVMGUISettingsRemoteProfile>();
            for (int i = 0; i < t_ProfileFilepaths.Length; ++i)
            {
                TVMGUISettingsRemoteProfile t_RemoteProfile = new TVMGUISettingsRemoteProfile(_ViewModel, 0);
                t_RemoteProfile.LoadFromIniFile(t_ProfileFilepaths[i]);
                t_Profiles.Add(t_RemoteProfile);

                if (t_SelProfileID == t_Profiles[i].ID) t_SelProfile = t_Profiles[i];
            }

            // Check whether any profiles have the same ID (as a result of parsing directory name)
            for (int i = 0; i < t_Profiles.Count; ++i)
            {
                for (int j = i + 1; j < t_Profiles.Count; ++j)
                {
                    if (t_Profiles[i].ID == t_Profiles[j].ID)
                    {
                        throw new ApplicationException("Duplicate profile IDs parsed from directory names (TVMGUISettings.LoadFromIniFile)");
                    }
                }
            }

            // Check whether any directory name differs from desired format and rename the directory accordingly
            for (int i = 0; i < t_Profiles.Count; ++i)
            {
                string current_dir_path = Path.GetDirectoryName(t_ProfileFilepaths[i]);
                string desired_dir_path = TVMGUISettingsRemoteProfile.GetBaseFolder(t_Profiles[i].ID);
                if (Path.GetFullPath(current_dir_path) != Path.GetFullPath(desired_dir_path))
                {
                    if ((Directory.Exists(current_dir_path) == true) && (Directory.Exists(desired_dir_path) == false))
                    {
                        Directory.Move(current_dir_path, desired_dir_path);
                    }
                    else
                    {
                        throw new ApplicationException("Invalid remote profile directory layout (TVMGUISettings.LoadFromIniFile)");
                    }
                }
            }

            // Transfer data
            RemoteProfiles = t_Profiles;
            LocalWorkspace = t_LocalWorkspace;

            // Select remote profile
            if (t_SelProfile != null)
            {  
                SelectedRemoteProfile = t_SelProfile;
            }
            else if ((t_SelProfileID >= 0) && (t_SelProfileID < _RemoteProfiles.Count))
            {
                SelectedRemoteProfile = _RemoteProfiles[t_SelProfileID];
            }
            else if (_RemoteProfiles.Count > 0)
            {
                SelectedRemoteProfile = _RemoteProfiles[0];
            }
            else
            {
                SelectedRemoteProfile = null;
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
