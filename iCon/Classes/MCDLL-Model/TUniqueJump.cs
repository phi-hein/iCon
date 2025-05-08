using System;
using System.Collections.Generic;
using System.Windows.Media.Media3D;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated data of one unique jump in one object
    /// </summary>
    public class TUniqueJump
    {
        #region Fields

        /// <summary>
        /// Length of the jump in angstrom
        /// </summary>
        public double Length;

        /// <summary>
        /// Coordinates of the jump environment atoms in the cylindrical jump coordinate system (sorted, but without respect to active and additive) [angstrom,angstrom,degree]
        /// </summary>
        public List<Vector3D> Coord;

        /// <summary>
        /// ElemIDs of the jump environment atoms (non-doped environment)
        /// </summary>
        public List<int> ElemID;

        /// <summary>
        /// true = jump is active = jump can happen in the simulation;
        /// false = jump never happens
        /// </summary>
        public bool JumpActive;

        /// <summary>
        /// true = environment atom is totally ignored, that means it does not exist regarding interactions and coding;
        /// false = environment atom is not ignored
        /// </summary>
        public List<bool> AtomIgnore;

        /// <summary>
        /// true = environment atom is active;
        /// false = environment atom is inactive, that means it is irrelevant if there is a dopand/vacancy on this position or the original element
        /// </summary>
        public List<bool> AtomActive;

        /// <summary>
        /// true = environment atom is additive, that means it has an additive contribution to the jump activation energy;
        /// false = environment atom is non-additive, that means it is used in the environment coding
        /// </summary>
        public List<bool> AtomAdditive;

        /// <summary>
        /// contains for every additive atom (zero-based) in Coord the corresponding CoordID = index in Coord
        /// </summary>
        public List<int> InteractionCoordID;

        /// <summary>
        /// contains for every additive atom (zero-based) in Coord the corresponding WWID = index of the correponding interaction
        /// </summary>
        public List<int> InteractionMap;

        /// <summary>
        /// contains for every additive atom (zero-based) in Coord the possible elements on this position
        /// </summary>
        public List<List<int>> InteractionElemIDs;

        /// <summary>
        /// contains for every additive atom (zero-based) in Coord and for every possible element on this position the additive energy contribution to the jump activation energy in eV
        /// </summary>
        public List<List<double>> InteractionEnergies;

        /// <summary>
        /// Non-equivalent permutations of the occupation of the non-additive jump environment positions by the possible elements (equivalent codes are separated by "-1" in every unique code)
        /// </summary>
        public List<string> UniqueCodes;

        /// <summary>
        /// Jump activation energies of the unique codes in eV
        /// </summary>
        public List<double> UniqueEnergies;

        /// <summary>
        /// Full list of all permutations of the occupation of the non-additive jump environment positions by the possible elements
        /// </summary>
        public List<string> FullCodes;

        /// <summary>
        /// contains the index of the corresponding UniqueCode for every FullCode
        /// </summary>
        public List<int> FullToUniqueMap;

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
        /// Construct new model which holds the current validated unique jump
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        /// <param name="UJumpID">Index of the unique jump</param>
        public TUniqueJump(TMCJobWrapper MCDLL, int JumpID)
        {
            Initialize();

            GetData(MCDLL, JumpID);
        }

        #region Methods

        /// <summary>
        /// Initialize all fields
        /// </summary>
        protected void Initialize()
        {
            Length = 0.0;
            Coord = new List<Vector3D>();
            ElemID = new List<int>();

            JumpActive = true;
            AtomIgnore = new List<bool>();
            AtomActive = new List<bool>();
            AtomAdditive = new List<bool>();

            InteractionCoordID = new List<int>();
            InteractionMap = new List<int>();
            InteractionElemIDs = new List<List<int>>();
            InteractionEnergies = new List<List<double>>();

            UniqueCodes = new List<string>();
            UniqueEnergies = new List<double>();
            FullCodes = new List<string>();
            FullToUniqueMap = new List<int>();

            _IsValid = false;
        }

        /// <summary>
        /// Retrieve data from Monte-Carlo-DLL
        /// </summary>
        protected void GetData(TMCJobWrapper MCDLL, int JumpID)
        {
            int ErrorCode = Constants.KMCERR_OK;
            _IsValid = false;

            // Check if MCDLL is valid object
            if (MCDLL == null)
            {
                ThrowError("MC-DLL object is null (TUniqueJump.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TUniqueJump.GetData)");
            }

            // Receive jump length
            ErrorCode = MCDLL.GetUJumpLength(JumpID, ref Length);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read jump length from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive jump activation state
            ErrorCode = MCDLL.GetUJumpActive(JumpID, ref JumpActive);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read jump activation state from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive number of jump environment atoms
            int t_atomcount = 0;
            ErrorCode = MCDLL.GetUJumpAtomCount(JumpID, ref t_atomcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of environment atoms from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive jump environment
            Coord.Clear();
            ElemID.Clear();
            AtomIgnore.Clear();
            AtomAdditive.Clear();
            AtomActive.Clear();
            if (t_atomcount > 0)
            {
                int t_int = 0;
                double t_ax = 0,t_ay = 0,t_az = 0;
                bool t_b1 = true, t_b2 = true, t_b3 = true;
                for (int i = 0; i < t_atomcount; i++)
                {
                    // Receive coordinates of the environment atom
                    ErrorCode = MCDLL.GetUJumpAtomZylPosition(JumpID, i, ref t_ax, ref t_ay, ref t_az);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read coordinates from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    Coord.Add(new Vector3D(t_ax, t_ay, t_az));

                    // Receive element of the environment atom
                    ErrorCode = MCDLL.GetUJumpAtomElement(JumpID, i, ref t_int);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read element from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    ElemID.Add(t_int);

                    // Receive flags of the environment atom
                    ErrorCode = MCDLL.GetUJumpAtomStates(JumpID, i, ref t_b1, ref t_b2, ref t_b3);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read element from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    AtomIgnore.Add(t_b1);
                    AtomAdditive.Add(t_b2);
                    AtomActive.Add(t_b3);
                }
            }

            // Receive number of additive environment atoms
            int t_addcount = 0;
            ErrorCode = MCDLL.GetUJumpWWAtomsCount(JumpID, ref t_addcount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of additive atoms from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive additive atom information
            InteractionCoordID.Clear();
            InteractionMap.Clear();
            InteractionElemIDs.Clear();
            InteractionEnergies.Clear();
            if (t_addcount > 0)
            {
                int t_coord = 0,t_ww = 0,t_energanz = 0;
                for (int i = 0; i < t_addcount; i++)
                {
                    // Receive indices and counts for additive atom
                    ErrorCode = MCDLL.GetUJumpWWAtomsInfo(JumpID, i, ref t_coord, ref t_ww, ref t_energanz);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read additive atom information from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    InteractionCoordID.Add(t_coord);
                    InteractionMap.Add(t_ww);
                    InteractionElemIDs.Add(new List<int>());
                    InteractionEnergies.Add(new List<double>());

                    // Receive possible elements and the additive energies for the additive position
                    if (t_energanz > 0)
                    {
                        int t_elem = 0;
                        double t_en = 0.0;
                        for (int j = 0; j < t_energanz; j++)
                        {
                            ErrorCode = MCDLL.GetUJumpWWAtomsEnergy(JumpID, i, j, ref t_elem, ref t_en);
                            switch (ErrorCode)
                            {
                                case Constants.KMCERR_OK:
                                    break;
                                default:
                                    ThrowError("Cannot read additive energy from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                                    break;
                            }
                            InteractionElemIDs[i].Add(t_elem);
                            InteractionEnergies[i].Add(t_en);
                        }
                    }
                }
            }

            // Receive number of unique codes
            int t_ucodecount = 0;
            ErrorCode = MCDLL.GetUJumpUCodesCount(JumpID, ref t_ucodecount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of unique codes from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive unique codes
            UniqueCodes.Clear();
            UniqueEnergies.Clear();
            if (t_ucodecount > 0)
            {
                string t_code = "";
                double t_en = 0.0;
                for (int i = 0; i < t_ucodecount; i++)
                {
                    ErrorCode = MCDLL.GetUJumpUCode(JumpID, i, ref t_code, ref t_en);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read unique code from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    UniqueCodes.Add(t_code);
                    UniqueEnergies.Add(t_en);
                }
            }

            // Receive number of full codes
            int t_fcodecount = 0;
            ErrorCode = MCDLL.GetUJumpFCodesCount(JumpID, ref t_fcodecount);
            switch (ErrorCode)
            {
                case Constants.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read number of full codes from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive full codes
            FullCodes.Clear();
            FullToUniqueMap.Clear();
            if (t_fcodecount > 0)
            {
                string t_code = "";
                int t_id = 0;
                for (int i = 0; i < t_fcodecount; i++)
                {
                    ErrorCode = MCDLL.GetUJumpFCode(JumpID, i, ref t_code, ref t_id);
                    switch (ErrorCode)
                    {
                        case Constants.KMCERR_OK:
                            break;
                        default:
                            ThrowError("Cannot read full code from MC object (TUniqueJump.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                            break;
                    }
                    FullCodes.Add(t_code);
                    FullToUniqueMap.Add(t_id);
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

        #region Static Methods

        /// <summary>
        /// Splits a code string into the separate ElemIDs of the included codes
        /// </summary>
        public static List<List<int>> SplitCode(string Code, int MaxElemID)
        {
            List<List<int>> o_code = new List<List<int>>();
            if ((Code == null) || (Code == "")) return o_code;
            
            // Split at "-1" for separate codes
            string[] codes = Code.Trim().Split(new string[] { "-1" }, StringSplitOptions.RemoveEmptyEntries);
            if (codes.Length == 0) return o_code;

            // Split each code into ElemIDs
            string[] separator = new string[] { " " };
            string[] s_elemids = null;
            int t_int = 0;
            List<int> i_elemids = null;
            for (int i = 0; i < codes.Length; i++)
            {
                s_elemids = codes[i].Trim().Split(separator, StringSplitOptions.RemoveEmptyEntries);
                if (s_elemids.Length == 0) continue;
                i_elemids = new List<int>();
                t_int = 0;
                for (int j = 0; j < s_elemids.Length; j++)
                {
                    if (int.TryParse(s_elemids[j], out t_int) == true)
                    {
                        if ((t_int >= 0) && (t_int <= MaxElemID))
                        {
                            i_elemids.Add(t_int);
                        }
                        else
                        {
                            t_int = -1;
                            break;
                        }
                    }
                    else
                    {
                        t_int = -1;
                        break;
                    }
                }
                if (t_int == -1) continue;
                o_code.Add(i_elemids);
            }

            return o_code;
        }

        #endregion Static Methods
    }
}
