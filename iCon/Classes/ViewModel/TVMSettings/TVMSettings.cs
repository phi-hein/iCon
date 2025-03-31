using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows;
using System.Windows.Media.Media3D;
using System.Windows.Threading;
using Renci.SshNet;
using Renci.SshNet.Common;


namespace iCon_General
{
    /// <summary> 
    /// Holds data for Settings tab 
    /// </summary>
    public class TVMSettings : ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMSettings(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _CurrentJob = new TVMSettingsJob(ViewModel);
            _JobList = new ObservableCollection<TVMSettingsJob>();
            _SelectedJob = null;
            _CalcType = true;
            _BaseDirectory = "";
            _JobNamePrefix = "Job";
            _DublicateCount = 1;
        }

        #region Properties

        protected TVMSettingsJob _CurrentJob;
        /// <summary> 
        /// Currently edited simulation job 
        /// </summary>
        public TVMSettingsJob CurrentJob
        {
            get
            {
                return _CurrentJob;
            }
            set
            {
                if (value != _CurrentJob)
                {
                    _CurrentJob = value;
                    Notify("CurrentJob");
                }
            }
        }

        protected ObservableCollection<TVMSettingsJob> _JobList;
        /// <summary> 
        /// List of simulation jobs (only validated jobs)
        /// </summary>
        public ObservableCollection<TVMSettingsJob> JobList
        {
            get
            {
                return _JobList;
            }
            set
            {
                if (value != _JobList)
                {
                    _JobList = value;
                    Notify("JobList");
                }
            }
        }

        protected TVMSettingsJob _SelectedJob;
        /// <summary> 
        /// Currently selected simulation job in job list
        /// </summary>
        public TVMSettingsJob SelectedJob
        {
            get
            {
                return _SelectedJob;
            }
            set
            {
                if (value != _SelectedJob)
                {
                    _SelectedJob = value;
                    Notify("SelectedJob");
                    Notify("IsJobSelected");
                }
            }
        }

        /// <summary> 
        /// Flag, true = a job in the job list is selected
        /// </summary>
        public bool IsJobSelected
        {
            get
            {
                return (_SelectedJob != null);
            }
        }

        /// <summary>
        /// Flag, true = there are submittable jobs in the job list
        /// </summary>
        public bool HasValidJobs
        {
            get
            {
                return (_JobList.Count != 0);
            }
        }

        protected bool _CalcType;
        /// <summary> 
        /// Calculation type, true = local calculation, false = submission to cluster
        /// </summary>
        public bool CalcType
        {
            get
            {
                return _CalcType;
            }
            set
            {
                if (value != _CalcType)
                {
                    _CalcType = value;
                    Notify("CalcType");
                }
            }
        }

        protected string _BaseDirectory;
        /// <summary> 
        /// Base directory for job submission (absolute or relative to workspace directory)
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid base directory.")]
        [StringLength(250, MinimumLength = 2, ErrorMessage = "Base directory has to contain 2 - 250 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_/~\\:\.\-\^\(\)\s]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _~\./:-^() and space.")]
        public string BaseDirectory
        {
            get
            {
                return _BaseDirectory;
            }
            set
            {
                ValidateNotify("BaseDirectory", value, ref _BaseDirectory);
            }
        }

        protected string _JobNamePrefix;
        /// <summary> 
        /// Prefix of the job for filename and cluster job name (will be combined with job number)
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid jobname prefix.")]
        [StringLength(15, MinimumLength = 2, ErrorMessage = "Job name has to contain 2 - 15 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\-]*$", ErrorMessage = @"Allowed characters: a-z, A-Z, 0-9, _ and -.")]
        public string JobNamePrefix
        {
            get
            {
                return _JobNamePrefix;
            }
            set
            {
                ValidateNotify("JobNamePrefix", value, ref _JobNamePrefix);
            }
        }

