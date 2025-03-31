using System.Windows;

namespace iCon_General
{
    /// <summary>
    /// Binding proxy class that can be used as a application-wide resource for special binding problems (e.g. DataGrid header binding)
    /// </summary>
    public class BindingProxy: Freezable
    {
        protected override Freezable CreateInstanceCore()
        {
            return new BindingProxy();
        }

        public object Data
        {
            get { return (object)GetValue(DataProperty); }
            set { SetValue(DataProperty, value); }
        }

        public static readonly DependencyProperty DataProperty =
            DependencyProperty.Register("Data", typeof(object), typeof(BindingProxy), new UIPropertyMetadata(null));
    }
}
