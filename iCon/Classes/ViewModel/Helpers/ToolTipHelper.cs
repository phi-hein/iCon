using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Global attached property provider for switching tooltips between normal state and error state
    /// </summary>
    public static class ToolTipHelper
    {
        /// <summary>
        /// Holds the default Tooltip value. OnMyToolTipChanged used to set ToolTip
        /// </summary>
        public static DependencyProperty MyToolTipProperty = DependencyProperty.RegisterAttached("MyToolTip", typeof(object), 
            typeof(ToolTipHelper), new UIPropertyMetadata(null, OnMyToolTipChanged));

        /// <summary>
        /// Gets the value of the MyToolTip property.
        /// </summary>
        /// <param name="dependencyObject">The DependencyObject to return the MyToolTip property value from.</param>
        /// <returns>The value of the MyToolTip property.</returns>
        public static object GetMyToolTip(DependencyObject dependencyObject)
        {
            return dependencyObject.GetValue(MyToolTipProperty);
        }

        /// <summary>
        /// Sets the value of the MyToolTip property.
        /// </summary>
        /// <param name="dependencyObject">The DependencyObject to set the MyToolTip property value of</param>
        /// <param name="value">The value to be assigned to the MyToolTip property.</param>
        public static void SetMyToolTip(DependencyObject dependencyObject, object value)
        {
            dependencyObject.SetValue(MyToolTipProperty, value);
        }

        /// <summary>
        /// Initially blank, set by style when an error occures (or goes away). Uses OnErrorToolTipChanged to update ToolTip.
        /// </summary>
        public static DependencyProperty ErrorToolTipProperty = DependencyProperty.RegisterAttached("ErrorToolTip", typeof(object), 
            typeof(ToolTipHelper), new UIPropertyMetadata(null, OnErrorToolTipChanged));

        /// <summary>
        /// Gets the value of the ErrorToolTip property.
        /// </summary>
        /// <param name="dependencyObject">The DependencyObject to return the ErrorToolTip property value from</param>
        /// <returns>The value of the ErrorToolTip property.</returns>
        public static object GetErrorToolTip(DependencyObject dependencyObject)
        {
            return dependencyObject.GetValue(ErrorToolTipProperty);
        }

        /// <summary>
        /// Sets the value of the ErrorToolTip property.
        /// </summary>
        /// <param name="dependencyObject">The DependencyObject to set the  ErrorToolTip property value of</param>
        /// <param name="value">The value to be assigned to the ErrorToolTip property.</param>
        public static void SetErrorToolTip(DependencyObject dependencyObject, object value)
        {
            dependencyObject.SetValue(ErrorToolTipProperty, value);
        }

        /// <summary>
        /// If an Error Tooltip is supplied, sets the ToolTip to that value, otherwise, resets it back to MyToolTipProperty
        /// </summary>
        /// <param name="dependencyObject">The control with the tooltip</param>
        /// <param name="e">The DependencyPropertyChangedEventArgs object containing event specific information.</param>
        public static void OnErrorToolTipChanged(DependencyObject dependencyObject, DependencyPropertyChangedEventArgs e)
        {
            if (dependencyObject is FrameworkElement)
            {
                var felem = dependencyObject as FrameworkElement;
                if (e.NewValue == null)
                {
                    // No error tooltip, reset to the non-error value
                    felem.ToolTip = dependencyObject.GetValue(MyToolTipProperty);
                }
                else
                {
                    // Use the error tooltip
                    felem.ToolTip = e.NewValue;
                }
            }
        }

        /// <summary>
        /// This should only be called when the value is first assigned to the control.
        /// </summary>
        /// <param name="dependencyObject">The Control</param>
        /// <param name="e">The DependencyPropertyChangedEventArgs object containing event 
        /// specific information.</param>
        public static void OnMyToolTipChanged(DependencyObject dependencyObject, DependencyPropertyChangedEventArgs e)
        {
            if (dependencyObject is FrameworkElement)
            {
                var felem = dependencyObject as FrameworkElement;
                if (e.OldValue != e.NewValue)
                {
                    felem.ToolTip = e.NewValue;
                }
            }
        }
    }
}
