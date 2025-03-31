using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;
using System.Windows.Media;
using System.Windows.Input;
using System.Windows.Controls;

namespace iCon_General
{
    /// <summary>
    /// Base class for all 3d models
    /// </summary>
    public class TVM3DModel: ViewModelObject
    {
        #region Internal Variables

        /// <summary>
        /// Visual3D-Container for always fully opaque objects (always first in rendering chain, no brushes with partial transparency allowed)
        /// </summary>
        protected readonly ContainerUIElement3D _NonsortedVisual3Ds;

        /// <summary>
        /// Visual3D-Container for possibly transparent objects (must be sorted if one or more object is transparent)
        /// </summary>
        protected readonly ContainerUIElement3D _SortedVisual3Ds;  

        /// <summary>
        /// Event listener for CompositionTarget.Rendering event of the viewport
        /// </summary>
        protected readonly RenderingEventListener _RenderingEventListener;

        /// <summary>
        /// Holds the current number of rendering ticks at the last sorting
        /// </summary>
        protected long _SortingStartTicks;

        /// <summary>
        /// List of selected atoms
        /// </summary>
        protected readonly List<AtomUIElement3D> _SelectedAtoms;

        #endregion Internal Variables

        #region Properties: Model

        protected ObservableCollection<ContainerUIElement3D> _Model;
        /// <summary>
        /// Total 3d model (binding to ItemsSource of HelixViewport3D)
        /// </summary>
        public ObservableCollection<ContainerUIElement3D> Model
        {
            get
            {
                return _Model;
            }
            set
            {
                if (value != _Model)
                {
                    _Model = value;
                    Notify("Model");
                }
            }
        }

        protected ProjectionCamera _DefaultCamera;
        /// <summary>
        /// Default camera (binding to DefaultCamera of HelixViewport3D)
        /// </summary>
        public ProjectionCamera DefaultCamera
        {
            get
            {
                return _DefaultCamera;
            }
            set
            {
                if (value != _DefaultCamera)
                {
                    _DefaultCamera = value;
                    Notify("DefaultCamera");
                }
            }
        }

        protected string _MeasurementResult;
        /// <summary>
        /// Contains the result of the atom selections
        /// </summary>
        public string MeasurementResult
        {
            get
            {
                return _MeasurementResult;
            }
            protected set
            {
                if (value != _MeasurementResult)
                {
                    _MeasurementResult = value;
                    Notify("MeasurementResult");
                }
            }
        }

        #endregion Properties: Model

        #region Properties: Control Flags

        protected bool _IsSorted;
        /// <summary>
        /// True = SortedVisual3Ds is sorted, false = not sorted
        /// </summary>
        public bool IsSorted
        {
            get
            {
                return _IsSorted;
            }
            protected set
            {
                if (value != _IsSorted)
                {
                    _IsSorted = value;
                    _SortingStartTicks = 0;
                    Notify("IsSorted");
                }
            }
        }

        protected bool _IsRendering;
        /// <summary>
        /// True = Geometry updates are performed before rendering, false = no updates
        /// </summary>
        public bool IsRendering
        {
            get
            {
                return _IsRendering;
            }
            protected set
            {
                if (value != _IsRendering)
                {
                    _IsRendering = value;
                    if (_IsRendering == true)
                    {
                        RenderingEventManager.AddListener(_RenderingEventListener);
                    }
                    else
                    {
                        RenderingEventManager.RemoveListener(_RenderingEventListener);
                    }
                    Notify("IsRendering");
                }
            }
        }

        protected bool _IsVisible;
        /// <summary>
        /// Flag, for transmitting if the corresponding viewport is visible (to prevent unneccessary rendering updates)
        /// </summary>
        public bool IsVisible
        {
            get
            {
                return _IsVisible;
            }
            set
            {
                if (value != _IsVisible)
                {
                    _IsVisible = value;
                    UpdateIsSorted();
                    UpdateIsRendering();
                    Notify("IsVisible");
                }
            }
        }

        #endregion Properties: Control Flags

        #region Properties: Model Options

