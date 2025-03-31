using System.ComponentModel.DataAnnotations;

namespace iCon_General
{
    /// <summary>
    /// Holds the information for an atom in the Structure tab
    /// </summary>
    public class TVMStructureAtom : ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMStructureAtom(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _Symbol = "H";
            _Name = "";
            _Charge = 0;
            _PositionX = 0;
            _PositionY = 0;
            _PositionZ = 0;
        }

        #region Properties

        public string _Symbol;
        /// <summary> 
        /// Elementary Symbol of the PSE 
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid element symbol.")]
        [StringLength(2, MinimumLength = 1, ErrorMessage = "Element symbol has to contain 1 or 2 characters.")]
        [RegularExpression(@"^[A-Za-z]*$", ErrorMessage = "Allowed characters: a-z, A-Z.")]
        public string Symbol
        {
            get
            {
                return _Symbol;
            }
            set
            {
                if (ValidateNotify("Symbol", value, ref _Symbol) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public string _Name;
        /// <summary> 
        /// Optional Name of the Atom 
        /// </summary>
        [StringLength(20, ErrorMessage = "Optional name has to contain 0 - 20 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\(\)]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9 and _().")]
        public string Name
        {
            get
            {
                return _Name;
            }
            set
            {
                if (ValidateNotify("Name", value, ref _Name) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _Charge;
        /// <summary> 
        /// Charge of the Atom [in e] 
        /// </summary>
        [DoubleRange(-1000.0, true, 1000.0, true, ErrorMessage = "Charge has to be in the interval [-1000.0, 1000.0].")]
        public double Charge
        {
            get
            {
                return _Charge;
            }
            set
            {
                if (ValidateNotify("Charge", value, ref _Charge) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _PositionX;
        /// <summary> 
        /// x-coordinate of the position of the atom (projection on unit cell vector a) -> out of [0,1) 
        /// </summary>
        [DoubleRange(0.0, true, 1.0, false, ErrorMessage = "Relative position component has to be in the interval [0.0, 1.0).")]
        public double PositionX
        {
            get
            {
                return _PositionX;
            }
            set
            {
                if (ValidateNotify("PositionX", value, ref _PositionX) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _PositionY;
        /// <summary> 
        /// y-coordinate of the position of the atom (projection on unit cell vector b) -> out of [0,1) 
        /// </summary>
        [DoubleRange(0.0, true, 1.0, false, ErrorMessage = "Relative position component has to be in the interval [0.0, 1.0).")]
        public double PositionY
        {
            get
            {
                return _PositionY;
            }
            set
            {
                if (ValidateNotify("PositionY", value, ref _PositionY) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _PositionZ;
        /// <summary> 
        /// z-coordinate of the position of the atom (projection on unit cell vector c) -> out of [0,1) 
        /// </summary>
        [DoubleRange(0.0, true, 1.0, false, ErrorMessage = "Relative position component has to be in the interval [0.0, 1.0).")]
        public double PositionZ
        {
            get
            {
                return _PositionZ;
            }
            set
            {
                if (ValidateNotify("PositionZ", value, ref _PositionZ) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Create a deep copy (except viewmodel reference) of this object
        /// </summary>
        public TVMStructureAtom GetCopy()
        {
            // Create shallow copy (all fields are copied, references still point so the same objects)
            TVMStructureAtom t_copy = (TVMStructureAtom)this.MemberwiseClone();

            return t_copy;
        }

        #endregion Methods
    }
}
