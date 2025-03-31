using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// Displays a Grid with TopRow and BottomRow separated by a GridSplitter that respects the MinHeight settings
    /// </summary>
    [TemplatePart(Name = "PART_toprow", Type = typeof(RowDefinition))]
    [TemplatePart(Name = "PART_bottomrow", Type = typeof(RowDefinition))]
    public class TwoRowGrid : Control
    {
        static TwoRowGrid()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(TwoRowGrid), new FrameworkPropertyMetadata(typeof(TwoRowGrid)));
        }

        // Template objects
        private RowDefinition toprow;
        private RowDefinition bottomrow;

        /// <summary>
        /// React to template -> update internal object handles
        /// </summary>
        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if (this.Template != null)
            {
                // Get internal objects
                toprow = this.Template.FindName("PART_toprow", this) as RowDefinition;
                bottomrow = this.Template.FindName("PART_bottomrow", this) as RowDefinition;

                // Update retrieved objects
                UpdateTopRowHeight(TopRowRelHeight);
                UpdateBottomRowHeight(BottomRowRelHeight);
            }
        }

        /// <summary>
        /// Contents of the top row
        /// </summary>
        public object TopRow
        {
            get { return (object)GetValue(TopRowProperty); }
            set { SetValue(TopRowProperty, value); }
        }
        public static readonly DependencyProperty TopRowProperty =
            DependencyProperty.Register("TopRow", typeof(object), typeof(TwoRowGrid), new UIPropertyMetadata());


        /// <summary>
        /// Contents of the bottom row
        /// </summary>
        public object BottomRow
        {
            get { return (object)GetValue(BottomRowProperty); }
            set { SetValue(BottomRowProperty, value); }
        }
        public static readonly DependencyProperty BottomRowProperty =
            DependencyProperty.Register("BottomRow", typeof(object), typeof(TwoRowGrid), new UIPropertyMetadata());


        /// <summary>
        /// MinHeight of the top row
        /// </summary>
        public double TopRowMinHeight
        {
            get { return (double)GetValue(TopRowMinHeightProperty); }
            set { SetValue(TopRowMinHeightProperty, value); }
        }
        public static readonly DependencyProperty TopRowMinHeightProperty =
            DependencyProperty.Register("TopRowMinHeight", typeof(double), typeof(TwoRowGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// MinHeight of the bottom row
        /// </summary>
        public double BottomRowMinHeight
        {
            get { return (double)GetValue(BottomRowMinHeightProperty); }
            set { SetValue(BottomRowMinHeightProperty, value); }
        }
        public static readonly DependencyProperty BottomRowMinHeightProperty =
            DependencyProperty.Register("BottomRowMinHeight", typeof(double), typeof(TwoRowGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// Desired relative height of the top row (gridlength-unit: star)
        /// </summary>
        public double TopRowRelHeight
        {
            get { return (double)GetValue(TopRowRelHeightProperty); }
            set { SetValue(TopRowRelHeightProperty, value); }
        }
        public static readonly DependencyProperty TopRowRelHeightProperty =
            DependencyProperty.Register("TopRowRelHeight", typeof(double), typeof(TwoRowGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnTopRowRelHeightPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for TopRowRelHeight-changes
        /// </summary>
        private static void OnTopRowRelHeightPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            TwoRowGrid control = source as TwoRowGrid;
            if ((control != null) && (e.NewValue != null))
            {
                control.UpdateTopRowHeight((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the Height for the top row
        /// </summary>
        private void UpdateTopRowHeight(double relheight)
        {
            if ((toprow != null) && (relheight > 0))
            {
                toprow.Height = new GridLength(relheight, GridUnitType.Star);
            }
        }


        /// <summary>
        /// Desired relative height of the bottom row (gridlength-unit: star)
        /// </summary>
        public double BottomRowRelHeight
        {
            get { return (double)GetValue(BottomRowRelHeightProperty); }
            set { SetValue(BottomRowRelHeightProperty, value); }
        }
        public static readonly DependencyProperty BottomRowRelHeightProperty =
            DependencyProperty.Register("BottomRowRelHeight", typeof(double), typeof(TwoRowGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnBottomRowRelHeightPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for BottomRowRelHeight-changes
        /// </summary>
        private static void OnBottomRowRelHeightPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            TwoRowGrid control = source as TwoRowGrid;
            if (control != null)
            {
                control.UpdateBottomRowHeight((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the Height for the bottom row
        /// </summary>
        private void UpdateBottomRowHeight(double relheight)
        {
            if ((bottomrow != null) && (relheight > 0))
            {
                bottomrow.Height = new GridLength(relheight, GridUnitType.Star);
            }
        }


        /// <summary>
        /// Height of the GridSplitter
        /// </summary>
        public double GridSplitterHeight
        {
            get { return (double)GetValue(GridSplitterHeightProperty); }
            set { SetValue(GridSplitterHeightProperty, value); }
        }
        public static readonly DependencyProperty GridSplitterHeightProperty =
            DependencyProperty.Register("GridSplitterHeight", typeof(double), typeof(TwoRowGrid),
            new FrameworkPropertyMetadata(5.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


    }
}
