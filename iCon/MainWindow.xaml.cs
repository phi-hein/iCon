using System;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Markup;
using System.Xml;
using iCon_General.CustomControls;
using iCon_General.Properties;

namespace iCon_General
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Internal Variables

        /// <summary> 
        /// Custom redirected console (from MCMisc_UDLL) 
        /// </summary>  
        private readonly TConsoleWrapper ConsoleWrapper = null;

        /// <summary> 
        /// ViewModel for MC data 
        /// </summary>
        private readonly TMCViewModel MCViewModel = null;

        #endregion Internal Variables

        #region Public Variables

        /// <summary>
        /// Flag which shows that the application is terminating because of an unhandled critical exception
        /// </summary>
        public bool HasCritException = false;

        #endregion Public Variables

        #region Constructor, Loaded, Closing, Destructor

        /// <summary> 
        /// Constructor 
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            // Initialize variables
            HasCritException = false;

            // Check if all required non-GUI third party dlls are there (i.e. those that would not fail on startup)
            if (File.Exists(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "Renci.SshNet.dll")) == false)
            {
                throw new ApplicationException("SSH.Net-Assembly is missing (MainWindow Constructor)");
            }

            // Check if all required dlls are there
            if (File.Exists(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "MCJob_MDLL.dll")) == false)
            {
                throw new ApplicationException("MCJob-Assembly is missing (MainWindow Constructor)");
            }
            if (File.Exists(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "MCJob_UDLL.dll")) == false)
            {
                throw new ApplicationException("MCJob-DLL is missing (MainWindow Constructor)");
            }
            if (File.Exists(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "MCMisc_MDLL.dll")) == false)
            {
                throw new ApplicationException("MCMisc-Assembly is missing (MainWindow Constructor)");
            }
            if (File.Exists(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "MCMisc_UDLL.dll")) == false)
            {
                throw new ApplicationException("MCMisc-DLL is missing (MainWindow Constructor)");
            }

            // Create and specify console
            ConsoleWrapper = new TConsoleWrapper();
            int ErrorCode = ConsoleWrapper.SetTitle("Status Window");
            if (ErrorCode != Constants.KMCERR_OK)
                throw new ApplicationException("Unable to set status window title (MainWindow Constructor, Code = " + ErrorCode.ToString() + ")");
            ErrorCode = ConsoleWrapper.SetBufferSize(10000, 400);
            if (ErrorCode != Constants.KMCERR_OK)
                throw new ApplicationException("Unable to set status window buffer (MainWindow Constructor, Code = " + ErrorCode.ToString() + ")");

            // Create viewmodel
            MCViewModel = new TMCViewModel();
        }
        
        /// <summary> 
        /// Loaded event handler (Initializations) 
        /// </summary>
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {

            // Bind ViewModel to GUI elements
            var oldProxy = (BindingProxy)FindResource("locBindingProxy");
            BindingProxy newProxy = (BindingProxy)oldProxy.Clone();
            newProxy.Data = MCViewModel;
            System.Windows.Application.Current.Resources["locBindingProxy"] = newProxy;
            MainWnd.DataContext = MCViewModel;

            // Listen to viewmodel property changes
            MCViewModel.PropertyChanged += ViewModel_PropertyChanged;

            // Sync WWAtomColumns
            UpdateWWAtomColumns();

            // Load ini-File or create new it if does not exist
            LoadFromIniFile();

            // Load ini-MCJob-File or create new if it does not exist
            MCViewModel.Load(if_from_inifile: true);
            SwitchConsole(false);

            // Initialize further viewmodel data
            v3dCrystal.OrthographicToggleGesture = new MouseGesture();          // disable gesture -> no switching to perspective camera
            v3dCrystal.DefaultCamera = MCViewModel.CrystalModel.DefaultCamera;  // necessary because bindings are loaded to late
            v3dCrystal.ResetCamera();
            v3dJumps.OrthographicToggleGesture = new MouseGesture();            // disable gesture -> no switching to perspective camera
            v3dJumps.DefaultCamera = MCViewModel.JumpsModel.DefaultCamera;      // neccessary because bindings are loaded to late
            v3dJumps.ResetCamera();
            v3dUniqueJumps.OrthographicToggleGesture = new MouseGesture();                  // disable gesture -> no switching to perspective camera
            v3dUniqueJumps.DefaultCamera = MCViewModel.UniqueJumpsModel.DefaultCamera;      // neccessary because bindings are loaded to late
            v3dUniqueJumps.ResetCamera();
            v3dCodes.OrthographicToggleGesture = new MouseGesture();                    // disable gesture -> no switching to perspective camera
            v3dCodes.DefaultCamera = MCViewModel.CodesModel.DefaultCamera;              // neccessary because bindings are loaded to late
            v3dCodes.ResetCamera();
            v3dInteractions.OrthographicToggleGesture = new MouseGesture();                    // disable gesture -> no switching to perspective camera
            v3dInteractions.DefaultCamera = MCViewModel.InteractionsModel.DefaultCamera;       // neccessary because bindings are loaded to late
            v3dInteractions.ResetCamera();

            // Disable zoom to extents gestures of the HelixViewport3Ds
            // Explanation:
            // In CameraHelper.ZoomExtents there is no change of the camera-model distance.
            // This causes a problem when zooming into the model till near plane clipping
            // and then calling ZoomExtents with an orthographic camera. Because of the 
            // camera-model distance remaining the same, the model remains partially clipped. 
            // Therefore the gesture for normal zooming to extents in the HelixViewport3D 
            // (Strg+Shift+E) should always be disabled.
            v3dCrystal.ZoomExtentsGesture = new MouseGesture();
            v3dJumps.ZoomExtentsGesture = new MouseGesture();
            v3dUniqueJumps.ZoomExtentsGesture = new MouseGesture();
            v3dCodes.ZoomExtentsGesture = new MouseGesture();
            v3dInteractions.ZoomExtentsGesture = new MouseGesture();
        }

        /// <summary> 
        /// Closing event handler (Final tasks) 
        /// </summary>
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            // Close owned windows
            if (OwnedWindows.Count != 0)
            {
                for (int i = 0; i < OwnedWindows.Count; i++)
                {
                    OwnedWindows[i].Close();
                }
            }

            // Save ini data only if no critical exception occured
            if (HasCritException == false)
            {
                // Update binding if textbox or datagrid has focus
                UpdateCurrentInputBox();

                // Save KMC data to ini-KMCJob-File
                MCViewModel.Save(if_to_inifile: true);

                // Save ini-File
                SaveToIniFile();
            }
        }

        /// <summary> 
        /// Destructor 
        /// </summary>
        ~MainWindow()
        {

        }

