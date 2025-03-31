using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;

namespace iCon_General
{
    public class TVM3DCodes : TVM3DModel
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

        protected ObservableCollection<TVM3DMultiID> _UJumpList;
        /// <summary>
        /// List of unique jumps
        /// </summary>
        public ObservableCollection<TVM3DMultiID> UJumpList
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

        protected TVM3DMultiID _SelectedUJump;
        /// <summary>
        /// Selected unique jump
        /// </summary>
        public TVM3DMultiID SelectedUJump
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

        protected TVM3DMultiID _SelectedUCode;
        /// <summary>
        /// Selected unique code
        /// </summary>
        public TVM3DMultiID SelectedUCode
        {
            get
            {
                return _SelectedUCode;
            }
            set
            {
                if (value != _SelectedUCode)
                {
                    _SelectedUCode = value;
                    Notify("SelectedUCode");
                }
            }
        }

        protected int? _SelectedFullCode;
        /// <summary>
        /// Selected full code
        /// </summary>
        public int? SelectedFullCode
        {
            get
            {
                return _SelectedFullCode;
            }
            set
            {
                if (value != _SelectedFullCode)
                {
                    _SelectedFullCode = value;
                    Notify("SelectedFullCode");
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
        [DoubleRange(0.01, true, 1.0, true, ErrorMessage="Arrow diameter has to be in the interval [0.01, 1.0]")]
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
        [DoubleRange(0.1, true, 10.0, true, ErrorMessage="Arrow head length has to be in the interval [0.1, 10.0]")]
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
        [DoubleRange(0.0, false, 1.0, true, ErrorMessage="Scaling factor has to be in the interval (0.0, 1.0]")]
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
        [DoubleRange(0.0, true, 1.0, true, ErrorMessage="Opacity has to be in the interval [0.0, 1.0]")]
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

        public TVM3DCodes(TMCViewModel ViewModel)
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
            _UJumpList = new ObservableCollection<TVM3DMultiID>();
            _SelectedUJump = null;
            _SelectedUCode = null;
            _SelectedFullCode = null;
            _IsArrowVisible = true;
            _ArrowDiameter = 0.2;
            _ArrowHeadLength = 3.0;
            _ArrowColor = Colors.Blue;
            _AtomScale = 0.5;
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

            // Save current code selection
            int t_selujump = -1;
            if (_SelectedUJump != null) t_selujump = _SelectedUJump.ID;
            int t_selucode = -1;
            if (_SelectedUCode != null) t_selucode = _SelectedUCode.ID;
            int t_selfullcode = -1;
            if (_SelectedFullCode != null) t_selfullcode = _SelectedFullCode.Value;

            // Update jump selection collections
            UJumpList.Clear();
            if (_ViewModel.MCJob.UJumps.UJumps.Count > 0)
            {
                TVM3DMultiID t_ujump = null;
                for (int i = 0; i < _ViewModel.MCJob.UJumps.UJumps.Count; i++)
                {
                    t_ujump = new TVM3DMultiID();
                    t_ujump.ID = i;
                    if (_ViewModel.MCJob.UJumps.UJumps[i].UniqueCodes.Count > 0)
                    {
                        TVM3DMultiID t_ucode = null;
                        for (int j = 0; j < _ViewModel.MCJob.UJumps.UJumps[i].UniqueCodes.Count; j++)
                        {
                            t_ucode = new TVM3DMultiID();
                            t_ucode.ID = j;
                            if (_ViewModel.MCJob.UJumps.UJumps[i].FullToUniqueMap.Count > 0)
                            {
                                for (int k = 0; k < _ViewModel.MCJob.UJumps.UJumps[i].FullToUniqueMap.Count; k++)
                                {
                                    if (_ViewModel.MCJob.UJumps.UJumps[i].FullToUniqueMap[k] == j) t_ucode.NextIDs.Add(k);
                                }
                            }

                            t_ujump.NextMultiIDs.Add(t_ucode);
                        }
                    }
                    UJumpList.Add(t_ujump);
                }
            }

            // Compare jump selection and available jumps
            SelectedUJump = null;
            SelectedUCode = null;
            SelectedFullCode = null;
            if (_UJumpList.Count > 0)
            {
                if ((t_selujump >= 0) && (t_selujump < _UJumpList.Count))
                {
                    SelectedUJump = _UJumpList[t_selujump];
                }
                else
                {
                    SelectedUJump = _UJumpList[0];
                }
            }
            if (_SelectedUJump != null)
            {
                if (_SelectedUJump.NextMultiIDs.Count > 0)
                {
                    if ((t_selucode >= 0) && (t_selucode < _SelectedUJump.NextMultiIDs.Count))
                    {
                        SelectedUCode = _SelectedUJump.NextMultiIDs[t_selucode];
                    }
                    else
                    {
                        SelectedUCode = _SelectedUJump.NextMultiIDs[0];
                    }
                }
            }
            if (_SelectedUCode != null)
            {
                if (_SelectedUCode.NextIDs.Count > 0)
                {
                    if (_SelectedUCode.NextIDs.Contains(t_selfullcode) == true)
                    {
                        SelectedFullCode = t_selfullcode;
                    }
                    else
                    {
                        SelectedFullCode = _SelectedUCode.NextIDs[0];
                    }
                }
            }

            // Check for valid code selection
            if ((_SelectedUJump == null) || (_SelectedUCode == null) || (_SelectedFullCode == null)) return;

            // Get reference to selected jump
            TUniqueJump jump = _ViewModel.MCJob.UJumps.UJumps[_SelectedUJump.ID];

            // Analyse selected (full) code
            List<List<int>> t_codeanalysis = TUniqueJump.SplitCode(jump.FullCodes[_SelectedFullCode.Value], _ViewModel.MCJob.Elements.ElemSymbol.Count - 1);
            List<int> code = t_codeanalysis[0];

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
            int t_codecounter = -1;
            if (jump.Coord.Count > 0)
            {
                for (int i = 0; i < jump.Coord.Count; i++)
                {
                    // Skip if additive or ignored
                    if ((jump.AtomIgnore[i] == true) || (jump.AtomAdditive[i] == true)) continue;

                    // Add atom
                    counter++;
                    t_codecounter++;
                    atom3d = new AtomUIElement3D();
                    atom3d.MouseDown += this.Atom_MouseDown;

                    atom3d.AtomID = counter;
                    atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[code[t_codecounter]];
                    atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[code[t_codecounter]];
                    atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[code[t_codecounter]];
                    t_pos = jump.Coord[i].X * t_jumpdir;
                    if (jump.Coord[i].Y != 0)
                    {
                        t_pos += jump.Coord[i].Y * (t_jumpdir * Vector3D.DotProduct(t_jumpdir, t_updir) +
                            Vector3D.CrossProduct(Vector3D.CrossProduct(t_jumpdir, t_updir), t_jumpdir) * Math.Cos(Math.PI * jump.Coord[i].Z / 180.0) +
                            Vector3D.CrossProduct(t_jumpdir, t_updir) * Math.Sin(Math.PI * jump.Coord[i].Z / 180.0));
                    }
                    atom3d.AtomPosition = t_pos;
                    atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[code[t_codecounter]];
                    atom3d.AtomOpacity = 1.0;
                    atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[code[t_codecounter]] * t_AtomScale;

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
