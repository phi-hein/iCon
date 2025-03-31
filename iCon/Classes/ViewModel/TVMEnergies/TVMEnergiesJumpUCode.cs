namespace iCon_General
{
    /// <summary> 
    /// Holds data for an unique code in the Energies tab 
    /// </summary>
    public class TVMEnergiesJumpUCode: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMEnergiesJumpUCode(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _CodeID = -1;
            _Energy = 0;
            _CodeStr = "";
        }

        #region Properties

        public int _CodeID;
        /// <summary> 
        /// Identification number of the unique code 
        /// </summary>
        public int CodeID
        {
            get
            {
                return _CodeID;
            }
            set
            {
                if (value != _CodeID)
                {
                    _CodeID = value;
                    Notify("CodeID");
                }
            }
        }

        public double _Energy;
        /// <summary> 
        /// Activation energy for this jump environment 
        /// </summary>
        [DoubleRange(-1000.0, true, 1000.0, true, ErrorMessage="Activation energy has to be in the interval [-1000.0,1000.0]")]
        public double Energy
        {
            get
            {
                return _Energy;
            }
            set
            {
                if (ValidateNotify("Energy", value, ref _Energy) == true)
                {
                    _ViewModel.IsEnergiesSynchronized = false;
                }
            }
        }

        public string _CodeStr;
        /// <summary> 
        /// Condensed description of the jump environment 
        /// </summary>
        public string CodeStr
        {
            get
            {
                return _CodeStr;
            }
            set
            {
                if (value != _CodeStr)
                {
                    _CodeStr = value;
                    Notify("CodeStr");
                }
            }
        }

        #endregion Properties
    }
}
