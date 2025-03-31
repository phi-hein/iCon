using System;
using System.ComponentModel.DataAnnotations;

namespace iCon_General
{
    /// <summary>
    /// Custom range validation attribute for checking int values against included or excluded maximum and minimum values
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false)]
    sealed public class IntRangeAttribute : ValidationAttribute
    {
        #region Internal Variables

        /// <summary>
        /// Internal minimum value
        /// </summary>
        private readonly int _Minimum;

        /// <summary>
        /// Internal maximum value
        /// </summary>
        private readonly int _Maximum;

        /// <summary>
        /// Internal flag: true = value has to be greater or equal to _Minimum, false = value has to be greater than _Minimum
        /// </summary>
        private readonly bool _IsMinIncluded;

        /// <summary>
        /// Internal flag: true = value has to be smaller or equal to _Maximum, false = value has to be smaller than _Maximum
        /// </summary>
        private readonly bool _IsMaxIncluded;

        #endregion Internal Variables

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="Minimum">Minimum value</param>
        /// <param name="IsMinIncluded">true = value has to be greater or equal to Minimum, false = value has to be greater than Minimum</param>
        /// <param name="Maximum">Maximum value</param>
        /// <param name="IsMaxIncluded">true = value has to be smaller or equal to Maximum, false = value has to be smaller than Maximum</param>
        public IntRangeAttribute(int Minimum, bool IsMinIncluded, int Maximum, bool IsMaxIncluded)
        {
            _Minimum = Minimum;
            _IsMinIncluded = IsMinIncluded;
            _Maximum = Maximum;
            _IsMaxIncluded = IsMaxIncluded;
        }

        /// <summary>
        /// Compares value to the range constraints
        /// </summary>
        public override bool IsValid(object value)
        {
            if (value == null) return false;
            if ((value is int) == false) return false;
            int int_val = (int)value;
            if (_IsMinIncluded == true)
            {
                if (int_val < _Minimum) return false;
            }
            else
            {
                if (int_val <= _Minimum) return false;
            }
            if (_IsMaxIncluded == true)
            {
                if (int_val > _Maximum) return false;
            }
            else
            {
                if (int_val >= _Maximum) return false;
            }
            return true;
        }
    }
}
