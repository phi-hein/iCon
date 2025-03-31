using System;
using System.Globalization;
using System.Windows.Data;

namespace iCon_General
{
    /// <summary>
    /// Converter for binding textbox contents to long-type viewmodel properties
    /// </summary>
    public class StrToLongConverterClass : IValueConverter
    {
        /// <summary>
        /// Convert long to string
        /// </summary>
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return "0";
            if ((value is long) == false) return "0";
            return string.Format("{0:D}", value);
        }

        /// <summary>
        /// Convert string to long
        /// </summary>
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return 0L;
            if ((value is string) == false) return 0L;
            string str_val = (string)value;
            long long_val;
            if (long.TryParse(str_val, out long_val) == true)
            {
                return long_val;
            }
            else
            {
                return 0L;
            }
        }
    }
}
