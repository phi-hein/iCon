using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using HelixToolkit.Wpf;

namespace iCon_General
{
    public class CustomMeshBuilder: MeshBuilder
    {
        public CustomMeshBuilder() 
            : base()
        {

        }

        #region Methods

        /// <summary>
        /// Adds a cylinder with arrow heads on both ends to the mesh
        /// </summary>
        public void AddDoubleArrow(Point3D point1, Point3D point2, double diameter, double headLength = 3, int thetaDiv = 18)
        {
            var dir = point2 - point1;
            double length = dir.Length;
            double r = diameter / 2;

            var pc = new PointCollection
                {
                    new Point(0, 0),
                    new Point(diameter * headLength, r * 2),
                    new Point(diameter * headLength, r),
                    new Point(length - (diameter * headLength), r),
                    new Point(length - (diameter * headLength), r * 2),
                    new Point(length, 0)
                };

            this.AddRevolvedGeometry(pc, null, point1, dir, thetaDiv);
        }

        #endregion Methods
    }
}
