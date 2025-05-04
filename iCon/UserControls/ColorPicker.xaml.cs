using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;

namespace iCon_General.UserControls
{
    /// <summary>
    /// Interaktionslogik für ColorPicker.xaml
    /// </summary>
    public partial class ColorPicker : UserControl
    {
        public ColorPicker()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Selected color (Dependency Property)
        /// </summary>
        public Color SelectedColor
        {
            get { return (Color)GetValue(SelectedColorProperty); }
            set { SetValue(SelectedColorProperty, value); }
        }
        public static readonly DependencyProperty SelectedColorProperty =
            DependencyProperty.Register("SelectedColor", typeof(Color), typeof(ColorPicker),
            new FrameworkPropertyMetadata(Colors.Black) { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Show alpha selection (Dependency Property)
        /// </summary>
        public bool ShowAlpha
        {
            get { return (bool)GetValue(ShowAlphaProperty); }
            set { SetValue(ShowAlphaProperty, value); }
        }
        public static readonly DependencyProperty ShowAlphaProperty =
            DependencyProperty.Register("ShowAlpha", typeof(bool), typeof(ColorPicker),
            new FrameworkPropertyMetadata(true) { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

    }
}
