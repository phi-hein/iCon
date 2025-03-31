using System;
using System.Windows.Data;

namespace iCon_General
{
    public class InvertConverterClass: IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (value == null) return true;
            if (!(value is bool)) throw new ArgumentException("InvertConverter: value not a bool", "value");
            return !(bool)value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (value == null) return true;
            if (!(value is bool)) throw new ArgumentException("InvertConverter: value not a bool", "value");
            return !(bool)value;
        }
    }
}
