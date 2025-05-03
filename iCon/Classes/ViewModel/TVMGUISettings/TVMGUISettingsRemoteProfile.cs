using System;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Reflection;

namespace iCon_General
{
    /// <summary>
    /// Holds data that specifies the access settings for some remote simulation server
    /// </summary>
    public class TVMGUISettingsRemoteProfile: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMGUISettingsRemoteProfile(TMCViewModel ViewModel, int NewID)
        {
            _ViewModel = ViewModel;

            // Set ID (without moving directory)
            _ID = NewID;
            Notify("ID");
        }

        public TVMGUISettingsRemoteProfile(TMCViewModel ViewModel, TVMGUISettingsRemoteProfile BaseProfile, int NewID)
        {
            _ViewModel = ViewModel;

            // Set ID (without moving directory)
            _ID = NewID;
            Notify("ID");

            // Transfer data
            if (BaseProfile != null)
            {
                Name = BaseProfile.Name;
                RemoteWorkspace = BaseProfile.RemoteWorkspace;
                RemoteBuildDir = BaseProfile.RemoteBuildDir;
                HostAdress = BaseProfile.HostAdress;
                HostPort = BaseProfile.HostPort;
                Username = BaseProfile.Username;
                WithPassword = BaseProfile.WithPassword;
                UserPassword = BaseProfile.UserPassword;
                WithPrivateKey = BaseProfile.WithPrivateKey;
                PrivateKeyPath = BaseProfile.PrivateKeyPath;
                PrivateKeyPassword = BaseProfile.PrivateKeyPassword;
                WithKeyboardInteractive = BaseProfile.WithKeyboardInteractive;
                HostFingerPrint = BaseProfile.HostFingerPrint;
                SimSubmitScript = BaseProfile.SimSubmitScript;
                SimJobScript = BaseProfile.SimJobScript;
                BuildScript = BaseProfile.BuildScript;
                AskConfigured = BaseProfile.AskConfigured;
            }
        }

        #region Properties

        protected int _ID = 0;
        /// <summary>
        /// Number of the remote profile
        /// </summary>
        public int ID
        {
            get
            {
                return _ID;
            }
            set
            {
                if (value != _ID)
                {
                    string i_old_dir = GetBaseFolder(_ID);
                    string i_new_dir = GetBaseFolder(value);
                    if (Directory.Exists(i_old_dir) == true)
                    {
                        if (Directory.Exists(i_new_dir) == false)
                        {
                            Directory.Move(i_old_dir, i_new_dir);
                            _ID = value;
                        }
                        else
                        {
                            throw new ApplicationException("Invalid remote profile directory layout (TVMGUISettingsRemoteProfile.ID.Set)");
                        }
                    }
                    Notify("ID");
                }
            }
        }

        protected string _Name = "";
        /// <summary>
        /// Name of the remote profile
        /// </summary>
        [StringLength(20, ErrorMessage = "Name of remote profile has to contain 20 characters or less.")]
        [RegularExpression(@"^[A-Za-z0-9_\-\(\)\s]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _-() and space.")]
        public string Name
        {
            get
            {
                return _Name;
            }
            set
            {
                ValidateNotify("Name", value.Trim(), ref _Name);
            }
        }

        protected string _RemoteWorkspace = "";
        /// <summary>
        /// Relative or absolute path to remote workspace directory (if relative, then relative to user home directory)
        /// </summary>
        [StringLength(250, ErrorMessage = "Remote workspace directory path has to contain 250 characters or less.")]
        [RegularExpression(@"^[A-Za-z0-9_/\-\(\)\s]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _/-() and space.")]
        public string RemoteWorkspace
        {
            get
            {
                return _RemoteWorkspace;
            }
            set
            {
                ValidateNotify("RemoteWorkspace", value.Trim(), ref _RemoteWorkspace);
            }
        }