#endregion Constructor, Loaded, Closing, Destructor

#region Critical Methods for App Behaviour

        /// <summary>
        /// Tab closing mechanics: menu item is clicked
        /// </summary>
        private void menuView_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Controls.MenuItem t_menuitem = e.Source as System.Windows.Controls.MenuItem;
            if (t_menuitem == null) return;
            switch (t_menuitem.Name)
            {
                case "menuView3D":
                    if (t_menuitem.IsChecked == true)
                    {
                        tab3D.Visibility = Visibility.Visible;
                        tab3D.IsSelected = true;
                        gridMain.IsRightColumnVisible = true;
                        if (tabProjFile.Visibility == Visibility.Visible)
                        {
                            menuViewProjFile.IsChecked = true;
                        }
                        if (tabExtSettings.Visibility == Visibility.Visible)
                        {
                            menuViewExtSettings.IsChecked = true;
                        }
                    }
                    else
                    {
                        tab3D.Visibility = Visibility.Collapsed;
                        if ((tabProjFile.Visibility == Visibility.Collapsed) &&
                            (tabExtSettings.Visibility == Visibility.Collapsed))
                        {
                            gridMain.IsRightColumnVisible = false;
                        }
                        else
                        {
                            if ((tabProjFile.IsSelected == false) && (tabExtSettings.IsSelected == false))
                            {
                                if (tabProjFile.Visibility == Visibility.Visible)
                                {
                                    tabProjFile.IsSelected = true;
                                }
                                else
                                {
                                    tabExtSettings.IsSelected = true;
                                }
                            }
                        }
                    }
                    break;
                case "menuViewProjFile":
                    if (t_menuitem.IsChecked == true)
                    {
                        tabProjFile.Visibility = Visibility.Visible;
                        tabProjFile.IsSelected = true;
                        gridMain.IsRightColumnVisible = true;
                        if (tab3D.Visibility == Visibility.Visible)
                        {
                            menuView3D.IsChecked = true;
                        }
                        if (tabExtSettings.Visibility == Visibility.Visible)
                        {
                            menuViewExtSettings.IsChecked = true;
                        }
                    }
                    else
                    {
                        tabProjFile.Visibility = Visibility.Collapsed;
                        if ((tab3D.Visibility == Visibility.Collapsed) &&
                            (tabExtSettings.Visibility == Visibility.Collapsed))
                        {
                            gridMain.IsRightColumnVisible = false;
                        }
                        else
                        {
                            if ((tab3D.IsSelected == false) && (tabExtSettings.IsSelected == false))
                            {
                                if (tab3D.Visibility == Visibility.Visible)
                                {
                                    tab3D.IsSelected = true;
                                }
                                else
                                {
                                    tabExtSettings.IsSelected = true;
                                }
                            }
                        }
                    }
                    break;
                case "menuViewExtSettings":
                    if (t_menuitem.IsChecked == true)
                    {
                        tabExtSettings.Visibility = Visibility.Visible;
                        tabExtSettings.IsSelected = true;
                        gridMain.IsRightColumnVisible = true;
                        if (tab3D.Visibility == Visibility.Visible)
                        {
                            menuView3D.IsChecked = true;
                        }
                        if (tabProjFile.Visibility == Visibility.Visible)
                        {
                            menuViewProjFile.IsChecked = true;
                        }
                    }
                    else
                    {
                        tabExtSettings.Visibility = Visibility.Collapsed;
                        if ((tab3D.Visibility == Visibility.Collapsed) &&
                            (tabProjFile.Visibility == Visibility.Collapsed))
                        {
                            gridMain.IsRightColumnVisible = false;
                        }
                        else
                        {
                            if ((tab3D.IsSelected == false) && (tabProjFile.IsSelected == false))
                            {
                                if (tab3D.Visibility == Visibility.Visible)
                                {
                                    tab3D.IsSelected = true;
                                }
                                else
                                {
                                    tabProjFile.IsSelected = true;
                                }
                            }
                        }
                    }
                    break;
                case "menuViewConsole":
                    SwitchConsole(menuViewConsole.IsChecked);
                    break;
            }
        }

        /// <summary>
        /// Tab closing mechanics: tab item close button is clicked
        /// </summary>
        private void btnTabClose_Click(object sender, RoutedEventArgs e)
        {
            ContentToggleButton t_btn = e.Source as ContentToggleButton;
            if (t_btn == null) return;
            switch (t_btn.Name)
            {
                case "btnCloseTab3D":
                    tab3D.Visibility = Visibility.Collapsed;
                    menuView3D.IsChecked = false;
                    if ((tabProjFile.Visibility == Visibility.Collapsed) &&
                        (tabExtSettings.Visibility == Visibility.Collapsed))
                    {
                        gridMain.IsRightColumnVisible = false;
                    }
                    else
                    {
                        if ((tabProjFile.IsSelected == false) && (tabExtSettings.IsSelected == false))
                        {
                            if (tabProjFile.Visibility == Visibility.Visible)
                            {
                                tabProjFile.IsSelected = true;
                            }
                            else
                            {
                                tabExtSettings.IsSelected = true;
                            }
                        }
                    }
                    break;
                case "btnCloseTabProjFile":
                    tabProjFile.Visibility = Visibility.Collapsed;
                    menuViewProjFile.IsChecked = false;
                    if ((tab3D.Visibility == Visibility.Collapsed) &&
                        (tabExtSettings.Visibility == Visibility.Collapsed))
                    {
                        gridMain.IsRightColumnVisible = false;
                    }
                    else
                    {
                        if ((tab3D.IsSelected == false) && (tabExtSettings.IsSelected == false))
                        {
                            if (tab3D.Visibility == Visibility.Visible)
                            {
                                tab3D.IsSelected = true;
                            }
                            else
                            {
                                tabExtSettings.IsSelected = true;
                            }
                        }
                    }
                    break;
                case "btnCloseTabExtSettings":
                    tabExtSettings.Visibility = Visibility.Collapsed;
                    menuViewExtSettings.IsChecked = false;
                    if ((tab3D.Visibility == Visibility.Collapsed) &&
                        (tabProjFile.Visibility == Visibility.Collapsed))
                    {
                        gridMain.IsRightColumnVisible = false;
                    }
                    else
                    {
                        if ((tab3D.IsSelected == false) && (tabProjFile.IsSelected == false))
                        {
                            if (tab3D.Visibility == Visibility.Visible)
                            {
                                tab3D.IsSelected = true;
                            }
                            else
                            {
                                tabProjFile.IsSelected = true;
                            }
                        }
                    }
                    break;
            }
        }

        /// <summary>
        /// Tab closing mechanics: react to closing of main column
        /// </summary>
        private void btnHideMainColumn_Click(object sender, RoutedEventArgs e)
        {
            if (gridMain.IsRightColumnVisible == true)
            {
                if ((tab3D.Visibility == Visibility.Collapsed) &&
                    (tabProjFile.Visibility == Visibility.Collapsed) &&
                    (tabExtSettings.Visibility == Visibility.Collapsed))
                {
                    tab3D.Visibility = Visibility.Visible;
                    tab3D.IsSelected = true;
                    menuView3D.IsChecked = true;
                }
                else
                {
                    if (tab3D.Visibility == Visibility.Visible) menuView3D.IsChecked = true;
                    if (tabProjFile.Visibility == Visibility.Visible) menuViewProjFile.IsChecked = true;
                    if (tabExtSettings.Visibility == Visibility.Visible) menuViewExtSettings.IsChecked = true;
                }
            }
            else
            {
                menuView3D.IsChecked = false;
                menuViewProjFile.IsChecked = false;
                menuViewExtSettings.IsChecked = false;
            }
        }

        /// <summary> 
        /// Event handler for property changes in viewmodel object 
        /// </summary>
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "WWEnergyColumnNumber")
            {
                UpdateWWAtomColumns();
            }
            if (e.PropertyName == "Selected3DTab")
            {
                if (MCViewModel.Selected3DTab == 0) rbt3DSelCrystal.IsChecked = true;
                if (MCViewModel.Selected3DTab == 1) rbt3DSelJumps.IsChecked = true;
                if (MCViewModel.Selected3DTab == 2) rbt3DSelUJumps.IsChecked = true;
                if (MCViewModel.Selected3DTab == 3) rbt3DSelCodes.IsChecked = true;
                if (MCViewModel.Selected3DTab == 4) rbt3DSelInteractions.IsChecked = true;
            }
        }

        /// <summary> 
        /// Update binding for current textbox or datagridcell (submit changes if it has focus) 
        /// </summary>
        private void UpdateCurrentInputBox()
        {
            if (Keyboard.FocusedElement is System.Windows.Controls.TextBox)
            {
                BindingExpression bindex = (Keyboard.FocusedElement as System.Windows.Controls.TextBox).GetBindingExpression(System.Windows.Controls.TextBox.TextProperty);
                if (bindex != null) bindex.UpdateSource();
            }
            if (Keyboard.FocusedElement is System.Windows.Controls.DataGridCell)
            {
                BindingExpression bindex = (Keyboard.FocusedElement as System.Windows.Controls.DataGridCell).GetBindingExpression(System.Windows.Controls.DataGridCell.ContentProperty);
                if (bindex != null) bindex.UpdateSource();
            }
        }

        /// <summary> 
        /// Update the number of columns in dgWWEnergies 
        /// </summary>
        private void UpdateWWAtomColumns()
        {
            if (MCViewModel == null) return;
            int old_colcount = dgWWEnergies.Columns.Count;          // this includes always six text columns
            int new_colcount = MCViewModel.WWEnergyColumnNumber;
            if (new_colcount < 0) return;
            if (old_colcount < 6) return;
            if (old_colcount == new_colcount + 6) return;
            while (old_colcount > new_colcount + 6)                 // delete columns until equal counts
            {
                old_colcount--;
                TextBlock t_Header = dgWWEnergies.Columns[old_colcount].Header as TextBlock;
                if (t_Header != null)
                {
                    BindingOperations.ClearAllBindings(t_Header);   // remove bindings from the header textblock (otherwise binding error from the removed column)
                    t_Header.DataContext = null;                    // (I think the wpf weak binding scheme does not work with the binding proxy workaround) 
                }
                dgWWEnergies.Columns.RemoveAt(old_colcount);
            }
            while (old_colcount < new_colcount + 6)                 // add columns until equal counts
            {
                string XamlDataGridTemplateColumn =
                    @"<DataGridTemplateColumn xmlns='http://schemas.microsoft.com/winfx/2006/xaml/presentation' 
                            xmlns:x='http://schemas.microsoft.com/winfx/2006/xaml' Width='2*' MinWidth='50'>
                        <DataGridTemplateColumn.Header>
                            <TextBlock DataContext='{DynamicResource locBindingProxy}' Style='{StaticResource StdDataGridHeaderTextBlockStyle}' 
                                Text='{Binding Data.Energies.ElemHeaders[" + (old_colcount - 6).ToString() + @"].ElemStr}'>
                                <TextBlock.ToolTip>
                                    <TextBlock Text='Energy contribution in electron volts that is added to the activation energy when the environment position is occupied by the element specified in the column header.' 
                                               Style='{StaticResource StdToolTipStyle}'/>
                                </TextBlock.ToolTip>
                            </TextBlock>
                        </DataGridTemplateColumn.Header>
                        <DataGridTemplateColumn.CellTemplate>
                            <DataTemplate>
                                <ContentPresenter x:Name='stdPresenter' Content='{Binding Energies[" + (old_colcount - 6).ToString() + @"]}' ContentTemplate='{StaticResource EmptyDatagridCellTemplate}'/>
                                <DataTemplate.Triggers>
                                    <DataTrigger Binding='{Binding Energies[" + (old_colcount - 6).ToString() + @"].IsEditable}' Value='True'>
                                        <Setter TargetName='stdPresenter' Property='ContentTemplate' Value='{StaticResource ReadonlyDatagridCellTemplate}'/>
                                    </DataTrigger>
                                </DataTemplate.Triggers>
                            </DataTemplate>
                        </DataGridTemplateColumn.CellTemplate>
                        <DataGridTemplateColumn.CellEditingTemplate>
                            <DataTemplate>
                                <ContentPresenter x:Name='edPresenter' Content='{Binding Energies[" + (old_colcount - 6).ToString() + @"]}' ContentTemplate='{StaticResource EmptyDatagridCellTemplate}'/>
                                <DataTemplate.Triggers>
                                    <DataTrigger Binding='{Binding Energies[" + (old_colcount - 6).ToString() + @"].IsEditable}' Value='True'>
                                        <Setter TargetName='edPresenter' Property='ContentTemplate' Value='{StaticResource EditableDatagridCellTemplate}'/>
                                    </DataTrigger>
                                </DataTemplate.Triggers>
                            </DataTemplate>
                        </DataGridTemplateColumn.CellEditingTemplate>
                    </DataGridTemplateColumn>";
                StringReader strReader = new StringReader(XamlDataGridTemplateColumn);
                XmlReader xmlReader = XmlReader.Create(strReader);
                dgWWEnergies.Columns.Add((DataGridTemplateColumn)XamlReader.Load(xmlReader));
                old_colcount++;
            }
            GC.Collect();
        }

