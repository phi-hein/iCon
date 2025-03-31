using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für LicenseWindow.xaml
    /// </summary>
    public partial class LicenseWindow : Window
    {
        #region Fields

        /// <summary> 
        /// ViewModel for LicenseWindow 
        /// </summary>
        private TVMLicenseWindow LicenseViewModel = null;

        #endregion Fields

        public LicenseWindow()
        {
            InitializeComponent();

            LicenseViewModel = new TVMLicenseWindow();
            this.DataContext = LicenseViewModel;
        }


    }
}