        protected string _RemoteBuildDir = "build";
        /// <summary>
        /// Relative path to remote build directory (relative to remote workspace directory)
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid build directory.")]
        [StringLength(100, ErrorMessage = "Remote build path has to contain 100 characters or less.")]
        [RegularExpression(@"^[A-Za-z0-9_/\-\(\)\s]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _/-() and space.")]
        public string RemoteBuildDir
        {
            get
            {
                return _RemoteBuildDir;
            }
            set
            {
                ValidateNotify("RemoteBuildDir", value.Trim(), ref _RemoteBuildDir);
            }
        }

        protected string _HostAdress = "";
        /// <summary>
        /// Adress of the cluster (e.g. IP)
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid remote host adress.")]
        [StringLength(253, ErrorMessage = "Remote host adress has to contain 253 characters or less.")]
        [RegularExpression(@"^[A-Za-z0-9_\.\-]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _.-.")]
        public string HostAdress
        {
            get
            {
                return _HostAdress;
            }
            set
            {
                ValidateNotify("HostAdress", value.Trim(), ref _HostAdress);
            }
        }

        protected int _HostPort = 22;
        /// <summary>
        /// Port that is used to connect to the cluster
        /// </summary>
        [IntRange(0, true, 65535, true, ErrorMessage = "Port number has to be in the interval [0, 65535].")]
        public int HostPort
        {
            get
            {
                return _HostPort;
            }
            set
            {
                ValidateNotify("HostPort", value, ref _HostPort);
            }
        }

        protected string _Username = "";
        /// <summary>
        /// Username for the cluster access
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid user name.")]
        [StringLength(50, ErrorMessage = "User name has to contain 50 characters or less.")]
        [RegularExpression(@"^[A-Za-z0-9_\-\(\)]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _() and -.")]
        public string Username
        {
            get
            {
                return _Username;
            }
            set
            {
                ValidateNotify("Username", value.Trim(), ref _Username);
            }
        }

        protected bool _WithPassword = true;
        /// <summary>
        /// Flag, true = access cluster with password authentification
        /// </summary>
        public bool WithPassword
        {
            get
            {
                return _WithPassword;
            }
            set
            {
                if (value != _WithPassword)
                {
                    _WithPassword = value;
                    Notify("WithPassword");
                }
            }
        }

        protected string _UserPassword = "";
        /// <summary>
        /// Password for password-only access to the cluster
        /// (PasswordBox will display no validation)
        /// </summary>
        public string UserPassword
        {
            get
            {
                return _UserPassword;
            }
            set
            {
                if (value != _UserPassword)
                {
                    _UserPassword = value;
                    Notify("UserPassword");
                }
            }
        }

        protected bool _WithPrivateKey = false;
        /// <summary>
        /// Flag, true = access cluster with private key authentification
        /// </summary>
        public bool WithPrivateKey
        {
            get
            {
                return _WithPrivateKey;
            }
            set
            {
                if (value != _WithPrivateKey)
                {
                    _WithPrivateKey = value;
                    Notify("WithPrivateKey");
                }
            }
        }

        protected string _PrivateKeyPath = "";
        /// <summary>
        /// Path to the private key
        /// </summary>
        public string PrivateKeyPath
        {
            get
            {
                return _PrivateKeyPath;
            }
            set
            {
                if (value != _PrivateKeyPath)
                {
                    _PrivateKeyPath = value;
                    Notify("PrivateKeyPath");
                }
            }
        }

        protected string _PrivateKeyPassword = "";
        /// <summary>
        /// Contains the password of the private key file
        /// (PasswordBox will display no validation)
        /// </summary>
        public string PrivateKeyPassword
        {
            get
            {
                return _PrivateKeyPassword;
            }
            set
            {
                if (value != _PrivateKeyPassword)
                {
                    _PrivateKeyPassword = value;
                    Notify("PrivateKeyPassword");
                }
            }
        }

