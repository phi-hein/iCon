using System;
using System.Globalization;
using System.IO;
using System.Threading;
using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {
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

            // Set up exception handlers
            Application.Current.DispatcherUnhandledException += Application_DispatcherUnhandledException;
            AppDomain.CurrentDomain.UnhandledException += AppDomain_UnhandledException;
            AppDomain.CurrentDomain.FirstChanceException += AppDomain_FirstChanceException;

            // Create and show main window
            MainWindow app_wnd = new MainWindow();
            app_wnd.Show();
        }

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
                Constants.SC_KMC_APPDATA_OUTERFOLDER, Constants.SC_KMC_APPDATA_INNERFOLDER);
            Directory.CreateDirectory(LocalAppDataPath);

            // Save to log file
            string LogPath = Path.Combine(LocalAppDataPath, Constants.SC_KMC_STD_ERRORLOG);
            File.AppendAllText(LogPath, ErrorMsg);
        }

        #endregion Unhandled Exceptions

    }
}
