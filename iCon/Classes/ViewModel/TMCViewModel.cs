using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using HelixToolkit.Wpf;
using Microsoft.Win32;

namespace iCon_General
{
    /// <summary> 
    /// Viewmodel for the Monte-Carlo GUI 
    /// </summary>
    public class TMCViewModel: ViewModelObject
    {
        #region Internal Variables

        /// <Summary> 
        /// C++/CLI object which handles internal calculations and MC data (from MCJob_UDLL) 
        /// </Summary>
        protected TMCJobWrapper MCDLL = null;

        /// <summary> 
        /// BackgroundWorker for transfering input data and doing MC preparations 
        /// </summary>
        protected BackgroundWorker BWorker = null;

        /// <summary> 
        /// Window for displaying progress 
        /// </summary>
        protected ProgressDialog ProgressDlg = null;

        #endregion Internal Variables

        #region Properties

        #region Properties: Progress Information

        protected string _ProgressStatus = "";
        /// <summary>
        /// Status text in the progress window (public readonly)
        /// </summary>
        public string ProgressStatus
        {
            get
            {
                return _ProgressStatus;
            }
            protected set
            {
                if (value != _ProgressStatus)
                {
                    _ProgressStatus = value;
                    Notify("ProgressStatus");
                }
            }
        }
        
        protected string _ProgressDesc = "";
        /// <summary> 
        /// Description text for task progress in progress window (public readonly) 
        /// </summary>
        public string ProgressDesc
        {
            get
            {
                return _ProgressDesc;
            }
            protected set
            {
                if (value != _ProgressDesc)
                {
                    _ProgressDesc = value;
                    Notify("ProgressDesc");
                }
            }
        }

        protected int _ProgressPerc = 0;
        /// <summary> 
        /// Task progress (integer in [0,100]) (public readonly) 
        /// </summary>
        public int ProgressPerc
        {
            get
            {
                return _ProgressPerc;
            }
            protected set
            {
                if (value != _ProgressPerc)
                {
                    _ProgressPerc = value;
                    Notify("ProgressPerc");
                }
            }
        }

        protected string _ProgressPercStr = "";
        /// <summary> 
        /// Task progress (string in [0,100] %) (public readonly) 
        /// </summary>
        public string ProgressPercStr
        {
            get
            {
                return _ProgressPercStr;
            }
            protected set
            {
                if (value != _ProgressPercStr)
                {
                    _ProgressPercStr = value;
                    Notify("ProgressPercStr");
                }
            }
        }

        #endregion Properties: Progress Information

        #region Properties: Project Management

        /// <summary> 
        /// Show if the viewmodel is ready for input (readonly) 
        /// </summary>
        public bool IsReady
        {
            get 
            {
                if (MCDLL == null) return false;
                if (MCDLL.IfReady() == false) return false;
                return true;
            }
        }

        protected bool _IsSaved = false;
        /// <summary> 
        /// true = input data and saved file are identical (public readonly) 
        /// </summary>
        public bool IsSaved
        {
            get 
            { 
                return _IsSaved; 
            }
            protected set 
            {
                if (value != _IsSaved)
                {
                    _IsSaved = value;
                    Notify("IsSaved");
                }
            }
        }      

        protected string _ProjectPath = "";
        /// <summary> 
        /// Save path for the project file (public readonly) 
        /// </summary>
        public string ProjectPath
        {
            get 
            { 
                return _ProjectPath; 
            }
            protected set 
            {
                if (value != _ProjectPath)
                {                    
                    _ProjectPath = value;
                    Notify("ProjectPath");
                }
            }
        }

        #endregion Properties: Project Management

        #region Properties: Combined Synchronization Flags

        /// <summary> 
        /// true = all data is synchronized with MCDLL (readonly) 
        /// </summary>
        public bool IsSynchronized
        {
            get
            {
                if (IsJobDescSynchronized == false) return false;
                if (IsStructureSynchronized == false) return false;
                if (IsShellCountsSynchronized == false) return false;
                if (IsUniqueJumpsSynchronized == false) return false;
                if (IsEnergiesSynchronized == false) return false;
                return true;
            }
        }

        protected bool _IsJobDescSynchronized = false;
        /// <summary> 
        /// true = Job Description data is synchronized with MCDLL 
        /// </summary>
        public bool IsJobDescSynchronized
        {
            get 
            { 
                return _IsJobDescSynchronized; 
            }
            set
            {
                if (value != _IsJobDescSynchronized)
                {
                    _IsJobDescSynchronized = value;
                    Notify("IsJobDescSynchronized");
                    Notify("IsSynchronized");
                }
                IsJobDescApplicable = !value;
            }
        }

        protected bool _IsJobDescApplicable = false;
        /// <summary> 
        /// true = Job Description data can be applied 
        /// </summary>
        public bool IsJobDescApplicable
        {
            get
            {
                return _IsJobDescApplicable;
            }
            set
            {
                if (value != _IsJobDescApplicable)
                {
                    _IsJobDescApplicable = value;
                    Notify("IsJobDescApplicable");
                }
            }
        }

        protected bool _IsStructureSynchronized = false;
        /// <summary> 
        /// true = Structure data is synchronized with MCDLL 
        /// </summary>
        public bool IsStructureSynchronized
        {
            get
            {
                return _IsStructureSynchronized;
            }
            set
            {
                if (value != _IsStructureSynchronized)
                {
                    _IsStructureSynchronized = value;
                    Notify("IsStructureSynchronized");
                    Notify("IsSynchronized");
                }
                IsStructureApplicable = !value;
            }
        }

        protected bool _IsStructureApplicable = false;
        /// <summary> 
        /// true = Structure data can be applied 
        /// </summary>
        public bool IsStructureApplicable
        {
            get
            {
                return _IsStructureApplicable;
            }
            set
            {
                if (value != _IsStructureApplicable)
                {
                    _IsStructureApplicable = value;
                    Notify("IsStructureApplicable");
                }
            }
        }

        protected bool _IsShellCountsSynchronized = false;
        /// <summary> 
        /// true = Shell Counts data is synchronized with MCDLL 
        /// </summary>
        public bool IsShellCountsSynchronized
        {
            get
            {
                return _IsShellCountsSynchronized;
            }
            set
            {
                if (value != _IsShellCountsSynchronized)
                {
                    _IsShellCountsSynchronized = value;
                    Notify("IsShellCountsSynchronized");
                    Notify("IsSynchronized");
                }
                IsShellCountsApplicable = !value;
            }
        }

        protected bool _IsShellCountsApplicable = false;
        /// <summary> 
        /// true = Shell Counts data can be applied
        /// </summary>
        public bool IsShellCountsApplicable
        {
            get
            {
                return _IsShellCountsApplicable;
            }
            set
            {
                if (value != _IsShellCountsApplicable)
                {
                    _IsShellCountsApplicable = value;
                    Notify("IsShellCountsApplicable");
                }
            }
        }

