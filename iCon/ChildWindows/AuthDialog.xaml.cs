using System.Security;
using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für AuthDialog.xaml
    /// </summary>
    public partial class AuthDialog : Window
    {
        protected AuthDialog(string title, string prompt, string label, bool show_notice)
        {
            InitializeComponent();

            // Initialize texts
            Title = title;
            txtPrompt.Text = prompt;
            lblResponseOne.LabelText = label;

            // Hide second password box
            lblResponseTwo.Visibility = Visibility.Collapsed;

            // Hide additional notice
            if (show_notice == false) txtNotice.Visibility = Visibility.Collapsed;

            // Set cursor to first password box
            pwbResponseOne.Focus();
        }

        protected AuthDialog(string title, string prompt, string labelone, string labeltwo)
        {
            InitializeComponent();

            // Initialize texts
            Title = title;
            txtPrompt.Text = prompt;
            lblResponseOne.LabelText = labelone;
            lblResponseTwo.LabelText = labeltwo;

            // Hide additional notice
            txtNotice.Visibility = Visibility.Collapsed;

            // Set cursor to first password box
            pwbResponseOne.Focus();
        }

        /// <summary>
        /// Open auth dialog for keyboard-interactive auth and get response
        /// </summary>
        public static SecureString KeyboardInteractivePrompt(string label, string question)
        {
            var dlg = new AuthDialog(
                Properties.Resources.AuthWindow_Title_Interactive,
                label + ": " + question.TrimEnd(new char[] { ':', ' ' }),
                Properties.Resources.AuthWindow_ResponseLbl,
                true)
            {
                Owner = Application.Current.MainWindow
            };
            dlg.ShowDialog();
            if (dlg.DialogResult == true) return dlg.ResponseOne;

            return null;
        }

        /// <summary>
        /// Open auth dialog for only user password and get response
        /// </summary>
        public static SecureString UserPasswordPrompt()
        {
            var dlg = new AuthDialog(
                Properties.Resources.AuthWindow_Title_Passwords,
                Properties.Resources.AuthWindow_PasswordPrompt,
                Properties.Resources.AuthWindow_PasswordLbl_User,
                false)
            {
                Owner = Application.Current.MainWindow
            };
            dlg.ShowDialog();
            if (dlg.DialogResult == true) return dlg.ResponseOne;

            return null;
        }

        /// <summary>
        /// Open auth dialog for only keyfile password and get response
        /// </summary>
        public static SecureString KeyfilePasswordPrompt()
        {
            var dlg = new AuthDialog(
                Properties.Resources.AuthWindow_Title_Passwords,
                Properties.Resources.AuthWindow_PasswordPrompt,
                Properties.Resources.AuthWindow_PasswordLbl_Keyfile,
                false)
            {
                Owner = Application.Current.MainWindow
            };
            dlg.ShowDialog();
            if (dlg.DialogResult == true) return dlg.ResponseOne;

            return null;
        }

        /// <summary>
        /// Open auth dialog for user and keyfile password and get responses
        /// </summary>
        public static (SecureString, SecureString) PasswordsPrompt()
        {
            var dlg = new AuthDialog(
                Properties.Resources.AuthWindow_Title_Passwords,
                Properties.Resources.AuthWindow_PasswordPrompt,
                Properties.Resources.AuthWindow_PasswordLbl_User,
                Properties.Resources.AuthWindow_PasswordLbl_Keyfile)
            {
                Owner = Application.Current.MainWindow
            };
            dlg.ShowDialog();
            if (dlg.DialogResult == true) return (dlg.ResponseOne, dlg.ResponseTwo);

            return (null, null);
        }

        /// <summary>
        /// Response of first password box
        /// </summary>
        public SecureString ResponseOne
        {
            get
            {
                return pwbResponseOne.SecurePassword;
            }
        }

        /// <summary>
        /// Response of second password box
        /// </summary>
        public SecureString ResponseTwo
        {
            get
            {
                return pwbResponseTwo.SecurePassword;
            }
        }

        private void btnOK_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
