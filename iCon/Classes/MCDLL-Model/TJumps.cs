using System;
using System.Collections.Generic;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated lattice jumps data in one object
    /// </summary>
    public class TJumps
    {
        #region Fields

        /// <summary>
        /// Number of jump destination shells
        /// </summary>
        public int MovShellCount;
		
        /// <summary>
        /// Number of jump environment shells around jump start positions
        /// </summary>
        public int JumpStartShellCount;
		
	    /// <summary>
        /// Number of jump environment shells around jump center (approximate transition state)
	    /// </summary>
        public int JumpTSShellCount;		     

        /// <summary>
        /// Number of jump environment shells around jump destination positions
        /// </summary>
        public int JumpDestShellCount;

        #endregion Fields

        #region Objects

        /// <summary>
        /// Vacancy jumps from the (0,0,0)-unit cell, [Atom][Direction]
        /// </summary>
        public List<List<TJump>> Jumps;

        #endregion Objects

        #region Properties

        protected bool _IsValid;
        /// <summary>
        /// Flag, that shows, if the construction of this object and its sub-objects was successful
        /// </summary>
        public bool IsValid
        {
            get
            {
                if (Jumps != null)
                {
                    if (Jumps.Count != 0)
                    {
                        for (int i = 0; i < Jumps.Count; i++)
                        {
                            if (Jumps[i] != null)
                            {
                                if (Jumps[i].Count != 0)
                                {
                                    for (int j = 0; j < Jumps[i].Count; j++)
                                    {
                                        if (Jumps[i][j] != null)
                                        {
                                            if (Jumps[i][j].IsValid == false) return false;
                                        }
                                        else return false;
                                    }
                                }
                            }
                            else return false;
                        }
                    }
                }
                else return false;
                return _IsValid;
            }
        }

        #endregion Properties

        /// <summary>
        /// Construct new model which holds the current validated lattice jumps
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        public TJumps(TMCJobWrapper MCDLL)
        {
            Initialize();

            GetData(MCDLL);
        }

        #region Methods

        /// <summary>
        /// Initialize all fields and object handles
        /// </summary>
        protected void Initialize()
        {
            MovShellCount = 0;
            JumpStartShellCount = 0;
            JumpTSShellCount = 0;
            JumpDestShellCount = 0;

            Jumps = new List<List<TJump>>();

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
                ThrowError("MC-DLL object is null (TJumps.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TJumps.GetData)");
            }

            // Receive shell counts
            ErrorCode = MCDLL.GetShellCounts(ref MovShellCount, ref JumpStartShellCount, ref JumpTSShellCount, ref JumpDestShellCount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read shell counts from MC object (TJumps.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of moving atoms in unit cell
            int t_AtomCount = 0;
            ErrorCode = MCDLL.GetMovCount(ref t_AtomCount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read atom number from MC object (TJumps.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }


            // Receive jumps
            Jumps.Clear();
            if (t_AtomCount > 0)
            {
                for (int i = 0; i < t_AtomCount; i++)
                {
                    // Create list of sub-objects for the moving atom i
                    Jumps.Add(new List<TJump>());

                    // Receive number of jump directions
                    int t_DirCount = 0;
                    ErrorCode = MCDLL.GetJumpDirCount(i, ref t_DirCount);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read jump directions from MC object (TJumps.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }

                    // Create jump sub-objects
                    if (t_DirCount > 0)
                    {
                        for (int j = 0; j < t_DirCount; j++)
                        {
                            Jumps[i].Add(new TJump(MCDLL,i,j));
                        }
                    }
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
