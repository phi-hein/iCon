using System;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;

namespace iCon_General
{
    public class TVM3DJumps: TVM3DModel
    {
        #region Internal Variables

        /// <summary>
        /// Link to ViewModel parent
        /// </summary>
        protected readonly TMCViewModel _ViewModel;

        /// <summary>
        /// Visual3D that contains the lattice lines
        /// </summary>
        protected readonly ModelVisual3D _Lattice;

        /// <summary>
        /// Model3D that contains the lattice lines
        /// </summary>
        protected readonly GeometryModel3D _LatticeModel;

        /// <summary>
        /// Visual3D that contains the arrow
        /// </summary>
        protected readonly ModelVisual3D _Arrow;

        /// <summary>
        /// Model3D that contains the arrow
        /// </summary>
        protected readonly GeometryModel3D _ArrowModel;

        /// <summary>
        /// Lattice vector a
        /// </summary>
        protected Vector3D _LatVecA;

        /// <summary>
        /// Lattice vector b
        /// </summary>
        protected Vector3D _LatVecB;

        /// <summary>
        /// Lattice vector c
        /// </summary>
        protected Vector3D _LatVecC;

        /// <summary>
        /// Vector to the start of the jump arrow
        /// </summary>
        protected Vector3D _ArrowStart;

        /// <summary>
        /// Vector to the end of the jump arrow
        /// </summary>
        protected Vector3D _ArrowEnd;

        /// <summary>
        /// First unit cell in a-direction
        /// </summary>
        protected int _UCStartA;

        /// <summary>
        /// First unit cell in b-direction
        /// </summary>
        protected int _UCStartB;

        /// <summary>
        /// First unit cell in c-direction
        /// </summary>
        protected int _UCStartC;

        /// <summary>
        /// Number of unit cells in a-direction
        /// </summary>
        protected int _UCCountA;

        /// <summary>
        /// Number of unit cells in b-direction
        /// </summary>
        protected int _UCCountB;

        /// <summary>
        /// Number of unit cells in c-direction
        /// </summary>
        protected int _UCCountC;

        #endregion Internal Variables

        #region Properties

        protected ObservableCollection<TVM3DMultiID> _JumpStarts;
        /// <summary>
        /// List of possible jump start positions inside of the central unit cell
        /// </summary>
        public ObservableCollection<TVM3DMultiID> JumpStarts
        {
            get
            {
                return _JumpStarts;
            }
            set
            {
                if (value != _JumpStarts)
                {
                    _JumpStarts = value;
                    Notify("JumpStarts");
                }
            }
        }

        protected TVM3DMultiID _SelectedJumpStart;
        /// <summary>
        /// Selected jump start position
        /// </summary>
        public TVM3DMultiID SelectedJumpStart
        {
            get
            {
                return _SelectedJumpStart;
            }
            set
            {
                if (value != _SelectedJumpStart)
                {
                    _SelectedJumpStart = value;
                    Notify("SelectedJumpStart");
                }
            }
        }

        protected int? _SelectedJumpDir;
        /// <summary>
        /// Selected jump direction
        /// </summary>
        public int? SelectedJumpDir
        {
            get
            {
                return _SelectedJumpDir;
            }
            set
            {
                if (value != _SelectedJumpDir)
                {
                    _SelectedJumpDir = value;
                    Notify("SelectedJumpDir");
                }
            }
        }

