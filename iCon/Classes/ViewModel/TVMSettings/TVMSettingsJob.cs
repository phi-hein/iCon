using System;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace iCon_General
{
    /// <summary>
    /// Holds the information for a single job setting in the Settings tab
    /// </summary>
    public class TVMSettingsJob : ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMSettingsJob(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _ID = 1;
            _SettingInfo = "";
            _Temperature = 0.0;
            _AttemptFrequency = 0.0;
            _WriteCheckpoint = true;
            _LoadCheckpoint = true;
            _EFieldMagnitude = 0.0;
            _EFieldDirX = 0;
            _EFieldDirY = 0;
            _EFieldDirZ = 0;
            _LatticeSize = 0;
            _AdditionalVacAnz = 0L;
            _DopConcentrations = new ObservableCollection<TVMSettingsJobConc>();
            _ConcInfo = "";
            _DoPrerun = false;
            _PreMCSP = 0L;
            _PreRecordAnz = 0;
            _DoDynNorm = false;
            _DynAttemptAnz = 0L;
            _DynRecordAnz = 0;
            _DynNormNum = 0;
            _MCSP = 0L;
            _MainRecordAnz = 0;
        }

        #region Properties

        public int _ID;
        /// <summary> 
        /// Job number 
        /// </summary>
        [IntRange(1, true, 100000, true, ErrorMessage="Job ID has to be in the interval [1, 100000]")]
        public int ID
        {
            get
            {
                return _ID;
            }
            set
            {
                ValidateNotify("ID", value, ref _ID);
            }
        }

        public string _SettingInfo;
        /// <summary> 
        /// Condensed setting information (without temperature, MCSP and doping concentrations) 
        /// </summary>
        public string SettingInfo
        {
            get
            {
                return _SettingInfo;
            }
            set
            {
                if (value != _SettingInfo)
                {
                    _SettingInfo = value;
                    Notify("SettingInfo");
                }
            }
        }

        public double _Temperature;
        /// <summary> 
        /// Simulation temperature [K]
        /// </summary>
        [DoubleRange(0.001, true, 10000, true, ErrorMessage="Temperature has to be in the interval [0.001, 10000]")]
        public double Temperature
        {
            get
            {
                return _Temperature;
            }
            set
            {
                ValidateNotify("Temperature", value, ref _Temperature);
            }
        }

        public double _AttemptFrequency;
        /// <summary> 
        /// Jump attempt frequency [1/s]
        /// </summary>
        [DoubleRange(0.001, true, 1.0E+34, true, ErrorMessage="Jump attempt frequency has to be in the interval [0.001, 1.0E+34]")]
        public double AttemptFrequency
        {
            get
            {
                return _AttemptFrequency;
            }
            set
            {
                ValidateNotify("AttemptFrequency", value, ref _AttemptFrequency);
            }
        }

        public bool _WriteCheckpoint;
        /// <summary> 
        /// Flag: true = checkpoint is written during simulation
        /// </summary>
        public bool WriteCheckpoint
        {
            get
            {
                return _WriteCheckpoint;
            }
            set
            {
                if (value != _WriteCheckpoint)
                {
                    _WriteCheckpoint = value;
                    Notify("WriteCheckpoint");
                }
            }
        }

        public bool _LoadCheckpoint;
        /// <summary> 
        /// Flag: true = try to load checkpoint before simulation
        /// </summary>
        public bool LoadCheckpoint
        {
            get
            {
                return _LoadCheckpoint;
            }
            set
            {
                if (value != _LoadCheckpoint)
                {
                    _LoadCheckpoint = value;
                    Notify("LoadCheckpoint");
                }
            }
        }

        public double _EFieldMagnitude;
        /// <summary> 
        /// Maximum contribution of the E-field to the activation energy [kT]
        /// </summary>
        [DoubleRange(0.0, true, 1000.0, true, ErrorMessage="E-field contribution has to be in the interval [0.0, 1000.0]")]
        public double EFieldMagnitude
        {
            get
            {
                return _EFieldMagnitude;
            }
            set
            {
                ValidateNotify("EFieldMagnitude", value, ref _EFieldMagnitude);
            }
        }

        public double _EFieldDirX;
        /// <summary> 
        /// x-coordinate of the E-field direction vector (force vector on positive test charge) 
        /// </summary>
        [DoubleRange(-1.0E+150, true, 1.0E+150, true, ErrorMessage = "E-field direction vector component has to be in the interval [-1.0E+150, 1.0E+150]")]
        public double EFieldDirX
        {
            get
            {
                return _EFieldDirX;
            }
            set
            {
                ValidateNotify("EFieldDirX", value, ref _EFieldDirX);
            }
        }

        public double _EFieldDirY;
        /// <summary> 
        /// y-coordinate of the E-field direction vector (force vector on positive test charge) 
        /// </summary>
        [DoubleRange(-1.0E+150, true, 1.0E+150, true, ErrorMessage = "E-field direction vector component has to be in the interval [-1.0E+150, 1.0E+150]")]
        public double EFieldDirY
        {
            get
            {
                return _EFieldDirY;
            }
            set
            {
                ValidateNotify("EFieldDirY", value, ref _EFieldDirY);
            }
        }

        public double _EFieldDirZ;
        /// <summary> 
        /// z-coordinate of the E-field direction vector (force vector on positive test charge) 
        /// </summary>
        [DoubleRange(-1.0E+150, true, 1.0E+150, true, ErrorMessage = "E-field direction vector component has to be in the interval [-1.0E+150, 1.0E+150]")]
        public double EFieldDirZ
        {
            get
            {
                return _EFieldDirZ;
            }
            set
            {
                ValidateNotify("EFieldDirZ", value, ref _EFieldDirZ);
            }
        }

        public int _LatticeSize;
        /// <summary> 
        /// Lattice size (number of unit cells along edge of cubic lattice of unit cells) 
        /// </summary>
        [IntRange(1, true, 65, true, ErrorMessage="Lattice size has to be in the interval [1, 65]")]
        public int LatticeSize
        {
            get
            {
                return _LatticeSize;
            }
            set
            {
                ValidateNotify("LatticeSize", value, ref _LatticeSize);
            }
        }

        public long _AdditionalVacAnz;
        /// <summary> 
        /// Additional, artificial vacancies (may be zero or negative) 
        /// </summary>
        [LongRange(-1000000L, true, 1000000L, true, ErrorMessage="Number of additional vacancies has to be in the interval [-1000000, 1000000]")]
        public long AdditionalVacAnz
        {
            get
            {
                return _AdditionalVacAnz;
            }
            set
            {
                ValidateNotify("AdditionalVacAnz", value, ref _AdditionalVacAnz);
            }
        }

        public ObservableCollection<TVMSettingsJobConc> _DopConcentrations;
        /// <summary> 
        /// Doping concentrations 
        /// </summary>
        public ObservableCollection<TVMSettingsJobConc> DopConcentrations
        {
            get
            {
                return _DopConcentrations;
            }
            set
            {
                if (value != _DopConcentrations)
                {
                    _DopConcentrations = value;
                    Notify("DopConcentrations");
                }
            }
        }

        public string _ConcInfo;
        /// <summary> 
        /// Condensed doping concentrations information 
        /// </summary>
        public string ConcInfo
        {
            get
            {
                return _ConcInfo;
            }
            set
            {
                if (value != _ConcInfo)
                {
                    _ConcInfo = value;
                    Notify("ConcInfo");
                }
            }
        }

        public bool _DoPrerun;
        /// <summary> 
        /// Flag, true = execute pre-run during simulation 
        /// </summary>
        public bool DoPrerun
        {
            get
            {
                return _DoPrerun;
            }
            set
            {
                if (value != _DoPrerun)
                {
                    _DoPrerun = value;
                    Notify("DoPrerun");
                }
            }
        }

        public long _PreMCSP;
        /// <summary> 
        /// Number of Monte-Carlo steps per particle during pre-run
        /// </summary>
        [LongRange(0L, true, 5000000L, true, ErrorMessage="MCSP of the pre-run has to be in the interval [0, 5000000]")]
        public long PreMCSP
        {
            get
            {
                return _PreMCSP;
            }
            set
            {
                ValidateNotify("PreMCSP", value, ref _PreMCSP);
            }
        }

        public int _PreRecordAnz;
        /// <summary> 
        /// Number of probabilities (highest), that are recorded during pre-run
        /// </summary>
        [IntRange(0, true, 10000, true, ErrorMessage="Pre-run record list size has to be in the interval [0, 10000]")]
        public int PreRecordAnz
        {
            get
            {
                return _PreRecordAnz;
            }
            set
            {
                ValidateNotify("PreRecordAnz", value, ref _PreRecordAnz);
            }
        }

        public bool _DoDynNorm;
        /// <summary> 
        /// Flag, true = execute dynamic normalization during simulation 
        /// </summary>
        public bool DoDynNorm
        {
            get
            {
                return _DoDynNorm;
            }
            set
            {
                if (value != _DoDynNorm)
                {
                    _DoDynNorm = value;
                    Notify("DoDynNorm");
                }
            }
        }

        public long _DynAttemptAnz;
        /// <summary> 
        /// Number of jump attempts during dynamic normalization
        /// </summary>
        [LongRange(0L, true, 5000000000L, true, ErrorMessage = "Dyn. norm. jump attempt number has to be in the interval [0, 5000000000]")]
        public long DynAttemptAnz
        {
            get
            {
                return _DynAttemptAnz;
            }
            set
            {
                ValidateNotify("DynAttemptAnz", value, ref _DynAttemptAnz);
            }
        }

        public int _DynRecordAnz;
        /// <summary> 
        /// Number of probabilities (highest), that are recorded during dynamic normalization
        /// </summary>
        [IntRange(0, true, 10000, true, ErrorMessage = "Dyn. norm. record list size has to be in the interval [0, 10000]")]
        public int DynRecordAnz
        {
            get
            {
                return _DynRecordAnz;
            }
            set
            {
                ValidateNotify("DynRecordAnz", value, ref _DynRecordAnz);
            }
        }

        public int _DynNormNum;
        /// <summary> 
        /// Number of the entry in the probability list, that is used for dynamic normalization
        /// </summary>
        [IntRange(0, true, 10000, true, ErrorMessage = "Dyn. norm. list entry number has to be in the interval [0, 10000]")]
        public int DynNormNum
        {
            get
            {
                return _DynNormNum;
            }
            set
            {
                ValidateNotify("DynNormNum", value, ref _DynNormNum);
            }
        }

        public long _MCSP;
        /// <summary> 
        /// Number of Monte-Carlo steps per particle during main simulation
        /// </summary>
        [LongRange(1, true, 5000000, true, ErrorMessage = "Main simulation MCSP has to be in the interval [1, 5000000]")]
        public long MCSP
        {
            get
            {
                return _MCSP;
            }
            set
            {
                ValidateNotify("MCSP", value, ref _MCSP);
            }
        }

        public int _MainRecordAnz;
        /// <summary> 
        /// Number of probabilities (highest), that are recorded during main simulation
        /// </summary>
        [IntRange(0, true, 10000, true, ErrorMessage = "Main simulation record list size has to be in the interval [0, 10000]")]
        public int MainRecordAnz
        {
            get
            {
                return _MainRecordAnz;
            }
            set
            {
                ValidateNotify("MainRecordAnz", value, ref _MainRecordAnz);
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Create a deep copy (except viewmodel reference) of this object
        /// </summary>
        public TVMSettingsJob GetCopy()
        {
            // Create shallow copy (all fields are copied, references still point so the same objects)
            TVMSettingsJob t_copy = (TVMSettingsJob)this.MemberwiseClone();

            // Create new objects for reference fields
            t_copy.DopConcentrations = new ObservableCollection<TVMSettingsJobConc>();
            if (_DopConcentrations.Count > 0)
            {
                for (int i = 0; i < _DopConcentrations.Count; i++)
                {
                    t_copy.DopConcentrations.Add(this._DopConcentrations[i].GetCopy());
                }
            }

            return t_copy;
        }

        /// <summary>
        /// Validate this object and all sub-objects
        /// </summary>
        public bool ValidateFullObject()
        {
            if (ValidateObject() == false) return false;
            if (_DopConcentrations.Count > 0)
            {
                for (int i = 0; i < _DopConcentrations.Count; i++)
                {
                    if (_DopConcentrations[i].ValidateObject() == false) return false;
                }
            }
            return true;
        }

        #endregion Methods

        #region Synchronization Methods

        /// <summary>
        /// Apply the user input values to the model (use only on a background worker thread)
        /// </summary>
        public bool ApplyData(TMCJobWrapper MCDLL, BackgroundWorker BWorker, DoWorkEventArgs e, bool ShowProgress = true)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            if (BWorker.CancellationPending == true) { e.Cancel = true; return false; }
            if (ShowProgress == true) BWorker.ReportProgress(0, "Validating physical parameters ... ");

            // Set temperature
            ErrorCode = MCDLL.SetTemperature(_Temperature);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Temperature\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Setting temperature failed (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }
            
            // Set attempt frequency
            ErrorCode = MCDLL.SetAttemptFrequency(_AttemptFrequency);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Attempt Frequency\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid jump attempt frequency (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Set checkpoint flags
            ErrorCode = MCDLL.SetCheckpointFlags(_WriteCheckpoint, _LoadCheckpoint);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Invalid checkpoint flags (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }
           
            // Set E-field
            ErrorCode = MCDLL.SetEField(_EFieldDirX, _EFieldDirY, _EFieldDirZ, _EFieldMagnitude);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid E-Field\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid E-field parameters (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Set lattice size
            ErrorCode = MCDLL.SetLatticeSize(_LatticeSize);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Lattice Size\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid lattice size (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Set additional vacancies
            ErrorCode = MCDLL.SetAdditionalVacAnz(_AdditionalVacAnz);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Additional Vacancies\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid additional vacancy number (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            if (ShowProgress == true) BWorker.ReportProgress(20, "OK\n");
            if (ShowProgress == true) System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return false; }
            if (ShowProgress == true) BWorker.ReportProgress(20, "Validating doping concentrations ... ");

            // Set doping concentrations
            if (_DopConcentrations.Count > 0)
            {
                for (int i = 0; i < _DopConcentrations.Count; i++)
                {
                    ErrorCode = MCDLL.SetDopandConcentration(i, _DopConcentrations[i]._Conc);
                    switch (ErrorCode)
                    {
                        case ConstantsClass.KMCERR_OK:
                            break;
                        case ConstantsClass.KMCERR_INVALID_INPUT:
                            e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Doping Concentration\n(see console for details)\n",
                                ConstantsClass.KMCERR_INVALID_INPUT, false);
                            return false;
                        default:
                            throw new ApplicationException("Invalid doping concentration (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                    }
                }
            }

            if (ShowProgress == true) BWorker.ReportProgress(50, "OK\n");
            if (ShowProgress == true) System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return false; }
            if (ShowProgress == true) BWorker.ReportProgress(50, "Validating control parameters ... ");

            // Set prerun options
            ErrorCode = MCDLL.SetPrerunOptions(_DoPrerun, _PreMCSP, _PreRecordAnz);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Pre-Run Options\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid pre-run parameters (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Set dynamic normalization options
            ErrorCode = MCDLL.SetDynNormParameters(_DoDynNorm, _DynAttemptAnz, _DynRecordAnz, _DynNormNum);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Dyn. Norm. Options\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid dyn. norm. parameters (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Set main simulation parameters
            ErrorCode = MCDLL.SetMainKMCOptions(_MCSP, _MainRecordAnz);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Main-Sim. Options\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid main simulation parameters (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            if (ShowProgress == true) BWorker.ReportProgress(70, "OK\n");
            if (ShowProgress == true) System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return false; }
            if (ShowProgress == true) BWorker.ReportProgress(70, "Validating settings ... ");

            // Validate current settings
            ErrorCode = MCDLL.MakeSettings();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Settings\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return false;
                default:
                    throw new ApplicationException("Invalid settings (TVMSettingsJob.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Retrieve data from MCDLL
            if (ShowProgress == true) BWorker.ReportProgress(90, "OK\nLoading ... ");
            if (ShowProgress == true) e.Result = new TMCJob(MCDLL);
            if (ShowProgress == true) BWorker.ReportProgress(100, "OK");

            if (ShowProgress == true) System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
            return true;
        }

        /// <summary>
        /// Clear the settings in the model (use only on a background worker thread)
        /// </summary>
        public void ClearData(TMCJobWrapper MCDLL, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Clearing settings ... ");

            // Clear all settings
            ErrorCode = MCDLL.ClearSettings();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Clearing settings failed (TVMSettingsJob.ClearData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(60, "OK\nLoading ... ");
            e.Result = new TMCJob(MCDLL);
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data from _ViewModel
        /// </summary>
        public void GetData()
        {
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMSettingsJob.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMSettingsJob.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMSettingsJob.GetData)");

            // Transfer data (skip validation by using fields)
            _ID = 1;
            _Temperature = MCJob.Settings.Temperature;
            _AttemptFrequency = MCJob.Settings.AttemptFrequency;
            _WriteCheckpoint = MCJob.Settings.WriteCheckpoint;
            _LoadCheckpoint = MCJob.Settings.LoadCheckpoint;
            _EFieldMagnitude = MCJob.Settings.EFieldMagnitude;
            _EFieldDirX = MCJob.Settings.EFieldDirection.X;
            _EFieldDirY = MCJob.Settings.EFieldDirection.Y;
            _EFieldDirZ = MCJob.Settings.EFieldDirection.Z;
            _LatticeSize = MCJob.Settings.LatticeSize;
            _AdditionalVacAnz = MCJob.Settings.AdditionalVacAnz;
            _DoPrerun = MCJob.Settings.DoPrerun;
            _PreMCSP = MCJob.Settings.PreMCSP;
            _PreRecordAnz = MCJob.Settings.PreRecordAnz;
            _DoDynNorm = MCJob.Settings.DoDynNorm;
            _DynAttemptAnz = MCJob.Settings.DynAttemptAnz;
            _DynRecordAnz = MCJob.Settings.DynRecordAnz;
            _DynNormNum = MCJob.Settings.DynNormNum;
            _MCSP = MCJob.Settings.MCSP;
            _MainRecordAnz = MCJob.Settings.MainRecordAnz;            
            _DopConcentrations.Clear();
            _ConcInfo = "";
            if (MCJob.Settings.DopandConc.Count != 0)
            {
                TVMSettingsJobConc t_Conc = null;
                ObservableCollection<TVMSettingsJobConc> t_Concs = new ObservableCollection<TVMSettingsJobConc>();
                if (MCJob.Settings.DopandConc.Count != MCJob.Structure.DopandID.Count) throw new ApplicationException("Inconsistent doping data (TVMSettingsJob.GetData)");
                for (int i = 0; i < MCJob.Settings.DopandConc.Count; i++)
                {
                    t_Conc = new TVMSettingsJobConc(_ViewModel);
                    t_Conc._Conc = MCJob.Settings.DopandConc[i];
                    if (MCJob.Elements.ElemName[MCJob.Structure.DopedID[i]] != "")
                    {
                        t_Conc._DopingDesc = MCJob.Elements.ElemSymbol[MCJob.Structure.DopedID[i]] + " [" +
                            MCJob.Elements.ElemName[MCJob.Structure.DopedID[i]] + ", " + 
                            string.Format(ConstantsClass.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[MCJob.Structure.DopedID[i]]) + "]";
                    }
                    else
                    {
                        t_Conc._DopingDesc = MCJob.Elements.ElemSymbol[MCJob.Structure.DopedID[i]] + " [" +
                            string.Format(ConstantsClass.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[MCJob.Structure.DopedID[i]]) + "]";
                    }
                    if (MCJob.Elements.ElemName[MCJob.Structure.DopandID[i]] != "")
                    {
                        t_Conc._DopingDesc += " -> " + MCJob.Elements.ElemSymbol[MCJob.Structure.DopandID[i]] + " [" +
                            MCJob.Elements.ElemName[MCJob.Structure.DopandID[i]] + ", " + 
                            string.Format(ConstantsClass.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[MCJob.Structure.DopandID[i]]) + "]";
                    }
                    else
                    {
                        t_Conc._DopingDesc += " -> " + MCJob.Elements.ElemSymbol[MCJob.Structure.DopandID[i]] + " [" +
                            string.Format(ConstantsClass.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[MCJob.Structure.DopandID[i]]) + "]";
                    }
                    if (i != 0) _ConcInfo += "; ";
                    _ConcInfo += string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, MCJob.Settings.DopandConc[i]) + " (" + 
                        MCJob.Settings.DopandAnz[i].ToString() + ")";
                    t_Concs.Add(t_Conc);
                }
                _DopConcentrations = t_Concs;
            }
            _SettingInfo = "Lattice Size: " + MCJob.Settings.LatticeSize.ToString() + "; ";
            _SettingInfo += "E-Field: (" + string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, MCJob.Settings.EField.X) + " , " + 
                string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, MCJob.Settings.EField.Y) + 
                " , " + string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, MCJob.Settings.EField.Z) + ") V/cm; ";
            _SettingInfo += "Att. Freq.: " + string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, MCJob.Settings.AttemptFrequency) + " Hz; ";
            _SettingInfo += "Chk-Write: ";
            if (MCJob.Settings.WriteCheckpoint == true)
            {
                _SettingInfo += "yes; ";
            }
            else
            {
                _SettingInfo += "no; ";
            }
            _SettingInfo += "Chk-Load: ";
            if (MCJob.Settings.LoadCheckpoint == true)
            {
                _SettingInfo += "yes; ";
            }
            else
            {
                _SettingInfo += "no; ";
            }
            _SettingInfo += "Add. Vac.: " + MCJob.Settings.AdditionalVacAnz.ToString() + "; ";
            _SettingInfo += "Total Vac.: " + string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, MCJob.Settings.TotalVacConc) + " (" + 
                MCJob.Settings.TotalVacAnz.ToString() + "); ";
            _SettingInfo += "Pre-Run: ";
            if (MCJob.Settings.DoPrerun == true)
            {
                _SettingInfo += "yes (" + MCJob.Settings.PreMCSP.ToString() + " MCSP, " + MCJob.Settings.PreRecordAnz.ToString() + " Rec.); ";
            }
            else
            {
                _SettingInfo += "no; ";
            }
            _SettingInfo += "Dyn.-Norm.: ";
            if (MCJob.Settings.DoDynNorm == true)
            {
                _SettingInfo += "yes (" + MCJob.Settings.DynAttemptAnz.ToString() + " Att., " + MCJob.Settings.DynRecordAnz.ToString() + " Rec., " +
                    MCJob.Settings.DynNormNum.ToString() + ". Entry); ";
            }
            else
            {
                _SettingInfo += "no; ";
            }
            _SettingInfo += "Main Sim.: " + MCJob.Settings.MainRecordAnz.ToString() + " Rec.";

            // Notify for binding updates
            Notify("ID");
            Notify("SettingInfo");
            Notify("Temperature");
            Notify("AttemptFrequency");
            Notify("WriteCheckpoint");
            Notify("LoadCheckpoint");
            Notify("EFieldMagnitude");
            Notify("EFieldDirX");
            Notify("EFieldDirY");
            Notify("EFieldDirZ");
            Notify("LatticeSize");
            Notify("AdditionalVacAnz");
            Notify("DopConcentrations");
            Notify("ConcInfo");
            Notify("DoPrerun");
            Notify("PreMCSP");
            Notify("PreRecordAnz");
            Notify("DoDynNorm");
            Notify("DynAttemptAnz");
            Notify("DynRecordAnz");
            Notify("DynNormNum");
            Notify("MCSP");
            Notify("MainRecordAnz");
        }

        #endregion Synchronization Methods
    }
}
