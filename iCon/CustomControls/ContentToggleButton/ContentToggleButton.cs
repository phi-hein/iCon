using System.Windows;
using System.Windows.Controls.Primitives;
using System.Windows.Data;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// ToggleButton with more control over the appearance and an additional state flag
    /// </summary>
    public class ContentToggleButton : ToggleButton
    {
        static ContentToggleButton()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ContentToggleButton), new FrameworkPropertyMetadata(typeof(ContentToggleButton)));
        }

        /// <summary>
        /// CornerRadius of the ToggleButton
        /// </summary>
        public CornerRadius CornerRadius
        {
            get { return (CornerRadius)GetValue(CornerRadiusProperty); }
            set { SetValue(CornerRadiusProperty, value); }
        }
        public static readonly DependencyProperty CornerRadiusProperty =
            DependencyProperty.Register("CornerRadius", typeof(CornerRadius), typeof(ContentToggleButton), new PropertyMetadata());


        /// <summary>
        /// Flag for additional triggering (e.g. when a Tab is selected or not)
        /// </summary>
        public bool IsActive
        {
            get { return (bool)GetValue(IsActiveProperty); }
            set { SetValue(IsActiveProperty, value); }
        }
        public static readonly DependencyProperty IsActiveProperty =
            DependencyProperty.Register("IsActive", typeof(bool), typeof(ContentToggleButton),
            new FrameworkPropertyMetadata(true) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        
    }
}
