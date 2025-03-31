namespace iCon_General
{
    /// <summary> 
    /// Holds data for an additive energy contribution of an additive environment atom in the Energies tab 
    /// </summary>
    public class TVMEnergiesJumpWWAtomEnergy: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMEnergiesJumpWWAtomEnergy(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _ElemID = -1;
            _Energy = 0;
            _IsEditable = false;
        }

        #region Properties

        public int _ElemID;
        /// <summary> 
        /// ElemID of the atom that is placed on the environment position 
        /// </summary>
        public int ElemID
        {
            get
            {
                return _ElemID;
            }
            set
            {
                if (value != _ElemID)
                {
                    _ElemID = value;
                    Notify("ElemID");
                }
            }
        }

        public double _Energy;
        /// <summary> 
        /// Additive contribution to activation energy if position is occupied by ElemID 
        /// </summary>
        [DoubleRange(-1000.0, true, 1000.0, true, ErrorMessage="Interaction energy contribution has to be in the interval [-1000.0,1000.0]")]
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

        public bool _IsEditable;
        /// <summary> 
        /// Flag which shows if the connected WWAtom needs an energy input for this ElemID 
        /// </summary>
        public bool IsEditable
        {
            get
            {
                return _IsEditable;
            }
            set
            {
                if (value != _IsEditable)
                {
                    _IsEditable = value;
                    Notify("IsEditable");
                }
            }
        }

        #endregion Properties
    }
}
