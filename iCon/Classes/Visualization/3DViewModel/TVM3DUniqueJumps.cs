using System;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;

namespace iCon_General
{
    public class TVM3DUniqueJumps: TVM3DModel
    {
        #region Internal Variables

        /// <summary>
        /// Link to ViewModel parent
        /// </summary>
        protected readonly TMCViewModel _ViewModel;

        /// <summary>
        /// Visual3D that contains the arrow
        /// </summary>
        protected readonly ModelVisual3D _Arrow;

        /// <summary>
        /// Model3D that contains the arrow
        /// </summary>
        protected readonly GeometryModel3D _ArrowModel;

        /// <summary>
        /// Vector to the start of the jump arrow
        /// </summary>
        protected Vector3D _ArrowStart;

        /// <summary>
        /// Vector to the end of the jump arrow
        /// </summary>
        protected Vector3D _ArrowEnd;

        #endregion Internal Variables

        #region Properties

        protected ObservableCollection<int> _UJumpList;
        /// <summary>
        /// List of possible unique jump IDs
        /// </summary>
        public ObservableCollection<int> UJumpList
        {
            get
            {
                return _UJumpList;
            }
            set
            {
                if (value != _UJumpList)
                {
                    _UJumpList = value;
                    Notify("UJumpList");
                }
            }
        }

        protected int? _SelectedUJump;
        /// <summary>
        /// Selected unique jump
        /// </summary>
        public int? SelectedUJump
        {
            get
            {
                return _SelectedUJump;
            }
            set
            {
                if (value != _SelectedUJump)
                {
                    _SelectedUJump = value;
                    Notify("SelectedUJump");
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

        protected bool _IsIgnoredAtomsVisible;
        /// <summary>
        /// true = ignored atoms are visible
        /// </summary>
        public bool IsIgnoredAtomsVisible
        {
            get
            {
                return _IsIgnoredAtomsVisible;
            }
            set
            {
                if (value != _IsIgnoredAtomsVisible)
                {
                    _IsIgnoredAtomsVisible = value;
                    Notify("IsIgnoredAtomsVisible");
                }
            }
        }

        protected double _IgnoredAtomOpacity;
        /// <summary>
        /// Opacity (0.0-1.0] of ignored atoms
        /// </summary>
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage = "Opacity has to be in the interval [0.0, 1.0]")]
        public double IgnoredAtomOpacity 
        {
            get
            {
                return _IgnoredAtomOpacity;
            }
            set
            {
                ValidateNotify("IgnoredAtomOpacity", value, ref _IgnoredAtomOpacity);
            }
        }

        protected bool _IsAdditiveAtomsVisible;
        /// <summary>
        /// true = additive atoms are visible
        /// </summary>
        public bool IsAdditiveAtomsVisible
        {
            get
            {
                return _IsAdditiveAtomsVisible;
            }
            set
            {
                if (value != _IsAdditiveAtomsVisible)
                {
                    _IsAdditiveAtomsVisible = value;
                    Notify("IsAdditiveAtomsVisible");
                }
            }
        }

        protected double _AdditiveAtomOpacity;
        /// <summary>
        /// Opacity (0.0-1.0] of additive atoms
        /// </summary>
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage = "Opacity has to be in the interval [0.0, 1.0]")]
        public double AdditiveAtomOpacity 
        {
            get
            {
                return _AdditiveAtomOpacity;
            }
            set
            {
                ValidateNotify("AdditiveAtomOpacity", value, ref _AdditiveAtomOpacity);
            }
        }

        protected bool _IsInactiveAtomsVisible;
        /// <summary>
        /// true = inactive atoms are visible
        /// </summary>
        public bool IsInactiveAtomsVisible
        {
            get
            {
                return _IsInactiveAtomsVisible;
            }
            set
            {
                if (value != _IsInactiveAtomsVisible)
                {
                    _IsInactiveAtomsVisible = value;
                    Notify("IsInactiveAtomsVisible");
                }
            }
        }

