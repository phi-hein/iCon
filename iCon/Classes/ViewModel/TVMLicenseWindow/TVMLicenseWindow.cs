using System.Collections.ObjectModel;
using System.Reflection;
using HelixToolkit.Wpf;
using Renci.SshNet;
using Xceed.Wpf.Toolkit;

namespace iCon_General
{
    public class TVMLicenseWindow : ViewModelObject
    {
        #region Properties

        protected ObservableCollection<TVMLicense> _Licenses;
        /// <summary>
        /// List of all licensed tools that are used within this program
        /// </summary>
        public ObservableCollection<TVMLicense> Licenses
        {
            get
            {
                return _Licenses;
            }
            set
            {
                if (value != _Licenses)
                {
                    _Licenses = value;
                    Notify("Licenses");
                }
            }
        }

        protected TVMLicense _SelectedLicense;
        /// <summary>
        /// Selected license
        /// </summary>
        public TVMLicense SelectedLicense
        {
            get
            {
                return _SelectedLicense;
            }
            set
            {
                if (value != _SelectedLicense)
                {
                    _SelectedLicense = value;
                    Notify("SelectedLicense");
                }
            }
        }

        protected string _ProgramTitle;
        /// <summary>
        /// Program title
        /// </summary>
        public string ProgramTitle
        {
            get
            {
                return _ProgramTitle;
            }
            set
            {
                if (value != _ProgramTitle)
                {
                    _ProgramTitle = value;
                    Notify("ProgramTitle");
                }
            }
        }

        protected string _ProgramDescription;
        /// <summary>
        /// Description of the program
        /// </summary>
        public string ProgramDescription
        {
            get
            {
                return _ProgramDescription;
            }
            set
            {
                if (value != _ProgramDescription)
                {
                    _ProgramDescription = value;
                    Notify("ProgramDescription");
                }
            }
        }

        protected string _ProgramVersion;
        /// <summary>
        /// Version information of the program
        /// </summary>
        public string ProgramVersion
        {
            get
            {
                return _ProgramVersion;
            }
            set
            {
                if (value != _ProgramVersion)
                {
                    _ProgramVersion = value;
                    Notify("ProgramVersion");
                }
            }
        }

        protected string _Company;
        /// <summary>
        /// Company information
        /// </summary>
        public string Company
        {
            get
            {
                return _Company;
            }
            set
            {
                if (value != _Company)
                {
                    _Company = value;
                    Notify("Company");
                }
            }
        }

        protected string _Copyright;
        /// <summary>
        /// Copyright or license information
        /// </summary>
        public string Copyright
        {
            get
            {
                return _Copyright;
            }
            set
            {
                if (value != _Copyright)
                {
                    _Copyright = value;
                    Notify("Copyright");
                }
            }
        }

        #endregion Properties

        public TVMLicenseWindow()
        {
            // Initialize properties
            _Licenses = new ObservableCollection<TVMLicense>();
            _SelectedLicense = null;

            // Get assembly information
            Assembly t_assembly = Assembly.GetExecutingAssembly();
            _ProgramTitle = ((AssemblyTitleAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyTitleAttribute), false)[0]).Title;
            _ProgramDescription = ((AssemblyDescriptionAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyDescriptionAttribute), false)[0]).Description;
            _ProgramVersion = t_assembly.GetName().Version.ToString();
            _Copyright = ((AssemblyCopyrightAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false)[0]).Copyright;
            _Company = ((AssemblyCompanyAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyCompanyAttribute), false)[0]).Company;

            // Set iCon License
            _Licenses.Add(new TVMLicense
            {
                Title = _ProgramTitle + " (Version " + _ProgramVersion + ")",
                LicenseText = Properties.Resources.EULA
            });

            // Set Helix Toolkit License
            _Licenses.Add(new TVMLicense
            {
                Title = Properties.Resources.PackageTitle_HelixToolkit,
                LicenseText = Properties.Resources.License_HelixToolkit
            });

            // Set SSH.NET Library License
            _Licenses.Add(new TVMLicense
            {
                Title = Properties.Resources.PackageTitle_SSHNet,
                LicenseText = Properties.Resources.License_SSHNet
            });

            // Set BouncyCastle Library License
            _Licenses.Add(new TVMLicense
            {
                Title = Properties.Resources.PackageTitle_BouncyCastle,
                LicenseText = Properties.Resources.License_BouncyCastle
            });

            _SelectedLicense = _Licenses[0];
        }
    }
}
