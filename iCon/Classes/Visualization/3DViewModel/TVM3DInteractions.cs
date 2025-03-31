using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;

namespace iCon_General
{
    public class TVM3DInteractions: TVM3DModel
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
        /// Visual3D that contains the interaction arrows
        /// </summary>
        protected readonly ModelVisual3D _InteractionArrows;

        /// <summary>
        /// Model3D that contains the interaction arrows
        /// </summary>
        protected readonly GeometryModel3D _InteractionArrowsModel;

        /// <summary>
        /// Vector to the start of the jump arrow
        /// </summary>
        protected Vector3D _ArrowStart;

        /// <summary>
        /// Vector to the end of the jump arrow
        /// </summary>
        protected Vector3D _ArrowEnd;

        /// <summary>
        /// Vector to the interacting atom
        /// </summary>
        protected Vector3D _InteractionPos;

        #endregion Internal Variables

        #region Properties

        protected ObservableCollection<TVM3DMultiID> _InteractionList;
        /// <summary>
        /// List of interaction IDs
        /// </summary>
        public ObservableCollection<TVM3DMultiID> InteractionList
        {
            get
            {
                return _InteractionList;
            }
            set
            {
                if (value != _InteractionList)
                {
                    _InteractionList = value;
                    Notify("InteractionList");
                }
            }
        }

        protected TVM3DMultiID _SelectedInteraction;
        /// <summary>
        /// Selected interaction
        /// </summary>
        public TVM3DMultiID SelectedInteraction
        {
            get
            {
                return _SelectedInteraction;
            }
            set
            {
                if (value != _SelectedInteraction)
                {
                    _SelectedInteraction = value;
                    Notify("SelectedInteraction");
                }
            }
        }

        protected int? _SelectedUJump;
        /// <summary>
        /// Selected unique jump (one interaction might belong to atoms of different unique jumps; this is a rare case but possible)
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

