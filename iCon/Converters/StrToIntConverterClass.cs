using System;
using System.Globalization;
using System.Windows.Data;

namespace iCon_General
{
    /// <summary>
    /// Converter for binding textbox contents to int-type viewmodel properties
    /// </summary>
    public class StrToIntConverterClass : IValueConverter
    {
        /// <summary>
        /// Convert int to string
        /// </summary>
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return "0";
            if ((value is int) == false) return "0";
            return string.Format("{0:D}", value);
        }

        /// <summary>
        /// Convert string to int
        /// </summary>
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return 0;
            if ((value is string) == false) return 0;
            string str_val = (string)value;
            int int_val;
            if (int.TryParse(str_val, out int_val) == true)
            {
                return int_val;
            }
            else
            {
                return 0;
            }
        }
    }
}
