using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace iCon_General.UserControls
{
    /// <summary>
    /// Interaktionslogik für HorizontalLabeledTextBox.xaml
    /// </summary>
    public partial class HorizontalLabeledTextBox : UserControl
    {
        public HorizontalLabeledTextBox()
        {
            InitializeComponent();
        }


        /// <summary>
        /// Label text (Dependency Property)
        /// </summary>
        public string LabelText
        {
            get { return (string)GetValue(LabelTextProperty); }
            set { SetValue(LabelTextProperty, value); }
        }
        public static readonly DependencyProperty LabelTextProperty =
            DependencyProperty.Register("LabelText", typeof(string), typeof(HorizontalLabeledTextBox),
            new FrameworkPropertyMetadata("") { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// Textbox text (Dependency Property)
        /// </summary>
        public string InputValue
        {
            get { return (string)GetValue(InputValueProperty); }
            set { SetValue(InputValueProperty, value); }
        }
        public static readonly DependencyProperty InputValueProperty =
            DependencyProperty.Register("InputValue", typeof(string), typeof(HorizontalLabeledTextBox),
            new FrameworkPropertyMetadata("") { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

    }
}
