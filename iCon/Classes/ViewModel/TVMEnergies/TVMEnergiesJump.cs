using System.Collections.ObjectModel;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for a unique jump in the Energies tab 
    /// </summary>
    public class TVMEnergiesJump: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMEnergiesJump(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _JumpID = -1;
            _Length = 0;
            _UniqueCodes = new ObservableCollection<TVMEnergiesJumpUCode>();
            _WWAtoms = new ObservableCollection<TVMEnergiesJumpWWAtom>();
            _SelectedUniqueCode = null;
            _SelectedWWAtom = null;
        }

        #region Properties

        public int _JumpID;
        /// <summary> 
        /// Identification number of the unique jump
        /// </summary>
        public int JumpID
        {
            get
            {
                return _JumpID;
            }
            set
            {
                if (value != _JumpID)
                {
                    _JumpID = value;
                    Notify("JumpID");
                }
            }
        }

        public double _Length;
        /// <summary> 
        /// Length of the unique jump 
        /// </summary>
        public double Length
        {
            get
            {
                return _Length;
            }
            set
            {
                if (value != _Length)
                {
                    _Length = value;
                    Notify("Length");
                }
            }
        }

        public ObservableCollection<TVMEnergiesJumpUCode> _UniqueCodes;
        /// <summary> 
        /// List of the unique jump environments 
        /// </summary>
        public ObservableCollection<TVMEnergiesJumpUCode> UniqueCodes
        {
            get
            {
                return _UniqueCodes;
            }
            set
            {
                if (value != _UniqueCodes)
                {
                    _UniqueCodes = value;
                    Notify("UniqueCodes");
                }
            }
        }

        public ObservableCollection<TVMEnergiesJumpWWAtom> _WWAtoms;
        /// <summary> 
        /// List of the additive atoms 
        /// </summary>
        public ObservableCollection<TVMEnergiesJumpWWAtom> WWAtoms
        {
            get
            {
                return _WWAtoms;
            }
            set
            {
                if (value != _WWAtoms)
                {
                    _WWAtoms = value;
                    Notify("WWAtoms");
                }
            }
        }

        public TVMEnergiesJumpUCode _SelectedUniqueCode;
        /// <summary> 
        /// Selected unique jump environment 
        /// </summary>
        public TVMEnergiesJumpUCode SelectedUniqueCode
        {
            get
            {
                return _SelectedUniqueCode;
            }
            set
            {
                if (value != _SelectedUniqueCode)
                {
                    _SelectedUniqueCode = value;
                    Notify("SelectedUniqueCode");
                }
            }
        }

        public TVMEnergiesJumpWWAtom _SelectedWWAtom;
        /// <summary> 
        /// Selected additive atom 
        /// </summary>
        public TVMEnergiesJumpWWAtom SelectedWWAtom
        {
            get
            {
                return _SelectedWWAtom;
            }
            set
            {
                if (value != _SelectedWWAtom)
                {
                    _SelectedWWAtom = value;
                    Notify("SelectedWWAtom");
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
            if (_UniqueCodes.Count > 0)
            {
                for (int i = 0; i < _UniqueCodes.Count; i++)
                {
                    if (_UniqueCodes[i].ValidateObject() == false) return false;
                }
            }
            if (_WWAtoms.Count > 0)
            {
                for (int i = 0; i < _WWAtoms.Count; i++)
                {
                    if (_WWAtoms[i].ValidateFullObject() == false) return false;
                }
            }
            return true;
        }

        #endregion Methods
    }
}
