using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

[assembly: ThemeInfo(
    ResourceDictionaryLocation.None, //Speicherort der designspezifischen Ressourcenwörterbücher
                                     //(wird verwendet, wenn eine Ressource auf der Seite
                                     // oder in den Anwendungsressourcen-Wörterbüchern nicht gefunden werden kann.)
    ResourceDictionaryLocation.SourceAssembly //Speicherort des generischen Ressourcenwörterbuchs
                                              //(wird verwendet, wenn eine Ressource auf der Seite, in der Anwendung oder einem 
                                              // designspezifischen Ressourcenwörterbuch nicht gefunden werden kann.)
)]

namespace iCon_General
{
    /// <summary>
    /// Custom entry point for singleton app
    /// </summary>
    class Program
    {
        /// <summary>
        /// Unique mutex string
        /// </summary>
        private const string UniqueAppName = "iConPHBOHGUniqueIDText267933";

        [STAThread]
        public static void Main()
        {
            string app_identifier = UniqueAppName + Environment.UserName;

            // Check if this is the first instance
            using var mutex = new Mutex(true, app_identifier, out bool is_first_instance);

            if (is_first_instance)
            {
                // Create application object
                var application = new App();

                // Initialize application
                application.InitializeComponent();

                // Run the first application instance
                application.Run();
            }
            else
            {
                // Show MainWindow
                if (Application.Current.MainWindow.IsVisible == false) 
                { 
                    Application.Current.MainWindow.Show(); 
                }
                if (Application.Current.MainWindow.WindowState == WindowState.Minimized) 
                { 
                    Application.Current.MainWindow.WindowState = WindowState.Normal; 
                }
                Application.Current.MainWindow.Activate();
                Application.Current.MainWindow.Topmost = true;
                Application.Current.MainWindow.Topmost = false;
                Application.Current.MainWindow.Focus();
            }
        }
    }
}
