using System.Windows;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;
using System.Windows.Media;
using System.ComponentModel;

namespace iCon_General
{
    /// <summary>
    /// Enumerated type for label composition
    /// </summary>
    [TypeConverter(typeof(EnumTypeConverter))]
    public enum TAtomLabelMode
    {
        [EnumDisplayName("None")]
        None = 0,
        [EnumDisplayName("ID")]
        ID = 1,
        [EnumDisplayName("Symbol")]
        Symbol = 2,
        [EnumDisplayName("Symbol+ID")]
        SymbolID = 3,
        [EnumDisplayName("Name")]
        Name = 4,
        [EnumDisplayName("Charge")]
        Charge = 5
    }

    /// <summary> 
    /// 3D representation of an atom which can be selected and which hosts a label
    /// </summary>
    public class AtomUIElement3D: UIElement3D
    {
        #region Internal Variables

        // Model3D for the atom
        protected GeometryModel3D _AtomModel;

        #endregion Internal Variables

        #region Properties

        protected int _AtomID;
        /// <summary>
        /// Identification number of this atom
        /// </summary>
        public int AtomID
        {
            get
            {
                return _AtomID;
            }
            set
            {
                if (value != _AtomID)
                {
                    _AtomID = value;
                    UpdateLabelText();
                }
            }
        }

        protected string _AtomSymbol;
        /// <summary>
        /// Symbol of the atom
        /// </summary>
        public string AtomSymbol
        {
            get
            {
                return _AtomSymbol;
            }
            set
            {
                if (value != _AtomSymbol)
                {
                    _AtomSymbol = value;
                    UpdateLabelText();
                }
            }
        }

        protected string _AtomName;
        /// <summary>
        /// Name of the atom
        /// </summary>
        public string AtomName
        {
            get
            {
                return _AtomName;
            }
            set
            {
                if (value != _AtomName)
                {
                    _AtomName = value;
                    UpdateLabelText();
                }
            }
        }

        protected double _AtomCharge;
        /// <summary>
        /// Charge of the atom
        /// </summary>
        public double AtomCharge
        {
            get
            {
                return _AtomCharge;
            }
            set
            {
                if (value != _AtomCharge)
                {
                    _AtomCharge = value;
                    UpdateLabelText();
                }
            }
        }

        protected Color _AtomColor;
        /// <summary>
        /// Color of this atom
        /// </summary>
        public Color AtomColor
        {
            get
            {
                return _AtomColor;
            }
            set
            {
                if (!_AtomColor.Equals(value))
                {
                    _AtomColor = value;
                    _AtomColor.A = (byte)(_AtomOpacity * 255);
                    UpdateAtomColor();
                }
            }
        }

        protected double _AtomOpacity;
        /// <summary>
        /// Opacity of this atom
        /// </summary>
        public double AtomOpacity
        {
            get
            {
                return _AtomOpacity;
            }
            set
            {
                if (value != _AtomOpacity)
                {
                    _AtomOpacity = value;
                    _AtomColor.A = (byte)(_AtomOpacity * 255);
                    UpdateAtomOpacity();
                }
            }
        }

        protected double _AtomRadius;
        /// <summary>
        /// Radius of this atom
        /// </summary>
        public double AtomRadius
        {
            get
            {
                return _AtomRadius;
            }
            set
            {
                if (value != _AtomRadius)
                {
                    _AtomRadius = value;
                    UpdateAtomGeometry();
                    if (_Label != null)
                    {
                        // set label offset to 110 % of radius (to prevent clipping between sphere and label plane)
                        _Label.DepthOffset = _AtomRadius * 1.1;
                    }
                }
            }
        }

        protected Vector3D _AtomPosition;
        /// <summary>
        /// Position of this atom (for translate transform)
        /// </summary>
        public Vector3D AtomPosition
        {
            get
            {
                return _AtomPosition;
            }
            set
            {
                if (!_AtomPosition.Equals(value))
                {
                    _AtomPosition = value;
                    UpdateAtomPosition();
                    if (_Label != null)
                    {
                        _Label.Position = _AtomPosition;
                    }
                }
            }
        }

        protected TAtomLabelMode _LabelMode;
        /// <summary>
        /// Label mode for the hosted label
        /// </summary>
        public TAtomLabelMode LabelMode
        {
            get
            {
                return _LabelMode;
            }
            set
            {
                if (value != _LabelMode)
                {
                    _LabelMode = value;
                    UpdateLabelText();
                }
            }
        }

