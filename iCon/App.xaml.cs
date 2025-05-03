using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;
using System.Windows;
using Microsoft.Shell;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application, ISingleInstanceApp
    {
        /// <summary>
        /// Unique mutex string
        /// </summary>
        private const string UniqueAppName = "iConPHBOHGUniqueIDText267933";

        [STAThread]
        public static void Main()
        {
            // Check if this is the first instance
            if (SingleInstance<App>.InitializeAsFirstInstance(UniqueAppName))
            {
                // Create application object
                var application = new App();

                // Initialize application
                application.InitializeComponent();

                // Run the first application instance
                application.Run();

                // Perform cleanup operations
                SingleInstance<App>.Cleanup();
            }
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            // Set culture
            CultureInfo culture = new CultureInfo("en-US");
            culture.NumberFormat.NegativeSign = "-";
            culture.NumberFormat.PositiveSign = "+";
            culture.NumberFormat.NumberGroupSeparator = "";
            culture.NumberFormat.NumberDecimalSeparator = ".";
            culture.NumberFormat.NumberNegativePattern = 1;
            Thread.CurrentThread.CurrentCulture = culture;
            Thread.CurrentThread.CurrentUICulture = culture;
            CultureInfo.DefaultThreadCurrentCulture = culture;
            CultureInfo.DefaultThreadCurrentUICulture = culture;

            // Link resources
            ResourceDictionary global_dic = new ResourceDictionary() { Source = new Uri("Resources/GlobalDictionary.xaml", UriKind.RelativeOrAbsolute) };
            this.Resources = global_dic;

            // Set up exception handlers
            Application.Current.DispatcherUnhandledException += Application_DispatcherUnhandledException;
            AppDomain.CurrentDomain.UnhandledException += AppDomain_UnhandledException;
            AppDomain.CurrentDomain.FirstChanceException += AppDomain_FirstChanceException;

            // Create and show main window
            MainWindow app_wnd = new MainWindow();
            app_wnd.Show();
        }

        #region ISingleInstanceApp Implementation

        /// <summary>
        /// This method gets called when a second instance is opened
        /// </summary>
        public bool SignalExternalCommandLineArgs(IList<string> args)
        {
            // Handle command line arguments of second instance
            // Example: return ((iCon_General.MainWindow)this.MainWindow).ProcessCommandLineArgs(args);

            // Show MainWindow
            if (Current.MainWindow.IsVisible == false) Current.MainWindow.Show();
            if (Current.MainWindow.WindowState == WindowState.Minimized) Current.MainWindow.WindowState = WindowState.Normal;
            Current.MainWindow.Activate();
            Current.MainWindow.Topmost = true;
            Current.MainWindow.Topmost = false;
            Current.MainWindow.Focus();

            return true;
        }

        #endregion ISingleInstanceApp Implementation

        #region Unhandled Exceptions

        /// <summary>
        /// Catches unhandled exceptions on main UI thread
        /// </summary>
        private void Application_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            // Create error text
            Exception curr_exept = e.Exception;
            string ErrorMsg = "iCon-Exception on main UI thread (" + DateTime.Now.ToString() + ") = "
                + curr_exept.GetType().Name + ": " + curr_exept.Message;
            string PreSpace = "";
            while (curr_exept.InnerException != null)
            {
                curr_exept = curr_exept.InnerException;
                PreSpace += "  ";
                ErrorMsg += Environment.NewLine + PreSpace + "-> " + curr_exept.GetType().Name + ": " + curr_exept.Message;
            }

            // Notify user
            MessageBox.Show("A critical error occured on the main thread. The program will terminate." + Environment.NewLine +
                "Please note the following error message and contact the developer:" + Environment.NewLine + ErrorMsg,
                "iCon - Error", MessageBoxButton.OK, MessageBoxImage.Error);

            // Mark exception as handled to prevent automatical Windows exception handling
            e.Handled = true;

            // Set MainWindow-Flag
            ((iCon_General.MainWindow)this.MainWindow).HasCritException = true;

            // Shutdown application
            Application.Current.Shutdown();
        }

        /// <summary>
        /// Catches unhandled exceptions on all other threads
        /// </summary>
        private void AppDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            // Create error text
            Exception curr_exept = (Exception)e.ExceptionObject;
            string ErrorMsg = "iCon-Exception (" + DateTime.Now.ToString() + ") = "
                + curr_exept.GetType().Name + ": " + curr_exept.Message;
            string PreSpace = "";
            while (curr_exept.InnerException != null)
            {
                curr_exept = curr_exept.InnerException;
                PreSpace += "  ";
                ErrorMsg += Environment.NewLine + PreSpace + "-> " + curr_exept.GetType().Name + ": " + curr_exept.Message;
            }

            // Notify user
            MessageBox.Show("A critical error occured on a non-UI thread. The program will terminate." + Environment.NewLine +
                "Please note the following error message and contact the developer:" + Environment.NewLine + ErrorMsg,
                "iCon - Error", MessageBoxButton.OK, MessageBoxImage.Error);

            // Set MainWindow-Flag
            ((iCon_General.MainWindow)this.MainWindow).HasCritException = true;

            // Shutdown application
            Application.Current.Shutdown();
        }

        /// <summary>
        /// Log all application-wide exceptions
        /// </summary>
        void AppDomain_FirstChanceException(object sender, System.Runtime.ExceptionServices.FirstChanceExceptionEventArgs e)
        {
            // Create error text
            Exception curr_exept = e.Exception;
            string ErrorMsg = "iCon-Exception (" + DateTime.Now.ToString() + ") = "
                + curr_exept.GetType().Name + ": " + curr_exept.Message + Environment.NewLine;
            string PreSpace = "";
            while (curr_exept.InnerException != null)
            {
                curr_exept = curr_exept.InnerException;
                PreSpace += "  ";
                ErrorMsg += PreSpace + "-> " + curr_exept.GetType().Name + ": " + curr_exept.Message + Environment.NewLine;
            }

            // Create app data folder if necessary
            string LocalAppDataPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                ConstantsClass.SC_KMC_APPDATA_OUTERFOLDER, ConstantsClass.SC_KMC_APPDATA_INNERFOLDER);
            Directory.CreateDirectory(LocalAppDataPath);

            // Save to log file
            string LogPath = Path.Combine(LocalAppDataPath, ConstantsClass.SC_KMC_STD_ERRORLOG);
            File.AppendAllText(LogPath, ErrorMsg);
        }

        #endregion Unhandled Exceptions

    }
}
