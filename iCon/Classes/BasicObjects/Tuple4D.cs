using System;

namespace iCon_General
{
    /// <summary>
    /// A class for describing discrete 4D positions
    /// (I know there exists a Tuple<> class but this would require more code in the using classes)
    /// </summary>
    public class Tuple4D
    {
        #region Fields

        /// <summary>
        /// First 4D coordinate
        /// </summary>
        public int X;

        /// <summary>
        /// Second 4D coordinate
        /// </summary>
        public int Y;

        /// <summary>
        /// Third 4D coordinate
        /// </summary>
        public int Z;

        /// <summary>
        /// Fourth 4D coordinate
        /// </summary>
        public int S;

        #endregion Fields

        public Tuple4D()
        {
            X = 0;
            Y = 0;
            Z = 0;
            S = -1;
        }

        public Tuple4D(int x, int y, int z, int s)
        {
            X = x;
            Y = y;
            Z = z;
            S = s;
        }

        #region Operators

        public static Tuple4D operator +(Tuple4D a, Tuple4D b)
        {
            if ((a == null) || (b == null)) return null;
            return new Tuple4D(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.S + b.S);
        }

        public static Tuple4D operator -(Tuple4D a, Tuple4D b)
        {
            if ((a == null) || (b == null)) return null;
            return new Tuple4D(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.S - b.S);
        }

        #endregion Operators

        #region Methods

        public void Set(int x, int y, int z, int s)
        {
            X = x;
            Y = y;
            Z = z;
            S = s;
        }

        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }

            Tuple4D tuple = obj as Tuple4D;
            if ((Object)tuple == null)
            {
                return false;
            }

            return ((X == tuple.X) && (Y == tuple.Y) && (Z == tuple.Z) && (S == tuple.S));
        }

        public bool Equals(Tuple4D tuple)
        {
            if ((Object)tuple == null)
            {
                return false;
            }

            return ((X == tuple.X) && (Y == tuple.Y) && (Z == tuple.Z) && (S == tuple.S));
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #endregion Methods
    }
}
