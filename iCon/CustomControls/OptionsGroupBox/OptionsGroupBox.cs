using System.Windows;
using System.Windows.Controls;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// Custom group box with a label and indented content
    /// </summary>
    public class OptionsGroupBox : ContentControl
    {
        static OptionsGroupBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(OptionsGroupBox), new FrameworkPropertyMetadata(typeof(OptionsGroupBox)));
        }


        /// <summary>
        /// CornerRadius of the group box
        /// </summary>
        public CornerRadius CornerRadius
        {
            get { return (CornerRadius)GetValue(CornerRadiusProperty); }
            set { SetValue(CornerRadiusProperty, value); }
        }
        public static readonly DependencyProperty CornerRadiusProperty =
            DependencyProperty.Register("CornerRadius", typeof(CornerRadius), typeof(OptionsGroupBox), new PropertyMetadata());


        /// <summary>
        /// Text of the top label
        /// </summary>
        public string LabelText
        {
            get { return (string)GetValue(LabelTextProperty); }
            set { SetValue(LabelTextProperty, value); }
        }
        public static readonly DependencyProperty LabelTextProperty =
            DependencyProperty.Register("LabelText", typeof(string), typeof(OptionsGroupBox), new PropertyMetadata(""));


        /// <summary>
        /// FontWeight of the top label
        /// </summary>
        public FontWeight LabelFontWeight
        {
            get { return (FontWeight)GetValue(LabelFontWeightProperty); }
            set { SetValue(LabelFontWeightProperty, value); }
        }
        public static readonly DependencyProperty LabelFontWeightProperty =
            DependencyProperty.Register("LabelFontWeight", typeof(FontWeight), typeof(OptionsGroupBox), new PropertyMetadata());


        /// <summary>
        /// Margin of the inner group
        /// </summary>
        public Thickness InnerMargin
        {
            get { return (Thickness)GetValue(InnerMarginProperty); }
            set { SetValue(InnerMarginProperty, value); }
        }
        public static readonly DependencyProperty InnerMarginProperty =
            DependencyProperty.Register("InnerMargin", typeof(Thickness), typeof(OptionsGroupBox), new PropertyMetadata());


        /// <summary>
        /// Margin of the top label
        /// </summary>
        public Thickness LabelMargin
        {
            get { return (Thickness)GetValue(LabelMarginProperty); }
            set { SetValue(LabelMarginProperty, value); }
        }
        public static readonly DependencyProperty LabelMarginProperty =
            DependencyProperty.Register("LabelMargin", typeof(Thickness), typeof(OptionsGroupBox), new PropertyMetadata());


        /// <summary>
        /// Padding of the top label
        /// </summary>
        public Thickness LabelPadding
        {
            get { return (Thickness)GetValue(LabelPaddingProperty); }
            set { SetValue(LabelPaddingProperty, value); }
        }
        public static readonly DependencyProperty LabelPaddingProperty =
            DependencyProperty.Register("LabelPadding", typeof(Thickness), typeof(OptionsGroupBox), new PropertyMetadata());

        
        
    }
}
