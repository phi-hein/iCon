namespace iCon_General
{
    /// <summary> 
    /// Auxiliary Return Class for DoWorkEventArgs.Result if Cancel==true 
    /// </summary>
    class BWorkerResultMessage
    {
        #region Member Variables

        /// <summary> Headline of Message </summary>
        public string MessageHeader;

        /// <summary> Message Body </summary>
        public string MessageText;

        /// <summary> MCDLL Error Code </summary>
        public int ErrorCode;

        /// <summary> true = User cancelled operation, false = KMCDLL Error occured </summary>
        public bool IsUserCancelled;

        #endregion Member Variables

        #region Methods

        public BWorkerResultMessage(string Header, string Msg, int Code, bool ByUser)
        {
            MessageHeader = Header;
            MessageText = Msg;
            ErrorCode = Code;
            IsUserCancelled = ByUser;
        }

        #endregion Methods
    }
}
