using System.Windows.Media;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;
using System;

namespace iCon_General
{
    public class TVM3DCrystal: TVM3DModel
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

        #endregion Internal Variables

        #region Properties

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
                    }

                }
                else
                {
                    if (_NonsortedVisual3Ds.Children.Contains(_Lattice) == true)
                    {
                        _NonsortedVisual3Ds.Children.Remove(_Lattice);
                    }
                }
                Notify("IsLatticeVisible");
            }
        }

        protected double _LatticeThickness;
        /// <summary>
        /// Thickness (Diameter) of the lattice tubes
        /// </summary>
        [DoubleRange(0.01, true, 1.0, true, ErrorMessage="Lattice thickness has to be in the interval [0.01, 1.0]")]
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

        protected int _UCCountA;
        /// <summary>
        /// Number of unit cells in a-direction
        /// </summary>
        [IntRange(1, true, 4, true, ErrorMessage="Number of unit cells in a-direction has to be in the interval [1, 4]")]
        public int UCCountA
        {
            get
            {
                return _UCCountA;
            }
            set
            {
                ValidateNotify("UCCountA", value, ref _UCCountA);
            }
        }

        protected int _UCCountB;
        /// <summary>
        /// Number of unit cells in b-direction
        /// </summary>
        [IntRange(1, true, 4, true, ErrorMessage = "Number of unit cells in b-direction has to be in the interval [1, 4]")]
        public int UCCountB
        {
            get
            {
                return _UCCountB;
            }
            set
            {
                ValidateNotify("UCCountB", value, ref _UCCountB);
            }
        }

        protected int _UCCountC;
        /// <summary>
        /// Number of unit cells in c-direction
        /// </summary>
        [IntRange(1, true, 4, true, ErrorMessage = "Number of unit cells in c-direction has to be in the interval [1, 4]")]
        public int UCCountC
        {
            get
            {
                return _UCCountC;
            }
            set
            {
                ValidateNotify("UCCountC", value, ref _UCCountC);
            }
        }

        protected double _AtomScale;
        /// <summary>
        /// Scaling factor (0,1] for atom radius
        /// </summary>
        [DoubleRange(0.0, false, 1.0, true, ErrorMessage="Scaling factor has to be in the interval (0.0, 1.0].")]
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

        #endregion Properties

        public TVM3DCrystal(TMCViewModel ViewModel)
            : base()
        {
            // Save parent
            _ViewModel = ViewModel;

            // Model initializations
            _Lattice = new ModelVisual3D();
            _LatticeModel = new GeometryModel3D();
            _Lattice.Content = _LatticeModel;

            // Field initializations
            _LatVecA = new Vector3D(0.0, 0.0, 0.0);
            _LatVecB = new Vector3D(0.0, 0.0, 0.0);
            _LatVecC = new Vector3D(0.0, 0.0, 0.0);

            // Property initializations
            _LatticeThickness = 0.05;
            _LatticeColor = Colors.Black;
            _IsLatticeVisible = true;
            _UCCountA = 1;
            _UCCountB = 1;
            _UCCountC = 1;
            _AtomScale = 0.5;
            _IsAdditionalAtomsVisible = true;

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

            // Basic scene composition
            var light = new SunLight();
            light.Brightness = 0.8;
            _NonsortedVisual3Ds.Children.Add(light);

            // Lattice update
            UpdateLatticeGeometry();
            UpdateLatticeMaterial();

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
            if (_ViewModel.MCJob.Structure.Coord.Count == 0) return;

            // Check for invalid parameters
            int t_UCCountA = 1;
            if (HasPropertyErrors("UCCountA") == false) t_UCCountA = _UCCountA;
            int t_UCCountB = 1;
            if (HasPropertyErrors("UCCountB") == false) t_UCCountB = _UCCountB;
            int t_UCCountC = 1;
            if (HasPropertyErrors("UCCountC") == false) t_UCCountC = _UCCountC;
            double t_AtomScale = 0.5;
            if (HasPropertyErrors("AtomScale") == false) t_AtomScale = _AtomScale;

            // Save lattice vectors
            _LatVecA = _ViewModel.MCJob.Structure.BasisA;
            _LatVecB = _ViewModel.MCJob.Structure.BasisB;
            _LatVecC = _ViewModel.MCJob.Structure.BasisC;

            // Add new atoms (incl. additional atoms = atoms on outer planes)
            AtomUIElement3D atom3d = null;
            int counter = 0;
            int borderA = t_UCCountA;
            int borderB = t_UCCountB;
            int borderC = t_UCCountC;
            if (_IsAdditionalAtomsVisible == true)
            {
                borderA++;
                borderB++;
                borderC++;
            }
            for (int a = 0; a < borderA; a++)
            {
                for (int b = 0; b < borderB; b++)
                {
                    for (int c = 0; c < borderC; c++)
                    {
                        for (int s = 0; s < _ViewModel.MCJob.Structure.Coord.Count; s++)
                        {
                            if ((a == t_UCCountA) && (_ViewModel.MCJob.Structure.Coord[s].X != 0)) continue;
                            if ((b == t_UCCountB) && (_ViewModel.MCJob.Structure.Coord[s].Y != 0)) continue;
                            if ((c == t_UCCountC) && (_ViewModel.MCJob.Structure.Coord[s].Z != 0)) continue;

                            counter++;
                            atom3d = new AtomUIElement3D();
                            atom3d.MouseDown += this.Atom_MouseDown;

                            atom3d.AtomID = counter;
                            atom3d.AtomSymbol = _ViewModel.MCJob.Elements.ElemSymbol[_ViewModel.MCJob.Structure.ElemID[s]];
                            atom3d.AtomName = _ViewModel.MCJob.Elements.ElemName[_ViewModel.MCJob.Structure.ElemID[s]];
                            atom3d.AtomCharge = _ViewModel.MCJob.Elements.ElemCharge[_ViewModel.MCJob.Structure.ElemID[s]];
                            atom3d.AtomPosition = (a + _ViewModel.MCJob.Structure.Coord[s].X) * _LatVecA + 
                                (b + _ViewModel.MCJob.Structure.Coord[s].Y) * _LatVecB +
                                (c + _ViewModel.MCJob.Structure.Coord[s].Z) * _LatVecC;
                            atom3d.AtomColor = _ViewModel.MCJob.Elements.ElemColor[_ViewModel.MCJob.Structure.ElemID[s]];
                            atom3d.AtomOpacity = 1.0;
                            atom3d.AtomRadius = _ViewModel.MCJob.Elements.ElemRadius[_ViewModel.MCJob.Structure.ElemID[s]] * t_AtomScale;

                            _SortedVisual3Ds.Children.Add(atom3d);
                        }
                    }
                }
            }
            
            // Lattice update
            UpdateLatticeGeometry();
            UpdateLatticeMaterial();

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
                (Vector3D.Equals(_LatVecA, _LatVecB) == true) ||
                (Vector3D.Equals(_LatVecB, _LatVecC) == true) ||
                (Vector3D.Equals(_LatVecA, _LatVecC) == true))
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
            for (int b = 0; b <= _UCCountB; b++)
            {
                for (int c = 0; c <= _UCCountC; c++)
                {
                    latticemeshbuilder.AddCylinder((b * _LatVecB + c * _LatVecC).ToPoint3D(),
                        (_UCCountA * _LatVecA + b * _LatVecB + c * _LatVecC).ToPoint3D(), t_LatticeThickness, 10);
                }
            }

            // Start points for lines in b-direction are in the a/c-plane
            for (int a = 0; a <= _UCCountA; a++)
            {
                for (int c = 0; c <= _UCCountC; c++)
                {
                    latticemeshbuilder.AddCylinder((a * _LatVecA + c * _LatVecC).ToPoint3D(),
                        (a * _LatVecA + _UCCountB * _LatVecB + c * _LatVecC).ToPoint3D(), t_LatticeThickness, 10);
                }
            }
            // Start points for lines in c-direction are in the a/b-plane
            for (int a = 0; a <= _UCCountA; a++)
            {
                for (int b = 0; b <= _UCCountB; b++)
                {
                    latticemeshbuilder.AddCylinder((a * _LatVecA + b * _LatVecB).ToPoint3D(),
                        (a * _LatVecA + b * _LatVecB + _UCCountC * _LatVecC).ToPoint3D(), t_LatticeThickness, 10);
                }
            }

            // Round corners:
            for (int a = 0; a <= 1; a++)
            {
                for (int b = 0; b <= 1; b++)
                {
                    for (int c = 0; c <= 1; c++)
                    {
                        latticemeshbuilder.AddSphere((a * _UCCountA * _LatVecA + b * _UCCountB * _LatVecB + c * _UCCountC * _LatVecC).ToPoint3D(), 
                            t_LatticeThickness / 2, 10, 10);
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

        #endregion Methods
    }
}
