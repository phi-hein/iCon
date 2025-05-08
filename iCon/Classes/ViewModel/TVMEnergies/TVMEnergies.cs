using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for Energies tab 
    /// </summary>
    public class TVMEnergies: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMEnergies(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _EnergiesJumps = new ObservableCollection<TVMEnergiesJump>();
            _ElemHeaders = new ObservableCollection<TVMEnergiesElemHeader>();
            _SelectedEnergiesJump = null;
        }

        #region Properties

        protected ObservableCollection<TVMEnergiesJump> _EnergiesJumps;
        /// <summary> 
        /// List of the unique jumps 
        /// </summary>
        public ObservableCollection<TVMEnergiesJump> EnergiesJumps
        {
            get
            {
                return _EnergiesJumps;
            }
            set
            {
                if (value != _EnergiesJumps)
                {
                    _EnergiesJumps = value;
                    Notify("EnergiesJumps");
                }
            }
        }

        protected ObservableCollection<TVMEnergiesElemHeader> _ElemHeaders;
        /// <summary> 
        /// List of the elements (for additive atom energy input) 
        /// </summary>
        public ObservableCollection<TVMEnergiesElemHeader> ElemHeaders
        {
            get
            {
                return _ElemHeaders;
            }
            set
            {
                if (value != _ElemHeaders)
                {
                    _ElemHeaders = value;
                    Notify("ElemHeaders");
                }
            }
        }

        protected TVMEnergiesJump _SelectedEnergiesJump;
        /// <summary> 
        /// Selected jump 
        /// </summary>
        public TVMEnergiesJump SelectedEnergiesJump
        {
            get
            {
                return _SelectedEnergiesJump;
            }
            set
            {
                if (value != _SelectedEnergiesJump)
                {
                    _SelectedEnergiesJump = value;
                    Notify("SelectedEnergiesJump");
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
            if (_EnergiesJumps.Count > 0)
            {
                for (int i = 0; i < _EnergiesJumps.Count; i++)
                {
                    if (_EnergiesJumps[i].ValidateFullObject() == false) return false;
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
            int ErrorCode = Constants.KMCERR_OK;
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Validating code energies ... ");

            if (_EnergiesJumps.Count < 1) throw new ApplicationException("No active unique jumps (TVMEnergies.ApplyData)");

            // Validate code energies
            for (int i = 0; i < _EnergiesJumps.Count; i++)
            {
                if (_EnergiesJumps[i]._UniqueCodes.Count != 0)
                {
                    for (int j = 0; j < _EnergiesJumps[i]._UniqueCodes.Count; j++)
                    {
                        ErrorCode = MCDLL.SetCodeEnergy(_EnergiesJumps[i]._JumpID, _EnergiesJumps[i]._UniqueCodes[j]._CodeID,
                            _EnergiesJumps[i]._UniqueCodes[j]._Energy);
                        switch (ErrorCode)
                        {
                            case Constants.KMCERR_OK:
                                break;
                            case Constants.KMCERR_INVALID_INPUT:
                                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Energy\n(see console for details)\n",
                                    Constants.KMCERR_INVALID_INPUT, false);
                                return;
                            default:
                                throw new ApplicationException("Setting code energy failed (TVMEnergies.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                        }
                    }
                }
            }

            BWorker.ReportProgress(40, "OK\n");
            System.Threading.Thread.Sleep(Constants.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(40, "Validating additive energies ... ");

            // Validate wwatoms energies
            for (int i = 0; i < _EnergiesJumps.Count; i++)
            {
                if (_EnergiesJumps[i]._WWAtoms.Count != 0)
                {
                    for (int j = 0; j < _EnergiesJumps[i]._WWAtoms.Count; j++)
                    {
                        if (_EnergiesJumps[i]._WWAtoms[j]._Energies.Count != 0)
                        {
                            for (int k = 0; k < _EnergiesJumps[i]._WWAtoms[j]._Energies.Count; k++)
                            {
                                if (_EnergiesJumps[i]._WWAtoms[j]._Energies[k]._IsEditable == true)
                                {
                                    ErrorCode = MCDLL.SetInteractionEnergyByCoord(_EnergiesJumps[i]._JumpID, _EnergiesJumps[i]._WWAtoms[j]._CoordID,
                                    _EnergiesJumps[i]._WWAtoms[j]._Energies[k]._ElemID, _EnergiesJumps[i]._WWAtoms[j]._Energies[k]._Energy);
                                    switch (ErrorCode)
                                    {
                                        case Constants.KMCERR_OK:
                                            break;
                                        case Constants.KMCERR_INVALID_INPUT:
                                            e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Energy\n(see console for details)\n",
                                                Constants.KMCERR_INVALID_INPUT, false);
                                            return;
                                        default:
                                            throw new ApplicationException("Setting interaction energy failed (TVMEnergies.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(80, "OK\nLoading ... ");
            e.Result = new TMCJob(MCDLL);
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(Constants.THREAD_FINISH_DELAY);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data from _ViewModel
        /// </summary>
        public void GetData()
        {
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMEnergies.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMEnergies.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMEnergies.GetData)");

            // Transfer data (skip validation by using fields)
            _ViewModel.WWEnergyColumnNumber = 0;
            _ElemHeaders.Clear();
            if (MCJob.Elements.ElemSymbol.Count != 0)
            {
                TVMEnergiesElemHeader t_Header = null;
                ObservableCollection<TVMEnergiesElemHeader> t_Headers = new ObservableCollection<TVMEnergiesElemHeader>();
                for (int i = 0; i < MCJob.Elements.ElemSymbol.Count; i++)
                {
                    t_Header = new TVMEnergiesElemHeader(_ViewModel);
                    t_Header._ElemID = i;
                    if (MCJob.Elements.ElemName[i] != "")
                    {
                        t_Header._ElemStr = MCJob.Elements.ElemSymbol[i] + " [" + MCJob.Elements.ElemName[i] + "," + 
                            string.Format(Constants.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[i]) + "]";
                    }
                    else
                    {
                        t_Header._ElemStr = MCJob.Elements.ElemSymbol[i] + " [" + 
                            string.Format(Constants.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[i]) + "]";
                    }

                    t_Headers.Add(t_Header);
                }
                _ElemHeaders = t_Headers;
                _ViewModel.WWEnergyColumnNumber = _ElemHeaders.Count;
            }
            _EnergiesJumps.Clear();
            if (MCJob.UJumps.UJumps.Count != 0)
            {
                TVMEnergiesJump t_Jump = null;
                ObservableCollection<TVMEnergiesJump> t_Jumps = new ObservableCollection<TVMEnergiesJump>();
                for (int i = 0; i < MCJob.UJumps.UJumps.Count; i++)
                {
                    if (MCJob.UJumps.UJumps[i].JumpActive == false) continue;
                    t_Jump = new TVMEnergiesJump(_ViewModel);
                    t_Jump._JumpID = i;
                    t_Jump._Length = MCJob.UJumps.UJumps[i].Length;
                    t_Jump._UniqueCodes.Clear();
                    if (MCJob.UJumps.UJumps[i].UniqueCodes.Count != 0)
                    {
                        List<List<int>> i_Code = null;
                        TVMEnergiesJumpUCode t_Code = null;
                        ObservableCollection<TVMEnergiesJumpUCode> t_Codes = new ObservableCollection<TVMEnergiesJumpUCode>();
                        for (int j = 0; j < MCJob.UJumps.UJumps[i].UniqueCodes.Count; j++)
                        {
                            t_Code = new TVMEnergiesJumpUCode(_ViewModel);
                            t_Code._CodeID = j;
                            t_Code._Energy = MCJob.UJumps.UJumps[i].UniqueEnergies[j];
                            i_Code = TUniqueJump.SplitCode(MCJob.UJumps.UJumps[i].UniqueCodes[j], MCJob.Elements.ElemSymbol.Count - 1);
                            t_Code._CodeStr = "";
                            if (i_Code.Count != 0)
                            {
                                for (int k = 0; k < i_Code.Count; k++)
                                {
                                    if (i_Code[k].Count != 0)
                                    {
                                        if (k != 0) t_Code._CodeStr += "; ";
                                        for (int l = 0; l < i_Code[k].Count; l++)
                                        {
                                            if (l != 0) t_Code._CodeStr += " , ";
                                            if (MCJob.Elements.ElemName[i_Code[k][l]] != "")
                                            {
                                                t_Code._CodeStr += MCJob.Elements.ElemSymbol[i_Code[k][l]] + " [" + 
                                                    MCJob.Elements.ElemName[i_Code[k][l]] + "," + 
                                                    string.Format(Constants.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[i_Code[k][l]]) + "]";
                                            }
                                            else
                                            {
                                                t_Code._CodeStr += MCJob.Elements.ElemSymbol[i_Code[k][l]] + " [" + 
                                                    string.Format(Constants.SC_KMC_SHORT_CHARGE_DOUBLEFORMAT, MCJob.Elements.ElemCharge[i_Code[k][l]]) + "]";
                                            }
                                        }
                                    }
                                }
                            }

                            t_Codes.Add(t_Code);
                        }
                        t_Jump._UniqueCodes = t_Codes;
                    }
                    t_Jump._SelectedUniqueCode = null;
                    t_Jump._WWAtoms.Clear();
                    if (MCJob.UJumps.UJumps[i].InteractionMap.Count != 0)
                    {
                        TVMEnergiesJumpWWAtom t_WWAtom = null;
                        ObservableCollection<TVMEnergiesJumpWWAtom> t_WWAtoms = new ObservableCollection<TVMEnergiesJumpWWAtom>();
                        for (int j = 0; j < MCJob.UJumps.UJumps[i].InteractionMap.Count; j++)
                        {
                            t_WWAtom = new TVMEnergiesJumpWWAtom(_ViewModel);
                            t_WWAtom._WWAtomID = j;
                            t_WWAtom._CoordID = MCJob.UJumps.UJumps[i].InteractionCoordID[j];
                            t_WWAtom._WWID = MCJob.UJumps.UJumps[i].InteractionMap[j];

                            t_WWAtom._StartDist = Math.Sqrt((MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].X + 0.5 * t_Jump._Length) *
                                (MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].X + 0.5 * t_Jump._Length) +
                                MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].Y * MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].Y);

                            t_WWAtom._CenterDist = Math.Sqrt(MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].X * MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].X +
                                MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].Y * MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].Y);

                            t_WWAtom._DestDist = Math.Sqrt((MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].X - 0.5 * t_Jump._Length) *
                                (MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].X - 0.5 * t_Jump._Length) +
                                MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].Y * MCJob.UJumps.UJumps[i].Coord[t_WWAtom._CoordID].Y);

                            t_WWAtom._Energies.Clear();
                            if (MCJob.Elements.ElemSymbol.Count != 0)
                            {
                                TVMEnergiesJumpWWAtomEnergy t_Energy = null;
                                ObservableCollection<TVMEnergiesJumpWWAtomEnergy> t_Energies = new ObservableCollection<TVMEnergiesJumpWWAtomEnergy>();
                                for (int k = 0; k < MCJob.Elements.ElemSymbol.Count; k++)
                                {
                                    t_Energy = new TVMEnergiesJumpWWAtomEnergy(_ViewModel);
                                    t_Energy._ElemID = k;
                                    t_Energy._Energy = 0;
                                    t_Energy._IsEditable = false;

                                    t_Energies.Add(t_Energy);
                                }
                                if (MCJob.UJumps.UJumps[i].InteractionEnergies[j].Count != 0)
                                {
                                    for (int k = 0; k < MCJob.UJumps.UJumps[i].InteractionEnergies[j].Count; k++)
                                    {
                                        t_Energies[MCJob.UJumps.UJumps[i].InteractionElemIDs[j][k]]._Energy = MCJob.UJumps.UJumps[i].InteractionEnergies[j][k];
                                        t_Energies[MCJob.UJumps.UJumps[i].InteractionElemIDs[j][k]]._IsEditable = true;
                                    }
                                }
                                t_WWAtom._Energies = t_Energies;
                            } 

                            t_WWAtoms.Add(t_WWAtom);
                        }
                        t_Jump._WWAtoms = t_WWAtoms;
                    }
                    t_Jump._SelectedWWAtom = null;
                    t_Jumps.Add(t_Jump);
                }
                _EnergiesJumps = t_Jumps;
            }
            if (_EnergiesJumps.Count != 0)
            {
                _SelectedEnergiesJump = _EnergiesJumps[0];
            }
            else
            {
                _SelectedEnergiesJump = null;
            }

            // Notify for binding updates
            Notify("EnergiesJumps");
            Notify("ElemHeaders");
            Notify("SelectedEnergiesJump");

            _ViewModel.IsEnergiesSynchronized = true;
        }

        #endregion Synchronization Methods
    }
}
