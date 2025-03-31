using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// Displays a Grid with LeftColumn and RightColumn separated by a GridSplitter that respects the MinWidth settings
    /// </summary>
    [TemplatePart(Name = "PART_leftcol", Type = typeof(ColumnDefinition))]
    [TemplatePart(Name = "PART_rightcol", Type = typeof(ColumnDefinition))]
    public class TwoColumnGrid : Control
    {
        static TwoColumnGrid()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(TwoColumnGrid), new FrameworkPropertyMetadata(typeof(TwoColumnGrid)));
        }

        // Template objects
        private ColumnDefinition leftcol;
        private ColumnDefinition rightcol;

        /// <summary>
        /// React to template -> update internal object handles
        /// </summary>
        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if (this.Template != null)
            {
                // Get internal objects
                leftcol = this.Template.FindName("PART_leftcol", this) as ColumnDefinition;
                rightcol = this.Template.FindName("PART_rightcol", this) as ColumnDefinition;

                // Update retrieved objects
                UpdateLeftColumnWidth(LeftColumnRelWidth);
                UpdateRightColumnWidth(RightColumnRelWidth);
            }
        }

        /// <summary>
        /// Contents of the left column
        /// </summary>
        public object LeftColumn
        {
            get { return (object)GetValue(LeftColumnProperty); }
            set { SetValue(LeftColumnProperty, value); }
        }
        public static readonly DependencyProperty LeftColumnProperty =
            DependencyProperty.Register("LeftColumn", typeof(object), typeof(TwoColumnGrid), new UIPropertyMetadata());


        /// <summary>
        /// Contents of the right column
        /// </summary>
        public object RightColumn
        {
            get { return (object)GetValue(RightColumnProperty); }
            set { SetValue(RightColumnProperty, value); }
        }
        public static readonly DependencyProperty RightColumnProperty =
            DependencyProperty.Register("RightColumn", typeof(object), typeof(TwoColumnGrid), new UIPropertyMetadata());


        /// <summary>
        /// MinWidth of the left column
        /// </summary>
        public double LeftColumnMinWidth
        {
            get { return (double)GetValue(LeftColumnMinWidthProperty); }
            set { SetValue(LeftColumnMinWidthProperty, value); }
        }
        public static readonly DependencyProperty LeftColumnMinWidthProperty =
            DependencyProperty.Register("LeftColumnMinWidth", typeof(double), typeof(TwoColumnGrid),
            new FrameworkPropertyMetadata(0.0,FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// MinWidth of the right column
        /// </summary>
        public double RightColumnMinWidth
        {
            get { return (double)GetValue(RightColumnMinWidthProperty); }
            set { SetValue(RightColumnMinWidthProperty, value); }
        }
        public static readonly DependencyProperty RightColumnMinWidthProperty =
            DependencyProperty.Register("RightColumnMinWidth", typeof(double), typeof(TwoColumnGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// Desired relative width of the left column (gridlength-unit: star)
        /// </summary>
        public double LeftColumnRelWidth
        {
            get { return (double)GetValue(LeftColumnRelWidthProperty); }
            set { SetValue(LeftColumnRelWidthProperty, value); }
        }
        public static readonly DependencyProperty LeftColumnRelWidthProperty =
            DependencyProperty.Register("LeftColumnRelWidth", typeof(double), typeof(TwoColumnGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnLeftColumnRelWidthPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for LeftColumnRelWidth-changes
        /// </summary>
        private static void OnLeftColumnRelWidthPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            TwoColumnGrid control = source as TwoColumnGrid;
            if ((control != null) && (e.NewValue != null))
            {
                control.UpdateLeftColumnWidth((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the Width for the left column
        /// </summary>
        private void UpdateLeftColumnWidth(double relwidth)
        {
            if ((leftcol != null) && (relwidth > 0))
            {
                leftcol.Width = new GridLength(relwidth, GridUnitType.Star);
            }
        }


        /// <summary>
        /// Desired relative width of the right column (gridlength-unit: star)
        /// </summary>
        public double RightColumnRelWidth
        {
            get { return (double)GetValue(RightColumnRelWidthProperty); }
            set { SetValue(RightColumnRelWidthProperty, value); }
        }
        public static readonly DependencyProperty RightColumnRelWidthProperty =
            DependencyProperty.Register("RightColumnRelWidth", typeof(double), typeof(TwoColumnGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnRightColumnRelWidthPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for RightColumnRelWidth-changes
        /// </summary>
        private static void OnRightColumnRelWidthPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            TwoColumnGrid control = source as TwoColumnGrid;
            if (control != null)
            {
                control.UpdateRightColumnWidth((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the Width for the right column
        /// </summary>
        private void UpdateRightColumnWidth(double relwidth)
        {
            if ((rightcol != null) && (relwidth > 0))
            {
                rightcol.Width = new GridLength(relwidth, GridUnitType.Star);
            }
        }


        /// <summary>
        /// Width of the GridSplitter
        /// </summary>
        public double GridSplitterWidth
        {
            get { return (double)GetValue(GridSplitterWidthProperty); }
            set { SetValue(GridSplitterWidthProperty, value); }
        }
        public static readonly DependencyProperty GridSplitterWidthProperty =
            DependencyProperty.Register("GridSplitterWidth", typeof(double), typeof(TwoColumnGrid),
            new FrameworkPropertyMetadata(5.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        
    }
}
