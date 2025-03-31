using System;

namespace iCon_General
{
    /// <summary>
    /// Holds data for Project File Tab
    /// </summary>
    public class TVMProjectFile: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMProjectFile(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _FileContent = "";
        }

        #region Properties

        protected string _FileContent;
        /// <summary> 
        /// Complete project as file 
        /// </summary>
        public string FileContent
        {
            get
            {
                return _FileContent;
            }
            protected set
            {
                if (value != _FileContent)
                {
                    _FileContent = value;
                    Notify("FileContent");
                }
            }
        }

        #endregion Properties

        #region Synchronization Methods

        /// <summary>
        /// Fill this part of the viewmodel with the current model data from _ViewModel
        /// </summary>
        public void GetData()
        {
            if (_ViewModel == null) throw new ApplicationException("Null pointer (TVMProjectFile.GetData)");

            GetData(_ViewModel.MCJob);
        }

        /// <summary>
        /// Fill this part of the viewmodel with the current model data
        /// </summary>
        public void GetData(TMCJob MCJob)
        {
            if (MCJob == null) throw new ApplicationException("Null argument (TVMProjectFile.GetData)");
            if (MCJob.IsValid == false) throw new ApplicationException("Invalid argument (TVMProjectFile.GetData)");

            // Transfer data (skip validation by using fields)
            _FileContent = MCJob.FileContent;

            // Notify for binding updates
            Notify("FileContent");
        }

        #endregion Synchronization Methods
    }
}
