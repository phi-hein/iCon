using System;
using System.Collections.Generic;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated data of one lattice jump in one object
    /// </summary>
    public class TJump
    {
        #region Fields

        /// <summary>
        /// 4D position of the jump start atom (absolute, in origin unit cell -> (0,0,0,s))
        /// </summary>
        public Tuple4D StartPos;

        /// <summary>
        /// 4D position of the jump destination atom (relative, vector from start to destination atom)
        /// </summary>
        public Tuple4D DestPos;

        /// <summary>
        /// DirID (2nd index in two dimensional list of TJump in TJumps) of the corresponding backjump (1st index derives from StartPos.S + DestPos.S)
        /// </summary>
        public int BackjumpDirID;

        /// <summary>
        /// 4D positions of the jump environment atoms (relative, vector from start to environment atom)
        /// </summary>
        public List<Tuple4D> EnvPos;

        /// <summary>
        /// Index of the corresponding TUniqueJump in the list in TUniqueJumps
        /// </summary>
        public int UniqueJumpID;

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
        /// <param name="AtomID">Index of the moving atom in the unit cell structure</param>
        /// <param name="DirID">Index of the jump direction of the moving atom</param>
        public TJump(TMCJobWrapper MCDLL, int AtomID, int DirID)
        {
            Initialize();

            GetData(MCDLL,AtomID,DirID);
        }

        #region Methods

        /// <summary>
        /// Initialize all fields
        /// </summary>
        protected void Initialize()
        {
            StartPos = new Tuple4D();
            DestPos = new Tuple4D();
            BackjumpDirID = -1;
            EnvPos = new List<Tuple4D>();
            UniqueJumpID = -1;
            _IsValid = false;
        }

        /// <summary>
        /// Retrieve data from Monte-Carlo-DLL
        /// </summary>
        protected void GetData(TMCJobWrapper MCDLL, int AtomID, int DirID)
        {
            int ErrorCode = Constants.KMCERR_OK;
            _IsValid = false;

            // Check if MCDLL is valid object
            if (MCDLL == null)
            {
                ThrowError("MC-DLL object is null (TJump.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TJump.GetData)");
            }

            // Receive jump start position
            ErrorCode = MCDLL.GetJumpStartPos(AtomID, DirID, ref StartPos.X, ref StartPos.Y, ref StartPos.Z, ref StartPos.S);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read jump start position from MC object (TJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive jump destination position
            ErrorCode = MCDLL.GetJumpDestPos(AtomID, DirID, ref DestPos.X, ref DestPos.Y, ref DestPos.Z, ref DestPos.S);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read jump destination position from MC object (TJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive backjump direction
            ErrorCode = MCDLL.GetJumpBackjumpDirID(AtomID, DirID, ref BackjumpDirID);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read backjump direction from MC object (TJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of environment atoms
            int t_envcount = 0;
            ErrorCode = MCDLL.GetJumpEnvCount(AtomID, DirID, ref t_envcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of environment atoms from MC object (TJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive environment information
            EnvPos.Clear();
            if (t_envcount > 0)
            {
                Tuple4D tuple = null;
                for (int i = 0; i < t_envcount; i++)
                {
                    tuple = new Tuple4D();
                    ErrorCode = MCDLL.GetJumpEnvAtom(AtomID, DirID, i, ref tuple.X, ref tuple.Y, ref tuple.Z, ref tuple.S);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read environment atom from MC object (TJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    EnvPos.Add(tuple);
                }
            }

            // Receive index of corresponding unique jump
            ErrorCode = MCDLL.GetJumpUniqueJumpID(AtomID, DirID, ref UniqueJumpID);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read unique jump index from MC object (TJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
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
