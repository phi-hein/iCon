using System;
using System.Collections.Generic;
using System.Windows.Media;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated elements data in one object
    /// </summary>
    public class TElements
    {
        #region Fields

        /// <summary>
        /// Symbol of the moving species
        /// </summary>
        public string MovSymbol;

        /// <summary>
        /// Name of the moving species
        /// </summary>
        public string MovName;

        /// <summary>
        /// Charge of the moving species in e
        /// </summary>
        public double MovCharge;

        /// <summary>
        /// Radius of the moving species in angstrom
        /// </summary>
        public double MovRadius;

        /// <summary>
        /// Color of the moving species
        /// </summary>
        public Color MovColor;		
        
        /// <summary>
        /// Symbol of the vacancies
        /// </summary>
        public string VacSymbol;

        /// <summary>
        /// Name of the vacancies
        /// </summary>
        public string VacName;

        /// <summary>
        /// Charge of the vacancies
        /// </summary>
        public double VacCharge;
        
        /// <summary>
        /// Radius of the vacancies
        /// </summary>
        public double VacRadius;

        /// <summary>
        /// Color of the vacancies
        /// </summary>
        public Color VacColor;

        /// <summary>
        /// Symbol of the elements
        /// </summary>
        public List<string> ElemSymbol;

        /// <summary>
        /// Name of the elements
        /// </summary>
        public List<string> ElemName;

        /// <summary>
        /// Charge of the elements in e
        /// </summary>
        public List<double> ElemCharge;

        /// <summary>
        /// Radius of the elements in angstrom
        /// </summary>
        public List<double> ElemRadius;

        /// <summary>
        /// Color of the elements
        /// </summary>
        public List<Color> ElemColor;

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
        /// Construct new model which holds the current validated elements
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        public TElements(TMCJobWrapper MCDLL)
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
            MovSymbol = "";
            MovName = "";
            MovCharge = 0;
            MovRadius = 0;
            MovColor = new Color();
            VacSymbol = "";
            VacName = "";
            VacCharge = 0;
            VacRadius = 0;
            VacColor = new Color();
            ElemSymbol = new List<string>();
            ElemName = new List<string>();
            ElemCharge = new List<double>();
            ElemRadius = new List<double>();
            ElemColor = new List<Color>();
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
                ThrowError("MC-DLL object is null (TElements.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TElements.GetData)");
            }

            // Receive moving species
            double t_ax = 0, t_ay = 0, t_az = 0;
            ErrorCode = MCDLL.GetMovingElement(ref MovSymbol, ref MovName, ref MovCharge, ref MovRadius, ref t_ax, ref t_ay, ref t_az);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read moving species from MC object (TElements.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }
            MovColor.ScR = (float)t_ax;
            MovColor.ScG = (float)t_ay;
            MovColor.ScB = (float)t_az;
            MovColor.ScA = 1.0F;

            // Receive vacancy information
            ErrorCode = MCDLL.GetVacancyElement(ref VacSymbol, ref VacName, ref VacCharge, ref VacRadius, ref t_ax, ref t_ay, ref t_az);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read vacancy species from MC object (TElements.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }
            VacColor.ScR = (float)t_ax;
            VacColor.ScG = (float)t_ay;
            VacColor.ScB = (float)t_az;
            VacColor.ScA = 1.0F;

            // Receive number of elements
            int t_elemcount = 0;
            ErrorCode = MCDLL.GetElementCount(ref t_elemcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read element count from MC object (TElements.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive element information
            ElemSymbol.Clear();
            ElemName.Clear();
            ElemCharge.Clear();
            ElemRadius.Clear();
            ElemColor.Clear();
            if (t_elemcount > 0)
            {
                string t_sym = "";
                string t_name = "";
                double t_charge = 0;
                double t_radius = 0;
                Color t_color = new Color();
                for (int i = 0; i < t_elemcount; i++)
                {
                    ErrorCode = MCDLL.GetElement(i, ref t_sym, ref t_name, ref t_charge, ref t_radius, ref t_ax, ref t_ay, ref t_az);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read element from MC object (TElements.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    ElemSymbol.Add(t_sym);
                    ElemName.Add(t_name);
                    ElemCharge.Add(t_charge);
                    ElemRadius.Add(t_radius);
                    t_color = new Color();
                    t_color.ScR = (float)t_ax;
                    t_color.ScG = (float)t_ay;
                    t_color.ScB = (float)t_az;
                    t_color.ScA = 1.0F;
                    ElemColor.Add(t_color);
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
