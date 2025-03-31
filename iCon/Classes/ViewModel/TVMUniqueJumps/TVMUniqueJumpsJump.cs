using System.Collections.ObjectModel;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for a UniqueJump in the UniqueJumps part of the Jumps tab 
    /// </summary>
    public class TVMUniqueJumpsJump: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMUniqueJumpsJump(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _JumpID = -1;
            _Length = 0;
            _IsActive = true;
            _UniqueJumpAtoms = new ObservableCollection<TVMUniqueJumpsJumpAtom>();
            _SelectedUniqueJumpAtom = null;
        }

        #region Properties

        public int _JumpID;
        /// <summary> 
        /// Identification number of this unique jump 
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

        public bool _IsActive;
        /// <summary> 
        /// Activation state of the unique jump 
        /// </summary>
        public bool IsActive
        {
            get
            {
                return _IsActive;
            }
            set
            {
                if (value != _IsActive)
                {
                    _IsActive = value;
                    _ViewModel.IsUniqueJumpsSynchronized = false;
                    Notify("IsActive");
                }
            }
        }

        public ObservableCollection<TVMUniqueJumpsJumpAtom> _UniqueJumpAtoms;
        /// <summary> 
        /// Environment atoms of the unique jump 
        /// </summary>
        public ObservableCollection<TVMUniqueJumpsJumpAtom> UniqueJumpAtoms
        {
            get
            {
                return _UniqueJumpAtoms;
            }
            set
            {
                if (value != _UniqueJumpAtoms)
                {
                    _UniqueJumpAtoms = value;
                    _ViewModel.IsUniqueJumpsSynchronized = false;
                    Notify("UniqueJumpAtoms");
                }
            }
        }

        public TVMUniqueJumpsJumpAtom _SelectedUniqueJumpAtom;
        /// <summary> 
        /// Currently selected environment atom 
        /// </summary>
        public TVMUniqueJumpsJumpAtom SelectedUniqueJumpAtom
        {
            get
            {
                return _SelectedUniqueJumpAtom;
            }
            set
            {
                if (value != _SelectedUniqueJumpAtom)
                {
                    _SelectedUniqueJumpAtom = value;
                    Notify("SelectedUniqueJumpAtom");
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
            if (_UniqueJumpAtoms.Count > 0)
            {
                for (int i = 0; i < _UniqueJumpAtoms.Count; i++)
                {
                    if (_UniqueJumpAtoms[i].ValidateObject() == false) return false;
                }
            }
            return true;
        }

        #endregion Methods
    }
}