        protected bool _IsUniqueJumpsSynchronized = false;
        /// <summary> 
        /// true = Unique Jumps data is synchronized with MCDLL 
        /// </summary>
        public bool IsUniqueJumpsSynchronized
        {
            get
            {
                return _IsUniqueJumpsSynchronized;
            }
            set
            {
                if (value != _IsUniqueJumpsSynchronized)
                {
                    _IsUniqueJumpsSynchronized = value;
                    Notify("IsUniqueJumpsSynchronized");
                    Notify("IsSynchronized");
                }
                IsUniqueJumpsApplicable = !value;
            }
        }

        protected bool _IsUniqueJumpsApplicable = false;
        /// <summary> 
        /// true = Unique Jumps data can be applied
        /// </summary>
        public bool IsUniqueJumpsApplicable
        {
            get
            {
                return _IsUniqueJumpsApplicable;
            }
            set
            {
                if (value != _IsUniqueJumpsApplicable)
                {
                    _IsUniqueJumpsApplicable = value;
                    Notify("IsUniqueJumpsApplicable");
                }
            }
        }

        protected bool _IsEnergiesSynchronized = false;
        /// <summary> 
        /// true = Energies data is synchronized with MCDLL 
        /// </summary>
        public bool IsEnergiesSynchronized
        {
            get
            {
                return _IsEnergiesSynchronized;
            }
            set
            {
                if (value != _IsEnergiesSynchronized)
                {
                    _IsEnergiesSynchronized = value;
                    Notify("IsEnergiesSynchronized");
                    Notify("IsSynchronized");
                }
                IsEnergiesApplicable = !value;
            }
        }

        protected bool _IsEnergiesApplicable = false;
        /// <summary> 
        /// true = Energies data can be applied
        /// </summary>
        public bool IsEnergiesApplicable
        {
            get
            {
                return _IsEnergiesApplicable;
            }
            set
            {
                if (value != _IsEnergiesApplicable)
                {
                    _IsEnergiesApplicable = value;
                    Notify("IsEnergiesApplicable");
                }
            }
        }

        #endregion Properties: Combined Synchronization Flags

        #region Properties: Data ViewModels

        protected TMCJob _MCJob = null;
        /// <summary>
        /// Model, which holds the current (validated) state of the MCDLL/MCJob_UDLL as light-weight C# object
        /// </summary>
        public TMCJob MCJob
        {
            get
            {
                return _MCJob;
            }
            protected set
            {
                if (value != _MCJob)
                {
                    _MCJob = value;
                    Notify("MCJob");
                }
            }
        }

        protected TVMJobDesc _JobDesc = null;
        /// <summary> 
        /// Data ViewModel for Job Description Tab 
        /// </summary>
        public TVMJobDesc JobDesc
        {
            get
            {
                return _JobDesc;
            }
            protected set
            {
                if (value != _JobDesc)
                {
                    _JobDesc = value;
                    IsJobDescSynchronized = false;
                    Notify("JobDesc");
                }
            }
        }

        protected TVMStructure _Structure = null;
        /// <summary> 
        /// Data ViewModel for Structure Input Tab 
        /// </summary>
        public TVMStructure Structure
        {
            get
            {
                return _Structure;
            }
            protected set
            {
                if (value != _Structure)
                {
                    _Structure = value;
                    IsStructureSynchronized = false;
                    Notify("Structure");
                }
            }
        }

        protected TVMShellCounts _ShellCounts = null;
        /// <summary> 
        /// Data ViewModel for Shell Counts in Jumps Tab 
        /// </summary>
        public TVMShellCounts ShellCounts
        {
            get
            {
                return _ShellCounts;
            }
            protected set
            {
                if (value != _ShellCounts)
                {
                    _ShellCounts = value;
                    IsShellCountsSynchronized = false;
                    Notify("ShellCounts");
                }
            }
        }

        protected TVMUniqueJumps _UniqueJumps = null;
        /// <summary> 
        /// Data ViewModel for Unique Jumps in Jumps Tab 
        /// </summary>
        public TVMUniqueJumps UniqueJumps
        {
            get
            {
                return _UniqueJumps;
            }
            protected set
            {
                if (value != _UniqueJumps)
                {
                    _UniqueJumps = value;
                    IsUniqueJumpsSynchronized = false;
                    Notify("UniqueJumps");
                }
            }
        }

        protected TVMEnergies _Energies = null;
        /// <summary> 
        /// Data ViewModel for Energies Tab
        /// </summary>
        public TVMEnergies Energies
        {
            get
            {
                return _Energies;
            }
            protected set
            {
                if (value != _Energies)
                {
                    _Energies = value;
                    IsEnergiesSynchronized = false;
                    Notify("Energies");
                }
            }
        }

        protected TVMSettings _Settings = null;
        /// <summary> 
        /// Data ViewModel for Settings Tab
        /// </summary>
        public TVMSettings Settings
        {
            get
            {
                return _Settings;
            }
            protected set
            {
                if (value != _Settings)
                {
                    _Settings = value;
                    Notify("Settings");
                }
            }
        }

        protected TVM3DCrystal _CrystalModel = null;
        /// <summary> 
        /// 3D model for unit cell (and supercell) visualization 
        /// </summary>
        public TVM3DCrystal CrystalModel
        {
            get
            {
                return _CrystalModel;
            }
            protected set
            {
                if (value != _CrystalModel)
                {
                    _CrystalModel = value;
                    Notify("CrystalModel");
                }
            }
        }

        protected TVM3DJumps _JumpsModel = null;
        /// <summary> 
        /// 3D model for lattice jump visualization 
        /// </summary>
        public TVM3DJumps JumpsModel
        {
            get
            {
                return _JumpsModel;
            }
            protected set
            {
                if (value != _JumpsModel)
                {
                    _JumpsModel = value;
                    Notify("JumpsModel");
                }
            }
        }

        protected TVM3DUniqueJumps _UniqueJumpsModel = null;
        /// <summary> 
        /// 3D model for unique jump visualization 
        /// </summary>
        public TVM3DUniqueJumps UniqueJumpsModel
        {
            get
            {
                return _UniqueJumpsModel;
            }
            protected set
            {
                if (value != _UniqueJumpsModel)
                {
                    _UniqueJumpsModel = value;
                    Notify("UniqueJumpsModel");
                }
            }
        }

        protected TVM3DCodes _CodesModel = null;
        /// <summary> 
        /// 3D model for code visualization 
        /// </summary>
        public TVM3DCodes CodesModel
        {
            get
            {
                return _CodesModel;
            }
            protected set
            {
                if (value != _CodesModel)
                {
                    _CodesModel = value;
                    Notify("CodesModel");
                }
            }
        }

        protected TVM3DInteractions _InteractionsModel = null;
        /// <summary> 
        /// 3D model for interaction visualization 
        /// </summary>
        public TVM3DInteractions InteractionsModel
        {
            get
            {
                return _InteractionsModel;
            }
            protected set
            {
                if (value != _InteractionsModel)
                {
                    _InteractionsModel = value;
                    Notify("InteractionsModel");
                }
            }
        }

