using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// Displays a Grid with LeftColumn and RightColumn separated by a GridSplitter that respects the MinWidth settings.
    /// Each column can be closed/hidden (like an expander) and the other column resizes appropriately.
    /// </summary>
    [TemplatePart(Name = "PART_leftcol", Type = typeof(ColumnDefinition))]
    [TemplatePart(Name = "PART_rightcol", Type = typeof(ColumnDefinition))]
    [TemplatePart(Name = "PART_leftcontent", Type = typeof(ContentPresenter))]
    [TemplatePart(Name = "PART_splitter", Type = typeof(GridSplitter))]
    [TemplatePart(Name = "PART_rightcontent", Type = typeof(ContentPresenter))]
    public class ClosableTwoColumnGrid : Control
    {
        static ClosableTwoColumnGrid()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ClosableTwoColumnGrid), new FrameworkPropertyMetadata(typeof(ClosableTwoColumnGrid)));
        }

        // Template objects
        protected ColumnDefinition leftcol;
        protected ColumnDefinition rightcol;
        protected ContentPresenter leftcontent;
        protected ContentPresenter rightcontent;
        protected GridSplitter splitter;

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
                leftcontent = this.Template.FindName("PART_leftcontent", this) as ContentPresenter;
                rightcontent = this.Template.FindName("PART_rightcontent", this) as ContentPresenter;
                splitter = this.Template.FindName("PART_splitter", this) as GridSplitter;

                // Update retrieved objects
                SetLeftColumnVisibility(IsLeftColumnVisible);
                SetRightColumnVisibility(IsRightColumnVisible);
                UpdateLeftColumnWidth(LeftColumnRelWidth);
                UpdateRightColumnWidth(RightColumnRelWidth);
                UpdateLeftColumnMinWidth(LeftColumnMinWidth);
                UpdateRightColumnMinWidth(RightColumnMinWidth);
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
            DependencyProperty.Register("LeftColumn", typeof(object), typeof(ClosableTwoColumnGrid), new UIPropertyMetadata());


        /// <summary>
        /// Contents of the right column
        /// </summary>
        public object RightColumn
        {
            get { return (object)GetValue(RightColumnProperty); }
            set { SetValue(RightColumnProperty, value); }
        }
        public static readonly DependencyProperty RightColumnProperty =
            DependencyProperty.Register("RightColumn", typeof(object), typeof(ClosableTwoColumnGrid), new UIPropertyMetadata());


        /// <summary>
        /// MinWidth of the left column
        /// </summary>
        public double LeftColumnMinWidth
        {
            get { return (double)GetValue(LeftColumnMinWidthProperty); }
            set { SetValue(LeftColumnMinWidthProperty, value); }
        }
        public static readonly DependencyProperty LeftColumnMinWidthProperty =
            DependencyProperty.Register("LeftColumnMinWidth", typeof(double), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnLeftColumnMinWidthPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for LeftColumnMinWidth-changes
        /// </summary>
        private static void OnLeftColumnMinWidthPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoColumnGrid control = source as ClosableTwoColumnGrid;
            if ((control != null) && (e.NewValue != null))
            {
                control.UpdateLeftColumnMinWidth((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the MinWidth for the left column
        /// </summary>
        private void UpdateLeftColumnMinWidth(double minwidth)
        {
            if ((leftcol != null) && (minwidth >= 0))
            {
                if (IsLeftColumnVisible == true)
                {
                    leftcol.MinWidth = minwidth;
                }
            }
        }


        /// <summary>
        /// MinWidth of the right column
        /// </summary>
        public double RightColumnMinWidth
        {
            get { return (double)GetValue(RightColumnMinWidthProperty); }
            set { SetValue(RightColumnMinWidthProperty, value); }
        }
        public static readonly DependencyProperty RightColumnMinWidthProperty =
            DependencyProperty.Register("RightColumnMinWidth", typeof(double), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnRightColumnMinWidthPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for RightColumnMinWidth-changes
        /// </summary>
        private static void OnRightColumnMinWidthPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoColumnGrid control = source as ClosableTwoColumnGrid;
            if ((control != null) && (e.NewValue != null))
            {
                control.UpdateRightColumnMinWidth((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the MinWidth for the right column
        /// </summary>
        private void UpdateRightColumnMinWidth(double minwidth)
        {
            if ((rightcol != null) && (minwidth >= 0))
            {
                if (IsRightColumnVisible == true)
                {
                    rightcol.MinWidth = minwidth;
                }
            }
        }


        /// <summary>
        /// Desired relative width of the left column (gridlength-unit: star)
        /// </summary>
        public double LeftColumnRelWidth
        {
            get { return (double)GetValue(LeftColumnRelWidthProperty); }
            set { SetValue(LeftColumnRelWidthProperty, value); }
        }
        public static readonly DependencyProperty LeftColumnRelWidthProperty =
            DependencyProperty.Register("LeftColumnRelWidth", typeof(double), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnLeftColumnRelWidthPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for LeftColumnRelWidth-changes
        /// </summary>
        private static void OnLeftColumnRelWidthPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoColumnGrid control = source as ClosableTwoColumnGrid;
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
                if (IsLeftColumnVisible == true)
                {
                    leftcol.Width = new GridLength(relwidth, GridUnitType.Star);
                }
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
            DependencyProperty.Register("RightColumnRelWidth", typeof(double), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnRightColumnRelWidthPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for RightColumnRelWidth-changes
        /// </summary>
        private static void OnRightColumnRelWidthPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoColumnGrid control = source as ClosableTwoColumnGrid;
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
                if (IsRightColumnVisible == true)
                {
                    rightcol.Width = new GridLength(relwidth, GridUnitType.Star);
                }
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
            DependencyProperty.Register("GridSplitterWidth", typeof(double), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(5.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// true = Left Column is visible (=> Right Column may be visible or hidden), false = Left Column is hidden (=> Right Column is visible)
        /// </summary>
        public bool IsLeftColumnVisible
        {
            get { return (bool)GetValue(IsLeftColumnVisibleProperty); }
            set { SetValue(IsLeftColumnVisibleProperty, value); }
        }
        public static readonly DependencyProperty IsLeftColumnVisibleProperty =
            DependencyProperty.Register("IsLeftColumnVisible", typeof(bool), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(true, OnIsLeftColumnVisiblePropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for IsLeftColumnVisible-changes
        /// </summary>
        private static void OnIsLeftColumnVisiblePropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoColumnGrid control = source as ClosableTwoColumnGrid;
            if (control != null)
            {
                control.SetLeftColumnVisibility((bool)e.NewValue);
            }
        }

        /// <summary>
        /// Sets the visibility of the left column (and for the right column appropriately)
        /// </summary>
        private void SetLeftColumnVisibility(bool isvisible)
        {
            if ((leftcol == null) || (rightcol == null) || (leftcontent == null) || (splitter == null)) return;
            if (isvisible == true)
            {
                UpdateLeftColumnWidth(LeftColumnRelWidth);
                UpdateRightColumnWidth(RightColumnRelWidth);
                UpdateLeftColumnMinWidth(LeftColumnMinWidth);
                UpdateRightColumnMinWidth(RightColumnMinWidth);
                leftcontent.Visibility = Visibility.Visible;
                if (IsRightColumnVisible == true) splitter.Visibility = Visibility.Visible;
            }
            else
            {
                if (IsRightColumnVisible == true)
                {
                    LeftColumnRelWidth = leftcol.Width.Value;
                    RightColumnRelWidth = rightcol.Width.Value;
                }
                IsRightColumnVisible = true;
                splitter.Visibility = Visibility.Collapsed;
                leftcontent.Visibility = Visibility.Collapsed;
                leftcol.MinWidth = 0;
                leftcol.Width = new GridLength(0);
            }
        }


        /// <summary>
        /// true = Right Column is visible (=> Left Column may be visible or hidden), false = Right Column is hidden (=> Left Column is visible)
        /// </summary>
        public bool IsRightColumnVisible
        {
            get { return (bool)GetValue(IsRightColumnVisibleProperty); }
            set { SetValue(IsRightColumnVisibleProperty, value); }
        }
        public static readonly DependencyProperty IsRightColumnVisibleProperty =
            DependencyProperty.Register("IsRightColumnVisible", typeof(bool), typeof(ClosableTwoColumnGrid),
            new FrameworkPropertyMetadata(true, OnIsRightColumnVisiblePropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for IsRightColumnVisible-changes
        /// </summary>
        private static void OnIsRightColumnVisiblePropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoColumnGrid control = source as ClosableTwoColumnGrid;
            if (control != null)
            {
                control.SetRightColumnVisibility((bool)e.NewValue);
            }
        }

        /// <summary>
        /// Sets the visibility of the right column (and for the left column appropriately)
        /// </summary>
        private void SetRightColumnVisibility(bool isvisible)
        {
            if ((leftcol == null) || (rightcol == null) || (rightcontent == null) || (splitter == null)) return;
            if (isvisible == true)
            {
                UpdateLeftColumnWidth(LeftColumnRelWidth);
                UpdateRightColumnWidth(RightColumnRelWidth);
                UpdateLeftColumnMinWidth(LeftColumnMinWidth);
                UpdateRightColumnMinWidth(RightColumnMinWidth);
                rightcontent.Visibility = Visibility.Visible;
                if (IsLeftColumnVisible == true) splitter.Visibility = Visibility.Visible;
            }
            else
            {
                if (IsLeftColumnVisible == true)
                {
                    LeftColumnRelWidth = leftcol.Width.Value;
                    RightColumnRelWidth = rightcol.Width.Value;
                }
                IsLeftColumnVisible = true;
                splitter.Visibility = Visibility.Collapsed;
                rightcontent.Visibility = Visibility.Collapsed;
                rightcol.MinWidth = 0;
                rightcol.Width = new GridLength(0);
            }
        }
    }
}
