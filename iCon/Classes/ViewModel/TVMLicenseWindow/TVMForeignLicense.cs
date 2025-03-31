namespace iCon_General
{
    public class TVMForeignLicense : ViewModelObject
    {
        #region Properties

        protected string _Title;
        /// <summary>
        /// Title of the licensed tool
        /// </summary>
        public string Title
        {
            get
            {
                return _Title;
            }
            set
            {
                if (value != _Title)
                {
                    _Title = value;
                    Notify("Title");
                }
            }
        }

        protected string _LicenseText;
        /// <summary>
        /// License text
        /// </summary>
        public string LicenseText
        {
            get
            {
                return _LicenseText;
            }
            set
            {
                if (value != _LicenseText)
                {
                    _LicenseText = value;
                    Notify("LicenseText");
                }
            }
        }

        #endregion Properties

        public TVMForeignLicense()
        {
            // Initialize Properties
            _Title = "";
            _LicenseText = "";
        }

        public TVMForeignLicense(string i_Title, string i_Text)
        {
            // Initialize Properties
            _Title = i_Title;
            _LicenseText = i_Text;
        }
    }
}
