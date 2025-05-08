using System;
using System.Collections.Generic;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated settings data in one object
    /// </summary>
    public class TSettings
    {
        #region Fields

        /// <summary>
        /// Simulation temperature [K]
        /// </summary>
        public double Temperature;

        /// <summary>
        /// Jump attempt frequency [1/s] 
        /// </summary>
        public double AttemptFrequency;

        /// <summary>
        /// Flag: true = Write a checkpoint file during simulation
        /// </summary>
        public bool WriteCheckpoint;

        /// <summary>
        /// Flag: true = Try to load a checkpoint file before the simulation
        /// </summary>
        public bool LoadCheckpoint;

        /// <summary>
        /// Maximum E-field contribution to jump activation energies [kT] 
        /// </summary>
        public double EFieldMagnitude;

        /// <summary>
        /// E-field direction vector (from plus-pole to minus-pole = force vector direction upon positive test charge)
        /// </summary>
        public Vector3D EFieldDirection;

        /// <summary>
        /// E-field vector [V/cm]
        /// </summary>
        public Vector3D EField;

        /// <summary>
        /// Size of the cubic lattice (= number of unit cells along one edge of the lattice cube)
        /// </summary>
        public int LatticeSize;

        /// <summary>
        /// Number of additional vacancies in the lattice (might be also zero or negative)
        /// </summary>
        public long AdditionalVacAnz;

        /// <summary>
        /// Total concentration of vacancies (= Vacancies/(Vacancies + MovingAtoms))
        /// </summary>
        public double TotalVacConc;

        /// <summary>
        /// Total number of vacancies
        /// </summary>
        public long TotalVacAnz;

        /// <summary>
        /// Volume concentration of the moving atoms [1/cm^3]
        /// </summary>
        public double MovVolConc;

        /// <summary>
        /// Dopand concentration of doping i
        /// </summary>
        public List<double> DopandConc;

        /// <summary>
        /// Number of dopand atoms of doping i
        /// </summary>
        public List<long> DopandAnz;

        /// <summary>
        /// Flag: true = execute prerun (simulation steps without movement detection for equilibration of the lattice)
        /// </summary>
        public bool DoPrerun;

        /// <summary>
        /// Number of monte-carlo steps per particle (per moving atom), that are accepted during prerun
        /// </summary>
        public long PreMCSP;

        /// <summary>
        /// Number of recorded, highest jump probabilities during prerun
        /// </summary>
        public int PreRecordAnz;

        /// <summary>
        /// Flag: true = execute dynamic normalization (simulation steps with probability analysis for occurence-based normalization)
        /// </summary>
        public bool DoDynNorm;

        /// <summary>
        /// Number of jump attempts, that are executed during dynamic normalization
        /// </summary>
        public long DynAttemptAnz;

        /// <summary>
        /// Number of recorded, highest jump probabilities during dynamic normalization
        /// </summary>
        public int DynRecordAnz;

        /// <summary>
        /// Index of the entry in the recorded jump probability list, that is used for dynamic normalization
        /// </summary>
        public int DynNormNum;

        /// <summary>
        /// Number of monte-carlo steps per particle (per moving atom), that are accepted during main KMC simulation
        /// </summary>
        public long MCSP;

        /// <summary>
        /// Number of recorded, highest jump probabilities during main KMC simulation
        /// </summary>
        public int MainRecordAnz;

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
        /// Construct new model which holds the current validated settings
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        public TSettings(TMCJobWrapper MCDLL)
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
            Temperature = 0.0;
            AttemptFrequency = 0.0;
            WriteCheckpoint = true;
            LoadCheckpoint = true;
            EFieldMagnitude = 0.0;
            EFieldDirection = new Vector3D();
            EField = new Vector3D();
            LatticeSize = 0;
            AdditionalVacAnz = 0;
            TotalVacConc = 0.0;
            TotalVacAnz = 0;
            MovVolConc = 0.0;
            DopandConc = new List<double>();
            DopandAnz = new List<long>();
            DoPrerun = false;
            PreMCSP = 0;
            PreRecordAnz = 0;
            DoDynNorm = false;
            DynAttemptAnz = 0;
            DynRecordAnz = 0;
            DynNormNum = 0;
            MCSP = 0;
            MainRecordAnz = 0;
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
                ThrowError("MC-DLL object is null (TSettings.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TSettings.GetData)");
            }

            // Receive temperature
            ErrorCode = MCDLL.GetTemperature(ref Temperature);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read temperature from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive attempt frequency
            ErrorCode = MCDLL.GetAttemptFrequency(ref AttemptFrequency);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read jump attempt frequency from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive checkpoint flags
            ErrorCode = MCDLL.GetCheckpointFlags(ref WriteCheckpoint, ref LoadCheckpoint);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read checkpoint flags from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive E-field settings
            double t_x = 0, t_y = 0, t_z = 0;
            ErrorCode = MCDLL.GetEFieldSettings(ref t_x, ref t_y, ref t_z, ref EFieldMagnitude);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read E-field settings from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }
            EFieldDirection = new Vector3D(t_x, t_y, t_z);

            // Receive E-field 
            ErrorCode = MCDLL.GetEField(ref t_x, ref t_y, ref t_z);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read E-field from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }
            EField = new Vector3D(t_x, t_y, t_z);

            // Receive lattice size
            ErrorCode = MCDLL.GetLatticeSize(ref LatticeSize);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read lattice size from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of additional vacancies
            ErrorCode = MCDLL.GetAdditionalVacAnz(ref AdditionalVacAnz);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of additional vacancies from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive total vacancy concentration
            ErrorCode = MCDLL.GetTotalVacancyConc(ref TotalVacConc);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read vacancy concentration from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive total vacancy number
            ErrorCode = MCDLL.GetTotalVacancyAnz(ref TotalVacAnz);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of the vacancies from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive volume concentration of the moving species
            ErrorCode = MCDLL.GetMovVolConc(ref MovVolConc);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read volume concentration from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of dopings
            int t_dopcount = 0;
            ErrorCode = MCDLL.GetDopingCount(ref t_dopcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of dopings from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive doping concentration and dopand number
            DopandConc.Clear();
            DopandAnz.Clear();
            if (t_dopcount > 0)
            {
                double t_dopandconc = 0.0;
                long t_dopandanz = 0;
                for (int i = 0; i < t_dopcount; i++)
                {
                    ErrorCode = MCDLL.GetDopandConc(i, ref t_dopandconc);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read dopand concentration from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    ErrorCode = MCDLL.GetDopandAnz(i, ref t_dopandanz);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read dopand number from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    DopandConc.Add(t_dopandconc);
                    DopandAnz.Add(t_dopandanz);
                }
            }

            // Receive prerun settings
            ErrorCode = MCDLL.GetPrerunOptions(ref DoPrerun, ref PreMCSP, ref PreRecordAnz);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read prerun settings from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive dynamic normalization settings
            ErrorCode = MCDLL.GetDynNormParameters(ref DoDynNorm, ref DynAttemptAnz, ref DynRecordAnz, ref DynNormNum);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read dynamic normalization settings from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive main KMC simulation settings
            ErrorCode = MCDLL.GetMainKMCOptions(ref MCSP, ref MainRecordAnz);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read main KMC simulation settings from MC object (TSettings.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
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