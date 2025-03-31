using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;
using System.Windows.Media;
using System.Windows;

namespace iCon_General
{
    /// <summary> 
    /// Adapted BillboardGeometryBuilder from Helix Toolkit for a non-scaling billboard 
    /// </summary>
    public class NonscaledBillboardGeometryBuilder
    {
        #region Internal Variables

        // Parent visual
        protected readonly Visual3D _Visual;

        // Transformation matrix (screen space to visual space)
        protected Matrix3D _ScreenToVisual;

        // Transformation matrix (visual space to screen space)
        protected Matrix3D _VisualToScreen;

        #endregion Internal Variables

        /// <summary>
        /// Create a nonscaled geometry builder for a specified visual
        /// </summary>
        /// <param name="visual">Parent visual</param>
        public NonscaledBillboardGeometryBuilder(Visual3D visual)
        {
            _Visual = visual;
        }

        #region Methods

        /// <summary>
        /// Generate triangle indices for a billboard mesh
        /// </summary>
        /// <param name="n">Number of billboards</param>
        /// <returns>Triangle indices</returns>
        public static Int32Collection CreateIndices(int n)
        {
            var indices = new Int32Collection(n * 6);
            for (int i = 0; i < n; i++)
            {
                // bottom right triangle
                indices.Add(i * 4 + 0);
                indices.Add(i * 4 + 1);
                indices.Add(i * 4 + 2);

                // top left triangle
                indices.Add(i * 4 + 2);
                indices.Add(i * 4 + 3);
                indices.Add(i * 4 + 0);
            }

            indices.Freeze();
            return indices;
        }

        /// <summary>
        /// Updates the transforms
        /// </summary>
        /// <returns>
        /// True if the transform was changed
        /// </returns>
        public bool UpdateTransforms()
        {
            var newTransform = CustomVisual3DHelper.GetViewportTransform(_Visual);

            if (double.IsNaN(newTransform.M11))
            {
                return false;
            }

            if (!newTransform.HasInverse)
            {
                return false;
            }

            if (newTransform == _VisualToScreen)
            {
                return false;
            }

            _VisualToScreen = _ScreenToVisual = newTransform;
            _ScreenToVisual.Invert();

            return true;
        }

        /// <summary>
        /// Calculates the billboard positions with the current screen transform
        /// </summary>
        /// <param name="bb">Billboard description</param>
        /// <returns>Points for Meshgeometry3D.Mesh.Positions</returns>
        public Point3DCollection GetPositions(NonscaledBillboard bb)
        {
            var positions = new Point3DCollection(4);

            var screenPoint = (Point4D)bb.Position * _VisualToScreen;

            double spx = screenPoint.X;
            double spy = screenPoint.Y;
            double spz = screenPoint.Z;
            double spw = screenPoint.W;

            var p0 = new Point4D(spx, spy, spz, spw) * _ScreenToVisual;
            double pwinverse = 1 / p0.W;

            // construct screen space x-direction unit vector
            var p1 = new Point4D(spx + 1.0e-3 * spw, spy, spz, spw) * _ScreenToVisual;
            Vector3D xdir = new Vector3D(p1.X * pwinverse, p1.Y * pwinverse, p1.Z * pwinverse) - bb.Position.ToVector3D();
            xdir /= xdir.Length;

            // construct screen space y-direction unit vector
            var p2 = new Point4D(spx, spy + 1.0e-3 * spw, spz, spw) * _ScreenToVisual;
            Vector3D ydir = new Vector3D(p2.X * pwinverse, p2.Y * pwinverse, p2.Z * pwinverse) - bb.Position.ToVector3D();
            ydir /= ydir.Length;

            // bottom left point
            positions.Add((bb.Position.ToVector3D() + bb.Left * xdir + bb.Bottom * ydir).ToPoint3D());

            // bottom right point
            positions.Add((bb.Position.ToVector3D() + bb.Right * xdir + bb.Bottom * ydir).ToPoint3D());

            // top right point
            positions.Add((bb.Position.ToVector3D() + bb.Right * xdir + bb.Top * ydir).ToPoint3D());

            // top left point
            positions.Add((bb.Position.ToVector3D() + bb.Left * xdir + bb.Top * ydir).ToPoint3D());

            positions.Freeze();
            return positions;
        }

        /// <summary>
        /// Creates a translation transform with an absolute depth offset in visual coordinates
        /// </summary>
        /// <param name="bb">Billboard description</param>
        /// <returns>Translation transform for depth offset</returns>
        public TranslateTransform3D GetDepthTransform(NonscaledBillboard bb)
        {
            if (bb.DepthOffset == 0.0)
            {
                return null;
            }

            var screenPoint = (Point4D)bb.Position * _VisualToScreen;

            double spx = screenPoint.X;
            double spy = screenPoint.Y;
            double spz = screenPoint.Z;
            double spw = screenPoint.W;

            spz -= 1.0e-3 * spw;

            var p0 = new Point4D(spx, spy, spz, spw) * _ScreenToVisual;

            var vec = new Vector3D(p0.X/p0.W, p0.Y/p0.W, p0.Z/p0.W) - bb.Position.ToVector3D();
            vec /= vec.Length;
            vec *= bb.DepthOffset;

            return new TranslateTransform3D(vec);
        }

        #endregion Methods
    }
}