        protected bool _IsLatticeVisible;
        /// <summary>
        /// Show/hide lattice lines
        /// </summary>
        public bool IsLatticeVisible
        {
            get
            {
                return _IsLatticeVisible;
            }
            set
            {
                _IsLatticeVisible = value;
                if (_IsLatticeVisible == true)
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_Lattice) == false)
                    {
                        _NonsortedVisual3Ds.Children.Add(_Lattice);
                        UpdateDefaultCamera();
                    }

                }
                else
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_Lattice) == true)
                    {
                        _NonsortedVisual3Ds.Children.Remove(_Lattice);
                        UpdateDefaultCamera();
                    }
                }
                Notify("IsLatticeVisible");
            }
        }

        protected double _LatticeThickness;
        /// <summary>
        /// Thickness (Diameter) of the lattice tubes
        /// </summary>
        [DoubleRange(0.01, true, 1.0, true, ErrorMessage = "Lattice thickness has to be in the interval [0.01, 1.0]")]
        public double LatticeThickness
        {
            get
            {
                return _LatticeThickness;
            }
            set
            {
                if (ValidateNotify("LatticeThickness", value, ref _LatticeThickness) == true)
                {
                    UpdateLatticeGeometry();
                }
            }
        }

        protected Color _LatticeColor;
        /// <summary>
        /// Color of the lattice lines
        /// </summary>
        public Color LatticeColor
        {
            get
            {
                return _LatticeColor;
            }
            set
            {
                if (value != _LatticeColor)
                {
                    _LatticeColor = value;
                    UpdateLatticeMaterial();
                    Notify("LatticeColor");
                }
            }
        }

        protected bool _IsArrowVisible;
        /// <summary>
        /// Show/hide arrow
        /// </summary>
        public bool IsArrowVisible
        {
            get
            {
                return _IsArrowVisible;
            }
            set
            {
                _IsArrowVisible = value;
                if (_IsArrowVisible == true)
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_Arrow) == false)
                    {
                        _NonsortedVisual3Ds.Children.Add(_Arrow);
                        UpdateDefaultCamera();
                    }

                }
                else
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_Arrow) == true)
                    {
                        _NonsortedVisual3Ds.Children.Remove(_Arrow);
                        UpdateDefaultCamera();
                    }
                }
                Notify("IsArrowVisible");
            }
        }

        protected double _ArrowDiameter;
        /// <summary>
        /// Diameter of the arrow
        /// </summary>
        [DoubleRange(0.01, true, 1.0, true, ErrorMessage = "Arrow diameter has to be in the interval [0.01, 1.0]")]
        public double ArrowDiameter
        {
            get
            {
                return _ArrowDiameter;
            }
            set
            {
                if (ValidateNotify("ArrowDiameter", value, ref _ArrowDiameter) == true)
                {
                    UpdateArrowGeometry();
                }
            }
        }

        protected double _ArrowHeadLength;
        /// <summary>
        /// Head length of the arrow
        /// </summary>
        [DoubleRange(0.1, true, 10.0, true, ErrorMessage = "Arrow head length has to be in the interval [0.1, 10.0]")]
        public double ArrowHeadLength
        {
            get
            {
                return _ArrowHeadLength;
            }
            set
            {
                if (ValidateNotify("ArrowHeadLength", value, ref _ArrowHeadLength) == true)
                {
                    UpdateArrowGeometry();
                }
            }
        }

        protected Color _ArrowColor;
        /// <summary>
        /// Color of the arrow
        /// </summary>
        public Color ArrowColor
        {
            get
            {
                return _ArrowColor;
            }
            set
            {
                if (value != _ArrowColor)
                {
                    _ArrowColor = value;
                    UpdateArrowMaterial();
                    Notify("ArrowColor");
                }
            }
        }

        protected double _AtomScale;
        /// <summary>
        /// Scaling factor (0,1] for atom radius
        /// </summary>
        [DoubleRange(0.0, false, 1.0, true, ErrorMessage = "Scaling factor has to be in the interval (0.0, 1.0]")]
        public double AtomScale
        {
            get
            {
                return _AtomScale;
            }
            set
            {
                ValidateNotify("AtomScale", value, ref _AtomScale);
            }
        }

        protected Color _VacancyColor;
        /// <summary>
        /// Color of the vacancy atom
        /// </summary>
        public Color VacancyColor
        {
            get
            {
                return _VacancyColor;
            }
            set
            {
                if (value != _VacancyColor)
                {
                    _VacancyColor = value;
                    Notify("VacancyColor");
                }
            }
        }

        protected double _VacancyOpacity;
        /// <summary>
        /// Opacity of the vacancy atom
        /// </summary>
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage = "Opacity has to be in the interval [0.0, 1.0]")]
        public double VacancyOpacity
        {
            get
            {
                return _VacancyOpacity;
            }
            set
            {
                ValidateNotify("VacancyOpacity", value, ref _VacancyOpacity);
            }
        }

        protected bool _IsAdditionalAtomsVisible;
        /// <summary>
        /// true = atoms on the outer planes are visible
        /// </summary>
        public bool IsAdditionalAtomsVisible
        {
            get
            {
                return _IsAdditionalAtomsVisible;
            }
            set
            {
                if (value != _IsAdditionalAtomsVisible)
                {
                    _IsAdditionalAtomsVisible = value;
                    Notify("IsAdditionalAtomsVisible");
                }
            }
        }

        protected double _IrrelevantAtomOpacity;
        /// <summary>
        /// Opacity (0.0-1.0] of atoms that are not part of the jump environment (or that are ignored)
        /// </summary>
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage = "Opacity has to be in the interval [0.0, 1.0]")]
        public double IrrelevantAtomOpacity 
        {
            get
            {
                return _IrrelevantAtomOpacity;
            }
            set
            {
                ValidateNotify("IrrelevantAtomOpacity", value, ref _IrrelevantAtomOpacity);
            }
        }

        protected bool _IsIrrelevantAtomsVisible;
        /// <summary>
        /// true = atoms of the unit cells that are not part of the jump environment (or those which are ignored) are visible
        /// </summary>
        public bool IsIrrelevantAtomsVisible
        {
            get
            {
                return _IsIrrelevantAtomsVisible;
            }
            set
            {
                if (value != _IsIrrelevantAtomsVisible)
                {
                    _IsIrrelevantAtomsVisible = value;
                    Notify("IsIrrelevantAtomsVisible");
                }
            }
        }

        protected bool _IsShowOnlyUCWithAtomsInside;
        /// <summary>
        /// true = Unit cells, who only contribute atoms to the jump environment which are on the border to other unit cells, are not shown
        /// </summary>
        public bool IsShowOnlyUCWithAtomsInside
        {
            get
            {
                return _IsShowOnlyUCWithAtomsInside;
            }
            set
            {
                if (value != _IsShowOnlyUCWithAtomsInside)
                {
                    _IsShowOnlyUCWithAtomsInside = value;
                    Notify("IsShowOnlyUCWithAtomsInside");
                }
            }
        }

        protected bool _IsInVacancyImage;
        /// <summary>
        /// true = Vacancy is jumping (not the normal atom)
        /// </summary>
        public bool IsInVacancyImage
        {
            get
            {
                return _IsInVacancyImage;
            }
            set
            {
                if (value != _IsInVacancyImage)
                {
                    _IsInVacancyImage = value;
                    Notify("IsInVacancyImage");
                }
            }
        }

        #endregion Properties

        public TVM3DJumps(TMCViewModel ViewModel)
            : base()
        {
            // Save parent
            _ViewModel = ViewModel;

            // Model setup
            _Lattice = new ModelVisual3D();
            _LatticeModel = new GeometryModel3D();
            _Lattice.Content = _LatticeModel;
            _Arrow = new ModelVisual3D();
            _ArrowModel = new GeometryModel3D();
            _Arrow.Content = _ArrowModel;

            // Field initializations
            _LatVecA = new Vector3D(0.0, 0.0, 0.0);
            _LatVecB = new Vector3D(0.0, 0.0, 0.0);
            _LatVecC = new Vector3D(0.0, 0.0, 0.0);
            _ArrowStart = new Vector3D(0.0, 0.0, 0.0);
            _ArrowEnd = new Vector3D(0.0, 0.0, 0.0);
            _UCStartA = 0;
            _UCStartB = 0;
            _UCStartC = 0;
            _UCCountA = 0;
            _UCCountB = 0;
            _UCCountC = 0;

            // Property initializations
            _JumpStarts = new ObservableCollection<TVM3DMultiID>();
            _SelectedJumpStart = null;
            _SelectedJumpDir = null;
            _IsLatticeVisible = true;
            _LatticeThickness = 0.05;
            _LatticeColor = Colors.Black;
            _IsArrowVisible = true;
            _ArrowDiameter = 0.2;
            _ArrowHeadLength = 3.0;
            _ArrowColor = Colors.Blue;
            _AtomScale = 0.5;
            _IsAdditionalAtomsVisible = true;
            _IrrelevantAtomOpacity = 0.5;
            _IsIrrelevantAtomsVisible = true;
            _IsShowOnlyUCWithAtomsInside = true;
            _VacancyColor = Colors.DarkGray;
            _VacancyOpacity = 1.0;
            _IsInVacancyImage = true;

            // Model initialization
            UpdateModel();
        }

        #region Methods

        /// <summary>
        /// Resets model to basic scene setup
        /// </summary>
        public void ClearModel()
        {
            // Unselect atoms
            SelectAtom(-1);

            // Remove 3d objects
            _NonsortedVisual3Ds.Children.Clear();
            _SortedVisual3Ds.Children.Clear();
            GC.Collect();

            // Field reset
            _LatVecA = new Vector3D(0.0, 0.0, 0.0);
            _LatVecB = new Vector3D(0.0, 0.0, 0.0);
            _LatVecC = new Vector3D(0.0, 0.0, 0.0);
            _ArrowStart = new Vector3D(0.0, 0.0, 0.0);
            _ArrowEnd = new Vector3D(0.0, 0.0, 0.0);
            _UCStartA = 0;
            _UCStartB = 0;
            _UCStartC = 0;
            _UCCountA = 0;
            _UCCountB = 0;
            _UCCountC = 0;

            // Basic scene composition
            var light = new SunLight();
            light.Brightness = 0.8;
            _NonsortedVisual3Ds.Children.Add(light);

            // Lattice update
            UpdateLatticeGeometry();
            UpdateLatticeMaterial();

            // Arrow update
            UpdateArrowGeometry();
            UpdateArrowMaterial();

            // Update rendering flags
            UpdateIsSorted();
            UpdateIsRendering();

            // Update correctly zoomed default camera
            UpdateDefaultCamera();
        }

        /// <summary>
        /// Update the model from the MCJob object of the ViewModel parent
        /// </summary>
        public void UpdateModel()
        {
            // Reset model
            ClearModel();

            // Check for valid model
            if (_ViewModel == null) return;
            if (_ViewModel.MCJob == null) return;
            if (_ViewModel.MCJob.IsValid == false) return;

            // Check for invalid parameters
            double t_AtomScale = 0.5;
            if (HasPropertyErrors("AtomScale") == false) t_AtomScale = _AtomScale;
            double t_VacancyOpacity = 1.0;
            if (HasPropertyErrors("VacancyOpacity") == false) t_VacancyOpacity = _VacancyOpacity;
            double t_IrrelevantAtomOpacity = 0.5;
            if (HasPropertyErrors("IrrelevantAtomOpacity") == false) t_IrrelevantAtomOpacity = _IrrelevantAtomOpacity;

            // Save current jump selection
            int t_selatom = -1;
            if (_SelectedJumpStart != null) t_selatom = _SelectedJumpStart.ID;
            int t_seldir = -1;
            if (_SelectedJumpDir != null) t_seldir = _SelectedJumpDir.Value;

            // Update jump selection collections
            JumpStarts.Clear();
            if (_ViewModel.MCJob.Jumps.Jumps.Count > 0)
            {
                TVM3DMultiID t_start = null;
                for (int i = 0; i < _ViewModel.MCJob.Jumps.Jumps.Count; i++)
                {
                    t_start = new TVM3DMultiID();
                    t_start.ID = i;
                    if (_ViewModel.MCJob.Jumps.Jumps[i].Count > 0)
                    {
                        for (int j = 0; j < _ViewModel.MCJob.Jumps.Jumps[i].Count; j++)
                        {
                            t_start.NextIDs.Add(j);
                        }
                    }
                    JumpStarts.Add(t_start);
                }
            }

            // Compare jump selection and available jumps
            SelectedJumpStart = null;
            SelectedJumpDir = null;
            if (_JumpStarts.Count > 0) {
                if ((t_selatom >= 0) && (t_selatom < _JumpStarts.Count))
                {
                    SelectedJumpStart = _JumpStarts[t_selatom];
                }
                else
                {
                    SelectedJumpStart = _JumpStarts[0];
                }
            }
            if (_SelectedJumpStart != null)
            {
                if (_SelectedJumpStart.NextIDs.Count > 0)
                {
                    if ((t_seldir >= 0) && (t_seldir < _SelectedJumpStart.NextIDs.Count))
                    {
                        SelectedJumpDir = t_seldir;
                    }
                    else
                    {
                        SelectedJumpDir = 0;
                    }
                }
            }

            // Check for valid jump selection
            if ((_SelectedJumpStart == null) || (_SelectedJumpDir == null)) return;

            // Get reference to selected jump
            TJump jump = _ViewModel.MCJob.Jumps.Jumps[_SelectedJumpStart.ID][_SelectedJumpDir.Value];

            // Save lattice vectors
            _LatVecA = _ViewModel.MCJob.Structure.BasisA;
            _LatVecB = _ViewModel.MCJob.Structure.BasisB;
            _LatVecC = _ViewModel.MCJob.Structure.BasisC;

            // Define lattice dimensions (Attention: envpos 4D vectors are relative to start 4D vector)
            int startA = Math.Min(jump.StartPos.X, jump.StartPos.X + jump.DestPos.X);
            int startB = Math.Min(jump.StartPos.Y, jump.StartPos.Y + jump.DestPos.Y);
            int startC = Math.Min(jump.StartPos.Z, jump.StartPos.Z + jump.DestPos.Z);
            int endA = Math.Max(jump.StartPos.X, jump.StartPos.X + jump.DestPos.X);
            int endB = Math.Max(jump.StartPos.Y, jump.StartPos.Y + jump.DestPos.Y);
            int endC = Math.Max(jump.StartPos.Z, jump.StartPos.Z + jump.DestPos.Z);
            if (jump.EnvPos.Count > 0)
            {
                for (int i = 0; i < jump.EnvPos.Count; i++)
                {
                    if ((jump.UniqueJumpID >= 0) && (jump.UniqueJumpID < _ViewModel.UniqueJumps.UJumps.Count))
                    {
                        if (_ViewModel.UniqueJumps.UJumps[jump.UniqueJumpID].UniqueJumpAtoms[i].IsIgnore == true)
                        {
                            continue;
                        }
                    }

                    startA = Math.Min(jump.StartPos.X + jump.EnvPos[i].X, startA);
                    startB = Math.Min(jump.StartPos.Y + jump.EnvPos[i].Y, startB);
                    startC = Math.Min(jump.StartPos.Z + jump.EnvPos[i].Z, startC);
                    endA = Math.Max(jump.StartPos.X + jump.EnvPos[i].X, endA);
                    endB = Math.Max(jump.StartPos.Y + jump.EnvPos[i].Y, endB);
                    endC = Math.Max(jump.StartPos.Z + jump.EnvPos[i].Z, endC);
                }
            }
            _UCStartA = startA;
            _UCStartB = startB;
            _UCStartC = startC;
            _UCCountA = endA - startA + 1;
            _UCCountB = endB - startB + 1;
            _UCCountC = endC - startC + 1;

            // Calculate arrow vectors
            _ArrowStart = (jump.StartPos.X + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S].X) * _LatVecA + 
                (jump.StartPos.Y + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S].Y) * _LatVecB + 
                (jump.StartPos.Z + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S].Z) * _LatVecC;
            _ArrowEnd = (jump.StartPos.X + jump.DestPos.X + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.DestPos.S].X) * _LatVecA + 
                (jump.StartPos.Y + jump.DestPos.Y + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.DestPos.S].Y) * _LatVecB +
                (jump.StartPos.Z + jump.DestPos.Z + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.DestPos.S].Z) * _LatVecC;

            // Determine, if there are outer unit cell layers (far x-layer, far y-layer and far z-layer) that only contain 
            // environment atoms on the border to inner layers (-> outer layer not printed if IsShowOnlyUCWithAtomsInside == true)
            bool omitfarx = true;
            bool omitfary = true;
            bool omitfarz = true;
            // Check if start atom is in a far layer and check if it is not on a border
            if ((jump.StartPos.X == _UCStartA + _UCCountA - 1) && (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S].X != 0)) omitfarx = false;
            if ((jump.StartPos.Y == _UCStartB + _UCCountB - 1) && (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S].Y != 0)) omitfary = false;
            if ((jump.StartPos.Z == _UCStartC + _UCCountC - 1) && (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S].Z != 0)) omitfarz = false;
            // Check if destination atom is in a far layer and check if it is not on a border
            if ((jump.StartPos.X + jump.DestPos.X == _UCStartA + _UCCountA - 1) &&
                (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.DestPos.S].X != 0)) omitfarx = false;
            if ((jump.StartPos.Y + jump.DestPos.Y == _UCStartB + _UCCountB - 1) &&
                (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.DestPos.S].Y != 0)) omitfary = false;
            if ((jump.StartPos.Z + jump.DestPos.Z == _UCStartC + _UCCountC - 1) &&
                (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.DestPos.S].Z != 0)) omitfarz = false;
            if (jump.EnvPos.Count > 0)
            {
                for (int i = 0; i < jump.EnvPos.Count; i++)
                {
                    // Abort if result is clear
                    if ((omitfarx == false) && (omitfary == false) && (omitfarz == false)) break;

                    // Ignore ignored atoms
                    if ((jump.UniqueJumpID >= 0) && (jump.UniqueJumpID < _ViewModel.UniqueJumps.UJumps.Count))
                    {
                        if (_ViewModel.UniqueJumps.UJumps[jump.UniqueJumpID].UniqueJumpAtoms[i].IsIgnore == true)
                        {
                            continue;
                        }
                    }

                    // Check if atom is in a far layer and check if it is not on a border
                    if ((jump.StartPos.X + jump.EnvPos[i].X == _UCStartA + _UCCountA - 1) &&
                        (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.EnvPos[i].S].X != 0)) omitfarx = false;
                    if ((jump.StartPos.Y + jump.EnvPos[i].Y == _UCStartB + _UCCountB - 1) &&
                        (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.EnvPos[i].S].Y != 0)) omitfary = false;
                    if ((jump.StartPos.Z + jump.EnvPos[i].Z == _UCStartC + _UCCountC - 1) &&
                        (_ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.EnvPos[i].S].Z != 0)) omitfarz = false;
                }
            }

            // Add atoms
            AtomUIElement3D atom3d = null;
            if (_IsIrrelevantAtomsVisible == false)
            {
                int counter = 0;

                // Add jump start atom/vacancy
                counter++;
                atom3d = new AtomUIElement3D();
                atom3d.MouseDown += this.Atom_MouseDown;
                atom3d.AtomID = counter;
                atom3d.AtomPosition = _ArrowStart;
                atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S]] * t_AtomScale;
                if (_IsInVacancyImage == true)
                {
                    atom3d.AtomSymbol = "Vc";
                    atom3d.AtomName = "";
                    atom3d.AtomCharge = 0;
                    atom3d.AtomColor = _VacancyColor;
                    atom3d.AtomOpacity = t_VacancyOpacity;
                }
                else
                {
                    atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S]];
                    atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S]];
                    atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S]];
                    atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S]];
                    atom3d.AtomOpacity = 1.0;
                }
                _SortedVisual3Ds.Children.Add(atom3d);

                // Add jump destination atom/vacancy
                counter++;
                atom3d = new AtomUIElement3D();
                atom3d.MouseDown += this.Atom_MouseDown;
                atom3d.AtomID = counter;
                atom3d.AtomPosition = _ArrowEnd;
                atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.DestPos.S]] * t_AtomScale;
                if (_IsInVacancyImage == false)
                {
                    atom3d.AtomSymbol = "Vc";
                    atom3d.AtomName = "";
                    atom3d.AtomCharge = 0;
                    atom3d.AtomColor = _VacancyColor;
                    atom3d.AtomOpacity = t_VacancyOpacity;
                }
                else
                {
                    atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.DestPos.S]];
                    atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.DestPos.S]];
                    atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.DestPos.S]];
                    atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.DestPos.S]];
                    atom3d.AtomOpacity = 1.0;
                }
                _SortedVisual3Ds.Children.Add(atom3d);

                // Add only jump environment atoms (non-ignored)
                if (jump.EnvPos.Count > 0)
                {
                    for (int i = 0; i < jump.EnvPos.Count; i++)
                    {
                        // Detect if the atom is non-ignored
                        if ((jump.UniqueJumpID >= 0) && (jump.UniqueJumpID < _ViewModel.UniqueJumps.UJumps.Count))
                        {
                            if (_ViewModel.UniqueJumps.UJumps[jump.UniqueJumpID].UniqueJumpAtoms[i].IsIgnore == true)
                            {
                                continue;
                            }
                        }

                        // Add atom
                        counter++;
                        atom3d = new AtomUIElement3D();
                        atom3d.MouseDown += this.Atom_MouseDown;

                        atom3d.AtomID = counter;
                        atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.EnvPos[i].S]];
                        atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.EnvPos[i].S]];
                        atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.EnvPos[i].S]];
                        atom3d.AtomPosition = (jump.StartPos.X + jump.EnvPos[i].X + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.EnvPos[i].S].X) * _LatVecA +
                            (jump.StartPos.Y + jump.EnvPos[i].Y + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.EnvPos[i].S].Y) * _LatVecB +
                            (jump.StartPos.Z + jump.EnvPos[i].Z + _ViewModel.MCJob.Structure.Coord[jump.StartPos.S + jump.EnvPos[i].S].Z) * _LatVecC;
                        atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.EnvPos[i].S]];
                        atom3d.AtomOpacity = 1.0;
                        atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[_ViewModel.MCJob.Structure.ElemID[jump.StartPos.S + jump.EnvPos[i].S]] * t_AtomScale;

                        _SortedVisual3Ds.Children.Add(atom3d);
                    }
                }
            }
            else
            {
                // Add all atoms within lattice borders (incl. additional atoms = atoms on outer planes)
                int counter = 0;
                int countA = _UCCountA;
                int countB = _UCCountB;
                int countC = _UCCountC;
                if (_IsAdditionalAtomsVisible == true)
                {
                    if (_IsShowOnlyUCWithAtomsInside == true)
                    {
                        if (omitfarx == false) countA++;
                        if (omitfary == false) countB++;
                        if (omitfarz == false) countC++;
                    }
                    else
                    {
                        countA++;
                        countB++;
                        countC++;
                    }
                }
                for (int a = _UCStartA; a < _UCStartA + countA; a++)
                {
                    for (int b = _UCStartB; b < _UCStartB + countB; b++)
                    {
                        for (int c = _UCStartC; c < _UCStartC + countC; c++)
                        {
                            for (int s = 0; s < _ViewModel.MCJob.Structure.Coord.Count; s++)
                            {
                                // Detect valid (= non-ignored) jump start, destination and environment atoms
                                bool isenv = false;
                                bool isvacancy = false;
                                if ((a == jump.StartPos.X) && (b == jump.StartPos.Y) && (c == jump.StartPos.Z) && (s == jump.StartPos.S))
                                {
                                    isenv = true;
                                    if (_IsInVacancyImage == true) isvacancy = true;
                                }
                                if ((a == jump.StartPos.X + jump.DestPos.X) && (b == jump.StartPos.Y + jump.DestPos.Y) &&
                                    (c == jump.StartPos.Z + jump.DestPos.Z) && (s == jump.StartPos.S + jump.DestPos.S))
                                {
                                    isenv = true;
                                    if (_IsInVacancyImage == false) isvacancy = true;
                                }
                                if (isenv == false)
                                {
                                    if (jump.EnvPos.Count > 0)
                                    {
                                        for (int i = 0; i < jump.EnvPos.Count; i++)
                                        {
                                            if ((a == jump.StartPos.X + jump.EnvPos[i].X) &&
                                                (b == jump.StartPos.Y + jump.EnvPos[i].Y) &&
                                                (c == jump.StartPos.Z + jump.EnvPos[i].Z) &&
                                                (s == jump.StartPos.S + jump.EnvPos[i].S))
                                            {
                                                if ((jump.UniqueJumpID >= 0) && (jump.UniqueJumpID < _ViewModel.UniqueJumps.UJumps.Count))
                                                {
                                                    if (_ViewModel.UniqueJumps.UJumps[jump.UniqueJumpID].UniqueJumpAtoms[i].IsIgnore == false)
                                                    {
                                                        isenv = true;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    isenv = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }

                                // x-Direction: determine if this atom should not be added to model
                                // Case 1:
                                if ((_IsAdditionalAtomsVisible == false) &&             // = Atoms on the border should not be added
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (a == _UCStartA + countA - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].X == 0))       // Atom is on the border to inner layer
                                {
                                    continue;     
                                }
                                // Case 2:
                                if ((_IsShowOnlyUCWithAtomsInside == true) &&           // = outer atoms should be omitted if no env-atoms are inside the outer unit cells
                                    (omitfarx == true) &&                               // = there are no env-atoms inside the outer unit cells (only on the border to inner cells)
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (a == _UCStartA + countA - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].X != 0))       // Atom is not on the border to inner layer
                                {
                                    continue;
                                }
                                // Case 3:
                                if (((_IsShowOnlyUCWithAtomsInside == false) || 
                                     (omitfarx == false)) &&                            // = there might be an additional outer layer for IsAdditionalAtomsVisible
                                    (_IsAdditionalAtomsVisible == true) &&              // = there is an additional outer layer only for border atoms (atoms inside should not be added)
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (a == _UCStartA + countA - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].X != 0))       // Atom is not on the border to inner layer
                                {
                                    continue;
                                }

                                // y-Direction: determine if this atom should not be added to model
                                // Case 1:
                                if ((_IsAdditionalAtomsVisible == false) &&             // = Atoms on the border should not be added
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (b == _UCStartB + countB - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].Y == 0))       // Atom is on the border to inner layer
                                {
                                    continue;
                                }
                                // Case 2:
                                if ((_IsShowOnlyUCWithAtomsInside == true) &&           // = outer atoms should be omitted if no env-atoms are inside the outer unit cells
                                    (omitfary == true) &&                               // = there are no env-atoms inside the outer unit cells (only on the border to inner cells)
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (b == _UCStartB + countB - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].Y != 0))       // Atom is not on the border to inner layer
                                {
                                    continue;
                                }
                                // Case 3:
                                if (((_IsShowOnlyUCWithAtomsInside == false) ||
                                     (omitfary == false)) &&                            // = there might be an additional outer layer for IsAdditionalAtomsVisible
                                    (_IsAdditionalAtomsVisible == true) &&              // = there is an additional outer layer only for border atoms (atoms inside should not be added)
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (b == _UCStartB + countB - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].Y != 0))       // Atom is not on the border to inner layer
                                {
                                    continue;
                                }

                                // z-Direction: determine if this atom should not be added to model
                                // Case 1:
                                if ((_IsAdditionalAtomsVisible == false) &&             // = Atoms on the border should not be added
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (c == _UCStartC + countC - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].Z == 0))       // Atom is on the border to inner layer
                                {
                                    continue;
                                }
                                // Case 2:
                                if ((_IsShowOnlyUCWithAtomsInside == true) &&           // = outer atoms should be omitted if no env-atoms are inside the outer unit cells
                                    (omitfarz == true) &&                               // = there are no env-atoms inside the outer unit cells (only on the border to inner cells)
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (c == _UCStartC + countC - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].Z != 0))       // Atom is not on the border to inner layer
                                {
                                    continue;
                                }
                                // Case 3:
                                if (((_IsShowOnlyUCWithAtomsInside == false) ||
                                     (omitfarz == false)) &&                            // = there might be an additional outer layer for IsAdditionalAtomsVisible
                                    (_IsAdditionalAtomsVisible == true) &&              // = there is an additional outer layer only for border atoms (atoms inside should not be added)
                                    (isenv == false) &&                                 // Atom is not in jump environment (or ignored)
                                    (c == _UCStartC + countC - 1) &&                    // Atom is in outer layer
                                    (_ViewModel.MCJob.Structure.Coord[s].Z != 0))       // Atom is not on the border to inner layer
                                {
                                    continue;
                                }

                                counter++;
                                atom3d = new AtomUIElement3D();
                                atom3d.MouseDown += this.Atom_MouseDown;

                                atom3d.AtomID = counter;
                                atom3d.AtomPosition = (a + _ViewModel.MCJob.Structure.Coord[s].X) * _LatVecA + 
                                    (b + _ViewModel.MCJob.Structure.Coord[s].Y) * _LatVecB + 
                                    (c + _ViewModel.MCJob.Structure.Coord[s].Z) * _LatVecC;
                                atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[_ViewModel.MCJob.Structure.ElemID[s]] * t_AtomScale;
                                if (isvacancy == true)
                                {
                                    atom3d.AtomSymbol = "Vc";
                                    atom3d.AtomName = "";
                                    atom3d.AtomCharge = 0;
                                    atom3d.AtomColor = _VacancyColor;
                                    atom3d.AtomOpacity = t_VacancyOpacity;
                                }
                                else
                                {
                                    atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[_ViewModel.MCJob.Structure.ElemID[s]];
                                    atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[_ViewModel.MCJob.Structure.ElemID[s]];
                                    atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[_ViewModel.MCJob.Structure.ElemID[s]];
                                    atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[_ViewModel.MCJob.Structure.ElemID[s]];

                                    // Switch opacity
                                    if (isenv == true)
                                    {
                                        atom3d.AtomOpacity = 1.0;
                                    }
                                    else
                                    {
                                        atom3d.AtomOpacity = t_IrrelevantAtomOpacity;
                                    }
                                }

                                _SortedVisual3Ds.Children.Add(atom3d);
                            }
                        }
                    }
                }
            }

            // Shrink lattice if some outer layers are omitted
            if (_IsShowOnlyUCWithAtomsInside == true)
            {
                if (omitfarx == true) _UCCountA--;
                if (omitfary == true) _UCCountB--;
                if (omitfarz == true) _UCCountC--;
            }
            
            // Lattice update
            UpdateLatticeGeometry();
            UpdateLatticeMaterial();

            // Arrow update
            UpdateArrowGeometry();
            UpdateArrowMaterial();

            // Update atom labels (update text and show if mode != none)
            UpdateAtomLabels();

            // Check for transparent objects and enable sorting if neccessary
            UpdateIsSorted();
            UpdateIsRendering();

            // Update correctly zoomed default camera
            UpdateDefaultCamera();
        }

        /// <summary>
        /// Updates the lattice lines
        /// </summary>
        public void UpdateLatticeGeometry()
        {
            // Test if valid lattice vectors are present (no test for linear independence)
            if ((_LatVecA.Length == 0) ||
                (_LatVecB.Length == 0) ||
                (_LatVecC.Length == 0) ||
                (Vector3D.Equals(_LatVecA,_LatVecB) == true) ||
                (Vector3D.Equals(_LatVecB,_LatVecC) == true) ||
                (Vector3D.Equals(_LatVecA,_LatVecC) == true))
            {
                _LatticeModel.Geometry = null;
                return;
            }

            // Check for invalid parameters
            double t_LatticeThickness = 0.05;
            if (HasPropertyErrors("LatticeThickness") == false) t_LatticeThickness = _LatticeThickness;

            // Create and assign lattice mesh:
            var latticemeshbuilder = new MeshBuilder();

            // Lines:
            // Start points for lines in a-direction are in the b/c-plane
            for (int b = _UCStartB; b <= _UCStartB + _UCCountB; b++)
            {
                for (int c = _UCStartC; c <= _UCStartC + _UCCountC; c++)
                {
                    latticemeshbuilder.AddCylinder((_UCStartA * _LatVecA + b * _LatVecB + c * _LatVecC).ToPoint3D(), 
                        ((_UCStartA + _UCCountA) * _LatVecA + b * _LatVecB + c * _LatVecC).ToPoint3D(), t_LatticeThickness, 10);
                }
            }

            // Start points for lines in b-direction are in the a/c-plane
            for (int a = _UCStartA; a <= _UCStartA + _UCCountA; a++)
            {
                for (int c = _UCStartC; c <= _UCStartC + _UCCountC; c++)
                {
                    latticemeshbuilder.AddCylinder((a * _LatVecA + _UCStartB * _LatVecB + c * _LatVecC).ToPoint3D(), 
                        (a * _LatVecA + (_UCStartB + _UCCountB) * _LatVecB + c * _LatVecC).ToPoint3D(), t_LatticeThickness, 10);
                }
            }
            // Start points for lines in c-direction are in the a/b-plane
            for (int a = _UCStartA; a <= _UCStartA + _UCCountA; a++)
            {
                for (int b = _UCStartB; b <= _UCStartB + _UCCountB; b++)
                {
                    latticemeshbuilder.AddCylinder((a * _LatVecA + b * _LatVecB + _UCStartC * _LatVecC).ToPoint3D(), 
                        (a * _LatVecA + b * _LatVecB + (_UCStartC + _UCCountC) * _LatVecC).ToPoint3D(), t_LatticeThickness, 10);
                }
            }

            // Round corners:
            for (int a = 0; a <= 1; a++)
            {
                for (int b = 0; b <= 1; b++)
                {
                    for (int c = 0; c <= 1; c++)
                    {
                        latticemeshbuilder.AddSphere(((_UCStartA + a * _UCCountA) * _LatVecA + (_UCStartB + b * _UCCountB) * _LatVecB + 
                            (_UCStartC + c * _UCCountC) * _LatVecC).ToPoint3D(), t_LatticeThickness / 2, 10, 10);
                    }
                }
            }

            // Set lattice geometry (and add to model if necessary)          
            _LatticeModel.Geometry = latticemeshbuilder.ToMesh();
            IsLatticeVisible = _IsLatticeVisible;

            // Update correctly zoomed default camera
            UpdateDefaultCamera();
        }

        /// <summary>
        /// Updates the color of the lattice
        /// </summary>
        public void UpdateLatticeMaterial()
        {
            _LatticeModel.Material = new DiffuseMaterial(new SolidColorBrush(_LatticeColor));
        }

        /// <summary>
        /// Updates the arrow shape
        /// </summary>
        public void UpdateArrowGeometry()
        {
            // Test if valid arrow points are present
            if (Vector3D.Equals(_ArrowStart, _ArrowEnd) == true)
            {
                _ArrowModel.Geometry = null;
                return;
            }

            // Check for invalid parameters
            double t_ArrowDiameter = 0.2;
            if (HasPropertyErrors("ArrowDiameter") == false) t_ArrowDiameter = _ArrowDiameter;
            double t_ArrowHeadLength = 3.0;
            if (HasPropertyErrors("ArrowHeadLength") == false) t_ArrowHeadLength = _ArrowHeadLength;

            // Create and assign arrow mesh:
            var arrowmeshbuilder = new MeshBuilder();
            arrowmeshbuilder.AddArrow(_ArrowStart.ToPoint3D(), _ArrowEnd.ToPoint3D(), t_ArrowDiameter, t_ArrowHeadLength, 10);

            // Set arrow geometry (and add to model if necessary)
            _ArrowModel.Geometry = arrowmeshbuilder.ToMesh();
            IsArrowVisible = _IsArrowVisible;

            // Update correctly zoomed default camera
            UpdateDefaultCamera();
        }

        /// <summary>
        /// Updates the color of the arrow
        /// </summary>
        public void UpdateArrowMaterial()
        {
            _ArrowModel.Material = MaterialHelper.CreateMaterial(_ArrowColor);
        }

        #endregion Methods
    }
}
