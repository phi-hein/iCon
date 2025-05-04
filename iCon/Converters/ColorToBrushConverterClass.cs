using System;
using System.Windows.Data;
using System.Windows.Media;

namespace iCon_General
{
    [ValueConversion(typeof(Color), typeof(SolidColorBrush))]
    public class ColorToBrushConverterClass: IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            Color col = (Color)value;
            Color argb_col = Color.FromArgb(col.A, col.R, col.G, col.B);
            return new SolidColorBrush(argb_col);
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            SolidColorBrush brush = (SolidColorBrush)value;
            Color col = Color.FromArgb(brush.Color.A, brush.Color.R, brush.Color.G, brush.Color.B);
            return col;
        }
    }
}
