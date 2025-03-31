using System;
using System.Globalization;
using System.Windows.Data;

namespace iCon_General
{
    /// <summary>
    /// Converter for binding textbox contents to double-type viewmodel properties
    /// </summary>
    public class StrToDoubleConverterClass : IValueConverter
    {
        /// <summary>
        /// Convert double to string
        /// </summary>
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return "0.0";
            if ((value is double) == false) return "0.0";
            return string.Format(ConstantsClass.SC_KMC_LONG_DOUBLEFORMAT, value);
        }

        /// <summary>
        /// Convert string to double
        /// </summary>
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return 0.0;
            if ((value is string) == false) return 0.0;
            string str_val = (string)value;
            str_val = str_val.Replace(',', '.');
            double double_val;
            if (double.TryParse(str_val, out double_val) == true)
            {
                return double_val;
            }
            else
            {
                return 0.0;
            }
        }
    }
}
