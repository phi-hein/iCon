using System;
using System.ComponentModel;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for the ShellCounts part of the Jumps tab 
    /// </summary>
    public class TVMShellCounts: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMShellCounts(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _MovShellCount = 1;
            _JumpStartShellCount = 1;
            _JumpTSShellCount = 0;
            _JumpDestShellCount = 1;
        }

        #region Properties

        protected int _MovShellCount;       
        /// <summary> 
        /// Number of shells around moving atoms for jump destinations 
        /// </summary>
        [IntRange(1, true, 5, true, ErrorMessage="Number of shells has to be in the interval [1,5].")]
        public int MovShellCount
        {
            get
            {
                return _MovShellCount;
            }
            set
            {
                if (ValidateNotify("MovShellCount", value, ref _MovShellCount) == true)
                {
                    _ViewModel.IsShellCountsSynchronized = false;
                }
            }
        }

        protected int _JumpStartShellCount;       
        /// <summary> 
        /// Number of shells around jump start position 
        /// </summary>
        [IntRange(0, true, 150, true, ErrorMessage = "Number of shells has to be in the interval [0,150].")]
        public int JumpStartShellCount
        {
            get
            {
                return _JumpStartShellCount;
            }
            set
            {
                if (ValidateNotify("JumpStartShellCount", value, ref _JumpStartShellCount) == true)
                {
                    _ViewModel.IsShellCountsSynchronized = false;
                }
            }
        }

        protected int _JumpTSShellCount;        
        /// <summary> 
        /// Number of shells around jump center position 
        /// </summary>
        [IntRange(0, true, 150, true, ErrorMessage = "Number of shells has to be in the interval [0,150].")]
        public int JumpTSShellCount
        {
            get
            {
                return _JumpTSShellCount;
            }
            set
            {
                if (ValidateNotify("JumpTSShellCount", value, ref _JumpTSShellCount) == true)
                {
                    _ViewModel.IsShellCountsSynchronized = false;
                }
            }
        }

        protected int _JumpDestShellCount;       
        /// <summary> 
        /// Number of shells around jump destination position 
        /// </summary>
        [IntRange(0, true, 150, true, ErrorMessage = "Number of shells has to be in the interval [0,150].")]
        public int JumpDestShellCount
        {
            get
            {
                return _JumpDestShellCount;
            }
            set
            {
                if (ValidateNotify("JumpDestShellCount", value, ref _JumpDestShellCount) == true)
                {
                    _ViewModel.IsShellCountsSynchronized = false;
                }
            }
        }

        #endregion Properties

        #region Synchronization Methods

        /// <summary>
        /// Apply the user input values to the model (use only on a background worker thread)
        /// </summary>
        public void ApplyData(TMCJobWrapper MCDLL, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Calculating jumps ... ");

            // Create all jumps and environments
            ErrorCode = MCDLL.ClearJumps();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Clearing jumps failed (TVMShellCounts.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            ErrorCode = MCDLL.MakeJumps(_MovShellCount, _JumpStartShellCount, _JumpTSShellCount, _JumpDestShellCount);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Shell Counts\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid shell counts (TVMShellCounts.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            BWorker.ReportProgress(40, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(40, "Creating unique jumps ... ");

            // Create unique jumps and unique environments
            ErrorCode = MCDLL.ClearUniqueJumps();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Clearing unique jumps failed (TVMShellCounts.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            ErrorCode = MCDLL.MakeUniqueJumps();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Invalid unique jumps (TVMShellCounts.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(80, "OK\nLoading ... ");
            e.Result = new TMCJob(MCDLL);
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data from _ViewModel
        /// </summary>
        public void GetData()
        {
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMShellCounts.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMShellCounts.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMShellCounts.GetData)");

            // Transfer data (skip validation by using fields)
            _MovShellCount = MCJob.Jumps.MovShellCount;
            _JumpStartShellCount = MCJob.Jumps.JumpStartShellCount;
            _JumpTSShellCount = MCJob.Jumps.JumpTSShellCount;
            _JumpDestShellCount = MCJob.Jumps.JumpDestShellCount;

            // Notify for binding updates
            Notify("MovShellCount");
            Notify("JumpStartShellCount");
            Notify("JumpTSShellCount");
            Notify("JumpDestShellCount");

            _ViewModel.IsShellCountsSynchronized = true;
        }

        #endregion Synchronization Methods
    }
}
