using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace iCon_General.CustomControls
{
    /// <summary>
    /// Displays a Grid with TopRow and BottomRow separated by a GridSplitter that respects the MinHeight settings.
    /// Each row can be closed/hidden (like an expander) and the other row resizes appropriately.
    /// </summary>
    [TemplatePart(Name = "PART_toprow", Type = typeof(RowDefinition))]
    [TemplatePart(Name = "PART_bottomrow", Type = typeof(RowDefinition))]
    [TemplatePart(Name = "PART_topcontent", Type = typeof(ContentPresenter))]
    [TemplatePart(Name = "PART_splitter", Type = typeof(GridSplitter))]
    [TemplatePart(Name = "PART_bottomcontent", Type = typeof(ContentPresenter))]
    public class ClosableTwoRowGrid : Control
    {
        static ClosableTwoRowGrid()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ClosableTwoRowGrid), new FrameworkPropertyMetadata(typeof(ClosableTwoRowGrid)));
        }

        // Template objects
        protected RowDefinition toprow;
        protected RowDefinition bottomrow;
        protected ContentPresenter topcontent;
        protected ContentPresenter bottomcontent;
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
                toprow = this.Template.FindName("PART_toprow", this) as RowDefinition;
                bottomrow = this.Template.FindName("PART_bottomrow", this) as RowDefinition;
                topcontent = this.Template.FindName("PART_topcontent", this) as ContentPresenter;
                bottomcontent = this.Template.FindName("PART_bottomcontent", this) as ContentPresenter;
                splitter = this.Template.FindName("PART_splitter", this) as GridSplitter;

                // Update retrieved objects
                SetTopRowVisibility(IsTopRowVisible);
                SetBottomRowVisibility(IsBottomRowVisible);
                UpdateTopRowHeight(TopRowRelHeight);
                UpdateBottomRowHeight(BottomRowRelHeight);
                UpdateTopRowMinHeight(TopRowMinHeight);
                UpdateBottomRowMinHeight(BottomRowMinHeight);
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
            DependencyProperty.Register("TopRow", typeof(object), typeof(ClosableTwoRowGrid), new UIPropertyMetadata());


        /// <summary>
        /// Contents of the bottom row
        /// </summary>
        public object BottomRow
        {
            get { return (object)GetValue(BottomRowProperty); }
            set { SetValue(BottomRowProperty, value); }
        }
        public static readonly DependencyProperty BottomRowProperty =
            DependencyProperty.Register("BottomRow", typeof(object), typeof(ClosableTwoRowGrid), new UIPropertyMetadata());


        /// <summary>
        /// MinHeight of the top row
        /// </summary>
        public double TopRowMinHeight
        {
            get { return (double)GetValue(TopRowMinHeightProperty); }
            set { SetValue(TopRowMinHeightProperty, value); }
        }
        public static readonly DependencyProperty TopRowMinHeightProperty =
            DependencyProperty.Register("TopRowMinHeight", typeof(double), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnTopRowMinHeightPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for TopRowMinHeight-changes
        /// </summary>
        private static void OnTopRowMinHeightPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoRowGrid control = source as ClosableTwoRowGrid;
            if ((control != null) && (e.NewValue != null))
            {
                control.UpdateTopRowMinHeight((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the MinHeight for the top row
        /// </summary>
        private void UpdateTopRowMinHeight(double minheight)
        {
            if ((toprow != null) && (minheight >= 0))
            {
                if (IsTopRowVisible == true)
                {
                    toprow.MinHeight = minheight;
                }
            }
        }


        /// <summary>
        /// MinHeight of the bottom row
        /// </summary>
        public double BottomRowMinHeight
        {
            get { return (double)GetValue(BottomRowMinHeightProperty); }
            set { SetValue(BottomRowMinHeightProperty, value); }
        }
        public static readonly DependencyProperty BottomRowMinHeightProperty =
            DependencyProperty.Register("BottomRowMinHeight", typeof(double), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnBottomRowMinHeightPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for BottomRowMinHeight-changes
        /// </summary>
        private static void OnBottomRowMinHeightPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoRowGrid control = source as ClosableTwoRowGrid;
            if ((control != null) && (e.NewValue != null))
            {
                control.UpdateBottomRowMinHeight((double)e.NewValue);
            }
        }

        /// <summary>
        /// Updates the MinHeight for the bottom row
        /// </summary>
        private void UpdateBottomRowMinHeight(double minheight)
        {
            if ((bottomrow != null) && (minheight >= 0))
            {
                if (IsBottomRowVisible == true)
                {
                    bottomrow.MinHeight = minheight;
                }
            }
        }


        /// <summary>
        /// Desired relative height of the top row (gridlength-unit: star)
        /// </summary>
        public double TopRowRelHeight
        {
            get { return (double)GetValue(TopRowRelHeightProperty); }
            set { SetValue(TopRowRelHeightProperty, value); }
        }
        public static readonly DependencyProperty TopRowRelHeightProperty =
            DependencyProperty.Register("TopRowRelHeight", typeof(double), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnTopRowRelHeightPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for TopRowRelHeight-changes
        /// </summary>
        private static void OnTopRowRelHeightPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoRowGrid control = source as ClosableTwoRowGrid;
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
                if (IsTopRowVisible == true)
                {
                    toprow.Height = new GridLength(relheight, GridUnitType.Star);
                }
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
            DependencyProperty.Register("BottomRowRelHeight", typeof(double), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.AffectsMeasure, OnBottomRowRelHeightPropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for BottomRowRelHeight-changes
        /// </summary>
        private static void OnBottomRowRelHeightPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoRowGrid control = source as ClosableTwoRowGrid;
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
                if (IsBottomRowVisible == true)
                {
                    bottomrow.Height = new GridLength(relheight, GridUnitType.Star);
                }
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
            DependencyProperty.Register("GridSplitterHeight", typeof(double), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(5.0, FrameworkPropertyMetadataOptions.AffectsMeasure) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });


        /// <summary>
        /// true = Top Row is visible (=> Bottom Row may be visible or hidden), false = Top Row is hidden (=> Bottom Row is visible)
        /// </summary>
        public bool IsTopRowVisible
        {
            get { return (bool)GetValue(IsTopRowVisibleProperty); }
            set { SetValue(IsTopRowVisibleProperty, value); }
        }
        public static readonly DependencyProperty IsTopRowVisibleProperty =
            DependencyProperty.Register("IsTopRowVisible", typeof(bool), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(true, OnIsTopRowVisiblePropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for IsTopRowVisible-changes
        /// </summary>
        private static void OnIsTopRowVisiblePropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoRowGrid control = source as ClosableTwoRowGrid;
            if (control != null)
            {
                control.SetTopRowVisibility((bool)e.NewValue);
            }
        }

        /// <summary>
        /// Sets the visibility of the top row (and for the bottom row appropriately)
        /// </summary>
        private void SetTopRowVisibility(bool isvisible)
        {
            if ((toprow == null) || (bottomrow == null) || (topcontent == null) || (splitter == null)) return;
            if (isvisible == true)
            {
                UpdateTopRowHeight(TopRowRelHeight);
                UpdateBottomRowHeight(BottomRowRelHeight);
                UpdateTopRowMinHeight(TopRowMinHeight);
                UpdateBottomRowMinHeight(BottomRowMinHeight);
                topcontent.Visibility = Visibility.Visible;
                if (IsBottomRowVisible == true) splitter.Visibility = Visibility.Visible;
            }
            else
            {
                if (IsBottomRowVisible == true)
                {
                    TopRowRelHeight = toprow.Height.Value;
                    BottomRowRelHeight = bottomrow.Height.Value;
                }
                IsBottomRowVisible = true;
                splitter.Visibility = Visibility.Collapsed;
                topcontent.Visibility = Visibility.Collapsed;
                toprow.MinHeight = 0;
                toprow.Height = new GridLength(0);
            }
        }


        /// <summary>
        /// true = Bottom Row is visible (=> Top Row may be visible or hidden), false = Bottom Row is hidden (=> Top Row is visible)
        /// </summary>
        public bool IsBottomRowVisible
        {
            get { return (bool)GetValue(IsBottomRowVisibleProperty); }
            set { SetValue(IsBottomRowVisibleProperty, value); }
        }
        public static readonly DependencyProperty IsBottomRowVisibleProperty =
            DependencyProperty.Register("IsBottomRowVisible", typeof(bool), typeof(ClosableTwoRowGrid),
            new FrameworkPropertyMetadata(true, OnIsBottomRowVisiblePropertyChanged) 
            { BindsTwoWayByDefault = true, DefaultUpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });

        /// <summary>
        /// Handler for IsBottomRowVisible-changes
        /// </summary>
        private static void OnIsBottomRowVisiblePropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
        {
            ClosableTwoRowGrid control = source as ClosableTwoRowGrid;
            if (control != null)
            {
                control.SetBottomRowVisibility((bool)e.NewValue);
            }
        }

        /// <summary>
        /// Sets the visibility of the bottom row (and for the top row appropriately)
        /// </summary>
        private void SetBottomRowVisibility(bool isvisible)
        {
            if ((toprow == null) || (bottomrow == null) || (bottomcontent == null) || (splitter == null)) return;
            if (isvisible == true)
            {
                UpdateTopRowHeight(TopRowRelHeight);
                UpdateBottomRowHeight(BottomRowRelHeight);
                UpdateTopRowMinHeight(TopRowMinHeight);
                UpdateBottomRowMinHeight(BottomRowMinHeight);
                bottomcontent.Visibility = Visibility.Visible;
                if (IsTopRowVisible == true) splitter.Visibility = Visibility.Visible;
            }
            else
            {
                if (IsTopRowVisible == true)
                {
                    TopRowRelHeight = toprow.Height.Value;
                    BottomRowRelHeight = bottomrow.Height.Value;
                }
                IsTopRowVisible = true;
                splitter.Visibility = Visibility.Collapsed;
                bottomcontent.Visibility = Visibility.Collapsed;
                bottomrow.MinHeight = 0;
                bottomrow.Height = new GridLength(0);
            }
        }
    }
}