        protected TAtomLabelMode _AtomLabelMode;
        /// <summary>
        /// Currently selected atom label mode
        /// </summary>
        public TAtomLabelMode AtomLabelMode
        {
            get
            {
                return _AtomLabelMode;
            }
            set
            {
                if (value != _AtomLabelMode)
                {
                    _AtomLabelMode = value;
                    UpdateAtomLabels();
                    Notify("AtomLabelMode");
                }
            }
        }

        protected Color _SelectedAtomColor;
        /// <summary>
        /// Color of the selected atoms
        /// </summary>
        public Color SelectedAtomColor
        {
            get
            {
                return _SelectedAtomColor;
            }
            set
            {
                if (value != _SelectedAtomColor)
                {
                    _SelectedAtomColor = value;
                    SelectAtom(-2);
                    Notify("SelectedAtomColor");
                }
            }
        }

        protected int _AtomLabelFontSize;
        /// <summary>
        /// Font size of the atom labels
        /// </summary>
        [IntRange(3, true, 10, true, ErrorMessage="Font size has to be in the interval [3, 10]")]
        public int AtomLabelFontSize
        {
            get
            {
                return _AtomLabelFontSize;
            }
            set
            {
                if (ValidateNotify("AtomLabelFontSize", value, ref _AtomLabelFontSize) == true)
                {
                    UpdateAtomLabels();
                }
            }
        }

        protected Color _AtomLabelColor;
        /// <summary>
        /// Foreground color of the atom labels
        /// </summary>
        public Color AtomLabelColor
        {
            get
            {
                return _AtomLabelColor;
            }
            set
            {
                if (value != _AtomLabelColor)
                {
                    _AtomLabelColor = value;
                    UpdateAtomLabels();
                    Notify("AtomLabelColor");
                }
            }
        }

        #endregion Properties: Model Options

        public TVM3DModel()
        {
            // Initialize rendering event listener
            _RenderingEventListener = new RenderingEventListener(this.OnCompositionTargetRendering);

            // Model Initializations
            _NonsortedVisual3Ds = new ContainerUIElement3D();
            _SortedVisual3Ds = new ContainerUIElement3D();
            _Model = new ObservableCollection<ContainerUIElement3D>();
            _DefaultCamera = null;
            _MeasurementResult = "";

            // Important: always fully opaque objects (nonsorted) first
            _Model.Add(_NonsortedVisual3Ds);
            _Model.Add(_SortedVisual3Ds);

            // Property Initialization
            _AtomLabelMode = TAtomLabelMode.None;
            _SelectedAtomColor = Colors.Gray;
            _AtomLabelColor = Colors.White;
            _AtomLabelFontSize = 6;
            _IsRendering = false;
            _IsVisible = false;
            _IsSorted = false;

            // Variable Initialization
            _SortingStartTicks = 0;
            _SelectedAtoms = new List<AtomUIElement3D>();
        }

        #region Methods

        /// <summary>
        /// Creates a proper oriented and zoomed default camera
        /// </summary>
        public void UpdateDefaultCamera()
        {
            // Create HelixToolkit default camera
            var cam = new OrthographicCamera();
            CameraHelper.Reset(cam);

            // Change camera properties
            //cam.LookDirection = new Vector3D(-1.944, 8.980, -1.793);
            cam.LookDirection = new Vector3D(-19.44, 89.80, -17.93);
            cam.UpDirection = new Vector3D(0.120, -0.617, 0.784);
            cam.Position = (-cam.LookDirection).ToPoint3D();

            // Set zoom
            ZoomExtents(cam);

            // Set default camera
            DefaultCamera = cam;
        }

