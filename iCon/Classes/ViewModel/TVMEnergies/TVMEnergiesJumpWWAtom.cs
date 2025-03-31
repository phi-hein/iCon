using System.Collections.ObjectModel;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for an additive atom in the Energies tab 
    /// </summary>
    public class TVMEnergiesJumpWWAtom: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMEnergiesJumpWWAtom(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _WWAtomID = -1;
            _CoordID = -1;
            _WWID = -1;
            _StartDist = 0;
            _CenterDist = 0;
            _DestDist = 0;
            _Energies = new ObservableCollection<TVMEnergiesJumpWWAtomEnergy>();
        }

        #region Properties

        public int _WWAtomID;
        /// <summary> 
        /// Identification number of this additive atom 
        /// </summary>
        public int WWAtomID
        {
            get
            {
                return _WWAtomID;
            }
            set
            {
                if (value != _WWAtomID)
                {
                    _WWAtomID = value;
                    Notify("WWAtomID");
                }
            }
        }

        public int _CoordID;
        /// <summary> 
        /// Identification number of this atom in the uniquejump-context 
        /// </summary>
        public int CoordID
        {
            get
            {
                return _CoordID;
            }
            set
            {
                if (value != _CoordID)
                {
                    _CoordID = value;
                    Notify("CoordID");
                }
            }
        }

        public int _WWID;
        /// <summary> 
        /// Identification number of the corresponding interaction 
        /// </summary>
        public int WWID
        {
            get
            {
                return _WWID;
            }
            set
            {
                if (value != _WWID)
                {
                    _WWID = value;
                    Notify("WWID");
                }
            }
        }

        public double _StartDist;
        /// <summary> 
        /// Distance of the interacting atom to jump start 
        /// </summary>
        public double StartDist
        {
            get
            {
                return _StartDist;
            }
            set
            {
                if (value != _StartDist)
                {
                    _StartDist = value;
                    Notify("StartDist");
                }
            }
        }

        public double _CenterDist;
        /// <summary> 
        /// Distance of the interacting atom to jump center 
        /// </summary>
        public double CenterDist
        {
            get
            {
                return _CenterDist;
            }
            set
            {
                if (value != _CenterDist)
                {
                    _CenterDist = value;
                    Notify("CenterDist");
                }
            }
        }

        public double _DestDist;
        /// <summary> 
        /// Distance of the interacting atom to jump end 
        /// </summary>
        public double DestDist
        {
            get
            {
                return _DestDist;
            }
            set
            {
                if (value != _DestDist)
                {
                    _DestDist = value;
                    Notify("DestDist");
                }
            }
        }

        public ObservableCollection<TVMEnergiesJumpWWAtomEnergy> _Energies;
        /// <summary> 
        /// Number of shells around moving atoms for jump destinations 
        /// </summary>
        public ObservableCollection<TVMEnergiesJumpWWAtomEnergy> Energies
        {
            get
            {
                return _Energies;
            }
            set
            {
                if (value != _Energies)
                {
                    _Energies = value;
                    Notify("Energies");
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
            if (_Energies.Count > 0)
            {
                for (int i = 0; i < _Energies.Count; i++)
                {
                    if (_Energies[i].ValidateObject() == false) return false;
                }
            }
            return true;
        }

        #endregion Methods
    }
}