        protected LabelVisual3D _Label;
        /// <summary>
        /// Hosted label for this atom (not included in the visual model of this object)
        /// </summary>
        public LabelVisual3D Label
        {
            get
            {
                return _Label;
            }
            protected set
            {
                if (value != _Label)
                {
                    _Label = value;
                    if (_Label != null)
                    {
                        _Label.InternalType = "AtomLabel";
                        _Label.Position = _AtomPosition;
                        _Label.DepthOffset = _AtomRadius * 1.1;
                    }
                    UpdateLabelText();
                }
            }
        }

        protected Brush _SelectedBrush;
        /// <summary>
        /// != null if atom is selected (atom gets the specified color)
        /// </summary>
        public Brush SelectedBrush
        {
            get
            {
                return _SelectedBrush;
            }
            set
            {
                if (value != _SelectedBrush)
                {
                    _SelectedBrush = value;
                    UpdateAtomColor();
                }
            }
        }

        #endregion Properties

        /// <summary>
        /// Create a new 3d atom
        /// </summary>
        public AtomUIElement3D()
        {
            // Property Initialization
            _AtomID = -1;
            _AtomSymbol = "";
            _AtomName = "";
            _AtomCharge = 0.0;
            _AtomColor = Colors.Blue;
            _SelectedBrush = null;
            _AtomOpacity = 1.0;
            _AtomRadius = 1.0;
            _AtomPosition = new Vector3D(0.0, 0.0, 0.0);
            _LabelMode = TAtomLabelMode.None;

            // Model Initialization
            _AtomModel = new GeometryModel3D();
            this.Visual3DModel = _AtomModel;
            UpdateAtomGeometry();
            UpdateAtomColor();

            // Label Initialization
            Label = new LabelVisual3D();
        }

        #region Methods

        /// <summary>
        /// Update the mesh of the atom (e.g. due to size change)
        /// </summary>
        public void UpdateAtomGeometry()
        {
            var atommeshbuilder = new MeshBuilder();
            atommeshbuilder.AddSphere(new Point3D(0, 0, 0), _AtomRadius, 40, 20);
            _AtomModel.Geometry = atommeshbuilder.ToMesh();
        }

        /// <summary>
        /// Update the color of the atom-material
        /// </summary>
        public void UpdateAtomColor()
        {
            if (_SelectedBrush != null)
            {
                _AtomModel.Material = MaterialHelper.CreateMaterial(_SelectedBrush);
            }
            else
            {
                _AtomModel.Material = MaterialHelper.CreateMaterial(_AtomColor);
            }
        }

        /// <summary>
        /// Change the opacity of the atom-material
        /// </summary>
        public void UpdateAtomOpacity()
        {
            Material t_mat = _AtomModel.Material.Clone();
            MaterialHelper.ChangeOpacity(t_mat, _AtomOpacity);
            t_mat.Freeze();
            _AtomModel.Material = t_mat;
        }

        /// <summary>
        /// Update the atom position (via new translate transform)
        /// </summary>
        public void UpdateAtomPosition()
        {
            this.Transform = new TranslateTransform3D(_AtomPosition);
        }

        /// <summary>
        /// Updates the label text according to label mode, atom id, element symbol, name and charge
        /// </summary>
        public void UpdateLabelText()
        {
            if (_Label != null)
            {
                switch (_LabelMode)
                {
                    case TAtomLabelMode.None:
                        _Label.Text = "";
                        break;
                    case TAtomLabelMode.ID:
                        _Label.Text = _AtomID.ToString();
                        break;
                    case TAtomLabelMode.Name:
                        _Label.Text = _AtomName.Trim();
                        break;
                    case TAtomLabelMode.Charge:
                        _Label.Text = string.Format("{0:0.#}", _AtomCharge);
                        break;
                    case TAtomLabelMode.Symbol:
                        _Label.Text = _AtomSymbol.Trim();
                        break;
                    case TAtomLabelMode.SymbolID:
                        _Label.Text = _AtomSymbol.Trim() + _AtomID.ToString();
                        break;
                    default:
                        _Label.Text = ""; 
                        break;
                }
            }
        }

        #endregion Methods

    }
}