        public int _DublicateCount;
        /// <summary> 
        /// Number of dublicates to be created when dublicating jobs 
        /// </summary>
        [IntRange(1, true, 100, true, ErrorMessage = "Number of dublicates has to be in the interval [1, 100]")]
        public int DublicateCount
        {
            get
            {
                return _DublicateCount;
            }
            set
            {
                ValidateNotify("DublicateCount", value, ref _DublicateCount);
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Add CurrentJob to job list (caution: only if CurrentJob is validated before)
        /// </summary>
        public void AddJob()
        {
            // Check for too many jobs in list
            if (_JobList.Count >= ConstantsClass.MAX_JOB_COUNT)
            {
                MessageBox.Show("Maximum number of jobs (" + ConstantsClass.MAX_JOB_COUNT.ToString() + ") reached.", "Information", MessageBoxButton.OK);
                return;
            }

            // Calculate and set new job ID (next highest, minimum = 1)
            int t_ID = 0;
            if (_JobList.Count > 0)
            {
                for (int i = 0; i < _JobList.Count; i++)
                {
                    if (_JobList[i].ID > t_ID) t_ID = _JobList[i].ID;
                }
            }
            t_ID += 1;

            // Copy data (not the same object in CurrentJob and in JobList), set ID and add job
            TVMSettingsJob t_Job = _CurrentJob.GetCopy();
            t_Job.ID = t_ID;
            JobList.Add(t_Job);

            // Select new job
            SelectedJob = t_Job;
            
            // Update HasValidJobs
            Notify("HasValidJobs");
        }

        /// <summary>
        /// Overwrite selected job in job list with CurrentJob (caution: only if CurrentJob is validated before)
        /// </summary>
        public void ChangeSelectedJob()
        {
            // Check if job is selected
            if (_SelectedJob == null) return;

            // Find selected job in collection
            int t_index = _JobList.IndexOf(_SelectedJob);
            if (t_index == -1) return;

            // Copy data (not the same object in CurrentJob and in JobList), set ID and change job
            TVMSettingsJob t_Job = _CurrentJob.GetCopy();
            t_Job.ID = _JobList[t_index].ID;
            JobList[t_index] = t_Job;
        }

        /// <summary>
        /// Add copies of the selected job to the job list
        /// </summary>
        public void DublicateJob()
        {
            // Check if job is selected
            if (_SelectedJob == null) return;

            // Validate DublicateCount property
            if (ValidateProperty("DublicateCount", _DublicateCount) == false) return;

            // Check for too many jobs in list
            if (_JobList.Count + _DublicateCount >= ConstantsClass.MAX_JOB_COUNT)
            {
                MessageBox.Show("Exceeds maximum number of jobs (" + ConstantsClass.MAX_JOB_COUNT.ToString() + ").", "Information", MessageBoxButton.OK);
                return;
            }

            // Calculate and set lowest new job ID (next highest, minimum = 1)
            int t_ID = 0;
            if (_JobList.Count > 0)
            {
                for (int i = 0; i < _JobList.Count; i++)
                {
                    if (_JobList[i].ID > t_ID) t_ID = _JobList[i].ID;
                }
            }
            t_ID += 1;

            // Add copies of the selected job
            TVMSettingsJob t_Job = null;
            for (int i = 0; i < _DublicateCount; i++)
            {
                t_Job = _SelectedJob.GetCopy();
                t_Job.ID = t_ID + i;
                JobList.Add(t_Job);
            }

            // Select last new job
            SelectedJob = t_Job;

            // Update HasValidJobs
            Notify("HasValidJobs");
        }

        /// <summary>
        /// Load selected job from job list to CurrentJob
        /// </summary>
        public void LoadSelectedJob()
        {
            // Check if job is selected
            if (_SelectedJob == null) return;

            // Copy data (not the same object in CurrentJob and in JobList)
            CurrentJob = _SelectedJob.GetCopy();
        }

        /// <summary>
        /// Delete selected job from job list
        /// </summary>
        public void DeleteSelectedJob()
        {
            // Check if job is selected
            if (_SelectedJob == null) return;

            // Delete job
            JobList.Remove(_SelectedJob);
            SelectedJob = null;

            // Update HasValidJobs
            Notify("HasValidJobs");
        }

        /// <summary>
        /// Clear all settings in job list
        /// </summary>
        public void ClearJobList()
        {
            // Check if job list is empty
            if (_JobList.Count == 0) return;

            // Clear job list
            SelectedJob = null;
            JobList.Clear();

            // Update HasValidJobs
            Notify("HasValidJobs");
        }

        /// <summary>
        /// Validate this object and all sub-objects except the CurrentJob
        /// </summary>
        public bool ValidateForJobSubmit()
        {
            if (ValidateProperty("BaseDirectory", _BaseDirectory) == false) return false;
            if (ValidateProperty("JobNamePrefix", _JobNamePrefix) == false) return false;
            if (ValidateObject() == false) return false;
            if (_JobList.Count > 0)
            {
                for (int i = 0; i < _JobList.Count; i++)
                {
                    if (_JobList[i].ValidateFullObject() == false) return false;
                }
            }
            return true;
        }

        #endregion Methods

        #region Synchronization Methods

        /// <summary>
        /// Start simulations for the jobs in the job list (use only on a background worker thread)
        /// </summary>
        public void SubmitJobs(TMCJobWrapper MCDLL, TVMGUISettings ExtendedSettings, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Validating job list ... ");

            // Check that there are jobs
            if (_JobList.Count < 1)
            {
                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid number of jobs\n(at least one job required)\n",
                    ConstantsClass.KMCERR_INVALID_INPUT, false);
                return;
            }

            // Check if any IDs are dublicates
            List<int> jobIDs = new List<int>();
            for (int i = 0; i < _JobList.Count; i++)
            {
                if (jobIDs.Contains(_JobList[i].ID) == true)
                {
                    e.Result = new BWorkerResultMessage("Invalid Input", "Job ID " + _JobList[i].ID.ToString() + 
                        " has a dublicate\n(every job has to have a unique ID)\n", ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                jobIDs.Add(_JobList[i].ID);
            }

            // Call local or remote submission routine
            if (_CalcType == true)
            {
                Console.WriteLine("Job Submission: local");
                Console.WriteLine("Number of jobs: " + _JobList.Count.ToString());
                SubmitLocalJobs(MCDLL, ExtendedSettings, BWorker, e);
            }
            else
            {
                Console.WriteLine("Job Submission: cluster");
                Console.WriteLine("Number of jobs: " + _JobList.Count.ToString());
                SubmitRemoteJobs(MCDLL, ExtendedSettings, BWorker, e);
            }
        }

        #endregion Synchronization Methods

        #region Submit Methods

        /// <summary>
        /// Start local simulations for the jobs in the job list (use only on a background worker thread)
        /// </summary>
        protected void SubmitLocalJobs(TMCJobWrapper MCDLL, TVMGUISettings ExtendedSettings, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            e.Result = new BWorkerResultMessage("Implementation Error", "Local job submission not implemented\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);

            /*
            // Path Convention:
            // Local, BaseDirectory = abs: BaseDirectory\Job_XXXX\JobNamePrefix_XXXX.kmc

            // Local, BaseDirectory = rel, Workspace = abs: LocalWorkspace\BaseDirectory\Job_XXXX\JobNamePrefix_XXXX.kmc

            // Local, BaseDirectory = rel, Workspace = rel: AppDirectory\Workspace\LocalWorkspace\BaseDirectory\Job_XXXX\JobNamePrefix_XXXX.kmc

            bool IfPreTotalAbsolute = false;
            string PreTotalBasePath = "";
            try
            {
                PreTotalBasePath = Path.Combine(ExtendedSettings.LocalWorkspace.Trim(), _BaseDirectory.Trim()).Trim();
                IfPreTotalAbsolute = Path.IsPathRooted(PreTotalBasePath);
            }
            catch (Exception ex)
            {
                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid submit paths\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                return;
            }
            */
        }

        /// <summary>
        /// Start remote simulations for the jobs in the job list (use only on a background worker thread)
        /// </summary>
        protected void SubmitRemoteJobs(TMCJobWrapper MCDLL, TVMGUISettings ExtendedSettings, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            BWorker.ReportProgress(5, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(5, "Loading authentication data ... ");

            // Create desired authentication method lists
            List<AuthenticationMethod> ssh_auth_methods = new List<AuthenticationMethod>();
            List<AuthenticationMethod> sftp_auth_methods = new List<AuthenticationMethod>();

            // Create authentication methods
            if (ExtendedSettings.SelectedRemoteProfile.WithPrivateKey == true)
            {
                Console.WriteLine("Authentication method: private key");

                // Check if private key file exists
                if (File.Exists(ExtendedSettings.SelectedRemoteProfile.PrivateKeyPath.Trim()) == false)
                {
                    Console.WriteLine("Error: Private key file (" + ExtendedSettings.SelectedRemoteProfile.PrivateKeyPath.Trim() + ") is missing.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Private key file does not exist\n(see console for details)\n",
                    ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }

                // Create keyfiles
                PrivateKeyFile ssh_keyfile = null;
                PrivateKeyFile sftp_keyfile = null;
                try
                {
                    ssh_keyfile = new PrivateKeyFile(ExtendedSettings.SelectedRemoteProfile.PrivateKeyPath.Trim(),
                        ExtendedSettings.SelectedRemoteProfile.PrivateKeyPassword.Trim());
                    sftp_keyfile = new PrivateKeyFile(ExtendedSettings.SelectedRemoteProfile.PrivateKeyPath.Trim(),
                        ExtendedSettings.SelectedRemoteProfile.PrivateKeyPassword.Trim());
                }
                catch (SshException ex)
                {
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file\n(see console for details)\n.",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (NotSupportedException ex)
                {
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: Private key file path has zero length, contains only white space or contains one or more invalid characters.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (PathTooLongException)
                {
                    Console.WriteLine("Error: Private key file path exceeds the system-defined maximum path length.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (DirectoryNotFoundException)
                {
                    Console.WriteLine("Error: Directory of the private key file does not exist.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (UnauthorizedAccessException)
                {
                    Console.WriteLine("Error: Private key file access permissions prevent loading of the file.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (FileNotFoundException)
                {
                    Console.WriteLine("Error: Private key file not found.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                catch (IOException)
                {
                    Console.WriteLine("Error: IO-error when opening private key file.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }

                // Add key file authentication methods
                try
                {
                    ssh_auth_methods.Add(new PrivateKeyAuthenticationMethod(ExtendedSettings.SelectedRemoteProfile.Username.Trim(), ssh_keyfile));
                    sftp_auth_methods.Add(new PrivateKeyAuthenticationMethod(ExtendedSettings.SelectedRemoteProfile.Username.Trim(), sftp_keyfile));
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: User name (" + ExtendedSettings.SelectedRemoteProfile.Username.Trim() + ") has zero length or contains only white space.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid user name\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
            }
            else
            {
                Console.WriteLine("Authentication method: password");

                // Add password authentication methods
                try
                {
                    ssh_auth_methods.Add(new PasswordAuthenticationMethod(ExtendedSettings.SelectedRemoteProfile.Username.Trim(), 
                        ExtendedSettings.SelectedRemoteProfile.UserPassword.Trim()));
                    sftp_auth_methods.Add(new PasswordAuthenticationMethod(ExtendedSettings.SelectedRemoteProfile.Username.Trim(), 
                        ExtendedSettings.SelectedRemoteProfile.UserPassword.Trim()));
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: User name (" + ExtendedSettings.SelectedRemoteProfile.Username.Trim() + ") has zero length or contains only white space.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid user name\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
            }

            // Create connection info objects
            ConnectionInfo ssh_coninfo = null;
            ConnectionInfo sftp_coninfo = null;
            try
            {
                ssh_coninfo = new ConnectionInfo(ExtendedSettings.SelectedRemoteProfile.HostAdress.Trim(), ExtendedSettings.SelectedRemoteProfile.HostPort, 
                    ExtendedSettings.SelectedRemoteProfile.Username.Trim(), ssh_auth_methods.ToArray());
                sftp_coninfo = new ConnectionInfo(ExtendedSettings.SelectedRemoteProfile.HostAdress.Trim(), ExtendedSettings.SelectedRemoteProfile.HostPort,
                    ExtendedSettings.SelectedRemoteProfile.Username.Trim(), sftp_auth_methods.ToArray());
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("Error: Invalid connection setting: " + ex.Message);
                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid connection settings\n(see console for details)\n",
                    ConstantsClass.KMCERR_INVALID_INPUT, false);
                return;
            }

            BWorker.ReportProgress(10, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(10, "Connecting to cluster ... ");

            // Create client objects
            using (SshClient sshcl = new SshClient(ssh_coninfo))
            using (SftpClient sftpcl = new SftpClient(sftp_coninfo))
            {
                // Set keep alive intervals
                sshcl.KeepAliveInterval = new TimeSpan(0, 0, 10);
                sftpcl.KeepAliveInterval = new TimeSpan(0, 0, 10);

                // Host key validation (only for first connect)
                sshcl.HostKeyReceived += delegate(object sender, HostKeyEventArgs exp)
                {
                    // The code below works but CanTrust = false throws an exception that causes hanging of the implicit sshcl.Dispose() 
                    // method at the end of the using block

                    // Check for fingerprint
                    if (exp.FingerPrint.Length == 0)
                    {
                        exp.CanTrust = false;
                        return;
                    }

                    // Create host key fingerprint string
                    StringBuilder str_builder = new StringBuilder();
                    str_builder.AppendFormat("{0} {1} {2:x}", exp.HostKeyName, exp.KeyLength.ToString(), exp.FingerPrint[0]);
                    if (exp.FingerPrint.Length > 1)
                    {
                        for (int i = 1; i < exp.FingerPrint.Length; i++)
                        {
                            str_builder.AppendFormat(":{0:X2}", exp.FingerPrint[i]);
                        }
                    }
                    Console.WriteLine("Host key fingerprint: " + str_builder.ToString());

                    // TODO: check trusted key here

                    /* Doing this will cause a connection timeout (but it works)
                    bool if_trust = (bool)Application.Current.Dispatcher.Invoke(new Func<string, bool>((string fingerprint) => {
                        MessageBoxResult res = MessageBox.Show("Host ssh key fingerprint is: " + fingerprint + "\nIs this the desired host?", "Host validation", 
                            MessageBoxButton.YesNo, MessageBoxImage.Question);
                        if (res == MessageBoxResult.Yes)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }), DispatcherPriority.Normal, str_builder.ToString());
                    */

                    // TODO: save fingerprint as trusted if if_trust == true

                    exp.CanTrust = true;
                };

                // Connect to cluster
                try
                {
                    sshcl.Connect();
                    if (sshcl.IsConnected == false)
                    {
                        e.Result = new BWorkerResultMessage("Invalid Input", "Cannot connect to specified server\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    }
                    sftpcl.Connect();
                    if (sftpcl.IsConnected == false)
                    {
                        e.Result = new BWorkerResultMessage("Invalid Input", "Cannot connect to specified server\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    }
                }
                catch (SshException ex)
                {
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("Connection Error", "Cannot connect to specified server\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                Console.WriteLine("Cluster connection established.");

                // Path Convention:
                // Remote, BaseDirectory = abs: BaseDirectory/Job_XXXX/JobNamePrefix_XXXX.kmc
                // Remote, BaseDirectory = rel, Workspace = abs: RemoteWorkspace/BaseDirectory/Job_XXXX/JobNamePrefix_XXXX.kmc
                // Remote, BaseDirectory = rel, Workspace = rel: UserHome/RemoteWorkspace/BaseDirectory/Job_XXXX/JobNamePrefix_XXXX.kmc

                // Create complete base path and a list of all sub-directories which should be created eventually (without user home directory)
                string totalBasePath = "";
                List<string> createDirs = null;
                try
                {
                    totalBasePath = CombineUnixPaths(ExtendedSettings.SelectedRemoteProfile.RemoteWorkspace.Trim(), _BaseDirectory.Trim()).Trim();
                    createDirs = GetSubDirectories(totalBasePath);
                    for (int i = 0; i < createDirs.Count; i++)
                    {
                        createDirs[i] = CombineUnixPaths(sftpcl.WorkingDirectory, createDirs[i]);
                    }
                    totalBasePath = createDirs[createDirs.Count - 1];
                }
                catch (Exception)
                {
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid submit path\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                Console.WriteLine("Project base path: " + totalBasePath);

                // Create project base directory
                Console.Write("Creating project base directory ... ");
                try
                {
                    for (int i = 0; i < createDirs.Count; i++)
                    {
                        if (sftpcl.Exists(createDirs[i]) == false)
                        {
                            sftpcl.CreateDirectory(createDirs[i]);
                        }
                    }
                }
                catch (SshException ex)
                {
                    Console.WriteLine();
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("SFTP Error", "Creating project directory failed\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                Console.WriteLine("OK.");

                // Create remote paths for executables and scripts (placed in base directory)
                string simExePath = CombineUnixPaths(totalBasePath, ExtendedSettings.SelectedRemoteProfile.GetSelectedSimExecutableName());
                string searchExePath = CombineUnixPaths(totalBasePath, ExtendedSettings.SelectedRemoteProfile.GetSelectedSearchExecutableName());
                string submitScriptPath = CombineUnixPaths(totalBasePath, ConstantsClass.SC_KMC_SUBMITSCRIPT);
                string jobScriptPath = CombineUnixPaths(totalBasePath, ConstantsClass.SC_KMC_JOBSCRIPT);

                // Transfer executables and scripts to base directory
                Console.Write("Uploading executables and scripts ... ");
                try
                {
                    // Simulation executable
                    using (FileStream exe_stream = File.OpenRead(ExtendedSettings.SelectedRemoteProfile.GetSelectedSimExecutablePath()))
                    {
                        sftpcl.UploadFile(exe_stream, simExePath, true);
                        sftpcl.ChangePermissions(simExePath, 744);
                    }

                    // Searcher executable
                    using (FileStream exe_stream = File.OpenRead(ExtendedSettings.SelectedRemoteProfile.GetSelectedSearchExecutablePath()))
                    {
                        sftpcl.UploadFile(exe_stream, searchExePath, true);
                        sftpcl.ChangePermissions(searchExePath, 744);
                    }

                    // Submit script
                    using (FileStream subscr_stream = File.OpenRead(
                        ExtendedSettings.SelectedRemoteProfile.GetRemoteProfileFilePath(ConstantsClass.SC_KMC_SUBMITSCRIPT)))
                    {
                        sftpcl.UploadFile(subscr_stream, submitScriptPath, true);
                        sftpcl.ChangePermissions(submitScriptPath, 744);
                    }

                    // Job script
                    using (FileStream jobscr_stream = File.OpenRead(
                        ExtendedSettings.SelectedRemoteProfile.GetRemoteProfileFilePath(ConstantsClass.SC_KMC_JOBSCRIPT)))
                    {
                        sftpcl.UploadFile(jobscr_stream, jobScriptPath, true);
                        sftpcl.ChangePermissions(jobScriptPath, 744);
                    }
                }
                catch (SshException ex)
                {
                    Console.WriteLine();
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("SFTP Error", "Executable and/or script transmission failed\n(see console for details)\n.",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                }
                Console.WriteLine("OK.");

                // Submit all jobs
                BWorker.ReportProgress(15, "OK\n");
                double jobPercInc = 80.0 / Convert.ToDouble(_JobList.Count);
                double subProgress = 15.0;
                for (int i = 0; i < _JobList.Count; i++)
                {
                    System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
                    if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
                    BWorker.ReportProgress(Convert.ToInt32(Math.Floor(subProgress)), "Submitting job " + (i + 1).ToString() + " (JobID: " + _JobList[i].ID.ToString() + ") ... ");
                    Console.WriteLine("Job " + (i + 1).ToString() + " of " + _JobList.Count.ToString() + " (ID: " + _JobList[i].ID.ToString() + "):");

                    // Load job settings
                    Console.Write("Loading job ... ");
                    if (_JobList[i].ApplyData(MCDLL, BWorker, e, false) == false) return;
                    Console.WriteLine("OK.");

                    // Get input file as string
                    Console.Write("Writing job input file ... ");
                    string inp_file_str = "";
                    ErrorCode = MCDLL.SaveToString(ref inp_file_str);
                    switch (ErrorCode)
                    {
                        case ConstantsClass.KMCERR_OK:
                            break;
                        default:
                            throw new ApplicationException("Cannot save job data to string (TVMSettings.SubmitJobs, ErrorCode: " + ErrorCode.ToString() + ")");
                    }
                    Console.WriteLine("OK.");

                    // Create job folder
                    string jobDirectory = CombineUnixPaths(totalBasePath, "Job_" + _JobList[i].ID.ToString("0000"));
                    Console.Write("Creating job folder (" + jobDirectory + ") ... ");
                    try
                    {
                        if (sftpcl.Exists(jobDirectory) == false)
                        {
                            sftpcl.CreateDirectory(jobDirectory);
                        }
                    }
                    catch (SshException ex)
                    {
                        Console.WriteLine();
                        Console.WriteLine("Error: " + ex.Message);
                        e.Result = new BWorkerResultMessage("SFTP Error", "Creating job directory failed\n(see console for details)\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    }
                    Console.WriteLine("OK.");

                    // Create file paths and job name
                    string jobName = _JobNamePrefix + "_" + _JobList[i].ID.ToString("0000");
                    string jobInputPath = CombineUnixPaths(jobDirectory, jobName + ".kmc");
                    string jobLogPath = CombineUnixPaths(jobDirectory, jobName + ".log");

                    // Transfer input file
                    Console.Write("Uploading job file ... ");
                    try
                    {
                        // Job input file
                        sftpcl.WriteAllText(jobInputPath, inp_file_str);
                        sftpcl.ChangePermissions(jobInputPath, 744);
                    }
                    catch (SshException ex)
                    {
                        Console.WriteLine();
                        Console.WriteLine("Error: " + ex.Message);
                        e.Result = new BWorkerResultMessage("SFTP Error", "Job file transmission failed\n(see console for details)\n.",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    }
                    Console.WriteLine("OK.");

                    // Submit job to queue
                    try
                    {
                        SshCommand submit_cmd = sshcl.CreateCommand("sh \"" + submitScriptPath + "\" \"" + jobScriptPath + "\" \"" + simExePath + "\" \"" + 
                            jobDirectory + "\" \"" + jobName + "\" \"" + jobInputPath + "\" \"" + jobLogPath + "\"");
                        string submit_result = submit_cmd.Execute();
                        Console.WriteLine(submit_result);
                    }
                    catch (SshException ex)
                    {
                        Console.WriteLine("Error: " + ex.Message);
                        e.Result = new BWorkerResultMessage("SSH Error", "Job submission failed\n(see console for details)\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    }

                    subProgress += jobPercInc;
                    BWorker.ReportProgress(Convert.ToInt32(Math.Floor(subProgress)), "OK\n");
                }
                Console.WriteLine("All jobs submitted.");

                sshcl.Disconnect();
                sftpcl.Disconnect();        // This will throw a silent, first-chance SshConnectionException (which is recorded in the ErrorLog)
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(95, "Loading ... ");
            e.Result = new TMCJob(MCDLL);
            BWorker.ReportProgress(100, "OK\n");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        #endregion Submit Methods

        #region Helper Methods

        /// <summary>
        /// Check if the unix path is an absolute path
        /// </summary>
        protected bool IsUnixPathRooted(string i_path)
        {
            if (string.IsNullOrWhiteSpace(i_path) == true)
            {
                return false;
            }

            string t_path = i_path.Trim();
            if (t_path[0] == '/')
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Removes slash(es) at the end of a path
        /// </summary>
        protected string RemoveEndSlash(string i_path)
        {
            if (string.IsNullOrWhiteSpace(i_path) == true)
            {
                return "";
            }

            string t_path = i_path.Trim();
            while ((t_path.Length > 0) && (t_path[t_path.Length - 1] == '/'))
            {
                t_path = t_path.Remove(t_path.Length - 1);
            }

            return t_path;
        }

        /// <summary>
        /// Combine two unix paths (if the second one is an absolute path, then this returns the second path), the result has no directory delimiter at the end
        /// </summary>
        protected string CombineUnixPaths(string i_path1, string i_path2)
        {
            string t_path1 = RemoveEndSlash(i_path1);
            string t_path2 = RemoveEndSlash(i_path2);

            if (string.IsNullOrWhiteSpace(t_path1) == true)
            {
                return t_path2;
            }

            if (string.IsNullOrWhiteSpace(i_path2) == true)
            {
                return t_path1;
            }

            if (IsUnixPathRooted(t_path2) == true)
            {
                return t_path2;
            }

            return t_path1 + "/" + t_path2;
        }

        /// <summary>
        /// Create a list of all directories that are contained in a path
        /// </summary>
        protected List<string> GetSubDirectories(string i_path)
        {
            string t_path = i_path.Trim();
            bool is_absolute = IsUnixPathRooted(t_path);

            char[] delim = new char[] {'/'};
            string[] dirs = t_path.Split(delim, StringSplitOptions.RemoveEmptyEntries);

            List<string> t_result = new List<string>();
            if (dirs.Length == 0)
            {
                t_result.Add("");
            }
            else
            {
                string t_concatstr = "";
                if (is_absolute == true)
                {
                    t_concatstr += "/";
                }
                for (int i = 0; i < dirs.Length; i++)
                {
                    if (string.IsNullOrWhiteSpace(dirs[i]) == true) continue;
                    string t_adddir = t_concatstr + dirs[i];
                    t_result.Add(t_adddir);
                    t_concatstr += dirs[i] + "/";
                }
            }
            if (t_result.Count == 0)
            {
                t_result.Add("");
            }

            return t_result;
        }

        #endregion Helper Methods
    }
}