        protected TVMProjectFile _VMProjectFile = null;
        /// <summary>
        /// Model for the ProjectFile tab
        /// </summary>
        public TVMProjectFile VMProjectFile
        {
            get
            {
                return _VMProjectFile;
            }
            protected set
            {
                if (value != _VMProjectFile)
                {
                    _VMProjectFile = value;
                    Notify("VMProjectFile");
                }
            }
        }

        protected TVMGUISettings _VMGUISettings = null;
        /// <summary>
        /// Model for all the ini-file stored GUI information (e.g. cluster profiles)
        /// </summary>
        public TVMGUISettings VMGUISettings
        {
            get
            {
                return _VMGUISettings;
            }
            protected set
            {
                if (value != _VMGUISettings)
                {
                    _VMGUISettings = value;
                    Notify("VMGUISettings");
                }
            }
        }

        #endregion Properties: Data ViewModels

        #region Properties: Program Control

        protected int _SelectedTab = 0;
        /// <summary> Selected Tab Number </summary>
        public int SelectedTab
        {
            get
            {
                return _SelectedTab;
            }

            set
            {
                if (value != _SelectedTab)
                {
                    _SelectedTab = value;
                    Notify("SelectedTab");
                }
            }
        }

        protected int _Selected3DTab = 0;
        /// <summary> Selected Tab Number in 3D-Visualization </summary>
        public int Selected3DTab
        {
            get
            {
                return _Selected3DTab;
            }

            set
            {
                if (value != _Selected3DTab)
                {
                    _Selected3DTab = value;
                    switch (_Selected3DTab)
                    {
                        case 0: 
                            _CrystalModel.IsVisible = true;
                            _JumpsModel.IsVisible = false;
                            _UniqueJumpsModel.IsVisible = false;
                            _CodesModel.IsVisible = false;
                            _InteractionsModel.IsVisible = false;
                            break;
                        case 1:
                            _CrystalModel.IsVisible = false;
                            _JumpsModel.IsVisible = true;
                            _UniqueJumpsModel.IsVisible = false;
                            _CodesModel.IsVisible = false;
                            _InteractionsModel.IsVisible = false;
                            break;
                        case 2:
                            _CrystalModel.IsVisible = false;
                            _JumpsModel.IsVisible = false;
                            _UniqueJumpsModel.IsVisible = true;
                            _CodesModel.IsVisible = false;
                            _InteractionsModel.IsVisible = false;
                            break;
                        case 3:
                            _CrystalModel.IsVisible = false;
                            _JumpsModel.IsVisible = false;
                            _UniqueJumpsModel.IsVisible = false;
                            _CodesModel.IsVisible = true;
                            _InteractionsModel.IsVisible = false;
                            break;
                        case 4:
                            _CrystalModel.IsVisible = false;
                            _JumpsModel.IsVisible = false;
                            _UniqueJumpsModel.IsVisible = false;
                            _CodesModel.IsVisible = false;
                            _InteractionsModel.IsVisible = true;
                            break;
                    }
                    Notify("Selected3DTab");
                }
            }
        }

        protected int _WWEnergyColumnNumber = 0;
        /// <summary> Number of WW-Energies[]-Columns necessary for Energies-Tab </summary>
        public int WWEnergyColumnNumber
        {
            get
            {
                return _WWEnergyColumnNumber;
            }
            set
            {
                if (value != _WWEnergyColumnNumber)
                {
                    _WWEnergyColumnNumber = value;
                    Notify("WWEnergyColumnNumber");
                }
            }
        }

        protected bool _IsJobDescEditable = false;
        /// <summary> Flag: true = JobDesc Tab Content is editable </summary>
        public bool IsJobDescEditable
        {
            get 
            {
                return _IsJobDescEditable;
            }
            protected set
            {
                if (value != _IsJobDescEditable)
                {
                    _IsJobDescEditable = value;
                    Notify("IsJobDescEditable");
                }
            }
        }

        protected bool _IsStructureEditable = false;
        /// <summary> Flag: true = Structure Tab Content is editable </summary>
        public bool IsStructureEditable
        {
            get
            {
                return _IsStructureEditable;
            }
            protected set
            {
                if (value != _IsStructureEditable)
                {
                    _IsStructureEditable = value;
                    Notify("IsStructureEditable");
                }
            }
        }

        protected bool _IsJumpsEditable = false;
        /// <summary> Flag: true = Jumps Tab Content is editable </summary>
        public bool IsJumpsEditable
        {
            get
            {
                return _IsJumpsEditable;
            }
            protected set
            {
                if (value != _IsJumpsEditable)
                {
                    _IsJumpsEditable = value;
                    Notify("IsJumpsEditable");
                }
            }
        }

        protected bool _IsEnergiesEditable = false;
        /// <summary> Flag: true = Energies Tab Content is editable </summary>
        public bool IsEnergiesEditable
        {
            get
            {
                return _IsEnergiesEditable;
            }
            protected set
            {
                if (value != _IsEnergiesEditable)
                {
                    _IsEnergiesEditable = value;
                    Notify("IsEnergiesEditable");
                }
            }
        }

        protected bool _IsSettingsEditable = false;
        /// <summary> Flag: true = Settings Tab Content is editable </summary>
        public bool IsSettingsEditable
        {
            get
            {
                return _IsSettingsEditable;
            }
            protected set
            {
                if (value != _IsSettingsEditable)
                {
                    _IsSettingsEditable = value;
                    Notify("IsSettingsEditable");
                }
            }
        }

        #endregion Properties: Program Control

        #endregion Properties

        /// <summary>
        /// Constructor
        /// </summary>
        public TMCViewModel()
        {
            // Initialize sub-viewmodel objects
            _JobDesc = new TVMJobDesc(this);
            _Structure = new TVMStructure(this);
            _ShellCounts = new TVMShellCounts(this);
            _UniqueJumps = new TVMUniqueJumps(this);
            _Energies = new TVMEnergies(this);
            _Settings = new TVMSettings(this);
            _CrystalModel = new TVM3DCrystal(this);
            _JumpsModel = new TVM3DJumps(this);
            _UniqueJumpsModel = new TVM3DUniqueJumps(this);
            _CodesModel = new TVM3DCodes(this);
            _InteractionsModel = new TVM3DInteractions(this);
            _VMProjectFile = new TVMProjectFile(this);
            _VMGUISettings = new TVMGUISettings(this);

            // Create new project
            New();
        }

        /// <summary>
        /// Destructor
        /// </summary>
        ~TMCViewModel()
        {
            
        }

        #region Methods

        #region Methods: Progress Documentation

        /// <summary>
        /// Handles the change of the background worker progress
        /// </summary>
        public void BWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            ProgressPerc = e.ProgressPercentage;
            ProgressPercStr = e.ProgressPercentage.ToString() + " %";
            if (e.UserState is string) ProgressDesc += e.UserState as string;
        }

