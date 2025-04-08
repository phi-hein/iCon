using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für AuthDialog.xaml
    /// </summary>
    public partial class AuthDialog : Window
    {
        public AuthDialog(string label, string question)
        {
            InitializeComponent();

            // Initialize texts
            txtPromptLabel.Content = label;
            txtPromptQuestion.Text = question;

            // Set cursor to password box
            pwbPromptResponse.Focus();
        }

        /// <summary>
        /// Open auth dialog and get response
        /// </summary>
        public static string Prompt(string label, string question)
        {
            AuthDialog dlg = new AuthDialog(label, question);
            dlg.Owner = Application.Current.MainWindow;
            dlg.ShowDialog();
            if (dlg.DialogResult == true)
                return dlg.PromptResponse;
            return null;
        }

        /// <summary>
        /// Response text
        /// </summary>
        public string PromptResponse
        {
            get
            {
                return pwbPromptResponse.Password;
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