        protected bool _IsWWArrowVisible;
        /// <summary>
        /// Show/hide interaction arrows
        /// </summary>
        public bool IsWWArrowVisible
        {
            get
            {
                return _IsWWArrowVisible;
            }
            set
            {
                _IsWWArrowVisible = value;
                if (_IsWWArrowVisible == true)
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_InteractionArrows) == false)
                    {
                        _NonsortedVisual3Ds.Children.Add(_InteractionArrows);
                        UpdateDefaultCamera();
                    }

                }
                else
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_InteractionArrows) == true)
                    {
                        _NonsortedVisual3Ds.Children.Remove(_InteractionArrows);
                        UpdateDefaultCamera();
                    }
                }
                Notify("IsWWArrowVisible");
            }
        }

        protected double _WWArrowDiameter;
        /// <summary>
        /// Diameter of the interaction arrows
        /// </summary>
        [DoubleRange(0.01, true, 1.0, true, ErrorMessage = "Arrow diameter has to be in the interval [0.01, 1.0]")]
        public double WWArrowDiameter
        {
            get
            {
                return _WWArrowDiameter;
            }
            set
            {
                if (ValidateNotify("WWArrowDiameter", value, ref _WWArrowDiameter) == true)
                {
                    UpdateInteractionArrowGeometry();
                }
            }
        }

        protected double _WWArrowHeadLength;
        /// <summary>
        /// Head length of the interaction arrows
        /// </summary>
        [DoubleRange(0.1, true, 10.0, true, ErrorMessage = "Arrow head length has to be in the interval [0.1, 10.0]")]
        public double WWArrowHeadLength
        {
            get
            {
                return _WWArrowHeadLength;
            }
            set
            {
                if (ValidateNotify("WWArrowHeadLength", value, ref _WWArrowHeadLength) == true)
                {
                    UpdateInteractionArrowGeometry();
                }
            }
        }

        protected Color _WWArrowColor;
        /// <summary>
        /// Color of the arrow
        /// </summary>
        public Color WWArrowColor
        {
            get
            {
                return _WWArrowColor;
            }
            set
            {
                if (value != _WWArrowColor)
                {
                    _WWArrowColor = value;
                    UpdateInteractionArrowMaterial();
                    Notify("WWArrowColor");
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

        protected bool _IsStartEnvVisible;
        /// <summary>
        /// true = atoms, that belong to the environment of the interaction with the jump start position, are visible
        /// </summary>
        public bool IsStartEnvVisible
        {
            get
            {
                return _IsStartEnvVisible;
            }
            set
            {
                if (value != _IsStartEnvVisible)
                {
                    _IsStartEnvVisible = value;
                    Notify("IsStartEnvVisible");
                }
            }
        }

        protected bool _IsDestEnvVisible;
        /// <summary>
        /// true = atoms, that belong to the environment of the interaction with the jump destination position, are visible
        /// </summary>
        public bool IsDestEnvVisible
        {
            get
            {
                return _IsDestEnvVisible;
            }
            set
            {
                if (value != _IsDestEnvVisible)
                {
                    _IsDestEnvVisible = value;
                    Notify("IsDestEnvVisible");
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

        public TVM3DInteractions(TMCViewModel ViewModel)
            : base()
        {
            // Save parent
            _ViewModel = ViewModel;

            // Model setup
            _Arrow = new ModelVisual3D();
            _ArrowModel = new GeometryModel3D();
            _Arrow.Content = _ArrowModel;
            _InteractionArrows = new ModelVisual3D();
            _InteractionArrowsModel = new GeometryModel3D();
            _InteractionArrows.Content = _InteractionArrowsModel;

            // Field initializations
            _ArrowStart = new Vector3D(0.0, 0.0, 0.0);
            _ArrowEnd = new Vector3D(0.0, 0.0, 0.0);
            _InteractionPos = new Vector3D(0.0, 0.0, 0.0);

            // Property initializations
            _InteractionList = new ObservableCollection<TVM3DMultiID>();
            _SelectedInteraction = null;
            _SelectedUJump = null;
            _IsArrowVisible = true;
            _ArrowDiameter = 0.2;
            _ArrowHeadLength = 3.0;
            _ArrowColor = Colors.Blue;
            _IsWWArrowVisible = true;
            _WWArrowDiameter = 0.1;
            _WWArrowHeadLength = 2.0;
            _WWArrowColor = Colors.Red;
            _AtomScale = 0.5;
            _VacancyColor = Colors.DarkGray;
            _VacancyOpacity = 1.0;
            _IsStartEnvVisible = true;
            _IsDestEnvVisible = true;
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
            _InteractionPos = new Vector3D(0.0, 0.0, 0.0);

            // Basic scene composition
            var light = new SunLight();
            light.Brightness = 0.8;
            _NonsortedVisual3Ds.Children.Add(light);

            // Arrow update
            UpdateArrowGeometry();
            UpdateArrowMaterial();

            // Interaction arrow update
            UpdateInteractionArrowGeometry();
            UpdateInteractionArrowMaterial();

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

            // Save current interaction selection
            int t_selint = -1;
            if (_SelectedInteraction != null) t_selint = _SelectedInteraction.ID;
            int t_selujump = -1;
            if (_SelectedUJump != null) t_selujump = _SelectedUJump.Value;

            // Update interaction collection
            InteractionList.Clear();
            if (_ViewModel.MCJob.UJumps.Interactions.Count > 0)
            {
                TVM3DMultiID t_inter = null;
                for (int i = 0; i < _ViewModel.MCJob.UJumps.Interactions.Count; i++)
                {
                    t_inter = new TVM3DMultiID();
                    t_inter.ID = i;

                    if (_ViewModel.MCJob.UJumps.UJumps.Count > 0)
                    {
                        bool t_found = false;
                        for (int j = 0; j < _ViewModel.MCJob.UJumps.UJumps.Count; j++)
                        {
                            t_found = false;
                            if (_ViewModel.MCJob.UJumps.UJumps[j].InteractionMap.Count > 0)
                            {
                                for (int k = 0; k < _ViewModel.MCJob.UJumps.UJumps[j].InteractionMap.Count; k++)
                                {
                                    if (_ViewModel.MCJob.UJumps.UJumps[j].InteractionMap[k] == i)
                                    {
                                        t_found = true;
                                        break;
                                    }
                                }
                            }
                            if (t_found == true)
                            {
                                t_inter.NextIDs.Add(j);
                            }
                        }
                    }

                    InteractionList.Add(t_inter);
                }
            }

            // Compare interaction selection and available interactions
            SelectedInteraction = null;
            SelectedUJump = null;
            if (_InteractionList.Count > 0) {
                if ((t_selint >= 0) && (t_selint < _InteractionList.Count))
                {
                    SelectedInteraction = _InteractionList[t_selint];
                }
                else
                {
                    SelectedInteraction = _InteractionList[0];
                }
            }
            if (_SelectedInteraction != null)
            {
                if (_SelectedInteraction.NextIDs.Count > 0)
                {
                    if (_SelectedInteraction.NextIDs.Contains(t_selujump) == true)
                    {
                        SelectedUJump = t_selujump;
                    }
                    else
                    {
                        SelectedUJump = _SelectedInteraction.NextIDs[0];
                    }
                }
            }

            // Check for valid interaction selection
            if ((_SelectedInteraction == null) || (_SelectedUJump == null)) return;

            // Get reference to selected interaction
            TInteraction interaction = _ViewModel.MCJob.UJumps.Interactions[_SelectedInteraction.ID];
            if ((interaction.StartDist == 0) || (interaction.DestDist == 0)) return;

            // Define jump direction, up direction and normal vector (all perpendicular!)
            // -> Interacting atom resides in the plane that is defined by jump direction and up direction
            // -> Environment atoms are tranformed with the normal vector
            Vector3D t_jumpdir = new Vector3D(1.0, 0.0, 0.0);
            Vector3D t_updir = new Vector3D(0.0, 0.0, 1.0);
            Vector3D t_normvec = new Vector3D(0.0, 1.0, 0.0);
            t_jumpdir.Normalize();
            t_updir.Normalize();
            t_normvec.Normalize();

            // Calculate arrow vectors
            double t_jumplength = _ViewModel.MCJob.UJumps.UJumps[_SelectedUJump.Value].Length;
            if (t_jumplength == 0.0) return;
            _ArrowStart = -0.5 * t_jumplength * t_jumpdir;
            _ArrowEnd = 0.5 * t_jumplength * t_jumpdir;
            if (interaction.StartDist + interaction.DestDist <= t_jumplength)           // Triangle inequality (interacting atom resides on jump vector)
            {
                _InteractionPos = (0.5 * t_jumplength - interaction.DestDist) * t_jumpdir;
            }
            else if (interaction.StartDist + t_jumplength <= interaction.DestDist)      // Triangle inequality (interacting atom resides on jump vector)
            {
                _InteractionPos = -(0.5*t_jumplength + interaction.StartDist) * t_jumpdir;
            }
            else if (interaction.DestDist + t_jumplength <= interaction.StartDist)      // Triangle inequality (interacting atom resides on jump vector)
            {
                _InteractionPos = (0.5 * t_jumplength + interaction.DestDist) * t_jumpdir;
            }
            else   // -> valid triangle
            {
                // Law of cosines (for angle at start position)
                double t_cos = (interaction.StartDist * interaction.StartDist + t_jumplength * t_jumplength -
                    interaction.DestDist * interaction.DestDist) / (2 * interaction.StartDist * t_jumplength);
                if (t_cos > 1.0) t_cos = 1.0;      // Prevent exceptions from rounding (negative square root in next line)
                double t_sin = Math.Sqrt(1 - t_cos * t_cos);

                _InteractionPos = (interaction.StartDist * t_cos - 0.5 * t_jumplength) * t_jumpdir + interaction.StartDist * t_sin * t_updir;
            }

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

            // Add interacting atom
            counter++;
            atom3d = new AtomUIElement3D();
            atom3d.MouseDown += this.Atom_MouseDown;
            atom3d.AtomID = counter;
            atom3d.AtomPosition = _InteractionPos;
            atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[interaction.ElemID] * t_AtomScale;
            atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[interaction.ElemID];
            atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[interaction.ElemID];
            atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[interaction.ElemID];
            atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[interaction.ElemID];
            atom3d.AtomOpacity = 1.0;
            _SortedVisual3Ds.Children.Add(atom3d);

            // Combine interaction environments (if necessary) and calculate positions
            Vector3D t_pos = new Vector3D(0.0, 0.0, 0.0);
            Vector3D t_wwvec = new Vector3D(0.0, 0.0, 0.0);
            List<Vector3D> t_coord = new List<Vector3D>();
            List<int> t_elemid = new List<int>();
            if ((_IsStartEnvVisible == true) && (interaction.StartWWCoord.Count > 0))
            {
                t_wwvec = _InteractionPos - _ArrowStart;
                t_wwvec.Normalize();
                for (int i = 0; i < interaction.StartWWCoord.Count; i++)
                {
                    t_pos = _ArrowStart + 0.5 * (_InteractionPos - _ArrowStart);
                    t_pos += interaction.StartWWCoord[i].X * t_wwvec;
                    if (interaction.StartWWCoord[i].Y != 0)
                    {
                        t_pos += interaction.StartWWCoord[i].Y * (t_wwvec * Vector3D.DotProduct(t_wwvec, t_normvec) +
                            Vector3D.CrossProduct(Vector3D.CrossProduct(t_wwvec, t_normvec), t_wwvec) * Math.Cos(Math.PI * interaction.StartWWCoord[i].Z / 180.0) +
                            Vector3D.CrossProduct(t_wwvec, t_normvec) * Math.Sin(Math.PI * interaction.StartWWCoord[i].Z / 180.0));
                    }

                    if ((t_pos - _ArrowEnd).Length < 0.001 * _ViewModel.MCJob.Elements.MovRadius) continue;

                    t_coord.Add(t_pos);
                    t_elemid.Add(interaction.StartWWElemID[i]);
                }
            }
            if ((_IsDestEnvVisible == true) && (interaction.DestWWCoord.Count > 0))
            {
                t_wwvec = _InteractionPos - _ArrowEnd;
                t_wwvec.Normalize();
                int t_start_env_count = t_coord.Count;
                bool t_found = false;
                for (int i = 0; i < interaction.DestWWCoord.Count; i++)
                {
                    t_pos = _ArrowEnd + 0.5 * (_InteractionPos - _ArrowEnd);
                    t_pos += interaction.DestWWCoord[i].X * t_wwvec;
                    if (interaction.DestWWCoord[i].Y != 0)
                    {
                        t_pos += interaction.DestWWCoord[i].Y * (t_wwvec * Vector3D.DotProduct(t_wwvec, t_normvec) +
                            Vector3D.CrossProduct(Vector3D.CrossProduct(t_wwvec, t_normvec), t_wwvec) * Math.Cos(Math.PI * interaction.DestWWCoord[i].Z / 180.0) +
                            Vector3D.CrossProduct(t_wwvec, t_normvec) * Math.Sin(Math.PI * interaction.DestWWCoord[i].Z / 180.0));
                    }

                    if ((t_pos - _ArrowStart).Length < 0.001 * _ViewModel.MCJob.Elements.MovRadius) continue;

                    if (t_start_env_count > 0)
                    {
                        t_found = false;
                        for (int j = 0; j < t_start_env_count; j++)
                        {
                            if ((t_pos - t_coord[j]).Length < 0.001 * _ViewModel.MCJob.Elements.ElemRadius[t_elemid[j]])
                            {
                                t_found = true;
                                break;
                            }
                        }
                        if (t_found == true) continue;
                    }

                    t_coord.Add(t_pos);
                    t_elemid.Add(interaction.DestWWElemID[i]);
                }
            }

            // Add interaction environment atoms
            if (t_coord.Count > 0)
            {
                for (int i = 0; i < t_coord.Count; i++)
                {
                    // Add atom
                    counter++;
                    atom3d = new AtomUIElement3D();
                    atom3d.MouseDown += this.Atom_MouseDown;

                    atom3d.AtomID = counter;
                    atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[t_elemid[i]];
                    atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[t_elemid[i]];
                    atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[t_elemid[i]];
                    atom3d.AtomPosition = t_coord[i];
                    atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[t_elemid[i]];
                    atom3d.AtomOpacity = 1.0;
                    atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[t_elemid[i]] * t_AtomScale;

                    _SortedVisual3Ds.Children.Add(atom3d);
                }
            }

            // Arrow update
            UpdateArrowGeometry();
            UpdateArrowMaterial();

            // Interaction arrow update
            UpdateInteractionArrowGeometry();
            UpdateInteractionArrowMaterial();

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

        /// <summary>
        /// Updates the shape of the interaction arrows
        /// </summary>
        public void UpdateInteractionArrowGeometry()
        {
            // Test if valid arrow points are present
            if ((Vector3D.Equals(_ArrowStart, _ArrowEnd) == true) ||
                (Vector3D.Equals(_ArrowStart, _InteractionPos) == true) ||
                (Vector3D.Equals(_ArrowEnd, _InteractionPos) == true))
            {
                _InteractionArrowsModel.Geometry = null;
                return;
            }

            // Check for invalid parameters
            double t_WWArrowDiameter = 0.1;
            if (HasPropertyErrors("WWArrowDiameter") == false) t_WWArrowDiameter = _WWArrowDiameter;
            double t_WWArrowHeadLength = 2.0;
            if (HasPropertyErrors("WWArrowHeadLength") == false) t_WWArrowHeadLength = _WWArrowHeadLength;

            // Create and assign arrow meshes:
            var arrowmeshbuilder = new CustomMeshBuilder();
            arrowmeshbuilder.AddDoubleArrow(_ArrowStart.ToPoint3D(), _InteractionPos.ToPoint3D(), t_WWArrowDiameter, t_WWArrowHeadLength, 10);
            arrowmeshbuilder.AddDoubleArrow(_ArrowEnd.ToPoint3D(), _InteractionPos.ToPoint3D(), t_WWArrowDiameter, t_WWArrowHeadLength, 10);

            // Set arrow geometry (and add to model if necessary)
            _InteractionArrowsModel.Geometry = arrowmeshbuilder.ToMesh();
            IsWWArrowVisible = _IsWWArrowVisible;

            // Update correctly zoomed default camera
            UpdateDefaultCamera();
        }

        /// <summary>
        /// Updates the color of the interaction arrows
        /// </summary>
        public void UpdateInteractionArrowMaterial()
        {
            _InteractionArrowsModel.Material = MaterialHelper.CreateMaterial(_WWArrowColor);
        }

        #endregion Methods
    }
}