using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;

namespace iCon_General.UserControls
{
    /// <summary>
    /// Interaktionslogik für HorizontalLabeledColorSelector.xaml
    /// </summary>
    public partial class HorizontalLabeledColorSelector : UserControl
    {
        public HorizontalLabeledColorSelector()
        {
            InitializeComponent();
        }


        /// <summary>
        /// Text of the label (Dependency Property)
        /// </summary>
        public string LabelText
        {
            get { return (string)GetValue(LabelTextProperty); }
            set { SetValue(LabelTextProperty, value); }
        }
        public static readonly DependencyProperty LabelTextProperty =
            DependencyProperty.Register("LabelText", typeof(string), typeof(HorizontalLabeledColorSelector),
            new FrameworkPropertyMetadata("") { BindsTwoWayByDefault=true, DefaultUpdateSourceTrigger=UpdateSourceTrigger.PropertyChanged });

        
        /// <summary>
        /// Selected color (Dependency Property)
        /// </summary>
        public Color InputValue
        {
            get { return (Color)GetValue(InputValueProperty); }
            set { SetValue(InputValueProperty, value); }
        }
        public static readonly DependencyProperty InputValueProperty =
            DependencyProperty.Register("InputValue", typeof(Color), typeof(HorizontalLabeledColorSelector),
            new FrameworkPropertyMetadata(Colors.White) { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        
    }
}
