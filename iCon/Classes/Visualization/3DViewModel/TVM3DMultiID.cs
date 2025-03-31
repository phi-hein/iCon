using System.Collections.ObjectModel;

namespace iCon_General
{
    public class TVM3DMultiID: ViewModelObject
    {
        #region Properties

        protected int _ID;
        /// <summary>
        /// Identification number
        /// </summary>
        public int ID
        {
            get
            {
                return _ID;
            }
            set
            {
                if (value != _ID)
                {
                    _ID = value;
                    Notify("ID");
                }
            }
        }

        protected ObservableCollection<int> _NextIDs;
        /// <summary>
        /// List of possible IDs in the next selection rank
        /// </summary>
        public ObservableCollection<int> NextIDs
        {
            get
            {
                return _NextIDs;
            }
            set
            {
                if (value != _NextIDs)
                {
                    _NextIDs = value;
                    Notify("NextIDs");
                }
            }
        }

        protected ObservableCollection<TVM3DMultiID> _NextMultiIDs;
        /// <summary>
        /// List of possible MultiIDs in the next selection rank
        /// </summary>
        public ObservableCollection<TVM3DMultiID> NextMultiIDs
        {
            get
            {
                return _NextMultiIDs;
            }
            set
            {
                if (value != _NextMultiIDs)
                {
                    _NextMultiIDs = value;
                    Notify("NextMultiIDs");
                }
            }
        }

        #endregion Properties

        public TVM3DMultiID()
        {
            // Property initializations
            _ID = -1;
            _NextIDs = new ObservableCollection<int>();
            _NextMultiIDs = new ObservableCollection<TVM3DMultiID>();
        }
    }
}
