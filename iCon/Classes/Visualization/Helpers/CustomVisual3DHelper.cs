using HelixToolkit.Wpf;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary>
    /// This class is an adapted version of Visual3DHelper from HelixToolkit,
    /// which provides methods that respect the existence of uielement3ds
    /// in the visual tree (essentially changing ModelVisual3D to Visual3D)
    /// </summary>
    public static class CustomVisual3DHelper
    {
        /// <summary>
        /// Create total transformation matrix (visual space to screen space)
        /// </summary>
        public static Matrix3D GetViewportTransform(Visual3D visual)
        {
            // This method is adapted from HelixToolkit: Visual3DHelper.GetViewportTransform

            var totalTransform = Matrix3D.Identity;

            DependencyObject obj = visual;
            while (obj != null)
            {
                var viewport3DVisual = obj as Viewport3DVisual;
                if (viewport3DVisual != null)
                {
                    totalTransform.Append(Viewport3DHelper.GetCameraTransform(viewport3DVisual));       // inserted methods from Viewport3DHelper directly
                    totalTransform.Append(Viewport3DHelper.GetViewportTransform(viewport3DVisual));
                    return totalTransform;
                }

                var mv = obj as Visual3D;           // some objects might derive from uielement3d
                if (mv != null)
                {
                    if (mv.Transform != null)
                    {
                        totalTransform.Append(mv.Transform.Value);
                    }
                }

                obj = VisualTreeHelper.GetParent(obj);
            }

            return totalTransform;
        }

        /// <summary>
        /// Creates the world transformation matrix (visual space to world space)
        /// </summary>
        public static Matrix3D GetTransform(Visual3D visual)
        {
            // This method is adapted from HelixToolkit: Visual3DHelper.GetTransform

            var totalTransform = Matrix3D.Identity;

            DependencyObject obj = visual;
            while (obj != null)
            {
                var viewport3DVisual = obj as Viewport3DVisual;
                if (viewport3DVisual != null)
                {
                    return totalTransform;
                }

                var mv = obj as Visual3D;           // some objects might derive from uielement3d
                if (mv != null)
                {
                    if (mv.Transform != null)
                    {
                        totalTransform.Append(mv.Transform.Value);
                    }
                }

                obj = VisualTreeHelper.GetParent(obj);
            }

            return totalTransform;
        }

        /// <summary>
        /// Gets the distance from the camera for the specified visual
        /// </summary>
        public static double GetCameraDistance(Visual3D c, Point3D cameraPos, Transform3D transform)
        {
            var bounds = Visual3DHelper.FindBounds(c, transform);
            
            // SortingMethod.BoundingBoxCenter:
            var mid = new Point3D(bounds.X + bounds.SizeX * 0.5, bounds.Y + bounds.SizeY * 0.5, bounds.Z + bounds.SizeZ * 0.5);
            return (mid - cameraPos).LengthSquared;

            //case SortingMethod.BoundingBoxCorners:
            /*
            double d = double.MaxValue;
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X, bounds.Y, bounds.Z)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X + bounds.SizeX, bounds.Y, bounds.Z)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X + bounds.SizeX, bounds.Y + bounds.SizeY, bounds.Z)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X, bounds.Y + bounds.SizeY, bounds.Z)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X, bounds.Y, bounds.Z + bounds.SizeZ)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X + bounds.SizeX, bounds.Y, bounds.Z + bounds.SizeZ)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X + bounds.SizeX, bounds.Y + bounds.SizeY, bounds.Z + bounds.SizeZ)));
            d = Math.Min(d, cameraPos.DistanceTo(new Point3D(bounds.X, bounds.Y + bounds.SizeY, bounds.Z + bounds.SizeZ)));
            return d;
            */

            //default:
            /*
            var boundingSphere = BoundingSphere.CreateFromRect3D(bounds);
            return boundingSphere.DistanceFrom(cameraPos);
            */
        }
    }
}
