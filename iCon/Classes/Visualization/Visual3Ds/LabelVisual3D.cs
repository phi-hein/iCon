using HelixToolkit.Wpf;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary>
    /// Label which always faces the screen and which moves with an absolute depth offset on a sphere around its position
    /// </summary>
    public class LabelVisual3D: UIElement3D
    {
        #region Internal Variables

        protected NonscaledBillboardGeometryBuilder _MeshBuilder;

        protected MeshGeometry3D _MeshGeometry;

        protected GeometryModel3D _Model;

        protected NonscaledBillboard _Billboard;

        protected Transform3DGroup _Transform;

        protected TranslateTransform3D _PositionTransform;

        protected TranslateTransform3D _DepthTransform;

        #endregion Internal Variables

        #region Properties

        protected string _Text;
        /// <summary>
        /// Text, which is displayed on the label
        /// </summary>
        public string Text
        {
            get
            {
                return _Text;
            }
            set
            {
                if (!_Text.Equals(value))
                {
                    _Text = value;
                    if (_IsInContentEditingMode == false)
                    {
                        UpdateMaterial();
                    }
                }
            }
        }

        protected int _FontSize;
        /// <summary>
        /// Font size of the label text
        /// </summary>
        public int FontSize
        {
            get
            {
                return _FontSize;
            }
            set
            {
                if (value != _FontSize)
                {
                    _FontSize = value;
                    if (_IsInContentEditingMode == false)
                    {
                        UpdateMaterial();
                    }
                }
            }
        }

        protected Color _Foreground;
        /// <summary>
        /// Text color of this label
        /// </summary>
        public Color Foreground
        {
            get
            {
                return _Foreground;
            }
            set
            {
                if (value != _Foreground)
                {
                    _Foreground = value;
                    if (_IsInContentEditingMode == false)
                    {
                        UpdateMaterial();
                    }
                }
            }
        }

        protected double _DepthOffset;
        /// <summary>
        /// Depth offset of the label
        /// </summary>
        public double DepthOffset
        {
            get
            {
                return _DepthOffset;
            }
            set
            {
                if (value != _DepthOffset)
                {
                    _DepthOffset = value;
                    _Billboard.DepthOffset = _DepthOffset;
                }
            }
        }

        protected Vector3D _Position;
        /// <summary>
        /// Position of this label (without depth offset)
        /// </summary>
        public Vector3D Position
        {
            get
            {
                return _Position;
            }
            set
            {
                if (value != _Position)
                {
                    _Position = value;
                    _Transform.Children.Remove(_PositionTransform);
                    _PositionTransform = new TranslateTransform3D(_Position);
                    _Transform.Children.Add(_PositionTransform);
                }
            }
        }

        protected bool _IsInContentEditingMode;
        /// <summary>
        /// True = Label is in content editing mode = changes of text, fontsize and foreground lead to no immidiate material update
        /// False = Material is updated
        /// </summary>
        public bool IsInContentEditingMode
        {
            get
            {
                return _IsInContentEditingMode;
            }
            set
            {
                if (value != _IsInContentEditingMode)
                {
                    _IsInContentEditingMode = value;
                    // Update material if editing mode is disabled
                    if (_IsInContentEditingMode == false)
                    {
                        UpdateMaterial();
                    }
                }
            }
        }

        protected string _InternalType;
        /// <summary>
        /// Internal type string for label destinction (e.g. atom labels, axis labels ...)
        /// </summary>
        public string InternalType
        {
            get
            {
                return _InternalType;
            }
            set
            {
                if (_InternalType != value)
                {
                    _InternalType = value;
                }
            }
        }

        #endregion Properties

        public LabelVisual3D()
        {
            // Initialize mesh builder
            _MeshBuilder = new NonscaledBillboardGeometryBuilder(this);

            // Initialize geometry
            _MeshGeometry = new MeshGeometry3D();
            _MeshGeometry.TriangleIndices = NonscaledBillboardGeometryBuilder.CreateIndices(1);
            _MeshGeometry.TextureCoordinates = new PointCollection
                                        {
                                            new Point(0, 1),
                                            new Point(1, 1),
                                            new Point(1, 0),
                                            new Point(0, 0)
                                        };

            // Initialize model
            _Model = new GeometryModel3D();
            _Model.Geometry = _MeshGeometry;
            this.Visual3DModel = _Model;       

            // Initialize billboard
            _Billboard = new NonscaledBillboard(new Point3D(0, 0, 0), 1.0, 1.0, HorizontalAlignment.Center, VerticalAlignment.Center, 0.0);

            // Initialize transformations
            _Transform = new Transform3DGroup();
            _PositionTransform = new TranslateTransform3D(0, 0, 0);
            _DepthTransform = null;
            _Transform.Children.Add(_PositionTransform);
            Transform = _Transform;

            // Initialize properties
            _IsInContentEditingMode = false;
            _InternalType = "";
            _Text = "";
            _FontSize = 10;
            _Foreground = Colors.White;
            _DepthOffset = 0.0;
            _Position = new Vector3D(0, 0, 0);
            IsHitTestVisible = false;
        }

        #region Methods

        /// <summary>
        /// Updates label appearance
        /// </summary>
        public void UpdateMaterial()
        {
            if (string.IsNullOrEmpty(_Text))
            {
                _Model.Material = null;
                return;
            }

            // create text block
            var txtblkLabel = new TextBlock(new Run(_Text));
            txtblkLabel.Foreground = new SolidColorBrush(_Foreground);
            txtblkLabel.Background = null;
            txtblkLabel.FontSize = _FontSize;
            txtblkLabel.Measure(new Size(1000, 1000));
            txtblkLabel.Arrange(new Rect(txtblkLabel.DesiredSize));

            // render image brush
            var rtbLabel = new RenderTargetBitmap(((int)txtblkLabel.ActualWidth + 1) * 50, ((int)txtblkLabel.ActualHeight + 1) * 50, 96 * 50, 96 * 50, PixelFormats.Pbgra32);
            rtbLabel.Render(txtblkLabel);

            // create emitting (always enlighted) material out of image brush
            MaterialGroup matGrp = new MaterialGroup();
            matGrp.Children.Add(new DiffuseMaterial(new ImageBrush(rtbLabel)));
            matGrp.Children.Add(new EmissiveMaterial(new ImageBrush(rtbLabel)));
            _Model.Material = matGrp;

            // set new billboard dimensions
            _Billboard.Width = txtblkLabel.ActualWidth / 10;
            _Billboard.Height = txtblkLabel.ActualHeight / 10;
            
            // get new positions for changed dimensions
            if (Visual3DHelper.IsAttachedToViewport3D(this) == true)
            {
                _MeshGeometry.Positions = _MeshBuilder.GetPositions(_Billboard);
            }
        }

        /// <summary>
        /// Forces geometry update even if there is no transform update
        /// </summary>
        public void ForceGeometryUpdate()
        {
            if (Visual3DHelper.IsAttachedToViewport3D(this) == true)
            {
                if (_DepthTransform != null)
                {
                    _Transform.Children.Remove(_DepthTransform);
                }
                _MeshBuilder.UpdateTransforms();
                _MeshGeometry.Positions = _MeshBuilder.GetPositions(_Billboard);
                _DepthTransform = _MeshBuilder.GetDepthTransform(_Billboard);
                if (_DepthTransform != null)
                {
                    _Transform.Children.Add(_DepthTransform);
                }
            }
        }

        /// <summary>
        /// Updates position of the label
        /// </summary>
        public void UpdateGeometry()
        {
            if (Visual3DHelper.IsAttachedToViewport3D(this) == true)
            {
                if (_DepthTransform != null)
                {
                    _Transform.Children.Remove(_DepthTransform);
                }
                if (_MeshBuilder.UpdateTransforms())
                {
                    _MeshGeometry.Positions = _MeshBuilder.GetPositions(_Billboard);
                    _DepthTransform = _MeshBuilder.GetDepthTransform(_Billboard);
                }
                if (_DepthTransform != null)
                {
                    _Transform.Children.Add(_DepthTransform);
                }
            }
        }

        #endregion Methods
    }
}