        /// <summary>
        /// Sets zoom to model extents and makes camera look at the center of the model
        /// </summary>
        public void ZoomExtents(ProjectionCamera camera, double animationtime = 0)
        {
            // Check argument  
            if (camera == null) return;

            // Get viewport size
            double vpwidth = 1;
            double vpheight = 1;
            Viewport3D viewport = Visual3DHelper.GetViewport3D(_SortedVisual3Ds);
            if (viewport != null)
            {
                if ((viewport.ActualWidth != 0) && (viewport.ActualHeight != 0))
                {
                    vpwidth = viewport.ActualWidth;
                    vpheight = viewport.ActualHeight;
                }
            }

            // Calculate bounds (extended method from HelixToolkit CameraHelper)
            var bounds = Visual3DHelper.FindBounds(_SortedVisual3Ds.Children);
            bounds.Union(Visual3DHelper.FindBounds(_NonsortedVisual3Ds.Children));
            var diagonal = new Vector3D(bounds.SizeX, bounds.SizeY, bounds.SizeZ);
            if ((bounds.IsEmpty) || (diagonal.LengthSquared == 0))
            {
                return;
            }

            // Calculate center and radius (extended method from HelixToolkit CameraHelper)
            var center = bounds.Location + diagonal * 0.5;
            double radius = diagonal.Length * 0.5;

            // Zoom extents (extended method from HelixToolkit CameraHelper)
            if (camera is PerspectiveCamera)
            {
                var pcam = camera as PerspectiveCamera;
                double disth = radius / Math.Tan(0.5 * pcam.FieldOfView * Math.PI / 180);
                double vfov = pcam.FieldOfView / vpwidth * vpheight;
                double distv = radius / Math.Tan(0.5 * vfov * Math.PI / 180);

                double dist = Math.Max(disth, distv);
                var dir = camera.LookDirection;
                dir.Normalize();
                CameraHelper.LookAt(camera, center, dir * dist, animationtime);
            }

            if (camera is OrthographicCamera)
            {
                // Important change in contrast to HelixToolkit CameraHelper:
                // In CameraHelper.ZoomExtents there is no change of the camera-model distance.
                // This causes a problem when zooming into the model till near plane clipping
                // and then calling ZoomExtents. Because of the camera-model distance remaining the
                // same, the model remains partially clipped.
                // Solution: Like in the ZoomHandler.Zoom method of HelixToolkit, the camera-model distance
                // has to be changed according to the newWidth/oldWidth ratio

                // calculate zoom factor
                var ocam = camera as OrthographicCamera;
                double newWidth = radius * 2;
                if (vpwidth > vpheight)
                {
                    newWidth = radius * 2 * vpwidth / vpheight;
                }
                double zoomfactor = newWidth / ocam.Width;

                // change camera properties
                CameraHelper.LookAt(camera, center, camera.LookDirection*zoomfactor, animationtime);
                CameraHelper.AnimateWidth(camera as OrthographicCamera, newWidth, animationtime);
                /*
                CameraHelper.LookAt(camera, center, camera.LookDirection, animationtime);
                double newWidth = radius * 2;

                if (vpwidth > vpheight)
                {
                    newWidth = radius * 2 * vpwidth / vpheight;
                }

                CameraHelper.AnimateWidth(camera as OrthographicCamera, newWidth, animationtime);
                */
            }
        }