        protected bool _WithKeyboardInteractive = true;
        /// <summary>
        /// Flag, true = access cluster with keyboard-interactive authentification
        /// </summary>
        public bool WithKeyboardInteractive
        {
            get
            {
                return _WithKeyboardInteractive;
            }
            set
            {
                if (value != _WithKeyboardInteractive)
                {
                    _WithKeyboardInteractive = value;
                    Notify("WithKeyboardInteractive");
                }
            }
        }

        protected string _HostFingerPrint = "";
        /// <summary>
        /// Stores the user-accepted host fingerprint
        /// </summary>
        public string HostFingerPrint
        {
            get
            {
                return _HostFingerPrint;
            }
            set
            {
                if (value != _HostFingerPrint)
                {
                    _HostFingerPrint = value;
                    Notify("HostFingerPrint");
                }
            }
        }

        protected string _SimSubmitScript = "";
        /// <summary>
        /// Script to submit simulation job to queue system
        /// </summary>
        public string SimSubmitScript
        {
            get
            {
                return _SimSubmitScript;
            }
            set
            {
                ValidateNotify("SimSubmitScript", value, ref _SimSubmitScript);
            }
        }

        protected string _SimJobScript = "";
        /// <summary>
        /// Script for executing simulation job
        /// </summary>
        public string SimJobScript
        {
            get
            {
                return _SimJobScript;
            }
            set
            {
                ValidateNotify("SimJobScript", value, ref _SimJobScript);
            }
        }

        protected string _BuildScript = "";
        /// <summary>
        /// Script to carry out remote compilation
        /// </summary>
        public string BuildScript
        {
            get
            {
                return _BuildScript;
            }
            set
            {
                ValidateNotify("BuildScript", value, ref _BuildScript);
            }
        }