        protected double _InactiveAtomOpacity;
        /// <summary>
        /// Opacity (0.0-1.0] of inactive atoms
        /// </summary>
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage = "Opacity has to be in the interval [0.0, 1.0]")]
        public double InactiveAtomOpacity
        {
            get
            {
                return _InactiveAtomOpacity;
            }
            set
            {
                ValidateNotify("InactiveAtomOpacity", value, ref _InactiveAtomOpacity);
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

        public TVM3DUniqueJumps(TMCViewModel ViewModel)
            : base()
        {
            // Save parent
            _ViewModel = ViewModel;

            // Model setup
            _Arrow = new ModelVisual3D();
            _ArrowModel = new GeometryModel3D();
            _Arrow.Content = _ArrowModel;

            // Field initializations
            _ArrowStart = new Vector3D(0.0, 0.0, 0.0);
            _ArrowEnd = new Vector3D(0.0, 0.0, 0.0);

            // Property initializations
            _UJumpList = new ObservableCollection<int>();
            _SelectedUJump = null;
            _IsArrowVisible = true;
            _ArrowDiameter = 0.2;
            _ArrowHeadLength = 3.0;
            _ArrowColor = Colors.Blue;
            _AtomScale = 0.5;
            _VacancyColor = Colors.DarkGray;
            _VacancyOpacity = 1.0;
            _IsIgnoredAtomsVisible = false;
            _IgnoredAtomOpacity = 1.0;
            _IsAdditiveAtomsVisible = true;
            _AdditiveAtomOpacity = 0.5;
            _IsInactiveAtomsVisible = true;
            _InactiveAtomOpacity = 1.0;
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
            _ArrowStart = new Vector3D(0.0, 0.0, 0.0);
            _ArrowEnd = new Vector3D(0.0, 0.0, 0.0);

            // Basic scene composition
            var light = new SunLight();
            light.Brightness = 0.8;
            _NonsortedVisual3Ds.Children.Add(light);

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
            double t_IgnoredAtomOpacity = 1.0;
            if (HasPropertyErrors("IgnoredAtomOpacity") == false) t_IgnoredAtomOpacity = _IgnoredAtomOpacity;
            double t_AdditiveAtomOpacity = 0.5;
            if (HasPropertyErrors("AdditiveAtomOpacity") == false) t_AdditiveAtomOpacity = _AdditiveAtomOpacity;
            double t_InactiveAtomOpacity = 1.0;
            if (HasPropertyErrors("InactiveAtomOpacity") == false) t_InactiveAtomOpacity = _InactiveAtomOpacity;

            // Save current unique jump selection
            int t_seljump = -1;
            if (_SelectedUJump != null) t_seljump = _SelectedUJump.Value;

            // Update jump selection collection
            UJumpList.Clear();
            if (_ViewModel.MCJob.UJumps.UJumps.Count > 0)
            {
                for (int i = 0; i < _ViewModel.MCJob.UJumps.UJumps.Count; i++)
                {
                    UJumpList.Add(i);
                }
            }

            // Compare jump selection and available jumps
            SelectedUJump = null;
            if (_UJumpList.Count > 0) {
                if ((t_seljump >= 0) && (t_seljump < _UJumpList.Count))
                {
                    SelectedUJump = t_seljump;
                }
                else
                {
                    SelectedUJump = 0;
                }
            }

            // Check for valid jump selection
            if (_SelectedUJump == null) return;

            // Get reference to selected jump
            TUniqueJump jump = _ViewModel.MCJob.UJumps.UJumps[_SelectedUJump.Value];

            // Define jump direction and up direction vector
            Vector3D t_jumpdir = new Vector3D(1.0, 0.0, 0.0);
            Vector3D t_updir = new Vector3D(0.0, 0.0, 1.0);
            t_jumpdir.Normalize();
            t_updir.Normalize();

            // Calculate arrow vectors
            _ArrowStart = -0.5 * jump.Length * t_jumpdir;
            _ArrowEnd = 0.5 * jump.Length * t_jumpdir;

            // Add atoms
            AtomUIElement3D atom3d = null;
            int counter = 0;

            // Add jump start atom/vacancy
            counter++;
            atom3d = new AtomUIElement3D();
            atom3d.MouseDown += this.Atom_MouseDown;
            atom3d.AtomID = counter;
            atom3d.AtomPosition = _ArrowStart;
            atom3d.AtomRadius = _ViewModel.MCJob.Elements.MovRadius * t_AtomScale;
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
                atom3d.AtomSymbol = _ViewModel.MCJob.Elements.MovSymbol;
                atom3d.AtomName = _ViewModel.MCJob.Elements.MovName;
                atom3d.AtomCharge = _ViewModel.MCJob.Elements.MovCharge;
                atom3d.AtomColor = _ViewModel.MCJob.Elements.MovColor;
                atom3d.AtomOpacity = 1.0;
            }
            _SortedVisual3Ds.Children.Add(atom3d);

            // Add jump destination atom/vacancy
            counter++;
            atom3d = new AtomUIElement3D();
            atom3d.MouseDown += this.Atom_MouseDown;
            atom3d.AtomID = counter;
            atom3d.AtomPosition = _ArrowEnd;
            atom3d.AtomRadius = _ViewModel.MCJob.Elements.MovRadius * t_AtomScale;
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
                atom3d.AtomSymbol = _ViewModel.MCJob.Elements.MovSymbol;
                atom3d.AtomName = _ViewModel.MCJob.Elements.MovName;
                atom3d.AtomCharge = _ViewModel.MCJob.Elements.MovCharge;
                atom3d.AtomColor = _ViewModel.MCJob.Elements.MovColor;
                atom3d.AtomOpacity = 1.0;
            }
            _SortedVisual3Ds.Children.Add(atom3d);

            // Add jump environment atoms
            Vector3D t_pos = new Vector3D(0.0, 0.0, 0.0);
            double t_opac = 1.0;
            if (jump.Coord.Count > 0)
            {
                for (int i = 0; i < jump.Coord.Count; i++)
                {
                    // Skip if additive, inactive or ignored
                    if ((_IsIgnoredAtomsVisible == false) && (jump.AtomIgnore[i] == true)) continue;
                    if ((_IsInactiveAtomsVisible == false) && (jump.AtomActive[i] == false)) continue;
                    if ((_IsAdditiveAtomsVisible == false) && (jump.AtomAdditive[i] == true)) continue;

                    // Add atom
                    counter++;
                    atom3d = new AtomUIElement3D();
                    atom3d.MouseDown += this.Atom_MouseDown;

                    atom3d.AtomID = counter;
                    atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[jump.ElemID[i]];
                    atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[jump.ElemID[i]];
                    atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[jump.ElemID[i]];
                    t_pos = jump.Coord[i].X * t_jumpdir;
                    if (jump.Coord[i].Y != 0)
                    {
                        t_pos += jump.Coord[i].Y * (t_jumpdir * Vector3D.DotProduct(t_jumpdir, t_updir) +
                            Vector3D.CrossProduct(Vector3D.CrossProduct(t_jumpdir, t_updir), t_jumpdir) * Math.Cos(Math.PI * jump.Coord[i].Z / 180.0) +
                            Vector3D.CrossProduct(t_jumpdir, t_updir) * Math.Sin(Math.PI * jump.Coord[i].Z / 180.0));
                    }
                    atom3d.AtomPosition = t_pos;
                    atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[jump.ElemID[i]];
                    t_opac = 1.0;
                    if (jump.AtomAdditive[i] == true) t_opac = t_AdditiveAtomOpacity;
                    if (jump.AtomActive[i] == false) t_opac = t_InactiveAtomOpacity;
                    if (jump.AtomIgnore[i] == true) t_opac = t_IgnoredAtomOpacity;
                    atom3d.AtomOpacity = t_opac;
                    atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[jump.ElemID[i]] * t_AtomScale;

                    _SortedVisual3Ds.Children.Add(atom3d);
                }
            }

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
