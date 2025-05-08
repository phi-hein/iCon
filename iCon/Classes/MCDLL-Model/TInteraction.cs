using System;
using System.Collections.Generic;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated data of one interaction in one object
    /// </summary>
    public class TInteraction
    {
        #region Fields

        /// <summary>
        /// ElemID of the interacting environment atom
        /// </summary>
        public int ElemID;

        /// <summary>
        /// Distance between the jump start position and the interacting environment atom
        /// </summary>
        public double StartDist;

        /// <summary>
        /// Distance between the jump destination position and the interacting environment atom
        /// </summary>
        public double DestDist;

        /// <summary>
        /// Coordinates (cylindrical coordinate system) of the environment of the interaction between jump start position and the interacting environment atom
        /// </summary>
        public List<Vector3D> StartWWCoord;

        /// <summary>
        /// ElemIDs of the environment of the interaction between jump start position and the interacting environment atom
        /// </summary>
        public List<int> StartWWElemID;

        /// <summary>
        /// Coordinates (cylindrical coordinate system) of the environment of the interaction between jump destination position and the interacting environment atom
        /// </summary>
        public List<Vector3D> DestWWCoord;

        /// <summary>
        /// ElemIDs of the environment of the interaction between jump destination position and the interacting environment atom
        /// </summary>
        public List<int> DestWWElemID;

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
        /// Construct new model which holds the current validated jump
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        /// <param name="WWID">Index of the interaction</param>
        public TInteraction(TMCJobWrapper MCDLL, int WWID)
        {
            Initialize();

            GetData(MCDLL, WWID);
        }

        #region Methods

        /// <summary>
        /// Initialize all fields
        /// </summary>
        protected void Initialize()
        {
            ElemID = -1;
            StartDist = 0.0;
            DestDist = 0.0;
            StartWWCoord = new List<Vector3D>();
            StartWWElemID = new List<int>();
            DestWWCoord = new List<Vector3D>();
            DestWWElemID = new List<int>();

            _IsValid = false;
        }

        /// <summary>
        /// Retrieve data from Monte-Carlo-DLL
        /// </summary>
        protected void GetData(TMCJobWrapper MCDLL, int WWID)
        {
            int ErrorCode = Constants.KMCERR_OK;
            _IsValid = false;

            // Check if MCDLL is valid object
            if (MCDLL == null)
            {
                ThrowError("MC-DLL object is null (TInteraction.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TInteraction.GetData)");
            }

            // Receive basic interaction description
            ErrorCode = MCDLL.GetInteractionDesc(WWID, ref ElemID, ref StartDist, ref DestDist);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read interaction description from MC object (TInteraction.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of interaction start environment atoms
            int t_startcount = 0;
            ErrorCode = MCDLL.GetInteractionStartEnvCount(WWID, ref t_startcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read atom count for start interaction environment from MC object (TInteraction.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive interaction start environment
            StartWWCoord.Clear();
            StartWWElemID.Clear();
            if (t_startcount > 0)
            {
                int t_elem = 0;
                double t_ax = 0,t_ay = 0,t_az = 0;
                for (int i = 0; i < t_startcount; i++)
                {
                    ErrorCode = MCDLL.GetInteractionStartEnvAtom(WWID, i, ref t_elem, ref t_ax, ref t_ay, ref t_az);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read interaction start environment from MC object (TInteraction.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    StartWWCoord.Add(new Vector3D(t_ax, t_ay, t_az));
                    StartWWElemID.Add(t_elem);
                }
            }

            // Receive number of interaction destination environment atoms
            int t_destcount = 0;
            ErrorCode = MCDLL.GetInteractionDestEnvCount(WWID, ref t_destcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read atom count for destination interaction environment from MC object (TInteraction.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive interaction destination environment
            DestWWCoord.Clear();
            DestWWElemID.Clear();
            if (t_destcount > 0)
            {
                int t_elem = 0;
                double t_ax = 0, t_ay = 0, t_az = 0;
                for (int i = 0; i < t_destcount; i++)
                {
                    ErrorCode = MCDLL.GetInteractionDestEnvAtom(WWID, i, ref t_elem, ref t_ax, ref t_ay, ref t_az);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read interaction destination environment from MC object (TInteraction.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    DestWWCoord.Add(new Vector3D(t_ax, t_ay, t_az));
                    DestWWElemID.Add(t_elem);
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
