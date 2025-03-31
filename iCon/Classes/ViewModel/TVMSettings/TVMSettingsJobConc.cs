namespace iCon_General
{
    /// <summary>
    /// Holds the information for one doping concentration in a single job setting in the Settings tab
    /// </summary>
    public class TVMSettingsJobConc : ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMSettingsJobConc(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _DopingDesc = "";
            _Conc = 0.0;
        }

        #region Properties

        public string _DopingDesc;
        /// <summary> 
        /// Doping description
        /// </summary>
        public string DopingDesc
        {
            get
            {
                return _DopingDesc;
            }
            set
            {
                if (value != _DopingDesc)
                {
                    _DopingDesc = value;
                    Notify("DopingDesc");
                }
            }
        }

        public double _Conc;
        /// <summary> 
        /// Doping concentration
        /// </summary>
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage="Doping concentration has to be in the interval [0.0, 1.0]")]
        public double Conc
        {
            get
            {
                return _Conc;
            }
            set
            {
                ValidateNotify("Conc", value, ref _Conc);
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Create a deep copy (except viewmodel reference) of this object
        /// </summary>
        public TVMSettingsJobConc GetCopy()
        {
            // Create shallow copy (all fields are copied, references still point so the same objects)
            TVMSettingsJobConc t_copy = (TVMSettingsJobConc)this.MemberwiseClone();

            return t_copy;
        }

        #endregion Methods
    }
}
