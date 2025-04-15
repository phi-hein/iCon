using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für FingerPrintDialog.xaml
    /// </summary>
    public partial class FingerPrintDialog : Window
    {
        public FingerPrintDialog(string hostname, string keytype, string keylength, string fingerprint)
        {
            InitializeComponent();

            // Initialize texts
            txtHostName.Text = hostname.Trim();
            txtHostKeyType.Text = keytype.Trim();
            txtHostKeyLength.Text = keylength.Trim();
            txtHostFingerPrint.Text = fingerprint.Trim();
        }

        /// <summary>
        /// Open fingerprint dialog and get response
        /// </summary>
        public static bool Prompt(string hostname, string keytype, string keylength, string fingerprint)
        {
            FingerPrintDialog dlg = new FingerPrintDialog(hostname, keytype, keylength, fingerprint);
            dlg.Owner = Application.Current.MainWindow;
            dlg.ShowDialog();
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
