using System;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for the UniqueJumps part of the Jumps tab 
    /// </summary>
    public class TVMUniqueJumps: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMUniqueJumps(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _InteractionShellCount = 0;
            _UJumps = new ObservableCollection<TVMUniqueJumpsJump>();
            _SelectedUJump = null;
        }

        #region Properties

        protected int _InteractionShellCount;       
        /// <summary> 
        /// Number of interaction environment shells for comparison of interactions 
        /// </summary>
        [IntRange(0, true, 5, true, ErrorMessage="Number of shells has to be in the interval [0,5].")]
        public int InteractionShellCount
        {
            get
            {
                return _InteractionShellCount;
            }
            set
            {
                if (value != _InteractionShellCount)
                {
                    _InteractionShellCount = value;
                    _ViewModel.IsUniqueJumpsSynchronized = false;
                    Notify("InteractionShellCount");
                }
            }
        }

        protected ObservableCollection<TVMUniqueJumpsJump> _UJumps;
        /// <summary> 
        /// Unique jumps 
        /// </summary>
        public ObservableCollection<TVMUniqueJumpsJump> UJumps
        {
            get
            {
                return _UJumps;
            }
            set
            {
                if (value != _UJumps)
                {
                    _UJumps = value;
                    _ViewModel.IsUniqueJumpsSynchronized = false;
                    Notify("UJumps");
                }
            }
        }

        protected TVMUniqueJumpsJump _SelectedUJump;
        /// <summary> 
        /// Selected unique jump 
        /// </summary>
        public TVMUniqueJumpsJump SelectedUJump
        {
            get
            {
                return _SelectedUJump;
            }
            set
            {
                if (value != _SelectedUJump)
                {
                    _SelectedUJump = value;
                    Notify("SelectedUJump");
                }
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Validate this object and all sub-objects
        /// </summary>
        public bool ValidateFullObject()
        {
            if (ValidateObject() == false) return false;
            if (_UJumps.Count > 0)
            {
                for (int i = 0; i < _UJumps.Count; i++)
                {
                    if (_UJumps[i].ValidateFullObject() == false) return false;
                }
            }
            return true;
        }

        #endregion Methods

        #region Synchronization Methods

        /// <summary>
        /// Apply the user input values to the model (use only on a background worker thread)
        /// </summary>
        public void ApplyData(TMCJobWrapper MCDLL, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Validating jump states ... ");

            // Activate/deactivate jumps
            if (_UJumps.Count < 1) throw new ApplicationException("No unique jumps (TVMUniqueJumps.ApplyData)");
            ErrorCode = MCDLL.ClearUniqueCodes();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Clearing unique codes failed (TVMUniqueJumps.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }
            for (int i = 0; i < _UJumps.Count; i++)
            {
                ErrorCode = MCDLL.SetJumpActive(_UJumps[i]._JumpID, _UJumps[i]._IsActive);
                switch (ErrorCode)
                {
                    case ConstantsClass.KMCERR_OK:
                        break;
                    case ConstantsClass.KMCERR_DEACTIVATION_IMPOSSIBLE:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Deactivation\n(see console for details)\n",
                            ConstantsClass.KMCERR_DEACTIVATION_IMPOSSIBLE, false);
                        return;
                    default:
                        throw new ApplicationException("Setting jump state failed (TVMUniqueJumps.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                }
            }

            BWorker.ReportProgress(10, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(10, "Validating atom states ... ");

            // Transfer atom states
            for (int i = 0; i < _UJumps.Count; i++)
            {
                if (_UJumps[i]._UniqueJumpAtoms.Count < 1) throw new ApplicationException("No atoms in unique jump (TVMUniqueJumps.ApplyData)");
                for (int j = 0; j < _UJumps[i]._UniqueJumpAtoms.Count; j++)
                {
                    ErrorCode = MCDLL.SetAtomIgnore(_UJumps[i]._JumpID, _UJumps[i]._UniqueJumpAtoms[j]._CoordID, _UJumps[i]._UniqueJumpAtoms[j]._IsIgnore);
                    switch (ErrorCode)
                    {
                        case ConstantsClass.KMCERR_OK:
                            break;
                        default:
                            throw new ApplicationException("Setting atom ignoration failed (TVMUniqueJumps.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                    }
                    ErrorCode = MCDLL.SetAtomAdditive(_UJumps[i]._JumpID, _UJumps[i]._UniqueJumpAtoms[j]._CoordID, _UJumps[i]._UniqueJumpAtoms[j]._IsAdditive);
                    switch (ErrorCode)
                    {
                        case ConstantsClass.KMCERR_OK:
                            break;
                        default:
                            throw new ApplicationException("Setting atom additivity failed (TVMUniqueJumps.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                    }
                    ErrorCode = MCDLL.SetAtomActive(_UJumps[i]._JumpID, _UJumps[i]._UniqueJumpAtoms[j]._CoordID, _UJumps[i]._UniqueJumpAtoms[j]._IsActive);
                    switch (ErrorCode)
                    {
                        case ConstantsClass.KMCERR_OK:
                            break;
                        default:
                            throw new ApplicationException("Setting atom activity failed (TVMUniqueJumps.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                    }
                }
            }

            BWorker.ReportProgress(20, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(20, "Creating unique environments ... ");

            // Generate codes and interactions
            ErrorCode = MCDLL.MakeUniqueCodes(_InteractionShellCount);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Input\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                case ConstantsClass.KMCERR_TOO_MANY_ACTIVE_ATOMS:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Input\n(see console for details)\n",
                        ConstantsClass.KMCERR_TOO_MANY_ACTIVE_ATOMS, false);
                    return;
                default:
                    throw new ApplicationException("Creating codes failed (TVMUniqueJumps.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(90, "OK\nLoading ... ");
            e.Result = new TMCJob(MCDLL);
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data from _ViewModel
        /// </summary>
        public void GetData()
        {
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMUniqueJumps.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMUniqueJumps.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMUniqueJumps.GetData)");

            // Transfer data (skip validation by using fields)
            _InteractionShellCount = MCJob.UJumps.InteractionShellCount;
            _UJumps.Clear();
            if (MCJob.UJumps.UJumps.Count != 0)
            {
                TVMUniqueJumpsJump t_UJump = null;
                ObservableCollection<TVMUniqueJumpsJump> t_UJumps = new ObservableCollection<TVMUniqueJumpsJump>();
                for (int i = 0; i < MCJob.UJumps.UJumps.Count; i++)
                {
                    t_UJump = new TVMUniqueJumpsJump(_ViewModel);
                    t_UJump._JumpID = i;
                    t_UJump._Length = MCJob.UJumps.UJumps[i].Length;
                    t_UJump._IsActive = MCJob.UJumps.UJumps[i].JumpActive;
                    t_UJump._UniqueJumpAtoms.Clear();
                    if (MCJob.UJumps.UJumps[i].ElemID.Count != 0)
                    {
                        TVMUniqueJumpsJumpAtom t_UniqueJumpAtom = null;
                        ObservableCollection<TVMUniqueJumpsJumpAtom> t_UniqueJumpAtoms = new ObservableCollection<TVMUniqueJumpsJumpAtom>();
                        for (int j = 0; j < MCJob.UJumps.UJumps[i].ElemID.Count; j++)
                        {
                            t_UniqueJumpAtom = new TVMUniqueJumpsJumpAtom(_ViewModel);
                            t_UniqueJumpAtom._CoordID = j;
                            t_UniqueJumpAtom._Symbol = MCJob.Elements.ElemSymbol[MCJob.UJumps.UJumps[i].ElemID[j]];
                            t_UniqueJumpAtom._Name = MCJob.Elements.ElemName[MCJob.UJumps.UJumps[i].ElemID[j]];
                            t_UniqueJumpAtom._Charge = MCJob.Elements.ElemCharge[MCJob.UJumps.UJumps[i].ElemID[j]];
                            t_UniqueJumpAtom._ZylPositionX = MCJob.UJumps.UJumps[i].Coord[j].X;
                            t_UniqueJumpAtom._ZylPositionY = MCJob.UJumps.UJumps[i].Coord[j].Y;
                            t_UniqueJumpAtom._ZylPositionZ = MCJob.UJumps.UJumps[i].Coord[j].Z;
                            t_UniqueJumpAtom._IsIgnore = MCJob.UJumps.UJumps[i].AtomIgnore[j];
                            t_UniqueJumpAtom._IsAdditive = MCJob.UJumps.UJumps[i].AtomAdditive[j];
                            t_UniqueJumpAtom._IsActive = MCJob.UJumps.UJumps[i].AtomActive[j];
                            
                            t_UniqueJumpAtom._StartDist = Math.Sqrt((t_UniqueJumpAtom._ZylPositionX + 0.5 * t_UJump._Length) * 
                                (t_UniqueJumpAtom._ZylPositionX + 0.5 * t_UJump._Length) + t_UniqueJumpAtom._ZylPositionY * t_UniqueJumpAtom._ZylPositionY);

                            t_UniqueJumpAtom._TSDist = Math.Sqrt(t_UniqueJumpAtom._ZylPositionX * t_UniqueJumpAtom._ZylPositionX +
                                t_UniqueJumpAtom._ZylPositionY * t_UniqueJumpAtom._ZylPositionY);

                            t_UniqueJumpAtom._DestDist = Math.Sqrt((t_UniqueJumpAtom._ZylPositionX - 0.5 * t_UJump._Length) *
                                (t_UniqueJumpAtom._ZylPositionX - 0.5 * t_UJump._Length) + t_UniqueJumpAtom._ZylPositionY * t_UniqueJumpAtom._ZylPositionY);

                            t_UniqueJumpAtoms.Add(t_UniqueJumpAtom);
                        }
                        t_UJump._UniqueJumpAtoms = t_UniqueJumpAtoms;
                    }
                    t_UJump._SelectedUniqueJumpAtom = null;
                    t_UJumps.Add(t_UJump);
                }
                _UJumps = t_UJumps;
            }
            if (_UJumps.Count != 0)
            {
                _SelectedUJump = _UJumps[0];
            }
            else
            {
                _SelectedUJump = null;
            }

            // Notify for binding updates
            Notify("InteractionShellCount");
            Notify("UJumps");
            Notify("SelectedUJump");

            _ViewModel.IsUniqueJumpsSynchronized = true;
        }

        #endregion Synchronization Methods
    }
}
