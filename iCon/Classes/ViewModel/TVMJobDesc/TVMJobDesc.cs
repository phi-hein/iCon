using System;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;

namespace iCon_General
{
    /// <summary> 
    /// Holds data for Job Description tab 
    /// </summary>
    public class TVMJobDesc: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMJobDesc(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _ProjectName = "";
            _UserName = "";
            _ProjectDate = "";
            _ProjectDescription = "";
        }

        #region Properties

        protected string _ProjectName;
        /// <summary> 
        /// Name of the project 
        /// </summary>
        [Required(AllowEmptyStrings=false,ErrorMessage="Enter valid project name.")]
        [StringLength(100, MinimumLength = 2, ErrorMessage = "Project name has to contain 2 - 100 characters.")]
        [RegularExpression(@"^[A-Za-z0-9_\-\^\(\)\s]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9, _-^() and space.")]
        public string ProjectName
        {
            get
            {
                return _ProjectName;
            }
            set
            {
                if (ValidateNotify("ProjectName", value, ref _ProjectName) == true)
                {
                    _ViewModel.IsJobDescSynchronized = false;
                }
            }
        }

        protected string _UserName;
        /// <summary> 
        /// User name for the project 
        /// </summary>
        [StringLength(100, ErrorMessage = "User name can contain a maximum of 100 characters.")]
        [RegularExpression(@"^[A-Za-z0-9]*$", ErrorMessage = "Allowed characters: a-z, A-Z, 0-9.")]
        public string UserName
        {
            get
            {
                return _UserName;
            }
            set
            {
                if (ValidateNotify("UserName", value, ref _UserName) == true)
                {
                    _ViewModel.IsJobDescSynchronized = false;
                }
            }
        }

        protected string _ProjectDate;
        /// <summary> 
        /// Date of creation for the project (public readonly) 
        /// </summary>
        public string ProjectDate
        {
            get
            {
                return _ProjectDate;
            }
            protected set
            {
                if (value != _ProjectDate)
                {
                    _ProjectDate = value;
                    _ViewModel.IsJobDescSynchronized = false;
                    Notify("ProjectDate");
                }
            }
        }

        protected string _ProjectDescription;
        /// <summary> 
        /// Description text for the project 
        /// </summary>
        [StringLength(10000, ErrorMessage = "Project description can contain a maximum of 10000 characters.")]
        public string ProjectDescription
        {
            get
            {
                return _ProjectDescription;
            }
            set
            {
                if (ValidateNotify("ProjectDescription", value, ref _ProjectDescription) == true)
                {
                    _ViewModel.IsJobDescSynchronized = false;
                }
            }
        }

        #endregion Properties

        #region Synchronization Methods

        /// <summary>
        /// Apply the user input values to the model (use only on a background worker thread)
        /// </summary>
        public void ApplyData(TMCJobWrapper MCDLL, BackgroundWorker BWorker, DoWorkEventArgs e)
        {
            int ErrorCode = ConstantsClass.KMCERR_OK;
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(0, "Validating project name ... ");

            // Send project name
            ErrorCode = MCDLL.SetProjectName(_ProjectName);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid project name\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid project name (TVMJobDesc.ApplyData, Errorcode: " + ErrorCode.ToString() + ")");
            }

            BWorker.ReportProgress(25, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(25, "Validating user name ... ");

            // Send user name
            ErrorCode = MCDLL.SetUserName(_UserName);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid user name\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid user name (TVMJobDesc.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            BWorker.ReportProgress(50, "OK\n");
            System.Threading.Thread.Sleep(ConstantsClass.THREAD_READING_DELAY);
            if (BWorker.CancellationPending == true) { e.Cancel = true; return; }
            BWorker.ReportProgress(50, "Validating description ... ");

            // Send project description
            ErrorCode = MCDLL.SetProjectDescription(_ProjectDescription);
            switch (ErrorCode)
            {
                case ConstantsClass.KMCERR_OK:
                    break;
                case ConstantsClass.KMCERR_INVALID_INPUT:
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid project description\n(see console for details)\n",
                        ConstantsClass.KMCERR_INVALID_INPUT, false);
                    return;
                default:
                    throw new ApplicationException("Invalid project description (TVMJobDesc.ApplyData, ErrorCode: " + ErrorCode.ToString() + ")");
            }

            // Retrieve data from MCDLL
            BWorker.ReportProgress(75, "OK\nLoading ... ");
            e.Result = new TMCJob(MCDLL);
            BWorker.ReportProgress(100, "OK");

            System.Threading.Thread.Sleep(ConstantsClass.THREAD_FINISH_DELAY);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data from _ViewModel
        /// </summary>
        public void GetData()
        {
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMJobDesc.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMJobDesc.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMJobDesc.GetData)");

            // Transfer data (skip validation by using fields)
            _ProjectName = MCJob.ProjectName;
            _UserName = MCJob.UserName;
            _ProjectDate = MCJob.ProjectDate;
            _ProjectDescription = MCJob.ProjectDescription;

            // Notify for binding updates
            Notify("ProjectName");
            Notify("UserName");
            Notify("ProjectDate");
            Notify("ProjectDescription");

            _ViewModel.IsJobDescSynchronized = true;
        }

        #endregion Synchronization Methods
    }
}
