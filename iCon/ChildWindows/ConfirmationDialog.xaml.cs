using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für ConfirmationDialog.xaml
    /// </summary>
    public partial class ConfirmationDialog : Window
    {
        public ConfirmationDialog(string message, string question, bool default_ischecked)
        {
            InitializeComponent();

            // Initialize texts
            txtMessage.Text = message.Trim();
            ckbQuestion.Content = question.Trim();
            ckbQuestion.IsChecked = default_ischecked;
        }

        /// <summary>
        /// IsChecked state of the checkbox in the dialog
        /// </summary>
        public bool IsChecked
        {
            get
            {
                return (ckbQuestion.IsChecked == true);
            }
        }

        /// <summary>
        /// Open confirmation dialog and get response
        /// </summary>
        public static bool Prompt(string message, string question, ref bool is_checked)
        {
            ConfirmationDialog dlg = new ConfirmationDialog(message, question, is_checked);
            dlg.Owner = Application.Current.MainWindow;
            dlg.ShowDialog();
            is_checked = dlg.IsChecked;
            return (dlg.DialogResult == true);
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