#endregion Critical Methods for App Behaviour

#region Menu-Buttons: Job Management

        /// <summary>
        /// Create new KMC job
        /// </summary>
        private void menuNew_Click(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.New();
                    }

        /// <summary>
        /// Save current KMC job
        /// </summary>
        private void menuSave_Click(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Save();
        }

        /// <summary>
        /// Save current KMC job to new path
        /// </summary>
        private void menuSaveAs_Click(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Save(if_save_as: true);
        }

        /// <summary>
        /// Load KMC job
        /// </summary>
        private void menuLoad_Click(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Load();
        }

#endregion Menu-Buttons: Job Management

#region Menu-Buttons: Additional Methods

        /// <summary>
        /// Show/Hide the console window
        /// </summary>
        private void SwitchConsole(bool i_isvisible)
        {
            if (i_isvisible == true)
            {
                int ErrorCode = ConsoleWrapper.Show();
                if (ErrorCode != Constants.KMCERR_OK)
                    throw new ApplicationException("Showing status window failed (menuView_Click, Code = " + ErrorCode.ToString() + ")");
                menuViewConsole.IsChecked = true;
            }
            else
            {
                int ErrorCode = ConsoleWrapper.Hide();
                if (ErrorCode != Constants.KMCERR_OK)
                    throw new ApplicationException("Hiding status window failed (menuView_Click, Code = " + ErrorCode.ToString() + ")");
                menuViewConsole.IsChecked = false;
            }
        }

        /// <summary>
        /// Show program info
        /// </summary>
        private void menuInfo_Click(object sender, RoutedEventArgs e)
        {
            LicenseWindow t_licwnd = new LicenseWindow();
            t_licwnd.Owner = System.Windows.Application.Current.MainWindow;
            t_licwnd.ShowDialog();
        }

        #endregion Menu-Buttons: Additional Methods

        #region KMC Job Input Handling

        /// <summary>
        /// Execute a delegate (show console before and restore console status afterwards)
        /// </summary>
        /// <param name="del"> Delegate to execute <br/>
        /// - Parameter (out bool): true = keep console visible after execution <br/>
        /// - Return value (bool): meaning depends on delegate
        /// </param>
        /// <returns> Return value of the delegate </returns>
        private bool ExecuteWithConsole(FlagFunc<bool, bool> del)
        {
            UpdateCurrentInputBox();

            // Save current console status
            int old_console_status = ConsoleWrapper.Status();

            // Show console
            if (old_console_status == Constants.KMCERR_NO_CONSOLE)
            {
                SwitchConsole(true);
            }

            bool result = del(out bool keep_console);

            // Restore original console status (if not otherwise requested by cmd)
            if ((keep_console == false) && (old_console_status == Constants.KMCERR_NO_CONSOLE))
            {
                SwitchConsole(false);
                this.Activate();
            }

            return result;
        }

        private void BtnCmd_JobDesc_Apply(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(0, out x));
        }

        private void BtnCmd_JobDesc_Cancel(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.JobDesc.GetData();
        }

        private void BtnCmd_Structure_AddAtom(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.AddAtom();
        }

        private void BtnCmd_Structure_DublicateAtom(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.DublicateAtom();
        }

        private void BtnCmd_Structure_SelectAsMovingSpecies(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.SelectAsMovingSpecies();
        }

        private void BtnCmd_Structure_DelAtom(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.DeleteSelectedAtom();
        }

        private void BtnCmd_Structure_DelAllAtoms(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.DeleteAllAtoms();
        }

        private void BtnCmd_Structure_AddDoping(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.AddDoping();
        }

        private void BtnCmd_Structure_DublicateDoping(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.DublicateDoping();
        }

        private void BtnCmd_Structure_DelDoping(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.DeleteSelectedDoping();
        }

        private void BtnCmd_Structure_DelAllDopings(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.DeleteAllDopings();
        }

        private void BtnCmd_Structure_Apply(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(1, out x));
            v3dCrystal.ResetCamera();
        }

        private void BtnCmd_Structure_Cancel(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Structure.GetData();
        }

        private void BtnCmd_ShellCounts_Apply(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(2, out x));
            v3dJumps.ResetCamera();
            v3dUniqueJumps.ResetCamera();
        }

        private void BtnCmd_ShellCounts_Cancel(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.ShellCounts.GetData();
        }

        private void BtnCmd_UniqueJumps_Apply(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(3, out x));
            v3dJumps.ResetCamera();
            v3dUniqueJumps.ResetCamera();
            v3dCodes.ResetCamera();
            v3dInteractions.ResetCamera();
        }

        private void BtnCmd_UniqueJumps_Cancel(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.UniqueJumps.GetData();
        }

        private void BtnCmd_Energies_Apply(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(4, out x));
        }

        private void BtnCmd_Energies_Cancel(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Energies.GetData();
        }

        private void BtnCmd_Settings_AddJob(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(5, out x));
        }

        private void BtnCmd_Settings_DublicateJob(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Settings.DublicateJob();
        }

        private void BtnCmd_Settings_LoadJob(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Settings.LoadSelectedJob();
        }

        private void BtnCmd_Settings_DeleteJob(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Settings.DeleteSelectedJob();
        }

        private void BtnCmd_Settings_ChangeJob(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(6, out x));
        }

        private void BtnCmd_Settings_ClearJob(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(7, out x));
        }

        private void BtnCmd_Settings_Submit(object sender, RoutedEventArgs e)
        {
            _ = ExecuteWithConsole((out bool x) => MCViewModel.ExecuteLongTask(8, out x));
        }

        private void BtnCmd_Settings_ClearJobList(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.Settings.ClearJobList();
        }