        /// <summary>
        /// Handles the cancellation of the background task
        /// </summary>
        public void ProgressDlg_Cancel(object sender, RoutedEventArgs e)
        {
            if (BWorker.IsBusy == true)
            {
                if (BWorker.CancellationPending == false)
                {
                    ProgressStatus = "Cancelled by User";
                    BWorker.CancelAsync();
                }
                ProgressDlg.btnCancel.IsEnabled = false;
            }
            else
            {
                ProgressDlg.Close();
            }
        }

        /// <summary>
        /// Handles the closing event of the progress dialog
        /// </summary>
        public void ProgressDlg_Closing(object sender, CancelEventArgs e)
        {
            if (BWorker.IsBusy == true)
            {
                if (BWorker.CancellationPending == false)
                {
                    ProgressStatus = "Cancelled by User";
                    BWorker.CancelAsync();
                }
                ProgressDlg.btnCancel.IsEnabled = false;
                e.Cancel = true;
            }
        }

        #endregion Methods: Progress Documentation

        #region Methods: Synchronization of Job Description

        /// <summary> 
        /// Send Job Description data to MCDLL on background worker thread 
        /// </summary>
        protected void JobDesc_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("MC object not ready (JobDesc_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMJobDesc) == false) throw new ApplicationException("Invalid argument (JobDesc_DoWork)");
            TVMJobDesc input = (TVMJobDesc)e.Argument;

            // Apply viewmodel data to MCDLL model
            input.ApplyData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread 
        /// </summary>
        protected void JobDesc_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _JobDesc.GetData(MCJob);
                _VMProjectFile.GetData(MCJob);
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (JobDesc_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Synchronization of Job Description

        #region Methods: Synchronization of Structure

        /// <summary> 
        /// Send Structure data to MCDLL on background worker thread 
        /// </summary>
        protected void Structure_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("MC object not ready (Structure_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMStructure) == false) throw new ApplicationException("Invalid DoWork argument (Structure_DoWork)");
            TVMStructure input = (TVMStructure)e.Argument;

            // Apply viewmodel data to MCDLL model
            input.ApplyData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread 
        /// </summary>
        protected void Structure_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _Structure.GetData(MCJob);
                _ShellCounts.GetData(MCJob);
                _UniqueJumps.GetData(MCJob);
                _Energies.GetData(MCJob);
                _Settings.CurrentJob.GetData(MCJob);
                _Settings.ClearJobList();
                _VMProjectFile.GetData(MCJob);
                IsJumpsEditable = true;
                IsShellCountsApplicable = true;
                SelectedTab = 2;
                _CrystalModel.UpdateModel();
                _JumpsModel.UpdateModel();
                _UniqueJumpsModel.UpdateModel();
                _CodesModel.UpdateModel();
                _InteractionsModel.UpdateModel();
                Selected3DTab = 0;
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (Structure_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Synchronization of Structure

        #region Methods: Synchronization of Shell Counts

        /// <summary> 
        /// Send Shell Counts data to MCDLL on background worker thread 
        /// </summary>
        protected void ShellCounts_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("MC object not ready (ShellCounts_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMShellCounts) == false) throw new ApplicationException("Invalid DoWork argument (ShellCounts_DoWork)");
            TVMShellCounts input = (TVMShellCounts)e.Argument;

            // Apply viewmodel data to MCDLL model
            input.ApplyData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread 
        /// </summary>
        protected void ShellCounts_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _ShellCounts.GetData(MCJob);
                _UniqueJumps.GetData(MCJob);
                _Energies.GetData(MCJob);
                _Settings.CurrentJob.GetData(MCJob);
                _Settings.ClearJobList();
                _VMProjectFile.GetData(MCJob);
                IsUniqueJumpsApplicable = true;
                _JumpsModel.UpdateModel();
                _UniqueJumpsModel.UpdateModel();
                _CodesModel.UpdateModel();
                _InteractionsModel.UpdateModel();
                Selected3DTab = 1;
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (ShellCounts_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Synchronization of Shell Counts

        #region Methods: Synchronization of UniqueJumps

        /// <summary> 
        /// Send UniqueJumps data to MCDLL on background worker thread 
        /// </summary>
        protected void UniqueJumps_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("MC object not ready (UniqueJumps_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMUniqueJumps) == false) throw new ApplicationException("Invalid DoWork argument (UniqueJumps_DoWork)");
            TVMUniqueJumps input = (TVMUniqueJumps)e.Argument;

            // Apply viewmodel data to MCDLL model
            input.ApplyData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread
        /// </summary>
        protected void UniqueJumps_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _UniqueJumps.GetData(MCJob);
                _Energies.GetData(MCJob);
                _Settings.CurrentJob.GetData(MCJob);
                _Settings.ClearJobList();
                _VMProjectFile.GetData(MCJob);
                IsEnergiesEditable = true;
                IsSettingsEditable = true;
                IsEnergiesApplicable = true;
                SelectedTab = 3;
                _JumpsModel.UpdateModel();
                _UniqueJumpsModel.UpdateModel();
                _CodesModel.UpdateModel();
                _InteractionsModel.UpdateModel();
                if (Selected3DTab < 2) Selected3DTab = 2;
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (UniqueJumps_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Synchronization of UniqueJumps

        #region Methods: Synchronization of Energies

        /// <summary> 
        /// Send Energy data to MCDLL on background worker thread 
        /// </summary>
        protected void Energies_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("KMC object not ready (Energies_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMEnergies) == false) throw new ApplicationException("Invalid DoWork argument (Energies_DoWork)");
            TVMEnergies input = (TVMEnergies)e.Argument;

            // Apply viewmodel data to MCDLL model
            input.ApplyData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread 
        /// </summary>
        protected void Energies_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _Energies.GetData(MCJob);
                _Settings.CurrentJob.GetData(MCJob);
                _Settings.ClearJobList();
                _VMProjectFile.GetData(MCJob);
                IsSettingsEditable = true;
                SelectedTab = 4;
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (Energies_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Synchronization of UniqueJumps

        #region Methods: Synchronization of Settings

        /// <summary> 
        /// Validate new setting via MCDLL on background worker thread 
        /// </summary>
        protected void Settings_NewJob_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("KMC object not ready (Settings_NewJob_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMSettings) == false) throw new ApplicationException("Invalid DoWork argument (Settings_NewJob_DoWork)");
            TVMSettings input = (TVMSettings)e.Argument;

            // Apply viewmodel data to MCDLL model
            if (input.CurrentJob == null) throw new ApplicationException("Invalid CurrentJob pointer (Settings_NewJob_DoWork)");
            input.CurrentJob.ApplyData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread (add validated settings to job list)
        /// </summary>
        protected void Settings_AddJob_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _Settings.CurrentJob.GetData(MCJob);
                _VMProjectFile.GetData(MCJob);
                _Settings.AddJob();
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (Settings_AddJob_RunWorkerCompleted)");
            }
        }

        /// <summary> 
        /// Finish background worker task on main thread (change selected job to new validated job setting)
        /// </summary>
        protected void Settings_ChangeJob_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _Settings.CurrentJob.GetData(MCJob);
                _VMProjectFile.GetData(MCJob);
                _Settings.ChangeSelectedJob();
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (Settings_ChangeJob_RunWorkerCompleted)");
            }
        }

        /// <summary> 
        /// Clear current setting on background worker thread 
        /// </summary>
        protected void Settings_ClearJob_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("KMC object not ready (Settings_ClearJob_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMSettings) == false) throw new ApplicationException("Invalid DoWork argument (Settings_ClearJob_DoWork)");
            TVMSettings input = (TVMSettings)e.Argument;

            // Apply viewmodel data to MCDLL model
            if (input.CurrentJob == null) throw new ApplicationException("Invalid CurrentJob pointer (Settings_ClearJob_DoWork)");
            input.CurrentJob.ClearData(MCDLL, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread
        /// </summary>
        protected void Settings_ClearJob_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _Settings.CurrentJob.GetData(MCJob);
                _VMProjectFile.GetData(MCJob);
                ProgressDlg.Close();
                IsSaved = false;
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (Settings_ClearJob_RunWorkerCompleted)");
            }
        }

        /// <summary> 
        /// Submit all jobs in job list (on background worker thread)
        /// </summary>
        protected void Settings_Submission_DoWork(object sender, DoWorkEventArgs e)
        {
            // Check if everything is ready for input
            if (IsReady == false) throw new ApplicationException("KMC object not ready (Settings_Submission_DoWork)");

            // Interpret argument
            if ((e.Argument is TVMSettings) == false) throw new ApplicationException("Invalid DoWork argument (Settings_Submission_DoWork)");
            TVMSettings input = (TVMSettings)e.Argument;

            // Submit jobs
            input.SubmitJobs(MCDLL, _VMGUISettings, BWorker, e);
        }

        /// <summary> 
        /// Finish background worker task on main thread
        /// </summary>
        protected void Settings_Submission_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is TMCJob)
            {
                MCJob = (TMCJob)e.Result;
                _VMProjectFile.GetData(MCJob);
                ProgressStatus = "Complete";
                ProgressDesc += "All simulations successfully submitted\n(see console for details)\n";
                ProgressDlg.btnCancel.Content = "OK";
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (Settings_Submission_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Synchronization of Settings

        #region Methods: Save Viewport3D Picture

        /// <summary>
        /// Save a picture of a HelixViewport3D
        /// </summary>
        public void SavePicture(HelixViewport3D HViewport)
        {
            // Check if valid viewport
            if (HViewport == null) return;
            if (HViewport.Children.Count == 0) return;

            // Get file path
            SaveFileDialog savedlg = new SaveFileDialog();
            savedlg.InitialDirectory = Directory.GetCurrentDirectory();
            savedlg.FileName = "Figure";
            savedlg.Title = "Save Image of 3D-Model";
            savedlg.DefaultExt = ".png";
            savedlg.Filter = "Image files (*.png, *.jpg)|*.png;*.jpg";
            savedlg.ValidateNames = true;
            savedlg.AddExtension = true;
            savedlg.OverwritePrompt = true;
            string t_savepath = "";
            if (savedlg.ShowDialog() == true)
            {
                t_savepath = savedlg.FileName;
            }
            else
            {
                return;
            }
            string t_ext = Path.GetExtension(t_savepath);
            if ((t_ext != ".png") && (t_ext != ".jpg")) return;

            // Save new current directory
            Directory.SetCurrentDirectory(Path.GetDirectoryName(t_savepath));

            // Create progress window
            ProgressDlg = new ProgressDialog();
            ProgressDlg.Owner = Application.Current.MainWindow;
            ProgressDlg.DataContext = this;
            ProgressPerc = 0;
            ProgressPercStr = "0 %";
            ProgressStatus = "Running";
            ProgressDesc = "";
            ProgressDlg.btnCancel.Click += new RoutedEventHandler(ProgressDlg_Cancel);
            ProgressDlg.Closing += new CancelEventHandler(ProgressDlg_Closing);

            // Initialize BackgroundWorker
            BWorker = new BackgroundWorker();
            BWorker.WorkerReportsProgress = true;
            BWorker.WorkerSupportsCancellation = true;
            BWorker.ProgressChanged += new ProgressChangedEventHandler(BWorker_ProgressChanged);

            // Start BackgroundWorker
            BWorker.DoWork += new DoWorkEventHandler(Save3DPicture_DoWork);
            BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Save3DPicture_RunWorkerCompleted);
            List<object> t_arguments = new List<object>();
            t_arguments.Add(HViewport);
            t_arguments.Add(t_savepath);
            BWorker.RunWorkerAsync(t_arguments);

            // Show progress dialog
            ProgressDlg.ShowDialog();
        }

        /// <summary> 
        /// Render and save picture on background worker thread 
        /// </summary>
        protected void Save3DPicture_DoWork(object sender, DoWorkEventArgs e)
        {
            // Interpret argument
            if ((e.Argument is List<object>) == false) throw new ApplicationException("Invalid DoWork argument (Save3DPicture_DoWork)");
            List<object> input = (List<object>)e.Argument;
            if (input.Count != 2) throw new ApplicationException("Invalid number of DoWork arguments (Save3DPicture_DoWork)");
            if ((input[0] is HelixViewport3D) == false) throw new ApplicationException("Invalid first DoWork argument (Save3DPicture_DoWork)");
            if ((input[1] is string) == false) throw new ApplicationException("Invalid second DoWork argument (Save3DPicture_DoWork)");
            HelixViewport3D i_viewport3d = (HelixViewport3D)input[0];
            string i_filename = (string)input[1];

            // Render big picture
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Rendering high quality image ... ");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);

            int t_maxpresize = 5000;
            int t_width = (int)i_viewport3d.RenderSize.Width;
            int t_height = (int)i_viewport3d.RenderSize.Height;
            if ((t_width == 0) || (t_height == 0)) return;
            double t_prescale = 1;
            if ((t_height < t_maxpresize) && (t_width < t_maxpresize))
            {
                if (t_width > t_height)
                {
                    t_prescale = (double)t_maxpresize / (double)t_width;
                }
                else
                {
                    t_prescale = (double)t_maxpresize / (double)t_height;
                }
            }
            t_width = (int)(t_width * t_prescale);
            t_height = (int)(t_height * t_prescale);
            int t_dpi = (int)(96 * t_prescale);
            RenderTargetBitmap t_renderer = null;
            i_viewport3d.Dispatcher.Invoke(DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        t_renderer = new RenderTargetBitmap(t_width, t_height, t_dpi, t_dpi, PixelFormats.Pbgra32);
                        t_renderer.Render(i_viewport3d);
                        t_renderer.Freeze();
                    }
            ));

            // Shrink picture
            BWorker.ReportProgress(40, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(40, "Resize image ... ");

            int t_maxfinalsize = 2500;
            double t_finalscale = 1;
            if ((t_height > t_maxfinalsize) || (t_width > t_maxfinalsize))
            {
                if (t_width > t_height)
                {
                    t_finalscale = (double)t_maxfinalsize / (double)t_width;
                }
                else
                {
                    t_finalscale = (double)t_maxfinalsize / (double)t_height;
                }
            }
            t_width = (int)(t_width * t_finalscale);
            t_height = (int)(t_height * t_finalscale);
            DrawingGroup t_drg = new DrawingGroup();
            RenderOptions.SetBitmapScalingMode(t_drg, BitmapScalingMode.Fant);
            t_drg.Children.Add(new ImageDrawing(BitmapFrame.Create(t_renderer), new Rect(0, 0, t_width, t_height)));
            DrawingVisual t_targetvisual = new DrawingVisual();
            DrawingContext t_targetcontext = t_targetvisual.RenderOpen();
            t_targetcontext.DrawDrawing(t_drg);
            RenderTargetBitmap t_secrenderer = new RenderTargetBitmap(t_width, t_height, 96, 96, PixelFormats.Pbgra32);
            t_targetcontext.Close();
            t_secrenderer.Render(t_targetvisual);

            // Save picture
            BWorker.ReportProgress(70, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(70, "Save image ... ");

            string t_ext = Path.GetExtension(i_filename);
            if ((t_ext != ".png") && (t_ext != ".jpg"))
            {
                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid file extension\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                return;
            }
            if (t_ext == ".png")
            {
                PngBitmapEncoder t_enc = new PngBitmapEncoder();
                t_enc.Frames.Add(BitmapFrame.Create(t_secrenderer));
                using (FileStream t_fstream = File.Create(i_filename)) t_enc.Save(t_fstream);
            }
            else
            {
                JpegBitmapEncoder t_enc = new JpegBitmapEncoder();
                t_enc.QualityLevel = 100;
                t_enc.Frames.Add(BitmapFrame.Create(t_secrenderer));
                using (FileStream t_fstream = File.Create(i_filename)) t_enc.Save(t_fstream);
            }

            e.Result = null;
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        /// <summary> 
        /// Finish background worker task on main thread
        /// </summary>
        protected void Save3DPicture_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }

            ProgressDlg.Close();
        }

        #endregion Methods: Save Viewport3D Picture

        #region Methods: Project Management

        /// <summary>
        /// Update alle sub-viewmodels with the current MCDLL-data and set program control properties according to project status
        /// </summary>
        protected void SyncCompleteMCJob()
        {
            // Get new MCJob object
            MCJob = new TMCJob(MCDLL);
            if (MCJob.IsValid == false) throw new ApplicationException("Creation of MCJob object failed (SyncCompleteMCJob)");

            // Synchronize data viewmodels
            _JobDesc.GetData(MCJob);
            _Structure.GetData(MCJob);
            _ShellCounts.GetData(MCJob);
            _UniqueJumps.GetData(MCJob);
            _Energies.GetData(MCJob);
            _Settings.CurrentJob.GetData(MCJob);
            _Settings.ClearJobList();
            _VMProjectFile.GetData(MCJob);

            // Update 3D models
            _CrystalModel.UpdateModel();
            _JumpsModel.UpdateModel();
            _UniqueJumpsModel.UpdateModel();
            _CodesModel.UpdateModel();
            _InteractionsModel.UpdateModel();

            // Interpret Project State (set Program Control Properties)
            SelectedTab = 0;
            Selected3DTab = 0;
            IsJobDescEditable = true;
            IsStructureEditable = true;
            IsJumpsEditable = false;
            IsEnergiesEditable = false;
            IsSettingsEditable = false;
            switch (MCJob.ProjectState)
            {
                case 0:         // no structure information present
                    break;
                case 1:         // elements are set, but no further structure
                    break;
                case 2:         // structure complete
                    IsJumpsEditable = true;
                    IsShellCountsApplicable = true;
                    SelectedTab = 2;
                    break;
                case 3:         // jumps complete
                    IsJumpsEditable = true;
                    IsShellCountsApplicable = true;
                    SelectedTab = 2;
                    Selected3DTab = 1;
                    break;
                case 4:         // unique jumps complete
                    IsJumpsEditable = true;
                    IsUniqueJumpsApplicable = true;
                    SelectedTab = 2;
                    Selected3DTab = 2;
                    break;
                case 5:         // unique codes complete
                    IsJumpsEditable = true;
                    IsEnergiesEditable = true;
                    IsSettingsEditable = true;
                    IsEnergiesApplicable = true;
                    SelectedTab = 3;
                    Selected3DTab = 3;
                    break;
                case 6:         // energy input complete, valid settings are present
                    IsJumpsEditable = true;
                    IsEnergiesEditable = true;
                    IsSettingsEditable = true;
                    SelectedTab = 4;
                    Selected3DTab = 3;
                    break;
                case 7:         // simulation initialization complete
                    IsJumpsEditable = true;
                    IsEnergiesEditable = true;
                    IsSettingsEditable = true;
                    SelectedTab = 4;
                    Selected3DTab = 3;
                    break;
                case 8:         // simulation complete
                    IsJumpsEditable = true;
                    IsEnergiesEditable = true;
                    IsSettingsEditable = true;
                    SelectedTab = 4;
                    Selected3DTab = 3;
                    break;
                case 9:         // results are present
                    IsJumpsEditable = true;
                    IsEnergiesEditable = true;
                    IsSettingsEditable = true;
                    SelectedTab = 4;
                    Selected3DTab = 3;
                    break;
                default:
                    throw new ApplicationException("Setting project state failed (SyncCompleteMCJob)");
            }
        }

        /// <summary>
        /// Create a new clean project
        /// </summary>
        public void New()
        {
            // Create new MCDLL object
            MCDLL = new TMCJobWrapper();
            if (IsReady == false) throw new ApplicationException("Creation of MCDLL object failed (New)");

            // Reset program control properties
            ProjectPath = "";
            IsSaved = false;
            WWEnergyColumnNumber = 0;

            // Sync sub-viewmodels via new MCJob object
            SyncCompleteMCJob();

            // Collect objects that are out of scope (like objects from previous projects)            
            GC.Collect();
        }

        /// <summary>
        /// Save project to file (if_save_as = true -> ask always for new file path, if_to_inifile = true -> save to default inifile)
        /// Note: if_to_inifile is superior to if_save_as
        /// </summary>
        public void Save(bool if_save_as = false, bool if_to_inifile = false)
        {
            // Check for valid project
            if (IsReady == false) return;

            // Create file path
            string t_filepath = "";
            if (if_to_inifile == true)
            {
                string t_dirpath = Path.Combine(
                    Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                    ConstantsClass.SC_KMC_APPDATA_OUTERFOLDER,
                    ConstantsClass.SC_KMC_APPDATA_INNERFOLDER);
                Directory.CreateDirectory(t_dirpath);
                t_filepath = Path.Combine(t_dirpath, ConstantsClass.SC_KMC_LASTSESSION_FILE);
            }
            else
            {
                // Ask because of pending changes
                if (IsSynchronized == false)
                {
                    if (MessageBox.Show("There are non-applied changes. Do you want to save without applying them?", "Warning",
                        MessageBoxButton.YesNo, MessageBoxImage.Question, MessageBoxResult.Yes) != MessageBoxResult.Yes)
                    {
                        return;
                    }
                }

                // Take old project path or select new path
                if ((_ProjectPath != "") && (File.Exists(_ProjectPath) == true) && (if_save_as == false))
                {
                    t_filepath = _ProjectPath;
                }
                else
                {
                    SaveFileDialog savedlg = new SaveFileDialog();
                    savedlg.InitialDirectory = Directory.GetCurrentDirectory();
                    if ((_JobDesc != null) && (_JobDesc.ProjectName != ""))
                    {
                        savedlg.FileName = _JobDesc.ProjectName;
                    }
                    else
                    {
                        savedlg.FileName = "New Project";
                    }
                    savedlg.Title = "Save KMC Project";
                    savedlg.DefaultExt = ".kmc";
                    savedlg.Filter = "KMC project files (*.kmc)|*.kmc";
                    savedlg.ValidateNames = true;
                    savedlg.AddExtension = true;
                    savedlg.OverwritePrompt = true;
                    if (savedlg.ShowDialog() == true)
                    {
                        t_filepath = savedlg.FileName;
                    }
                    else
                    {
                        return;
                    }
                    Directory.SetCurrentDirectory(Path.GetDirectoryName(t_filepath));
                }
            }

            // Save to file
            int ErrorCode = MCDLL.SaveToFile(t_filepath);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    MessageBox.Show("Invalid file name (see console).", "Notification", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    return;
                default:
                    throw new ApplicationException("Saving project failed (Save, ErrorCode = " + ErrorCode.ToString() + ")");
            }

            // Adjust related properties
            if (if_to_inifile == false)
            {
                IsSaved = true;
                ProjectPath = t_filepath;
            }
        }

        /// <summary> 
        /// Load project from file (if_from_inifile = true -> load from default inifile)
        /// </summary>
        public void Load(bool if_from_inifile = false)
        {
            // Check for valid project
            if (IsReady == false) return;

            // Create file path
            string t_filepath = "";
            if (if_from_inifile == true)
            {
                t_filepath = Path.Combine(
                    Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                    ConstantsClass.SC_KMC_APPDATA_OUTERFOLDER,
                    ConstantsClass.SC_KMC_APPDATA_INNERFOLDER,
                    ConstantsClass.SC_KMC_LASTSESSION_FILE);
            }
            else
            {
                OpenFileDialog opendlg = new OpenFileDialog();
                opendlg.InitialDirectory = Directory.GetCurrentDirectory();
                opendlg.Title = "Load KMC Project";
                opendlg.DefaultExt = ".kmc";
                opendlg.Filter = "KMC project files (*.kmc)|*.kmc";
                opendlg.Multiselect = false;
                opendlg.AddExtension = true;
                opendlg.ValidateNames = true;
                if (opendlg.ShowDialog() == true)
                {
                    t_filepath = opendlg.FileName;
                }
                else
                {
                    return;
                }
                Directory.SetCurrentDirectory(Path.GetDirectoryName(t_filepath));
            }
            if (File.Exists(t_filepath) == false) return;

            // Create progress window
            ProgressDlg = new ProgressDialog();
            ProgressDlg.Owner = Application.Current.MainWindow;
            ProgressDlg.DataContext = this;
            ProgressPerc = 0;
            ProgressPercStr = "0 %";
            ProgressStatus = "Running";
            ProgressDesc = "";
            ProgressDlg.btnCancel.Click += new RoutedEventHandler(ProgressDlg_Cancel);
            ProgressDlg.Closing += new CancelEventHandler(ProgressDlg_Closing);

            // Initialize BackgroundWorker
            BWorker = new BackgroundWorker();
            BWorker.WorkerReportsProgress = true;
            BWorker.WorkerSupportsCancellation = true;
            BWorker.ProgressChanged += new ProgressChangedEventHandler(BWorker_ProgressChanged);

            // Start loading process
            BWorker.DoWork += new DoWorkEventHandler(LoadProject_DoWork);
            BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(LoadProject_RunWorkerCompleted);
            List<object> t_arguments = new List<object>();
            t_arguments.Add(t_filepath);
            t_arguments.Add(if_from_inifile);
            BWorker.RunWorkerAsync(t_arguments);

            // Show progress dialog
            ProgressDlg.ShowDialog();
        }

        /// <summary>
        /// Load project on background thread
        /// </summary>
        protected void LoadProject_DoWork(object sender, DoWorkEventArgs e)
        {
            // Interpret argument
            if ((e.Argument is List<object>) == false) throw new ApplicationException("Invalid DoWork argument (LoadProject_DoWork)");
            List<object> input = (List<object>)e.Argument;
            if (input.Count != 2) throw new ApplicationException("Invalid number of DoWork arguments (LoadProject_DoWork)");
            if ((input[0] is string) == false) throw new ApplicationException("Invalid first DoWork argument (LoadProject_DoWork)");
            if ((input[1] is bool) == false) throw new ApplicationException("Invalid second DoWork argument (LoadProject_DoWork)");
            string i_filepath = (string)input[0];
            bool i_if_from_inifile = (bool)input[1];

            // Load project file
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Validating project file ... ");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);

            int ErrorCode = MCDLL.LoadFromFile(i_filepath);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid File\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                case ConstantsClass.KMCERR_INVALID_FILE_FORMAT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid File Format\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_FILE_FORMAT, false);
                    return;
                case ConstantsClass.KMCERR_INVALID_FILE_CONTENT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid File Content\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_FILE_CONTENT, false);
                    return;
                default:
                    throw new ApplicationException("Loading file failed (LoadProject_DoWork, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(70, "OK\nLoading data ... ");
            List<object> t_result = new List<object>();
            t_result.Add(new TMCJob(MCDLL));
            t_result.Add(i_filepath);
            t_result.Add(i_if_from_inifile);
            e.Result = t_result;
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        /// <summary> 
        /// Finish background worker task on main thread (complete loading process)
        /// </summary>
        protected void LoadProject_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                throw e.Error;
            }
            else if (e.Cancelled == true)
            {
                ProgressDlg.Close();
            }
            else if (e.Result is BWorkerResultMessage)
            {
                BWorkerResultMessage returnmsg = (BWorkerResultMessage)e.Result;
                ProgressStatus = returnmsg.MessageHeader;
                ProgressDesc += returnmsg.MessageText;
                ProgressDlg.btnCancel.Content = "OK";
            }
            else if (e.Result is List<object>)
            {
                List<object> o_result = (List<object>)e.Result;
                if (o_result.Count != 3) throw new ApplicationException("Invalid number of result objects (LoadProject_RunWorkerCompleted)");
                if ((o_result[0] is TMCJob) == false) throw new ApplicationException("Invalid first result object (LoadProject_RunWorkerCompleted)");
                if ((o_result[1] is string) == false) throw new ApplicationException("Invalid second result object (LoadProject_RunWorkerCompleted)");
                if ((o_result[2] is bool) == false) throw new ApplicationException("Invalid third result object (LoadProject_RunWorkerCompleted)");
                MCJob = (TMCJob)o_result[0];
                string o_filepath = (string)o_result[1];
                bool o_if_from_inifile = (bool)o_result[2];

                // Synchronize sub-viewmodels
                SyncCompleteMCJob();

                // Reset program control properties
                if (o_if_from_inifile == false)
                {
                    ProjectPath = o_filepath;
                    IsSaved = true;
                }

                // Collect objects that are out of scope (like objects from temporary loaded projects)            
                GC.Collect();

                ProgressDlg.Close();
            }
            else
            {
                throw new ApplicationException("Invalid background worker result (LoadProject_RunWorkerCompleted)");
            }
        }

        #endregion Methods: Project Management

        #region Methods: Button Commands

        /// <summary> Execute long task (e.g. MCDLL-Sync.) on background thread </summary>
        /// <param name="TabNumber"> Number of the task </param>
        public void ExecuteLongTask(int TaskNumber)
        {
            // Task-Description:
            // 0: Send Data of JobDesc-Tab to MCDLL
            // 1: Send Data of Structure-Tab to MCDLL
            // 2: Send ShellCounts Data of Jumps-Tab to MCDLL
            // 3: Send UniqueJumps Data of Jumps-Tab to MCDLL
            // 4: Send Data of Energies-Tab to MCDLL
            // 5: Add CurrentJob setting to job list (after validation via MCDLL)
            // 6: Change SelectedJob in job list to CurrentJob setting (after validation via MCDLL)
            // 7: Clear CurrentJob setting
            // 8: Start calculations for all jobs in job list

            // Only execute if necessary
            switch (TaskNumber)
            {
                case 0:
                    if (IsJobDescApplicable == false) return;
                    if (JobDesc.ValidateObject() == false)
                    {
                        MessageBox.Show("Please enter valid job description data.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 1:
                    if (IsStructureApplicable == false) return;
                    if (Structure.ValidateFullObject() == false)
                    {
                        MessageBox.Show("Please enter valid structure data.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 2:
                    if (IsShellCountsApplicable == false) return;
                    if (ShellCounts.ValidateObject() == false)
                    {
                        MessageBox.Show("Please enter valid shell counts data.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 3:
                    if (IsUniqueJumpsApplicable == false) return;
                    if (UniqueJumps.ValidateFullObject() == false)
                    {
                        MessageBox.Show("Please enter valid jump customization data.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 4:
                    if (IsEnergiesApplicable == false) return;
                    if (Energies.ValidateFullObject() == false)
                    {
                        MessageBox.Show("Please enter valid energy data.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 5:
                    if (Settings.CurrentJob.ValidateFullObject() == false)
                    {
                        MessageBox.Show("Please enter valid job settings.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 6:
                    if (_Settings.SelectedJob == null) return;
                    if (Settings.CurrentJob.ValidateFullObject() == false)
                    {
                        MessageBox.Show("Please enter valid job settings.", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    break;
                case 7:
                    break;
                case 8:
                    if (Settings.HasValidJobs == false) return;
                    if (Settings.ValidateForJobSubmit() == false)
                    {
                        MessageBox.Show("Please enter valid job submission data (IDs, Directory, etc.).", "Information", 
                            MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    if (VMGUISettings.ValidateForSubmit(Settings.CalcType) == false)
                    {
                        MessageBox.Show("Please enter valid extended settings (Workspace directories, Remote access settings, etc.).", "Information", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                        return;
                    }
                    if (IsSynchronized == false)
                    {
                        if (MessageBox.Show("There are non-applied changes. Do you still want to start the simulations?", 
                            "Warning", MessageBoxButton.YesNo) == MessageBoxResult.No)
                        {
                            return;
                        }
                    }
                    break;
                default:
                    throw new ApplicationException("Invalid task number (ExecuteLongTask)");
            }

            // Create progress window
            ProgressDlg = new ProgressDialog();
            ProgressDlg.Owner = Application.Current.MainWindow;
            ProgressDlg.DataContext = this;
            ProgressPerc = 0;
            ProgressPercStr = "0 %";
            ProgressStatus = "Running";
            ProgressDesc = "";
            ProgressDlg.btnCancel.Click += new RoutedEventHandler(ProgressDlg_Cancel);
            ProgressDlg.Closing += new CancelEventHandler(ProgressDlg_Closing);

            // Initialize BackgroundWorker
            BWorker = new BackgroundWorker();
            BWorker.WorkerReportsProgress = true;
            BWorker.WorkerSupportsCancellation = true;
            BWorker.ProgressChanged += new ProgressChangedEventHandler(BWorker_ProgressChanged);

            // Start background worker
            switch (TaskNumber)
            {
                case 0:
                    BWorker.DoWork += new DoWorkEventHandler(JobDesc_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(JobDesc_RunWorkerCompleted);
                    BWorker.RunWorkerAsync(_JobDesc);
                    break;
                case 1:
                    BWorker.DoWork += new DoWorkEventHandler(Structure_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Structure_RunWorkerCompleted);
                    IsJumpsEditable = false;
                    BWorker.RunWorkerAsync(_Structure);
                    break;
                case 2:
                    BWorker.DoWork += new DoWorkEventHandler(ShellCounts_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(ShellCounts_RunWorkerCompleted);
                    IsEnergiesEditable = false;
                    BWorker.RunWorkerAsync(_ShellCounts);
                    break;
                case 3:
                    BWorker.DoWork += new DoWorkEventHandler(UniqueJumps_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(UniqueJumps_RunWorkerCompleted);
                    IsEnergiesEditable = false;
                    BWorker.RunWorkerAsync(_UniqueJumps);
                    break;
                case 4:
                    BWorker.DoWork += new DoWorkEventHandler(Energies_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Energies_RunWorkerCompleted);
                    IsSettingsEditable = false;
                    BWorker.RunWorkerAsync(_Energies);
                    break;
                case 5:
                    BWorker.DoWork += new DoWorkEventHandler(Settings_NewJob_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Settings_AddJob_RunWorkerCompleted);
                    BWorker.RunWorkerAsync(_Settings);
                    break;
                case 6:
                    BWorker.DoWork += new DoWorkEventHandler(Settings_NewJob_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Settings_ChangeJob_RunWorkerCompleted);
                    BWorker.RunWorkerAsync(_Settings);
                    break;
                case 7:
                    BWorker.DoWork += new DoWorkEventHandler(Settings_ClearJob_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Settings_ClearJob_RunWorkerCompleted);
                    BWorker.RunWorkerAsync(_Settings);
                    break;
                case 8:
                    BWorker.DoWork += new DoWorkEventHandler(Settings_Submission_DoWork);
                    BWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(Settings_Submission_RunWorkerCompleted);
                    BWorker.RunWorkerAsync(_Settings);
                    break;
            }

            // Show progress dialog
            ProgressDlg.ShowDialog();
        }

        #endregion Methods: Button Commands

        #endregion Methods
    }
}
