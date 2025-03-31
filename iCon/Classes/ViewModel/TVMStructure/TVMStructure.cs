using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Windows;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for Structure tab 
    /// </summary>
    public class TVMStructure: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel; 

        #endregion Fields

        public TVMStructure(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _StructureName = "";
            _LatticeParamChoice = true;
            _LatticeLengthA = 0;
            _LatticeLengthB = 0;
            _LatticeLengthC = 0;
            _LatticeAngleA = 0;
            _LatticeAngleB = 0;
            _LatticeAngleC = 0;
            _LatticeVectorAX = 0;
            _LatticeVectorAY = 0;
            _LatticeVectorAZ = 0;
            _LatticeVectorBX = 0;
            _LatticeVectorBY = 0;
            _LatticeVectorBZ = 0;
            _LatticeVectorCX = 0;
            _LatticeVectorCY = 0;
            _LatticeVectorCZ = 0;
            _MovSymbol = "H";
            _MovName = "";
            _MovCharge = 0;
            _Atoms = new ObservableCollection<TVMStructureAtom>();
            _Dopings = new ObservableCollection<TVMStructureDoping>();
            _SelectedAtom = null;
            _SelectedDoping = null;
        }

        #region Properties

        protected string _StructureName;
        /// <summary> 
        /// Name of the simulated compound 
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid structure name.")]
        [StringLength(100, MinimumLength = 1, ErrorMessage = "Structure name has to contain 1 - 100 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\-\^\(\)\s]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9, _-^() and space.")]
        public string StructureName
        {
            get
            {
                return _StructureName;
            }
            set
            {
                if (ValidateNotify("StructureName", value, ref _StructureName) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected bool _LatticeParamChoice;
        /// <summary> 
        /// true = LatticeParam defined by Length+Angles, false = by Vectors 
        /// </summary>
        public bool LatticeParamChoice
        {
            get
            {
                return _LatticeParamChoice;
            }
            set
            {
                if (value != _LatticeParamChoice)
                {
                    _LatticeParamChoice = value;
                    Notify("LatticeParamChoice");
                }
            }
        }

        protected double _LatticeLengthA;
        /// <summary> 
        /// Lengths of lattice vector a [in Angstrom] 
        /// </summary>
        [DoubleRange(0.0, false, 100.0, true, ErrorMessage="Lattice vector length has to be in the interval (0.0, 100.0].")]
        public double LatticeLengthA
        {
            get
            {
                return _LatticeLengthA;
            }
            set
            {
                if (ValidateNotify("LatticeLengthA", value, ref _LatticeLengthA) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeLengthB;
        /// <summary> 
        /// Lengths of lattice vector b [in Angstrom] 
        /// </summary>
        [DoubleRange(0.0, false, 100.0, true, ErrorMessage = "Lattice vector length has to be in the interval (0.0, 100.0].")]
        public double LatticeLengthB
        {
            get
            {
                return _LatticeLengthB;
            }
            set
            {
                if (ValidateNotify("LatticeLengthB", value, ref _LatticeLengthB) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeLengthC;
        /// <summary> 
        /// Lengths of lattice vector c [in Angstrom] 
        /// </summary>
        [DoubleRange(0.0, false, 100.0, true, ErrorMessage = "Lattice vector length has to be in the interval (0.0, 100.0].")]
        public double LatticeLengthC
        {
            get
            {
                return _LatticeLengthC;
            }
            set
            {
                if (ValidateNotify("LatticeLengthC", value, ref _LatticeLengthC) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeAngleA;
        /// <summary> 
        /// Lattice angle alpha, between vectors b and c [in degrees] 
        /// </summary>
        [DoubleRange(0.0, false, 180.0, false, ErrorMessage = "Lattice vector angle has to be in the interval (0.0, 180.0).")]
        public double LatticeAngleA
        {
            get
            {
                return _LatticeAngleA;
            }
            set
            {
                if (ValidateNotify("LatticeAngleA", value, ref _LatticeAngleA) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeAngleB;
        /// <summary> 
        /// Lattice angle beta, between vectors a and c [in degrees] 
        /// </summary>
        [DoubleRange(0.0, false, 180.0, false, ErrorMessage = "Lattice vector angle has to be in the interval (0.0, 180.0).")]
        public double LatticeAngleB
        {
            get
            {
                return _LatticeAngleB;
            }
            set
            {
                if (ValidateNotify("LatticeAngleB", value, ref _LatticeAngleB) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeAngleC;
        /// <summary> 
        /// Lattice angle gamma, between vectors a and b [in degrees] 
        /// </summary>
        [DoubleRange(0.0, false, 180.0, false, ErrorMessage = "Lattice vector angle has to be in the interval (0.0, 180.0).")]
        public double LatticeAngleC
        {
            get
            {
                return _LatticeAngleC;
            }
            set
            {
                if (ValidateNotify("LatticeAngleC", value, ref _LatticeAngleC) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorAX;
        /// <summary> 
        /// x-coordinate of lattice vector a [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorAX
        {
            get
            {
                return _LatticeVectorAX;
            }
            set
            {
                if (ValidateNotify("LatticeVectorAX", value, ref _LatticeVectorAX) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorAY;
        /// <summary> 
        /// y-coordinate of lattice vector a [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorAY
        {
            get
            {
                return _LatticeVectorAY;
            }
            set
            {
                if (ValidateNotify("LatticeVectorAY", value, ref _LatticeVectorAY) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorAZ;
        /// <summary> 
        /// z-coordinate of lattice vector a [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorAZ
        {
            get
            {
                return _LatticeVectorAZ;
            }
            set
            {
                if (ValidateNotify("LatticeVectorAZ", value, ref _LatticeVectorAZ) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorBX;
        /// <summary> 
        /// x-coordinate of lattice vector b [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorBX
        {
            get
            {
                return _LatticeVectorBX;
            }
            set
            {
                if (ValidateNotify("LatticeVectorBX", value, ref _LatticeVectorBX) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorBY;
        /// <summary> 
        /// y-coordinate of lattice vector b [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorBY
        {
            get
            {
                return _LatticeVectorBY;
            }
            set
            {
                if (ValidateNotify("LatticeVectorBY", value, ref _LatticeVectorBY) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorBZ;
        /// <summary> 
        /// z-coordinate of lattice vector b [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorBZ
        {
            get
            {
                return _LatticeVectorBZ;
            }
            set
            {
                if (ValidateNotify("LatticeVectorBZ", value, ref _LatticeVectorBZ) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorCX;
        /// <summary> 
        /// x-coordinate of lattice vector c [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorCX
        {
            get
            {
                return _LatticeVectorCX;
            }
            set
            {
                if (ValidateNotify("LatticeVectorCX", value, ref _LatticeVectorCX) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorCY;
        /// <summary> 
        /// y-coordinate of lattice vector c [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorCY
        {
            get
            {
                return _LatticeVectorCY;
            }
            set
            {
                if (ValidateNotify("LatticeVectorCY", value, ref _LatticeVectorCY) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _LatticeVectorCZ;
        /// <summary> 
        /// z-coordinate of lattice vector c [in Angstrom] 
        /// </summary>
        [DoubleRange(-100.0, true, 100.0, true, ErrorMessage = "Lattice vector component has to be in the interval [-100.0, 100.0].")]
        public double LatticeVectorCZ
        {
            get
            {
                return _LatticeVectorCZ;
            }
            set
            {
                if (ValidateNotify("LatticeVectorCZ", value, ref _LatticeVectorCZ) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected string _MovSymbol;
        /// <summary> 
        /// Symbol of the moving species 
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid element symbol.")]
        [StringLength(2, MinimumLength = 1, ErrorMessage = "Element symbol has to contain 1 or 2 characters.")]
        [RegularExpression(@"^[A-Za-z]*$", ErrorMessage = "Allowed characters: a-z, A-Z.")]
        public string MovSymbol
        {
            get
            {
                return _MovSymbol;
            }
            set
            {
                if (ValidateNotify("MovSymbol", value, ref _MovSymbol) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected string _MovName;
        /// <summary> 
        /// Optional name of the moving species 
        /// </summary>
        [StringLength(20, ErrorMessage = "Optional name has to contain 0 - 20 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\(\)]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9 and _().")]
        public string MovName
        {
            get
            {
                return _MovName;
            }
            set
            {
                if (ValidateNotify("MovName", value, ref _MovName) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected double _MovCharge;
        /// <summary> 
        /// Charge [in e] of the moving species 
        /// </summary>
        [DoubleRange(-1000.0, true, 1000.0, true, ErrorMessage = "Charge has to be in the interval [-1000.0, 1000.0].")]
        public double MovCharge
        {
            get
            {
                return _MovCharge;
            }
            set
            {
                if (ValidateNotify("MovCharge", value, ref _MovCharge) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        protected ObservableCollection<TVMStructureAtom> _Atoms;
        /// <summary> 
        /// List of all atoms in the unit cell 
        /// </summary>
        public ObservableCollection<TVMStructureAtom> Atoms
        {
            get
            {
                return _Atoms;
            }
            set
            {
                if (value != _Atoms)
                {
                    _Atoms = value;
                    _ViewModel.IsStructureSynchronized = false;
                    Notify("Atoms");
                }
            }
        }

        protected ObservableCollection<TVMStructureDoping> _Dopings;
        /// <summary> 
        /// List of all doped element + dopand element pairs 
        /// </summary>
        public ObservableCollection<TVMStructureDoping> Dopings
        {
            get
            {
                return _Dopings;
            }
            set
            {
                if (value != _Dopings)
                {
                    _Dopings = value;
                    _ViewModel.IsStructureSynchronized = false;
                    Notify("Dopings");
                }
            }
        }

        protected TVMStructureAtom _SelectedAtom;
        /// <summary> 
        /// Currently selected atom 
        /// </summary>
        public TVMStructureAtom SelectedAtom
        {
            get
            {
                return _SelectedAtom;
            }
            set
            {
                if (value != _SelectedAtom)
                {
                    _SelectedAtom = value;
                    Notify("SelectedAtom");
                    Notify("IsAtomSelected");
                }
            }
        }

        protected TVMStructureDoping _SelectedDoping;
        /// <summary> 
        /// Currently selected doping 
        /// </summary>
        public TVMStructureDoping SelectedDoping
        {
            get
            {
                return _SelectedDoping;
            }
            set
            {
                if (value != _SelectedDoping)
                {
                    _SelectedDoping = value;
                    Notify("SelectedDoping");
                    Notify("IsDopingSelected");
                }
            }
        }

        /// <summary>
        /// Flag, true = an atom is selected
        /// </summary>
        public bool IsAtomSelected
        {
            get
            {
                return (_SelectedAtom != null);
            }
        }

        /// <summary>
        /// Flag, true = a doping is selected
        /// </summary>
        public bool IsDopingSelected
        {
            get
            {
                return (_SelectedDoping != null);
            }
        }

        /// <summary>
        /// Flag, true = there are atoms in the atom list
        /// </summary>
        public bool HasAtoms
        {
            get
            {
                return (_Atoms.Count != 0);
            }
        }

        /// <summary>
        /// Flag, true = there are dopings in the doping list
        /// </summary>
        public bool HasDopings
        {
            get
            {
                return (_Dopings.Count != 0);
            }
        }

        #endregion Properties

        #region Methods

        /// <summary> 
        /// Add a new atom 
        /// </summary>
        public void AddAtom()
        {
            if (_Atoms.Count >= ConstantsClass.MAX_ATOM_COUNT)
            {
                MessageBox.Show("Maximum number of atoms (" + ConstantsClass.MAX_ATOM_COUNT.ToString() + ") reached.", "Information", MessageBoxButton.OK);
                return;
            }

            Atoms.Add(new TVMStructureAtom(_ViewModel));
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasAtoms");
        }

        /// <summary> 
        /// Add a dublicate of the selected atom 
        /// </summary>
        public void DublicateAtom()
        {
            if (_Atoms.Count >= ConstantsClass.MAX_ATOM_COUNT)
            {
                MessageBox.Show("Maximum number of atoms (" + ConstantsClass.MAX_ATOM_COUNT.ToString() + ") reached.", "Information", MessageBoxButton.OK);
                return;
            }

            if (_SelectedAtom == null) return;

            Atoms.Add(_SelectedAtom.GetCopy());
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasAtoms");
        }

        /// <summary> 
        /// Selects the element of the selected atom as moving species 
        /// </summary>
        public void SelectAsMovingSpecies()
        {
            if (_SelectedAtom == null) return;

            MovSymbol = _SelectedAtom.Symbol;
            MovName = _SelectedAtom.Name;
            MovCharge = _SelectedAtom.Charge;
        }

        /// <summary> 
        /// Delete the selected atom 
        /// </summary>
        public void DeleteSelectedAtom()
        {
            if (_SelectedAtom == null) return;

            Atoms.Remove(_SelectedAtom);
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasAtoms");
        }

        /// <summary> 
        /// Delete all atoms 
        /// </summary>
        public void DeleteAllAtoms()
        {
            if (_Atoms.Count == 0) return;
            
            Atoms.Clear();
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasAtoms");
        }

        /// <summary> 
        /// Add a new doping 
        /// </summary>
        public void AddDoping()
        {
            if (_Dopings.Count >= ConstantsClass.MAX_DOPING_COUNT)
            {
                MessageBox.Show("Maximum number of dopings (" + ConstantsClass.MAX_DOPING_COUNT.ToString() + ") reached.", "Information", MessageBoxButton.OK);
                return;
            }

            Dopings.Add(new TVMStructureDoping(_ViewModel));
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasDopings");
        }

        /// <summary> 
        /// Add a dublicate of the selected doping 
        /// </summary>
        public void DublicateDoping()
        {
            if (_Dopings.Count >= ConstantsClass.MAX_DOPING_COUNT)
            {
                MessageBox.Show("Maximum number of dopings (" + ConstantsClass.MAX_DOPING_COUNT.ToString() + ") reached.", "Information", MessageBoxButton.OK);
                return;
            }

            if (_SelectedDoping == null) return;

            Dopings.Add(_SelectedDoping.GetCopy());
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasDopings");
        }

        /// <summary> 
        /// Delete the selected doping 
        /// </summary>
        public void DeleteSelectedDoping()
        {
            if (_SelectedDoping == null) return;
            
            Dopings.Remove(_SelectedDoping);
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasDopings");
        }

        /// <summary> 
        /// Delete all dopings 
        /// </summary>
        public void DeleteAllDopings()
        {
            if (_Dopings.Count == 0) return;
            
            Dopings.Clear();
            _ViewModel.IsStructureSynchronized = false;
            Notify("HasDopings");
        }

        /// <summary>
        /// Validate this object and all sub-objects
        /// </summary>
        public bool ValidateFullObject()
        {
            if (ValidateObject() == false) return false;
            if (_Atoms.Count > 0)
            {
                for (int i = 0; i < _Atoms.Count; i++)
                {
                    if (_Atoms[i].ValidateObject() == false) return false;
                }
            }
            if (_Dopings.Count > 0)
            {
                for (int i = 0; i < _Dopings.Count; i++)
                {
                    if (_Dopings[i].ValidateObject() == false) return false;
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
            BWorker.ReportProgress(0, "Specifying elements ... ");

            // Send elements
            if (_Atoms.Count < 1)
            {
                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid number of atoms\n(at least one atom required)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                return;
            }
            ErrorCode = MCDLL.ClearElements();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Clearing elements failed (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }
            for (int i = 0; i < _Atoms.Count; ++i)
            {
                ErrorCode = MCDLL.AddElement(_Atoms[i]._Symbol, _Atoms[i]._Name, _Atoms[i]._Charge);
                switch (ErrorCode)
                {
                    case ConstantsClass.KMCERR_OK:
                        break;
                    case ConstantsClass.KMCERR_INVALID_INPUT:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Element\n(see console for details)\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    case ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Maximum Input Reached\n(see console for details)\n",
                            ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED, false);
                        return;
                    default:
                        throw new ApplicationException("Invalid element parameters (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                }
            }
            if (_Dopings.Count != 0)
            {
                for (int i = 0; i < _Dopings.Count; ++i)
                {
                    ErrorCode = MCDLL.AddElement(_Dopings[i]._dopSymbol, _Dopings[i]._dopName, _Dopings[i]._dopCharge);
                    switch (ErrorCode)
                    {
                        case ConstantsClass.KMCERR_OK:
                            break;
                        case ConstantsClass.KMCERR_INVALID_INPUT:
                            e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Element\n(see console for details)\n",
                                ConstantsClass.KMCERR_INVALID_INPUT, false);
                            return;
                        case ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED:
                            e.Result = new BWorkerResultMessage("Invalid Input", "Maximum Input Reached\n(see console for details)\n",
                                ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED, false);
                            return;
                        default:
                            throw new ApplicationException("Invalid dopand element parameters (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                    }
                }
            }
            ErrorCode = MCDLL.SetMovingElement(_MovSymbol, _MovName, _MovCharge);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Moving Species\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                case ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Maximum Input Reached\n(see console for details)\n",
                        ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED, false);
                    return;
                default:
                    throw new ApplicationException("Invalid moving species (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }
            ErrorCode = MCDLL.MakeElements();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Validating elements failed (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            BWorker.ReportProgress(20, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(20, "Validating structure name ... ");

            // Send structure name
            ErrorCode = MCDLL.ClearStructure();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    throw new ApplicationException("Clearing structure failed (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }
            ErrorCode = MCDLL.SetStructureName(_StructureName);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Structure Name\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid structure name (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            BWorker.ReportProgress(30, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(30, "Setting lattice parameters ... ");

            // Send lattice parameters
            if (_LatticeParamChoice == true)
            {
                ErrorCode = MCDLL.SetBasisByLatticeParam(_LatticeLengthA, _LatticeLengthB, _LatticeLengthC, _LatticeAngleA, _LatticeAngleB, _LatticeAngleC);
            }
            else
            {
                ErrorCode = MCDLL.SetBasisByLatticeVectors(_LatticeVectorAX, _LatticeVectorAY, _LatticeVectorAZ, _LatticeVectorBX,
                    _LatticeVectorBY, _LatticeVectorBZ, _LatticeVectorCX, _LatticeVectorCY, _LatticeVectorCZ);
            }
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Lattice Definition\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid lattice parameters (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            BWorker.ReportProgress(40, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(40, "Adding atoms ... ");

            // Add atoms
            for (int i = 0; i < _Atoms.Count; ++i)
            {
                ErrorCode = MCDLL.AddAtom(_Atoms[i]._PositionX, _Atoms[i]._PositionY, _Atoms[i]._PositionZ,
                    _Atoms[i]._Symbol, _Atoms[i]._Name, _Atoms[i]._Charge);
                switch (ErrorCode)
                {
                    case ConstantsClass.KMCERR_OK:
                        break;
                    case ConstantsClass.KMCERR_INVALID_INPUT:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Atom\n(see console for details)\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    case ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Maximum Input Reached\n(see console for details)\n",
                            ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED, false);
                        return;
                    case ConstantsClass.KMCERR_INPUT_ALREADY_EXISTS:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Atom Dublicate\n(see console for details)\n",
                            ConstantsClass.KMCERR_INPUT_ALREADY_EXISTS, false);
                        return;
                    default:
                        throw new ApplicationException("Invalid atom parameters (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                }
            }

            BWorker.ReportProgress(60, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(60, "Setting dopings ... ");

            // Add dopings
            for (int i = 0; i < _Dopings.Count; ++i)
            {
                ErrorCode = MCDLL.AddDoping(_Dopings[i]._origSymbol, _Dopings[i]._origName, _Dopings[i]._origCharge,
                    _Dopings[i]._dopSymbol, _Dopings[i]._dopName, _Dopings[i]._dopCharge, _Dopings[i]._VacDopRatio);
                switch (ErrorCode)
                {
                    case ConstantsClass.KMCERR_OK:
                        break;
                    case ConstantsClass.KMCERR_INVALID_INPUT:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Doping\n(see console for details)\n",
                            ConstantsClass.KMCERR_INVALID_INPUT, false);
                        return;
                    case ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Maximum Input Reached\n(see console for details)\n",
                            ConstantsClass.KMCERR_MAXIMUM_INPUT_REACHED, false);
                        return;
                    case ConstantsClass.KMCERR_NO_DOPED_ATOM_FOUND:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Doping\n(see console for details)\n",
                            ConstantsClass.KMCERR_NO_DOPED_ATOM_FOUND, false);
                        return;
                    case ConstantsClass.KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Doping\n(see console for details)\n",
                            ConstantsClass.KMCERR_VACMOV_NOT_ALLOWED_AS_DOPING, false);
                        return;
                    case ConstantsClass.KMCERR_INPUT_ALREADY_EXISTS:
                        e.Result = new BWorkerResultMessage("Invalid Input", "Doping Dublicate\n(see console for details)\n",
                            ConstantsClass.KMCERR_INPUT_ALREADY_EXISTS, false);
                        return;
                    default:
                        throw new ApplicationException("Invalid doping parameters (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
                }
            }

            BWorker.ReportProgress(80, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(80, "Validating structure ... ");

            ErrorCode = MCDLL.MakeStructure();
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid Structure\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid structure (TVMStructure.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
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
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMStructure.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMStructure.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMStructure.GetData)");

            // Transfer data (skip validation by using fields)
            _StructureName = MCJob.Structure.Name;
            _LatticeLengthA = MCJob.Structure.BasisA.Length;
            _LatticeLengthB = MCJob.Structure.BasisB.Length;
            _LatticeLengthC = MCJob.Structure.BasisC.Length;
            if ((MCJob.Structure.BasisA.Length == 0) || (MCJob.Structure.BasisB.Length == 0) || (MCJob.Structure.BasisC.Length == 0))
            {
                _LatticeAngleA = 0;
                _LatticeAngleB = 0;
                _LatticeAngleC = 0;
            }
            else
            {
                _LatticeAngleA = Vector3D.AngleBetween(MCJob.Structure.BasisB, MCJob.Structure.BasisC);
                _LatticeAngleB = Vector3D.AngleBetween(MCJob.Structure.BasisA, MCJob.Structure.BasisC);
                _LatticeAngleC = Vector3D.AngleBetween(MCJob.Structure.BasisA, MCJob.Structure.BasisB);
            }
            _LatticeVectorAX = MCJob.Structure.BasisA.X;
            _LatticeVectorAY = MCJob.Structure.BasisA.Y;
            _LatticeVectorAZ = MCJob.Structure.BasisA.Z;
            _LatticeVectorBX = MCJob.Structure.BasisB.X;
            _LatticeVectorBY = MCJob.Structure.BasisB.Y;
            _LatticeVectorBZ = MCJob.Structure.BasisB.Z;
            _LatticeVectorCX = MCJob.Structure.BasisC.X;
            _LatticeVectorCY = MCJob.Structure.BasisC.Y;
            _LatticeVectorCZ = MCJob.Structure.BasisC.Z;
            _MovSymbol = MCJob.Elements.MovSymbol;
            _MovName = MCJob.Elements.MovName;
            _MovCharge = MCJob.Elements.MovCharge;
            _Atoms.Clear();
            if (MCJob.Structure.ElemID.Count != 0)
            {
                TVMStructureAtom t_Atom = null;
                ObservableCollection<TVMStructureAtom> t_Atoms = new ObservableCollection<TVMStructureAtom>();
                for (int i = 0; i < MCJob.Structure.ElemID.Count; i++)
                {
                    t_Atom = new TVMStructureAtom(_ViewModel);
                    t_Atom._Symbol = MCJob.Elements.ElemSymbol[MCJob.Structure.ElemID[i]];
                    t_Atom._Name = MCJob.Elements.ElemName[MCJob.Structure.ElemID[i]];
                    t_Atom._Charge = MCJob.Elements.ElemCharge[MCJob.Structure.ElemID[i]];
                    t_Atom._PositionX = MCJob.Structure.Coord[i].X;
                    t_Atom._PositionY = MCJob.Structure.Coord[i].Y;
                    t_Atom._PositionZ = MCJob.Structure.Coord[i].Z;
                    t_Atoms.Add(t_Atom);
                }
                _Atoms = t_Atoms;
            }
            _Dopings.Clear();
            if (MCJob.Structure.DopedID.Count != 0)
            {
                TVMStructureDoping t_Doping = null;
                ObservableCollection<TVMStructureDoping> t_Dopings = new ObservableCollection<TVMStructureDoping>();
                for (int i = 0; i < MCJob.Structure.DopedID.Count; i++)
                {
                    t_Doping = new TVMStructureDoping(_ViewModel);
                    t_Doping._origSymbol = MCJob.Elements.ElemSymbol[MCJob.Structure.DopedID[i]];
                    t_Doping._origName = MCJob.Elements.ElemName[MCJob.Structure.DopedID[i]];
                    t_Doping._origCharge = MCJob.Elements.ElemCharge[MCJob.Structure.DopedID[i]];
                    t_Doping._dopSymbol = MCJob.Elements.ElemSymbol[MCJob.Structure.DopandID[i]];
                    t_Doping._dopName = MCJob.Elements.ElemName[MCJob.Structure.DopandID[i]];
                    t_Doping._dopCharge = MCJob.Elements.ElemCharge[MCJob.Structure.DopandID[i]];
                    t_Doping._VacDopRatio = MCJob.Structure.VacDopRatio[i];
                    t_Dopings.Add(t_Doping);
                }
                _Dopings = t_Dopings;
            }
            _SelectedAtom = null;
            _SelectedDoping = null;

            // Notify for binding updates
            Notify("StructureName");
            Notify("LatticeParamChoice");
            Notify("LatticeLengthA");
            Notify("LatticeLengthB");
            Notify("LatticeLengthC");
            Notify("LatticeAngleA");
            Notify("LatticeAngleB");
            Notify("LatticeAngleC");
            Notify("LatticeVectorAX");
            Notify("LatticeVectorAY");
            Notify("LatticeVectorAZ");
            Notify("LatticeVectorBX");
            Notify("LatticeVectorBY");
            Notify("LatticeVectorBZ");
            Notify("LatticeVectorCX");
            Notify("LatticeVectorCY");
            Notify("LatticeVectorCZ");
            Notify("MovSymbol");
            Notify("MovName");
            Notify("MovCharge");
            Notify("Atoms");
            Notify("Dopings");
            Notify("SelectedAtom");
            Notify("SelectedDoping");
            Notify("IsAtomSelected");
            Notify("IsDopingSelected");
            Notify("HasAtoms");
            Notify("HasDopings");

            _ViewModel.IsStructureSynchronized = true;
        }

        #endregion Synchronization Methods
    }
}
