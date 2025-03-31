using System.Windows;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary> 
    /// Specification of a billboard geometry 
    /// </summary>
    public class NonscaledBillboard
    {
        #region Fields

        /// <summary>
        /// The position
        /// </summary>
        public Point3D Position;

        /// <summary>
        /// The relative left position (screen coordinates).
        /// </summary>
        public double Left;

        /// <summary>
        /// The relative right position (screen coordinates).
        /// </summary>
        public double Right;

        /// <summary>
        /// The relative top position (screen coordinates).
        /// </summary>
        public double Top;

        /// <summary>
        /// The relative bottom position (screen coordinates).
        /// </summary>
        public double Bottom;

        /// <summary>
        /// The absolute depth offset
        /// </summary>
        public double DepthOffset;

        #endregion Fields

        #region Properties

        protected double _Width;
        /// <summary>
        /// The width
        /// </summary>
        public double Width
        {
            get
            {
                return _Width;
            }
            set
            {
                if (value != _Width)
                {
                    _Width = value;
                    UpdateRelativePositions();
                }
            }
        }

        protected double _Height;
        /// <summary>
        /// The height
        /// </summary>
        public double Height
        {
            get
            {
                return _Height;
            }
            set
            {
                if (value != _Height)
                {
                    _Height = value;
                    UpdateRelativePositions();
                }
            }
        }

        protected HorizontalAlignment _HoriAlign;
        /// <summary>
        /// The horizontal alignment
        /// </summary>
        public HorizontalAlignment HoriAlign
        {
            get
            {
                return _HoriAlign;
            }
            set
            {
                if (!_HoriAlign.Equals(value))
                {
                    _HoriAlign = value;
                    UpdateRelativePositions();
                }
            }
        }

        protected VerticalAlignment _VertAlign;
        /// <summary>
        /// The vertical alignment
        /// </summary>
        public VerticalAlignment VertAlign
        {
            get
            {
                return _VertAlign;
            }
            set
            {
                if (!_VertAlign.Equals(value))
                {
                    _VertAlign = value;
                    UpdateRelativePositions();
                }
            }
        }

        #endregion Properties

        /// <summary>
        /// Initializes a new instance of the <see cref="NonscaledBillboard" /> class.
        /// </summary>
        /// <param name="position">The position.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <param name="horizontalAlignment">The horizontal alignment.</param>
        /// <param name="verticalAlignment">The vertical alignment.</param>
        /// <param name="depthOffset">The absolute depth offset.</param>
        public NonscaledBillboard(Point3D position, double width = 1.0, double height = 1.0, HorizontalAlignment horizontalAlignment = HorizontalAlignment.Center, VerticalAlignment verticalAlignment = VerticalAlignment.Center, double depthOffset = 0.0)
        {
            // Initialize values
            Left = 0.0;
            Right = 0.0;
            Top = 0.0;
            Bottom = 0.0;
            Position = position;
            _Width = width;
            _Height = height;
            _HoriAlign = horizontalAlignment;
            _VertAlign = verticalAlignment;
            DepthOffset = depthOffset;

            // Calculate Left, Right, Bottom and Top
            UpdateRelativePositions();
        }

        #region Methods

        /// <summary>
        /// Update Left, Right, Top and Bottoom values
        /// </summary>
        protected void UpdateRelativePositions()
        {
            // Set horizontal alignment factor
            double xa = -0.5;
            if (HoriAlign == HorizontalAlignment.Left) xa = 0;
            if (HoriAlign == HorizontalAlignment.Right) xa = -1;

            // Set vertical alignment factor
            double ya = -0.5;
            if (VertAlign == VerticalAlignment.Top) ya = 0;
            if (VertAlign == VerticalAlignment.Bottom) ya = -1;

            double left = xa * _Width;
            double top = ya * _Height;

            this.Left = left;
            this.Right = left + _Width;
            this.Top = top;
            this.Bottom = top + _Height;
        }

        #endregion Methods
    }
}
