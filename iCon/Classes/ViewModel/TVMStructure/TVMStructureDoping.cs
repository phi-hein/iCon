using System.ComponentModel.DataAnnotations;

namespace iCon_General
{
    /// <summary>
    /// Holds the information for a doping in the Structure tab
    /// </summary>
    public class TVMStructureDoping: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMStructureDoping(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _origSymbol = "H";
            _origName = "";
            _origCharge = 0;
            _dopSymbol = "H";
            _dopName = "";
            _dopCharge = 0;
            _VacDopRatio = 0;
        }

        #region Properties

        public string _origSymbol;
        /// <summary> 
        /// Symbol of the doped element 
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid element symbol.")]
        [StringLength(2, MinimumLength = 1, ErrorMessage = "Element symbol has to contain 1 or 2 characters.")]
        [RegularExpression(@"^[A-Za-z]*$", ErrorMessage = "Allowed characters: a-z, A-Z.")]
        public string origSymbol
        {
            get
            {
                return _origSymbol;
            }
            set
            {
                if (ValidateNotify("origSymbol", value, ref _origSymbol) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public string _origName;
        /// <summary> 
        /// Optional name of the doped element 
        /// </summary>
        [StringLength(20, ErrorMessage = "Optional name has to contain 0 - 20 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\(\)]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9 and _().")]
        public string origName
        {
            get
            {
                return _origName;
            }
            set
            {
                if (ValidateNotify("origName", value, ref _origName) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _origCharge;
        /// <summary> 
        /// Charge [in e] of the doped element 
        /// </summary>
        [DoubleRange(-1000.0, true, 1000.0, true, ErrorMessage = "Charge has to be in the interval [-1000.0, 1000.0].")]
        public double origCharge
        {
            get
            {
                return _origCharge;
            }
            set
            {
                if (ValidateNotify("origCharge", value, ref _origCharge) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public string _dopSymbol;
        /// <summary> 
        /// Symbol of the dopand element 
        /// </summary>
        [Required(AllowEmptyStrings = false, ErrorMessage = "Enter valid element symbol.")]
        [StringLength(2, MinimumLength = 1, ErrorMessage = "Element symbol has to contain 1 or 2 characters.")]
        [RegularExpression(@"^[A-Za-z]*$", ErrorMessage = "Allowed characters: a-z, A-Z.")]
        public string dopSymbol
        {
            get
            {
                return _dopSymbol;
            }
            set
            {
                if (ValidateNotify("dopSymbol", value, ref _dopSymbol) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public string _dopName;
        /// <summary> 
        /// Optional name of the dopand element 
        /// </summary>
        [StringLength(20, ErrorMessage = "Optional name has to contain 0 - 20 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\(\)]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9 and _().")]
        public string dopName
        {
            get
            {
                return _dopName;
            }
            set
            {
                if (ValidateNotify("dopName", value, ref _dopName) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _dopCharge;
        /// <summary> 
        /// Charge [in e] of the dopand element 
        /// </summary>
        [DoubleRange(-1000.0, true, 1000.0, true, ErrorMessage = "Charge has to be in the interval [-1000.0, 1000.0].")]
        public double dopCharge
        {
            get
            {
                return _dopCharge;
            }
            set
            {
                if (ValidateNotify("dopCharge", value, ref _dopCharge) == true)
                {
                    _ViewModel.IsStructureSynchronized = false;
                }
            }
        }

        public double _VacDopRatio;
        /// <summary> 
        /// Vacancy/Dopand Ratio: number of vacancies per dopand atom 
        /// </summary>
        [DoubleRange(-10.0, true, 10.0, true, ErrorMessage = "Vacancy to dopand ratio has to be in the interval [-10.0, 10.0].")]
        public double VacDopRatio
        {
            get
            {
                return _VacDopRatio;
            }
            set
            {
                if (ValidateNotify("VacDopRatio", value, ref _VacDopRatio) == true)
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
        public TVMStructureDoping GetCopy()
        {
            // Create shallow copy (all fields are copied, references still point so the same objects)
            TVMStructureDoping t_copy = (TVMStructureDoping)this.MemberwiseClone();

            return t_copy;
        }

        #endregion Methods
    }
}
