using System;
using System.Collections.Generic;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated unique jumps data in one object
    /// </summary>
    public class TUniqueJumps
    {
        #region Fields
        
        /// <summary>
        /// Number of interaction environment shells
        /// </summary>
        public int InteractionShellCount;

        #endregion Fields

        #region Objects

        /// <summary>
        /// Unique vacancy jumps
        /// </summary>
        public List<TUniqueJump> UJumps;

        /// <summary>
        /// Unique interactions between moving vacancy and additive environment atoms
        /// </summary>
        public List<TInteraction> Interactions;

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
                if (UJumps != null)
                {
                    if (UJumps.Count != 0)
                    {
                        for (int i = 0; i < UJumps.Count; i++)
                        {
                            if (UJumps[i] != null)
                            {
                                if (UJumps[i].IsValid == false) return false;
                            }
                            else return false;
                        }
                    }
                }
                else return false;
                if (Interactions != null)
                {
                    if (Interactions.Count != 0)
                    {
                        for (int i = 0; i < Interactions.Count; i++)
                        {
                            if (Interactions[i] != null)
                            {
                                if (Interactions[i].IsValid == false) return false;
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
        /// Construct new model which holds the current validated unique jumps
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        public TUniqueJumps(TMCJobWrapper MCDLL)
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
            InteractionShellCount = 0;

            UJumps = new List<TUniqueJump>();
            Interactions = new List<TInteraction>();

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
                ThrowError("MC-DLL object is null (TUniqueJumps.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TUniqueJumps.GetData)");
            }

            // Receive shell counts
            ErrorCode = MCDLL.GetInteractionShellCount(ref InteractionShellCount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read interaction shell count from MC object (TUniqueJumps.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of unique jumps
            int t_UJumpsCount = 0;
            ErrorCode = MCDLL.GetUJumpCount(ref t_UJumpsCount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of unique jumps from MC object (TUniqueJumps.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }


            // Receive unique jumps
            UJumps.Clear();
            if (t_UJumpsCount > 0)
            {
                for (int i = 0; i < t_UJumpsCount; i++)
                {
                    UJumps.Add(new TUniqueJump(MCDLL, i));
                }
            }

            // Receive number of interactions
            int t_InteractionsCount = 0;
            ErrorCode = MCDLL.GetInteractionCount(ref t_InteractionsCount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of interactions from MC object (TUniqueJumps.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }


            // Receive interactions
            Interactions.Clear();
            if (t_InteractionsCount > 0)
            {
                for (int i = 0; i < t_InteractionsCount; i++)
                {
                    Interactions.Add(new TInteraction(MCDLL, i));
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
