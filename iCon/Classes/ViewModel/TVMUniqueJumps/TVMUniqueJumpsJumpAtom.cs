namespace iCon_General
{
    /// <summary> 
    /// Holds data for an atom in a UniqueJump in the UniqueJumps part of the Jumps tab 
    /// </summary>
    public class TVMUniqueJumpsJumpAtom: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMUniqueJumpsJumpAtom(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _CoordID = -1;
            _Symbol = "";
            _Name = "";
            _Charge = 0;
            _ZylPositionX = 0;
            _ZylPositionY = 0;
            _ZylPositionZ = 0;
            _IsIgnore = false;
            _IsAdditive = false;
            _IsActive = true;
            _StartDist = 0;
            _TSDist = 0;
            _DestDist = 0;
        }

        #region Properties

        public int _CoordID;
        /// <summary> 
        /// Identification number of this atom 
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

        public string _Symbol;
        /// <summary> 
        /// Elementary symbol of the PSE 
        /// </summary>
        public string Symbol
        {
            get
            {
                return _Symbol;
            }
            set
            {
                if (value != _Symbol)
                {
                    _Symbol = value;
                    Notify("Symbol");
                }
            }
        }

        public string _Name;
        /// <summary> 
        /// Optional element name 
        /// </summary>
        public string Name
        {
            get
            {
                return _Name;
            }
            set
            {
                if (value != _Name)
                {
                    _Name = value;
                    Notify("Name");
                }
            }
        }

        public double _Charge;
        /// <summary> 
        /// Charge of the element 
        /// </summary>
        public double Charge
        {
            get
            {
                return _Charge;
            }
            set
            {
                if (value != _Charge)
                {
                    _Charge = value;
                    Notify("Charge");
                }
            }
        }

        public double _ZylPositionX;
        /// <summary> 
        /// x-coordinate in cylindrical coordinates (along jump axis, centered at jump center) 
        /// </summary>
        public double ZylPositionX
        {
            get
            {
                return _ZylPositionX;
            }
            set
            {
                if (value != _ZylPositionX)
                {
                    _ZylPositionX = value;
                    Notify("ZylPositionX");
                }
            }
        }

        public double _ZylPositionY;
        /// <summary> 
        /// y-coordinate in cylindrical coordinates (along jump axis, centered at jump center) 
        /// </summary>
        public double ZylPositionY
        {
            get
            {
                return _ZylPositionY;
            }
            set
            {
                if (value != _ZylPositionY)
                {
                    _ZylPositionY = value;
                    Notify("ZylPositionY");
                }
            }
        }

        public double _ZylPositionZ;
        /// <summary> 
        /// z-coordinate in cylindrical coordinates (along jump axis, centered at jump center) 
        /// </summary>
        public double ZylPositionZ
        {
            get
            {
                return _ZylPositionZ;
            }
            set
            {
                if (value != _ZylPositionZ)
                {
                    _ZylPositionZ = value;
                    Notify("ZylPositionZ");
                }
            }
        }

        public bool _IsIgnore;
        /// <summary> 
        /// Ignoration state of the atom 
        /// </summary>
        public bool IsIgnore
        {
            get
            {
                return _IsIgnore;
            }
            set
            {
                if (value != _IsIgnore)
                {
                    _IsIgnore = value;
                    _ViewModel.IsUniqueJumpsSynchronized = false;
                    Notify("IsIgnore");
                }
            }
        }

        public bool _IsAdditive;
        /// <summary> 
        /// Additivity state of the atom 
        /// </summary>
        public bool IsAdditive
        {
            get
            {
                return _IsAdditive;
            }
            set
            {
                if (value != _IsAdditive)
                {
                    _IsAdditive = value;
                    _ViewModel.IsUniqueJumpsSynchronized = false;
                    Notify("IsAdditive");
                }
            }
        }

        public bool _IsActive;
        /// <summary> 
        /// Activation state of the atom 
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

        public double _StartDist;
        /// <summary> 
        /// Distance between environment atom and jump start 
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

        public double _TSDist;
        /// <summary> 
        /// Distance between environment atom and jump center 
        /// </summary>
        public double TSDist
        {
            get
            {
                return _TSDist;
            }
            set
            {
                if (value != _TSDist)
                {
                    _TSDist = value;
                    Notify("TSDist");
                }
            }
        }

        public double _DestDist;
        /// <summary> 
        /// Distance between environment atom and jump destination 
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

        #endregion Properties
    }
}