        /// <summary>
        /// Handles the mousedown event of all atoms
        /// </summary>
        protected void Atom_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                AtomUIElement3D atom3d = sender as AtomUIElement3D;
                if (atom3d != null)
                {
                    SelectAtom(atom3d.AtomID, atom3d);
                    e.Handled = true;
                }
            }
        }

        /// <summary> Manages the selection of an atom </summary>
        /// <param name="AtomID"> ID of the selected atom (-1 = unselect all, -2 = only update properties) </param>
        public void SelectAtom(int atomid, AtomUIElement3D sender = null)
        {
            AtomUIElement3D atom = sender;

            // Handle selection changes
            if (atomid < -2)
            {
                // No actions required
                return;
            }
            else if (atomid == -1)
            {
                // Deselect all atoms
                foreach (AtomUIElement3D atom3d in _SelectedAtoms)
                {
                    if (atom3d != null)
                    {
                        atom3d.SelectedBrush = null;
                    }
                }
                _SelectedAtoms.Clear();
            }
            else if (atomid == -2)
            {
                // Update all related atom properties according to selection
                foreach (AtomUIElement3D atom3d in _SelectedAtoms)
                {
                    if (atom3d != null)
                    {
                        atom3d.SelectedBrush = new SolidColorBrush(_SelectedAtomColor);
                    }
                }
            }
            else if (atomid >= 0)
            {
                // Find atom if not specified
                if (atom == null)
                {
                    AtomUIElement3D tempatom = null;
                    foreach (Visual3D vis3d in _SortedVisual3Ds.Children)
                    {
                        tempatom = vis3d as AtomUIElement3D;
                        if (tempatom != null)
                        {
                            if (tempatom.AtomID == atomid)
                            {
                                atom = tempatom;
                                break;
                            }
                        }
                    }
                }
                if (atom == null)
                {
                    return;
                }

                // Try to find this atom in selection
                int index = _SelectedAtoms.IndexOf(atom);
                if (index != -1)
                {
                    // If the last selected atom was selected again, it is deselected
                    if (index == _SelectedAtoms.Count - 1)
                    {
                        atom.SelectedBrush = null;
                        _SelectedAtoms.Remove(atom);
                    }
                    // If one of the non-last selected atoms was selected again, start new selection with this atom
                    else
                    {
                        foreach (AtomUIElement3D atom3d in _SelectedAtoms)
                        {
                            if (atom3d != null)
                            {
                                atom3d.SelectedBrush = null;
                            }
                        }
                        _SelectedAtoms.Clear();
                        atom.SelectedBrush = new SolidColorBrush(_SelectedAtomColor);
                        _SelectedAtoms.Add(atom);
                    }
                }
                else
                {
                    // If new atom was selected, add this atom if less than four are selected
                    if (_SelectedAtoms.Count < 4)
                    {
                        atom.SelectedBrush = new SolidColorBrush(_SelectedAtomColor);
                        _SelectedAtoms.Add(atom);
                    }
                    // If new atom was selected, start new collection with this atom if four are selected
                    else
                    {
                        foreach (AtomUIElement3D atom3d in _SelectedAtoms)
                        {
                            if (atom3d != null)
                            {
                                atom3d.SelectedBrush = null;
                            }
                        }
                        _SelectedAtoms.Clear();
                        atom.SelectedBrush = new SolidColorBrush(_SelectedAtomColor);
                        _SelectedAtoms.Add(atom);
                    }
                } 
            }
            
            // Update measurement text
            string newstring = "";
            switch (_SelectedAtoms.Count)
            {
                case 0:
                    newstring = "";
                    break;
                case 1:
                    newstring += "[" + _SelectedAtoms[0].AtomID.ToString() + "]: " + _SelectedAtoms[0].AtomSymbol + ", ";
                    if (string.IsNullOrWhiteSpace(_SelectedAtoms[0].AtomName) == false)
                    {
                        newstring += _SelectedAtoms[0].AtomName + ", ";
                    }
                    newstring += "q = " + string.Format("{0:0.##}", _SelectedAtoms[0].AtomCharge) + "e, ";
                    newstring += "p = " + string.Format("({0:0.00##} \u212B, {1:0.00##} \u212B, {2:0.00##} \u212B)", _SelectedAtoms[0].AtomPosition.X, 
                        _SelectedAtoms[0].AtomPosition.Y, _SelectedAtoms[0].AtomPosition.Z);
                    break;
                case 2:
                    newstring += "[" + _SelectedAtoms[0].AtomID.ToString() + "," + _SelectedAtoms[1].AtomID.ToString() + "]: ";
                    newstring += string.Format("d = {0:0.00##} \u212B", (_SelectedAtoms[0].AtomPosition - _SelectedAtoms[1].AtomPosition).Length);
                    break;
                case 3:
                    newstring += "[" + _SelectedAtoms[0].AtomID.ToString() + "," + _SelectedAtoms[1].AtomID.ToString() + ",";
                    newstring += _SelectedAtoms[2].AtomID.ToString() + "]: ";
                    newstring += string.Format("\u03B1 = {0:0.0##} \u00B0",
                        Vector3D.AngleBetween(_SelectedAtoms[0].AtomPosition - _SelectedAtoms[1].AtomPosition, _SelectedAtoms[2].AtomPosition - _SelectedAtoms[1].AtomPosition));
                    break;
                case 4:
                    newstring += "[" + _SelectedAtoms[0].AtomID.ToString() + "," + _SelectedAtoms[1].AtomID.ToString() + ",";
                    newstring += _SelectedAtoms[2].AtomID.ToString() + "," + _SelectedAtoms[3].AtomID.ToString() + "]: ";
                    Vector3D a = _SelectedAtoms[1].AtomPosition - _SelectedAtoms[0].AtomPosition;
                    Vector3D b = _SelectedAtoms[2].AtomPosition - _SelectedAtoms[1].AtomPosition;
                    Vector3D c = _SelectedAtoms[3].AtomPosition - _SelectedAtoms[2].AtomPosition;
                    newstring += string.Format("\u03B4 = {0:0.0##} \u00B0",
                        Math.Atan2(b.Length*Vector3D.DotProduct(a,Vector3D.CrossProduct(b,c)),Vector3D.DotProduct(Vector3D.CrossProduct(a,b),Vector3D.CrossProduct(b,c)))*180/Math.PI);
                    break;
                default:
                    throw new ApplicationException("Undefined selection state (in KMCData3DModel.SelectAtom)");
            }
            MeasurementResult = newstring;
        }

        /// <summary>
        /// Returns the number of AtomUIElement3Ds in the model
        /// </summary>
        /// <returns>Number of atoms in the model</returns>
        public int GetAtomCount()
        {
            int counter = 0;
            AtomUIElement3D atom3d = null;
            foreach (Visual3D vis3d in _SortedVisual3Ds.Children)
            {
                atom3d = vis3d as AtomUIElement3D;
                if (atom3d != null) counter++;
            }
            return counter;
        }

        /// <summary>
        /// Updates atom labels due to global label mode
        /// </summary>
        public void UpdateAtomLabels()
        {
            // Remove all atom labels from model
            LabelVisual3D label3d = null;
            for (int i = _SortedVisual3Ds.Children.Count - 1; i >= 0; i--)
            {
                label3d = _SortedVisual3Ds.Children[i] as LabelVisual3D;
                if (label3d != null)
                {
                    if (label3d.InternalType == "AtomLabel")
                    {
                        _SortedVisual3Ds.Children.RemoveAt(i);
                    }
                }
            }

            // Check for invalid parameters
            int t_FontSize = 6;
            if (HasPropertyErrors("AtomLabelFontSize") == false) t_FontSize = _AtomLabelFontSize;

            // Change atom label properties and add labels if mode != None
            AtomUIElement3D atom3d = null;
            for (int i = _SortedVisual3Ds.Children.Count - 1; i >= 0; i--)
            {
                atom3d = _SortedVisual3Ds.Children[i] as AtomUIElement3D;
                if (atom3d != null)
                {
                    if (atom3d.Label != null)
                    {
                        atom3d.Label.IsInContentEditingMode = true;
                        atom3d.Label.FontSize = t_FontSize;
                        atom3d.Label.Foreground = _AtomLabelColor;
                    }
                    atom3d.LabelMode = _AtomLabelMode;
                    if (atom3d.Label != null)
                    {
                        atom3d.Label.IsInContentEditingMode = false;
                        if (_AtomLabelMode != TAtomLabelMode.None)
                        {
                            _SortedVisual3Ds.Children.Add(atom3d.Label);
                            atom3d.Label.ForceGeometryUpdate();
                        }
                    }
                }
            }

            UpdateIsSorted();
            UpdateIsRendering();
        }

        /// <summary>
        /// Updates internal IsSorted flag for sorting of _SortedVisual3Ds
        /// </summary>
        public void UpdateIsSorted()
        {
            // Search for transparent objects
            AtomUIElement3D atom3d = null;
            foreach (Visual3D vis3d in _SortedVisual3Ds.Children)
            {
                atom3d = vis3d as AtomUIElement3D;
                if (atom3d != null)
                {
                    if (atom3d.AtomOpacity != 1.0)
                    {
                        IsSorted = true;
                        return;
                    }
                }
                if (vis3d is LabelVisual3D)
                {
                    IsSorted = true;
                    return;
                }
            }

            IsSorted = false;
        }

        /// <summary> 
        /// Updates IsRendering according to e.g. IsAtomLabelsVisible 
        /// </summary>
        public void UpdateIsRendering()
        {
            // Rendering only if this model is in a visible viewport
            if (_IsVisible == false)
            {
                IsRendering = false;
                return;
            }

            // Rendering only neccessary if this model is attached to some viewport
            if (Visual3DHelper.IsAttachedToViewport3D(_SortedVisual3Ds) == false)
            {
                IsRendering = false;
                return;
            }

            // Rendering neccessary, if there is sorting to do
            if (_IsSorted == true)
            {
                IsRendering = true;
                return;
            }

            // Search for objects that have to be rerendered
            foreach (Visual3D vis3d in _SortedVisual3Ds.Children)
            {
                if (vis3d is LabelVisual3D)
                {
                    IsRendering = true;
                    return;
                }
            }
        }

        /// <summary>
        /// Identifies if an visual3d object is transparent
        /// </summary>
        protected static bool IsTransparent(Visual3D visual)
        {
            // LabelVisual3Ds will always have transparent background
            if (visual is LabelVisual3D)
            {
                return true;
            }

            // AtomUIElement3Ds have an AtomOpacity property
            AtomUIElement3D atom3d = visual as AtomUIElement3D;
            if (atom3d != null)
            {
                if (atom3d.AtomOpacity != 1.0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            return ElementSortingHelper.IsTransparent(visual);
        }

        /// <summary> 
        /// Updates the label orientations (as billboards) 
        /// </summary>
        public void UpdateLabelOrientations()
        {
            LabelVisual3D label3d = null;
            foreach (Visual3D vis3d in _SortedVisual3Ds.Children)
            {
                label3d = vis3d as LabelVisual3D;
                if (label3d != null)
                {
                    label3d.UpdateGeometry();
                }              
            }
        }

        /// <summary>
        /// Sort all children of SortedVisual3Ds (for proper transparency handling)
        /// </summary>
        public void SortSortedVisual3Ds()
        {
            // This method is adapted from HelixToolkit: SortingVisual3D.SortChildren()

            // Get parent viewport
            Viewport3D vp = Visual3DHelper.GetViewport3D(_SortedVisual3Ds);
            if (vp == null)
            {
                return;
            }

            // Get camera of the viewport
            ProjectionCamera cam = vp.Camera as ProjectionCamera;
            if (cam == null)
            {
                return;
            }

            Point3D cameraPos = cam.Position;
            Transform3D transform = new MatrixTransform3D(CustomVisual3DHelper.GetTransform(_SortedVisual3Ds));

            IList<Visual3D> transparentChildren = new List<Visual3D>();
            IList<Visual3D> opaqueChildren = new List<Visual3D>();
            foreach (var child in _SortedVisual3Ds.Children)
            {
                if (IsTransparent(child))
                {
                    transparentChildren.Add(child);
                }
                else
                {
                    opaqueChildren.Add(child);
                }
            }

            // sort the children by distance from camera (note that OrderBy is a stable sort algorithm)
            var sortedTransparentChildren =
                transparentChildren.OrderBy(item => -CustomVisual3DHelper.GetCameraDistance(item, cameraPos, transform)).ToList();

            _SortedVisual3Ds.Children.Clear();

            // add the opaque children
            foreach (var c in opaqueChildren)
            {
                _SortedVisual3Ds.Children.Add(c);
            }

            // add the sorted transparent children
            foreach (var c in sortedTransparentChildren)
            {
                _SortedVisual3Ds.Children.Add(c);
            }
        }

        /// <summary> 
        /// Method that is called each time the viewport is rendered 
        /// </summary>
        protected void OnCompositionTargetRendering(object sender, RenderingEventArgs e)
        {
            if (_IsRendering == true)
            {
                UpdateLabelOrientations();
                if (_IsSorted == true)
                {
                    if (_SortingStartTicks == 0)
                    {
                        _SortingStartTicks = e.RenderingTime.Ticks;
                    }
                    double time = 100e-9 * (e.RenderingTime.Ticks - _SortingStartTicks);
                    if (time >= 1.0)     // = Time between sorting (in seconds)
                    {
                        _SortingStartTicks = e.RenderingTime.Ticks;
                        SortSortedVisual3Ds();
                    }
                }
            }
        }

        #endregion Methods
    }
}
