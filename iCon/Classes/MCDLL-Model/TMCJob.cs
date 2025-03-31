using System;

namespace iCon_General
{
    /// <summary>
    /// Light-weight class for holding the complete, last validated TMCJob-DLL-data in one object
    /// </summary>
    public class TMCJob
    {
        #region Fields

        /// <summary>
        /// Name of the MC-Project
        /// </summary>
        public string ProjectName;
		
        /// <summary>
        /// Name of the user, who created the project
        /// </summary>
        public string UserName;
		
	    /// <summary>
        /// Date of the project creation
	    /// </summary>
        public string ProjectDate;		     

        /// <summary>
        /// Project description
        /// </summary>
        public string ProjectDescription;

        /// <summary>
        /// Project status
        /// </summary>
        public int ProjectState;
        
        /// <summary>
        /// Complete project as text
        /// </summary>
        public string FileContent;

        #endregion Fields

        #region Objects

        /// <summary>
        /// Description of the used elements
        /// </summary>
        public TElements Elements;

        /// <summary>
        /// Description of the crystal structure
        /// </summary>
        public TStructure Structure;

        /// <summary>
        /// Description of all possible lattice jumps
        /// </summary>
        public TJumps Jumps;

        /// <summary>
        /// Description of unique jumps and interactions
        /// </summary>
        public TUniqueJumps UJumps;

        /// <summary>
        /// Description of the simulation settings
        /// </summary>
        public TSettings Settings;

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
                if (Elements != null)
                {
                    if (Elements.IsValid == false) return false;
                }
                else return false;
                if (Structure != null)
                {
                    if (Structure.IsValid == false) return false;
                }
                else return false;
                if (Jumps != null)
                {
                    if (Jumps.IsValid == false) return false;
                }
                else return false;
                if (UJumps != null)
                {
                    if (UJumps.IsValid == false) return false;
                }
                else return false;
                if (Settings != null)
                {
                    if (Settings.IsValid == false) return false;
                }
                else return false;
                return _IsValid;
            }
        }

        #endregion Properties

        /// <summary>
        /// Construct new model which holds the current state of the DLL content
        /// </summary>
        /// <param name="MCDLL">Wrapper object for the Monte-Carlo-DLL</param>
        public TMCJob(TMCJobWrapper MCDLL)
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
            ProjectName = "";
            UserName = "";
            ProjectDate = "";
            ProjectDescription = "";
            ProjectState = -1;
            FileContent = "";

            Elements = null;
            Structure = null;
            Jumps = null;
            UJumps = null;
            Settings = null;

            _IsValid = false;
        }

        /// <summary>
        /// Retrieve data from Monte-Carlo-DLL
        /// </summary>
        protected void GetData(TMCJobWrapper MCDLL)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            _IsValid = false;

            // Check if MCDLL is valid object
            if (MCDLL == null)
            {
                ThrowError("MC-DLL object is null (TMCJob.GetData)");
            }

            // Receive DLL status
            if (MCDLL.IfReady() == false)
            {
                ThrowError("MC-DLL object is not ready (TMCJob.GetData)");
            }

            // Receive project name
            ErrorCode = MCDLL.GetProjectName(ref ProjectName);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read project name from MC object (TMCJob.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive user name
            ErrorCode = MCDLL.GetUserName(ref UserName);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read user name from MC object (TMCJob.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive project date
            ErrorCode = MCDLL.GetProjectDate(ref ProjectDate);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read date from MC object (TMCJob.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive project description
            ErrorCode = MCDLL.GetProjectDescription(ref ProjectDescription);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read description from MC object (TMCJob.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive project state
            ErrorCode = MCDLL.GetProjectState(ref ProjectState);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read project state from MC object (TMCJob.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Receive project file as text
            ErrorCode = MCDLL.SaveToString(ref FileContent);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                default:
                    ThrowError("Cannot read file content from MC object (TMCJob.GetData, ErrorCode: " + ErrorCode.ToString() + ")");
                    break;
            }

            // Get sub-objects
            Elements = new TElements(MCDLL);
            Structure = new TStructure(MCDLL);
            Jumps = new TJumps(MCDLL);
            UJumps = new TUniqueJumps(MCDLL);
            Settings = new TSettings(MCDLL);

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