        protected bool _AskConfigured = true;
        /// <summary>
        /// Flag, true = ask whether profile was configured before running submission routines
        /// </summary>
        public bool AskConfigured
        {
            get
            {
                return _AskConfigured;
            }
            set
            {
                if (value != _AskConfigured)
                {
                    _AskConfigured = value;
                    Notify("AskConfigured");
                }
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Create a path for the folder that contains a remote profile
        /// </summary>
        /// <param name="i_ID">Number of the profile</param>
        /// <returns>Path to the profile folder</returns>
        public static string GetBaseFolder(int i_ID)
        {
            return Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                ConstantsClass.SC_KMC_APPDATA_OUTERFOLDER,
                ConstantsClass.SC_KMC_APPDATA_INNERFOLDER,
                ConstantsClass.SC_KMC_USERSCRIPTS_DIR,
                ConstantsClass.SC_KMC_REMOTEPROFILE_DIR + i_ID.ToString(ConstantsClass.SC_KMC_REMOTEPROFILE_ID_FORMAT));
        }

        /// <summary>
        /// Create a path for a file residing in the default scripts folder
        /// </summary>
        /// <param name="i_Filename">Filename to append to scripts folder</param>
        /// <returns>Path for the script file</returns>
        protected static string GetDefaultScriptPath(string i_Filename)
        {
            return Path.Combine(
                Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                ConstantsClass.SC_KMC_SCRIPTSFOLDER,
                i_Filename);
        }

        /// <summary>
        /// Create a path for a file residing in the remote profile folder
        /// </summary>
        /// <param name="i_Filename">Filename to append to base folder</param>
        /// <returns>Path for the profile-related file</returns>
        public string GetRemoteProfileFilePath(string i_Filename)
        {
            return Path.Combine(
                GetBaseFolder(_ID),
                i_Filename);
        }

        /// <summary>
        /// Save submit, job and build script to remote profile folder
        /// </summary>
        public void SaveScripts()
        {
            // Create folder if not present
            Directory.CreateDirectory(GetBaseFolder(_ID));

            // Replace newline characters
            SimSubmitScript = _SimSubmitScript.Replace("\r\n", "\n");
            SimJobScript = _SimJobScript.Replace("\r\n", "\n");
            BuildScript = _BuildScript.Replace("\r\n", "\n");

            // Write scripts to file
            File.WriteAllText(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_SUBMITSCRIPT), _SimSubmitScript);
            File.WriteAllText(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_JOBSCRIPT), _SimJobScript);
            File.WriteAllText(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_BUILDSCRIPT), _BuildScript);
        }

        /// <summary>
        /// Load submit, job and build script from remote profile folder (if they exist)
        /// </summary>
        public void LoadScripts()
        {
            // Load scripts from files
            if (File.Exists(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_SUBMITSCRIPT)) == true)
            {
                SimSubmitScript = File.ReadAllText(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_SUBMITSCRIPT));
            }
            if (File.Exists(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_JOBSCRIPT)) == true)
            {
                SimJobScript = File.ReadAllText(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_JOBSCRIPT));
            }
            if (File.Exists(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_BUILDSCRIPT)) == true)
            {
                BuildScript = File.ReadAllText(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_BUILDSCRIPT));
            }
        }

        /// <summary>
        /// Load default submit, job and build scripts
        /// </summary>
        public void LoadDefaultScripts()
        {
            LoadDefaultSubmitScript();
            LoadDefaultJobScript();
            LoadDefaultBuildScript();
        }

        /// <summary>
        /// Load default submit script
        /// </summary>
        public void LoadDefaultSubmitScript()
        {
            // Load script from file
            SimSubmitScript = File.ReadAllText(GetDefaultScriptPath(ConstantsClass.SC_KMC_SUBMITSCRIPT));
        }

        /// <summary>
        /// Load default job script
        /// </summary>
        public void LoadDefaultJobScript()
        {
            // Load script from file
            SimJobScript = File.ReadAllText(GetDefaultScriptPath(ConstantsClass.SC_KMC_JOBSCRIPT));
        }

        /// <summary>
        /// Load default build script
        /// </summary>
        public void LoadDefaultBuildScript()
        {
            // Load script from file
            BuildScript = File.ReadAllText(GetDefaultScriptPath(ConstantsClass.SC_KMC_BUILDSCRIPT));
        }

        /// <summary>
        /// Delete all files and folders associated with this profile
        /// </summary>
        public void DeleteFiles()
        {
            // Get path to profile folder
            string t_dirpath = GetBaseFolder(_ID);

            // Remove folder of this profile
            if (Directory.Exists(t_dirpath) == true)
            {
                Directory.Delete(t_dirpath, true);
            }
        }

        /// <summary>
        /// Validate the job-submit-related properties of this object and of all sub-objects
        /// </summary>
        public bool ValidateForSubmit()
        {
            if (ValidateObject() == false) return false;

            // Save current scripts
            SaveScripts();

            // Check if private key exists if enabled
            if (_WithPrivateKey == true)
            {
                if (File.Exists(PrivateKeyPath) == false)
                {
                    return false;
                }
            }
            
            return true;
        }

        /// <summary>
        /// Write remote profile to an ini file
        /// </summary>
        public void SaveToIniFile()
        {
            try
            {
                // Create profile folder if not present
                Directory.CreateDirectory(GetBaseFolder(_ID));

                // Write ini file
                using (StreamWriter swriter = new StreamWriter(GetRemoteProfileFilePath(ConstantsClass.SC_KMC_REMOTEPROFILE_INIFILE)))
                {
                    // Write file header
                    swriter.WriteLine("GUI REMOTE PROFILE INI FILE");
                    swriter.WriteLine("iCon - Kinetic Monte-Carlo Simulation of Ionic Conductivity");
                    swriter.WriteLine("Copyright 2014");
                    swriter.WriteLine("RWTH Aachen, IPC, Workgroup Martin");
                    swriter.WriteLine("Version: " + Assembly.GetExecutingAssembly().GetName().Version.ToString());
                    swriter.WriteLine("File creation time: " + DateTime.Now.ToString());
                    swriter.WriteLine();

                    // Write information to stream
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_START);
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_NAME + " " + _Name.Trim());
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WORKSPACE + " " + _RemoteWorkspace.Trim());
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_BUILDDIR + " " + _RemoteBuildDir.Trim());
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_HOSTADRESS + " " + _HostAdress.Trim());
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_PORT + " " + _HostPort.ToString());
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_USERNAME + " " + _Username.Trim());
                    if (_WithPassword == true)
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WITHPW + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_TRUE);
                    }
                    else
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WITHPW + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE);
                    }
                    if (_WithPrivateKey == true)
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WITHKEY + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_TRUE);
                    }
                    else
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WITHKEY + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE);
                    }
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_PRIVATEKEY + " " + _PrivateKeyPath.Trim());
                    if (_WithKeyboardInteractive == true)
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WITHINTERACT + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_TRUE);
                    }
                    else
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_WITHINTERACT + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE);
                    }
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_HOSTFINGERPRINT + " " + _HostFingerPrint.Trim());
                    if (_AskConfigured == true)
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_ASKCONFIG + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_TRUE);
                    }
                    else
                    {
                        swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_OFFSET + ConstantsClass.SC_KMC_OUT_CPROFILE_ASKCONFIG + " " + ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE);
                    }
                    swriter.WriteLine(ConstantsClass.SC_KMC_OUT_CPROFILE_END);
                }

                // Save submit, job and search scripts
                SaveScripts();
            }
            catch (Exception e)
            {
                throw new ApplicationException("Failed to write remote profile ini file (TVMGUISettingsRemoteProfile.SaveToIniFile)", e);
            }
        }

        /// <summary>
        /// Load remote profile from ini file
        /// </summary>
        /// <param name="RemoteProfileIniPath">Path of remote profile ini file</param>
        public void LoadFromIniFile(string RemoteProfileIniPath)
        {
            // Check for valid input
            if (string.IsNullOrWhiteSpace(RemoteProfileIniPath) == true)
            {
                throw new ApplicationException("Empty path (TVMGUISettingsRemoteProfile.LoadFromIniFile)");
            }
            if (File.Exists(RemoteProfileIniPath) == false)
            {
                throw new ApplicationException("Profile ini file does not exist (TVMGUISettingsRemoteProfile.LoadFromIniFile)");
            }

            // Analyze path
            string i_filename = Path.GetFileName(RemoteProfileIniPath);
            string i_dirpath = Path.GetDirectoryName(RemoteProfileIniPath);
            string i_dirname = Path.GetFileName(i_dirpath.TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar));
            
            // Check for valid filename
            if (ConstantsClass.SC_KMC_REMOTEPROFILE_INIFILE.Equals(i_filename) == false)
            {
                throw new ApplicationException("Invalid remote profile filename (TVMGUISettingsRemoteProfile.LoadFromIniFile)");
            }

            // Extract profile ID from directory name
            int t_ID = 0;
            if (i_dirname.Contains(ConstantsClass.SC_KMC_REMOTEPROFILE_DIR) == false)
            {
                throw new ApplicationException("Invalid remote profile folder name (TVMGUISettingsRemoteProfile.LoadFromIniFile)");
            }
            try
            {
                t_ID = int.Parse(i_dirname.Replace(ConstantsClass.SC_KMC_REMOTEPROFILE_DIR, ""));
            }
            catch (Exception e)
            {
                throw new ApplicationException("Failed to convert profile ID (TVMGUISettingsRemoteProfile.LoadFromIniFile)", e);
            }

            // Initialize new data
            string t_Name = "";
            string t_RemoteWorkspace = "";
            string t_RemoteBuildDir = "build";
            string t_HostAdress = "";
            string t_HostPort = "22";
            string t_Username = "";
            string t_WithPassword = "";
            string t_WithPrivateKey = "";
            string t_PrivateKeyPath = "";
            string t_WithKeyboardInteractive = "";
            string t_HostFingerPrint = "";
            string t_AskConfigured = "";
            try
            {
                using (StreamReader sreader = new StreamReader(RemoteProfileIniPath))
                {
                    bool t_hasprofile = false;
                    string t_line = "";
                    while ((t_line = sreader.ReadLine()) != null)
                    {
                        t_line = t_line.Trim();

                        // Load information
                        if (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_START) == true)
                        {
                            t_hasprofile = true;
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_END) == true))
                        {
                            break;
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_NAME) == true))
                        {
                            t_Name = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_NAME.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_WORKSPACE) == true))
                        {
                            t_RemoteWorkspace = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_WORKSPACE.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_BUILDDIR) == true))
                        {
                            t_RemoteBuildDir = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_BUILDDIR.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_HOSTADRESS) == true))
                        {
                            t_HostAdress = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_HOSTADRESS.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_PORT) == true))
                        {
                            t_HostPort = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_PORT.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_USERNAME) == true))
                        {
                            t_Username = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_USERNAME.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_WITHPW) == true))
                        {
                            t_WithPassword = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_WITHPW.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_WITHKEY) == true))
                        {
                            t_WithPrivateKey = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_WITHKEY.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_PRIVATEKEY) == true))
                        {
                            t_PrivateKeyPath = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_PRIVATEKEY.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_WITHINTERACT) == true))
                        {
                            t_WithKeyboardInteractive = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_WITHINTERACT.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_HOSTFINGERPRINT) == true))
                        {
                            t_HostFingerPrint = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_HOSTFINGERPRINT.Length).Trim();
                        }
                        if ((t_hasprofile == true) && (t_line.StartsWith(ConstantsClass.SC_KMC_OUT_CPROFILE_ASKCONFIG) == true))
                        {
                            t_AskConfigured = t_line.Remove(0, ConstantsClass.SC_KMC_OUT_CPROFILE_ASKCONFIG.Length).Trim();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                throw new ApplicationException("Error during reading of remote profile (TVMGUISettingsRemoteProfile.LoadFromIniFile)", e);
            }

            // Parse port and flags
            int tp_HostPort = 0;
            if (int.TryParse(t_HostPort, out tp_HostPort) == false)
            {
                tp_HostPort = 22;
            }
            bool tp_WithPassword = true;
            if (t_WithPassword == ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE)
            {
                tp_WithPassword = false;
            }
            bool tp_WithPrivateKey = true;
            if (t_WithPrivateKey == ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE)
            {
                tp_WithPrivateKey = false;
            }
            bool tp_WithKeyboardInteractive = true;
            if (t_WithKeyboardInteractive == ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE)
            {
                tp_WithKeyboardInteractive = false;
            }
            bool tp_AskConfigured = true;
            if (t_AskConfigured == ConstantsClass.SC_KMC_OUT_CPROFILE_FALSE)
            {
                tp_AskConfigured = false;
            }

            // Check if private key file exists
            if (File.Exists(t_PrivateKeyPath) == false)
            {
                t_PrivateKeyPath = "";
            }

            // Set profile ID (without triggering directory renaming)
            _ID = t_ID;
            Notify("ID");

            // Transfer data
            Name = t_Name;
            RemoteWorkspace = t_RemoteWorkspace;
            RemoteBuildDir = t_RemoteBuildDir;
            HostAdress = t_HostAdress;
            HostPort = tp_HostPort;
            Username = t_Username;
            WithPassword = tp_WithPassword;
            UserPassword = "";
            WithPrivateKey = tp_WithPrivateKey;
            PrivateKeyPath = t_PrivateKeyPath;
            PrivateKeyPassword = "";
            WithKeyboardInteractive = tp_WithKeyboardInteractive;
            HostFingerPrint = t_HostFingerPrint;
            AskConfigured = tp_AskConfigured;

            // Load scripts if they exist
            LoadScripts();
        }

        #endregion Methods
    }
}
