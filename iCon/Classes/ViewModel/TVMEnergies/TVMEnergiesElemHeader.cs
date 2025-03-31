namespace iCon_General
{
    /// <summary> 
    /// Holds data for column headers for the additive energy contributions in the Energies tab 
    /// </summary>
    public class TVMEnergiesElemHeader: ViewModelObject
    {
        #region Fields

        protected readonly TMCViewModel _ViewModel;

        #endregion Fields

        public TVMEnergiesElemHeader(TMCViewModel ViewModel)
        {
            _ViewModel = ViewModel;
            _ElemID = -1;
            _ElemStr = "";
        }

        #region Properties

        public int _ElemID;
        /// <summary> 
        /// Identification number of the element 
        /// </summary>
        public int ElemID
        {
            get
            {
                return _ElemID;
            }
            set
            {
                if (value != _ElemID)
                {
                    _ElemID = value;
                    Notify("ElemID");
                }
            }
        }

        public string _ElemStr;
        /// <summary> 
        /// Element description as single string 
        /// </summary>
        public string ElemStr
        {
            get
            {
                return _ElemStr;
            }
            set
            {
                if (value != _ElemStr)
                {
                    _ElemStr = value;
                    Notify("ElemStr");
                }
            }
        }

        #endregion Properties
    }
}
