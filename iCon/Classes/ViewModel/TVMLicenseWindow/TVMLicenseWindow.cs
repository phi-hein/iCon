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

        protected ObservableCollection<TVMForeignLicense> _LicensedTools;
        /// <summary>
        /// List of all licensed tools that are used within this program
        /// </summary>
        public ObservableCollection<TVMForeignLicense> LicensedTools
        {
            get
            {
                return _LicensedTools;
            }
            set
            {
                if (value != _LicensedTools)
                {
                    _LicensedTools = value;
                    Notify("LicensedTools");
                }
            }
        }

        protected TVMForeignLicense _SelectedLicense;
        /// <summary>
        /// Selected license
        /// </summary>
        public TVMForeignLicense SelectedLicense
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
            _LicensedTools = new ObservableCollection<TVMForeignLicense>();
            _SelectedLicense = null;

            // Get assembly information
            Assembly t_assembly = Assembly.GetExecutingAssembly();
            _ProgramTitle = ((AssemblyTitleAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyTitleAttribute), false)[0]).Title;
            _ProgramDescription = ((AssemblyDescriptionAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyDescriptionAttribute), false)[0]).Description;
            _ProgramVersion = t_assembly.GetName().Version.ToString();
            _Copyright = ((AssemblyCopyrightAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false)[0]).Copyright;
            _Company = ((AssemblyCompanyAttribute)t_assembly.GetCustomAttributes(typeof(AssemblyCompanyAttribute), false)[0]).Company;

            // Set Helix Toolkit License
            TVMForeignLicense t_lic = new TVMForeignLicense();
            t_lic.Title = "Helix 3D Toolkit (Version " + Assembly.GetAssembly(typeof(HelixViewport3D)).GetName().Version.ToString() + ")";
            t_lic.LicenseText = "Helix 3D Toolkit\nMIT License\nCopyright (c) 2012 Oystein Bjorke\n\nPermission is hereby granted, free of charge, to any " +
                "person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software " +
                "without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, " +
                "and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\n" +
                "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\n" +
                "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES " +
                "OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE " +
                "LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN " +
                "CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";
            _LicensedTools.Add(t_lic);

            // Set WPF Extended Toolkit License
            t_lic = new TVMForeignLicense();
            t_lic.Title = "Extended WPF Toolkit\u2122 - Community Edition (Version " + Assembly.GetAssembly(typeof(ColorPicker)).GetName().Version.ToString() + ")";
            t_lic.LicenseText = "Extended WPF Toolkit\u2122 - Community Edition\nMicrosoft Public License (Ms-PL)\nThis license governs use of the " +
                "accompanying software. If you use the software, you accept this license. If you do not accept the license, do not use the software.\n\n" +
                "1. Definitions\n\nThe terms \"reproduce\", \"reproduction\", \"derivative works\" and \"distribution\" have the same meaning here as " +
                "under U.S. copyright law.\n\nA \"contribution\" is the original software, or any additions or changes to the software.\n\nA \"contributor\" " +
                "is any person that distributes its contribution under this license.\n\n\"Licensed patents\" are a contributor's patent claims that read " +
                "directly on its contribution.\n\n2. Grant of Rights\n\n(A) Copyright Grant- Subject to the terms of this license, including the license " +
                "conditions and limitations in section 3, each contributor grants you a non-exclusive, worldwide, royalty-free copyright license to reproduce " +
                "its contribution, prepare derivative works of its contribution, and distribute its contribution or any derivative works that you create.\n\n" +
                "(B) Patent Grant- Subject to the terms of this license, including the license conditions and limitations in section 3, each contributor " +
                "grants you a non-exclusive, worldwide, royalty-free license under its licensed patents to make, have made, use, sell, offer for sale, " +
                "import, and/or otherwise dispose of its contribution in the software or derivative works of the contribution in the software.\n\n" +
                "3. Conditions and Limitations\n\n(A) No Trademark License- This license does not grant you rights to use any contributors' name, logo, " +
                "or trademarks.\n\n(B) If you bring a patent claim against any contributor over patents that you claim are infringed by the software, " +
                "your patent license from such contributor to the software ends automatically.\n\n(C) If you distribute any portion of the software, " +
                "you must retain all copyright, patent, trademark, and attribution notices that are present in the software.\n\n(D) If you distribute " +
                "any portion of the software in source code form, you may do so only under this license by including a complete copy of this license " +
                "with your distribution. If you distribute any portion of the software in compiled or object code form, you may only do so under a license " +
                "that complies with this license.\n\n(E) The software is licensed \"as-is.\" You bear the risk of using it. The contributors give no " +
                "express warranties, guarantees or conditions. You may have additional consumer rights under your local laws which this license cannot " +
                "change. To the extent permitted under your local laws, the contributors exclude the implied warranties of merchantability, fitness for a " +
                "particular purpose and non-infringement.";
            _LicensedTools.Add(t_lic);

            // Set SSH.NET Library License
            t_lic = new TVMForeignLicense();
            t_lic.Title = "SSH.NET Library (Version " + Assembly.GetAssembly(typeof(SshClient)).GetName().Version.ToString() + ")";
            t_lic.LicenseText = "SSH.NET Library\nNew BSD License\nCopyright (c) 2010, RENCI\nAll rights reserved.\n\nRedistribution and use in source " +
                "and binary forms, with or without modification, are permitted provided that the following conditions are met:\n\n* Redistributions of " +
                "source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n\n* Redistributions in binary " +
                "form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other " +
                "materials provided with the distribution.\n\n* Neither the name of RENCI nor the names of its contributors may be used to endorse or " +
                "promote products derived from this software without specific prior written permission.\n\nTHIS SOFTWARE IS PROVIDED BY THE COPYRIGHT " +
                "HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF " +
                "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR " +
                "ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE " +
                "GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN " +
                "CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED " +
                "OF THE POSSIBILITY OF SUCH DAMAGE.";
            _LicensedTools.Add(t_lic);

            _SelectedLicense = _LicensedTools[0];
        }
    }
}
