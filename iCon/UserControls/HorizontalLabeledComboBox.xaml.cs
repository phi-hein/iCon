using System;
using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace iCon_General.UserControls
{
    /// <summary>
    /// Interaktionslogik für HorizontalLabeledComboBox.xaml
    /// </summary>
    public partial class HorizontalLabeledComboBox : UserControl
    {
        public HorizontalLabeledComboBox()
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
            DependencyProperty.Register("LabelText", typeof(string), typeof(HorizontalLabeledComboBox),
            new FrameworkPropertyMetadata("") { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        
        /// <summary>
        /// Path to the displayed member of the combobox items (Dependency Property)
        /// </summary>
        public string ComboBoxDisplayMemberPath
        {
            get { return (string)GetValue(ComboBoxDisplayMemberPathProperty); }
            set { SetValue(ComboBoxDisplayMemberPathProperty, value); }
        }
        public static readonly DependencyProperty ComboBoxDisplayMemberPathProperty =
            DependencyProperty.Register("ComboBoxDisplayMemberPath", typeof(string), typeof(HorizontalLabeledComboBox),
            new FrameworkPropertyMetadata("") { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// ItemsSource of the combobox (Dependency Property)
        /// </summary>
        public IEnumerable ComboBoxItemsSource
        {
            get { return (IEnumerable)GetValue(ComboBoxItemsSourceProperty); }
            set { SetValue(ComboBoxItemsSourceProperty, value); }
        }
        public static readonly DependencyProperty ComboBoxItemsSourceProperty =
            DependencyProperty.Register("ComboBoxItemsSource", typeof(IEnumerable), typeof(HorizontalLabeledComboBox),
            new FrameworkPropertyMetadata(null) { BindsTwoWayByDefault = false, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// SelectedItem of the combobox (Dependency Property)
        /// </summary>
        public Object ComboBoxSelectedItem
        {
            get { return (Object)GetValue(ComboBoxSelectedItemProperty); }
            set { SetValue(ComboBoxSelectedItemProperty, value); }
        }
        public static readonly DependencyProperty ComboBoxSelectedItemProperty =
            DependencyProperty.Register("ComboBoxSelectedItem", typeof(Object), typeof(HorizontalLabeledComboBox),
            new FrameworkPropertyMetadata(null) { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        
    }
}
