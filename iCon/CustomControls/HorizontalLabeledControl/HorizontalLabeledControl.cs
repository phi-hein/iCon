using System.Windows;
using System.Windows.Controls;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// Control that contains a label on the left side and a stretched content on the right
    /// </summary>
    public class HorizontalLabeledControl : ContentControl
    {
        static HorizontalLabeledControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(HorizontalLabeledControl), new FrameworkPropertyMetadata(typeof(HorizontalLabeledControl)));
        }


        /// <summary>
        /// Text of the label
        /// </summary>
        public string LabelText
        {
            get { return (string)GetValue(LabelTextProperty); }
            set { SetValue(LabelTextProperty, value); }
        }
        public static readonly DependencyProperty LabelTextProperty =
            DependencyProperty.Register("LabelText", typeof(string), typeof(HorizontalLabeledControl), new PropertyMetadata(""));


        /// <summary>
        /// FontWeight of the label
        /// </summary>
        public FontWeight LabelFontWeight
        {
            get { return (FontWeight)GetValue(LabelFontWeightProperty); }
            set { SetValue(LabelFontWeightProperty, value); }
        }
        public static readonly DependencyProperty LabelFontWeightProperty =
            DependencyProperty.Register("LabelFontWeight", typeof(FontWeight), typeof(HorizontalLabeledControl), new PropertyMetadata());


        /// <summary>
        /// Margin of the label
        /// </summary>
        public Thickness LabelMargin
        {
            get { return (Thickness)GetValue(LabelMarginProperty); }
            set { SetValue(LabelMarginProperty, value); }
        }
        public static readonly DependencyProperty LabelMarginProperty =
            DependencyProperty.Register("LabelMargin", typeof(Thickness), typeof(HorizontalLabeledControl), new PropertyMetadata());


        /// <summary>
        /// Padding of the label
        /// </summary>
        public Thickness LabelPadding
        {
            get { return (Thickness)GetValue(LabelPaddingProperty); }
            set { SetValue(LabelPaddingProperty, value); }
        }
        public static readonly DependencyProperty LabelPaddingProperty =
            DependencyProperty.Register("LabelPadding", typeof(Thickness), typeof(HorizontalLabeledControl), new PropertyMetadata());


    }
}
