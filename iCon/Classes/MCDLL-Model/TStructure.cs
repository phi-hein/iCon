using System;
using System.Collections.Generic;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated structure data in one object
    /// </summary>
    public class TStructure
    {
        #region Fields

        /// <summary>
        /// Name of the compound
        /// </summary>
        public string Name;

        /// <summary>
        /// Basis vector a 
        /// </summary>
        public Vector3D BasisA;

        /// <summary>
        /// Basis vector b 
        /// </summary>
        public Vector3D BasisB;

        /// <summary>
        /// Basis vector c 
        /// </summary>
        public Vector3D BasisC;

        /// <summary>
        /// ElemID of atom i
        /// </summary>
        public List<int> ElemID;

        /// <summary>
        /// Relative coordinates of atom i
        /// </summary>
        public List<Vector3D> Coord;

        /// <summary>
        /// ElemID of the doped element for doping i
        /// </summary>
        public List<int> DopedID;

        /// <summary>
        /// ElemID of the dopand element for doping i
        /// </summary>
        public List<int> DopandID;

        /// <summary>
        /// Vacancy/Dopand-ratio, may be positive (dopand adds vacancies), zero (no influence on vacancies) and negative (dopand removes vacancies)
        /// </summary>
        public List<double> VacDopRatio;

        #endregion Fields

        #region Properties

        protected bool _IsValid;
        /// <summary>
        /// Flag, that shows, if the construction of this object and its sub-objects was successful
        /// </summary>
        public bool IsValid
        {
            get
            {
                return _IsValid;
            }
        }

        #endregion Properties

        /// <summary>
        /// Construct new model which holds the current validated structure
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        public TStructure(TMCJobWrapper MCDLL)
        {
            Initialize();

            GetData(MCDLL);
        }

        #region Methods

        /// <summary>
        /// Initialize all fields
        /// </summary>
        protected void Initialize()
        {
            Name = "";
            BasisA = new Vector3D();
            BasisB = new Vector3D();
            BasisC = new Vector3D();
            ElemID = new List<int>();
            Coord = new List<Vector3D>();
            DopedID = new List<int>();
            DopandID = new List<int>();
            VacDopRatio = new List<double>();
            _IsValid = false;
        }

        /// <summary>
        /// Retrieve data from Monte-Carlo-DLL
        /// </summary>
        protected void GetData(TMCJobWrapper MCDLL)
        {
            int ErrorCode = Constants.KMCERR_OK;
            _IsValid = false;

            // Check if MCDLL is valid object
            if (MCDLL == null)
            {
                ThrowError("MC-DLL object is null (TStructure.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TStructure.GetData)");
            }

            // Receive structure name
            ErrorCode = MCDLL.GetStructureName(ref Name);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read structure name from MC object (TStructure.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive lattice vectors
            double t_ax = 0, t_ay = 0, t_az = 0;
            double t_bx = 0, t_by = 0, t_bz = 0;
            double t_cx = 0, t_cy = 0, t_cz = 0;
            ErrorCode = MCDLL.GetBasisByLatticeVectors(ref t_ax, ref t_ay, ref t_az, ref t_bx, ref t_by, ref t_bz, ref t_cx, ref t_cy, ref t_cz);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read lattice vectors from MC object (TStructure.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }
            BasisA = new Vector3D(t_ax, t_ay, t_az);
            BasisB = new Vector3D(t_bx, t_by, t_bz);
            BasisC = new Vector3D(t_cx, t_cy, t_cz);

            // Receive number of atoms
            int t_atomcount = 0;
            ErrorCode = MCDLL.GetAtomCount(ref t_atomcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read atom count from MC object (TStructure.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive atom information
            ElemID.Clear();
            Coord.Clear();
            if (t_atomcount > 0)
            {
                int t_elemid = -1;
                for (int i = 0; i < t_atomcount; i++)
                {
                    ErrorCode = MCDLL.GetAtom(i, ref t_ax, ref t_ay, ref t_az, ref t_elemid);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read atom from MC object (TStructure.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    Coord.Add(new Vector3D(t_ax, t_ay, t_az));
                    ElemID.Add(t_elemid);
                }
            }

            // Receive number of dopings
            int t_dopcount = 0;
            ErrorCode = MCDLL.GetDopingCount(ref t_dopcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of dopings from MC object (TStructure.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive doping information
            DopedID.Clear();
            DopandID.Clear();
            VacDopRatio.Clear();
            if (t_dopcount > 0)
            {
                int t_dopedid = -1;
                int t_dopandid = -1;
                for (int i = 0; i < t_dopcount; i++)
                {
                    ErrorCode = MCDLL.GetDoping(i, ref t_dopedid, ref t_dopandid, ref t_ax);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read doping from MC object (TStructure.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    DopedID.Add(t_dopedid);
                    DopandID.Add(t_dopandid);
                    VacDopRatio.Add(t_ax);
                }
            }

            _IsValid = true;
        }

        /// <summary>
        /// Dispatch an exception for a synchronization error
        /// </summary>
        /// <param name="Text"></param>
        protected void ThrowError(string Text)
        {
            _IsValid = false;
            throw new ApplicationException(Text);
        }

        #endregion Methods
    }
}
