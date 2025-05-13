using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Net.Sockets;
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
        /// Calculation type, true = submission to cluster, false = local calculation
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
                ValidateNotify("BaseDirectory", value.Trim(), ref _BaseDirectory);
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
                ValidateNotify("JobNamePrefix", value.Trim(), ref _JobNamePrefix);
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
            if (_JobList.Count >= Constants.MAX_JOB_COUNT)
            {
                MessageBox.Show("Maximum number of jobs (" + Constants.MAX_JOB_COUNT.ToString() + ") reached.", "Information", MessageBoxButton.OK);
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
            if (_JobList.Count + _DublicateCount >= Constants.MAX_JOB_COUNT)
            {
                MessageBox.Show("Exceeds maximum number of jobs (" + Constants.MAX_JOB_COUNT.ToString() + ").", "Information", MessageBoxButton.OK);
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
                    Constants.KMCERR_INVALID_INPUT, false);
                return;
            }

            // Check if any IDs are dublicates
            List<int> jobIDs = new List<int>();
            for (int i = 0; i < _JobList.Count; i++)
            {
                if (jobIDs.Contains(_JobList[i].ID) == true)
                {
                    e.Result = new BWorkerResultMessage("Invalid Input", "Job ID " + _JobList[i].ID.ToString() + 
                        " has a dublicate\n(every job has to have a unique ID)\n", Constants.KMCERR_INVALID_INPUT, false);
                    return;
                }
                jobIDs.Add(_JobList[i].ID);
            }

            // Call local or remote submission routine
            if (_CalcType == false)
            {
                Console.WriteLine("Job Submission: local");
                Console.WriteLine("Number of jobs: " + _JobList.Count.ToString());
                SubmitLocalJobs(MCDLL, ExtendedSettings, BWorker, e);
            }
            else
            {
                Console.WriteLine("Job Submission: cluster");
                Console.WriteLine("Number of jobs: " + _JobList.Count.ToString());
                SubmitRemoteJobs(MCDLL, ExtendedSettings.SelectedRemoteProfile, BWorker, e);
            }
        }

        #endregion Synchronization Methods

        #region Submit Methods

        /// <summary>
        /// Constructs an exemplary directory path for the submission 
        /// (according to the conventions in the submit methods, just for displaying in GUI)
        /// </summary>
        public static string GetFullSubmitPath(bool IsRemoteCalc, string BaseDir, string JobPfx, string RemoteWsp, string LocalWsp)
        {
            if (IsRemoteCalc)
            {
                return RemotePaths.Combine(RemotePaths.Combine("~", RemoteWsp, BaseDir), JobPfx + "_0001");
            }
            else
            {
                return Path.Combine(TVMGUISettings.GetLocalWorkspacePath(LocalWsp), BaseDir);
            }
        }

        /// <summary>
        /// Start local simulations for the jobs in the job list (use only on a background worker thread)
        /// </summary>
        protected void SubmitLocalJobs(TMCJobWrapper MCDLL, TVMGUISettings ExtendedSettings, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            e.Result = new BWorkerResultMessage("Implementation Error", "Local job submission not implemented\n",
                            Constants.KMCERR_INVALID_INPUT, false);

            /*
            // Path Convention:
            // Local, BaseDirectory = abs: BaseDirectory\Job_XXXX\JobNamePrefix_XXXX.kmc

            // Local, BaseDirectory = rel, Workspace = abs: LocalWorkspace\BaseDirectory\Job_XXXX\JobNamePrefix_XXXX.kmc

            // Local, BaseDirectory = rel, Workspace = rel: (MyDocuments)\iCon-Workspace\LocalWorkspace\BaseDirectory\Job_XXXX\JobNamePrefix_XXXX.kmc

            string totalBasePath = Path.Combine(ExtendedSettings.GetLocalWorkspacePath(), _BaseDirectory);

            // Do not overwrite existing jobs
            */
        }

        /// <summary>
        /// Start remote simulations for the jobs in the job list (use only on a background worker thread)
        /// </summary>
        protected void SubmitRemoteJobs(TMCJobWrapper MCDLL, TVMGUISettingsRemoteProfile RemoteProfile, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            int ErrorCode = Constants.KMCERR_OK;
            BWorker.ReportProgress(5, "OK\n");
            System.Threading.Thread.Sleep(Constants.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(5, "Loading authentication data ... ");

            // Create remote cluster object
            using RemoteCluster cluster = new RemoteCluster();
            
            // Initialize the connection settings
            if (!cluster.Initialize(e, RemoteProfile)) return;

            BWorker.ReportProgress(7, "OK\n");
            System.Threading.Thread.Sleep(Constants.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(7, "Connecting to cluster ... ");

            // Connect to cluster
            if (!cluster.Connect(e)) return;
            Console.WriteLine("Cluster connection established.");

            BWorker.ReportProgress(10, "OK\n");
            System.Threading.Thread.Sleep(Constants.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(10, "Preparing executables and scripts ... ");

            // Path Convention:
            // Remote, BaseDirectory = abs: BaseDirectory/Job_XXXX/JobNamePrefix_XXXX.kmc
            // Remote, BaseDirectory = rel, Workspace = abs: RemoteWorkspace/BaseDirectory/Job_XXXX/JobNamePrefix_XXXX.kmc
            // Remote, BaseDirectory = rel, Workspace = rel: UserHome/RemoteWorkspace/BaseDirectory/Job_XXXX/JobNamePrefix_XXXX.kmc

            // Create full paths for the job base directory and the build directory
            string totalBasePath = RemotePaths.Combine(cluster.HomeDirectory,
                    RemoteProfile.RemoteWorkspace, _BaseDirectory);
            string totalBuildPath = RemotePaths.Combine(cluster.HomeDirectory,
                    RemoteProfile.RemoteWorkspace, RemoteProfile.RemoteBuildDir);

            // Create project base directory
            Console.WriteLine("Project base path: " + totalBasePath);
            Console.Write("Creating project base directory ... ");
            if (!cluster.CreateDirectory(e, totalBasePath)) return;
            Console.WriteLine("OK.");

            // Create remote paths for executables
            string simExePath = RemotePaths.Combine(totalBasePath, Constants.SC_KMC_REMOTESIMEXE);
            string searchExePath = RemotePaths.Combine(totalBasePath, Constants.SC_KMC_REMOTESEARCHEXE);
            string simExeBuildPath = RemotePaths.Combine(totalBuildPath, Constants.SC_KMC_REMOTESIMEXE);
            string searchExeBuildPath = RemotePaths.Combine(totalBuildPath, Constants.SC_KMC_REMOTESEARCHEXE);

            // Check whether remote executables are present and up-to-date
            Console.WriteLine("Remote build path: " + totalBuildPath);
            Console.Write("Check remote executables ... ");
            bool buildRequired = true;

            // The try-catch clause is just for the Exists and ExecuteCommandSilent requests
            try
            {
                // Check job base folder first
                if (cluster.Exists(simExePath) && cluster.Exists(searchExePath))
                {
                    Version sim_version = new Version(
                        cluster.ExecuteCommandSilent(e, "\"" + simExePath + "\" -version"));

                    Version search_version = new Version(
                        cluster.ExecuteCommandSilent(e, "\"" + searchExePath + "\" -version"));

                    Version req_version = Assembly.GetExecutingAssembly().GetName().Version;
                    if ((sim_version >= req_version) && (search_version >= req_version))
                    {
                        buildRequired = false;
                        Console.WriteLine("OK.");
                    }
                }

                // Check build folder (and copy eventually)
                if (buildRequired == true)
                {
                    if (cluster.Exists(simExeBuildPath) && cluster.Exists(searchExeBuildPath))
                    {
                        Version sim_version = new Version(
                        cluster.ExecuteCommandSilent(e, "\"" + simExeBuildPath + "\" -version"));

                        Version search_version = new Version(
                            cluster.ExecuteCommandSilent(e, "\"" + searchExeBuildPath + "\" -version"));

                        Version req_version = Assembly.GetExecutingAssembly().GetName().Version;
                        if ((sim_version >= req_version) && (search_version >= req_version))
                        {
                            if (!cluster.ExecuteCommand(e, "cp -p \"" + simExeBuildPath + "\" \"" + simExePath + "\"")) return;
                            if (!cluster.ExecuteCommand(e, "cp -p \"" + searchExeBuildPath + "\" \"" + searchExePath + "\"")) return;

                            buildRequired = false;
                            Console.WriteLine("OK.");
                        }
                        else Console.WriteLine("Old version.");
                    }
                    else Console.WriteLine("Not found.");
                }
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                e.Result = new BWorkerResultMessage("SSH Error", "SSH connection is invalid\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return;
            }
            catch (SftpPermissionDeniedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                e.Result = new BWorkerResultMessage("SFTP Error", "SFTP permission denied\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return;
            }
            catch (SshException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                e.Result = new BWorkerResultMessage("SSH Error", "An SSH error occured\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return;
            }
            catch (ArgumentNullException ex)
            {
                if (e.Result == null)
                {
                    Console.WriteLine("");
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("SSH Error", "Failed to retrieve version number\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                }
                return;
            }

            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }

            // Build remote executables and copy to job base directory
            if (buildRequired == true)
            {
                Console.Write("Creating remote build directory ... ");
                if (!cluster.CreateDirectory(e, totalBuildPath)) return;
                Console.WriteLine("OK.");

                Console.Write("Uploading source files ... ");
                string sourceArchivePath = RemotePaths.Combine(totalBuildPath, Constants.SC_KMC_SOURCEARCHIVE);
                string buildScriptPath = RemotePaths.Combine(totalBuildPath, Constants.SC_KMC_BUILDSCRIPT);

                // Source archive
                if (!cluster.UploadFile(e, Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                    Constants.SC_KMC_SOURCEARCHIVE), sourceArchivePath, 644)) return;

                // Build script
                if (!cluster.UploadFile(e, RemoteProfile.GetRemoteProfileFilePath(Constants.SC_KMC_BUILDSCRIPT),
                    buildScriptPath, 744)) return;
                    
                Console.WriteLine("OK.");

                if (BWorker.CancellationPending == true) { e.Cancel = true; return; }

                Console.WriteLine("Compiling remote executables:");
                if (!cluster.ExecuteCommand(e, "sh \"" + buildScriptPath + "\"")) return;

                if (!cluster.ExecuteCommand(e, "cp -p \"" + simExeBuildPath + "\" \"" + simExePath + "\"")) return;

                if (!cluster.ExecuteCommand(e, "cp -p \"" + searchExeBuildPath + "\" \"" + searchExePath + "\"")) return;
            }

            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }

            // Transfer scripts to base directory
            Console.Write("Uploading scripts ... ");
            string submitScriptPath = RemotePaths.Combine(totalBasePath, Constants.SC_KMC_SUBMITSCRIPT);
            string jobScriptPath = RemotePaths.Combine(totalBasePath, Constants.SC_KMC_JOBSCRIPT);
                
            // Submit script
            if (!cluster.UploadFile(e, RemoteProfile.GetRemoteProfileFilePath(Constants.SC_KMC_SUBMITSCRIPT), 
                submitScriptPath, 744)) return;

            // Job script
            if (!cluster.UploadFile(e, RemoteProfile.GetRemoteProfileFilePath(Constants.SC_KMC_JOBSCRIPT), 
                jobScriptPath, 744)) return;
                
            Console.WriteLine("OK.");
            Console.WriteLine();

            // Submit all jobs
            BWorker.ReportProgress(15, "OK\n");
            double jobPercInc = 80.0 / Convert.ToDouble(_JobList.Count);
            double subProgress = 15.0;
            List<int> skipped_IDs = [];
            for (int i = 0; i < _JobList.Count; i++)
            {
                System.Threading.Thread.Sleep(Constants.THREAD_READING_DELAY);
                if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
                BWorker.ReportProgress(Convert.ToInt32(Math.Floor(subProgress)), "Submitting job " + (i + 1).ToString() + " (JobID: " + _JobList[i].ID.ToString() + ") ... ");
                Console.WriteLine("Job " + (i + 1).ToString() + " of " + _JobList.Count.ToString() + " (ID: " + _JobList[i].ID.ToString() + "):");

                // Create paths
                string jobName = _JobNamePrefix + "_" + _JobList[i].ID.ToString("0000");
                string jobDirectory = RemotePaths.Combine(totalBasePath, jobName);
                string jobInputPath = RemotePaths.Combine(jobDirectory, jobName + ".kmc");
                string jobLogPath = RemotePaths.Combine(jobDirectory, jobName + ".log");

                // Skip if folder and kmc file already exist
                if ((cluster.Exists(jobDirectory)) && (cluster.Exists(jobInputPath)))
                {
                    skipped_IDs.Add(_JobList[i].ID);
                    Console.WriteLine("Job directory: " + jobDirectory);
                    Console.WriteLine("Job input file: " + jobInputPath);
                    Console.WriteLine("Job directory and input file already exist. Skipping this KMC job.");
                    Console.WriteLine();

                    subProgress += jobPercInc;
                    BWorker.ReportProgress(Convert.ToInt32(Math.Floor(subProgress)), "Skipped\n");
                    continue;
                }

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
                    case Constants.KMCERR_OK:
                        break;
                    default:
                        throw new ApplicationException("Cannot save job data to string (TVMSettings.SubmitJobs, ErrorCode: " + ErrorCode.ToString() + ")");
                }
                Console.WriteLine("OK.");

                // Create job folder
                Console.Write("Creating job folder (" + jobDirectory + ") ... ");
                if (!cluster.CreateSingleDirectory(e, jobDirectory)) return;
                Console.WriteLine("OK.");

                // Transfer input file
                Console.Write("Uploading job file ... ");
                if (!cluster.CreateTextFile(e, jobInputPath, inp_file_str, 744)) return;
                Console.WriteLine("OK.");

                // Submit job to queue
                if (!cluster.ExecuteCommand(e, "sh \"" + submitScriptPath + "\" \"" + jobScriptPath + "\" \"" + 
                    simExePath + "\" \"" + jobDirectory + "\" \"" + jobName + "\" \"" + jobInputPath + 
                    "\" \"" + jobLogPath + "\"")) return;

                subProgress += jobPercInc;
                BWorker.ReportProgress(Convert.ToInt32(Math.Floor(subProgress)), "OK\n");
            }
            if (skipped_IDs.Count > 0)
            {
                Console.WriteLine("All jobs submitted except for the following IDs: " + skipped_IDs.ToRangeString());
                Console.WriteLine("Overwriting existing jobs is not permitted.");
                Console.WriteLine("Assign new unused IDs for these jobs and re-start their submission.");
            }
            else
            {
                Console.WriteLine("All jobs submitted.");
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(95, "Loading ... ");
            e.Result = new TMCJob(MCDLL);

            // Report completion
            if (skipped_IDs.Count > 0)
            {
                BWorker.ReportProgress(100, "OK\n(see console for skipped IDs)\n");
            }
            else
            {
                BWorker.ReportProgress(100, "OK\n");
            }

            System.Threading.Thread.Sleep(Constants.THREAD_FINISH_DELAY);
        }

        #endregion Submit Methods
    }
}