#endregion KMC Job Input Handling

#region Additional Input Handling

        private void BtnCmd_VMGUISettings_SetPrivateKeyPath(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.VMGUISettings.SelectPrivateKeyFile();
        }

        private void BtnCmd_VMGUISettings_AddRemoteProfile(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.VMGUISettings.AddRemoteProfile();
        }

        private void BtnCmd_VMGUISettings_CopyRemoteProfile(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.VMGUISettings.CopyRemoteProfile();
        }

        private void BtnCmd_VMGUISettings_DeleteRemoteProfile(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.VMGUISettings.DeleteRemoteProfile();
        }

        private void BtnCmd_VMGUISettings_LoadDefaultSubmitScript(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();

            MessageBoxResult confirmation = MessageBox.Show("Attention: Resetting the submit script to the default script cannot be undone. Continue?", "Confirmation", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (confirmation == MessageBoxResult.Yes)
            {
                MCViewModel.VMGUISettings.SelectedRemoteProfile?.LoadDefaultSubmitScript();
            }
        }

        private void BtnCmd_VMGUISettings_LoadDefaultJobScript(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();

            MessageBoxResult confirmation = MessageBox.Show("Attention: Resetting the job script to the default script cannot be undone. Continue?", "Confirmation", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (confirmation == MessageBoxResult.Yes)
            {
                MCViewModel.VMGUISettings.SelectedRemoteProfile?.LoadDefaultJobScript();
            }
        }

        private void BtnCmd_VMGUISettings_LoadDefaultBuildScript(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();

            MessageBoxResult confirmation = MessageBox.Show("Attention: Resetting the build script to the default script cannot be undone. Continue?", "Confirmation", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (confirmation == MessageBoxResult.Yes)
            {
                MCViewModel.VMGUISettings.SelectedRemoteProfile?.LoadDefaultBuildScript();
            }
        }

        #endregion Additional Input Handling

        #region 3D Visualization Methods

        private void rbt3DSel_Checked(object sender, RoutedEventArgs e)
        {
            var rbtsender = sender as System.Windows.Controls.RadioButton;
            if ((MCViewModel == null) || (rbtsender == null))
            {
                return;
            }

            if (rbtsender == rbt3DSelCrystal)
            {
                MCViewModel.Selected3DTab = 0;
            }
            else if (rbtsender == rbt3DSelJumps)
            {
                MCViewModel.Selected3DTab = 1;
            }
            else if (rbtsender == rbt3DSelUJumps)
            {
                MCViewModel.Selected3DTab = 2;
            }
            else if (rbtsender == rbt3DSelCodes)
            {
                MCViewModel.Selected3DTab = 3;
            }
            else if (rbtsender == rbt3DSelInteractions)
            {
                MCViewModel.Selected3DTab = 4;
            }
        }

        private void BtnCmd_3DCrystal_Update(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.CrystalModel.UpdateModel();
        }

        private void BtnCmd_3DCrystal_SavePicture(object sender, RoutedEventArgs e)
        {
            MCViewModel.SavePicture(v3dCrystal);
        }

        private void BtnCmd_3DCrystal_ResetCamera(object sender, RoutedEventArgs e)
        {
            v3dCrystal.ResetCamera();
            MCViewModel.CrystalModel.ZoomExtents(v3dCrystal.Camera);
        }

        private void BtnCmd_3DCrystal_ZoomExtents(object sender, RoutedEventArgs e)
        {
            MCViewModel.CrystalModel.ZoomExtents(v3dCrystal.Camera, 100);
        }

        private void BtnCmd_3DJumps_Update(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.JumpsModel.UpdateModel();
        }

        private void BtnCmd_3DJumps_SavePicture(object sender, RoutedEventArgs e)
        {
            MCViewModel.SavePicture(v3dJumps);
        }

        private void BtnCmd_3DJumps_ResetCamera(object sender, RoutedEventArgs e)
        {
            v3dJumps.ResetCamera();
            MCViewModel.JumpsModel.ZoomExtents(v3dJumps.Camera);
        }

        private void BtnCmd_3DJumps_ZoomExtents(object sender, RoutedEventArgs e)
        {
            MCViewModel.JumpsModel.ZoomExtents(v3dJumps.Camera, 100);
        }

        private void BtnCmd_3DUniqueJumps_Update(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.UniqueJumpsModel.UpdateModel();
        }

        private void BtnCmd_3DUniqueJumps_SavePicture(object sender, RoutedEventArgs e)
        {
            MCViewModel.SavePicture(v3dUniqueJumps);
        }

        private void BtnCmd_3DUniqueJumps_ResetCamera(object sender, RoutedEventArgs e)
        {
            v3dUniqueJumps.ResetCamera();
            MCViewModel.UniqueJumpsModel.ZoomExtents(v3dUniqueJumps.Camera);
        }

        private void BtnCmd_3DUniqueJumps_ZoomExtents(object sender, RoutedEventArgs e)
        {
            MCViewModel.UniqueJumpsModel.ZoomExtents(v3dUniqueJumps.Camera, 100);
        }

        private void BtnCmd_3DCodes_Update(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.CodesModel.UpdateModel();
        }

        private void BtnCmd_3DCodes_SavePicture(object sender, RoutedEventArgs e)
        {
            MCViewModel.SavePicture(v3dCodes);
        }

        private void BtnCmd_3DCodes_ResetCamera(object sender, RoutedEventArgs e)
        {
            v3dCodes.ResetCamera();
            MCViewModel.CodesModel.ZoomExtents(v3dCodes.Camera);
        }

        private void BtnCmd_3DCodes_ZoomExtents(object sender, RoutedEventArgs e)
        {
            MCViewModel.CodesModel.ZoomExtents(v3dCodes.Camera, 100);
        }

        private void BtnCmd_3DInteractions_Update(object sender, RoutedEventArgs e)
        {
            UpdateCurrentInputBox();
            MCViewModel.InteractionsModel.UpdateModel();
        }

        private void BtnCmd_3DInteractions_SavePicture(object sender, RoutedEventArgs e)
        {
            MCViewModel.SavePicture(v3dInteractions);
        }

        private void BtnCmd_3DInteractions_ResetCamera(object sender, RoutedEventArgs e)
        {
            v3dInteractions.ResetCamera();
            MCViewModel.InteractionsModel.ZoomExtents(v3dInteractions.Camera);
        }

        private void BtnCmd_3DInteractions_ZoomExtents(object sender, RoutedEventArgs e)
        {
            MCViewModel.InteractionsModel.ZoomExtents(v3dInteractions.Camera, 100);
        }

        private void hv3DCrystal_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.CrystalModel != null)
                {
                    MCViewModel.CrystalModel.SelectAtom(-1);
                    e.Handled = true;
                }
            }
        }

        private void hv3DJumps_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.JumpsModel != null)
                {
                    MCViewModel.JumpsModel.SelectAtom(-1);
                    e.Handled = true;
                }
            }
        }

        private void hv3DUniqueJumps_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.UniqueJumpsModel != null)
                {
                    MCViewModel.UniqueJumpsModel.SelectAtom(-1);
                    e.Handled = true;
                }
            }
        }

        private void hv3DCodes_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.CodesModel != null)
                {
                    MCViewModel.CodesModel.SelectAtom(-1);
                    e.Handled = true;
                }
            }
        }

        private void hv3DInteractions_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.InteractionsModel != null)
                {
                    MCViewModel.InteractionsModel.SelectAtom(-1);
                    e.Handled = true;
                }
            }
        }

#endregion 3D Visualization Methods

#region Ini-File Management

        /// <summary> 
        /// Loads misc data from last session from ini-File 
        /// </summary>
        private void LoadFromIniFile()
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.VMGUISettings != null)
                {
                    MCViewModel.VMGUISettings.LoadFromIniFile();
                }
            }
        }

        /// <summary> 
        /// Saves misc data to ini-File 
        /// </summary>
        private void SaveToIniFile()
        {
            if (MCViewModel != null)
            {
                if (MCViewModel.VMGUISettings != null)
                {
                    MCViewModel.VMGUISettings.SaveToIniFile();
                }
            }
        }

#endregion Ini-File Management

    }
}
